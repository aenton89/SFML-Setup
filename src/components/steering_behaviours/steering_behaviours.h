#pragma once
// #include "../../game_objects/enemy/enemy.h"
#include <SFML/System/Vector2.hpp>

class Enemy;



class SteeringBehaviours {
public:
	Enemy* parent;

	void setParent(Enemy* _parent);

	// obliczaja kolejne siły wpływające na Enemy
	sf::Vector2f seek(sf::Vector2f target_pos);
	sf::Vector2f wallAvoidance();
	sf::Vector2f obstacleAvoidance();

	// oblicza sume z wszystkich uwzględnianych wektorów
	sf::Vector2f calculate(sf::Vector2f target_pos);
};
