#include "map_manager.h"

#include <fstream>

#include "map_element.h"
#include "grid_maker.h"
#include "map_file_format.h"
#include "path.h"

#include "interfaces/i_hero.h"
#include "interfaces/i_player.h"
#include "interfaces/i_town.h"
#include "interfaces/i_towns_manager.h"
#include "interfaces/i_heroes_manager.h"

MapManager::~MapManager()
{
    if (m_map2DArray)
        delete[] m_map2DArray;
}

void MapManager::setTownsManager(std::shared_ptr<ITownsManager> _townsManager)
{
    m_townsManager = _townsManager;
}

void MapManager::setHeroesManager(std::shared_ptr<IHeroesManager> _heroesManager)
{
    m_heroesManager = _heroesManager;
}

std::vector<Race> MapManager::getIncludedRaces(const std::string &_name)
{
    using namespace MapFileFormat;

    std::vector<Race> retvalVec;
    std::string fullPath = Path::getInstance().getPathToMaps() + "/" + _name;

    // Open file
    std::ifstream file;
    file.open(fullPath.c_str(), std::ios_base::binary);
    if (!file.is_open())
        return retvalVec;

    // Read header
    FileHeader header;

    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.close();

    if (header.hasHumans)
        retvalVec.emplace_back(Race::Human);
    if (header.hasElves)
        retvalVec.emplace_back(Race::Elf);
    if (header.hasDwarves)
        retvalVec.emplace_back(Race::Dwarf);
    if (header.hasUndeads)
        retvalVec.emplace_back(Race::Undead);

    return retvalVec;
}

void MapManager::saveMap(const std::string& _fullPath)
{
    using namespace MapFileFormat;

    auto size = getMapSize();

    // Create file
    std::fstream file(_fullPath.c_str(), std::ios_base::in);
    std::ios_base::openmode mode = std::ios_base::binary | std::ios_base::out;
    if (file)
    {
        file.close();
        mode |= std::ios_base::trunc;
    }

    file.open(_fullPath.c_str(), mode);
    if (!file.is_open())
        return;

    // Fill header
    FileHeader header;
    header.mapSize_x = size.x;
    header.mapSize_y = size.y;

    // Calc players count
    auto hasHeroOfRace = [this](Race _race) -> bool {
        auto heroes = m_heroesManager->getHeroes(_race);
        for (const auto& hero : heroes)
        {
            if (hero->placedOnMap())
                return true;
        }
        return false;
    };

    header.hasHumans    = hasHeroOfRace(Race::Human);
    header.hasDwarves   = hasHeroOfRace(Race::Dwarf);
    header.hasElves     = hasHeroOfRace(Race::Elf);
    header.hasUndeads   = hasHeroOfRace(Race::Undead);

    if (!header.getPlayersCount())
        return;

    // Write header
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // Write all tiles
    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            FileElement fileElem;
            auto tile = getMapElement({x, y});
            fileElem.terrainType = static_cast<typeof(fileElem.terrainType)>(tile->type());
            if (auto obj = tile->interactableObject())
            {
                fileElem.interactObjType = static_cast<typeof(fileElem.interactObjType)> (obj->objectType());
                fileElem.setInteractObjName(getInteractObjNameForFile(obj));
                if (obj->objectType() == ObjectType::Resource)
                {
                    auto resource = std::dynamic_pointer_cast<const Resource>(obj);
                    if (resource)
                        fileElem.objectAmount = resource->value();
                }
            }

            auto elemSize = fileElem.isEmptyTile() ? fileElem.getSizeofEmptyTile()
                                                   : fileElem.getSizeOfTileWithObj();
            // Write in file
            file.write(reinterpret_cast<const char*>(&fileElem), elemSize);

        } // by X
    } // by Y

    file.close();
}

