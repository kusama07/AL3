#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation) {
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	
	worldTransform_.rotation_ = rotation;

	worldTransform_.scale_ = 1.0f;

	viewProjection_.Initialize();

}

void RailCamera::Update() { 
	worldTransform_.translation_.z += 0.03f;
	//worldTransform_.rotation_.x += 1.0f;
	
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.03f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 1.0f);

	ImGui::End();
}
