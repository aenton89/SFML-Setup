#include "steering_behaviours.h"
#include "../../game_objects/enemy/enemy.h"



void SteeringBehaviours::setParent(Enemy *_parent) {
	parent = _parent;
}

sf::Vector2f SteeringBehaviours::calculate(sf::Vector2f target_pos) {
	if (!parent)
		return sf::Vector2f(0, 0);

	return seek(target_pos) + obstacleAvoidance() + wallAvoidance();
}

// Vector2D DesiredVelocity = Vec2DNormalize(TargetPos - m_pVehicle->Pos()) * m_pVehicle->MaxSpeed();
// return (DesiredVelocity - m_pVehicle->Velocity());
sf::Vector2f SteeringBehaviours::seek(sf::Vector2f target_pos) {
	sf::Vector2f desired_vel = (target_pos) * MAX_ENEMY_SPEED;
	return (desired_vel - parent->velocity);
}

sf::Vector2f SteeringBehaviours::wallAvoidance() {
	return sf::Vector2f(0, 0);
}

sf::Vector2f SteeringBehaviours::obstacleAvoidance() {
	return sf::Vector2f(0, 0);
}