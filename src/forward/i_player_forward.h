#ifndef I_PLAYER_FORWARD_H
#define I_PLAYER_FORWARD_H

#include <memory>
#include <vector>

class IPlayer;
using IPlayerShared         = std::shared_ptr<IPlayer>;
using IPlayerWeak           = std::weak_ptr<IPlayer>;
using IPlayerSharedConst    = std::shared_ptr<const IPlayer>;
using IPlayerWeakConst      = std::weak_ptr<const IPlayer>;

using IPlayerWeakVec          = std::vector<IPlayerWeak>;
using IPlayerWeakConstVec     = std::vector<IPlayerWeakConst>;
using IPlayerSharedVec        = std::vector<IPlayerShared>;
using IPlayerSharedConstVec   = std::vector<IPlayerSharedConst>;

#endif // I_PLAYER_FORWARD_H
