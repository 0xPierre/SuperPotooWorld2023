#pragma once

#include "Settings.h"
#include "Camera.h"

class TitleCamera : public Camera
{
public:
    TitleCamera(Scene &scene);
    virtual ~TitleCamera();

    virtual void Update() override;
};