#pragma once

#include "Settings.h"

class GameObject;

class ObjectManager
{
public:
    ObjectManager();
    ObjectManager(ObjectManager const&) = delete;
    ObjectManager& operator=(ObjectManager const&) = delete;
    ~ObjectManager();

    void AddObject(GameObject *object);
    void DeleteObject(GameObject *object);
    void SetEnabled(GameObject *object, bool isEnabled);
    void SetVisible(GameObject *object, bool isVisible);
    bool IsVisible(GameObject *object) const;
    void AddVisibleBodies(PE_World &world, PE_AABB &worldView);
    void SetToRespawn(GameObject *object, bool shouldRespawn);
    void DeleteObjects();

    void ClearVisibleObjects();
         
    void ProcessObjects();
    void ProcessVisibleObjects();
         
    void RespawnObjects();

    std::set<GameObject *>::iterator begin();
    std::set<GameObject *>::iterator end();

    const std::vector<GameObject *>::iterator visibleObjectsBegin();
    const std::vector<GameObject *>::iterator visibleObjectsEnd();

    void PrintObjects();

private:
    friend class GameObject;
    int m_nextID;

    std::vector<GameObject *> m_visibleObjects;
    std::set<GameObject *> m_objects;
    std::set<GameObject *> m_visibleSet;
    std::set<GameObject *> m_toProcess;
};

inline std::set<GameObject*>::iterator ObjectManager::begin()
{
    return m_objects.begin();
}

inline std::set<GameObject*>::iterator ObjectManager::end()
{
    return m_objects.end();
}

inline const std::vector<GameObject *>::iterator ObjectManager::visibleObjectsBegin()
{
    return m_visibleObjects.begin();
}

inline const std::vector<GameObject *>::iterator ObjectManager::visibleObjectsEnd()
{
    return m_visibleObjects.end();
}

inline bool ObjectManager::IsVisible(GameObject *object) const
{
    auto it = m_visibleSet.find(object);
    return it != m_visibleSet.end();
}
