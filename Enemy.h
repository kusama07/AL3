#pragma once
#include "WorldTransform.h"
#include "model.h"

class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection viewProjection);

	void ApproachPhase();

	void LeavePhase();

	enum class Phase {
		Approach,
		Leave,
	};

	~Enemy();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	Phase phase_ = Phase::Approach;

	//メンバ関数ポインタのテーブル
	static void (Enemy::*spFuncTable[])();

};
