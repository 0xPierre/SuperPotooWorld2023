#include "LevelData.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <fstream>

ThemeID extractBackgroundType(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string line;
    std::string backgroundType;

    while (std::getline(file, line)) {
        if (line.substr(0, 3) == "bg:") {
            backgroundType = line.substr(3);
            break;
        }
    }

    if (backgroundType == "SKY") {
        return ThemeID::SKY;
	}
    else if (backgroundType == "MOUNTAINS") {
        return ThemeID::MOUNTAINS;
	}
    else {
        return ThemeID::LAKE;
	}
}

LevelData::LevelData(const std::string &nameIn, const std::string &pathIn, ThemeID themeIDIn) :
    name(nameIn), path(pathIn), themeID(themeIDIn)
{
}

std::vector<LevelData> LevelData::Init()
{
    std::vector<LevelData> data;

    /*
        Research levels dynamically and add them in data
    */
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA("../Assets/Level/*.txt", &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
			std::string name = findData.cFileName;
			std::string path = "../Assets/Level/" + name;
            printf("Level file found (%s)\n", name.c_str());
			std::string nameWithoutExtension = name.substr(0, name.find_last_of("."));
            data.push_back(LevelData(
				nameWithoutExtension,
				path,
                extractBackgroundType(path)
			));
		} while (FindNextFileA(hFind, &findData));
		FindClose(hFind);
	}

    /*data.push_back(LevelData(
        u8"Montagnes",
        u8"../Assets/Level/Level01.txt",
        ThemeID::MOUNTAINS
    ));
    data.push_back(LevelData(
        u8"Démo",
        u8"../Assets/Level/LevelDemo.txt",
        ThemeID::SKY
    ));*/
    return data;
}
