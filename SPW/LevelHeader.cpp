#include "LevelHeader.h"
#include "LevelScene.h"
#include "Image.h"
#include "Camera.h"

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

    RE_Atlas *atlasPlayer = assets.GetAtlas(AtlasID::UI);


    float currFireX = 0.0f;
    float currFireY = 0.0f;
    float currLivesX = 0.0f;
    float currLivesY = 0.5f;

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

    
    
    // Image number of lives
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
    m_livesCount = new Text(scene, "0", font, color);
    m_livesCount->SetAnchor(RE_Anchor::WEST);
    m_livesCount->GetLocalRect().anchorMin.Set(0.0f, 1.0f);
    m_livesCount->GetLocalRect().anchorMax.Set(0.0f, 1.0f);
    m_livesCount->GetLocalRect().offsetMin.Set(currLivesX, currLivesY);
    m_livesCount->GetLocalRect().offsetMax.Set(currLivesX + numW, currLivesY + imgH);
    m_livesCount->SetParent(this);


    // Creative selected block

    // lives counter
    m_selectedBlock = new Text(scene, "Block Selected :", font, color);
    m_selectedBlock->SetAnchor(RE_Anchor::WEST);
    m_selectedBlock->GetLocalRect().anchorMin.Set(-1.0f, 12.0f);
    m_selectedBlock->GetLocalRect().anchorMax.Set(-1.0f, 12.0f);
    m_selectedBlock->GetLocalRect().offsetMin.Set(currLivesX, currLivesY);
    m_selectedBlock->GetLocalRect().offsetMax.Set(currLivesX + numW, currLivesY + imgH);
    m_selectedBlock->SetParent(this);
}

void LevelHeader::Update()
{
    ControlsInput& controls = m_levelScene.GetInputManager().GetControls();
    RE_Atlas* atlasCreative = m_levelScene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);

    float currLivesX = 0.0f;
    float currLivesY = 0.5f;
    float imgH = 80.0f;
    float imgW = imgH;

    
    Player* player = m_levelScene.GetPlayer();
    m_fireflyCount->SetString(std::to_string(player->GetFireflyCount()));
    m_livesCount->SetString(std::to_string(player->GetLifeCount()));

    // Update creative block
    const char* terrain = "Wood";
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
    }

    if (m_blockImage != nullptr) m_blockImage->Delete();
    
    RE_AtlasPart* m_block = atlasCreative->GetPart(terrain);
    AssertNew(m_block);


    m_blockImage = new Image(m_levelScene, m_block, terrainGroundId);
    if (terrain == "LevelEnd")
    {
        m_blockImage->GetLocalRect().anchorMin.Set(2.0f, 12.0f);
        m_blockImage->GetLocalRect().anchorMax.Set(2.0f, 12.0f);
    }
    else
    {
        m_blockImage->GetLocalRect().anchorMin.Set(2.0f, 12.0f);
        m_blockImage->GetLocalRect().anchorMax.Set(2.0f, 12.0f);
    }
    m_blockImage->GetLocalRect().offsetMin.Set(currLivesX, currLivesY);
    m_blockImage->GetLocalRect().offsetMax.Set(currLivesX + imgW, currLivesY + imgH);
    m_blockImage->SetParent(this);
}

