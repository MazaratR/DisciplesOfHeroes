#ifndef I_UNIT_FORWARD_H
#define I_UNIT_FORWARD_H

#include <memory>
#include <vector>

class IUnit;
using IUnitShared           = std::shared_ptr<IUnit>;
using IUnitWeak             = std::weak_ptr<IUnit>;
using IUnitSharedConst      = std::shared_ptr<const IUnit>;
using IUnitWeakConst        = std::weak_ptr<const IUnit>;

using IUnitSharedVec        = std::vector<IUnitShared>;
using IUnitWeakVec          = std::vector<IUnitWeak>;
using IUnitSharedConstVec   = std::vector<IUnitSharedConst>;
using IUnitWeakConstVec     = std::vector<IUnitWeakConst>;

#endif // I_UNIT_FORWARD_H
