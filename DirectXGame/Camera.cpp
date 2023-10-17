#include "Camera.h"

void Camera::Initialize() { viewProjection_.Initialize(); }

void Camera::Update() { 
	viewProjection_.translation_.y = 50;
	viewProjection_.translation_.z -= 20;
	viewProjection_.rotation_.x = 3.14f/2.5f;

	viewProjection_.UpdateMatrix(); 
}