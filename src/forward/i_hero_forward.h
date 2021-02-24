#ifndef I_HERO_FORWARD_H
#define I_HERO_FORWARD_H

#include <vector>
#include <memory>

class IHero;
using IHeroShared       = std::shared_ptr<IHero>;
using IHeroWeak         = std::weak_ptr<IHero>;
using IHeroSharedConst  = std::shared_ptr<const IHero>;
using IHeroWeakConst    = std::weak_ptr<const IHero>;

using IHeroSharedVec        = std::vector<IHeroShared>;
using IHeroSharedConstVec   = std::vector<IHeroSharedConst>;
using IHeroWeakVec          = std::vector<IHeroWeak>;
using IHeroWeakConstVec     = std::vector<IHeroWeakConst>;

#endif // I_HERO_FORWARD_H
