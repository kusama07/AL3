#include "Enemy.h"
#include "Matrix.h"
#include <cassert>

Enemy::~Enemy() { 
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 10.0f;
}

void (Enemy::*Enemy::spFuncTable[])() = {

    &Enemy::ApproachPhase,
    &Enemy::LeavePhase
};

void Enemy::Update() { 
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.05f;

	worldTransform_.TransferMatrix(); 

	move.z -= kCharacterSpeed;

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	static_cast<size_t>(phase_);

	(this->*spFuncTable[0])();

	if (worldTransform_.translation_.z < 0.0f) {
		(this->*spFuncTable[1])();
	}

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//デバッグ用表示
	printf("pos%f", worldTransform_.translation_.x);
	
}


void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void Enemy::ApproachPhase() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	const float kApproachCharacterSpeed = 0.05f;
	move.z -= kApproachCharacterSpeed;

	// 移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
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