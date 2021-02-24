#include "resource.h"

#include "interfaces/i_hero.h"

Resource::Resource()
    : IInteractableObject(ObjectType::Resource)
{}

Resource::Resource(ResourceType _type, int _value)
    : IInteractableObject(ObjectType::Resource),
      m_resourceType(_type), m_value(_value)
{}

ResourceType Resource::resourceType() const
{
    return m_resourceType;
}

void Resource::interact(IHeroShared &_hero, IInteractObjShared &)
{
    _hero->collectResource(m_resourceType, m_value);
    m_needToDestroy = true;
}

std::string Resource::title() const
{
    return Convert::resourceToText(m_resourceType);
}
