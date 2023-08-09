#include "Matrix.h"
#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& positon, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = positon;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//Z方向に延びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	//y軸回り角度
	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);

	//Y軸回りに-Θy回す回転行列を計算
	//velocity_二回転行列を掛け算してvelocityZを求める
	float VelocityZ = std::sqrt((velocity.x * velocity.x) + (velocity.z * velocity.z));

	//X軸回り角度(Θx)
	worldTransform_.rotation_.x = std::atan2(-velocity.y, VelocityZ);

}

void EnemyBullet::Update() { 
	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}