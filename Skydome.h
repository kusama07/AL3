#pragma once
#include "WorldTransform.h"
#include "model.h"

class Skydome {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection viewProjection);
	
private:
	// モデル
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

};