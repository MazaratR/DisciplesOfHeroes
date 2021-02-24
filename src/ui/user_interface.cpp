#include "user_interface.h"
#include "ui_user_interface.h"

#include <QAccessible>
#include <QPainter>
#include <QElapsedTimer>
#include <QDir>
#include <QKeyEvent>

#include <iostream>
#include <cmath>

#include "colors.h"
#include "races.h"
#include "grid_maker.h"
#include "map_element.h"
#include "calendar.h"
#include "units/unit_types.h"
#include "randomizer.h"
#include "attack_result.h"
#include "images_paths.h"
#include "path.h"
#include "move_directions.h"

#include "events/event_manager.h"

#include "ui/battle_result_view.h"
#include "ui/notification_view.h"
#include "ui/unit_stats_view.h"

#include "interfaces/i_hero.h"
#include "interfaces/i_town.h"
#include "interfaces/i_game_logic.h"
#include "interfaces/i_map_manager.h"
#include "interfaces/i_players_manager.h"
#include "interfaces/i_player.h"
#include "interfaces/i_unit.h"

UserInterface::UserInterface(std::shared_ptr<IMapManager> _manager)
    : QMainWindow(nullptr)
    , ui(new Ui::UserInterface)
    , m_mapManager(_manager)
{
    ui->setupUi(this);
    ui->mainStack->setFocusPolicy(Qt::NoFocus);
    ui->pb_hero_1->setFocusPolicy(Qt::NoFocus);
    ui->pb_hero_2->setFocusPolicy(Qt::NoFocus);
    ui->pb_nextTurn->setFocusPolicy(Qt::NoFocus);
    ui->wgt_resources->setFocusPolicy(Qt::NoFocus);

    m_playerSettingsVec = {
        ui->wgt_player1,
        ui->wgt_player2,
        ui->wgt_player3,
        ui->wgt_player4,
    };

    QDir mapsDir(Path::getInstance().getPathToMaps().c_str());
    QStringList mapNames = mapsDir.entryList(QStringList({"*.map","*.MAP"}), QDir::Files);

    for (const auto& mapName : mapNames)
    {
        ui->cb_map->addItem(mapName);
    }

    m_globalMap = QPixmap(ui->wgt_globalMap->size());
    ui->wgt_globalMap->setPixmapToDraw(&m_globalMap);

    installEventFilter(this);
    ui->wgt_globalMap->installEventFilter(this);
    ui->wgt_globalMap->setMouseTracking(true);
    ui->wgt_globalMap->setGeometry(0,0,800,600);

    initTerrainTilesImages();
    initResourcesImages();
    initHeroesImages();
    initTownsImages();
    initUnitImages();

    ui->wgt_armyLayout->setTitle("Army");
    ui->wgt_armyLayout->setUnitsPixmap(&m_unitPixmaps);
    ui->wgt_battlefield->setUnitsPixmap(&m_unitPixmaps);
    ui->wgt_townView->setUnitsPixmap(&m_unitPixmaps);

    connect(ui->wgt_battlefield, &Battlefield::surrender,         this, &UserInterface::onSurrender);
    connect(ui->wgt_battlefield, &Battlefield::skipMove,          this, &UserInterface::onSkipMove);
    connect(ui->wgt_battlefield, &Battlefield::attackUnitOnRight, this, &UserInterface::onAttackUnitOnRight);
    connect(ui->wgt_battlefield, &Battlefield::attackUnitOnLeft,  this, &UserInterface::onAttackUnitOnLeft);
    connect(ui->wgt_battlefield, &Battlefield::showUnitInfo,      this, &UserInterface::onShowUnitInfo);
    connect(ui->wgt_battlefield, &Battlefield::stopShowUnitInfo,  this, &UserInterface::stopShowUnitInfo);

    connect(ui->wgt_townView, &TownView::exitFromTown,            this, &UserInterface::onExitTown);
    connect(ui->wgt_townView, &TownView::heroStayAsGarrison,      this, &UserInterface::onHeroStayAsGarrison);
    connect(ui->wgt_townView, &TownView::hireUnitInTown,          this, &UserInterface::onHireUnitInTown);
    connect(ui->wgt_townView, &TownView::hireUnitInTown,          this, &UserInterface::onHireUnitInTown);
    connect(ui->wgt_townView, &TownView::swapUnits,               this, &UserInterface::onSwapUnits);
    connect(ui->wgt_townView, &TownView::showUnitInfo,            this, &UserInterface::onShowUnitInfo);
    connect(ui->wgt_townView, &TownView::stopShowUnitInfo,        this, &UserInterface::stopShowUnitInfo);

    connect(ui->wgt_armyLayout, &ArmyLayout::swapUnits,           this, &UserInterface::onSwapUnits);
    connect(ui->wgt_armyLayout, &ArmyLayout::showUnitInfo,        this, &UserInterface::onShowUnitInfo);
    connect(ui->wgt_armyLayout, &ArmyLayout::stopShowUnitInfo,    this, &UserInterface::stopShowUnitInfo);

//    QCoreApplication::instance()->installEventFilter(this);
}

