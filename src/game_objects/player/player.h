#pragma once
#include <SFML/Graphics.hpp>
#include "../game_object.h"



class Player : public GameObject {
public:
	Player(float speed = 200.f);

	void handleInput(float deltaTime);
	void updateRotation(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

	void setPosition(const sf::Vector2f& pos) override;
	sf::Vector2f getForwardDirection(sf::RenderWindow& window) const;

private:
	sf::CircleShape shape;
	float speed;
};
