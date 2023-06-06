#pragma once
#include "LevelScene.h"
#include "Scene.h"
#include "MouseInput.h"
#include "ControlsInput.h"
#include "StaticMap.h"

class Creative {
public:
	Creative(LevelScene& levelScene, MouseInput& mouse);

	void AddItem(Tile::Type tileType, int groundSelect);
	void RemoveItem();
	void SaveInFile();

private:
	LevelScene* m_levelScene;
	MouseInput* m_mouse;
};