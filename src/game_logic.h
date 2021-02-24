#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <map>
#include <memory>
#include <queue>

#include "interfaces/i_game_logic.h"

class IUnitFactory;
class IPlayersManager;
class IMapManager;
class IUserInterface;
class IBattleManager;
class ITownsManager;

class GameLogic
        : public IGameLogic
{
public:
    GameLogic() = default;

    void createPlayer(ControlSide _controlSide,
                      Race _race,
                      Color _color) override;
    void onMapSelected(const std::string &_mapName) override;
    void onStartGame() override;
    void moveCurrentHero(MoveDirection _dir) override;
    void onNextTurn() override;
    void onNewWeek() override;

    IPlayerShared getCurrentPlayer() override;
    void onHeroSelected(const std::string& _name) override;

    void setPlayersManager(std::shared_ptr<IPlayersManager> _manager);
    void setTownsManager(std::shared_ptr<ITownsManager> _townsManager);
    void setMapManager(std::shared_ptr<IMapManager> _manager);
    void setUnitFactory(std::shared_ptr<IUnitFactory> _unitFactory);
    void setUi(std::shared_ptr<IUserInterface> _ui);
    void setBattleManager(std::shared_ptr<IBattleManager> _battleManager);

   IHeroSharedConst getSelectedHero() const override;
   IHeroShared getSelectedHero() override;

   void swapUnits(IUnitSharedConst _first, IUnitSharedConst _second) override;

   // Battle interactions
   void battleBegin(IHeroShared& _attacker, IHeroShared& _defender) override;
   void battleEnd(const BattleResult& _result) override;
   void surrender() override;
   void skipMove()  override;
   AttackResult processAttack(UnitPosition _defenderPos, bool _attackedUnitOnLeft) override;
   const BattleQueueSharedVec& getBattleQueueVec() const override;

   // Town interactions
   void setHeroAsGarrison(ITownShared &_town, IHeroShared& _hero) override;
   void hireUnitInTown(ITownShared _town, IHeroShared _hero, UnitLevel _level, int _amount) override;

private:
    void removeDeletedObjectsFromMap();
    void checkGameOver();

private:
    std::string m_currentMapName;

    std::shared_ptr<IPlayersManager> m_playersManager;
    std::shared_ptr<IMapManager> m_mapManager;
    std::shared_ptr<IUnitFactory> m_unitFactory;
    std::shared_ptr<IUserInterface> m_ui;
    std::shared_ptr<IBattleManager> m_battleManager;
    std::shared_ptr<ITownsManager> m_townsManager;

    IPlayerShared m_currentPlayer;
    std::queue<IPlayerShared> m_playersTurnQueue;

    IHeroSharedConst m_selectedHero;
};

#endif // GAMELOGIC_H
