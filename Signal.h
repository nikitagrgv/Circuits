#pragma once

#include <cassert>
#include <iostream>

class Signal final
{
public:
    static inline Signal ONE() { return Signal{1.0f}; }
    static inline Signal ZERO() { return Signal{0.0f}; }
    static inline Signal INVALID() { return Signal{}; }

    Signal() = default;
    explicit Signal(bool value) { setValue(value); }
    explicit Signal(float value) { setValue(value); }

    bool notZero() const { return !isZero(); }
    bool isZero() const { return is_valid_ && value_ == 0.0f; }
    bool isValid() const { return is_valid_; }

    void toZero() { setValue(0.0f); }
    void toOne() { setValue(1.0f); }
    void invalidate() { is_valid_ = false; }

    void setValue(bool value)
    {
        is_valid_ = true;
        value_ = value ? 1.0f : 0.0f;
    }

    void setValue(float value)
    {
        is_valid_ = true;
        value_ = value;
    }

    bool getBool() const
    {
        assert(isValid());
        return value_ != 0.0f;
    }

    float getFloat() const
    {
        assert(isValid());
        return value_;
    }

    bool operator==(const Signal &rhs) const
    {
        return is_valid_ == rhs.is_valid_ && value_ == rhs.value_;
    }
    bool operator!=(const Signal &rhs) const { return !(rhs == *this); }

private:
    bool is_valid_{false};
    float value_{0.0f};
};

inline std::ostream &operator<<(std::ostream &os, const Signal signal)
{
    if (!signal.isValid())
    {
        os << "*";
    }
    else
    {
        os << signal.getFloat();
    }
    return os;
}
