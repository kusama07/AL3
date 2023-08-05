#include "Enemy.h"
#include "Matrix.h"
#include <cassert>
#include "TransforNormal.h"
#include "EnemyBullet.h"
#include "GameScene.h"

Vector3 Enemy::GetWorldPosition() {

	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

Enemy::~Enemy() { 

}

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 position) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_.x = position.x;
	worldTransform_.translation_.y = position.y;
	worldTransform_.translation_.z = position.z;

	ApproachInitialize();

	isDead_ = false;
}

void Enemy::Update() { 

	worldTransform_.TransferMatrix(); 
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.05f;


	move.z -= kCharacterSpeed;

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	switch (phase_) { 
	case Phase::Approach:
	default:
		ApproachPhase();
		// 弾更新
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

	fireTimer--;
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
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}

void Enemy::Fire() {
	assert(player_);
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//自キャラのワールド座標を取得
	Vector3 playerWorldPos = player_->GetWorldPosition();
	// 敵のワールド座標を取得
	Vector3 enemyWorldPos = GetWorldPosition();
	// 敵キャラから自キャラへの差分ベクトルを求める
	Vector3 diffVector = Subtract(playerWorldPos, enemyWorldPos);

	diffVector = Normalize(diffVector);

	// ベクトルの長さを速さに合わせる
	velocity = VectorScale(diffVector, kBulletSpeed);

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);

	
}

void Enemy::Draw(ViewProjection viewProjection) {
	if (isDead_ == false) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
}

void Enemy::OnCollision() { isDead_ = true; }