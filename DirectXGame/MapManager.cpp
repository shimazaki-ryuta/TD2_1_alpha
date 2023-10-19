#include "MapManager.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "Input.h"

MapManager* MapManager::GetInstance()
{
	static MapManager instance;
	return &instance;
}

void MapManager::Initialize() {
	bombs_.clear();
	MapRead(); 
	model.reset(Model::CreateFromOBJ("RoadBase",true));
	modelTop_.reset(Model::CreateFromOBJ("RoadTop", true));
	modelDown_.reset(Model::CreateFromOBJ("RoadDown", true));
	modelLeft_.reset(Model::CreateFromOBJ("RoadLeft", true));
	modelRight_.reset(Model::CreateFromOBJ("RoadRight", true));
	//modelVertical_.reset(Model::CreateFromOBJ("RoadVertical",true));
	//modelHorizon_.reset(Model::CreateFromOBJ("RoadHorizon",true));

	blockTextureHandle_ = TextureManager::Load("road.png");
	coreTextureHandle_ = TextureManager::Load("Core.png");
	BombTextureHandle_ = TextureManager::Load("Bomb.png");
	unChaindTextureHandle_ = TextureManager::Load("UnChaind.png");
}

void MapManager::Update() {
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		MapRead();
	}
#endif // _DEBUG

}

void MapManager::MapRead()
{
	char readString[256];
	char* ptr;
	char* context = nullptr;
	FILE* fp = NULL;
	fopen_s(&fp, "./mapSample.csv", "rt");
	if (fp == NULL) {
		return;
	}
	int x = 0, y = 0;
	while (fgets(readString, sizeof(readString) / sizeof(char), fp) != NULL && y < kMapHeight) {
		ptr = strtok_s(readString, ",", &context);

		map[y][0].mapstate = MapState(atoi(ptr));
		map[y][0].worldTransform.Initialize();
		map[y][0].worldTransform.translation_.x = float(0)*2.0f;
		map[y][0].worldTransform.translation_.z = -float(y)*2.0f;
		map[y][0].worldTransform.UpdateMatrix();
		if (map[y][0].mapstate == MapState::Bomb) {
			bombs_.push_back({
			    {x, 0},
                0
            });
		}
		x = 1;
		while (ptr != NULL && x < kMapWidth) {
			ptr = strtok_s(NULL, ",", &context);
			if (ptr != NULL) {
				map[y][x].mapstate = MapState(atoi(ptr));
				map[y][x].worldTransform.Initialize();
				map[y][x].worldTransform.translation_.x = float(x)*2.0f;
				map[y][x].worldTransform.translation_.z = -float(y)*2.0f;
				map[y][x].worldTransform.UpdateMatrix();
				if (map[y][x].mapstate == MapState::Bomb)
				{
					bombs_.push_back({
					    {x, y},
                        0
                    });
				}
			}
			x++;
		}
		y++;
	}
	fclose(fp);
	for (Bomb& bomb : bombs_) {
		bomb.chaind = 0;
		if (bomb.position.y > 0 &&
		    map[bomb.position.y - 1][bomb.position.x].mapstate == MapState::Block) {
			bomb.chaind++;
		}
		if (bomb.position.y < kMapHeight - 1 &&
		    map[bomb.position.y + 1][bomb.position.x].mapstate == MapState::Block) {
			bomb.chaind++;
		}
		if (bomb.position.x > 0 &&
		    map[bomb.position.y][bomb.position.x - 1].mapstate == MapState::Block) {
			bomb.chaind++;
		}
		if (bomb.position.x < kMapWidth - 1 &&
		    map[bomb.position.y][bomb.position.x + 1].mapstate == MapState::Block) {
			bomb.chaind++;
		}
		if (bomb.chaind==0) {
			map[bomb.position.y][bomb.position.x].mapstate = MapState::UnChaindBomb;
		}
	}
}

void MapManager::FindChain()
{
	int centerX = 0, centerY = 0;
	bool isFind = false;
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			if (map[y][x].mapstate == MapState::Core) {
				centerX = x;
				centerY = y;
				Protect(x, y);
				isFind = true;
				break;
			}
		}
		if (isFind) {
			break;
		}
	}

	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			if (map[y][x].mapstate == MapState::Block) {
				map[y][x].mapstate = MapState::None;
			}
			if (map[y][x].mapstate == MapState::Bomb) {
				map[y][x].mapstate = MapState::UnChaindBomb;
			}
		}
	}
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			if (map[y][x].mapstate == MapState::MaskBlock) {
				map[y][x].mapstate = MapState::Block;
			}
			if (map[y][x].mapstate == MapState::ChaindBomb) {
				map[y][x].mapstate = MapState::Bomb;
			}
		}
	}
	if (isFind) {
		map[centerY][centerX].mapstate = MapState::Core;
	}
}

