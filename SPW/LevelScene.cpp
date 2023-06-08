#include "LevelScene.h"
#include "LevelParser.h"
#include "MainCamera.h"
#include "DebugCamera.h"
#include "Background.h"
#include "StaticMap.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

LevelScene::LevelScene(SDL_Renderer *renderer, RE_Timer &mainTime, LevelData &level, bool isCreative, bool isNewWorld) :
    Scene(renderer, mainTime, level.themeID), m_paused(false),
    m_camIndex(0), m_cameras(), m_stepDelay(0.0f)
{
    m_inputManager.GetApplication().SetEnabled(true);
    m_inputManager.GetMouse().SetEnabled(true);
    m_inputManager.GetControls().SetEnabled(true);
    m_inputManager.GetDebug().SetEnabled(true);
    m_creative = new Creative(*this);

    SetCreative(isCreative);
    SetNewWorld(isNewWorld);

    m_player = new Player(*this);
    m_player->SetName("Player");
    m_map = NULL;

    // Crée les caméras
    m_cameras[0] = new MainCamera(*this);
    m_cameras[1] = new DebugCamera(*this, *m_cameras[0]);
    m_activeCam = m_cameras[m_camIndex];

    // Parse le niveau
    std::string path = level.path;
    
    if (m_is_new_world) {
        LevelParser parser("../Assets/Level/Creative/NewWorldStarter.txt");
        std::string newPath;
        int i = 1;
        std::string newName;
        while (true)
        {
            std::string newName = "New World " + std::to_string(i);
            newPath = "../Assets/Level/" + newName + ".txt";
            if (!fs::exists(newPath))
            {
                break;
            }
            i++;
        }
        parser.InitScene(*this);
        LevelData* level = new LevelData(newName, newPath, ThemeID::LAKE);
        m_levelData = level;
    }
    else {
        LevelParser parser(level.path);
        parser.InitScene(*this);

        m_levelData = &level;
    }
    this->GetMap()->InitTiles();

    // Canvas
    m_canvas = new LevelCanvas(*this);
    m_current_theme_id = level.themeID;
    UpdateBackground();
}

LevelScene::~LevelScene()
{
}

void LevelScene::UpdateBackground()
{
    m_background = new Background(*this, Layer::BACKGROUND);
    std::vector<SDL_Texture*> m_textures = m_assetManager.GetBackgrounds();
    switch (GetLevelData()->themeID)
    {
    case ThemeID::LAKE:
    {
        PE_Vec2 worldDim(24.0f, 24.0f * 1080.0f / 1920.0f);
        m_background->SetWorldDimensions(worldDim);
        float factors[] = { 0.0f, 0.05f, 0.3f, 0.6f, 0.7f };
        for (int i = 0; i < 5; i++)
        {
            m_background->SetTexture(i, m_textures[i], PE_Vec2(factors[i], factors[i]));
        }
        break;
    }

    case ThemeID::SKY:
    {
        PE_Vec2 worldDim(24.0f, 24.0f * 1080.0f / 1920.0f);
        m_background->SetWorldDimensions(worldDim);
        float factors[] = { 0.0f, 0.05f, 0.1f, 0.2f, 0.35f, 0.5f, 0.7f };
        for (int i = 0; i < 7; i++)
        {
            m_background->SetTexture(i, m_textures[i], PE_Vec2(factors[i], factors[i]));
        }
        break;
    }

    case ThemeID::MOUNTAINS:
    default:
    {
        PE_Vec2 worldDim(36.0f, 36.0f * 1080.0f / 2880.0f);
        m_background->SetWorldDimensions(worldDim);
        float factors[] = { 0.0f, 0.05f, 0.3f, 0.6f };
        for (int i = 0; i < 4; i++)
        {
            m_background->SetTexture(i, m_textures[i], PE_Vec2(factors[i], factors[i]));
        }

        m_foreground = new Background(*this, Layer::FOREGROUND);
        worldDim.Set(36.0f, 36.0f * 400.0f / 2880.0f);
        m_foreground->SetWorldDimensions(worldDim);
        m_foreground->SetTexture(0, m_textures[4], PE_Vec2(1.4f, 1.4f));
        break;
    }
    }
}

inline void LevelScene::SetPaused(bool isPaused)
{
    if (isPaused == m_paused)
        return;

    if (isPaused)
    {
        m_time.SetTimeScale(0.0f);
        m_paused = true;
        m_inputManager.GetControls().SetEnabled(false);
        m_canvas->OpenPauseMenu();
    }
    else
    {
        m_time.SetTimeScale(1.0f);
        m_paused = false;
        m_inputManager.GetControls().SetEnabled(true);
        m_canvas->ClosePauseMenu();
    }
}

bool LevelScene::Update()
{
    // Appelle la méthode de la classe mère
    bool quit = Scene::Update();

    ApplicationInput &appInput = m_inputManager.GetApplication();

    if (appInput.quitPressed)
    {
        return true;
    }

    if (appInput.pausePressed)
    {
        SetPaused(!m_paused);
    }

    DebugInput &debugInput = m_inputManager.GetDebug();

    if (debugInput.bodyPressed)
    {
        m_drawPhysics = !(m_drawPhysics);
    }
    if (debugInput.gizmosPressed)
    {
        m_drawGizmos = !(m_drawGizmos);
    }
    if (debugInput.idPressed)
    {
        m_drawID = !(m_drawID);
    }
    if (debugInput.camPressed)
    {
        m_camIndex = (m_camIndex + 1) % 2;
        m_activeCam = m_cameras[m_camIndex];
    }


    if (m_paused == false)
    {
        if (m_mode != UpdateMode::STEP_BY_STEP && debugInput.nextStepPressed)
        {
            m_mode = UpdateMode::STEP_BY_STEP;
        }
        else if (debugInput.quitStepPressed)
        {
            m_mode = UpdateMode::REALTIME;
        }

        // Mode pas-à-pas
        if (m_mode == UpdateMode::STEP_BY_STEP)
        {
            if (debugInput.nextStepPressed)
            {
                m_stepDelay = 0.5f;
                m_makeStep = true;
            }
            else if (debugInput.nextStepDown)
            {
                m_stepDelay -= m_time.GetUnscaledDelta();
            }

            if (m_stepDelay < 0.0f)
            {
                m_makeStep = true;
                m_stepDelay = 1.0f / 10.0f;
            }
        }
    }

    if (m_current_theme_id != m_levelData->themeID)
    {
        m_current_theme_id = m_levelData->themeID;
        m_background->SetEnabled(false);
        SetAssetManager(m_levelData->themeID);
		UpdateBackground();
	}

    return quit;
}

void LevelScene::OnRespawn()
{
    SetPaused(false);
}
