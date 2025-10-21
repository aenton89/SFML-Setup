#pragma once
#include "../game_object.h"

#define ENEMY_SIZE 15.f



class Enemy : public GameObject {
public:
	float speed;
	bool was_hit = false;

	Enemy(float x, float y, float speed = 50.f);

	void updateColliderPosition();
	void steeringBehavior();

	void update(float dt, sf::RenderWindow& window) override;
};
