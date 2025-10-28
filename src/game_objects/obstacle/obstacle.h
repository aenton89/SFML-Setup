#pragma once
#include <SFML/Graphics.hpp>
#include "../game_object.h"



class Obstacle : public GameObject {
public:
	Obstacle(float _x = 0, float _y = 0, float _radius = 20.f);
};