UserInterface::~UserInterface()
{
    delete ui;
}

void UserInterface::resizeEvent(QResizeEvent *_event)
{
    Q_UNUSED(_event);

    auto& gridMaker = GridMaker::getInstance();

    auto fullScreenRect = this->rect();

    // UI width
    int newInterfaceWidth = fullScreenRect.width() * 0.2;
    newInterfaceWidth = std::max(newInterfaceWidth, 250);
    ui->gb_gameInterface->setMinimumWidth(newInterfaceWidth);

    // Main canvas size
    int gameScreenNewWidth = fullScreenRect.width() - newInterfaceWidth;
    fullScreenRect.setWidth(gameScreenNewWidth);
    ui->wgt_globalMap->setGeometry(fullScreenRect);

    gridMaker.setSizeInPixels(fullScreenRect.width(), fullScreenRect.height());
    m_globalMap = m_globalMap.scaled(fullScreenRect.width(), fullScreenRect.height());
}

bool UserInterface::eventFilter(QObject *_object, QEvent *_event)
{
    if (_object == ui->wgt_globalMap)
    {
        if (_event->type() == QEvent::MouseButtonPress)
        {
            auto cursorPos = ui->wgt_globalMap->mapFromGlobal(QCursor::pos());
            auto mouseEvent = dynamic_cast<QMouseEvent*>(_event);
            if (mouseEvent && mouseEvent->button() == Qt::LeftButton)
            {
                auto tileCoord = GridMaker::getInstance().getTileIndexFromPixels(cursorPos.x(), cursorPos.y());
                auto tile = m_mapManager->getMapElement(tileCoord);
                if (auto obj = tile->interactableObject())
                {
                    if (obj->objectType() == ObjectType::Hero)
                        m_gameLogic->onHeroSelected(obj->title());
                    else if (obj->objectType() == ObjectType::Town)
                    {
                        auto town = std::dynamic_pointer_cast<ITown>(obj);
                        if (town && town->garrisonHero())
                            m_gameLogic->onHeroSelected(town->garrisonHero()->name());
                    }
                }

                return true;
            }
            else if (mouseEvent && mouseEvent->button() == Qt::RightButton)
            {

                /// \todo show object info

                return true;
            }
        }
    }

    return QObject::eventFilter(_object, _event);
}

void UserInterface::keyPressEvent(QKeyEvent *_keyevent)
{
    if (ui->mainStack->currentWidget() != ui->page_globalMap)
        return;

    /// \!warning Only for ENG keyboard

    if (_keyevent->key() == Qt::Key_W)
        m_gameLogic->moveCurrentHero(MoveDirection::Up);
    if (_keyevent->key() == Qt::Key_A)
        m_gameLogic->moveCurrentHero(MoveDirection::Left);
    if (_keyevent->key() == Qt::Key_S)
        m_gameLogic->moveCurrentHero(MoveDirection::Down);
    if (_keyevent->key() == Qt::Key_D)
        m_gameLogic->moveCurrentHero(MoveDirection::Right);
}

void UserInterface::setGameLogic(std::shared_ptr<IGameLogic> _logic)
{
    m_gameLogic = _logic;
}

void UserInterface::setPlayersManager(std::shared_ptr<IPlayersManager> _manager)
{
    m_playersManager = _manager;
}

void UserInterface::render()
{
    if (ui->mainStack->currentWidget() == ui->page_globalMap)
    {
        renderUi();
        prepareDataForRenderGlobalMap();
    }
    else if (ui->mainStack->currentWidget() == ui->page_battlefield)
        renderBattlefield();
    else if (ui->mainStack->currentWidget() == ui->page_townView)
        renderTownView();
}

