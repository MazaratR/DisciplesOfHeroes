#include "map_editor.h"

#include <QApplication>
#include <QTimer>
#include <QFontDatabase>

#include "game_logic.h"
#include "players_manager.h"
#include "map_manager.h"
#include "heroes_manager.h"
#include "towns_manager.h"
#include "battle_manager.h"
#include "units/unit_factory.h"
#include "path.h"

#include "events/event_manager.h"
#include "events/event_handler.h"

constexpr int INTERVAL_MS = 100;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Path::getInstance().setPathToApp(qApp->applicationDirPath().toStdString());

    QFontDatabase::addApplicationFont(":/fonts/AncientModernTales-a7Po.ttf");

    auto ui             = std::make_shared<MapEditor>();
    auto mapManager     = std::make_shared<MapManager>();
    auto heroesManager  = std::make_shared<HeroesManager>();
    auto townsManager   = std::make_shared<TownsManager>();
    auto unitFactory    = std::make_shared<UnitFactory>();

    ui->setMapManager(mapManager);
    ui->setWindowTitle("Map editor");

    townsManager->setUnitFantory(unitFactory);

    mapManager->setTownsManager(townsManager);
    mapManager->setHeroesManager(heroesManager);

    ui->showFullScreen();

    auto mainLoop = [ui]()
    {
        ui->render();
    };

    QTimer mainTimer;
    QObject::connect(&mainTimer, &QTimer::timeout, mainLoop);

    mainTimer.start(INTERVAL_MS);

    return a.exec();
}
