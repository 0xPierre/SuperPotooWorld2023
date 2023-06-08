#pragma once

#include "UIObject.h"
#include "Text.h"
#include "Image.h"
#include "Button.h"
#include "StaticMap.h"

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
    Button *m_selectedBlock;
    Image *m_blockImage;
    std::vector<Tile::Type> m_objects_v;
    std::vector<Tile::Type> m_objects_h;
};