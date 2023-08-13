#pragma once
#include <list>
#include "Collider.h"

class CollisionManager {
public:
	void CheckAllCollisions();

	void RegisterCollider(Collider* collider) { colliders_.push_back(collider); }

	void ClearCollider() { colliders_.clear(); }

private:
	std::list<Collider*> colliders_;

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};
