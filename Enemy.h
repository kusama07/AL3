#pragma once
#include "WorldTransform.h"
#include "model.h"
#include "EnemyBullet.h"
#include "Matrix.h"
#include <list>

class Player;

class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection viewProjection);

	void ApproachPhase();

	void LeavePhase();

	void Fire();

	enum class Phase {
		Approach,
		Leave,
	};

	static const int kFireInterval = 60;

	void ApproachInitialize();

	~Enemy();

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	Phase phase_ = Phase::Approach;

	EnemyBullet* bullet_ = nullptr;

	// 弾
	std::list<EnemyBullet*> bullets_;

	//発射タイマー
	int32_t fireTimer = 0;

	//自キャラ
	Player* player_ = nullptr;

};
