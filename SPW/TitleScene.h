#pragma once

#include "Settings.h"
#include "Scene.h"
#include "TitleCanvas.h"
#include "LevelData.h"

class TitleScene : public Scene
{
public:
    TitleScene(SDL_Renderer *renderer, RE_Timer &mainTime, const std::vector<LevelData> &levels);
    virtual ~TitleScene();

    const std::vector<LevelData> &GetLevels() const;
    void SetLevelID(int levelID);
    int GetLevelID() const;

private:
    TitleCanvas *m_canvas;
    const std::vector<LevelData> &m_levels;
    int m_selectedLevel;
};

inline const std::vector<LevelData>& TitleScene::GetLevels() const
{
    return m_levels;
}

inline void TitleScene::SetLevelID(int levelID)
{
    m_selectedLevel = levelID;
}

inline int TitleScene::GetLevelID() const
{
    return m_selectedLevel;
}
