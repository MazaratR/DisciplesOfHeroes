#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <QMainWindow>
#include <memory>

#include "terrain.h"
#include "races.h"

#include "units/unit_types.h"
#include "units/unit_position.h"
#include "units/unit_levels.h"

#include "interfaces/i_user_interface.h"

#include "forward/i_interactable_object_forward.h"
#include "forward/i_unit_forward.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UserInterface; }
QT_END_NAMESPACE

class IGameLogic;
class IMapManager;
class IPlayersManager;
class IInteractableObject;
class BattleResultView;
class NotificationView;
class PlayerSettings;
class UnitStatsView;

struct BattleResult;

class UserInterface
        : public QMainWindow
        , public IUserInterface
{
    Q_OBJECT

public:
    UserInterface(std::shared_ptr<IMapManager> _manager = nullptr);
    ~UserInterface();
    void resizeEvent(QResizeEvent* _event) override;
    bool eventFilter(QObject* _object, QEvent* _event) override;
    void keyPressEvent(QKeyEvent* _keyevent) override;

    void setGameLogic(std::shared_ptr<IGameLogic> _logic);
    void setPlayersManager(std::shared_ptr<IPlayersManager> _manager);

    void render();

    void battleBegin(IHeroShared& _attacker, IHeroShared& _defender) override;
    void battleEnd(const BattleResult& _result) override;
    void arrivalAtTown(ITownShared& _town, IHeroShared& _hero) override;
    void onNewWeek() override;
    void onNotify(const std::string& _text) override;

private:
    void initTerrainTilesImages();
    void initResourcesImages();
    void initTownsImages();
    void initHeroesImages();
    void initUnitImages();

    void renderUi();
    void renderHeroesUi();
    void renderTownsUi();
    void renderArmyUi();
    void renderResourcesUi();
    void renderCalendar();

    void prepareDataForRenderGlobalMap();
    void paintTerrainAndInteractObjects();
    void paintObjectsHighlight();

    void renderBattlefield();
    void renderTownView();

    QString extractHeroHame(const QString &_text);
    QPixmap getPixmap(IInteractObjSharedConst& _obj) const;
    void configPlayersSettings(std::vector<Race> _races);

private slots:
    void on_cb_map_currentTextChanged(const QString& _mapFileName);

    void on_pb_startGame_clicked();

    void on_pb_hero_1_clicked();
    void on_pb_hero_2_clicked();

    void on_pb_nextTurn_clicked();

    void onSurrender();
    void onSkipMove();
    void onAttackUnitOnRight(UnitPosition _pos);
    void onAttackUnitOnLeft(UnitPosition _pos);
    void onShowUnitInfo(IUnitSharedConst _unit, int _x, int _y);
    void stopShowUnitInfo();

    void onBattleResultViewClose();
    void onNotificationViewClose();

    void onExitTown();
    void onHeroStayAsGarrison(ITownShared _town, IHeroShared _hero);
    void onHireUnitInTown(ITownShared _town, IHeroShared _hero, UnitLevel _level, int _amount);
    void onSwapUnits(IUnitSharedConst _first, IUnitSharedConst _second);


private:
    Ui::UserInterface *ui;
    std::shared_ptr<IGameLogic> m_gameLogic;
    std::shared_ptr<IMapManager> m_mapManager;
    std::shared_ptr<IPlayersManager> m_playersManager;

    // Settings
    uint8_t m_playersCount = 0;
    std::vector<PlayerSettings*> m_playerSettingsVec;

    // Global map and textures
    QPixmap m_globalMap;
    std::map<TerrainType, QPixmap> m_terrainTilesPixmaps;
    std::map<std::string, QPixmap> m_resourcesPixmaps;
    std::map<std::string, QPixmap> m_townsPixmaps;
    std::map<std::string, QPixmap> m_heroesPixmaps;
    std::map<UnitType,    QPixmap> m_unitPixmaps;

    // Additional dialogs
    std::shared_ptr<BattleResultView> m_battleResultView;
    std::shared_ptr<NotificationView> m_notificationView;
    std::shared_ptr<UnitStatsView>    m_unitStatsView;

};
#endif // USER_INTERFACE_H
