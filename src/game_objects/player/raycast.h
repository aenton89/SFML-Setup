#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "../../components/collision/collider.h"
#include "../obstacle/obstacle.h"
#include "../enemy/enemy.h"



// struktura i metoda pomocnicza
struct HitInfo {
	sf::Vector2f point;
	float distance;
};

std::optional<HitInfo> rayCircleIntersect(const sf::Vector2f& origin, const sf::Vector2f& dir, const Collider& circle);



class Raycast {
public:
	Raycast();

	void shoot(const sf::Vector2f& origin, const sf::Vector2f& direction, const std::vector<std::unique_ptr<Obstacle>>& obstacles, std::vector<std::unique_ptr<Enemy>>& enemies);
	void draw(sf::RenderWindow& window);
	void clear();

	bool isActive() const;

private:
	sf::VertexArray line;
	bool active;
};
