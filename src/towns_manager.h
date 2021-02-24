#ifndef TOWNS_MANAGER_H
#define TOWNS_MANAGER_H

#include <map>
#include "races.h"

#include "interfaces/i_towns_manager.h"

#include "forward/i_town_forward.h"

class TownsManager
        : public ITownsManager
{
public:
    TownsManager() = default;

    void setUnitFantory(std::shared_ptr<IUnitFactory> _unitFatory);

    ITownShared getTown(const std::string& _name) const override;
    ITownSharedVec getTowns(Race _race) const override;

    void processWeeklyGrow() override;

private:
    void initTowns();

private:
    std::shared_ptr<IUnitFactory> m_unitFactory;

    std::map<Race, ITownSharedVec> m_townsPool;

};

#endif // TOWNS_MANAGER_H
