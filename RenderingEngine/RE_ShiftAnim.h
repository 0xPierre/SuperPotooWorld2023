#pragma once

#include "RE_Settings.h"
#include "RE_Animation.h"
#include "RE_Utils.h"

class RE_ShiftAnim : public RE_Animation
{
public:
    RE_ShiftAnim(RE_Animator &animator, const std::string &name, Vec2 shift0, Vec2 shift1);

    void Update(float dt);
    float GetScale();
    void SetScale(float scale);

protected:
    float m_scale;
    Vec2 m_shifts[2];
};

inline float RE_ShiftAnim::GetScale()
{
    return m_scale;
};

inline void RE_ShiftAnim::SetScale(float scale)
{
    m_scale = scale;
};
