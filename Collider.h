#pragma once
#include "Vector3.h"

class Collider {
private:
	float radius_ = 1.0f;

	//衝突属性(自分)
	uint32_t colliderAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;

public:
	// 半径を取得
	float GetRadius() const { return radius_; }

	// 半径を設定
	void SetRadius(float& radius) { radius_ = radius; }

	// 衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() const { return colliderAttribute_; }
	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t colliderAttribute ) { colliderAttribute_ = colliderAttribute; }
	// 衝突マスク(相手)を取得
	uint32_t GetCollisionMask() const { return CollisionMask_; }
	// 衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t collisionMask) { CollisionMask_ = collisionMask; }

	virtual void OnCollision() = 0;

	virtual Vector3 GetWorldPosition() = 0;

};