void MapManager::Protect(int x, int y) {
	if (map[y][x].mapstate == MapState::Block)
	{
		map[y][x].mapstate = MapState::MaskBlock;
	}
	if (map[y][x].mapstate == MapState::Bomb) {
		map[y][x].mapstate = MapState::ChaindBomb;
	}
	if (y > 0 && map[y - 1][x].mapstate == MapState::Block || 
		y > 0 && map[y - 1][x].mapstate == MapState::Bomb) {
		Protect(x, y - 1);
	}
	if (y < kMapHeight - 1 && map[y + 1][x].mapstate == MapState::Block ||
	    y < kMapHeight - 1 && map[y + 1][x].mapstate == MapState::Bomb) {
		Protect(x, y + 1);
	}
	if (x > 0 && map[y][x - 1].mapstate == MapState::Block || 
		x > 0 && map[y][x - 1].mapstate == MapState::Bomb) {
		Protect(x - 1, y);
	}
	if (x < kMapWidth - 1 && map[y][x + 1].mapstate == MapState::Block || 
		x < kMapWidth - 1 && map[y][x + 1].mapstate == MapState::Bomb) {
		Protect(x + 1, y);
	}
}
/*
VectorInt GetMapNum(const Vector3& worldPosition)
{

}
*/

Vector3 MapManager::GetworldPosition(VectorInt2 vector) { return GetInstance()->map[vector.y][vector.x].worldTransform.translation_; }
Vector3 MapManager::GetCenterworldPosition() {
	return GetInstance()->map[kMapHeight/2][kMapWidth/2].worldTransform.translation_;
}

void MapManager::Draw(const ViewProjection& viewProjecttion)
{
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			if (map[y][x].mapstate != MapState::None) {
				if (map[y][x].mapstate == MapState::Block)
				{
					model->Draw(map[y][x].worldTransform, viewProjecttion);
					if (map[y][x].top) {
						modelTop_->Draw(map[y][x].worldTransform, viewProjecttion);
					}
					if (map[y][x].down) {
						modelDown_->Draw(map[y][x].worldTransform, viewProjecttion);
					}
					if (map[y][x].left) {
						modelLeft_->Draw(map[y][x].worldTransform, viewProjecttion);
					}
					if (map[y][x].right) {
						modelRight_->Draw(map[y][x].worldTransform, viewProjecttion);
					}
				}
				if (map[y][x].mapstate == MapState::Core) {
					model->Draw(map[y][x].worldTransform, viewProjecttion,coreTextureHandle_);
				}
				if (map[y][x].mapstate == MapState::Bomb) {
					model->Draw(map[y][x].worldTransform, viewProjecttion, BombTextureHandle_);
				}
				if (map[y][x].mapstate == MapState::UnChaindBomb) {
					model->Draw(map[y][x].worldTransform, viewProjecttion, unChaindTextureHandle_);
				}
			}
		}
	}
}

void MapManager::BreakBlock(const VectorInt2& position) {
	if (map[position.y][position.x].mapstate == MapState::Block) {
		map[position.y][position.x].mapstate = MapState::None;

		VectorInt2 clampdPos;
		clampdPos.x = std::clamp(int(position.x), 1, int(kMapWidth - 1));
		clampdPos.y = std::clamp(int(position.y), 1, int(kMapHeight - 1));


		map[clampdPos.y - 1][clampdPos.x].down = false;
		map[clampdPos.y + 1][clampdPos.x].top = false;
		map[clampdPos.y][clampdPos.x - 1].right = false;
		map[clampdPos.y][clampdPos.x + 1].left = false;
	
		FindChain();
	}
}

