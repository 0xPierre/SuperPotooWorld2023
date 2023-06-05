#pragma once

#include "Settings.h"
#include "Camera.h"
#include "Utils.h"

class LevelScene;

class MainCamera : public Camera
{
public:
    MainCamera(LevelScene &scene);
    virtual ~MainCamera();

    virtual void Update() override;

protected:
    DampedVec2 m_center;
};