void UserInterface::battleBegin(IHeroShared& _attacker, IHeroShared& _defender)
{
    ui->wgt_battlefield->setParticipants(_attacker, _defender);
    ui->mainStack->setCurrentWidget(ui->page_battlefield);
}

void UserInterface::battleEnd(const BattleResult &_result)
{
    ui->mainStack->setCurrentWidget(ui->page_globalMap);

    m_battleResultView = std::make_shared<BattleResultView>(this, _result, &m_unitPixmaps);
    connect(m_battleResultView.get(), &BattleResultView::close,
            this,                     &UserInterface::onBattleResultViewClose);

    m_battleResultView->show();
}

void UserInterface::arrivalAtTown(ITownShared &_town, IHeroShared &_hero)
{
    ui->wgt_townView->setDisplayedTown(_town);
    ui->wgt_townView->setArrivedHero(_hero);
    ui->wgt_townView->resetUi();

    ui->mainStack->setCurrentWidget(ui->page_townView);
}

void UserInterface::onNewWeek()
{
    QString notificationText = "New week is upon us! "
                               "More recruits are now available at all towns";
    m_notificationView = std::make_shared<NotificationView>(this, notificationText);
    connect(m_notificationView.get(), &NotificationView::close,
            this,                     &UserInterface::onNotificationViewClose);

    m_notificationView->show();
}

void UserInterface::onNotify(const std::string &_text)
{
    m_notificationView = std::make_shared<NotificationView>(this, _text.c_str());
    connect(m_notificationView.get(), &NotificationView::close,
            this,                     &UserInterface::onNotificationViewClose);

    m_notificationView->show();
}

void UserInterface::initTerrainTilesImages()
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

void UserInterface::initResourcesImages()
{
    m_resourcesPixmaps = {
        {Convert::resourceToText(ResourceType::Gold),   QPixmap(IMAGE_RESOURCE_GOLD )},
        {Convert::resourceToText(ResourceType::Gem),    QPixmap(IMAGE_RESOURCE_GEM  )},
        {Convert::resourceToText(ResourceType::Wood),   QPixmap(IMAGE_RESOURCE_WOOD )},
        {Convert::resourceToText(ResourceType::Stone),  QPixmap(IMAGE_RESOURCE_STONE)}
    };
}

void UserInterface::initTownsImages()
{
    m_townsPixmaps = {
        {Convert::raceToText(Race::Human)  + " town", QPixmap(IMAGE_TOWN_HUMAN )},
        {Convert::raceToText(Race::Elf)    + " town", QPixmap(IMAGE_TOWN_ELF   )},
        {Convert::raceToText(Race::Dwarf)  + " town", QPixmap(IMAGE_TOWN_DWARF )},
        {Convert::raceToText(Race::Undead) + " town", QPixmap(IMAGE_TOWN_UNDEAD)}
    };
}

void UserInterface::initHeroesImages()
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

void UserInterface::initUnitImages()
{
    m_unitPixmaps = {
        {UnitType::Swordsman,  QPixmap(IMAGE_UNIT_SWORDSMAN   )},
        {UnitType::Archer,     QPixmap(IMAGE_UNIT_ARCHER      )},
        {UnitType::Monk,       QPixmap(IMAGE_UNIT_MONK        )},
        {UnitType::Angel,      QPixmap(IMAGE_UNIT_ANGEL       )},

        {UnitType::Wisp,       QPixmap(IMAGE_UNIT_WISP        )},
        {UnitType::Sniper,     QPixmap(IMAGE_UNIT_SNIPER      )},
        {UnitType::Pegas,      QPixmap(IMAGE_UNIT_PEGAS       )},
        {UnitType::GreenDragon,QPixmap(IMAGE_UNIT_GREEN_DRAGON)},

        {UnitType::Shieldman,  QPixmap(IMAGE_UNIT_SHIELDMAN   )},
        {UnitType::Axethrower, QPixmap(IMAGE_UNIT_AXETHROWER  )},
        {UnitType::Axemaster,  QPixmap(IMAGE_UNIT_AXEMASTER   )},
        {UnitType::Runemaster, QPixmap(IMAGE_UNIT_RUNEMASTER  )},

        {UnitType::Skeleton,   QPixmap(IMAGE_UNIT_SKELETON    )},
        {UnitType::Vampire,    QPixmap(IMAGE_UNIT_VAMPIRE     )},
        {UnitType::Lich,       QPixmap(IMAGE_UNIT_LICH        )},
        {UnitType::BoneDragon, QPixmap(IMAGE_UNIT_BONE_DRAGON )}
    };
}

