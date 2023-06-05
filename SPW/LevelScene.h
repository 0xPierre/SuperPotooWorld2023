#pragma once

#include "Settings.h"
#include "Scene.h"
#include "Player.h"
#include "LevelCanvas.h"
#include "LevelData.h"

class LevelScene : public Scene
{
public:
    LevelScene(SDL_Renderer *renderer, RE_Timer &mainTime, const LevelData &level);
    LevelScene(LevelScene const&) = delete;
    LevelScene& operator=(LevelScene const&) = delete;
    virtual ~LevelScene();

    virtual bool Update() override;
    virtual void OnRespawn() override;

    Player *GetPlayer() const;
    bool IsPaused() const;
    void SetPaused(bool isPaused);

private:
    std::array<Camera *, 2> m_cameras;
    Player *m_player;
    LevelCanvas *m_canvas;

    PE_Vec2 m_startPos;

    bool m_paused;
    int m_camIndex;
    float m_stepDelay;
};

inline Player *LevelScene::GetPlayer() const
{
    return m_player;
}

inline bool LevelScene::IsPaused() const
{
    return m_paused;
}
