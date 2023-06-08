#pragma once
#include "LevelScene.h"
#include "Scene.h"
#include "MouseInput.h"
#include "ControlsInput.h"
#include "StaticMap.h"
#include "GameBody.h"

class Creative {
public:
	Creative(LevelScene& levelScene);

	void AddItem(Tile::Type tileType, int groundSelect, MouseInput& mouse);
	void RemoveItem(MouseInput& mouse);
	Tile::Type SelectItem(MouseInput& mouse);
	void SaveInFile();

private:
	LevelScene* m_levelScene;
	MouseInput* m_mouse;
};