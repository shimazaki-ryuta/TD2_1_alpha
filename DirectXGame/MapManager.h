#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <stdint.h>
#include <array>
#include <list>
#include "VectorInt.h"
#include <Vector3.h>
#include <memory>
#include "Model.h"
#include "TextureManager.h"
class MapManager {
public: 
	enum class MapState {
		None,
		Block,
		Core,
		Bomb,
		MaskBlock,
		ChaindBomb,
		UnChaindBomb,
	};

	enum class Direction {
		None,
		Vertical,
		Horizon,
	};

	struct Map {
		MapState mapstate;
		bool vertical;
		bool horizon;
		WorldTransform worldTransform;
	};

	struct Bomb {
		VectorInt2 position;
		int chaind;
	};

	static const uint32_t kMapWidth = 15;
	static const uint32_t kMapHeight = 10;

	const float kBlockWidth = 1.0f;
	const float kBlockHeight = 1.0f;

	static MapManager* GetInstance();
	void Initialize();
	void Update();
	void MapRead();
	void FindChain();
	void Protect(int x, int y);
	//static VectorInt GetMapNum(const Vector3& worldPosition);
	static Vector3 GetworldPosition(VectorInt2 vector);
	static Vector3 GetCenterworldPosition();

	void BreakBlock(const VectorInt2& position);
	void CreateBlock(const VectorInt2& position);
	void CreateBlock(const VectorInt2& position,Direction direction);

	MapState GetState(const VectorInt2& positiom) { return map[positiom.y][positiom.x].mapstate; };

	VectorInt2 GetCorePosition();

	VectorInt2 GetPriority();

	void Draw(const ViewProjection& );

private:
	MapManager() = default;
	~MapManager() = default;
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;

	std::unique_ptr<Model> model;
	std::unique_ptr<Model> modelVertical_;
	std::unique_ptr<Model> modelHorizon_;


	uint32_t blockTextureHandle_=0;
	uint32_t coreTextureHandle_=0;
	uint32_t BombTextureHandle_ = 0;
	uint32_t unChaindTextureHandle_ = 0;


	Map map[kMapHeight][kMapWidth];
	//std::list<WorldTransform> worldTransforms_;
	
	std::list<Bomb> bombs_;
};
