#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>



// tu sa deklaracje i definicje funkcji pomocniczych - inline rozwiązuje problem wielokrotnego include'pwania

// funkcja pomocnicza do obliczenia odległości między wektorami
inline float distanceVec2D(sf::Vector2f from, sf::Vector2f to) {
	float dx = from.x - to.x;
	float dy = from.y - to.y;
	return std::sqrt(dx * dx + dy * dy);
}

// funkcja pomocnicza do normalizacji wektora 2D
inline sf::Vector2f normalizeVec2D(const sf::Vector2f& v) {
	float len = std::sqrt(v.x * v.x + v.y * v.y);
	if (len != 0)
		return v / len;
	return sf::Vector2f(0.f, 0.f);
}