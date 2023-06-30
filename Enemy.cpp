﻿#include "Enemy.h"
#include "Matrix.h"
#include <cassert>
#include "TransforNormal.h"
#include "Player.h" 

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

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

	worldTransform_.translation_.x = 7.0f;

	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 15.0f;

	ApproachInitialize();
}

void Enemy::Update() { 
	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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
		// 弾更新
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
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//自キャラのワールド座標を取得
	player_->Player::GetWorldPosition();
	//敵キャラのワールド座標を取得する
	GetWorldPosition();
	//敵キャラ→自キャラの差分ベクトルを求める
	float a;
	a = Player::worldTransform_.translation_.z;
	//ベクトルの正規化
	velocity = Normalize(velocity);
	//ベクトルの長さを、速さに合わせる
	velocity.z = a;


	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し,初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);

}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}