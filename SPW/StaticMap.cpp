#include "StaticMap.h"
#include "Camera.h"
#include "Player.h"

StaticMap::StaticMap(Scene& scene, int width, int height, bool isCreative) :
	GameBody(scene, Layer::TERRAIN_BACKGROUND), m_width(width), m_height(height), m_is_creative(isCreative)
{
	m_name = "StaticMap";
	m_max_width = 1000;

	m_tiles = new Tile * [m_max_width];
	for (int x = 0; x < m_max_width; x++)
	{
		m_tiles[x] = new Tile[height];
		for (int y = 0; y < height; y++)
		{
			Tile& tile = m_tiles[x][y];
			tile.collider = nullptr;
			tile.partIdx = 0;
			tile.type = Tile::Type::EMPTY;
		}
	}

	RE_Atlas* atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);

	m_woodPart = atlas->GetPart("Wood");
	AssertNew(m_woodPart);

	m_oneWayPart = atlas->GetPart("OneWay");
	AssertNew(m_oneWayPart);

	m_terrainPart = atlas->GetPart("Terrain");
	AssertNew(m_terrainPart);

	m_spikePart = atlas->GetPart("Spike");
	AssertNew(m_spikePart);

	m_brickPart = atlas->GetPart("Brick");
	AssertNew(m_brickPart);

	m_bonusFullPart = atlas->GetPart("BonusFull");
	AssertNew(m_bonusFullPart);

	m_bonusEmptyPart = atlas->GetPart("BonusEmpty");
	AssertNew(m_bonusEmptyPart);

	m_checkpointFullPart = atlas->GetPart("CheckpointFull");
	AssertNew(m_checkpointFullPart);

	m_checkpointEmptyPart = atlas->GetPart("CheckpointEmpty");
	AssertNew(m_checkpointEmptyPart);

	m_movingPlatformPart = atlas->GetPart("MovingPlatform");
	AssertNew(m_movingPlatformPart);


	// Couleur des colliders en debug
	m_debugColor.r = 255;
	m_debugColor.g = 200;
	m_debugColor.b = 0;
}

StaticMap::~StaticMap()
{
	if (m_tiles)
	{
		for (int x = 0; x < m_width; x++)
		{
			delete[] m_tiles[x];
		}
		delete[] m_tiles;
	}
}

void StaticMap::SetTile(int x, int y, Tile::Type type, int partIdx)
{
	//if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	//{
	//    // Disable this assert because user can place Tile outside the map in creative mode
	//    //assert(false);
	//    return;
	//}
	if (x < 0 || x >= m_max_width || y < 0 || y >= m_height)
	{
		// Disable this assert because user can place Tile outside the map in creative mode
		//assert(false);
		return;
	}
	Tile& tile = m_tiles[x][y];

	tile.partIdx = partIdx;
	tile.type = type;
}


bool StaticMap::RemoveTile(int x, int y)
{
	if (x < 0 || x >= m_max_width || y < 0 || y >= m_height)
	{
		// Disable this assert because user can place Tile outside the map in creative mode
		//assert(false);
		return false;
	}
	Tile tile = m_tiles[x][y];
	tile.partIdx = 0;
	tile.type = Tile::Type::EMPTY;

	m_tiles[x][y] = tile;

	return true;
}

