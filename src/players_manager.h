#ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#include <memory>
#include <map>
#include <queue>

#include "control_side.h"
#include "races.h"
#include "colors.h"

#include "interfaces/i_players_manager.h"

#include "forward/i_player_forward.h"

class IHeroesManager;

class PlayersManager
        : public IPlayersManager
{
public:
    PlayersManager();

    IPlayerShared createNewPLayer(ControlSide _controlSide,
                                  Race _race,
                                  Color _color) override;

    std::queue<IPlayerShared> getPlayersQueue() const override;
    IPlayerSharedVec getPlayersVec() const override;

    void setHeroManager(std::shared_ptr<IHeroesManager> _heroManager);

    bool hireHero(uint8_t _playerId, const std::string& _heroName = "") override;

private:
    uint8_t generateNewId() { return m_uniqueId++; }

private:
    uint8_t m_uniqueId = 1;
    std::map<uint8_t, IPlayerShared> m_playersMap;

    std::shared_ptr<IHeroesManager> m_heroManager;
};

#endif // PLAYERSMANAGER_H
