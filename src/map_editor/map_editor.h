#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include <QMainWindow>

#include <memory>

#include "terrain.h"
#include "races.h"
#include "resource_types.h"

#include "forward/i_interactable_object_forward.h"

namespace Ui {
class MapEditor;
}

class IMapManager;

class MapEditor : public QMainWindow
{
    Q_OBJECT

    enum class EditingMode : std::uint8_t
    {
        None = 0,
        AddTerrain,
        AddTown,
        AddHero,
        AddResource
    };

public:
    explicit MapEditor(QWidget *parent = nullptr);
    ~MapEditor();

    void resizeEvent(QResizeEvent* _event) override;
    bool eventFilter(QObject* _object, QEvent* _event) override;

    void setMapManager(std::shared_ptr<IMapManager> _manager);

    void render();

private slots:
    void on_pb_addWater_clicked();
    void on_pb_addLava_clicked();
    void on_pb_addGrass_clicked();
    void on_pb_addDirt_clicked();
    void on_pb_addSnow_clicked();

    void on_pb_addTownHuman_clicked();
    void on_pb_addTownElf_clicked();
    void on_pb_addTownDwarf_clicked();
    void on_pb_addTownUndead_clicked();

    void on_pb_addHeroHuman_clicked();
    void on_pb_addHeroElf_clicked();
    void on_pb_addHeroDwarf_clicked();
    void on_pb_addHeroUndead_clicked();

    void on_pb_addResourceGold_clicked();
    void on_pb_addResourceGem_clicked();
    void on_pb_addResourceWood_clicked();
    void on_pb_addResourceStone_clicked();

    void on_pb_load_clicked();
    void on_pb_save_clicked();

    void on_pb_createMap_30x20_clicked();
    void on_pb_createMap_40x30_clicked();

private:
    void initTerrainTilesImages();
    void initResourcesImages();
    void initTownsImages();
    void initHeroesImages();

    QPixmap getPixmap(IInteractObjSharedConst& _obj) const;

    bool applyEditing(const QPoint& _coords);

    void addTerrain(const QPoint& _coords);
    void addResource(const QPoint& _coords);
    void addTown(const QPoint& _coords);
    void addHero(const QPoint& _coords);

    void updateMapList();

private:
    Ui::MapEditor *ui;
    std::shared_ptr<IMapManager> m_mapManager;
    QPixmap m_globalMap;

    std::map<TerrainType, QPixmap> m_terrainTilesPixmaps;
    std::map<std::string, QPixmap> m_resourcesPixmaps;
    std::map<std::string, QPixmap> m_townsPixmaps;
    std::map<std::string, QPixmap> m_heroesPixmaps;

    bool m_mousePressed = false;

    EditingMode m_editingMode = EditingMode::None;
    TerrainType m_selectedTerrainType = TerrainType::Water;
    Race m_selectedHeroRace = Race::Unknown;
    Race m_selectedTownRace = Race::Unknown;
    ResourceType m_selectedResourceType = ResourceType::Unknown;

};

#endif // MAP_EDITOR_H
