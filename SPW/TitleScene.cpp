#include "TitleScene.h"
#include "TitleCamera.h"
#include "TitleCanvas.h"
#include "Background.h"

TitleScene::TitleScene(SDL_Renderer *renderer, RE_Timer &mainTime, const std::vector<LevelData> &levels) :
    Scene(renderer, mainTime, ThemeID::MOUNTAINS),
    m_levels(levels), m_selectedLevel(-1)
{
    m_inputManager.GetApplication().SetEnabled(true);
    m_inputManager.GetMouse().SetEnabled(true);
    m_inputManager.GetControls().SetEnabled(false);
    m_inputManager.GetDebug().SetEnabled(false);

    // Crée la caméra
    m_activeCam = new TitleCamera(*this);

    // Canvas
    m_canvas = new TitleCanvas(*this);

    // Crée le fond
    Background *background = new Background(*this, Layer::BACKGROUND);
    auto m_textures = m_assetManager.GetBackgrounds();
    PE_Vec2 worldDim(36.0f, 36.0f * 1080.0f / 2880.0f);
    background->SetWorldDimensions(worldDim);
    float factors[] = { 0.0f, 0.05f, 0.3f, 0.6f };

    assert(m_textures.size() == 5);
    for (int i = 0; i < 4; i++)
    {
        background->SetTexture(i, m_textures[i], PE_Vec2(factors[i], factors[i]));
    }

    Background *foreground = new Background(*this, Layer::FOREGROUND);
    worldDim.Set(36.0f, 36.0f * 400.0f / 2880.0f);
    foreground->SetWorldDimensions(worldDim);
    foreground->SetTexture(0, m_textures[4], PE_Vec2(1.4f, 1.4f));
}

TitleScene::~TitleScene()
{
}
