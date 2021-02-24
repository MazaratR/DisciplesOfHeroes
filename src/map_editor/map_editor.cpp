#include "map_editor.h"
#include "ui_map_editor.h"

#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>

#include "grid_maker.h"
#include "map_element.h"
#include "path.h"
#include "images_paths.h"

#include "interfaces/i_map_manager.h"

MapEditor::MapEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapEditor)
{
    ui->setupUi(this);

    m_globalMap = QPixmap(ui->wgt_globalMap->size());
    ui->wgt_globalMap->setPixmapToDraw(&m_globalMap);

    ui->wgt_globalMap->installEventFilter(this);
    ui->wgt_globalMap->setMouseTracking(true);


    initTerrainTilesImages();
    initResourcesImages();
    initTownsImages();
    initHeroesImages();

    updateMapList();
}

MapEditor::~MapEditor()
{
    delete ui;
}

void MapEditor::resizeEvent(QResizeEvent *_event)
{
    Q_UNUSED(_event);

    auto& gridMaker = GridMaker::getInstance();

    auto fullScreenRect = this->rect();

    // UI width
    int newInterfaceWidth = fullScreenRect.width() * 0.2;
    newInterfaceWidth = std::max(newInterfaceWidth, 250);
    ui->groupBox->setMinimumWidth(newInterfaceWidth);

    // Main canvas size
    int gameScreenNewWidth = fullScreenRect.width() - newInterfaceWidth;
    fullScreenRect.setWidth(gameScreenNewWidth);
    ui->wgt_globalMap->setGeometry(fullScreenRect);

    gridMaker.setSizeInPixels(fullScreenRect.width(), fullScreenRect.height());
    m_globalMap = m_globalMap.scaled(fullScreenRect.width(), fullScreenRect.height());
}

void MapEditor::initTerrainTilesImages()
{
    m_terrainTilesPixmaps = {
        {TerrainType::Unknown, QPixmap(IMAGE_TERRAIN_UNKNOWN)},
        {TerrainType::Grass,   QPixmap(IMAGE_TERRAIN_GRASS  )},
        {TerrainType::Water,   QPixmap(IMAGE_TERRAIN_WATER  )},
        {TerrainType::Lava,    QPixmap(IMAGE_TERRAIN_LAVA   )},
        {TerrainType::Dirt,    QPixmap(IMAGE_TERRAIN_DIRT   )},
        {TerrainType::Snow,    QPixmap(IMAGE_TERRAIN_SNOW   )},
    };
}

void MapEditor::initResourcesImages()
{
    m_resourcesPixmaps = {
        {Convert::resourceToText(ResourceType::Gold),   QPixmap(IMAGE_RESOURCE_GOLD )},
        {Convert::resourceToText(ResourceType::Gem),    QPixmap(IMAGE_RESOURCE_GEM  )},
        {Convert::resourceToText(ResourceType::Wood),   QPixmap(IMAGE_RESOURCE_WOOD )},
        {Convert::resourceToText(ResourceType::Stone),  QPixmap(IMAGE_RESOURCE_STONE)}
    };
}

void MapEditor::initTownsImages()
{
    m_townsPixmaps = {
        {Convert::raceToText(Race::Human)  + " town", QPixmap(IMAGE_TOWN_HUMAN )},
        {Convert::raceToText(Race::Elf)    + " town", QPixmap(IMAGE_TOWN_ELF   )},
        {Convert::raceToText(Race::Dwarf)  + " town", QPixmap(IMAGE_TOWN_DWARF )},
        {Convert::raceToText(Race::Undead) + " town", QPixmap(IMAGE_TOWN_UNDEAD)}
    };
}

void MapEditor::initHeroesImages()
{
    m_heroesPixmaps = {
        {"Arthur",    QPixmap(IMAGE_HERO_ARTHUR   )},
        {"Cathrine",  QPixmap(IMAGE_HERO_CATHRINE )},
        {"Gildor",    QPixmap(IMAGE_HERO_GILDOR   )},
        {"Galadriel", QPixmap(IMAGE_HERO_GALADRIEL)},
        {"Sturmir",   QPixmap(IMAGE_HERO_STURMIR  )},
        {"Wotan",     QPixmap(IMAGE_HERO_WOTAN    )},
        {"Sandro",    QPixmap(IMAGE_HERO_SANDRO   )},
        {"Mortis",    QPixmap(IMAGE_HERO_MORTIS   )}
    };
}

