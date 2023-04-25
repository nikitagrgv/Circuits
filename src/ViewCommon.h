#pragma once

#include <cmath>

namespace View
{
inline const float MAX_SIGNAL_VALUE = 1.f;
inline const float MIN_SIGNAL_VALUE = -1.f;

inline sf::Color colorFromSignal(const Signal &signal)
{
    if (!signal.isValid())
    {
        return sf::Color::Blue;
    }
    float value = signal.getFloat();
    value = std::clamp(value, MIN_SIGNAL_VALUE, MAX_SIGNAL_VALUE);

    if (value >= 0)
    {
        return {0, (unsigned char)(value / MAX_SIGNAL_VALUE * 255), 0};
    }
    else
    {
        return {(unsigned char)(value / MIN_SIGNAL_VALUE * 255), 0, 0};
    }
}
} // namespace View
