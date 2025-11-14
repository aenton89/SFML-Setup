#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "../../helpers/helper_methods.h"



class Collider {
public:
	sf::Vector2f position;
	float radius;

	Collider(float _x = 0, float _y = 0, float _r = 10);

	bool checkCollision(const Collider& other) const;
};