QPixmap MapEditor::getPixmap(IInteractObjSharedConst &_obj) const
{
    auto findInMap = [](const std::map<std::string, QPixmap>& _map,
                        const std::string _key) -> QPixmap
    {
        auto it = _map.find(_key);
        if (it != _map.end())
            return it->second;
        else
            return QPixmap();
    };

    std::string key = _obj->title();
    switch (_obj->objectType())
    {
    case ObjectType::Hero:      return findInMap(m_heroesPixmaps,    key);
    case ObjectType::Resource:  return findInMap(m_resourcesPixmaps, key);
    case ObjectType::Town:      return findInMap(m_townsPixmaps,     key);
    default: return QPixmap();
    }
}

bool MapEditor::eventFilter(QObject *_object, QEvent *_event)
{
    if (_object == ui->wgt_globalMap)
    {
        if (_event->type() == QEvent::MouseButtonPress)
        {
            auto mouseEvent = dynamic_cast<QMouseEvent*>(_event);
            if (mouseEvent && mouseEvent->button() == Qt::LeftButton)
            {
                m_mousePressed = true;
                auto cursorPos = ui->wgt_globalMap->mapFromGlobal(QCursor::pos());
                return applyEditing(cursorPos);
            }
        }
        else if (_event->type() == QEvent::MouseButtonRelease)
        {
            auto mouseEvent = dynamic_cast<QMouseEvent*>(_event);
            if (mouseEvent && mouseEvent->button() == Qt::LeftButton)
            {
                m_mousePressed = false;
                return true;
            }
        }
        else if (_event->type() == QEvent::MouseMove)
        {
            if (m_mousePressed)
            {
                auto cursorPos = ui->wgt_globalMap->mapFromGlobal(QCursor::pos());
                return applyEditing(cursorPos);
            }
        }
    }

    return QObject::eventFilter(_object, _event);
}

void MapEditor::setMapManager(std::shared_ptr<IMapManager> _manager)
{
    m_mapManager = _manager;
}

void MapEditor::render()
{
    m_globalMap = QPixmap(m_globalMap.width(), m_globalMap.height());

    QPainter painter(&m_globalMap);
    auto& gridMaker = GridMaker::getInstance();

    Point2D_int size = m_mapManager->getMapSize();
    Point2D_int tileSize = gridMaker.getTileSize();
    Point2D_int topLeft;

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            // Render terrain
            const MapElement* mapElem = m_mapManager->getMapElement({x, y});
            topLeft = gridMaker.getTileLeftTopFromIndex(x, y);
            QPixmap tilePixmap;

            if (!mapElem)
                tilePixmap = m_terrainTilesPixmaps[TerrainType::Unknown].scaled(tileSize.x, tileSize.y);
            else if (mapElem)
                tilePixmap = m_terrainTilesPixmaps[mapElem->type()].scaled(tileSize.x, tileSize.y);

            painter.drawPixmap(topLeft.x, topLeft.y, tilePixmap);

            // Grid
            painter.drawRect(topLeft.x, topLeft.y, tilePixmap.rect().width(), tilePixmap.rect().height());

            if (!mapElem)
                continue;

            // Render interactable objects
            if (auto interactableObject = mapElem->interactableObject())
            {
                tilePixmap = getPixmap(interactableObject);
                if (tilePixmap.isNull())
                    continue;

                tilePixmap = tilePixmap.scaled(tileSize.x, tileSize.y);
                painter.drawPixmap(topLeft.x, topLeft.y, tilePixmap);
            }
        } // by Y
    } // by X

    ui->wgt_globalMap->update();
}

bool MapEditor::applyEditing(const QPoint &_coords)
{
    switch (m_editingMode)
    {
    case EditingMode::AddTerrain:   addTerrain(_coords);   break;
    case EditingMode::AddResource:  addResource(_coords);  break;
    case EditingMode::AddTown:      addTown(_coords);      break;
    case EditingMode::AddHero:      addHero(_coords);      break;
    default:                        return false;
    }

    return true;
}

void MapEditor::addTerrain(const QPoint &_coords)
{
    auto tileCoords = GridMaker::getInstance().getTileIndexFromPixels(_coords.x(), _coords.y());
    m_mapManager->setTerrainType(tileCoords, m_selectedTerrainType);
}

void MapEditor::addResource(const QPoint &_coords)
{
    auto tileCoords = GridMaker::getInstance().getTileIndexFromPixels(_coords.x(), _coords.y());
    m_mapManager->placeResource(tileCoords, m_selectedResourceType, ui->sb_resourceAmount->value());
}

