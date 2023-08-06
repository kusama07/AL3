#pragma once

#include "WorldTransform.h"
#include "model.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include <list>

///< summary>
/// 自キャラ
///  </summary>
class Player {

public:
	//<summary>
	// 初期化
	//</summary>
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& playerPosition);
	//<param name="model">モデル</param>
	//<param name= "textureHandle">テクスチャハンドル</param>

	//<summary>
	// 更新
	//</summary>
	void Update(ViewProjection viewProjection);

	void Rotate();

	//<summary>
	// 描画
	//</summary>
	void Draw(ViewProjection viewProjection);

	//<summary>
	// 攻撃
	//</summary>
	void Attack();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* parent);

	void GetMouse(ViewProjection viewProjection);

	void DrawUI();

private:
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;

	WorldTransform worldTransform3DReticle_;

	Sprite* sprite2DReticle_ = nullptr;

	Vector2 ReticlePos_ = {640,320};
	Vector3 positionReticle_;

	XINPUT_STATE joyState_;

	//弾
	std::list<PlayerBullet*> bullets_;
};