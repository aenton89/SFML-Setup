#include "steering_behaviours.h"
#include "../../game/game.h"
#include "../../game_objects/enemy/enemy.h"
#include "../../game_objects/obstacle/obstacle.h"
#include "../../helpers/parameters.h"



SteeringBehaviours::SteeringBehaviours() {
    // startowa prędkość dla Wander
    wanderTarget = sf::Vector2f(WANDER_RADIUS, 0.f);
}

void SteeringBehaviours::setParent(Enemy *_parent) {
	parent = _parent;
}

sf::Vector2f SteeringBehaviours::calculate() {
	if (!parent)
		return sf::Vector2f(0, 0);

	return wander() + obstacleAvoidance(parent->player->game->obstacles, parent->player->game->enemies) + wallAvoidance();
}

// --- seek ---

sf::Vector2f SteeringBehaviours::seek() {
	sf::Vector2f desired_vel = normalizeVec2D(parent->player->getPosition() - parent->getPosition()) * MAX_ENEMY_SPEED;
	return (desired_vel - parent->velocity);
}

// --- wander ---

sf::Vector2f SteeringBehaviours::wander() {
    wanderTarget += sf::Vector2f(randomClamped() * WANDER_JITTER, randomClamped() * WANDER_JITTER);
    wanderTarget = normalizeVec2D(wanderTarget);
    wanderTarget = sf::Vector2f(wanderTarget.x * WANDER_RADIUS, wanderTarget.y * WANDER_RADIUS);

    sf::Vector2f targetLocal = wanderTarget + sf::Vector2f(WANDER_DISTANCE, 0.f);
    sf::Vector2f targetWorld = pointToWorldSpace(targetLocal, parent->heading, parent->side, parent->getPosition());

    // DEBUG:
    // std::cout << (targetWorld - parent->getPosition()).x << " | " << (targetWorld - parent->getPosition()).y << "\n";

    return targetWorld - parent->getPosition();
}

// --- wall avoidance ---

sf::Vector2f SteeringBehaviours::wallAvoidance() {
    createFeelers();
    sf::Vector2f steeringForce(0.f, 0.f);

    // Uproszczony, dla 4-ech ścian = brzegów ekranu
    for (const auto& feeler : feelers) {
        if (feeler.x < 0) {
            float overshoot = -feeler.x;
            steeringForce += sf::Vector2f(1.f, 0.f) * overshoot;
        }
        else if (feeler.x > 1600.f) {
            float overshoot = feeler.x - 1600.f;
            steeringForce += sf::Vector2f(-1.f, 0.f) * overshoot;
        }

        if (feeler.y < 0) {
            float overshoot = -feeler.y;
            steeringForce += sf::Vector2f(0.f, 1.f) * overshoot;
        }
        else if (feeler.y > 900.f) {
            float overshoot = feeler.y - 900.f;
            steeringForce += sf::Vector2f(0.f, -1.f) * overshoot;
        }
    }
    return steeringForce * 50.f;
}

void SteeringBehaviours::createFeelers(){
    feelers.clear();

    sf::Vector2f pos = parent->getPosition();
    sf::Vector2f heading = parent->heading;
    float feelerLength = 80.f;

    feelers.push_back(pos + heading * feelerLength);

    auto rotate = [](const sf::Vector2f& v, float degrees) {
        float rad = degrees * 3.1415926f / 180.0f;
        float cs = std::cos(rad);
        float sn = std::sin(rad);
        return sf::Vector2f(v.x * cs - v.y * sn, v.x * sn + v.y * cs);
    };
    sf::Vector2f leftFeeler  = pos + rotate(heading, 45.0f)  * feelerLength * 0.5f;
    sf::Vector2f rightFeeler = pos + rotate(heading, -45.0f) * feelerLength * 0.5f;

    feelers.push_back(leftFeeler);
    feelers.push_back(rightFeeler);
}

// --- obstacle avoidance ---

sf::Vector2f SteeringBehaviours::obstacleAvoidance(const std::vector<std::unique_ptr<Obstacle>>& obstacles, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!parent)
        return sf::Vector2f(0, 0);

    // przygotuj wektor wszystkich obiektów do sprawdzenia
    std::vector<GameObject*> allObjects;

    // dodaj obstacles
    for (const auto& obstacle : obstacles) {
        allObjects.push_back(obstacle.get());
    }

    // dodaj enemies (pomijając siebie - findIntersections to też sprawdzi, ale można wcześniej)
    for (const auto& enemy : enemies) {
        allObjects.push_back(enemy.get());
    }

    // wywołaj findIntersections
    GameObject* closestObject = nullptr;
    double distToClosestIP = 0.0;
    sf::Vector2f localPosOfClosest;
    float dBoxLength = 0.0f;
    sf::Vector2f heading, side;

    findIntersections(allObjects, closestObject, distToClosestIP, localPosOfClosest, dBoxLength, heading, side);

    // jeśli znaleziono obiekt, oblicz siłę sterującą
    sf::Vector2f steeringForce(0, 0);

    if (closestObject) {
        // im bliżej obiektu, tym silniejsza siła
        float multiplier = 1.0f + (dBoxLength - distToClosestIP) / dBoxLength;

        // siła w osi bocznej (lateral force)
        steeringForce.y = (closestObject->collider.radius - localPosOfClosest.y) * multiplier;

        // siła hamująca
        const float brakingWeight = 0.2f;
        steeringForce.x = (closestObject->collider.radius - localPosOfClosest.x) * brakingWeight;

        // przekształć do world space
        steeringForce = vectorToWorldSpace(steeringForce, heading, side);
    }

    return steeringForce;
}

