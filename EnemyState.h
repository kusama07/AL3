#pragma once
#include "Enemy.h"
#include "Vector3.h"

class Enemy;

class EnemyState {
public:
	virtual ~EnemyState(){};

	virtual void Update(Enemy * enemy) = 0;
};

class EnemyApproach : public EnemyState
{
public:
	void Update(Enemy* enemy);

	const Vector3 kCharacterApproachSpeed = {0.0f, 0.0f, -0.2f};
};

class EnemyLeave : public EnemyState
{
public:
	void Update(Enemy* enemy);

	const Vector3 kCharacterLeveaveSpeed = {-0.3f, 0.3f, 0.0f};
};
