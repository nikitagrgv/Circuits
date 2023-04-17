#pragma once

#include "SFML/System/Vector2.hpp"

namespace Math
{

inline float length(sf::Vector2f vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

inline float toDegrees(float radians)
{
    return radians * 180.f / 3.1415926f;
}

} // namespace Math
