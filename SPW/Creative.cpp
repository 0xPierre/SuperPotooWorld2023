#include "LevelScene.h"
#include "Creative.h"
#include "StaticMap.h"
#include "Camera.h"
#include "ControlsInput.h"
#include "ObjectManager.h"
#include "Firefly.h"
#include "Nut.h"

Creative::Creative(LevelScene& levelScene)
{
	this->m_levelScene = &levelScene;
};


void Creative::AddItem(Tile::Type tileType, int groundSelected, MouseInput& mouse) {
	PE_Vec2 Pos;
	m_levelScene->GetActiveCamera()->ViewToWorld((int)mouse.viewPos.x, (int)mouse.viewPos.y, Pos);
		
	// Remove actual tile
	bool removed = m_levelScene->GetMap()->RemoveTile(Pos.x, Pos.y);

	PE_Vec2 lower;
	lower.x = (int)Pos.x;
	lower.y = (int)Pos.y;
	PE_Vec2 upper;
	upper.x = (int)Pos.x + 1;
	upper.y = (int)Pos.y + 1;

	PE_AABB aabb;
	aabb.lower = lower;
	aabb.upper = upper;

	GameBody* gm = m_levelScene->OverlapArea(aabb, CATEGORY_COLLECTABLE);

	if (gm != nullptr)
	{
		gm->SetEnabled(false);
	}

	LevelEnd* levelEnd;
	Firefly* firefly;
	Nut* nut;

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
		firefly = new Firefly(*m_levelScene);
		Pos.x = (int)Pos.x;
		Pos.y = (int)Pos.y;
		firefly->SetStartPosition(Pos);

		break;
	case Tile::Type::NUT:
		nut = new Nut(*m_levelScene);
		Pos.x = (int)Pos.x;
		Pos.y = (int)Pos.y;
		nut->SetStartPosition(Pos);
	}
}

void Creative::RemoveItem(MouseInput& mouse) {
	PE_Vec2 Pos;
	m_levelScene->GetActiveCamera()->ViewToWorld((int)mouse.viewPos.x, (int)mouse.viewPos.y, Pos);

	PE_Vec2 lower;
	lower.x = (int)Pos.x;
	lower.y = (int)Pos.y;
	PE_Vec2 upper;
	upper.x = (int)Pos.x + 1;
	upper.y = (int)Pos.y + 1;

	PE_AABB aabb;
	aabb.lower = lower;
	aabb.upper = upper;

	GameBody* gm = m_levelScene->OverlapArea(aabb, CATEGORY_COLLECTABLE | CATEGORY_ENEMY);

	if (gm != nullptr)
	{
		gm->SetEnabled(false);
		// Make sur nothing is deleted behind
		return;
	}

	bool removed = m_levelScene->GetMap()->RemoveTile(Pos.x, Pos.y);

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
		case 8:
			return ')';
		case 11:
			return '(';
		case 0:
			return '{';
		case 2:
			return '}';
		}

		break;
	case Tile::Type::BRICK:
		return 'B';
	case Tile::Type::BONUSFULL:
		return '?';
	case Tile::Type::BONUSEMPTY:
		return '^';
	case Tile::Type::WOOD:
		return 'W';
	case Tile::Type::ONE_WAY:
		return '=';
	case Tile::Type::SPIKE:
		return 'A';
	case Tile::Type::STEEP_SLOPE_R:
		return '/';
	case Tile::Type::STEEP_SLOPE_L:
		return '\\';
	case Tile::Type::GENTLE_SLOPE_R1:
		return 'L';
	case Tile::Type::GENTLE_SLOPE_R2:
		return 'l';
	case Tile::Type::GENTLE_SLOPE_L1:
		return 'R';
	case Tile::Type::GENTLE_SLOPE_L2:
		return 'r';
	}
	return c;
}

void Creative::SaveInFile() {
	printf("Update : %s\n", m_levelScene->GetLevelData()->path.c_str());
	FILE* levelFile = fopen(m_levelScene->GetLevelData()->path.c_str(), "wb");
	AssertNew(levelFile);

	const int width = m_levelScene->GetMap()->GetMaxWidth();
	const int height = m_levelScene->GetMap()->GetHeight();
	
	Tile** tiles = m_levelScene->GetMap()->GetTiles();

	PE_Vec2 level_end = m_levelScene->GetLevelEnd()->GetStartPosition();
	PE_Vec2 starting_point = m_levelScene->GetStartingPoint();

	PE_Vec2 lower;
	PE_Vec2 upper;
	PE_AABB aabb;
	

	char c;
	for (int y = height; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			std::vector<GameBody*> gms;
			lower.x = x;
			lower.y = y;
			upper.x = x + 1;
			upper.y = y + 1;
			aabb.lower = lower;
			aabb.upper = upper;

			gms = m_levelScene->OverlapAreaAllBodies(aabb, CATEGORY_COLLECTABLE | CATEGORY_ENEMY);

			if (gms.size() > 0)
			{
				bool gmHasBeenPut = false;
				for (int i = 0; i < gms.size(); i++)
				{
					GameBody* gm = gms[i];

					if ((int)gm->GetPosition().x == x && (int)gm->GetPosition().y == y)
					{
						std::string gmName = gm->GetName();
						if (gmName == "Nut")
						{
							fputc('e', levelFile);
							gmHasBeenPut = true;
							break;
						}
						else if (gmName == "Firefly")
						{
							fputc('o', levelFile);
							gmHasBeenPut = true;
							break;
						}
					}
				}

				if (gmHasBeenPut)
					continue;
			}
			
			if (x == (int)level_end.x && y == (int)level_end.y) {
				fputc('F', levelFile);
				continue;
			}
			else if (x == (int)starting_point.x && y == (int)starting_point.y)
			{
				fputc('S', levelFile);
				continue;
			}
			else
			{
				char c = GetCharFromTile(tiles[x][y]);
				fputc(c, levelFile);
				continue;
			}

		}
		fputc('\n', levelFile);
	}


	fclose(levelFile);
}