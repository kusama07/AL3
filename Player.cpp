#include "Player.h"
#include <cassert>
#include "Input.h"
#include "Matrix.h"
#include "Vector3.h"
#include "ImGuiManager.h"
#include "TransforNormal.h"

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;

	}
	delete sprite2DReticle_;

}

void Player::Initialize(Model* model, uint32_t textureHandle,const Vector3& playerPosition) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = playerPosition;

	worldTransform3DReticle_.Initialize();

	input_ = Input::GetInstance();

	uint32_t textureReticle = TextureManager::Load("reticle.png");

	ReticlePos_.x = 760;
	ReticlePos_.y = 320;

	// スプライト生成
	sprite2DReticle_ =
	    Sprite::Create(textureReticle, ReticlePos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});


}

void Player::Update(ViewProjection viewProjection) { 
	worldTransform_.TransferMatrix();


	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	if (Input::GetInstance()->GetJoystickState(0, joyState_)) {
		move.x += (float)joyState_.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState_.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	float inputFloat3[3] = {0, 0, 0};
	float moves[3] = {move.x + 1, move.y + 1, move.z + 1};

	ImGui::Begin("Player");
	ImGui::SliderFloat3("SliderFloat3", moves, 0.0f, 2.0f);
	ImGui::Text("SPACE:DebugCamera");
	ImGui::End();

	move.x = moves[0] - 1;
	move.y = moves[1] - 1;
	move.z = moves[2] - 1;


	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);


	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.UpdateMatrix();

	// 移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

		// 自機が3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;

	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};

	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);

	// ベクトルの長さを整える
	offset = Normalize(offset);
	offset.x *= kDistancePlayerTo3DReticle;
	offset.y *= kDistancePlayerTo3DReticle;
	offset.z *= kDistancePlayerTo3DReticle;

	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = offset.x + worldPos.x;
	worldTransform3DReticle_.translation_.y = offset.y + worldPos.y;
	worldTransform3DReticle_.translation_.z = offset.z + worldPos.z;

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();

	Vector3 positionReticle;
	positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];


	if (Input::GetInstance()->GetJoystickState(0, joyState_)) {
		ReticlePos_.x += (float)joyState_.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		ReticlePos_.y -= (float)joyState_.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		sprite2DReticle_->SetPosition(ReticlePos_);
	}

	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列を合成する
	Matrix4x4 matViewprojectionViewport =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

	positionReticle = Transform(positionReticle, matViewprojectionViewport);
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	
	GetMouse(viewProjection);

	Rotate();

	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

}

void Player::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Rotate() {
	// 回転の速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;

	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {

	if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
		return;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		velocity.x = worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x;
		velocity.y = worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y;
		velocity.z = worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z;

		velocity = Normalize(velocity);

		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;

		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);
	}
}

void Player::GetMouse(ViewProjection viewProjection) {
	//マウスカーソルのスクリーンさ表からワールド座標を取得して3Dレティクル配置

	POINT mousePosition;

	GetCursorPos(&mousePosition);

	HWND hwnd = WinApp::GetInstance()->GetHwnd();

	ScreenToClient(hwnd, &mousePosition);

	Vector2 Reticle;
	Reticle.x = float(ReticlePos_.x);
	Reticle.y = float(ReticlePos_.y);
	sprite2DReticle_->SetPosition(Reticle);

	Matrix4x4 matVPV = Multiply(
	    viewProjection.matView,
	    Multiply(
	        viewProjection.matProjection,
	        MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1)));

	Matrix4x4 matInveresVPV = Inverse(matVPV);

	Vector3 PosNear = Vector3(float(ReticlePos_.x), float(ReticlePos_.y), 0);
	Vector3 PosFar = Vector3(float(ReticlePos_.x), float(ReticlePos_.y), 1);

	PosNear = Transform(PosNear, matInveresVPV);
	PosFar = Transform(PosFar, matInveresVPV);

	Vector3 mouseDirection = Add(PosFar, PosNear);
	mouseDirection = Normalize(mouseDirection);

	const float kDistanceTestobject = 50.0f;
	worldTransform3DReticle_.translation_.x = PosNear.x + mouseDirection.x * kDistanceTestobject;
	worldTransform3DReticle_.translation_.y = PosNear.y + mouseDirection.y * kDistanceTestobject;
	worldTransform3DReticle_.translation_.z = PosNear.z + mouseDirection.z * kDistanceTestobject;

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::SetParent(const WorldTransform* parent){
	worldTransform_.parent_ = parent;

};

void Player::OnCollision(){};