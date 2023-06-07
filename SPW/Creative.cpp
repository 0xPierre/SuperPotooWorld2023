#include "LevelScene.h"
#include "Creative.h"
#include "StaticMap.h"
#include "Camera.h"
#include "ControlsInput.h"
#include "ObjectManager.h"
#include "Firefly.h"

Creative::Creative(LevelScene& levelScene)
{
	this->m_levelScene = &levelScene;
};


void Creative::AddItem(Tile::Type tileType, int groundSelected, MouseInput& mouse) {
	PE_Vec2 Pos;
	m_levelScene->GetActiveCamera()->ViewToWorld((int)mouse.viewPos.x, (int)mouse.viewPos.y, Pos);
	LevelEnd* levelEnd;

	PE_Vec2 lower;
	lower.x = (int)Pos.x;
	lower.y = (int)Pos.y;
	PE_Vec2 upper;
	upper.x = (int)Pos.x + 1;
	upper.y = (int)Pos.y + 1;

	PE_AABB aabb;
	aabb.lower = lower;
	aabb.upper = upper;

	printf("test\n");
	GameBody* gm = m_levelScene->OverlapArea(aabb, CATEGORY_COLLECTABLE);

	if (gm != nullptr)
	{
		printf("%f %f", gm->GetPosition().x, gm->GetPosition().y);
	}

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
		// Set position to middle of the block
		Pos.x = ((float)(int)Pos.x) + 0.3f;
		Pos.y = (int)Pos.y;
		// Create another one
		levelEnd = new LevelEnd(*m_levelScene);
		levelEnd->SetStartPosition(Pos);
		m_levelScene->SetLevelEnd(levelEnd);
		break;
	case Tile::Type::FIREFLY:
		Firefly* firefly = new Firefly(*m_levelScene);
		Pos.x = (int)Pos.x;
		Pos.y = (int)Pos.y;
		firefly->SetStartPosition(Pos);

		break;
	}
}

void Creative::RemoveItem(MouseInput& mouse) {
	PE_Vec2 Pos;

	m_levelScene->GetActiveCamera()->ViewToWorld((int)mouse.viewPos.x, (int)mouse.viewPos.y, Pos);
	Tile tile;
	bool removed = m_levelScene->GetMap()->RemoveTile(Pos.x, Pos.y, tile);

	PE_Vec2 PosLevelEnd = m_levelScene->GetLevelEnd()->GetPosition();
	// No beautiful but it works
	if (((int)Pos.x == (int)PosLevelEnd.x || (int)Pos.x == ((int)PosLevelEnd.x+1)) && (int)Pos.y == (int)PosLevelEnd.y)
	{
		m_levelScene->GetLevelEnd()->SetEnabled(false);
	}
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