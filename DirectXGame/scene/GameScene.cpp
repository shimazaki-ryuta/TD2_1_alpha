#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() { delete debugCamera_; }

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	viewProjection_.Initialize();

	MapManager::GetInstance()->Initialize();
	camera_.Initialize();
	camera_.SetPosition(MapManager::GetInstance()->GetCenterworldPosition());

	player_.reset(new Player());
	player_->Initialize();

	unit_.reset(new Unit());
	unit_->Initialize();
}

void GameScene::Update() {
#ifdef _DEBUG
// デバッグカメラを有効化
if (input_->TriggerKey(DIK_0)) {
	isDebugCameraActive_ = !isDebugCameraActive_;
}
#endif // _DEBUG

	player_->Update();
	unit_->Update();

	if (isDebugCameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else
	{
		viewProjection_.matView = camera_.viewProjection_.matView;
		viewProjection_.matProjection = camera_.viewProjection_.matProjection;
		viewProjection_.TransferMatrix();
	}
		//viewProjection_.UpdateMatrix();
	debugCamera_->Update();
	camera_.SetPosition(MapManager::GetInstance()->GetCenterworldPosition());
	camera_.Update();
	MapManager::GetInstance()->Update();
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

	MapManager::GetInstance()->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	unit_->Draw(viewProjection_);

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
