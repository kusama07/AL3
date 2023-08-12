﻿#pragma once
#include "Vector3.h"
#include "model.h"
#include "WorldTransform.h"
#include <cmath>
#include <iostream>
#include "Matrix.h"
#include "Player.h"

class EnemyBullet {
public:
	///< summary>
	/// 初期化
	///</summary>
	/// <param name="position">モデル</param>
	/// <param name="positon">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initialize(Model* model, const Vector3& positon, const Vector3& velocity);

	///< summary>
	/// 更新
	///</summary>
	void Update();

	///< summary>
	/// 描画
	///</summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	//プレイヤー
	Player* player_ = nullptr;
	Vector3 toPlayer;

};
