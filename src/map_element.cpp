#include "map_element.h"

MapElement::MapElement(TerrainType _type)
    : m_type(_type)
{ }

MapElement::MapElement(TerrainType _type, IInteractObjShared &_interactableObject)
    : m_type(_type)
    , m_interactableObject(_interactableObject)
{ }

void MapElement::setVisibilityByPlayer(IPlayerShared &_player, bool _isVisible)
{
    m_visibleByPlayer[_player] = _isVisible;
}

bool MapElement::isVisibleByPlayer(IPlayerShared &_player) const
{
    auto it = m_visibleByPlayer.find(_player);
    if (it != m_visibleByPlayer.end())
        return it->second;

    return false;
}

void MapElement::setType(TerrainType _type)
{
    m_type = _type;
}

TerrainType MapElement::type() const
{
    return m_type;
}

IInteractObjShared MapElement::interactableObject()
{
    return m_interactableObject;
}

IInteractObjSharedConst MapElement::interactableObject() const
{
    return m_interactableObject;
}

void MapElement::acceptInteractableObject(IInteractObjShared _obj)
{
    m_interactableObject = _obj;
}

void MapElement::releaseInteractableObject()
{
    m_interactableObject.reset();
}