void MapEditor::addTown(const QPoint &_coords)
{
    auto tileCoords = GridMaker::getInstance().getTileIndexFromPixels(_coords.x(), _coords.y());
    m_mapManager->placeTown(tileCoords, m_selectedTownRace);
}

void MapEditor::addHero(const QPoint &_coords)
{
    auto tileCoords = GridMaker::getInstance().getTileIndexFromPixels(_coords.x(), _coords.y());
    m_mapManager->placeHero(tileCoords, m_selectedHeroRace);
}

void MapEditor::updateMapList()
{
    QDir mapsDir(Path::getInstance().getPathToMaps().c_str());
    QStringList mapNames = mapsDir.entryList(QStringList({"*.map","*.MAP"}), QDir::Files);

    ui->cb_mapList->clear();
    for (const auto& mapName : mapNames)
    {
        ui->cb_mapList->addItem(mapName);
    }
}

void MapEditor::on_pb_addWater_clicked()
{
    m_editingMode = EditingMode::AddTerrain;
    m_selectedTerrainType = TerrainType::Water;
}

void MapEditor::on_pb_addLava_clicked()
{
    m_editingMode = EditingMode::AddTerrain;
    m_selectedTerrainType = TerrainType::Lava;
}

void MapEditor::on_pb_addGrass_clicked()
{
    m_editingMode = EditingMode::AddTerrain;
    m_selectedTerrainType = TerrainType::Grass;
}

void MapEditor::on_pb_addDirt_clicked()
{
    m_editingMode = EditingMode::AddTerrain;
    m_selectedTerrainType = TerrainType::Dirt;
}

void MapEditor::on_pb_addSnow_clicked()
{
    m_editingMode = EditingMode::AddTerrain;
    m_selectedTerrainType = TerrainType::Snow;
}

void MapEditor::on_pb_addTownHuman_clicked()
{
    m_editingMode = EditingMode::AddTown;
    m_selectedTownRace = Race::Human;
}

void MapEditor::on_pb_addTownElf_clicked()
{
    m_editingMode = EditingMode::AddTown;
    m_selectedTownRace = Race::Elf;
}

void MapEditor::on_pb_addTownDwarf_clicked()
{
    m_editingMode = EditingMode::AddTown;
    m_selectedTownRace = Race::Dwarf;
}

void MapEditor::on_pb_addTownUndead_clicked()
{
    m_editingMode = EditingMode::AddTown;
    m_selectedTownRace = Race::Undead;
}

void MapEditor::on_pb_addHeroHuman_clicked()
{
    m_editingMode = EditingMode::AddHero;
    m_selectedHeroRace = Race::Human;
}

void MapEditor::on_pb_addHeroElf_clicked()
{
    m_editingMode = EditingMode::AddHero;
    m_selectedHeroRace = Race::Elf;
}

void MapEditor::on_pb_addHeroDwarf_clicked()
{
    m_editingMode = EditingMode::AddHero;
    m_selectedHeroRace = Race::Dwarf;
}

void MapEditor::on_pb_addHeroUndead_clicked()
{
    m_editingMode = EditingMode::AddHero;
    m_selectedHeroRace = Race::Undead;
}

void MapEditor::on_pb_addResourceGold_clicked()
{
    m_editingMode = EditingMode::AddResource;
    m_selectedResourceType = ResourceType::Gold;
}

void MapEditor::on_pb_addResourceGem_clicked()
{
    m_editingMode = EditingMode::AddResource;
    m_selectedResourceType = ResourceType::Gem;
}

void MapEditor::on_pb_addResourceWood_clicked()
{
    m_editingMode = EditingMode::AddResource;
    m_selectedResourceType = ResourceType::Wood;
}

void MapEditor::on_pb_addResourceStone_clicked()
{
    m_editingMode = EditingMode::AddResource;
    m_selectedResourceType = ResourceType::Stone;
}

void MapEditor::on_pb_load_clicked()
{
    m_mapManager->loadMap(ui->cb_mapList->currentText().toStdString());
}

void MapEditor::on_pb_save_clicked()
{
    QString savePath = QFileDialog::getSaveFileName(this, "Select save file name", Path::getInstance().getPathToMaps().c_str(), "Map files (*.map)");
    m_mapManager->saveMap(savePath.toStdString());
    updateMapList();
}

void MapEditor::on_pb_createMap_30x20_clicked()
{
    m_mapManager->createMap(30, 20, TerrainType::Water);
}

void MapEditor::on_pb_createMap_40x30_clicked()
{
    m_mapManager->createMap(40, 30, TerrainType::Water);
}
