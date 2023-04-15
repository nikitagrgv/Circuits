#pragma once

#include <iostream>

class Signal final
{
public:
    enum class State
    {
        Unknown = -1,
        Off = 0,
        On = 1,
    };

    static inline Signal ON() { return {State::On}; }
    static inline Signal OFF() { return {State::Off}; }
    static inline Signal UNKNOWN() { return {State::Unknown}; }

    Signal(State state = State::Unknown) { state_ = state; }

    bool isOn() const { return state_ == State::On; }
    bool isOff() const { return state_ == State::Off; }
    bool isValid() const { return state_ != State::Unknown; }

    void on() { state_ = State::On; }
    void off() { state_ = State::Off; }
    void reset() { state_ = State::Unknown; }

    Signal &operator=(Signal signal)
    {
        state_ = signal.state_;
        return *this;
    }

    bool operator==(const Signal &rhs) const { return state_ == rhs.state_; }

    bool operator!=(const Signal &rhs) const { return !(rhs == *this); }

private:
    State state_{State::Unknown};
};

inline std::ostream &operator<<(std::ostream &os, const Signal signal)
{
    os << (!signal.isValid() ? "*" : signal.isOn() ? "1" : "0");
    return os;
}
