#include "game_object.h"



GameObject::GameObject(float x, float y, float r): collider(x, y, r) {}

void GameObject::setPosition(const sf::Vector2f& pos) {
	collider.position = {pos.x, pos.y};
	shape.setPosition(pos);
}

sf::Vector2f GameObject::getPosition() const {
	return {collider.position.x, collider.position.y};
}

void GameObject::draw(sf::RenderWindow& window) {
	window.draw(shape);
}