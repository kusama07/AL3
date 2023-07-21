#pragma once
#include "WorldTransform.h"
#include "model.h"

class Skydome {
public:
	void Initialize(Model* model, uint32_t skydomeTextureHandle);

	void Update();

	void Draw(ViewProjection viewProjection);
	
private:
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t skydomeTextureHandle_ = 0u;
	WorldTransform worldTransform_;

};