void SteeringBehaviours::findIntersections(const std::vector<GameObject*>& objects, GameObject*& closestObject, double& distToClosestIP, sf::Vector2f& localPosOfClosest, float& dBoxLength, sf::Vector2f& heading, sf::Vector2f& side) {
    if (!parent)
        return;

    // długość detection box proporcjonalna do prędkości
    float speed = std::sqrt(parent->velocity.x * parent->velocity.x + parent->velocity.y * parent->velocity.y);
    dBoxLength = MIN_DETECTION_BOX_LENGTH + (speed / MAX_ENEMY_SPEED) * MIN_DETECTION_BOX_LENGTH;

    // zresetuj wyniki
    closestObject = nullptr;
    distToClosestIP = std::numeric_limits<double>::max();
    localPosOfClosest = sf::Vector2f(0, 0);

    // oblicz heading i side dla parent
    heading = normalizeVec2D(parent->velocity);
    side = sf::Vector2f(-heading.y, heading.x);

    for (auto* object : objects) {
        // pomiń siebie (jeśli object jest Enemy)
        if (object == parent)
            continue;

        float distToObject = distanceVec2D(parent->getPosition(), object->getPosition());

        if (distToObject < dBoxLength + object->collider.radius) {
            sf::Vector2f localPos = pointToLocalSpace(
                object->getPosition(),
                heading,
                side,
                parent->getPosition()
            );

            if (localPos.x >= 0) {
                float expandedRadius = object->collider.radius + parent->collider.radius;

                if (std::abs(localPos.y) < expandedRadius) {
                    double cX = localPos.x;
                    double cY = localPos.y;

                    double sqrtPart = std::sqrt(expandedRadius * expandedRadius - cY * cY);
                    double ip = cX - sqrtPart;

                    if (ip <= 0)
                        ip = cX + sqrtPart;

                    if (ip < distToClosestIP) {
                        distToClosestIP = ip;
                        closestObject = object;
                        localPosOfClosest = localPos;
                    }
                }
            }
        }
    }
}

// TODO: do debugowania dla obstacle avoidance
void SteeringBehaviours::debugDrawIntersections(sf::RenderWindow& window, const sf::Vector2f& heading, float dBoxLength, GameObject* closestObstacle, double distToClosestIP) {
    if (!parent)
        return;

    // rysuj detection box
    sf::RectangleShape detectionBox;
    detectionBox.setSize(sf::Vector2f(dBoxLength, parent->collider.radius * 2));
    detectionBox.setOrigin(0, parent->collider.radius);
    detectionBox.setPosition(parent->getPosition());

    float angle = std::atan2(heading.y, heading.x) * 180.0f / 3.14159f;
    detectionBox.setRotation(angle);
    detectionBox.setFillColor(sf::Color(255, 255, 0, 50));
    detectionBox.setOutlineColor(sf::Color::Yellow);
    detectionBox.setOutlineThickness(1.0f);
    window.draw(detectionBox);

    // rysuj linię i markery do najbliższej przeszkody
    if (closestObstacle) {
        // linia do przeszkody
        sf::Vertex line[] = {
            sf::Vertex(parent->getPosition(), sf::Color::Red),
            sf::Vertex(closestObstacle->getPosition(), sf::Color::Red)
        };
        window.draw(line, 2, sf::Lines);

        // punkt przecięcia
        sf::Vector2f intersectionPoint = parent->getPosition() + heading * static_cast<float>(distToClosestIP);
        sf::CircleShape intersectionMarker(5.0f);
        intersectionMarker.setOrigin(5.0f, 5.0f);
        intersectionMarker.setPosition(intersectionPoint);
        intersectionMarker.setFillColor(sf::Color::Magenta);
        window.draw(intersectionMarker);

        // podświetlenie przeszkody
        sf::CircleShape highlight(closestObstacle->collider.radius);
        highlight.setOrigin(closestObstacle->collider.radius, closestObstacle->collider.radius);
        highlight.setPosition(closestObstacle->getPosition());
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Red);
        highlight.setOutlineThickness(2.0f);
        window.draw(highlight);
    }
}