#pragma once

#include "Settings.h"
#include "AssetManager.h"

class LevelData
{
public:
    LevelData(const std::string &name, const std::string &path, ThemeID themeID);

    const std::string name;
    const std::string path;
    ThemeID themeID;

    static std::vector<LevelData> Init();
};