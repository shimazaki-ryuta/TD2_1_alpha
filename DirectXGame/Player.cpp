#include "Player.h"
#include "Input.h"
#include <algorithm>
#include "ImGuiManager.h"
void (Player::*Player::phaseTable[])() = {&Player::Idle, &Player::Move, &Player::Break};

void Player::Initialize(){
	input_ = Input::GetInstance();
	model_.reset(Model::Create());
	worldTransform_.Initialize();
	targetWorldTransform_.Initialize();
	nowWorldTransform_.Initialize();
}

void Player::Update() {
	/* if (input_->TriggerKey(DIK_LEFT)) {
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
	mapPosition_.x = std::clamp(mapPosition_.x, 0, int(MapManager::GetInstance()->kMapWidth-1));
	mapPosition_.y = std::clamp(mapPosition_.y, 0, int(MapManager::GetInstance()->kMapHeight-1));

	Vector3 worldPos = MapManager::GetworldPosition(mapPosition_);
	worldTransform_.translation_.x = worldPos.x;
	worldTransform_.translation_.z = worldPos.z;
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.UpdateMatrix();
	if (input_->TriggerKey(DIK_SPACE))
	{
		MapManager::GetInstance()->BreakBlock(mapPosition_);
	}*/
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_X)) {
		MapManager::GetInstance()->CreateBlock(mapPosition_);
	}
#endif // _DEBUG

	(this->*phaseTable[static_cast<size_t>(phase_)])();
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.UpdateMatrix();
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::SliderInt("move", &moveEnd, 1, 120);
	ImGui::End();
#endif
}

void Player::Idle() {
	//moveTarget_ = {0,0};
	if (input_->TriggerKey(DIK_LEFT)) {
		moveTarget_.x=-1;
		moveTarget_.y = 0;
		isMove_ = true;
	}
	else if (input_->TriggerKey(DIK_RIGHT)) {
		moveTarget_.x=1;
		moveTarget_.y = 0;
		isMove_ = true;
	}
	else if (input_->TriggerKey(DIK_UP)) {
		moveTarget_.y=-1;
		moveTarget_.x = 0;
		isMove_ = true;
	}
	else if (input_->TriggerKey(DIK_DOWN)) {
		moveTarget_.y=1;
		moveTarget_.x = 0;
		isMove_ = true;
	}
	if (isMove_)
	{
		isMove_ = false;
		frameCount_ = 0;
		phase_ = Phase::Move;
		moveTarget_.x = moveTarget_.x + mapPosition_.x;
		moveTarget_.y = moveTarget_.y + mapPosition_.y;
		moveTarget_.x =
		    std::clamp(moveTarget_.x, 0, int(MapManager::GetInstance()->kMapWidth - 1));
		moveTarget_.y =
		    std::clamp(moveTarget_.y, 0, int(MapManager::GetInstance()->kMapHeight - 1));

		targetWorldTransform_.translation_ = MapManager::GetInstance()->GetworldPosition(moveTarget_);
		nowWorldTransform_.translation_ = MapManager::GetInstance()->GetworldPosition(mapPosition_);
		nextPosition_ = moveTarget_;
		moveTarget_ = {0,0};
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		//MapManager::GetInstance()->BreakBlock(mapPosition_);
		phase_ = Phase::Break;
	}
}

void Player::Move() {
	if (input_->TriggerKey(DIK_LEFT)) {
		moveTarget_.x = -1;
		moveTarget_.y = 0;
		isMove_ = true;
	} else if (input_->TriggerKey(DIK_RIGHT)) {
		moveTarget_.x = 1;
		moveTarget_.y = 0;
		isMove_ = true;
	} else if (input_->TriggerKey(DIK_UP)) {
		moveTarget_.y = -1;
		moveTarget_.x = 0;
		isMove_ = true;
	} else if (input_->TriggerKey(DIK_DOWN)) {
		moveTarget_.y = 1;
		moveTarget_.x = 0;
		isMove_ = true;
	}

	float t = float(frameCount_) / float(moveEnd);
	worldTransform_.translation_.x =
	    (1.0f - t) * nowWorldTransform_.translation_.x + t * targetWorldTransform_.translation_.x;
	worldTransform_.translation_.z =
	    (1.0f - t) * nowWorldTransform_.translation_.z + t * targetWorldTransform_.translation_.z;
	if (frameCount_ >= moveEnd) {
		mapPosition_ = nextPosition_;

		phase_ = Phase::Idle;
	}
	frameCount_++;
}

void Player::Break(){
	MapManager::GetInstance()->BreakBlock(mapPosition_);
	phase_ = Phase::Idle;
}

void Player::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_,viewProjection); }