#pragma once
#include <cmath>



struct Vec2 {
	float x, y;

	Vec2() : x(0), y(0) {}
	Vec2(float x, float y) : x(x), y(y) {}

	float distance(const Vec2& other) const {
		float dx = x - other.x;
		float dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}
};

class Collider {
public:
	Vec2 position;
	float radius;

	Collider(float x = 0, float y = 0, float r = 10);

	bool checkCollision(const Collider& other) const;
};
