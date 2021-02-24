#ifndef I_TOWNS_MANAGER_H
#define I_TOWNS_MANAGER_H

#include "races.h"
#include "forward/i_town_forward.h"

class IUnitFactory;

class ITownsManager
{
public:
    ITownsManager() = default;
    virtual ~ITownsManager() = default;

    virtual ITownShared getTown(const std::string& _name) const = 0;
    virtual ITownSharedVec getTowns(Race _race) const = 0;

    virtual void processWeeklyGrow() = 0;
};

#endif // I_TOWNS_MANAGER_H
