#pragma once
#include "WorldTransform.h"
#include "model.h"
#include "EnemyState.h"
#include "Vector3.h"

class EnemyState;

class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection viewProjection);

	void Move(Vector3 speed);

	void ChangePhase(EnemyState* newState);

	Vector3 GetTranslation() { return worldTransform_.translation_; };

	~Enemy();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	EnemyState* phase_ = nullptr;
};
