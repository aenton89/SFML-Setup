#pragma once
#include "../game_object.h"
#include "../../components/steering_behaviours/steering_behaviours.h"
#include "../player/player.h"

#define ENEMY_SIZE 15.f
// na długość wektora prędkości
#define MAX_ENEMY_SPEED 200.f



class Enemy : public GameObject {
public:
	Player* player;
	sf::Vector2f velocity;
	// zakładamy, że masa jest jednostkowa, więc jest to nasze przyśpieszenie
	sf::Vector2f steering_force;
	bool was_hit = false;
	// komponent zachowań sterujących - do konstruktora przyjmuje this
	SteeringBehaviours steering;

	Enemy(float _x, float _y, Player* _player);

	void updateColliderPosition();
	void steeringBehavior();

	void update(float dt, sf::RenderWindow& window) override;
};
