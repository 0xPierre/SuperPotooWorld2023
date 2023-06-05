#pragma once

#include "Settings.h"
#include "UIObject.h"
#include "TitleCanvas.h"

class TitleScene;

class StartScreen : public UIObject
{
public:
    StartScreen(TitleScene &scene);
};