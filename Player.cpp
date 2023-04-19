#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);

	worldTransform_.Initialize();

}

void Player::Update() {

}

void Player::Draw() {

}
