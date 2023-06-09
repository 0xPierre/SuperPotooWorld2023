#include "LevelHeader.h"
#include "LevelScene.h"
#include "Image.h"
#include "Camera.h"
#include "Button.h"

class ChangeBackgroundListener : public ButtonListener
{
public:
    ChangeBackgroundListener(LevelScene& scene) :
        m_levelScene(scene)
    {
    }

    virtual void OnPress()
    {
        m_levelScene.GetLevelData()->themeID = (ThemeID)(((int)m_levelScene.GetLevelData()->themeID + 1) % 3);
    }

private:
    LevelScene& m_levelScene;
};

class ChangeViewListener : public ButtonListener
{
public:
    ChangeViewListener(LevelScene& scene) :
        m_levelScene(scene)
    {
    }

    virtual void OnPress()
    {
        m_levelScene.SetCamIndex((m_levelScene.GetCamIndex()+1) % 2);
    }

private:
    LevelScene& m_levelScene;
};

class SelectItemListener : public ButtonListener
{
public:
    SelectItemListener(LevelScene& scene, Tile::Type type, int partIdx) :
        m_levelScene(scene),
        m_type(type),
        m_part(partIdx)
    {
    }

    virtual void OnPress()
    {
        m_levelScene.SetSelectedTile(m_type);
        m_levelScene.SetSelectedPartIdx(m_part);
    }

private:
    LevelScene& m_levelScene;
    Tile::Type m_type;
    int m_part;
};


RE_AtlasPart* GetAtlasFromTile(Tile::Type type, Scene &scene, int &partIdx)
{
    const char* terrain = "-";
    const char* uiPlayer = "-";
    const char* uiEnemy = "-";

    switch (type)
    {
    case Tile::Type::WOOD:
        terrain = "Wood";
        break;
    case Tile::Type::ONE_WAY:
        terrain = "OneWay";
        break;
    case Tile::Type::SPIKE:
        terrain = "Spike";
        break;
    case Tile::Type::BRICK:
        terrain = "Brick";
        break;
    case Tile::Type::BONUSFULL:
        terrain = "BonusFull";
        break;
    case Tile::Type::BONUSEMPTY:
        terrain = "BonusEmpty";
        break;
    case Tile::Type::CHECKPOINTFULL:
        terrain = "CheckPointFull";
        break;
    case Tile::Type::CHECKPOINTEMPTY:
        terrain = "CheckPointEmpty";
        break;
    case Tile::Type::LEVELEND:
        terrain = "LevelEnd";
        break;
    case Tile::Type::MOVINGPLATFORM:
        terrain = "LevelEnd";
        break;
    case Tile::Type::GROUND:
        terrain = "Terrain";
        partIdx = 4;
        break;
    case Tile::Type::STEEP_SLOPE_L:
        terrain = "Terrain";
        partIdx = 9;
        break;
    case Tile::Type::STEEP_SLOPE_R:
        terrain = "Terrain";
        partIdx = 10;
        break;
    case Tile::Type::GENTLE_SLOPE_L1:
        terrain = "Terrain";
        partIdx = 13;
        break;
    case Tile::Type::GENTLE_SLOPE_L2:
        terrain = "Terrain";
        partIdx = 12;
        break;
    case Tile::Type::GENTLE_SLOPE_R1:
        terrain = "Terrain";
        partIdx = 15;
        break;
    case Tile::Type::GENTLE_SLOPE_R2:
        terrain = "Terrain";
        partIdx = 16;
        break;
    case Tile::Type::FIREFLY:
        uiPlayer = "Firefly";
        break;
    case Tile::Type::NUT:
        uiEnemy = "NutIdle";
        break;
    }

    AssetManager& assets = scene.GetAssetManager();
    RE_Atlas* atlasPlayer = assets.GetAtlas(AtlasID::UI);
    RE_Atlas* atlasCreative = assets.GetAtlas(AtlasID::TERRAIN);
    RE_Atlas* atlasEnemy = assets.GetAtlas(AtlasID::ENEMY);
    RE_AtlasPart* m_block;

    if (uiPlayer != "-")
        m_block = atlasPlayer->GetPart(uiPlayer);
    else if (uiEnemy != "-")
        m_block = atlasEnemy->GetPart(uiEnemy);
    else
        m_block = atlasCreative->GetPart(terrain);

    return m_block;
}

