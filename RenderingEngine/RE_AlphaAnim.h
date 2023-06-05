#pragma once

#include "RE_Settings.h"
#include "RE_Animation.h"

class RE_AlphaAnim : public RE_Animation
{
public:
    RE_AlphaAnim(RE_Animator &animator, const std::string &name, float alpha0, float alpha1);

    void Update(float dt);

protected:
    float m_alphas[2];
};
