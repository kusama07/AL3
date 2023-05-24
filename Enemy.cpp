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

void Enemy::Update() { 
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.05f;

	worldTransform_.TransferMatrix(); 

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
		break;
	case Phase::Leave:
		LeavePhase();
		break;
	}
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
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
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