void UserInterface::renderUi()
{
    constexpr static int fastUpdatePeriod_ms = 16;
    constexpr static int slowUpdatePeriod_ms = 100;
    static QElapsedTimer fastTimer;
    if (!fastTimer.isValid())
        fastTimer.start();
    static QElapsedTimer slowTimer;
    if (!slowTimer.isValid())
        slowTimer.start();

    // Fast update
    if (fastTimer.elapsed() >= fastUpdatePeriod_ms)
    {
        renderHeroesUi();

        fastTimer.restart();
    }

    // Slow update
    if (slowTimer.elapsed() >= slowUpdatePeriod_ms)
    {
        renderTownsUi();
        renderArmyUi();
        renderResourcesUi();
        renderCalendar();

        slowTimer.restart();
    }
}

void UserInterface::renderHeroesUi()
{
    static std::vector<QPushButton*> iconsVec = {ui->pb_hero_1,
                                                 ui->pb_hero_2};
    auto player = m_gameLogic->getCurrentPlayer();

    if (!player)
        return;

    if (player->isHumanControl())
    {
        const auto& heroesVec = player->getHeroes();
        for (unsigned i = 0; i < iconsVec.size(); i++)
        {
            auto icon = iconsVec.at(i);
            IHeroShared hero;
            if (i < heroesVec.size())
                hero = heroesVec.at(i);

            QString heroText;
            if (hero)
            {
                heroText = hero->name().c_str();
                auto currentMovePoints = hero->movePointsLeft();
                auto maxMovePoints = hero->movePointsMax();
                heroText += " (" + QString::number(currentMovePoints) +  "/" +
                                   QString::number(maxMovePoints) + ")";
            }

            icon->setEnabled(hero.get());
            icon->setText(heroText);
        }
    }
}

void UserInterface::renderTownsUi()
{
    /// \todo After town progression implemented
}

void UserInterface::renderArmyUi()
{
    IHeroSharedConst hero = m_gameLogic->getSelectedHero();
    ui->wgt_armyLayout->setRenderData(hero);
}

void UserInterface::renderResourcesUi()
{
    auto player = m_gameLogic->getCurrentPlayer();

    if (!player)
        return;

    if (player->isHumanControl())
    {
        const auto& resourcesMap = player->getResourceMap();
        ui->wgt_resources->updateResourceAmount(resourcesMap);
    }
}

void UserInterface::renderCalendar()
{
    auto& calendar = Calendar::getInstance();
    auto day = calendar.getDay();
    auto week = calendar.getWeek();
    auto month = calendar.getMonth();

    QString dateStr = "Day:" + QString::number(day) + "   Week: " + QString::number(week) +
                      "   Month: " + QString::number(month);

    ui->lbl_date->setText(dateStr);
}

void UserInterface::prepareDataForRenderGlobalMap()
{
    if (ui->mainStack->currentWidget() != ui->page_globalMap)
        return;

    m_globalMap = QPixmap(m_globalMap.width(), m_globalMap.height());

    paintTerrainAndInteractObjects();
    paintObjectsHighlight();

    ui->wgt_globalMap->update();
}

