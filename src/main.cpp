#include <QApplication>
#include <QTimer>
#include <QFontDatabase>

#include "ui/user_interface.h"
#include "game_logic.h"
#include "players_manager.h"
#include "map_manager.h"
#include "heroes_manager.h"
#include "towns_manager.h"
#include "battle_manager.h"
#include "units/unit_factory.h"
#include "path.h"
#include "images_paths.h"

#include "events/event_manager.h"
#include "events/event_handler.h"

constexpr int INTERVAL_MS = 100;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Path::getInstance().setPathToApp(qApp->applicationDirPath().toStdString());

    QFontDatabase::addApplicationFont(":/fonts/AncientModernTales-a7Po.ttf");

    auto mapManager     = std::make_shared<MapManager>();
    auto ui             = std::make_shared<UserInterface>(mapManager);
    auto gameLogic      = std::make_shared<GameLogic>();
    auto playersManager = std::make_shared<PlayersManager>();
    auto heroesManager  = std::make_shared<HeroesManager>();
    auto townsManager   = std::make_shared<TownsManager>();
    auto unitFactory    = std::make_shared<UnitFactory>();
    auto eventHandler   = std::make_shared<EventHandler>();
    auto battleManager  = std::make_shared<BattleManager>();

    ui->setGameLogic(gameLogic);
    ui->setPlayersManager(playersManager);
//    ui->setCursor(QCursor(QPixmap(IMAGE_CURSOR_NORMAL)));
    ui->setWindowTitle("Disciples of Heroes");

    gameLogic->setPlayersManager(playersManager);
    gameLogic->setTownsManager(townsManager);
    gameLogic->setMapManager(mapManager);
    gameLogic->setUnitFactory(unitFactory);
    gameLogic->setUi(ui);
    gameLogic->setBattleManager(battleManager);

    mapManager->setTownsManager(townsManager);
    mapManager->setHeroesManager(heroesManager);

    playersManager->setHeroManager(heroesManager);

    battleManager->setGameLogic(gameLogic);
    battleManager->setUnitFactory(unitFactory);

    EventManager::getInstance().setHandler(eventHandler);
    eventHandler->setGameLogic(gameLogic);
    eventHandler->setUseInterface(ui);

    townsManager->setUnitFantory(unitFactory);

    ui->showFullScreen();

    auto mainLoop = [gameLogic, ui, eventHandler]()
    {
        eventHandler->processEventQueue();
        ui->render();
    };

    QTimer mainTimer;
    QObject::connect(&mainTimer, &QTimer::timeout, mainLoop);

    mainTimer.start(INTERVAL_MS);

    return a.exec();
}