void MapManager::loadMap(const std::string &_name)
{
    using namespace MapFileFormat;

    std::string fullPath = Path::getInstance().getPathToMaps() + "/" + _name;

    // Open file
    std::ifstream file;
    file.open(fullPath.c_str(), std::ios_base::binary);
    if (!file.is_open())
        return;

    // Read header
    FileHeader header;

    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    m_size.x = header.mapSize_x;
    m_size.y = header.mapSize_y;
    GridMaker::getInstance().setTileCount(m_size.x, m_size.y);

    // Create map
    if (m_map2DArray)
        delete[] m_map2DArray;

    m_map2DArray = new MapElement[m_size.x*m_size.y];

    // Read all tiles
    size_t readPos = sizeof(header);
    for (int y = 0; y < header.mapSize_y; y++)
    {
        for (int x = 0; x < header.mapSize_x; x++)
        {
            // Read tile
            FileElement fileElem;
            // Base part
            file.seekg(readPos, std::ios_base::beg);
            file.read(reinterpret_cast<char*>(&fileElem), fileElem.getSizeofEmptyTile());

            auto tile = getMapElement({x, y});

            tile->setType(static_cast<TerrainType>(fileElem.terrainType));

            if (!fileElem.isEmptyTile())
            {
                // Read again with all data
                file.seekg(readPos, std::ios_base::beg);
                file.read(reinterpret_cast<char*>(&fileElem), fileElem.getSizeOfTileWithObj());

                ObjectType type = static_cast<ObjectType>(fileElem.interactObjType);
                auto name = fileElem.getInteractObjName();
                auto amount = fileElem.objectAmount;

                switch (type)
                {
                case ObjectType::Hero:      placeHero({x,y}, name); break;
                case ObjectType::Resource:  placeResource({x,y}, Convert::resourceFromText(name), amount); break;
                case ObjectType::Town:      placeTown({x,y}, name); break;
                default:                    break;
                }

            }

            readPos += fileElem.isEmptyTile() ? fileElem.getSizeofEmptyTile()
                                              : fileElem.getSizeOfTileWithObj();
        } // by X
    } // by Y

    file.close();
}

void MapManager::createMap(int _sizeX, int _sizeY, TerrainType _type)
{
    if (m_map2DArray)
        delete[] m_map2DArray;

    if (_sizeX < 0 || _sizeY < 0)
        return;

    m_size.x = _sizeX;
    m_size.y = _sizeY;
    GridMaker::getInstance().setTileCount(m_size.x, m_size.y);

    int size = _sizeX*_sizeY;
    m_map2DArray = new MapElement[size];
    std::fill_n(m_map2DArray, size, _type);
}

MapElement *MapManager::getMapElement(const Point2D_int &_coord)
{
    if (!isValidCoords(_coord))
        return nullptr;

    int index = getArrayIndex(_coord);
    return &m_map2DArray[index];
}

const MapElement* MapManager::getMapElement(const Point2D_int &_coord) const
{
    if (!isValidCoords(_coord))
        return nullptr;

    int index = getArrayIndex(_coord);
    return &m_map2DArray[index];
}

bool MapManager::isVisibleByPlayer(const Point2D_int &_coord, IPlayerShared &_player) const
{
    if (!isValidCoords(_coord))
        return true;

    const auto tile = getMapElement(_coord);
    return tile && tile->isVisibleByPlayer(_player);
}

Point2D_int MapManager::getMapSize() const
{
    return m_size;
}

void MapManager::processPlayerVisibility(IPlayerShared &_player)
{
    auto heroesVec = _player->getHeroes();

    for (const auto& hero : heroesVec)
    {
        auto coords = hero->tileCoord();
        auto radius = hero->params().m_radius;

        int begin_x = std::max(0,        coords.x - radius + 1);
        int begin_y = std::max(0,        coords.y - radius + 1);
        int end_x   = std::min(m_size.x, coords.x + radius);
        int end_y   = std::min(m_size.y, coords.y + radius);

        for (auto x = begin_x; x < end_x; x++)
        {
            for (auto y = begin_y; y < end_y; y++)
            {
                setVisibilityByPlayer({x, y}, _player, true);
            }
        }
    }
}

