#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "../helper_methods.h"

class Enemy;



// klasa odpowiadająca za obliczanie steering behaviours
class SteeringBehaviours {
public:
	Enemy* parent;

	void setParent(Enemy* _parent);

	// obliczaja kolejne siły wpływające na Enemy
	sf::Vector2f seek();
	sf::Vector2f wallAvoidance();
	sf::Vector2f obstacleAvoidance();

	// oblicza sume z wszystkich uwzględnianych wektorów
	sf::Vector2f calculate();
};
