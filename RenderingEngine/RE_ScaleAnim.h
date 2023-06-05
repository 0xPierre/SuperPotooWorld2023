#pragma once

#include "RE_Settings.h"
#include "RE_Animation.h"
#include "RE_Utils.h"

class RE_ScaleAnim : public RE_Animation
{
public:
    RE_ScaleAnim(RE_Animator &animator, const std::string &name, Vec2 scale0, Vec2 scale1);

    void Update(float dt);

protected:
    Vec2 m_scale0;
    Vec2 m_scale1;
};
