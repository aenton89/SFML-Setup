#include "collider.h"



Collider::Collider(float _x, float _y, float _r): position(_x, _y), radius(_r) {}

bool Collider::checkCollision(const Collider& other) const {
	return position.distance(other.position) < (radius + other.radius);
}
