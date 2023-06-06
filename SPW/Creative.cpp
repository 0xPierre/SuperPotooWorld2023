#include "LevelScene.h"
#include "Creative.h"
#include "StaticMap.h"
#include "Camera.h"
#include "ControlsInput.h"

Creative::Creative(LevelScene& levelScene, MouseInput& mouse)
{
	this->m_levelScene = &levelScene;
	this->m_mouse = &mouse;
};


void Creative::AddItem(Tile::Type tileType, int groundSelected) {
	PE_Vec2 Pos;
	
	switch (tileType)
	{
	case Tile::Type::GROUND:
	case Tile::Type::STEEP_SLOPE_L:
	case Tile::Type::STEEP_SLOPE_R:
	case Tile::Type::GENTLE_SLOPE_L1:
	case Tile::Type::GENTLE_SLOPE_L2:
	case Tile::Type::GENTLE_SLOPE_R1:
	case Tile::Type::GENTLE_SLOPE_R2:
	case Tile::Type::WOOD:
	case Tile::Type::ONE_WAY:
	case Tile::Type::SPIKE:
	case Tile::Type::BRICK:
	case Tile::Type::BONUSFULL:
	case Tile::Type::BONUSEMPTY:
	case Tile::Type::CHECKPOINTFULL:
	case Tile::Type::CHECKPOINTEMPTY:
		m_levelScene->GetActiveCamera()->ViewToWorld((int)m_mouse->viewPos.x, (int)m_mouse->viewPos.y, Pos);
		m_levelScene->GetMap()->SetTile(Pos.x, Pos.y, tileType, groundSelected);
	}
	/*case Tile::Type::LEVELEND:
		levelEnd *leve*/

	SaveInFile();

}

void Creative::RemoveItem() {
	PE_Vec2 Pos;

	m_levelScene->GetActiveCamera()->ViewToWorld((int)m_mouse->viewPos.x, (int)m_mouse->viewPos.y, Pos);
	Tile tile;
	bool removed = m_levelScene->GetMap()->RemoveTile(Pos.x, Pos.y, tile);
}

void Creative::SaveInFile() {
	printf("%s\n", m_levelScene->GetLevelData()->path.c_str());
	FILE* levelFile = fopen("../assets/Level/aa.txt", "wb");
	AssertNew(levelFile);

	const int width = m_levelScene->GetMap()->GetMaxWidth();
	const int height = m_levelScene->GetMap()->GetHeight();



	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
		}
	}

	printf("Update level\n");
}