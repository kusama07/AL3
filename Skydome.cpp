#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model, uint32_t skydomeTextureHandle) {
	assert(model);

	model_ = model;
	skydomeTextureHandle_ = skydomeTextureHandle;
	worldTransform_.Initialize();

	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = 0.0f;
	worldTransform_.translation_.z = 0.0f;
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
