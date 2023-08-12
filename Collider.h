#pragma once
#include "Vector3.h"

class Collider {
private:
	float radius_ = 1.0f;

public:
	// 半径を取得
	float GetRadius() const { return radius_; }

	// 半径を設定
	void SetRadius(float& radius) { radius_ = radius; }

	virtual void OnCollision() = 0;

	virtual Vector3 GetWorldPosition() = 0;

};
