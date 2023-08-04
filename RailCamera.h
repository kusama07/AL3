#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Matrix.h"
#include "ImGuiManager.h"

class RailCamera {
public:
	void Initialize(const Vector3& position,const Vector3&rotation);

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

};
