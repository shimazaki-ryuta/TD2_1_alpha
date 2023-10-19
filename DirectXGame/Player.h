#pragma once
#include "MapManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "VectorInt.h"

#include <memory>

class Player {
public:

	enum class Phase {
		Idle,
		Move,
		Break,
	};

	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void Idle();
	void Move();
	void Break();

private:
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	VectorInt2 mapPosition_;
	VectorInt2 moveTarget_;
	VectorInt2 nextPosition_;

	WorldTransform worldTransform_;

	int frameCount_ = 0;
	int moveEnd = 3;
	Phase phase_ = Phase::Idle;
	static void (Player::*phaseTable[])();

	WorldTransform targetWorldTransform_;
	WorldTransform nowWorldTransform_;

	bool isMove_ = false;
};