void MapManager::CreateBlock(const VectorInt2& position) {
	VectorInt2 clampdPos;
	clampdPos.x = std::clamp(int(position.x), 1, int(kMapWidth - 1));
	clampdPos.y = std::clamp(int(position.y), 1, int(kMapHeight - 1));

	if (map[position.y][position.x].mapstate == MapState::None) {
		map[position.y][position.x].mapstate = MapState::Block;
		if (map[clampdPos.y-1][clampdPos.x].mapstate == MapState::UnChaindBomb){
			map[clampdPos.y-1][clampdPos.x].mapstate = MapState::Bomb;
			map[position.y][position.x].top = true;
		}
		if (map[clampdPos.y+1][clampdPos.x].mapstate == MapState::UnChaindBomb) {
			map[clampdPos.y+1][clampdPos.x].mapstate = MapState::Bomb;
			map[position.y][position.x].down = true;
		}
		if (map[clampdPos.y][clampdPos.x-1].mapstate == MapState::UnChaindBomb) {
			map[clampdPos.y][clampdPos.x-1].mapstate = MapState::Bomb;
			map[position.y][position.x].right = true;
		}
		if (map[clampdPos.y][clampdPos.x+1].mapstate == MapState::UnChaindBomb) {
			map[clampdPos.y][clampdPos.x + 1].mapstate = MapState::Bomb;
			map[position.y][position.x].left = true;
		}
	}
}

void MapManager::CreateBlock(const VectorInt2& position, Direction direction) {
	if (map[position.y][position.x].mapstate == MapState::None) {
		map[position.y][position.x].top = false;
		map[position.y][position.x].down = false;
		map[position.y][position.x].left = false;
		map[position.y][position.x].right = false;

		if (direction == Direction::Top) {
			map[position.y][position.x].down = true;
		} else if (direction == Direction::Down) {
			map[position.y][position.x].top = true;
		} else if (direction == Direction::Left) {
			map[position.y][position.x].right = true;
		} else if (direction == Direction::Right) {
			map[position.y][position.x].left = true;
		}
		VectorInt2 clampdPos;
		clampdPos.x = std::clamp(int(position.x), 1, int(kMapWidth - 1));
		clampdPos.y = std::clamp(int(position.y), 1, int(kMapHeight - 1));
		
		if (map[position.y][position.x].mapstate == MapState::None) {
			if (map[clampdPos.y - 1][clampdPos.x].mapstate == MapState::Block || 
				map[clampdPos.y - 1][clampdPos.x].mapstate == MapState::Core) {
				map[clampdPos.y - 1][clampdPos.x].down = true;
				map[position.y][position.x].top = true;
			}
			if (map[clampdPos.y + 1][clampdPos.x].mapstate == MapState::Block || 
				map[clampdPos.y + 1][clampdPos.x].mapstate == MapState::Core) {
				map[clampdPos.y + 1][clampdPos.x].top = true;
				map[position.y][position.x].down = true;
			}
			if (map[clampdPos.y][clampdPos.x - 1].mapstate == MapState::Block || 
				map[clampdPos.y][clampdPos.x - 1].mapstate == MapState::Core) {
				map[clampdPos.y][clampdPos.x - 1].right = true;
				map[position.y][position.x].left = true;
			}
			if (map[clampdPos.y][clampdPos.x + 1].mapstate == MapState::Block || 
				map[clampdPos.y][clampdPos.x + 1].mapstate == MapState::Core) {
				map[clampdPos.y][clampdPos.x + 1].left = true;
				map[position.y][position.x].right = true;
			}
		}
		
		CreateBlock(position);
	}
}

VectorInt2 MapManager::GetPriority() {
	for (Bomb &bomb : bombs_){
		bomb.chaind=0;
		if (bomb.position.y > 0 && map[bomb.position.y - 1][bomb.position.x].mapstate == MapState::Block) {
			bomb.chaind++;
		}
		if (bomb.position.y < kMapHeight - 1 && map[bomb.position.y + 1][bomb.position.x].mapstate == MapState::Block) {
			bomb.chaind++;
		}
		if (bomb.position.x > 0 && map[bomb.position.y][bomb.position.x - 1].mapstate == MapState::Block) {
			bomb.chaind++;
		}
		if (bomb.position.x < kMapWidth - 1 && map[bomb.position.y][bomb.position.x + 1].mapstate == MapState::Block) {
			bomb.chaind++;
		}	
	}
	Bomb priolity = bombs_.front();
	for (Bomb& bomb : bombs_) {
		if (priolity.chaind > bomb.chaind)
		{
			priolity = bomb;
		}
	}
	return priolity.position;
}

VectorInt2 MapManager::GetCorePosition() {
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			if (map[y][x].mapstate == MapState::Core) {
				return VectorInt2{x,y};
			}
		}
	}
	return {0,0};
}