#include "GameObject.h"
#include "Scene.h"
#include "ObjectManager.h"


GameObject::GameObject(Scene &scene, Layer layer) :
    m_scene(scene), m_enabled(true), m_layer(layer),
    m_children(), m_parent(nullptr), m_depth(0), m_name(), m_flags(Flag::NONE),
    m_objectID(-1)
{
    scene.m_objectManager.AddObject(this);
}

void GameObject::AddChild(GameObject *child)
{
    assert(child);

    // Vérifie que child n'est pas déjà parmi les enfants
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if (child == *it)
            return;
    }

    // Ajoute child à la liste des enfants
    m_children.push_back(child);

    child->m_depth = m_depth + 1;

    // Définit le parent de child
    child->m_parent = this;
}

void GameObject::RemoveChild(GameObject *child)
{
    assert(child);

    bool found = false;
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if (child == *it)
        {
            found = true;
            m_children.erase(it);
            break;
        }
    }
    if (!found)
    {
        assert(false);
        return;
    }

    child->m_parent = NULL;
    child->m_depth = 0;
}

void GameObject::SetParent(GameObject *parent)
{
    if (m_parent) m_parent->RemoveChild(this);
    if (parent) parent->AddChild(this);
}

GameObject::~GameObject()
{
    // C'est la scene qui s'occupe de supprimer les objets fils
}

void GameObject::DrawGizmos()
{
}

void GameObject::FixedUpdate()
{
}

void GameObject::OnDisable()
{
}

void GameObject::OnEnable()
{
}

void GameObject::OnRespawn()
{
}

void GameObject::Render()
{
}

void GameObject::Start()
{
}

void GameObject::Update()
{
}
