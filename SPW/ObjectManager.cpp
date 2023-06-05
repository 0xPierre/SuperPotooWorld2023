#include "ObjectManager.h"
#include "GameObject.h"
#include "GameBody.h"

ObjectManager::ObjectManager() :
    m_objects(), m_toProcess(), m_visibleObjects(), m_nextID(0)
{
}

ObjectManager::~ObjectManager()
{
    assert(m_objects.size() == 0);
}

void ObjectManager::AddObject(GameObject *object)
{
    if (object->TestFlag(GameObject::Flag::TO_PROCESS) == false)
    {
        m_objects.insert(object);
        m_toProcess.insert(object);
        object->AddFlags(GameObject::Flag::TO_PROCESS);
        object->AddFlags(GameObject::Flag::TO_START);
        object->m_objectID = m_nextID++;
    }
}

void ObjectManager::DeleteObject(GameObject *object)
{
    if (object->TestFlag(GameObject::Flag::TO_PROCESS) == false)
    {
        m_toProcess.insert(object);
    }

    object->AddFlags(GameObject::Flag::TO_DELETE);

    for (auto it = object->ChildrenBegin(); it != object->ChildrenEnd(); ++it)
    {
        DeleteObject(*it);
    }
}

void ObjectManager::SetEnabled(GameObject *object, bool isEnabled)
{
    if (object->m_enabled == isEnabled)
        return;

    if (isEnabled == true)
    {
        if (object->TestFlag(GameObject::Flag::TO_PROCESS))
        {
            object->SubFlags(GameObject::Flag::TO_DISABLE);
            object->AddFlags(GameObject::Flag::TO_ENABLE);
            return;
        }
        else
        {
            m_toProcess.insert(object);
            object->AddFlags(GameObject::Flag::TO_ENABLE);
        }
    }
    else
    {
        if (object->TestFlag(GameObject::Flag::TO_PROCESS))
        {
            object->SubFlags(GameObject::Flag::TO_ENABLE);
            object->AddFlags(GameObject::Flag::TO_DISABLE);
            return;
        }
        else
        {
            m_toProcess.insert(object);
            object->AddFlags(GameObject::Flag::TO_DISABLE);
        }
    }
}

void ObjectManager::SetToRespawn(GameObject *object, bool shouldRespawn)
{
    if (shouldRespawn)
    {
        object->AddFlags(GameObject::Flag::TO_RESPAWN);
    }
    else
    {
        object->SubFlags(GameObject::Flag::TO_RESPAWN);
    }
}


void ObjectManager::SetVisible(GameObject *object, bool isVisible)
{
    if (isVisible)
        m_visibleSet.insert(object);
    else
        m_visibleSet.erase(object);
}

void ObjectManager::DeleteObjects()
{
    for (GameObject *gameObject : m_objects)
    {
        delete gameObject;
    }
    m_objects.clear();
    m_toProcess.clear();
    m_visibleObjects.clear();
    m_visibleSet.clear();
}

void ObjectManager::ClearVisibleObjects()
{
    m_visibleObjects.clear();
}

void ObjectManager::ProcessObjects()
{
    std::list<GameObject *> toDelete;
    std::vector<GameObject *> toProcessCopy(m_toProcess.begin(), m_toProcess.end());

    // Remet à zéro le conteneur
    m_toProcess.clear();
    for (auto &gameObject : toProcessCopy)
    {
        gameObject->SubFlags(GameObject::Flag::TO_PROCESS);
    }

    for (auto &gameObject : toProcessCopy)
    {
        // DELETE
        if (gameObject->TestFlag(GameObject::Flag::TO_DELETE))
        {
            toDelete.push_back(gameObject);
        }

        // START
        if (gameObject->TestFlag(GameObject::Flag::TO_START))
        {
            gameObject->SubFlags(GameObject::Flag::TO_START);
            gameObject->Start();

            gameObject->SubFlags(GameObject::Flag::TO_ENABLE);
            gameObject->m_enabled = true;
            gameObject->OnEnable();
        }

        // ENABLE
        if (gameObject->TestFlag(GameObject::Flag::TO_ENABLE))
        {
            gameObject->SubFlags(GameObject::Flag::TO_ENABLE);
            gameObject->m_enabled = true;
            gameObject->OnEnable();
        }

        // DISABLE
        if (gameObject->TestFlag(GameObject::Flag::TO_DISABLE))
        {
            gameObject->SubFlags(GameObject::Flag::TO_DISABLE);
            gameObject->m_enabled = false;
            gameObject->OnDisable();
        }

    }

    for (auto &gameObject : toDelete)
    {
        gameObject->SetParent(nullptr);
        for (auto child : gameObject->m_children)
        {
            child->m_parent = nullptr;
        }
    }
    for (auto &gameObject : toDelete)
    {
        m_objects.erase(gameObject);
        delete gameObject;
    }
    toDelete.clear();
}

void ObjectManager::PrintObjects()
{
    std::cout << "Liste des objets :" << std::endl;
    for (GameObject *gameObject : m_objects)
    {
        std::cout << "- (ID=" << gameObject->m_objectID << ") ";
        if (gameObject->IsEnabled() == false)
            std::cout << "[DISABLED] ";
        std::cout << gameObject->GetName() << std::endl;
    }
}

class VisibleCallback : public PE_QueryCallback
{
public:
    VisibleCallback(ObjectManager &objectManager) :
        m_objManager(objectManager)
    {
    }
    virtual ~VisibleCallback() {}

    virtual bool ReportCollider(PE_Collider *collider)
    {
        GameBody *object = GameBody::GetFromBody(collider->GetBody());
        
        // Ajoute l'objet aux objects visibles
        m_objManager.SetVisible(object, true);

        // On continue la recherche
        return true;
    }

private:
    ObjectManager &m_objManager;
};

void ObjectManager::AddVisibleBodies(PE_World &world, PE_AABB &worldView)
{
    VisibleCallback callback(*this);

    // Ajoute chaque GameBody visible par la caméra
    world.QueryAABB(callback, worldView);
}

bool CompareGameObjects(const GameObject *objectA, const GameObject *objectB)
{
    if (objectA->GetLayer() < objectB->GetLayer())
    {
        return true;
    }
    else if (objectA->GetLayer() == objectB->GetLayer())
    {
        return (objectA->GetDepth() < objectB->GetDepth());
    }
    return false;
}

void ObjectManager::ProcessVisibleObjects()
{
    m_visibleObjects.clear();
    for (GameObject *gameObject : m_visibleSet)
    {
        m_visibleObjects.push_back(gameObject);
    }
    m_visibleSet.clear();
    
    // Trie les objets selon leurs layers
    std::sort(m_visibleObjects.begin(), m_visibleObjects.end(), CompareGameObjects);
}

void ObjectManager::RespawnObjects()
{
    for (GameObject *gameObject : m_objects)
    {
        if (gameObject->TestFlag(GameObject::Flag::TO_RESPAWN))
        {
            gameObject->SubFlags(GameObject::Flag::TO_ENABLE);
            gameObject->OnEnable();
            gameObject->m_enabled = true;

            gameObject->SubFlags(GameObject::Flag::TO_RESPAWN);
            gameObject->OnRespawn();
        }
    }
}
