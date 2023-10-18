#pragma once
#include "MapManager.h"
#include "Model.h"
#include "VectorInt.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>

class Unit {
public:

	enum class Phase {
		Next,
		Move,
		Create,
	};

	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void Next();
	void Move();
	void Create();

private:
	std::unique_ptr<Model> model_;
	VectorInt2 mapPosition_;
	WorldTransform worldTransform_;
	WorldTransform targetWorldTransform_;
	WorldTransform nowWorldTransform_;

	VectorInt2 target_;

	Vector3 velocity_;
	int frameCount_=0;
	const int moveEnd = 60;
	Phase phase_ = Phase::Next;
	static void (Unit::*phaseTable[])();

	//生きてるか
	bool isLive_;
	int respawnCoolTime = 0;
	int kRespawnTime = 300;
};
