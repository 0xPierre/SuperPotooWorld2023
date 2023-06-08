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

    // Creative selected block
    // lives counter
    m_selectedBlock = new Text(scene, " ", font, color);
    m_selectedBlock->SetAnchor(RE_Anchor::WEST);
    m_selectedBlock->GetLocalRect().anchorMin.Set(-1.0f, 11.8f);
    m_selectedBlock->GetLocalRect().anchorMax.Set(-1.0f, 11.8f);
    m_selectedBlock->GetLocalRect().offsetMin.Set(currLivesX, currLivesY);
    m_selectedBlock->GetLocalRect().offsetMax.Set(currLivesX + numW, currLivesY + imgH);
    m_selectedBlock->SetParent(this);

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

    m_selectedBlock->SetString("Item selected :");
    
    // Update creative block
    const char* terrain = "-";
    const char* uiPlayer = "-";
    const char* uiEnemy = "-";
    int terrainGroundId = 0;
    switch ((Tile::Type)controls.terrainSelected)
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
    case Tile::Type::STEEP_SLOPE_L:
    case Tile::Type::STEEP_SLOPE_R:
    case Tile::Type::GENTLE_SLOPE_L1:
    case Tile::Type::GENTLE_SLOPE_L2:
    case Tile::Type::GENTLE_SLOPE_R1:
    case Tile::Type::GENTLE_SLOPE_R2:
        terrain = "Terrain";
        terrainGroundId = controls.groundSelected;
        break;
    case Tile::Type::FIREFLY:
        uiPlayer = "Firefly";
		break;
    case Tile::Type::NUT:
        uiEnemy = "NutIdle";
        break;
    }

    if (uiPlayer == "-" && terrain == "-" && uiEnemy == "-")
        return;

    RE_AtlasPart* m_block;
    if (uiPlayer != "-")
        m_block = atlasPlayer->GetPart(uiPlayer);
    else if (uiEnemy != "-")
        m_block = atlasEnemy->GetPart(uiEnemy);
    else
        m_block = atlasCreative->GetPart(terrain);

    if (m_blockImage != nullptr) m_blockImage->Delete();
    AssertNew(m_block);


    m_blockImage = new Image(m_levelScene, m_block, terrainGroundId);
    if (terrain == "LevelEnd")
    {
        m_blockImage->GetLocalRect().anchorMin.Set(2.0f, 10.8f);
        m_blockImage->GetLocalRect().anchorMax.Set(2.2f, 11.9f);
    }
    else
    {
        m_blockImage->GetLocalRect().anchorMin.Set(2.0f, 11.9f);
        m_blockImage->GetLocalRect().anchorMax.Set(2.0f, 11.9f);
    }
    m_blockImage->GetLocalRect().offsetMin.Set(currLivesX, currLivesY);
    m_blockImage->GetLocalRect().offsetMax.Set(currLivesX + imgW, currLivesY + imgH);
    m_blockImage->SetParent(this);
}

