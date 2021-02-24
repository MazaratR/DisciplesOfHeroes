#ifndef I_LOGIC_H
#define I_LOGIC_H

#include <string>
#include <memory>

#include "races.h"
#include "colors.h"
#include "control_side.h"
#include "move_directions.h"
#include "units/unit_position.h"
#include "units/unit_levels.h"

#include "forward/i_player_forward.h"
#include "forward/i_hero_forward.h"
#include "forward/i_town_forward.h"
#include "forward/i_unit_forward.h"
#include "forward/battle_queue_element_forward.h"

class ITown;
struct BattleResult;
struct AttackResult;

class IGameLogic
{
public:
    IGameLogic() = default;
    virtual ~IGameLogic() = default;

    virtual void createPlayer(ControlSide _controlSide,
                              Race _race,
                              Color _color) = 0;
    virtual void onMapSelected(const std::string& _mapName) = 0;
    virtual void onStartGame() = 0;
    virtual void onNextTurn() = 0;
    virtual void onNewWeek() = 0;

    virtual void moveCurrentHero(MoveDirection _dir) = 0;

    virtual IPlayerShared getCurrentPlayer() = 0;
    virtual void onHeroSelected(const std::string& _name) = 0;

    virtual IHeroSharedConst getSelectedHero() const = 0;
    virtual IHeroShared getSelectedHero() = 0;

    virtual void swapUnits(IUnitSharedConst _first, IUnitSharedConst _second) = 0;

    // Battle interactions
    virtual void battleBegin(IHeroShared& _attacker, IHeroShared& _defender) = 0;
    virtual void battleEnd(const BattleResult& _result) = 0;
    virtual void surrender() = 0;
    virtual void skipMove() = 0;
    virtual AttackResult processAttack(UnitPosition _defenderPos, bool _attackedUnitOnLeft) = 0;
    virtual const BattleQueueSharedVec& getBattleQueueVec() const = 0;

    // Town interactions
    virtual void setHeroAsGarrison(ITownShared& _town, IHeroShared& _hero) = 0;
    virtual void hireUnitInTown(ITownShared _town, IHeroShared _hero, UnitLevel _level, int _amount) = 0;
};

#endif // I_LOGIC_H
