#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0;
	// スプライト
	Sprite* sprite_ = nullptr;
	// 3Dモデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransfrom_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// サウンドデータハンドル
	uint32_t soudDateHandle_ = 0;
	// 音声再生ハンドル
	uint32_t voiceHandle_ = 0;
	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};
	// でバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};