#include "Enemy.h"
#include "Matrix.h"
#include <cassert>
#include "TransforNormal.h"
#include "EnemyBullet.h"

Vector3 Enemy::GetWorldPosition() {

	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Enemy::~Enemy() { 
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_.x = 9.0f;
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 70.0f;

	ApproachInitialize();
}

void Enemy::Update() { 
	worldTransform_.TransferMatrix();

	Vector3 move = {0, 0, 0};

	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachPhase();
		for (EnemyBullet* bullet : bullets_) {
			bullet->Update();
		}

		break;

	case Phase::Leave:
		LeavePhase();
		break;
	}
}

void Enemy::ApproachInitialize() {
	//発射タイマーを初期化
	fireTimer = 0;
}

void Enemy::ApproachPhase() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	const float kApproachCharacterSpeed = 0.05f;
	move.z -= kApproachCharacterSpeed;

	// 移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 既定の位置に到達したら離脱
	/*if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}*/

	//発射タイマーをデクリメント
	fireTimer--;
	//指定時間に達した
	if (fireTimer < 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}
}

void Enemy::LeavePhase() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kLeaveCharacterSpeed = 0.05f;
	move.x -= kLeaveCharacterSpeed;
	move.y += kLeaveCharacterSpeed;
	// 移動（ベクトルを加算）
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標を取得
	player_->GetWorldPosition();

	Vector3 velocity{0, 0, kBulletSpeed};

	EnemyBullet* newBullet = new EnemyBullet();

	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	newBullet->SetPlayer(player_);

	bullets_.push_back(newBullet);
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}