LevelHeader::LevelHeader(LevelScene &scene):
    UIObject(scene), m_levelScene(scene), m_fireflyCount(nullptr)
{
    m_name = "LevelHeader";

    AssetManager &assets = scene.GetAssetManager();
    TTF_Font *font = assets.GetFont(FontID::LARGE);
    SDL_Color color = assets.GetColor(ColorID::NORMAL);
    LevelScene* levelScene = (LevelScene* )(&m_scene);

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

    RE_Atlas *atlasPlayer = assets.GetAtlas(AtlasID::UI);


    float currFireX = 0.0f;
    float currFireY = 0.0f;
    float currLivesX = 0.0f;
    float currLivesY = 0.5f;

    if (!levelScene->IsCreative())
    {
        // Image du nombre de luciolles
        RE_AtlasPart *part = atlasPlayer->GetPart("Firefly");
        AssertNew(part);
        Image *fireflyImage = new Image(scene, part, 0);
        fireflyImage->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
        fireflyImage->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
        fireflyImage->GetLocalRect().offsetMin.Set(currFireX, currFireY);
        fireflyImage->GetLocalRect().offsetMax.Set(currFireX + imgW, currFireY + imgH);
        fireflyImage->SetParent(this);
    
        currFireX += imgW + sep;
    
        // Compteur du nombre de luciolles
        m_fireflyCount = new Text(scene, "0", font, color);
        m_fireflyCount->SetAnchor(RE_Anchor::WEST);
        m_fireflyCount->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
        m_fireflyCount->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
        m_fireflyCount->GetLocalRect().offsetMin.Set(currFireX, currFireY);
        m_fireflyCount->GetLocalRect().offsetMax.Set(currFireX + numW, currFireY + imgH);
        m_fireflyCount->SetParent(this);

    
    
        // Image number of heart
        RE_AtlasPart *heart = atlasPlayer->GetPart("Heart");
        AssertNew(heart);
        Image *heartImage = new Image(scene, heart, 0);
        heartImage->GetLocalRect().anchorMin.Set(0.0f, 1.0f);
        heartImage->GetLocalRect().anchorMax.Set(0.0f, 1.0f);
        heartImage->GetLocalRect().offsetMin.Set(currLivesX, currLivesY);
        heartImage->GetLocalRect().offsetMax.Set(currLivesX + imgW, currLivesY + imgH);
        heartImage->SetParent(this);
    
        currLivesX += imgW + sep;
    
        // lives counter
        m_heartCount = new Text(scene, "0", font, color);
        m_heartCount->SetAnchor(RE_Anchor::WEST);
        m_heartCount->GetLocalRect().anchorMin.Set(0.0f, 1.0f);
        m_heartCount->GetLocalRect().anchorMax.Set(0.0f, 1.0f);
        m_heartCount->GetLocalRect().offsetMin.Set(currLivesX, currLivesY);
        m_heartCount->GetLocalRect().offsetMax.Set(currLivesX + numW, currLivesY + imgH);
        m_heartCount->SetParent(this);

        return;
    }

    // Add a Change background btn
    RE_Atlas* atlas = assets.GetAtlas(AtlasID::UI);
    AssertNew(atlas);
    RE_AtlasPart* buttonPart = atlas->GetPart("Button");
    AssertNew(buttonPart);
    SDL_Color colorUp = assets.GetColor(ColorID::NORMAL);
    SDL_Color colorHover = assets.GetColor(ColorID::BLACK);
    SDL_Color colorDown = assets.GetColor(ColorID::NORMAL);
    font = assets.GetFont(FontID::NORMAL);

    Button* createWorldButton = new Button(scene, buttonPart);
    createWorldButton->GetLocalRect().anchorMin.Set(-0.1f, -0.5f);
    createWorldButton->GetLocalRect().anchorMax.Set(1.6f, -0.5f);
    createWorldButton->GetLocalRect().offsetMin.Set(0.0f, 50.f);
    createWorldButton->GetLocalRect().offsetMax.Set(0.0f, 50.f + 60.f);
    createWorldButton->SetParent(this);
    createWorldButton->SetBorders(new UIBorders(25, 25, 25, 25));

    createWorldButton->SetListener(new ChangeBackgroundListener(scene));

    Text* createWorldButtonLabel = new Text(scene, u8"Change background", font, colorUp);
    createWorldButton->SetText(createWorldButtonLabel, Button::State::UP);

    createWorldButtonLabel = new Text(scene, u8"Change background", font, colorHover);
    createWorldButton->SetText(createWorldButtonLabel, Button::State::HOVER);

    createWorldButtonLabel = new Text(scene, u8"Change background", font, colorDown);
    createWorldButton->SetText(createWorldButtonLabel, Button::State::DOWN);

    // Add a Change view btn
    Button* changeViewBtn = new Button(scene, buttonPart);
    changeViewBtn->GetLocalRect().anchorMin.Set(-0.1f, -0.5f);
    changeViewBtn->GetLocalRect().anchorMax.Set(1.6f, -0.5f);
    changeViewBtn->GetLocalRect().offsetMin.Set(0.0f, 120.f);
    changeViewBtn->GetLocalRect().offsetMax.Set(0.0f, 120.f + 60.f);
    changeViewBtn->SetParent(this);
    changeViewBtn->SetBorders(new UIBorders(25, 25, 25, 25));

    changeViewBtn->SetListener(new ChangeViewListener(scene));

    Text* changeViewButtonLabel = new Text(scene, u8"Change view", font, colorUp);
    changeViewBtn->SetText(changeViewButtonLabel, Button::State::UP);

    changeViewButtonLabel = new Text(scene, u8"Change view", font, colorHover);
    changeViewBtn->SetText(changeViewButtonLabel, Button::State::HOVER);

    changeViewButtonLabel = new Text(scene, u8"Change view", font, colorDown);
    changeViewBtn->SetText(changeViewButtonLabel, Button::State::DOWN);

    RE_Atlas* atlasCreative = m_levelScene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    RE_AtlasPart* block = atlasCreative->GetPart("Wood");


    m_objects_v = {
        Tile::Type::GENTLE_SLOPE_L1,
        Tile::Type::GENTLE_SLOPE_R1,
        Tile::Type::GENTLE_SLOPE_L2,
        Tile::Type::GENTLE_SLOPE_R2,
        Tile::Type::STEEP_SLOPE_L,
        Tile::Type::STEEP_SLOPE_R,
        Tile::Type::LEVELEND,
    };

    float offsetY = 0.f;
    float offsetX = 9.4f;
    float objectHeight = 90.f;
    float objectWidth = 0.50f;
    for (int i = 0; i < m_objects_v.size(); i++)
    {
        int partIdx = 0;
        RE_AtlasPart* block = GetAtlasFromTile(m_objects_v[i], m_scene, partIdx);

        if (m_objects_v[i] == Tile::Type::LEVELEND)
        {
            objectHeight = 180.f;
            objectWidth = 0.55f;
        }

        Button* item = new Button(scene, block, true, partIdx);
        item->GetLocalRect().anchorMin.Set(offsetX, 0.f);
        item->GetLocalRect().anchorMax.Set(offsetX + objectWidth, 0.f);
        item->GetLocalRect().offsetMin.Set(0.0f, offsetY);
        item->GetLocalRect().offsetMax.Set(0.0f, offsetY + objectHeight);
        item->SetParent(this);
        item->SetBorders(new UIBorders(25, 25, 25, 25));
        item->SetListener(new SelectItemListener(scene, m_objects_v[i], partIdx));

        offsetY += objectHeight + 25.f;
        objectHeight = 90.f;
        objectWidth = 0.50f;
    }

    m_objects_h = {
        Tile::Type::WOOD,
        Tile::Type::SPIKE,
        Tile::Type::GROUND,
        Tile::Type::BRICK,
        Tile::Type::BONUSFULL,
        Tile::Type::BONUSEMPTY,
        Tile::Type::FIREFLY,
        Tile::Type::NUT,
        Tile::Type::ONE_WAY,
    };

    offsetY = 0.f;
    offsetX = 3.7f;
    objectHeight = 90.f;
    objectWidth = 0.50f;
    for (int i = 0; i < m_objects_h.size(); i++)
    {
        int partIdx = 0;
        RE_AtlasPart* block = GetAtlasFromTile(m_objects_h[i], m_scene, partIdx);

        Button* item = new Button(scene, block, true, partIdx);
        item->GetLocalRect().anchorMin.Set(offsetX, 0.f);
        item->GetLocalRect().anchorMax.Set(offsetX + objectWidth, 0.f);
        item->GetLocalRect().offsetMin.Set(0.0f, offsetY);
        item->GetLocalRect().offsetMax.Set(0.0f, offsetY + objectHeight);
        item->SetParent(this);
        item->SetBorders(new UIBorders(25, 25, 25, 25));
        item->SetListener(new SelectItemListener(scene, m_objects_h[i], partIdx));

        /*if (m_objects_h[i] == levelScene->GetSelectedTile() && partIdx == levelScene->GetSelectedPartIdx())
        {
            
        }*/

        offsetX += objectWidth + 0.10f;
    }

    // Btn is on the first item
    offsetY = 0.f;
    offsetX = 3.7f;
    objectHeight = 90.f;
    objectWidth = 0.50f;
    m_selectedBlock = new Button(scene, atlas->GetPart("Button"), true);
    m_selectedBlock->GetLocalRect().anchorMin.Set(offsetX - 0.07, offsetY);
    m_selectedBlock->GetLocalRect().anchorMax.Set(offsetX + objectWidth + 0.07f, offsetY);
    m_selectedBlock->GetLocalRect().offsetMin.Set(0.0f, 0.0f - 9.f);
    m_selectedBlock->GetLocalRect().offsetMax.Set(0.0f, 0.0f + objectHeight + 9.f);
    m_selectedBlock->SetParent(this);
    m_selectedBlock->SetBorders(new UIBorders(25, 25, 25, 25));
    
}

