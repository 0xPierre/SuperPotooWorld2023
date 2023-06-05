#pragma once

#include "Settings.h"
#include "Camera.h"

class LevelScene;

class DebugCamera : public Camera
{
public:
    DebugCamera(LevelScene &scene, const Camera &trackedCam);

    virtual void Update() override;

protected:
    bool m_track;
    const Camera &m_trackedCam;
    PE_Vec2 m_trackedLower;
};