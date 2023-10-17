#include "Player.h"
#include "Input.h"
#include <algorithm>
void Player::Initialize(){
	input_ = Input::GetInstance();
	model_.reset(Model::Create());
	worldTransform_.Initialize();
}

void Player::Update() {
	if (input_->TriggerKey(DIK_LEFT)) {
		mapPosition_.x--;
	}
	if (input_->TriggerKey(DIK_RIGHT)) {
		mapPosition_.x++;
	}
	if (input_->TriggerKey(DIK_UP)) {
		mapPosition_.y--;
	}
	if (input_->TriggerKey(DIK_DOWN)) {
		mapPosition_.y++;
	}
	mapPosition_.x = std::clamp(mapPosition_.x, 0, int(MapManager::GetInstance()->kMapWidth));
	mapPosition_.y = std::clamp(mapPosition_.y, 0, int(MapManager::GetInstance()->kMapHeight));

	Vector3 worldPos = MapManager::GetworldPosition(mapPosition_);
	worldTransform_.translation_.x = worldPos.x;
	worldTransform_.translation_.z = worldPos.z;
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.UpdateMatrix();
	if (input_->TriggerKey(DIK_SPACE))
	{
		MapManager::GetInstance()->BreakBlock(mapPosition_);
	}
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_X)) {
		MapManager::GetInstance()->CreateBlock(mapPosition_);
	}
#endif // _DEBUG
}


void Player::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_,viewProjection); }