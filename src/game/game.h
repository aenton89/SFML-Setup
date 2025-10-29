#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../game_objects/player/player.h"
#include "../game_objects/player/raycast.h"
#include "../game_objects/obstacle/obstacle.h"
#include "../game_objects/enemy/enemy.h"

#define MIN_OBSTACLE_SIZE 20.f
#define MAX_OBSTACLE_SIZE 40.f
#define MAX_OBSTACLES_AMOUNT 12

#define PLAYER_SPEED 200.f

#define MAX_ENEMIES_AMOUNT 10
#define MIN_ENEMY_EDGE_DIST 20.f
#define MAX_ENEMY_EDGE_DIST 100.f



class Game {
public:
	Game();
	void run();

private:
	sf::RenderWindow window;
	sf::Clock clock;

	Player player;
	Raycast raycast;
	std::vector<std::unique_ptr<Obstacle>> obstacles;
	std::vector<std::unique_ptr<Enemy>> enemies;

	void processEvents();
	void update(float deltaTime);
	void render();

	void initObstacles(int count);
	void spawnEnemies(int max_amount);
	void deleteDeadEnemies();
	void keepInsideWindow(GameObject& obj);
	void gameOver();

	void debug();
};
