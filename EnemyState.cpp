#include "EnemyState.h"

void EnemyApproach::Update(Enemy* enemy) { 
	enemy->Move(kCharacterApproachSpeed);

		if (enemy->GetTranslation().z < 0.0f)
		{
			enemy->ChangePhase(new EnemyLeave());
		}
}

void EnemyLeave::Update(Enemy* enemy) { 
	enemy->Move(kCharacterLeveaveSpeed); 
}
