#pragma once

#include "RE_Settings.h"
#include "RE_Animation.h"
#include "RE_Atlas.h"

class RE_TexAnim : public RE_Animation
{
public:
    RE_TexAnim(RE_Animator &animator, const std::string &name, RE_AtlasPart *atlasPart);

    void OnPlay();
    void OnTextureChanged();
    void Update(float dt);

protected:
    RE_AtlasPart *m_atlasPart;
};
