#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../game_objects/player/player.h"
#include "../game_objects/player/raycast.h"
#include "../game_objects/obstacle/obstacle.h"



class Game {
public:
	Game();
	void run();

private:
	sf::RenderWindow window;
	sf::Clock clock;

	Player player;
	Raycast raycast;
	std::vector<Obstacle> obstacles;

	void processEvents();
	void update(float deltaTime);
	void render();

	void initObstacles(int count);
	void keepInsideWindow(GameObject& obj);
};