void StaticMap::InitTiles()
{
	for (int x = 0; x < m_max_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			Tile& tile = m_tiles[x][y];
			Tile::Type type = GetTileType(x, y);

			switch (type)
			{
			case Tile::Type::GROUND:
				if ((GetTileType(x - 1, y) == Tile::Type::EMPTY || GetTileType(x - 1, y) == Tile::Type::SPIKE) && (GetTileType(x, y + 1) == Tile::Type::EMPTY || GetTileType(x, y + 1) == Tile::Type::SPIKE))
				{
					tile.partIdx = 0;
				}
				else if ((GetTileType(x + 1, y) == Tile::Type::EMPTY || GetTileType(x + 1, y) == Tile::Type::SPIKE) && (GetTileType(x, y + 1) == Tile::Type::EMPTY || GetTileType(x, y + 1) == Tile::Type::SPIKE))
				{
					tile.partIdx = 2;
				}
				else if (GetTileType(x, y + 1) == Tile::Type::EMPTY || GetTileType(x, y + 1) == Tile::Type::SPIKE)
				{
					tile.partIdx = 1;
				}
				else if (
					(GetTileType(x, y + 1) == Tile::Type::STEEP_SLOPE_R
						|| GetTileType(x, y + 1) == Tile::Type::GENTLE_SLOPE_R1
						|| GetTileType(x, y + 1) == Tile::Type::GENTLE_SLOPE_R2)
					&& IsGround(x - 1, y) && !(
						GetTilePartIndex(x - 1, y) == 17
						)
					)
				{
					if (GetTileType(x, y + 1) == Tile::Type::STEEP_SLOPE_R)
						tile.partIdx = 11;
					else
						tile.partIdx = 17;
				}
				else if (
					(GetTileType(x, y + 1) == Tile::Type::STEEP_SLOPE_L
						|| GetTileType(x, y + 1) == Tile::Type::GENTLE_SLOPE_L1
						|| GetTileType(x, y + 1) == Tile::Type::GENTLE_SLOPE_L2)
					&& IsGround(x + 1, y) && !(
						GetTilePartIndex(x + 1, y) == 14
						)
					)
				{
					if (GetTileType(x, y + 1) == Tile::Type::STEEP_SLOPE_L)
						tile.partIdx = 8;
					else
						tile.partIdx = 14;
				}
				/*else if (
					(GetTileType(x, y + 1) == Tile::Type::STEEP_SLOPE_L
						|| GetTileType(x, y + 1) == Tile::Type::GENTLE_SLOPE_L1
						|| GetTileType(x, y + 1) == Tile::Type::GENTLE_SLOPE_L2)
					&& (GetTileType(x + 1, y) == Tile::Type::GROUND)
					&& GetTilePartIndex(x + 1, y) == 1
					)
				{
					tile.partIdx = 8;
				}*/

				/*else if (IsGround(x, y + 1))
				{
					tile.partIdx = 4;
				}*/

				else
				{
					tile.partIdx = 4;
				}
				break;

			default:
				//tile.partIdx = 0;
				break;
			}
		}
	}
}

void StaticMap::Render()
{
	SDL_Renderer* renderer = m_scene.GetRenderer();
	Camera* camera = m_scene.GetActiveCamera();

	PE_AABB view = camera->GetWorldView();
	int x0 = (int)view.lower.x - 1;
	int y0 = (int)view.lower.y - 1;
	int x1 = (int)view.upper.x + 2;
	int y1 = (int)view.upper.y + 2;

	x0 = PE_Max(x0, 0);
	y0 = PE_Max(y0, 0);
	x1 = PE_Min(x1, m_max_width);
	y1 = PE_Min(y1, m_height);

	for (int x = x0; x < x1; ++x)
	{
		for (int y = y0; y < y1; ++y)
		{
			Tile& tile = m_tiles[x][y];
			PE_Collider* collider = tile.collider;

			PE_Vec2 position((float)x, (float)y);
			SDL_FRect dst = { 0 };

			camera->WorldToView(position, dst.x, dst.y);
			float scale = camera->GetWorldToViewScale();
			dst.w = scale * 1.0f;
			dst.h = scale * 1.0f;

			switch (tile.type)
			{
			case Tile::Type::GROUND:
			case Tile::Type::STEEP_SLOPE_L:
			case Tile::Type::STEEP_SLOPE_R:
			case Tile::Type::GENTLE_SLOPE_L1:
			case Tile::Type::GENTLE_SLOPE_L2:
			case Tile::Type::GENTLE_SLOPE_R1:
			case Tile::Type::GENTLE_SLOPE_R2:
				m_terrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
				break;
			case Tile::Type::WOOD:
				m_woodPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
				break;
			case Tile::Type::ONE_WAY:
				m_oneWayPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
				break;
			case Tile::Type::SPIKE:
				m_spikePart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
				break;
			case Tile::Type::BONUSFULL:
				m_bonusFullPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
				break;
			case Tile::Type::BONUSEMPTY:
				m_bonusEmptyPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
				break;
			case Tile::Type::CHECKPOINTFULL:
				m_checkpointFullPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
				break;
			case Tile::Type::CHECKPOINTEMPTY:
				m_checkpointEmptyPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
				break;
			case Tile::Type::MOVINGPLATFORM:
				m_movingPlatformPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
				break;
			default:
				break;
			}
		}
	}
}

