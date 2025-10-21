#pragma once
#include <SFML/Graphics.hpp>
#include "../game_object.h"

#define PLAYER_SIZE 15.f



class Player : public GameObject {
public:
	float speed;

	Player(float speed = 200.f);

	void handleInput(float deltaTime);
	void updateRotation(sf::RenderWindow& window);
	void updateColliderPosition();

	void update(float dt, sf::RenderWindow& window) override;

	sf::Vector2f getForwardDirection(sf::RenderWindow& window) const;
};
