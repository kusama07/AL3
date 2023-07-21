#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵の生成
	enemy_ = new Enemy();
	//敵の初期化
	enemy_->Initialize(model_, textureHandle_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(50,50);
}

void GameScene::CheckAllCollisions() { 

	Vector3 posA, posB;

	float playerRadius = 1.5f;
	float enemyRadius = 1.5f;
	float enemyBulletRadius = 1.5f;
	float playerBulletRadius = 1.5f;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->Player::GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->EnemyBullet::GetWorldPosition();
		// 座標Aと座標Bの距離を求める
		Vector3 distanceMagnitude = magnitude(posB, posA);

		if (distanceMagnitude.x + distanceMagnitude.y + distanceMagnitude.z <=
		    (enemyBulletRadius + playerRadius) * (enemyBulletRadius + playerRadius)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す　
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	posA = enemy_->Enemy::GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->PlayerBullet::GetWorldPosition();
		Vector3 distanceMagnitude = magnitude(posB, posA);

		if (distanceMagnitude.x + distanceMagnitude.y + distanceMagnitude.z <=
		    (playerBulletRadius + enemyRadius) * (playerBulletRadius + enemyRadius)) {
			bullet->OnCollision();
			enemy_->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (EnemyBullet* enemyBullet : enemyBullets) {

		posA = enemyBullet->EnemyBullet::GetWorldPosition();
		for (PlayerBullet* playerBullet : playerBullets) {

			posB = playerBullet->PlayerBullet::GetWorldPosition();
			Vector3 distanceMagnitude = magnitude(posB, posA);

			if (distanceMagnitude.x + distanceMagnitude.y + distanceMagnitude.z <=
			    (playerBulletRadius + enemyBulletRadius) *
			        (playerBulletRadius + enemyBulletRadius)) {
				enemyBullet->OnCollision();
				playerBullet->OnCollision();
			}
		}
	}

#pragma endregion

}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
	CheckAllCollisions();

	if (enemy_ != nullptr) {
		enemy_->Update();
	}

	//デバッグカメラの更新
	debugCamera_->Update();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
	#endif

	//カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);

	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}