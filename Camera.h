#pragma once
#include "ViewProjection.h"
#include "Vector3.h"
class Camera {
public:
	void Initialize();
	void Update();
	void SetPosition(const Vector3& position) { viewProjection_.translation_ = position; };
	ViewProjection viewProjection_;
};
