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
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	VectorInt2 mapPosition_;
	WorldTransform worldTransform_;
};
