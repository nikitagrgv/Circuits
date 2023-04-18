#pragma once

#include "SFML/System/Vector2.hpp"

namespace Math
{

template <class T>
inline auto length(T vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

template <class T>
inline T normalize(T vec)
{
    return vec / length(vec);
}

inline float toDegrees(float radians)
{
    return radians * 180.f / 3.1415926f;
}

template <class T>
inline sf::Vector2f getBottomRight(T shape)
{
    const sf::FloatRect bounds = shape.getLocalBounds();
    return {bounds.left + bounds.width, bounds.top + bounds.height};
}

} // namespace Math
