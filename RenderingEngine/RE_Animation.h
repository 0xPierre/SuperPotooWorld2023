#pragma once

#include "RE_Settings.h"
#include "RE_Utils.h"

class RE_Animator;

/// @ingroup RE_Animator
/// @brief Type d'une courbe d'accélération.
/// Ces fonctions sont définies sur [0,1] et vérifient généralement f(0) = 0 et f(1) = 1.
typedef float (*RE_EasingFct)(float);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération linéaire.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_Linear(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération douce en entrée.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_In(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération douce en sortie.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_Out(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération avec douce en entrée et en sortie.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_InOut(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération définie à partir du cosinus.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_Cos(float x);

enum class RE_AnimFlag : uint32_t
{
    /// @brief Met l'animation en pause.
    PAUSED = 1 << 0,
    /// @brief Joue l'animation dans le sens inverse.
    REVERSED = 1 << 1,
    /// @brief Joue l'animation en avant puis en arrière.
    ALTERNATE = 1 << 2,
    /// @brief Arrête automatiquement l'animation à la fin.
    STOP_AT_END = 1 << 3,
    STOPPED = 1 << 4
};

constexpr enum RE_AnimFlag operator |(const enum RE_AnimFlag selfValue, const enum RE_AnimFlag inValue)
{
    return (enum RE_AnimFlag)(uint32_t(selfValue) | uint32_t(inValue));
}

constexpr enum RE_AnimFlag operator &(const enum RE_AnimFlag selfValue, const enum RE_AnimFlag inValue)
{
    return (enum RE_AnimFlag)(uint32_t(selfValue) & uint32_t(inValue));
}

constexpr enum RE_AnimFlag operator ~(const enum RE_AnimFlag selfValue)
{
    return (enum RE_AnimFlag)(~uint32_t(selfValue));
}

class RE_Animation
{
public:
    RE_Animation(RE_Animator &animator, const std::string &name);
    RE_Animation(RE_Animation const&) = delete;
    RE_Animation& operator=(RE_Animation const&) = delete;
    virtual ~RE_Animation();

    void SetCycleTime(float cycleTime);
    void SetCycleCount(int cycleCount);
    void SetDelay(float delay);
    void SetSpeed(float speed);
    void SetPhase(float phase);
    void SetEasing(RE_EasingFct easing);

    const std::string &GetName() const;
    RE_Animator &GetAnimator();
    float GetRawProgression();
    float GetProgression();

    void AddFlags(RE_AnimFlag flags);
    void SubFlags(RE_AnimFlag flags);
    bool IsStopped();

    virtual void OnPlay();
    virtual void OnResume();
    virtual void OnStop();
    virtual void OnTextureChanged();
    virtual void Update(float dt);

protected:
    std::string m_name;

    RE_Animator &m_animator;

    /// @brief Courbe d'accélération de l'animation.
    RE_EasingFct m_easing;

    /// @brief Phase de l'animation.
    float m_phase;

    /// @brief Le coefficient multiplicateur de la vitesse de lecture.
    float m_speed;

    /// @brief La durée d'un cycle.
    float m_cycleTime;

    /// @brief Délai avant le début de l'animation.
    float m_delay;

    /// @brief Accumulateur.
    float m_accu;

    /// @brief Accumulateur pour le délai.
    float m_delayAccu;

    /// @brief Les flags de l'animation.
    /// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
    RE_AnimFlag m_flags;

    /// @brief Nombre de cycles de l'animation.
    /// L'animation tourne en boucle si ce membre vaut -1.
    int m_cycleCount;

    /// @brief Indice du cycle courant. 
    int m_cycleIdx;
};

inline void RE_Animation::SetCycleTime(float cycleTime)
{
    m_cycleTime = cycleTime;
}

inline void RE_Animation::SetDelay(float delay)
{
    m_delay = delay;
}
inline void RE_Animation::SetSpeed(float speed)
{
    m_speed = speed;
}

inline void RE_Animation::SetPhase(float phase)
{
    m_phase = phase;
}

inline void RE_Animation::SetEasing(RE_EasingFct easing)
{
    m_easing = easing;
}

inline void RE_Animation::AddFlags(RE_AnimFlag flags)
{
    m_flags = m_flags | flags;
}

inline void RE_Animation::SubFlags(RE_AnimFlag flags)
{
    m_flags = m_flags & ~flags;
}
inline float RE_Animation::GetRawProgression()
{
    return m_accu / m_cycleTime;
}

inline RE_Animator &RE_Animation::GetAnimator()
{
    return m_animator;
}

inline bool RE_Animation::IsStopped()
{
    return uint32_t(m_flags & RE_AnimFlag::STOPPED);
}

inline const std::string &RE_Animation::GetName() const
{
    return m_name;
}