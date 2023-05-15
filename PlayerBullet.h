#pragma once
#include "Vector3.h"
#include "model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	///<summary>
	///初期化
	///</summary>
	/// <param name="position">モデル</param>
	/// <param name="positon">初期座標</param>
	void Initialize(Model* model, const Vector3& positon);

	///<summary>
	///更新
	///</summary>
	void Update();

	///< summary>
	/// 描画
	///</summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
