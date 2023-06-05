#include "LevelHeader.h"
#include "LevelScene.h"
#include "Image.h"

LevelHeader::LevelHeader(LevelScene &scene):
    UIObject(scene), m_levelScene(scene), m_fireflyCount(nullptr)
{
    m_name = "LevelHeader";

    AssetManager &assets = scene.GetAssetManager();
    TTF_Font *font = assets.GetFont(FontID::LARGE);
    SDL_Color color = assets.GetColor(ColorID::NORMAL);

    int w, h;
    TTF_SizeUTF8(font, u8"00", &w, &h);
    float imgH = 80.0f;
    float imgW = imgH;
    float numW = (float)w;
    float sep = 20.0f;
    float panelW = imgW + sep + numW;
    float panelH = imgH;

    m_rect.anchorMin.Set(0.0f, 0.0f);
    m_rect.anchorMax.Set(0.0f, 0.0f);
    m_rect.offsetMin.Set(80.0f, 40.0f);
    m_rect.offsetMax = m_rect.offsetMin + Vec2(panelW, panelH);

    RE_Atlas *atlas = assets.GetAtlas(AtlasID::UI);

    float currX = 0.0f;
    float currY = 0.0f;

    // Image du nombre de luciolles
    RE_AtlasPart *part = atlas->GetPart("Firefly");
    AssertNew(part);
    Image *fireflyImage = new Image(scene, part, 0);
    fireflyImage->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    fireflyImage->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
    fireflyImage->GetLocalRect().offsetMin.Set(currX, currY);
    fireflyImage->GetLocalRect().offsetMax.Set(currX + imgW, currY + imgH);
    fireflyImage->SetParent(this);

    currX += imgW + sep;

    // Compteur du nombre de luciolles
    m_fireflyCount = new Text(scene, "0", font, color);
    m_fireflyCount->SetAnchor(RE_Anchor::WEST);
    m_fireflyCount->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    m_fireflyCount->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
    m_fireflyCount->GetLocalRect().offsetMin.Set(currX, currY);
    m_fireflyCount->GetLocalRect().offsetMax.Set(currX + numW, currY + imgH);
    m_fireflyCount->SetParent(this);
}

void LevelHeader::Update()
{
    Player *player = m_levelScene.GetPlayer();
    m_fireflyCount->SetString(std::to_string(player->GetFireflyCount()));
}
