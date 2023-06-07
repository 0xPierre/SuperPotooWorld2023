#pragma once

#include "Settings.h"
#include "GameBody.h"

struct Tile
{
    enum class Type : int
    {
        EMPTY, GROUND, GROUND_TOP, WOOD, ONE_WAY, SPIKE,
        STEEP_SLOPE_L, STEEP_SLOPE_R,
        GENTLE_SLOPE_L1, GENTLE_SLOPE_L2, GENTLE_SLOPE_R1, GENTLE_SLOPE_R2,
        BRICK, BONUSFULL, BONUSEMPTY, CHECKPOINTFULL, CHECKPOINTEMPTY, LEVELEND, MOVINGPLATFORM,
        FIREFLY, NUT
    };
    Type type;
    int partIdx;
    PE_Collider *collider;
};

class StaticMap : public GameBody
{
public:
    StaticMap(Scene &scene, int width, int height, bool isCreative);
    virtual ~StaticMap();

    void SetTile(int x, int y, Tile::Type type, int partIdx = 0);
    bool RemoveTile(int x, int y);
    void InitTiles();

    virtual void Render () override;
    virtual void Start() override;
    // NOT REALLY USED FOR THE MOMENT, MAYBE ONE DAY IT WILL WORKS
    virtual void AddNewTileToCollide(int x, int y);
    virtual void OnCollisionStay(GameCollision &collision) override;

    int GetMaxWidth() const;
    int GetHeight() const;
    Tile** GetTiles() const;

private:

    RE_AtlasPart *m_woodPart;
    RE_AtlasPart *m_oneWayPart;
    RE_AtlasPart *m_terrainPart;
    RE_AtlasPart *m_spikePart;
    RE_AtlasPart *m_brickPart;
    RE_AtlasPart *m_bonusFullPart;
    RE_AtlasPart *m_bonusEmptyPart;
    RE_AtlasPart *m_checkpointFullPart;
    RE_AtlasPart* m_checkpointEmptyPart;
    RE_AtlasPart *m_levelEndPart;
    RE_AtlasPart *m_movingPlatformPart;

    Tile **m_tiles;
    int m_width;
    int m_height;

    int m_max_width;
    bool m_is_creative;

    Tile::Type GetTileType(int x, int y) const;
    bool IsGround(int x, int y) const;
};


inline int StaticMap::GetMaxWidth() const
{
	return m_max_width;
}

inline int StaticMap::GetHeight() const
{
	return m_height;
}

inline Tile** StaticMap::GetTiles() const
{
	return m_tiles;
}