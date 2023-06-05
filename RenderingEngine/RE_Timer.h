#pragma once

#include "RE_Settings.h"

/// @ingroup RE_Timer
/// @brief Structure représentant un chronomètre.
class RE_Timer
{
public:
    RE_Timer();
    RE_Timer(RE_Timer const&) = delete;
    RE_Timer& operator=(RE_Timer const&) = delete;

    void Start();
    void Update();

    void SetMaximumDeltaTime(float maxDelta);
    void SetTimeScale(float scale);
    float GetTimeScale() const;
    float GetDelta() const;
    float GetUnscaledDelta() const;
    float GetElapsed() const;
    float GetUnscaledElapsed() const;

protected:
    /// @protected
    /// @brief Temps de départ.
    float m_startTime;

    /// @protected
    /// @brief Temps du dernier appel à RE_Timer_Update().
    float m_currentTime;

    /// @protected
    /// @brief Temps de l'avant dernier appel à RE_Timer_Update().
    float m_previousTime;

    /// @protected
    /// @brief Ecart entre les deux derniers appels à RE_Timer_Update().
    float m_delta;
    float m_unscaledDelta;

    float m_scale;

    float m_maxDelta;

    float m_elapsed;
    float m_unscaledElapsed;
};

inline void RE_Timer::SetMaximumDeltaTime(float maxDelta)
{
    m_maxDelta = maxDelta;
}

inline void RE_Timer::SetTimeScale(float scale)
{
    m_scale = scale;
}

inline float RE_Timer::GetDelta() const
{
    return m_delta;
}

inline float RE_Timer::GetTimeScale() const
{
    return m_scale;
};

inline float RE_Timer::GetUnscaledDelta() const
{
    return m_unscaledDelta;
}

inline float RE_Timer::GetElapsed() const
{
    return m_elapsed;
}

inline float RE_Timer::GetUnscaledElapsed() const
{
    return m_unscaledElapsed;
}
