#include "Background.h"
#include "Camera.h"
#include "DebugCamera.h"

Background::Background(Scene &scene, Layer layer) :
    GameObject(scene, layer), m_layers(),
    m_shiftFactors(), m_worldDim(PE_Vec2::one)
{
    m_name = "Background";

    for (int i = 0; i < 10; ++i)
    {
        m_layers.push_back(nullptr);
        m_shiftFactors.push_back(PE_Vec2::zero);
    }
}

Background::~Background()
{
}

void Background::Update()
{
    SetVisible(true);
}

void Background::Render()
{
    Camera *camera = m_scene.GetActiveCamera();
    AssertNew(camera);

    SDL_Renderer *renderer = m_scene.GetRenderer();
    PE_AABB view = camera->GetWorldView();

    DebugCamera *debugCamera = dynamic_cast<DebugCamera *>(camera);
    if (debugCamera != nullptr)
        return;

    // Dimension du fond dans le référentiel monde
    float scale = camera->GetWorldToViewScale();
    float layerW = scale * m_worldDim.x;
    float layerH = scale * m_worldDim.y;

    // Dessine les différents calques du fond (parallax)
    for (int i = 0; i < m_layers.size(); ++i)
    {
        if (m_layers[i] == nullptr)
            continue;

        PE_Vec2 origin(
            view.lower.x * (1.0f - m_shiftFactors[i].x),
            view.lower.y * (1.0f - m_shiftFactors[i].y)
        );

        float x, y;
        camera->WorldToView(origin, x, y);

        while (x < layerW)
        {
            x += layerW;
        }
        while (x > layerW)
        {
            x -= layerW;
        }

        SDL_FRect dstRect = { 0 };
        dstRect.x = x;
        dstRect.y = y;
        dstRect.w = layerW;
        dstRect.h = layerH;
        RE_RenderCopyF(renderer, m_layers[i], NULL, &dstRect, RE_Anchor::SOUTH_WEST);

        dstRect.x = x - layerW;
        RE_RenderCopyF(renderer, m_layers[i], NULL, &dstRect, RE_Anchor::SOUTH_WEST);
    }
}
