#ifndef I_TOWN_FORWARD_H
#define I_TOWN_FORWARD_H

#include <memory>
#include <vector>

class ITown;
using ITownShared       = std::shared_ptr<ITown>;
using ITownWeak         = std::weak_ptr<ITown>;
using ITownSharedConst  = std::shared_ptr<const ITown>;
using ITownWeakConst    = std::weak_ptr<const ITown>;

using ITownSharedVec        = std::vector<ITownShared>;
using ITownSharedConstVec   = std::vector<ITownSharedConst>;
using ITownWeakVec          = std::vector<ITownWeak>;
using ITownWeakConstVec     = std::vector<ITownWeakConst>;

#endif // I_TOWN_FORWARD_H
