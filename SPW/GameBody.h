#pragma once

#include "Settings.h"
#include "GameObject.h"

class GameCollision;

class GameBody : public GameObject
{
public:
    GameBody(Scene &scene, Layer layer);
    virtual ~GameBody();

    virtual void OnDisable() override;
    virtual void OnEnable() override;

    virtual void OnCollisionEnter(GameCollision &collision);
    virtual void OnCollisionStay(GameCollision &collision);
    virtual void OnCollisionExit(GameCollision &collision);

    virtual void DrawBody(PE_Body *body);
    virtual void DrawCollider(PE_Collider *collider);
    virtual void DrawID();

    static GameBody *GetFromBody(PE_Body *body);

    void SetBody(PE_Body *body);
    PE_Body *GetBody();

    void SetStartPosition(const PE_Vec2 &position);
    PE_Vec2 GetStartPosition() const;

    void DeleteBody();
    void SetBodyEnabled(bool bodyEnabled);

    PE_Vec2 GetPosition() const;
    PE_Vec2 GetVelocity() const;

protected:
    SDL_Color m_debugColor;
    SDL_Color m_sleepColor;

private:
    PE_Body *m_body;
    PE_Vec2 m_startPos;
    RE_Text *m_textID;
};

inline PE_Body *GameBody::GetBody()
{
    return m_body;
}

inline void GameBody::SetStartPosition(const PE_Vec2 &position)
{
    m_startPos = position;
}

inline PE_Vec2 GameBody::GetStartPosition() const
{
    return m_startPos;
}

inline void GameBody::SetBodyEnabled(bool bodyEnabled)
{
    if (m_body)
    {
        m_body->SetEnabled(bodyEnabled);
    }
}

inline void GameBody::OnDisable()
{
    SetBodyEnabled(false);
}

inline void GameBody::OnEnable()
{
    SetBodyEnabled(true);
}