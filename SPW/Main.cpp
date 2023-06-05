#include "Settings.h"
#include "LevelScene.h"
#include "TitleScene.h"

//#define FHD
//#define FULLSCREEN
#define SKIP_MENU
#define DEFAUT_LEVEL 0

#ifdef FHD
#define WINDOW_WIDTH   FHD_WIDTH
#define WINDOW_HEIGHT  FHD_HEIGHT
#else
#define WINDOW_WIDTH   HD_WIDTH
#define WINDOW_HEIGHT  HD_HEIGHT
#endif

#define LOGICAL_WIDTH  FHD_WIDTH
#define LOGICAL_HEIGHT FHD_HEIGHT

enum class GameState
{
    MAIN_MENU, LEVEL
};

int main(int argc, char *argv[])
{
    // Initialise la lib RE (SDL2, SDL2 image, SDL2 ttf)
    RE_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK, IMG_INIT_PNG);

    // Initialise la SDL2 mixer
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3)
    {
        printf("ERROR - Mix_Init %s\n", Mix_GetError());
        assert(false);
        abort();
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        printf("ERROR - Mix_OpenAudio %s\n", Mix_GetError());
        assert(false);
        abort();
    }
    Mix_AllocateChannels(16);

    int exitStatus;
    exitStatus = Mix_MasterVolume((int)(0.50f * MIX_MAX_VOLUME));
    exitStatus = Mix_VolumeMusic((int)(0.70f * MIX_MAX_VOLUME));
    exitStatus = Mix_Volume((int)ChannelID::COLLECTABLE, (int)(0.40f * MIX_MAX_VOLUME));
    exitStatus = Mix_Volume((int)ChannelID::ENEMY, (int)(0.50f * MIX_MAX_VOLUME));
    exitStatus = Mix_Volume((int)ChannelID::PLAYER, (int)(0.50f * MIX_MAX_VOLUME));
    exitStatus = Mix_Volume((int)ChannelID::SYSTEM_1, (int)(0.40f * MIX_MAX_VOLUME));
    exitStatus = Mix_Volume((int)ChannelID::SYSTEM_2, (int)(0.60f * MIX_MAX_VOLUME));

    Mix_Music *music = Mix_LoadMUS("../Assets/Music/main.mp3");
    Mix_PlayMusic(music, -1);

    // Crée la fenêtre
    Uint32 sdlFlags = 0;
#ifdef FULLSCREEN
    sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif

    SDL_Window *window = SDL_CreateWindow(
        u8"Super Potoo World ++", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, sdlFlags
    );

    if (!window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false); abort();
    }

    // Crée le moteur de rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (!renderer)
    {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        assert(false); abort();
    }

    // Vérifie s'il y a un joystick
    SDL_Joystick *gameController = nullptr;
    if (SDL_NumJoysticks() >= 1)
    {
        gameController = SDL_JoystickOpen(0);
    }

    // Crée le temps global du jeu
    RE_Timer time;
    time.Start();

    // Boucle de jeu
    Scene *scene = nullptr;
    bool quitGame = false;
    GameState state = GameState::MAIN_MENU;
    const std::vector<LevelData> levels(LevelData::Init());
    int levelID = 0;

#ifdef SKIP_MENU
    state = GameState::LEVEL;
    levelID = DEFAUT_LEVEL;
#endif

    while (quitGame == false)
    {
        time.SetTimeScale(1.0f);

        // Construction de la scène
        switch (state)
        {
        case GameState::LEVEL:
            assert(0 <= levelID && levelID < levels.size());
            scene = new LevelScene(renderer, time, levels[levelID]);
            break;

        case GameState::MAIN_MENU:
        default:
            scene = new TitleScene(renderer, time, levels);
            break;
        }

        // Boucle de rendu
        while (true)
        {
            // Met à jour le temps
            time.Update();

            // Met à jour la scène
            bool quit = scene->Update();

            if (scene->GetInputManager().GetApplication().quitPressed)
            {
                quitGame = true;
                break;
            }
            if (quit)
            {
                break;
            }

            // Efface le rendu précédent
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Dessine la scène
            scene->Render();

            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }

        switch (state)
        {
        case GameState::LEVEL:
            state = GameState::MAIN_MENU;
            break;

        case GameState::MAIN_MENU:
        default:
            levelID = ((TitleScene *)scene)->GetLevelID();
            if (levelID < 0)
            {
                quitGame = true;
            }
            state = GameState::LEVEL;
            break;
        }

        if (scene)
        {
            delete scene;
            scene = nullptr;
        }
    }

    if (gameController)
    {
        SDL_JoystickClose(gameController);
        gameController = nullptr;
    }
    if (music)
    {
        Mix_FreeMusic(music);
        music = nullptr;
    }

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    Mix_Quit();
    RE_Quit();

    return 0;
}
