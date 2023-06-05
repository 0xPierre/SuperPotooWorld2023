#include "AssetManager.h"

struct AudioSpec
{
    SoundID soundID;
    const char *path;

    AudioSpec(SoundID soundID, const char *path);
};

AudioSpec::AudioSpec(SoundID soundIDIn, const char *pathIn) :
    soundID(soundIDIn), path(pathIn)
{
}


AssetManager::AssetManager(SDL_Renderer *renderer, ThemeID themeID) :
    m_renderer(renderer), m_theme(themeID),
    m_atlases(), m_fonts(), m_backgrounds(), m_sounds()
{
    std::array<AudioSpec, 12> audioSpecs{
        AudioSpec(SoundID::BRICK,           "../Assets/SoundFX/Brick.wav"),
        AudioSpec(SoundID::COLLECT_FIREFLY, "../Assets/SoundFX/CollectFirefly.wav"),
        AudioSpec(SoundID::COLLECT_ITEM,    "../Assets/SoundFX/CollectItem.wav"),
        AudioSpec(SoundID::ENEMY_JUMP,      "../Assets/SoundFX/EnemyJump.wav"),
        AudioSpec(SoundID::ENEMY_DOWN,      "../Assets/SoundFX/EnemyDown.wav"),
        AudioSpec(SoundID::HEART_APPEAR,    "../Assets/SoundFX/HeartAppear.wav"),
        AudioSpec(SoundID::PLAYER_HIT,      "../Assets/SoundFX/PlayerHit.wav"),
        AudioSpec(SoundID::PLAYER_JUMP,     "../Assets/SoundFX/PlayerJump.wav"),
        AudioSpec(SoundID::PLAYER_LAND,     "../Assets/SoundFX/PlayerLand.wav"),
        AudioSpec(SoundID::SYSTEM_HOVER,    "../Assets/SoundFX/SystHover.wav"),
        AudioSpec(SoundID::SYSTEM_PAUSE,    "../Assets/SoundFX/SystPause.wav"),
        AudioSpec(SoundID::SYSTEM_SELECT,   "../Assets/SoundFX/SystSelect.wav"),
    };

    for (int i = 0; i < audioSpecs.size(); i++)
    {
        Mix_Chunk *chunck = Mix_LoadWAV(audioSpecs[i].path);
        if (chunck == nullptr)
        {
            printf("ERROR - Loading audio %s\n", audioSpecs[i].path);
            printf("      - %s\n", Mix_GetError());
            assert(false);
            abort();
        }

        m_sounds.insert(std::make_pair(audioSpecs[i].soundID, chunck));
    }
}

AssetManager::~AssetManager()
{
    for (auto it = m_atlases.begin(); it != m_atlases.end(); ++it)
    {
        RE_Atlas *atlas = it->second;
        if (atlas) delete atlas;
    }
    for (auto it = m_fonts.begin(); it != m_fonts.end(); ++it)
    {
        TTF_Font *font = it->second;
        if (font) TTF_CloseFont(font);
    }
    for (SDL_Texture *texture : m_backgrounds)
    {
        if (texture) SDL_DestroyTexture(texture);
    }
    for (auto it = m_sounds.begin(); it != m_sounds.end(); ++it)
    {
        Mix_Chunk *chunck = it->second;
        if (chunck) Mix_FreeChunk(chunck);
    }
}

RE_Atlas *AssetManager::GetAtlas(AtlasID atlasID)
{
    RE_Atlas *atlas = nullptr;

    auto it = m_atlases.find(atlasID);
    if (it != m_atlases.end())
    {
        // L'asset est déjà chargé en mémoire
        atlas = it->second;
    }
    else
    {
        // L'asset n'est pas chargé en mémoire
        std::string path;
        switch (atlasID)
        {
        case AtlasID::COLLECTABLE:
            path.assign("../Assets/Atlas/Collectable.json");
            break;
        case AtlasID::ENEMY:
            path.assign("../Assets/Atlas/Enemy.json");
            break;
        case AtlasID::LOGO:
            path.assign("../Assets/Atlas/Logo.json");
            break;
        case AtlasID::PLAYER:
            path.assign("../Assets/Atlas/Player.json");
            break;
        case AtlasID::TERRAIN:
            path.assign("../Assets/Atlas/Terrain.json");
            break;
        case AtlasID::UI:
            path.assign("../Assets/Atlas/UI.json");
            break;
        default:
            assert(false);
            break;
        }
        atlas = new RE_Atlas(m_renderer, path);
        m_atlases.insert(std::make_pair(atlasID, atlas));
    }
    return atlas;
}

TTF_Font *AssetManager::GetFont(FontID fontID)
{
    TTF_Font *font = nullptr;

    auto it = m_fonts.find(fontID);
    if (it != m_fonts.end())
    {
        // L'asset est déjà chargé en mémoire
        font = it->second;
    }
    else
    {
        // L'asset n'est pas chargé en mémoire
        int size = 14;
        switch (fontID)
        {
        case FontID::SMALL:
            size = 18;
            break;
        case FontID::NORMAL:
            size = 28;
            break;
        case FontID::LARGE:
            size = 64;
            break;
        default:
            break;
        }
        font = TTF_OpenFont(
            "../Assets/Font/Berlin_Sans_FB_Demi_Bold.ttf",
            size
        );
        AssertNew(font);
        m_fonts.insert(std::make_pair(fontID, font));
    }
    return font;
}

SDL_Color AssetManager::GetColor(ColorID colorID) const
{
    SDL_Color color = { 0 };
    color.a = 255;

    switch (colorID)
    {
    case ColorID::BLACK:
        color.r = 0;
        color.g = 0;
        color.b = 0;
        break;
    case ColorID::WHITE:
        color.r = 255;
        color.g = 255;
        color.b = 255;
        break;
    case ColorID::NORMAL:
        color.r = 252;
        color.g = 242;
        color.b = 225;
        break;
    default:
        break;
    }
    return color;
}

std::vector<SDL_Texture *> &AssetManager::GetBackgrounds()
{
    if (m_backgrounds.empty())
    {
        std::string folder;
        int layerCount = 0;
        switch (m_theme)
        {
        case ThemeID::LAKE:
            folder.assign("../Assets/Theme_Lake");
            layerCount = 5;
            break;
        case ThemeID::SKY:
            folder.assign("../Assets/Theme_Sky");
            layerCount = 7;
            break;
        case ThemeID::MOUNTAINS:
        default:
            layerCount = 5;
            folder.assign("../Assets/Theme_Mountains");
            break;
        }

        for (int i = 0; i < layerCount; ++i)
        {
            std::string path(folder);
            path.append("/Layer_" + std::to_string(i) + ".png");
            SDL_Texture *texture = IMG_LoadTexture(m_renderer, path.c_str());
            if (texture == nullptr)
            {
                printf("ERROR - IMG_LoadTexture %s\n", SDL_GetError());
                assert(false); abort();
            }
            m_backgrounds.push_back(texture);
        }
    }
    return m_backgrounds;
}

void AssetManager::PlaySound(SoundID soundID, ChannelID channelID)
{
    int exitStatus = Mix_PlayChannel((int)channelID, m_sounds[soundID], 0);
}