void UserInterface::paintTerrainAndInteractObjects()
{
    QPainter painter(&m_globalMap);
    auto& gridMaker = GridMaker::getInstance();

    Point2D_int size = m_mapManager->getMapSize();
    Point2D_int tileSize = gridMaker.getTileSize();
    Point2D_int topLeft;

    auto currentPlayer = m_gameLogic->getCurrentPlayer();

    if (currentPlayer && currentPlayer->isHumanControl())
    {
        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                // Render terrain
                bool invisibleTile = !m_mapManager->isVisibleByPlayer({x, y}, currentPlayer);
                const MapElement* tile = m_mapManager->getMapElement({x, y});
                topLeft = gridMaker.getTileLeftTopFromIndex(x, y);
                QPixmap tilePixmap;

                if (invisibleTile || !tile)
                    tilePixmap = m_terrainTilesPixmaps[TerrainType::Unknown].scaled(tileSize.x, tileSize.y);
                else if (tile)
                    tilePixmap = m_terrainTilesPixmaps[tile->type()].scaled(tileSize.x, tileSize.y);

                painter.drawPixmap(topLeft.x, topLeft.y, tilePixmap);

                // Debug grid
                // painter.drawRect(topLeft.x, topLeft.y, tilePixmap.rect().width(), tilePixmap.rect().height());

                if (!tile || invisibleTile)
                    continue;

                // Render interactable objects
                if (auto interactableObject = tile->interactableObject())
                {
                    tilePixmap = getPixmap(interactableObject);
                    if (tilePixmap.isNull())
                        continue;

                    tilePixmap = tilePixmap.scaled(tileSize.x, tileSize.y);
                    painter.drawPixmap(topLeft.x, topLeft.y, tilePixmap);
                }
            } // by Y
        } // by X
    } // if Player controlled by human
}

void UserInterface::paintObjectsHighlight()
{
    QPainter painter(&m_globalMap);
    auto& gridMaker = GridMaker::getInstance();

    Point2D_int tileSize = gridMaker.getTileSize();
    Point2D_int tileCoords;
    Point2D_int topLeft;
    Point2D_int center;

    auto currentPlayer = m_gameLogic->getCurrentPlayer();

    auto players = m_playersManager->getPlayersVec();
    for (auto player : players)
    {
        QPen pen(QBrush(QColor(Convert::colorToHexValue(player->color()))), 2);
        painter.setPen(pen);

        // Heroes
        auto heroesVec = player->getHeroes();
        for (const auto& hero : heroesVec)
        {
            tileCoords = hero->tileCoord();
            if (!m_mapManager->isVisibleByPlayer(tileCoords, currentPlayer))
                continue;

            bool highlightHero = true;
            if (hero == m_gameLogic->getSelectedHero())
            {
                // Blink
                static bool blink = false;
                static QElapsedTimer blinkTimer;
                if (!blinkTimer.isValid())
                    blinkTimer.start();

                if (blinkTimer.elapsed() >= 300)
                {
                    blinkTimer.restart();
                    blink = !blink;
                }

                highlightHero = !blink;
            }

            // Highlight heroes
            if (highlightHero)
            {
                topLeft = gridMaker.getTileLeftTopFromIndex(tileCoords.x, tileCoords.y);
                painter.drawRect(topLeft.x, topLeft.y, tileSize.x, tileSize.y);
            }
        } // by heroes

        // Towns
        auto towns = player->getTowns();
        for (const auto& town : towns)
        {
            tileCoords = town->tileCoord();
            if (!m_mapManager->isVisibleByPlayer(tileCoords, currentPlayer))
                continue;


            bool highlightTown = true;
            if (auto hero = town->garrisonHero())
            {
                auto tilePixmap = m_heroesPixmaps[hero->name()].scaled(tileSize.x/2, tileSize.y/2);
                if (tilePixmap.isNull())
                    continue;

                highlightTown = town->garrisonHero() != m_gameLogic->getSelectedHero();

                // Render hero in town
                center = gridMaker.getTileCenterFromIndex(tileCoords.x, tileCoords.y);
                painter.drawPixmap(center.x, center.y, tilePixmap);
            }

            // Highlight towns
            if (highlightTown)
            {
                topLeft = gridMaker.getTileLeftTopFromIndex(tileCoords.x, tileCoords.y);
                painter.drawRect(topLeft.x, topLeft.y, tileSize.x, tileSize.y);
            }
        } // by towns
    } // by Players
}

void UserInterface::renderBattlefield()
{
    constexpr static int updatePeriod_ms = 100;
    static QElapsedTimer timer;
    if (!timer.isValid())
        timer.start();

    if (timer.elapsed() >= updatePeriod_ms)
        ui->wgt_battlefield->setRenderData(m_gameLogic->getBattleQueueVec());
}

void UserInterface::renderTownView()
{
    constexpr static int updatePeriod_ms = 100;
    static QElapsedTimer timer;
    if (!timer.isValid())
        timer.start();

    if (timer.elapsed() >= updatePeriod_ms)
        ui->wgt_townView->renderView();
}

QString UserInterface::extractHeroHame(const QString& _text)
{
    // There is a move points count after hero's name, devided by space
    auto index = _text.lastIndexOf(" ");
    return QString(_text.data(), index);
}

