#include "hero_functions.h"

void HeroFunctions::onHeroDefeat(IHeroShared _defeatedHero)
{
    _defeatedHero->setNeedToDestroy(true);

    auto player = _defeatedHero->controllingPlayer();
    if (player)
        player->removeHero(_defeatedHero);
}
