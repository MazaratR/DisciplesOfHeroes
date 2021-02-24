#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <memory>

#include "point_2d.h"
#include "races.h"

#include "interfaces/i_map_manager.h"

#include "forward/i_player_forward.h"
#include "forward/i_interactable_object_forward.h"

class MapElement;
class ITownsManager;
class IHeroesManager;

class MapManager
        : public IMapManager
{
public:
    MapManager() = default;
    ~MapManager();

    void setTownsManager(std::shared_ptr<ITownsManager> _townsManager);
    void setHeroesManager(std::shared_ptr<IHeroesManager> _heroesManager);

    std::vector<Race> getIncludedRaces(const std::string& _name) override;
    void saveMap(const std::string& _fullPath) override;
    void loadMap(const std::string& _name) override;
    void createMap(int _sizeX, int _sizeY, TerrainType _type = TerrainType::Unknown) override;
    Point2D_int getMapSize() const override;

    MapElement * getMapElement(const Point2D_int& _coord) override;
    const MapElement * getMapElement(const Point2D_int& _coord) const override;
    bool isVisibleByPlayer(const Point2D_int& _coord, IPlayerShared& _player) const override;

    void processPlayerVisibility(IPlayerShared& _player) override;

    void setTerrainType(const Point2D_int& _coord, TerrainType _terrain) override;
    void placeResource(const Point2D_int& _coord, ResourceType _type, int _amount) override;
    void placeTown(const Point2D_int& _coord, const std::string &_name) override;
    void placeTown(const Point2D_int& _coord, Race _townRace) override;
    void placeHero(const Point2D_int& _coord, const std::string& _name) override;
    void placeHero(const Point2D_int& _coord, Race _heroRace) override;

private:
    void setMapElement(const Point2D_int& _coord, const MapElement& _element);
    void setVisibilityByPlayer(const Point2D_int& _coord,
                               IPlayerShared _player,
                               bool _isVisible = true);

    inline int getArrayIndex(const Point2D_int& _coord) const;
    inline bool isValidCoords(const Point2D_int& _coord) const;

    void placeInteractableObject(const Point2D_int& _coord, IInteractObjShared _obj);
    std::string getInteractObjNameForFile(IInteractObjShared _obj) const;

private:
    std::shared_ptr<ITownsManager> m_townsManager;
    std::shared_ptr<IHeroesManager> m_heroesManager;

    MapElement* m_map2DArray = nullptr;
    Point2D_int m_size;
};

#endif // MAPMANAGER_H
