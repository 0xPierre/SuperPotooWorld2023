#pragma once

#include "Settings.h"
#include "Scene.h"
#include "Player.h"
#include "LevelCanvas.h"
#include "LevelData.h"
#include "StaticMap.h"
#include "LevelEnd.h"
#include "Creative.h"
#include "Firefly.h"

class LevelScene : public Scene
{
public:
	LevelScene(SDL_Renderer* renderer, RE_Timer& mainTime, LevelData& level, bool isCreative);
	LevelScene(LevelScene const&) = delete;
	LevelScene& operator=(LevelScene const&) = delete;
	virtual ~LevelScene();

	virtual bool Update() override;
	virtual void OnRespawn() override;

	Player* GetPlayer() const;
	bool IsPaused() const;
	void SetPaused(bool isPaused);

	StaticMap* GetMap() const;
	void SetMap(StaticMap* map);

	bool IsCreative() const;
	void SetCreative(bool isCreative);

	LevelData* GetLevelData() const;

	LevelEnd* GetLevelEnd() const;
	void SetLevelEnd(LevelEnd* levelEnd);

	PE_Vec2 GetStartingPoint() const;
	void SetStartingPoint(PE_Vec2 startingPoint);

	Creative* GetCreative() const;

	//std::vector<Firefly> GetFireflies() const;

private:
	std::array<Camera*, 2> m_cameras;
	Player* m_player;
	bool m_is_creative;
	LevelCanvas* m_canvas;
	LevelData* m_levelData;

	PE_Vec2 m_startPos;
	StaticMap* m_map;

	bool m_paused;
	int m_camIndex;
	float m_stepDelay;

	LevelEnd* m_levelEnd;
	PE_Vec2 m_startingPoint;

	Creative* m_creative;
	
	//std::vector<Firefly> m_fireflies;
};

//inline std::vector<Firefly> LevelScene::GetFireflies() const
//{
//	return m_fireflies;
//}

inline LevelData* LevelScene::GetLevelData() const
{
	return m_levelData;
}

inline Creative* LevelScene::GetCreative() const
{
	return m_creative;
}


inline StaticMap* LevelScene::GetMap() const
{
	return m_map;
}

inline void LevelScene::SetMap(StaticMap* map)
{
	m_map = map;
}

inline bool LevelScene::IsCreative() const
{
	return m_is_creative;
}

inline void LevelScene::SetCreative(bool isCreative)
{
	m_is_creative = isCreative;
}

inline Player* LevelScene::GetPlayer() const
{
	return m_player;
}

inline bool LevelScene::IsPaused() const
{
	return m_paused;
}

inline LevelEnd* LevelScene::GetLevelEnd() const
{
	return m_levelEnd;
}

inline void LevelScene::SetLevelEnd(LevelEnd* levelEnd)
{
	m_levelEnd = levelEnd;
}

inline PE_Vec2 LevelScene::GetStartingPoint() const
{
	return m_startingPoint;
}

inline void LevelScene::SetStartingPoint(PE_Vec2 startingPoint)
{
	m_startingPoint = startingPoint;
}