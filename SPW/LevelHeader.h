#pragma once

#include "UIObject.h"
#include "Text.h"
#include "Image.h"

class LevelScene;

class LevelHeader : public UIObject
{
public:
    LevelHeader(LevelScene &scene);

    virtual void Update() override;

private:
    LevelScene &m_levelScene;
    Text *m_fireflyCount;
    Text *m_livesCount;
    Text* m_heartCount;
    Text *m_selectedBlock;
    Image *m_blockImage;
};