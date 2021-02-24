#ifndef I_PLAYERS_MANAGER_H
#define I_PLAYERS_MANAGER_H

#include <memory>
#include <queue>

#include "races.h"
#include "control_side.h"
#include "colors.h"

#include "forward/i_player_forward.h"

class IPlayersManager
{
public:
    IPlayersManager() = default;
    virtual ~IPlayersManager() = default;

    virtual IPlayerShared createNewPLayer(ControlSide _controlSide,
                                          Race _race,
                                          Color _color) = 0;

    virtual std::queue<IPlayerShared> getPlayersQueue() const = 0;
    virtual IPlayerSharedVec getPlayersVec() const = 0;

    virtual bool hireHero(uint8_t _playerId, const std::string& _heroName = "") = 0;
};

#endif // I_PLAYERS_MANAGER_H
