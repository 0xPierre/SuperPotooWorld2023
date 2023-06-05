#include "LevelSelection.h"
#include "TitleScene.h"
#include "Button.h"
#include "Text.h"

namespace LevelSelectionNS
{
    class SelectionListener : public ButtonListener
    {
    public:
        SelectionListener(TitleScene &scene, int levelID) :
            m_titleScene(scene), m_levelID(levelID)
        {
        }

        virtual void OnPress()
        {
            m_titleScene.SetLevelID(m_levelID);
            m_titleScene.Quit();
        }

    private:
        TitleScene &m_titleScene;
        int m_levelID;
    };
}

LevelSelection::LevelSelection(TitleScene &scene) :
    UIObject(scene)
{
    float buttonH = 55.0f;
    float topSkip = 100.0f;
    float sep = 10.0f;
    float panelW = 350.0f;
    float panelH = topSkip + 4.0f * buttonH + 3.0f * sep;

    m_rect.anchorMin.Set(0.5f, 0.5f);
    m_rect.anchorMax.Set(0.5f, 0.5f);
    m_rect.offsetMin.Set(-0.5f * panelW, -0.5f * panelH);
    m_rect.offsetMax.Set(+0.5f * panelW, +0.5f * panelH);

    AssetManager &assets = scene.GetAssetManager();
    RE_Atlas *atlas = assets.GetAtlas(AtlasID::UI);
    AssertNew(atlas);

    // Création du titre
    TTF_Font *font = assets.GetFont(FontID::LARGE);
    Text *title = new Text(scene, u8"Sélection du niveau", font, assets.GetColor(ColorID::NORMAL));
    title->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    title->GetLocalRect().anchorMax.Set(1.0f, 0.0f);
    title->GetLocalRect().offsetMin.Set(0.0f, 0);
    title->GetLocalRect().offsetMax.Set(0.0f, topSkip);
    title->SetAnchor(RE_Anchor::NORTH);
    title->SetParent(this);

    // Création des boutons
    RE_AtlasPart *buttonPart = atlas->GetPart("Button");
    AssertNew(buttonPart);
    SDL_Color colorUp = assets.GetColor(ColorID::NORMAL);
    SDL_Color colorHover = assets.GetColor(ColorID::BLACK);
    SDL_Color colorDown = assets.GetColor(ColorID::NORMAL);
    font = assets.GetFont(FontID::NORMAL);

    const std::vector<LevelData> &levels = scene.GetLevels();

    float curY = topSkip;
    for (int i = 0; i < levels.size(); i++, curY += buttonH + sep)
    {
        Button *button = new Button(scene, buttonPart);
        button->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
        button->GetLocalRect().anchorMax.Set(1.0f, 0.0f);
        button->GetLocalRect().offsetMin.Set(0.0f, curY);
        button->GetLocalRect().offsetMax.Set(0.0f, curY + buttonH);
        button->SetParent(this);
        button->SetBorders(new UIBorders(25, 25, 25, 25));
        button->SetListener(new LevelSelectionNS::SelectionListener(scene, i));

        Text *buttonLabel = new Text(scene, levels[i].name, font, colorUp);
        button->SetText(buttonLabel, Button::State::UP);

        buttonLabel = new Text(scene, levels[i].name, font, colorHover);
        button->SetText(buttonLabel, Button::State::HOVER);

        buttonLabel = new Text(scene, levels[i].name, font, colorDown);
        button->SetText(buttonLabel, Button::State::DOWN);
    }
}
