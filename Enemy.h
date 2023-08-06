#pragma once
#include "WorldTransform.h"
#include "model.h"
#include "EnemyBullet.h"
#include "Matrix.h"
#include "Player.h" 
#include <list>

class Player;

class GameScene;

class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

	void Update();

	void Draw(ViewProjection viewProjection);

	void ApproachPhase();

	void LeavePhase();

	void Fire();

	enum class Phase {
		Start,
		Approach,
		Leave,
	};

	static const int kFireInterval = 60;

	void ApproachInitialize();

	~Enemy();

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	void OnCollision();


	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	
	bool IsDead() const { return isDead_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	Phase phase_ = Phase::Start;

	EnemyBullet* bullet_ = nullptr;

	//発射タイマー
	int32_t fireTimer = 0;

	//自キャラ
	Player* player_ = nullptr;

	// ワールド座標を入れる変数
	Vector3 worldPos;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	bool isDead_ = false;
};
