#pragma once
#include <type_traits>
#include <limits>
#include <cmath>

template<typename FloatType>
std::enable_if_t<!std::numeric_limits<FloatType>::is_integer, bool> IsAlmostEqual(FloatType t1, FloatType t2)
{
    return std::fabs(t1 - t2) <= std::numeric_limits<FloatType>::epsilon() * std::fabs(t1 + t2) * 2;
}