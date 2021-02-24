#ifndef UNIT_STATS_H
#define UNIT_STATS_H

#include <cstdint>
#include "scale_value.h"
#include "range.h"

struct UnitStats
{
    ScaleValue<uint16_t> health;
    ScaleValue<uint8_t> ammo;
    Range<uint8_t> damageMelee;
    Range<uint8_t> damageRange;
    uint8_t armor = 0;
    uint8_t speed = 0;
};

#endif // UNIT_STATS_H
