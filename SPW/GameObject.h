#pragma once

#include "Settings.h"

enum Category : uint32_t
{
    CATEGORY_PLAYER      = 1 << 0,
    CATEGORY_COLLECTABLE = 1 << 1,
    CATEGORY_ENEMY       = 1 << 2,
    CATEGORY_TERRAIN     = 1 << 3
};

enum class Layer : uint32_t
{
    BACKGROUND,
    TERRAIN_BACKGROUND,
    TERRAIN,
    TERRAIN_FOREGROUND,
    COLLECTABLE,
    ENEMY,
    PLAYER,
    FOREGROUND,
    UI_BACKGROUND,
    UI,
    UI_FOREGROUND
};

class Scene;

class GameObject
{
public:
    GameObject(Scene &scene, Layer layer);
    GameObject(GameObject const&) = delete;
    GameObject& operator=(GameObject const&) = delete;
    virtual ~GameObject();

    virtual void DrawGizmos();
    virtual void FixedUpdate();
    virtual void OnDisable();
    virtual void OnEnable();
    virtual void OnRespawn();
    virtual void Render();
    virtual void Start();
    virtual void Update();

    bool IsEnabled();
    Layer GetLayer() const;
    void SetLayer(Layer layer);
    void SetVisible(bool isVisible);
    void SetToRespawn(bool shouldRespawn);
    void SetEnabled(bool isEnabled);
    void Delete();

    void SetParent(GameObject *parent);
    GameObject *GetParent() const;

    void SetName(const std::string &name);
    std::string &GetName();

    int GetDepth() const;
    int GetID() const;

    std::list<GameObject *>::iterator ChildrenBegin();
    std::list<GameObject *>::iterator ChildrenEnd();

protected:
    Scene &m_scene;
    std::string m_name;

private:
    friend class ObjectManager;

    void AddChild(GameObject *child);
    void RemoveChild(GameObject *child);


    bool m_enabled;
    Layer m_layer;
    int m_depth;
    int m_objectID;

    enum class Flag : uint32_t
    {
        NONE       = 0x00,
        TO_START   = 0x01,
        TO_ENABLE  = 0x02,
        TO_DISABLE = 0x04,
        TO_DELETE  = 0x08,
        TO_PROCESS = 0x10,
        TO_RESPAWN = 0x20,
    };
    Flag m_flags;

    void AddFlags(GameObject::Flag flags);
    void SubFlags(GameObject::Flag flags);
    bool TestFlag(GameObject::Flag flag) const;

    GameObject *m_parent;
    std::list<GameObject *> m_children;
};

#include "scene.h"

constexpr enum GameObject::Flag operator |(
    const enum GameObject::Flag selfValue, const enum GameObject::Flag inValue)
{
    return (enum GameObject::Flag)(uint32_t(selfValue) | uint32_t(inValue));
}

constexpr enum GameObject::Flag operator &(
    const enum GameObject::Flag selfValue, const enum GameObject::Flag inValue)
{
    return (enum GameObject::Flag)(uint32_t(selfValue) & uint32_t(inValue));
}

constexpr enum GameObject::Flag operator ~(
    const enum GameObject::Flag selfValue)
{
    return (enum GameObject::Flag)(~uint32_t(selfValue));
}

inline bool GameObject::IsEnabled()
{
    return m_enabled;
}

inline Layer GameObject::GetLayer() const
{
    return m_layer;
}

inline void GameObject::SetLayer(Layer layer)
{
    m_layer = layer;
}

inline void GameObject::SetVisible(bool isVisible)
{
    m_scene.m_objectManager.SetVisible(this, isVisible);
}

inline void GameObject::SetToRespawn(bool shouldRespawn)
{
    m_scene.m_objectManager.SetToRespawn(this, shouldRespawn);
}

inline void GameObject::SetEnabled(bool isEnabled)
{
    m_scene.m_objectManager.SetEnabled(this, isEnabled);
}

inline void GameObject::Delete()
{
    m_scene.m_objectManager.DeleteObject(this);
}

inline GameObject *GameObject::GetParent() const
{
    return m_parent;
}

inline void GameObject::SetName(const std::string &name)
{
    m_name = name;
}

inline std::string &GameObject::GetName()
{
    return m_name;
}

inline int GameObject::GetDepth() const
{
    return m_depth;
}

inline int GameObject::GetID() const
{
    return m_objectID;
}

inline std::list<GameObject *>::iterator GameObject::ChildrenBegin()
{
    return m_children.begin();
}

inline std::list<GameObject *>::iterator GameObject::ChildrenEnd()
{
    return m_children.end();
}

inline void GameObject::AddFlags(GameObject::Flag flags)
{
    m_flags = m_flags | flags;
}

inline void GameObject::SubFlags(GameObject::Flag flags)
{
    m_flags = m_flags & (~flags);
}

inline bool GameObject::TestFlag(GameObject::Flag flag) const
{
    return (flag == (m_flags & flag));
}