void LevelHeader::Update()
{
    ControlsInput& controls = m_levelScene.GetInputManager().GetControls();
    RE_Atlas* atlasCreative = m_levelScene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    RE_Atlas* atlasPlayer = m_levelScene.GetAssetManager().GetAtlas(AtlasID::UI);
    RE_Atlas* atlasEnemy = m_levelScene.GetAssetManager().GetAtlas(AtlasID::ENEMY);
    LevelScene* levelScene = (LevelScene* )(&m_scene);

    float currLivesX = 0.0f;
    float currLivesY = 0.5f;
    float imgH = 80.0f;
    float imgW = imgH;

    
    if (!levelScene->IsCreative())
    {
        Player* player = m_levelScene.GetPlayer();
        m_fireflyCount->SetString(std::to_string(player->GetFireflyCount()));
        m_heartCount->SetString(std::to_string(player->GetHeartCount()));

        return;
    }


    float offsetY = 0.f;
    float offsetX = 9.4f;
    float objectHeight = 90.f;
    float objectWidth = 0.50f;

    for (int i = 0; i < m_objects_v.size(); i++)
    {
        int partIdx = 0;
        RE_AtlasPart* block = GetAtlasFromTile(m_objects_v[i], m_scene, partIdx);
        if (m_objects_v[i] == levelScene->GetSelectedTile() && partIdx == levelScene->GetSelectedPartIdx())
        {
         if (levelScene->GetSelectedTile() == Tile::Type::LEVELEND)
         {
             m_selectedBlock->GetLocalRect().anchorMin.Set(offsetX - 0.07, 0.f);
             m_selectedBlock->GetLocalRect().anchorMax.Set(offsetX + objectWidth + 0.07f, 0.f);
             m_selectedBlock->GetLocalRect().offsetMin.Set(0.0f, offsetY - 9.f);
             m_selectedBlock->GetLocalRect().offsetMax.Set(0.0f, offsetY + objectHeight + 99.f);
         }
         else {
             m_selectedBlock->GetLocalRect().anchorMin.Set(offsetX - 0.07, 0.f);
             m_selectedBlock->GetLocalRect().anchorMax.Set(offsetX + objectWidth + 0.07, 0.f);
             m_selectedBlock->GetLocalRect().offsetMin.Set(0.0f, offsetY - 9.f);
             m_selectedBlock->GetLocalRect().offsetMax.Set(0.0f, offsetY + objectHeight + 9.f);
         }
        }
         offsetY += objectHeight + 25.f;
         objectHeight = 90.f;
         objectWidth = 0.50f;
    }

    offsetY = 0.f;
    offsetX = 3.7f;
    objectHeight = 90.f;
    objectWidth = 0.50f;

    for (int i = 0; i < m_objects_h.size(); i++)
    {
        int partIdx = 0;
        RE_AtlasPart* block = GetAtlasFromTile(m_objects_h[i], m_scene, partIdx);

        if (m_objects_h[i] == levelScene->GetSelectedTile() && partIdx == levelScene->GetSelectedPartIdx())
        {
            if (levelScene->GetSelectedTile() == Tile::Type::LEVELEND)
            {
                m_selectedBlock->GetLocalRect().anchorMin.Set(offsetX - 0.07, offsetY);
                m_selectedBlock->GetLocalRect().anchorMax.Set(offsetX + objectWidth + 0.07f, offsetY);
                m_selectedBlock->GetLocalRect().offsetMin.Set(0.0f, 0.0f - 9.f);
                m_selectedBlock->GetLocalRect().offsetMax.Set(0.0f, offsetY + objectHeight + 93.f);
            }
            else
            {
                m_selectedBlock->GetLocalRect().anchorMin.Set(offsetX - 0.07, offsetY);
                m_selectedBlock->GetLocalRect().anchorMax.Set(offsetX + objectWidth + 0.07f, offsetY);
                m_selectedBlock->GetLocalRect().offsetMin.Set(0.0f, 0.0f - 9.f);
                m_selectedBlock->GetLocalRect().offsetMax.Set(0.0f, offsetY + objectHeight + 9.f);
            }
        }

        offsetX += objectWidth + 0.10f;
    }
}

