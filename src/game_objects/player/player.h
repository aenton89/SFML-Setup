#pragma once
#include <SFML/Graphics.hpp>
#include "../game_object.h"
// #include "../../game/game.h"

class Game;



class Player : public GameObject {
public:
	Game* game;
	float speed;

	Player(float _speed = 200.f, Game* _game = nullptr);

	void handleInput(float deltaTime);
	void updateRotation(sf::RenderWindow& window);
	void updateColliderPosition();

	void update(float dt, sf::RenderWindow& window) override;

	sf::Vector2f getForwardDirection(sf::RenderWindow& window) const;
};
