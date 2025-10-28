#include "game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>



Game::Game(): window(sf::VideoMode(1600, 900), "ASSIGNMENT 1: zombie game"), player(PLAYER_SPEED) {
	sf::Vector2u winSize = window.getSize();
	player.setPosition({ winSize.x / 2.f, winSize.y / 2.f });

	std::srand(static_cast<unsigned>(std::time(nullptr)));
	initObstacles(MAX_OBSTACLES_AMOUNT);
	spawnEnemies(MAX_ENEMIES_AMOUNT);
}

void Game::initObstacles(int count) {
	sf::Vector2u winSize = window.getSize();
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	for (int i = 0; i < count; ++i) {
		bool placed = false;
		int attempts = 0;

		// max 10 prób na każdą przeszkodę
		while (!placed && attempts < 10) {
			float x = static_cast<float>(std::rand() % winSize.x);
			float y = static_cast<float>(std::rand() % winSize.y);
			float r = MIN_OBSTACLE_SIZE + static_cast<float>(std::rand() % static_cast<int>(MAX_OBSTACLE_SIZE - MIN_OBSTACLE_SIZE));

			Obstacle newObstacle(x, y, r);

			// sprawdzenie kolizji z graczem i innymi przeszkodami
			if (player.collider.checkCollision(newObstacle.collider)) {
				attempts++;
				continue;
			}

			bool overlap = false;
			for (const auto& existing : obstacles) {
				if (existing.collider.checkCollision(newObstacle.collider)) {
					overlap = true;
					break;
				}
			}

			if (!overlap) {
				obstacles.push_back(newObstacle);
				placed = true;
			} else {
				attempts++;
			}
		}
	}
}

// wrogownie pojawiaja się losowo w zakresie MIN_ENEMY_EDGE_DIST do MAX_ENEMY_EDGE_DIST pikseli od krawędzi ekranu
void Game::spawnEnemies(int max_amount) {
	if (enemies.size() >= max_amount)
		return;

	sf::Vector2u winSize = window.getSize();

	while (enemies.size() < max_amount) {
		// wybierz losową krawędź ekranu
		int edge = std::rand() % 4;
		float x = 0.f, y = 0.f;

		switch (edge) {
			// lewa
			case 0:
				x = MIN_ENEMY_EDGE_DIST + static_cast<float>(std::rand()) / RAND_MAX * (MAX_ENEMY_EDGE_DIST - MIN_ENEMY_EDGE_DIST);
				y = static_cast<float>(std::rand() % winSize.y);
				break;
			// prawa
			case 1:
				x = winSize.x - (MIN_ENEMY_EDGE_DIST + static_cast<float>(std::rand()) / RAND_MAX * (MAX_ENEMY_EDGE_DIST - MIN_ENEMY_EDGE_DIST));
				y = static_cast<float>(std::rand() % winSize.y);
				break;
			// góra
			case 2:
				x = static_cast<float>(std::rand() % winSize.x);
				y = MIN_ENEMY_EDGE_DIST + static_cast<float>(std::rand()) / RAND_MAX * (MAX_ENEMY_EDGE_DIST - MIN_ENEMY_EDGE_DIST);
				break;
			// dół
			case 3:
				x = static_cast<float>(std::rand() % winSize.x);
				y = winSize.y - (MIN_ENEMY_EDGE_DIST + static_cast<float>(std::rand()) / RAND_MAX * (MAX_ENEMY_EDGE_DIST - MIN_ENEMY_EDGE_DIST));
				break;
		}

		Enemy newEnemy(x, y, ENEMY_SPEED);

		// sprawdzenie, że nie koliduje z graczem
		bool overlap = player.collider.checkCollision(newEnemy.collider);
		// ani przeszkodami
		if (!overlap) {
			for (const auto& obs : obstacles) {
				if (obs.collider.checkCollision(newEnemy.collider)) {
					overlap = true;
					break;
				}
			}
		}
		// oraz kolizji z już istniejącymi wrogami
		if (!overlap) {
			for (const auto& enemy : enemies) {
				if (enemy.collider.checkCollision(newEnemy.collider)) {
					overlap = true;
					break;
				}
			}
		}

		if (!overlap)
			enemies.push_back(newEnemy);
	}
}

void Game::deleteDeadEnemies() {
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),[](const Enemy& e) {
		return e.was_hit;
	}), enemies.end());
}

void Game::keepInsideWindow(GameObject& obj) {
	float r = obj.collider.radius;
	sf::Vector2f pos = obj.getPosition();

	// ograniczenie do granic okna
	if (pos.x - r < 0)
		pos.x = r;
	if (pos.x + r > window.getSize().x)
		pos.x = window.getSize().x - r;
	if (pos.y - r < 0)
		pos.y = r;
	if (pos.y + r > window.getSize().y)
		pos.y = window.getSize().y - r;

	obj.setPosition(pos);
}

void Game::run() {
	while (window.isOpen()) {
		processEvents();
		float deltaTime = clock.restart().asSeconds();
		update(deltaTime);
		render();
	}
}

void Game::processEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			window.close();
	}
}

void Game::update(float deltaTime) {
	sf::Vector2f prevPos = player.getPosition();

	// GRACZ: update + kolizje
	player.update(deltaTime, window);
	keepInsideWindow(player);
	// TODO: idk czy tak ma być ogarnięte, potem doczytać - kolizje z przeszkodami
	for (auto& o : obstacles) {
		if (player.collider.checkCollision(o.collider)) {
			// cofnij pozycję
			player.setPosition(prevPos);
			player.collider.position = { prevPos.x, prevPos.y };
			break;
		}
	}

	// WROGOWIE: update + kolizje + spawnowanie
	for (auto& e : enemies) {
		e.update(deltaTime, window);
		keepInsideWindow(e);

		// kolizje z przeszkodami
		for (auto& o : obstacles) {
			if (e.collider.checkCollision(o.collider)) {
				// cofnij pozycję
				e.setPosition(prevPos);
				e.collider.position = { prevPos.x, prevPos.y };
				break;
			}
		}

		// kolizje z graczem
		if (e.collider.checkCollision(player.collider)) {
			// cofnij pozycję
			e.setPosition(prevPos);
			e.collider.position = { prevPos.x, prevPos.y };
			break;
		}
	}
	// usuń + dodaj przeciwników, jeśli gracz ich zabije
	deleteDeadEnemies();
	spawnEnemies(MAX_ENEMIES_AMOUNT);

	// RAYCAST
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2f dir = player.getForwardDirection(window);
		raycast.shoot(player.getPosition(), dir, obstacles, enemies);
	} else {
		raycast.clear();
	}
}

void Game::render() {
	window.clear(sf::Color::Black);

	// rysuj przeszkody
	for (auto& o : obstacles)
		o.draw(window);

	// no i rysuj przeciwników
	for (auto& e : enemies)
		e.draw(window);

	// rysuj gracza i promień
	raycast.draw(window);
	player.draw(window);

	window.display();
}
