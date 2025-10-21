#pragma once
#include <SFML/Graphics.hpp>
#include "../game_object.h"



class Obstacle : public GameObject {
public:
	Obstacle(float x = 0, float y = 0, float radius = 20.f);
};
