#include "Enemy.h"
#include "Matrix.h"
#include <cassert>

Enemy::~Enemy() { 
	delete phase_;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	phase_ = new EnemyApproach();

	worldTransform_.Initialize();

	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 10.0f;



}

void Enemy::Update() { 
	phase_->Update(this);

	worldTransform_.UpdateMatrix();

}

void Enemy::ChangePhase(EnemyState* newState) {
	delete phase_;
	phase_ = newState;
}

void Enemy::Move(Vector3 speed) { worldTransform_.translation_ += speed; };

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}