void StaticMap::Start()
{
	PE_World& world = m_scene.GetWorld();
	PE_Body* body = NULL;

	// Crée le corps
	PE_BodyDef bodyDef;
	bodyDef.type = PE_BodyType::STATIC;
	bodyDef.position.SetZero();
	bodyDef.name = (char*)"StaticMap";
	body = world.CreateBody(bodyDef);
	AssertNew(body);
	SetBody(body);

	// Crée les colliders
	PE_Vec2 vertices[3];
	PE_PolygonShape polygon;
	PE_ColliderDef colliderDef;

	for (int x = 0; x < m_max_width; ++x)
	{
		for (int y = 0; y < m_height; ++y)
		{
			Tile& tile = m_tiles[x][y];
			if (tile.type == Tile::Type::EMPTY)
			{
				continue;
			}

			PE_Vec2 position((float)x, (float)y);
			bool newCollider = true;
			colliderDef.SetDefault();
			colliderDef.shape = &polygon;
			colliderDef.friction = 0.5f;
			colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
			colliderDef.userData.id = 0;

			switch (tile.type)
			{
			case Tile::Type::ONE_WAY:
				colliderDef.isOneWay = true;
				polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
				break;

			case Tile::Type::GROUND:
			case Tile::Type::WOOD:
				polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
				break;

			case Tile::Type::STEEP_SLOPE_L:
				colliderDef.filter.categoryBits = CATEGORY_SLOPE;
				vertices[0] = position + PE_Vec2(0.0f, 0.0f);
				vertices[1] = position + PE_Vec2(1.0f, 0.0f);
				vertices[2] = position + PE_Vec2(0.0f, 1.0f);
				polygon.SetVertices(vertices, 3);
				break;
			case Tile::Type::STEEP_SLOPE_R:
				colliderDef.filter.categoryBits = CATEGORY_SLOPE;
				vertices[0] = position + PE_Vec2(0.0f, 0.0f);
				vertices[1] = position + PE_Vec2(1.0f, 0.0f);
				vertices[2] = position + PE_Vec2(1.0f, 1.0f);
				polygon.SetVertices(vertices, 3);
				break;

			case Tile::Type::GENTLE_SLOPE_R1:
				colliderDef.filter.categoryBits = CATEGORY_SLOPE;
				vertices[0] = position + PE_Vec2(0.0f, 0.0f);
				vertices[1] = position + PE_Vec2(1.0f, 0.0f);
				vertices[2] = position + PE_Vec2(1.0f, 0.5f);
				polygon.SetVertices(vertices, 3);
				break;

			case Tile::Type::GENTLE_SLOPE_R2:
				colliderDef.filter.categoryBits = CATEGORY_SLOPE;
				vertices[0] = position + PE_Vec2(0.0f, 0.5f);
				vertices[1] = position + PE_Vec2(1.0f, 0.0f);
				vertices[2] = position + PE_Vec2(1.0f, 1.0f);
				polygon.SetVertices(vertices, 3);
				break;

			case Tile::Type::GENTLE_SLOPE_L1:
				colliderDef.filter.categoryBits = CATEGORY_SLOPE;
				vertices[0] = position + PE_Vec2(0.0f, 0.5f);
				vertices[1] = position + PE_Vec2(0.0f, 0.0f);
				vertices[2] = position + PE_Vec2(1.0f, 0.0f);
				polygon.SetVertices(vertices, 3);
				break;

			case Tile::Type::GENTLE_SLOPE_L2:
				colliderDef.filter.categoryBits = CATEGORY_SLOPE;
				vertices[0] = position + PE_Vec2(0.0f, 0.0f);
				vertices[1] = position + PE_Vec2(1.0f, 0.5f);
				vertices[2] = position + PE_Vec2(0.0f, 1.0f);
				polygon.SetVertices(vertices, 3);
				break;

			case Tile::Type::SPIKE:
				colliderDef.userData.id = 1;

				vertices[0] = position + PE_Vec2(0.1f, 0.0f);
				vertices[1] = position + PE_Vec2(0.9f, 0.0f);
				vertices[2] = position + PE_Vec2(0.5f, 0.8f);
				polygon.SetVertices(vertices, 3);
				break;

			default:
				newCollider = false;
				break;
			}
			if (newCollider)
			{
				tile.collider = body->CreateCollider(colliderDef);
				AssertNew(tile.collider);
			}
			else
			{
				tile.collider = nullptr;
			}
		}
	}

	// Limite à gauche du monde
	polygon.SetAsBox(-1.0f, -2.0f, 0.0f, (float)m_height + 10.0f);
	colliderDef.SetDefault();
	colliderDef.friction = 0.0f;
	colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
	colliderDef.shape = &polygon;
	body->CreateCollider(colliderDef);

	if (!m_is_creative)
	{
		// Limite à droite du monde
		polygon.SetAsBox((float)m_width, -2.0f, (float)m_width + 1.0f, (float)m_height + 10.0f);
		colliderDef.SetDefault();
		colliderDef.friction = 0.0f;
		colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
		colliderDef.shape = &polygon;
		body->CreateCollider(colliderDef);
	}
}

