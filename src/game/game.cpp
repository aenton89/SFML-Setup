#include "game.h"
#include <cstdlib>
#include <ctime>



Game::Game(): window(sf::VideoMode(1600, 900), "ASSIGNMENT 1: zombie game"), player(200.f) {
	sf::Vector2u winSize = window.getSize();
	player.setPosition({ winSize.x / 2.f, winSize.y / 2.f });

	std::srand(static_cast<unsigned>(std::time(nullptr)));
	initObstacles(12);
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
			float r = 20.f + static_cast<float>(std::rand() % 20);

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

	player.handleInput(deltaTime);
	player.updateRotation(window);
	keepInsideWindow(player);

	// aktualizuj pozycję collidera
	player.collider.position = { player.getPosition().x, player.getPosition().y };

	// TODO: idk czy tak ma być ogarnięte, potem doczytać - kolizje z przeszkodami
	for (auto& o : obstacles) {
		if (player.collider.checkCollision(o.collider)) {
			// cofnij pozycję
			player.setPosition(prevPos);
			player.collider.position = { prevPos.x, prevPos.y };
			break;
		}
	}

	// raycast tylko gdy przycisk wciśnięty
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2f dir = player.getForwardDirection(window);
		raycast.shoot(player.getPosition(), dir, obstacles);
	} else {
		raycast.clear();
	}

}

void Game::render() {
	window.clear(sf::Color::Black);

	// rysuj przeszkody
	for (auto& o : obstacles)
		o.draw(window);

	// rysuj gracza i promień
	raycast.draw(window);
	player.draw(window);

	window.display();
}
