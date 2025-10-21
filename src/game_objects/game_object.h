#pragma once
#include <SFML/Graphics.hpp>
#include "../collision/collider.h"



class GameObject {
public:
	Collider collider;
	sf::CircleShape shape;

	GameObject(float x = 0, float y = 0, float r = 10);
	virtual ~GameObject() = default;

	virtual void update(float dt, sf::RenderWindow& window) {};
	void draw(sf::RenderWindow& window);

	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& pos);
};