void StaticMap::AddNewTileToCollide(int x, int y)
{
	//PE_World& world = m_scene.GetWorld();
	//PE_Body *body = GetBody();

	//// Crée les colliders
	//PE_Vec2 vertices[3];
	//PE_PolygonShape polygon;
	//PE_ColliderDef colliderDef;

	//Tile& tile = m_tiles[x][y];

	//if (tile.type != Tile::Type::EMPTY)
	//{
	//    PE_Vec2 position((float)x, (float)y);
	//    bool newCollider = true;
	//    colliderDef.SetDefault();
	//    colliderDef.shape = &polygon;
	//    colliderDef.friction = 0.5f;
	//    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
	//    colliderDef.userData.id = 0;

	//    switch (tile.type)
	//    {
	//    case Tile::Type::ONE_WAY:
	//        colliderDef.isOneWay = true;
	//        polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
	//        break;

	//    case Tile::Type::GROUND:
	//    case Tile::Type::WOOD:
	//        polygon.SetAsBox(PE_AABB(position, position + PE_Vec2(1.0f, 1.0f)));
	//        break;

	//    case Tile::Type::SPIKE:
	//        colliderDef.userData.id = 1;

	//        vertices[0] = position + PE_Vec2(0.1f, 0.0f);
	//        vertices[1] = position + PE_Vec2(0.9f, 0.0f);
	//        vertices[2] = position + PE_Vec2(0.5f, 0.8f);
	//        polygon.SetVertices(vertices, 3);
	//        break;

	//    default:
	//        newCollider = false;
	//        break;
	//    }
	//    if (newCollider)
	//    {
	//        tile.collider = body->CreateCollider(colliderDef);
	//        AssertNew(tile.collider);
	//    }
	//    else
	//    {
	//        tile.collider = nullptr;
	//    }
	//}
}

void StaticMap::OnCollisionStay(GameCollision& collision)
{
	// On vérifie que la collision concerne une pique
	if (collision.collider->GetUserData().id != 1)
		return;

	if (collision.otherCollider->CheckCategory(CATEGORY_PLAYER))
	{
		Player* player = dynamic_cast<Player*>(collision.gameBody);
		if (player == nullptr)
		{
			assert(false);
			return;
		}

		player->Damage();
		player->Bounce();
	}
}

Tile::Type StaticMap::GetTileType(int x, int y) const
{
	if (x < 0 || x >= m_width || y < 0)
		return Tile::Type::GROUND;
	else if (y >= m_height)
		return Tile::Type::EMPTY;
	else
		return m_tiles[x][y].type;
}

int StaticMap::GetTilePartIndex(int x, int y) const
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return 0;
	else
		return m_tiles[x][y].partIdx;
}

bool StaticMap::IsGround(int x, int y) const
{
	switch (GetTileType(x, y))
	{
	case Tile::Type::GROUND:
	case Tile::Type::STEEP_SLOPE_L:
	case Tile::Type::STEEP_SLOPE_R:
	case Tile::Type::GENTLE_SLOPE_L1:
	case Tile::Type::GENTLE_SLOPE_L2:
	case Tile::Type::GENTLE_SLOPE_R1:
	case Tile::Type::GENTLE_SLOPE_R2:
		return true;
	default:
		return false;
	}
}

//char StaticMap::GetTileChar(int x, int y) const
//{
//	if (x < 0 || x >= m_max_width || y < 0 || y >= m_height)
//		return ' ';
//	else
//		return m_tiles[x][y].type;
//}	