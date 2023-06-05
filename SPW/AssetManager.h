#pragma once

#include "Settings.h"

enum class AtlasID : uint32_t
{
    COLLECTABLE, ENEMY, LOGO, PLAYER, TERRAIN, UI
};
enum class FontID : uint32_t
{
    SMALL, NORMAL, LARGE
};
enum class ColorID : uint32_t
{
    BLACK, WHITE, NORMAL
};
enum class ThemeID : uint32_t
{
    MOUNTAINS, LAKE, SKY
};

enum class SoundID : uint32_t
{
    PLAYER_JUMP, PLAYER_LAND, PLAYER_HIT,
    ENEMY_JUMP, ENEMY_DOWN,
    COLLECT_FIREFLY, COLLECT_ITEM, HEART_APPEAR,
    BRICK,
    SYSTEM_HOVER, SYSTEM_PAUSE, SYSTEM_SELECT
};
enum class ChannelID : uint32_t
{
    PLAYER, COLLECTABLE, ENEMY, SYSTEM_1, SYSTEM_2
};

class AssetManager
{
public:
    AssetManager(SDL_Renderer *renderer, ThemeID themeID);
    AssetManager(AssetManager const&) = delete;
    AssetManager& operator=(AssetManager const&) = delete;
    ~AssetManager();

    RE_Atlas *GetAtlas(AtlasID atlasID);
    TTF_Font *GetFont(FontID fontID);
    SDL_Color GetColor(ColorID colorID) const;
    std::vector<SDL_Texture *> &GetBackgrounds();

    void PlaySound(SoundID soundID, ChannelID channelID);

private:
    std::map<AtlasID, RE_Atlas *> m_atlases;
    std::map<FontID, TTF_Font *> m_fonts;
    std::vector<SDL_Texture *> m_backgrounds;
    std::map<SoundID, Mix_Chunk *> m_sounds;

    ThemeID m_theme;
    std::string m_themeFolder;
    SDL_Renderer *m_renderer;
};