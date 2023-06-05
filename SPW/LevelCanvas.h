#pragma once

#include "Settings.h"
#include "UIObject.h"
#include "PauseMenu.h"

class LevelScene;

class LevelCanvas : public UIObject
{
public:
    LevelCanvas(LevelScene &scene);

    virtual void Update() override;
    virtual void OnRespawn() override;
    virtual void Render() override;

    void OpenPauseMenu();
    void ClosePauseMenu();

private:
    PauseMenu *m_pauseMenu;
    LevelScene &m_levelScene;
};