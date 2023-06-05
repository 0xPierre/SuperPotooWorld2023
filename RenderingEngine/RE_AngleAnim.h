#pragma once

#include "RE_Settings.h"
#include "RE_Animation.h"

class RE_AngleAnim : public RE_Animation
{
public:
    RE_AngleAnim(RE_Animator &animator, const std::string &name, float angle0, float angle1);

    void Update(float dt);

protected:
    float m_angles[2];
};
