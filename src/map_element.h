#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include "terrain.h"
#include "resource.h"
#include "point_2d.h"

#include "forward/i_player_forward.h"
#include "forward/i_interactable_object_forward.h"

class MapElement
{
public:
    MapElement() = default;
    MapElement(TerrainType _type);
    MapElement(TerrainType _type, IInteractObjShared& _interactableObject);

    void setVisibilityByPlayer(IPlayerShared& _player, bool _isVisible);
    bool isVisibleByPlayer(IPlayerShared& _player) const;

    void setType(TerrainType _type);
    TerrainType type() const;

    IInteractObjShared interactableObject();
    IInteractObjSharedConst interactableObject() const;
    void acceptInteractableObject(IInteractObjShared _obj);
    void releaseInteractableObject();

private:
    TerrainType m_type = TerrainType::Unknown;
    IInteractObjShared m_interactableObject;
    std::map<IPlayerWeak, bool, std::owner_less<IPlayerWeak>> m_visibleByPlayer;
};

#endif // MAPELEMENT_H
