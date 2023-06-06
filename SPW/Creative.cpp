#include "LevelScene.h"
#include "Creative.h"
#include "StaticMap.h"
#include "Camera.h"
#include "ControlsInput.h"
#include "ObjectManager.h"

Creative::Creative(LevelScene& levelScene, MouseInput& mouse)
{
	this->m_levelScene = &levelScene;
	this->m_mouse = &mouse;
};


void Creative::AddItem(Tile::Type tileType, int groundSelected) {
	PE_Vec2 Pos;
	m_levelScene->GetActiveCamera()->ViewToWorld((int)m_mouse->viewPos.x, (int)m_mouse->viewPos.y, Pos);

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
		
		m_levelScene->GetMap()->SetTile(Pos.x, Pos.y, tileType, groundSelected);
		break;
	case Tile::Type::LEVELEND:
		// Delete the older Level End
		m_levelScene->GetLevelEnd()->SetEnabled(false);
		// Create another one
		LevelEnd* levelEnd = new LevelEnd(*m_levelScene);
		levelEnd->SetStartPosition(Pos);
		m_levelScene->SetLevelEnd(levelEnd);
		//m_levelScene->GetLevelEnd()->SetStartPosition(Pos);
		//m_levelScene->GetLevelEnd()->SetToRespawn(true);
		//m_levelScene->GetLevelEnd()->Start();
		//m_levelScene->GetLevelEnd()->SetEnabled(true);
		break;
	}

	SaveInFile();

}

void Creative::RemoveItem() {
	PE_Vec2 Pos;

	m_levelScene->GetActiveCamera()->ViewToWorld((int)m_mouse->viewPos.x, (int)m_mouse->viewPos.y, Pos);
	Tile tile;
	bool removed = m_levelScene->GetMap()->RemoveTile(Pos.x, Pos.y, tile);

	PE_Vec2 PosLevelEnd = m_levelScene->GetLevelEnd()->GetPosition();
	// No beautiful but it works
	if (((int)Pos.x == (int)PosLevelEnd.x || (int)Pos.x == ((int)PosLevelEnd.x+1)) && (int)Pos.y == (int)PosLevelEnd.y)
	{
		m_levelScene->GetLevelEnd()->SetEnabled(false);
	}

	SaveInFile();
}

char GetCharFromTile(Tile tile)
{
	char c = '.';
	switch (tile.type)
	{
	case Tile::Type::GROUND:
		switch (tile.partIdx)
		{
		case 4:
			return '#';
		case 1:
			return '_';
		case 17:
			return '[';
		case 14:
			return ']';
		case 0:
			return '{';
		case 2:
			return '}';
		}

		break;
	case Tile::Type::WOOD:
		return 'W';
	case Tile::Type::ONE_WAY:
		return '=';
	case Tile::Type::SPIKE:
		return 'A';
	case Tile::Type::STEEP_SLOPE_L:
		return '//';
	case Tile::Type::STEEP_SLOPE_R:
		return '\\';
	case Tile::Type::GENTLE_SLOPE_L1:
		return 'L';
	case Tile::Type::GENTLE_SLOPE_L2:
		return 'l';
	case Tile::Type::GENTLE_SLOPE_R1:
		return 'R';
	case Tile::Type::GENTLE_SLOPE_R2:
		return 'r';
	}

	return c;
}

void Creative::SaveInFile() {
	printf("%s\n", m_levelScene->GetLevelData()->path.c_str());
	FILE* levelFile = fopen("../assets/Level/aa.txt", "wb");
	AssertNew(levelFile);

	const int width = m_levelScene->GetMap()->GetMaxWidth();
	const int height = m_levelScene->GetMap()->GetHeight();

	
	Tile** tiles = m_levelScene->GetMap()->GetTiles();

	PE_Vec2 level_end = m_levelScene->GetLevelEnd()->GetStartPosition();
	PE_Vec2 starting_point = m_levelScene->GetStartingPoint();
	printf("%f %f %f %f\n", level_end.x, level_end.y, starting_point.x, starting_point.y);

	char c;
	for (int y = height; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			if (x == (int)level_end.x && y == (int)level_end.y) {
				fputc('F', levelFile);
			}
			else if (x == (int)starting_point.x && y == (int)starting_point.y)
			{
				fputc('S', levelFile);
			}
			else
			{
				char c = GetCharFromTile(tiles[x][y]);
				fputc(c, levelFile);
			}
		}
		fputc('\n', levelFile);
	}


	fclose(levelFile);

	printf("Update level\n");
}