QPixmap UserInterface::getPixmap(IInteractObjSharedConst &_obj) const
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

void UserInterface::configPlayersSettings(std::vector<Race> _races)
{
    m_playersCount = _races.size();

    for (unsigned i = 0; i < m_playerSettingsVec.size(); i++)
    {
        auto settings = m_playerSettingsVec.at(i);
        bool visible = i < m_playersCount;
        if (visible)
            settings->configAsPlayer(i+1, _races.at(i));

        settings->setVisible(visible);
    }
}

void UserInterface::on_pb_startGame_clicked()
{
    if (!m_gameLogic)
        return;

    // Must go fist, so we can get info about heroes on the map
    m_gameLogic->onMapSelected(ui->cb_map->currentText().toStdString());

    for (unsigned i = 0 ; i < m_playersCount; i++)
    {
        auto wgt_setting = m_playerSettingsVec.at(i);
        m_gameLogic->createPlayer(ControlSide::Human,
                                  wgt_setting->getSelectedRace(),
                                  wgt_setting->getSelectedColor());
    }

    m_gameLogic->onStartGame();

    ui->mainStack->setCurrentWidget(ui->page_globalMap);
}

void UserInterface::on_pb_hero_1_clicked()
{
    auto name = extractHeroHame(ui->pb_hero_1->text());
    m_gameLogic->onHeroSelected(name.toStdString());
}

void UserInterface::on_pb_hero_2_clicked()
{
    auto name = extractHeroHame(ui->pb_hero_2->text());
    m_gameLogic->onHeroSelected(name.toStdString());
}

void UserInterface::on_pb_nextTurn_clicked()
{
    m_gameLogic->onNextTurn();
}

void UserInterface::onSurrender()
{
    m_gameLogic->surrender();
}

void UserInterface::onSkipMove()
{
    m_gameLogic->skipMove();
}

void UserInterface::onAttackUnitOnRight(UnitPosition _pos)
{
    auto result = m_gameLogic->processAttack(_pos, false);
    ui->wgt_battlefield->renderAttackResult(result);
}

void UserInterface::onAttackUnitOnLeft(UnitPosition _pos)
{
    auto result = m_gameLogic->processAttack(_pos, true);
    ui->wgt_battlefield->renderAttackResult(result);
}

void UserInterface::onShowUnitInfo(IUnitSharedConst _unit, int _x, int _y)
{
    stopShowUnitInfo();

    auto coords = this->mapFromGlobal({_x, _y});

    m_unitStatsView = std::make_shared<UnitStatsView>(this, _unit);

    // Check if window is fit free space
    if (coords.x() + m_unitStatsView->width() > this->width())
        coords.setX(coords.x() - m_unitStatsView->width());
    if (coords.y() + m_unitStatsView->height() > this->height())
        coords.setY(coords.y() - m_unitStatsView->height());

    m_unitStatsView->move(coords);
    m_unitStatsView->show();
}

void UserInterface::stopShowUnitInfo()
{
    m_unitStatsView.reset();
}

void UserInterface::onBattleResultViewClose()
{
    m_battleResultView.reset();
    EventManager::getInstance().currentEventEnds();
}

void UserInterface::onNotificationViewClose()
{
    m_notificationView.reset();
    EventManager::getInstance().currentEventEnds();
}

void UserInterface::onExitTown()
{
    ui->mainStack->setCurrentWidget(ui->page_globalMap);
}

void UserInterface::onHeroStayAsGarrison(ITownShared _town, IHeroShared _hero)
{
    m_gameLogic->setHeroAsGarrison(_town, _hero);
    ui->mainStack->setCurrentWidget(ui->page_globalMap);
}

void UserInterface::onHireUnitInTown(ITownShared _town, IHeroShared _hero, UnitLevel _level, int _amount)
{
    m_gameLogic->hireUnitInTown(_town, _hero, _level, _amount);
}

void UserInterface::onSwapUnits(IUnitSharedConst _first, IUnitSharedConst _second)
{
    m_gameLogic->swapUnits(_first, _second);
}

void UserInterface::on_cb_map_currentTextChanged(const QString &_mapFileName)
{
    auto races = m_mapManager->getIncludedRaces(_mapFileName.toStdString());
    configPlayersSettings(races);
}
