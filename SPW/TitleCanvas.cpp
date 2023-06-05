#include "TitleCanvas.h"
#include "TitleScene.h"
#include "StartScreen.h"

TitleCanvas::TitleCanvas(TitleScene &scene) :
    UIObject(scene)
{
    m_name = "TitleCanvas";
    SetToRespawn(true);
    SetGizmosFillOpacity(0);

    SDL_Renderer *renderer = m_scene.GetRenderer();
    int width, height;
    SDL_RenderGetLogicalSize(renderer, &width, &height);

    m_rect.offsetMin.Set(0.0f, 0.0f);
    m_rect.offsetMax.Set((float)width, (float)height);
    m_rect.anchorMin.Set(0.0f, 0.0f);
    m_rect.anchorMax.Set(0.0f, 0.0f);

    // Ouvre l'écran de démarrage
    StartScreen *startScreen = new StartScreen(scene);
    startScreen->SetParent(this);
}

void TitleCanvas::Update()
{
    SetVisible(true);
}

void TitleCanvas::Render()
{
    // Obscursit le fond
    SDL_Renderer *renderer = m_scene.GetRenderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, NULL);
}