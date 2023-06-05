#pragma once

#include "RE_Settings.h"
#include "RE_Timer.h"
#include "RE_Renderer.h"
#include "RE_Utils.h"
#include "RE_Atlas.h"

//#include "RE_Animation.h"
//#include "RE_AlphaAnim.h"
//#include "RE_AngleAnim.h"
//#include "RE_BezierAnim.h"
//#include "RE_ScaleAnim.h"
//#include "RE_ShiftAnim.h"
#include "RE_TexAnim.h"

#include <map>
#include <string>

class RE_Animation;


/// @brief Structure représentant un animateur.
/// Cela permet d'animer un ensemble de plusieurs atlas de textures
/// et d'y ajouter des effets comme des variations de position ou d'opacité.
class RE_Animator
{
public:
    RE_Animator();
    RE_Animator(RE_Animator const&) = delete;
    RE_Animator& operator=(RE_Animator const&) = delete;
    virtual ~RE_Animator();

    enum class Mode
    {
        SCALED_TIME,
        UNSCALED_TIME
    };

    RE_Animation *GetAnimation(const std::string &name)
    {
        return m_animHT.at(name);
    }
    int DeleteAnimation(const std::string &name);


    int PlayAnimation(const std::string &name);
    int StopAnimation(const std::string &name);
    int ResumeAnimation(const std::string &name);

    void StopAnimations();
    void Update(const RE_Timer &timer);

    void SetUpdateMode(Mode mode)
    {
        m_unscaledTime = (mode == Mode::UNSCALED_TIME);
    }

    int RenderCopyF(const SDL_FRect *dstRect, RE_Anchor anchor);

    int RenderCopyExF(
        const SDL_FRect *dstRect, RE_Anchor anchor,
        const double angle, Vec2 center, SDL_RendererFlip flip
    );

    //------------------------------------------------------------
    // Protected ?

    void SetAtlasPart(RE_AtlasPart *atlasPart)
    {
        m_atlasPart = atlasPart;
    }

    void SetFrame(int frameIdx)
    {
        m_frameIdx = frameIdx;
    }

    void AddShift(Vec2 shift)
    {
        m_shift += shift;
    }

    void AddAngle(float angle)
    {
        m_angle += angle;
    }

    void MultScale(Vec2 scale)
    {
        m_scale.x *= scale.x;
        m_scale.y *= scale.y;
    }

    void MultAlpha(float alpha)
    {
        m_alpha *= alpha;
    }

protected:
    friend class RE_Animation;
    friend class RE_TexAnim;

    void AddAnimation(RE_Animation *anim, const std::string &name)
    {
        m_animHT.insert(std::make_pair(name, anim));
    }
    void SetActiveTexAnim(RE_TexAnim *anim);

    /// @brief Table de hachage contenant les textures animées.
    std::map<std::string, RE_Animation *> m_animHT;

    RE_AtlasPart *m_atlasPart;

    /// @protected
    /// @brief Indice de la partie à rendre dans l'atlas de texture actif.
    int m_frameIdx;
    Vec2 m_center;
    Vec2 m_shift;
    Vec2 m_scale;
    float m_angle;
    float m_alpha;

    bool m_unscaledTime;
};

inline int RE_Animator::RenderCopyF(const SDL_FRect *dstRect, RE_Anchor anchor)
{
    return RenderCopyExF(dstRect, anchor, 0.0f, m_center, SDL_FLIP_NONE);
}
