#include "LevelScene.h"
#include "Creative.h"
#include "StaticMap.h"
#include "Camera.h"
#include "ControlsInput.h"
#include "ObjectManager.h"
#include "Firefly.h"
#include "Nut.h"
#include "Brick.h"
#include "Bonus.h"

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

	std::vector<GameBody*> gms;
	gms = m_levelScene->OverlapAreaAllBodies(aabb, CATEGORY_COLLECTABLE | CATEGORY_ENEMY | CATEGORY_TERRAIN_ENTITY);

	if (gms.size() > 0)
	{
		bool gmHasBeenRemoved = false;
		for (int i = 0; i < gms.size(); i++)
		{
			GameBody* gm = gms[i];

			if ((int)gm->GetPosition().x == lower.x && (int)gm->GetPosition().y == lower.y)
			{
				gm->SetEnabled(false);
				gmHasBeenRemoved = true;
			}
		}
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
	case Tile::Type::BONUSEMPTY:
	case Tile::Type::CHECKPOINTFULL:
	case Tile::Type::CHECKPOINTEMPTY:
		
		m_levelScene->GetMap()->SetTile(Pos.x, Pos.y, tileType, groundSelected);
		m_levelScene->GetMap()->AddTileCollider(Pos.x, Pos.y);
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
		break;
	case Tile::Type::BRICK:
	{
		Brick* brick = new Brick(*m_levelScene);
		Pos.x = (int)Pos.x;
		Pos.y = (int)Pos.y;
		brick->SetStartPosition(Pos);
		break;
	}
	case Tile::Type::BONUSFULL:
	{
		Bonus* bonus = new Bonus(*m_levelScene);
		Pos.x = (int)Pos.x;
		Pos.y = (int)Pos.y;
		bonus->SetStartPosition(Pos);
		break;
	}
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

	std::vector<GameBody*> gms;
	gms = m_levelScene->OverlapAreaAllBodies(aabb, CATEGORY_COLLECTABLE | CATEGORY_ENEMY | CATEGORY_TERRAIN_ENTITY);

	if (gms.size() > 0)
	{
		bool gmHasBeenRemoved = false;
		for (int i = 0; i < gms.size(); i++)
		{
			GameBody* gm = gms[i];

			if ((int)gm->GetPosition().x == lower.x && (int)gm->GetPosition().y == lower.y)
			{
				gm->SetEnabled(false);
				gmHasBeenRemoved = true;
			}
		}
	}

	m_levelScene->GetMap()->RemoveTile(Pos.x, Pos.y);
}

Tile::Type Creative::SelectItem(MouseInput& mouse)
{
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

	std::vector<GameBody*> gms;
	gms = m_levelScene->OverlapAreaAllBodies(aabb, CATEGORY_COLLECTABLE | CATEGORY_ENEMY | CATEGORY_TERRAIN_ENTITY | CATEGORY_TERRAIN);

	if (gms.size() > 0)
	{
		bool gmHasBeenSelected = false;
		int i;
		for (i=0; i < gms.size(); i++)
		{
			GameBody* gm = gms[i];

			if ((int)gm->GetPosition().x == lower.x && (int)gm->GetPosition().y == lower.y)
			{
				gmHasBeenSelected = true;
				break;
			}
		}
		if (gmHasBeenSelected) {
			std::string gmName = gms[i]->GetName();
			if (gmName == "Nut")
			{
				return Tile::Type::NUT;
			}
			else if (gmName == "Firefly")
			{
				return Tile::Type::FIREFLY;
			}
			else if (gmName == "Brick")
			{
				return Tile::Type::BRICK;
			}
			else if (gmName == "Bonus")
			{
				return Tile::Type::BONUSFULL;
			}
		}
	}

	return m_levelScene->GetMap()->GetTileType(Pos.x, Pos.y);
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
		return 'R';
	case Tile::Type::GENTLE_SLOPE_R2:
		return 'r';
	case Tile::Type::GENTLE_SLOPE_L1:
		return 'L';
	case Tile::Type::GENTLE_SLOPE_L2:
		return 'l';
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

			gms = m_levelScene->OverlapAreaAllBodies(aabb, CATEGORY_COLLECTABLE | CATEGORY_ENEMY | CATEGORY_TERRAIN_ENTITY);

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
						else if (gmName == "Brick")
						{
							fputc('B', levelFile);
							gmHasBeenPut = true;
							break;
						}
						else if (gmName == "Bonus")
						{
							fputc('?', levelFile);
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

	if (m_levelScene->GetLevelData()->themeID == ThemeID::MOUNTAINS)
		fwrite("\nbg:MOUNTAINS\n", 1, 14, levelFile);
	else if (m_levelScene->GetLevelData()->themeID == ThemeID::SKY)
		fwrite("\nbg:SKY\n", 1, 8, levelFile);
	else
		fwrite("\nbg:LAKE\n", 1, 9, levelFile);

	fclose(levelFile);
}