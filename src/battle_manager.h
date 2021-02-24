#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H

#include <queue>

#include "battle_result.h"
#include "battle_queue_element.h"
#include "interfaces/i_battle_manager.h"

#include "forward/i_unit_forward.h"

class IGameLogic;
class IUnitFactory;

class BattleManager
        : public IBattleManager
{

public:
    BattleManager() = default;

    void setGameLogic(std::shared_ptr<IGameLogic> _gameLogic);
    void setUnitFactory(std::shared_ptr<IUnitFactory> _unitFactory);
    void setPartisipants(IHeroShared& _heroOnLeft, IHeroShared& _heroOnRight) override;

    void onNextUnit();
    void onNextTurn();

    void surrender() override;
    void skipMove() override;
    AttackResult processAttack(UnitPosition _defenderPos, bool _attackedUnitOnLeft) override;
    const BattleQueueSharedVec& getBattleQueueVec() const override;

private:
    void initAllUnitsVec();
    void rememberUnitsPartySize();
    void updateUnitsQueue();
    void sortUnitsQueue(BattleQueueSharedVec &_vec);

    IHeroShared& getHero(BattleQueueElemShared _elem);
    BattleQueueElemShared getNextUnit();

    bool needToEndBattle();
    void endBattle();
    void calcResults();
    void updateIsDefeatedState(IHeroShared& _hero);

private:
    std::shared_ptr<IGameLogic> m_gameLogic;
    std::shared_ptr<IUnitFactory> m_unitFactory;

    IHeroShared m_heroOnLeft;
    IHeroShared m_heroOnRight;

    BattleQueueSharedVec m_allUnitsVec;
    BattleQueueElemShared m_currentUnit;

    std::map<BattleQueueElemShared, int> m_partySizeBeforeBattle;
    BattleResult m_result;
};

#endif // BATTLE_MANAGER_H
