#pragma once
#include <cmath>
#include <memory>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include "../../helpers/helper_methods.h"

class GameObject;
class Enemy;
class Obstacle;



// klasa odpowiadająca za obliczanie steering behaviours
class SteeringBehaviours {
public:
	Enemy* parent;
	// potrzebne pod wall avoidance
	std::vector<sf::Vector2f> feelers;
	sf::Vector2f wanderTarget;

	SteeringBehaviours();

	void setParent(Enemy* _parent);
	// funkcja tworząca feelery do wall avoidance
	void createFeelers();
	// znajduje najbliższy obiekt na kolizji i zwraca wyniki przez referencje
	void findIntersections(const std::vector<GameObject*>& objects, GameObject*& closestObject, double& distToClosestIP, sf::Vector2f& localPosOfClosest, float& dBoxLength, sf::Vector2f& heading, sf::Vector2f& side);
	// DEBUG: wizualizacja detection box i przecięć
	void debugDrawIntersections(sf::RenderWindow& window, const sf::Vector2f& heading, float dBoxLength, GameObject* closestObstacle, double distToClosestIP);

	// obliczaja kolejne siły wpływające na Enemy
	sf::Vector2f seek();
	sf::Vector2f wander();
	sf::Vector2f wallAvoidance();
	sf::Vector2f obstacleAvoidance(const std::vector<std::unique_ptr<Obstacle>>& obstacles, const std::vector<std::unique_ptr<Enemy>>& enemies);
	// oblicza sume z wszystkich uwzględnianych wektorów
	sf::Vector2f calculate();
};
