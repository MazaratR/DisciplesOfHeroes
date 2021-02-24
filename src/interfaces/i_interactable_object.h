#ifndef I_INTERACTABLE_OBJECT_H
#define I_INTERACTABLE_OBJECT_H

#include <memory>
#include "object_types.h"
#include "point_2d.h"

#include "forward/i_hero_forward.h"
#include "forward/i_interactable_object_forward.h"

class MapElement;

class IInteractableObject
{
    friend MapElement;
public:
    IInteractableObject(ObjectType _type) : m_objectType(_type) {}
    virtual ~IInteractableObject() = default;

    virtual Point2D_int tileCoord() const { return m_tileCoord; };
    virtual void moveToTile(Point2D_int _coord)  { m_tileCoord = _coord; };

    virtual ObjectType objectType() const { return m_objectType; }
    virtual void interact(IHeroShared& _hero, IInteractObjShared& _selfShared) = 0;
    virtual std::string title() const = 0;

    virtual void setPlacedOnMap(bool _placed) { m_placedOnMap = _placed; }
    bool placedOnMap() const { return m_placedOnMap; }

    bool waitingForDestroying() const { return m_needToDestroy; }
    void setNeedToDestroy(bool _destroy) { m_needToDestroy = _destroy; }

protected:
    ObjectType m_objectType = ObjectType::Unknown;
    bool m_needToDestroy = false;
    Point2D_int m_tileCoord;
    bool m_placedOnMap = false;
};

#endif // I_INTERACTABLE_OBJECT_H
