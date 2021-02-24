#ifndef HERO_PARAMETERS_H
#define HERO_PARAMETERS_H

#include <cstdint>
#include "scale_value.h"

struct HeroParameters
{
    HeroParameters() = default;

    HeroParameters(uint16_t radius,
                   uint16_t movePoints)
        : m_radius(radius)
        , m_movePoints(movePoints)
    {}

    uint16_t m_radius = 0;
    ScaleValue<uint16_t> m_movePoints;
};

#endif // HERO_PARAMETERS_H
