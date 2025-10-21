#include "enemy.h"



Enemy::Enemy(float x, float y, float speed): GameObject(x, y, ENEMY_SIZE), speed(speed) {
	shape.setRadius(ENEMY_SIZE);
	shape.setOrigin(ENEMY_SIZE, ENEMY_SIZE);
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(x, y);
}

// TODO: tu steering behaviour
void Enemy::steeringBehavior() {
	// w sumie tu po prostu wywołania innych funkcji
	// jak wander, seek itd.
	return;
}

void Enemy::updateColliderPosition() {
	collider.position = { shape.getPosition().x, shape.getPosition().y };
}

void Enemy::update(float dt, sf::RenderWindow& window) {
	// steeringBehavior();
	updateColliderPosition();
}