#pragma once
#include <SFML/Graphics.hpp>
#include "../collision/collider.h"



class GameObject {
public:
	Collider collider;

	GameObject(float x = 0, float y = 0, float r = 10);
	virtual ~GameObject() = default;

	virtual void update(float dt) {}
	virtual void draw(sf::RenderWindow& window) = 0;

	sf::Vector2f getPosition() const;
	virtual void setPosition(const sf::Vector2f& pos);
};
