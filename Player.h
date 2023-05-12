#pragma once

#include "WorldTransform.h"
#include "model.h"
#include "Input.h"

///< summary>
/// 自キャラ
///  </summary>
class Player {
public:
	//<summary>
	// 初期化
	//</summary>
	void Initialize(Model* model, uint32_t textureHandle);
	//<param name="model">モデル</param>
	//<param name= "textureHandle">テクスチャハンドル</param>

	//<summary>
	// 更新
	//</summary>
	void Update();

	//<summary>
	// 描画
	//</summary>
	void Draw(ViewProjection viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;

};