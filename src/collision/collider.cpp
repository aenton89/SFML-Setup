#include "collider.h"



Collider::Collider(float x, float y, float r): position(x, y), radius(r) {}

bool Collider::checkCollision(const Collider& other) const {
	return position.distance(other.position) < (radius + other.radius);
}
