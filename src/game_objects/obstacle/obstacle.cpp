#include "obstacle.h"



Obstacle::Obstacle(float x, float y, float radius): GameObject(x, y, radius) {
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
	shape.setFillColor(sf::Color(100, 100, 100));
	shape.setPosition(x, y);
}