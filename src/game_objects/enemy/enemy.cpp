#include "enemy.h"



Enemy::Enemy(float _x, float _y, Player* _player): GameObject(_x, _y, ENEMY_SIZE) {
	shape.setRadius(ENEMY_SIZE);
	shape.setOrigin(ENEMY_SIZE, ENEMY_SIZE);
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(_x, _y);

	steering.setParent(this);
	player = _player;
}

void Enemy::updateColliderPosition() {
	collider.position = { shape.getPosition().x, shape.getPosition().y };
}

void Enemy::update(float dt, sf::RenderWindow& window) {
	steering_force = steering.calculate(player->getPosition());
	velocity += steering_force * dt;

	// ograniczenie prędkości
	float speedLength = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	if (speedLength > MAX_ENEMY_SPEED) {
		speedLength = MAX_ENEMY_SPEED / speedLength;
		velocity.x *= speedLength;
		velocity.y *= speedLength;
	}

	shape.move(velocity);
	updateColliderPosition();
}