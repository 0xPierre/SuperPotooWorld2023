#pragma once

#include "Settings.h"
#include "UIObject.h"

class TitleScene;

class TitleCanvas : public UIObject
{
public:
    TitleCanvas(TitleScene &scene);

    virtual void Update() override;
    virtual void Render() override;
};