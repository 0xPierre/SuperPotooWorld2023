#include "StartScreen.h"
#include "TitleScene.h"
#include "LevelSelection.h"
#include "Image.h"
#include "Button.h"

namespace StartScreenNS
{
    class QuitListener : public ButtonListener
    {
    public:
        QuitListener(TitleScene &titleScene) : m_titleScene(titleScene) {}
        virtual void OnPress()
        {
            m_titleScene.Quit();
        }
    private:
        TitleScene &m_titleScene;
    };

    class SelectionListener : public ButtonListener
    {
    public:
        SelectionListener(TitleScene &titleScene, StartScreen &startScreen) :
            m_titleScene(titleScene), m_startScreen(startScreen)
        {
        }
        virtual void OnPress() override
        {
            LevelSelection *levelSelection = new LevelSelection(m_titleScene);
            levelSelection->SetParent(m_startScreen.GetParent());
            m_startScreen.Delete();
        }
    private:
        TitleScene &m_titleScene;
        StartScreen &m_startScreen;
    };
}

StartScreen::StartScreen(TitleScene &scene) :
    UIObject(scene)
{
    AssetManager &assets = scene.GetAssetManager();

    float buttonH = 55.0f;
    float topSkip = 400.0f;
    float sep = 10.0f;
    float panelW = 350.0f;
    float panelH = topSkip + 4.0f * buttonH + 3.0f * sep;

    m_rect.anchorMin.Set(0.5f, 0.5f);
    m_rect.anchorMax.Set(0.5f, 0.5f);
    m_rect.offsetMin.Set(-0.5f * panelW, -0.5f * panelH);
    m_rect.offsetMax.Set(+0.5f * panelW, +0.5f * panelH);

    RE_Atlas *logoAtlas = assets.GetAtlas(AtlasID::LOGO);
    AssertNew(logoAtlas);
    RE_Atlas *uiAtlas = assets.GetAtlas(AtlasID::UI);
    AssertNew(uiAtlas);

    // Création du logo
    RE_AtlasPart *logoPart = logoAtlas->GetPart("Logo");
    Image *logo = new Image(scene, logoPart, 0);
    float ratio = logo->GetNaturalRatio();
    float logoW = 700.0f;
    logo->GetLocalRect().anchorMin.Set(0.5f, 0.0f);
    logo->GetLocalRect().anchorMax.Set(0.5f, 0.0f);
    logo->GetLocalRect().offsetMin.Set(-0.5f * logoW, 0.0f);
    logo->GetLocalRect().offsetMax.Set(+0.5f * logoW, logoW / ratio);
    logo->SetParent(this);

    // Création des boutons
    RE_AtlasPart *buttonPart = uiAtlas->GetPart("Button");
    AssertNew(buttonPart);
    SDL_Color colorUp = assets.GetColor(ColorID::NORMAL);
    SDL_Color colorHover = assets.GetColor(ColorID::BLACK);
    SDL_Color colorDown = assets.GetColor(ColorID::NORMAL);
    TTF_Font *font = assets.GetFont(FontID::NORMAL);

    const std::string texts[2] = { u8"Démarrer", u8"Quitter" };
    ButtonListener *listener[2] = { 0 };
    listener[0] = new StartScreenNS::SelectionListener(scene, *this);
    listener[1] = new StartScreenNS::QuitListener(scene);

    float curY = topSkip;
    for (int i = 0; i < 2; i++, curY += buttonH + sep)
    {
        Button *button = new Button(scene, buttonPart);
        button->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
        button->GetLocalRect().anchorMax.Set(1.0f, 0.0f);
        button->GetLocalRect().offsetMin.Set(0.0f, curY);
        button->GetLocalRect().offsetMax.Set(0.0f, curY + buttonH);
        button->SetParent(this);
        button->SetBorders(new UIBorders(25, 25, 25, 25));
        button->SetListener(listener[i]);

        Text *buttonLabel = new Text(scene, texts[i], font, colorUp);
        button->SetText(buttonLabel, Button::State::UP);

        buttonLabel = new Text(scene, texts[i], font, colorHover);
        button->SetText(buttonLabel, Button::State::HOVER);

        buttonLabel = new Text(scene, texts[i], font, colorDown);
        button->SetText(buttonLabel, Button::State::DOWN);
    }
}
