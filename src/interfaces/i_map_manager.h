#ifndef I_MAP_MANAGER_H
#define I_MAP_MANAGER_H

#include <string>
#include <memory>

#include "point_2d.h"
#include "terrain.h"
#include "resource_types.h"

#include "forward/i_player_forward.h"

class MapElement;
enum class Race : std::uint8_t;

class IMapManager
{
public:
    IMapManager() = default;
    virtual ~IMapManager() = default;

    virtual std::vector<Race> getIncludedRaces(const std::string& _name) = 0;
    virtual void saveMap(const std::string& _fullPath) = 0;
    virtual void loadMap(const std::string& _name) = 0;
    virtual void createMap(int _sizeX, int _sizeY, TerrainType _type = TerrainType::Unknown) = 0;

    virtual MapElement* getMapElement(const Point2D_int& _coord) = 0;
    virtual const MapElement* getMapElement(const Point2D_int& _coord) const = 0;
    virtual bool isVisibleByPlayer(const Point2D_int& _coord, IPlayerShared& _player) const = 0;
    virtual Point2D_int getMapSize() const = 0;

    virtual void processPlayerVisibility(IPlayerShared& _player) = 0;

    // Map editing
    virtual void setTerrainType(const Point2D_int& _coord, TerrainType _terrain) = 0;
    virtual void placeResource(const  Point2D_int& _coord, ResourceType _type, int _amount) = 0;
    virtual void placeTown(const Point2D_int& _coord, const std::string& _name) = 0;
    virtual void placeTown(const Point2D_int& _coord, Race _townRace) = 0;
    virtual void placeHero(const Point2D_int& _coord, const std::string& _name) = 0;
    virtual void placeHero(const Point2D_int& _coord, Race _heroRace) = 0;

};

#endif // I_MAP_MANAGER_H