void MapManager::setTerrainType(const Point2D_int &_coord, TerrainType _terrain)
{
    if (!m_map2DArray)
        return;

    if (!isValidCoords(_coord))
        return;

    int index = getArrayIndex(_coord);

    m_map2DArray[index].setType(_terrain);
}

void MapManager::placeResource(const Point2D_int &_coord, ResourceType _type, int _amount)
{
    if (!m_map2DArray)
        return;

    if (!isValidCoords(_coord))
        return;

    int index = getArrayIndex(_coord);
    m_map2DArray[index].acceptInteractableObject(std::make_shared<Resource>(_type, _amount));
}

void MapManager::placeTown(const Point2D_int &_coord, const std::string& _name)
{
    if (!m_map2DArray)
        return;

    if (!isValidCoords(_coord))
        return;

    auto town = m_townsManager->getTown(_name);
    if (!town || town->placedOnMap())
        return;

    placeInteractableObject(_coord, town);
}

void MapManager::placeTown(const Point2D_int &_coord, Race _townRace)
{
    if (!m_map2DArray)
        return;

    if (!isValidCoords(_coord))
        return;

    auto towns = m_townsManager->getTowns(_townRace);

    // Place first available town
    for (auto& town : towns)
    {
        if (!town->placedOnMap())
        {
            placeInteractableObject(_coord, town);
            break;
        }
    }
}

void MapManager::placeHero(const Point2D_int &_coord, const std::string &_name)
{
    if (!m_map2DArray)
        return;

    if (!isValidCoords(_coord))
        return;

    auto hero = m_heroesManager->getHero(_name);
    if (!hero || hero->placedOnMap())
        return;

    placeInteractableObject(_coord, hero);
}

void MapManager::placeHero(const Point2D_int &_coord, Race _heroRace)
{
    if (!m_map2DArray)
        return;

    if (!isValidCoords(_coord))
        return;

    auto heroes = m_heroesManager->getHeroes(_heroRace);

    // Place first available town
    for (auto& hero : heroes)
    {
        if (!hero->placedOnMap())
        {
            placeInteractableObject(_coord, hero);
            break;
        }
    }
}

void MapManager::setMapElement(const Point2D_int &_coord, const MapElement &_element)
{
    if (!m_map2DArray)
        return;

    if (!isValidCoords(_coord))
        return;

    int index = getArrayIndex(_coord);

    m_map2DArray[index] = _element;
}

void MapManager::setVisibilityByPlayer(const Point2D_int& _coord,
                                       IPlayerShared _player,
                                       bool _isVisible)
{
    auto tile = getMapElement(_coord);
    if (!tile)
        return;

    tile->setVisibilityByPlayer(_player, _isVisible);
}

int MapManager::getArrayIndex(const Point2D_int &_coord) const
{
    return _coord.x + _coord.y * m_size.x;
}

bool MapManager::isValidCoords(const Point2D_int &_coord) const
{
    return (_coord.x < m_size.x) && (_coord.y < m_size.y);
}

void MapManager::placeInteractableObject(const Point2D_int &_coord, IInteractObjShared _obj)
{
    _obj->moveToTile(_coord);
    _obj->setPlacedOnMap(true);
    int index = getArrayIndex(_coord);

    if (auto prevObj = m_map2DArray[index].interactableObject())
        prevObj->setPlacedOnMap(false);

    m_map2DArray[index].acceptInteractableObject(_obj);

}

std::string MapManager::getInteractObjNameForFile(IInteractObjShared _obj) const
{
    switch (_obj->objectType())
    {
    case ObjectType::Town:
    {
        auto town = std::dynamic_pointer_cast<ITown>(_obj);
        if (town)
            return town->name();
        else
            return _obj->title();
    }
    default: return _obj->title();
    }
}
