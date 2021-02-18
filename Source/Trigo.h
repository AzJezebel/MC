#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace Trigo
{
    constexpr float DEGREE_TO_RADIAN (((float)M_PI) / 180.f);
    constexpr float RADIAN_TO_DEGREE (180.f / ((float)M_PI));

    constexpr float ONE_CIRCLE (((float)M_PI) * 2.f);
    constexpr float HALF_CIRCLE_RAD ((float)M_PI);
    constexpr float QUARTER_CIRCLE_RAD ((float)M_PI_2);
    constexpr float EIGHTH_CIRCLE_RAD ((float)M_PI_4);
}