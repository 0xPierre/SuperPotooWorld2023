#pragma once

#include "RE_Settings.h"
#include "RE_Animation.h"
#include "RE_Utils.h"

class RE_BezierAnim : public RE_Animation
{
public:
    RE_BezierAnim(RE_Animator &animator, const std::string &name, const Vec2 points[4]);

    void Update(float dt);
    void SetPoints(const Vec2 points[4]);

protected:
    Vec2 m_p[4];
};


inline void RE_BezierAnim::SetPoints(const Vec2 points[4])
{
    for (int i = 0; i < 4; i++)
    {
        m_p[i] = points[i];
    }
}
