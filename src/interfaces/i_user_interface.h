#ifndef I_USER_INTERFACE_H
#define I_USER_INTERFACE_H

#include "forward/i_hero_forward.h"
#include "forward/i_town_forward.h"

class ITown;
struct BattleResult;

class IUserInterface
{
public:
    IUserInterface() = default;

    virtual void battleBegin(IHeroShared& _attacker, IHeroShared& _defender) = 0;
    virtual void battleEnd(const BattleResult& _result) = 0;
    virtual void arrivalAtTown(ITownShared& _town, IHeroShared& _hero) = 0;    
    virtual void onNewWeek() = 0;
    virtual void onNotify(const std::string& _text) = 0;
};

#endif // I_USER_INTERFACE_H
