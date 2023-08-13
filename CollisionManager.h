#pragma once
#include <list>
#include "Collider.h"

class CollisionManager {
public:
	void colliderlistRegister();

	void colliderlistClear();

private:
	std::list<Collider*> colliders_;

};
