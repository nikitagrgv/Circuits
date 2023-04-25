#pragma once

#include "Signal.h"
#include "Object.h"

#include <cassert>
#include <string>

class Node : public Object
{
public:
    virtual ~Node() = default;

    virtual int getNumInputs() const = 0;
    virtual int getNumOutputs() const = 0;

    void setInput(int num, Signal signal)
    {
        assert(num >= 0 && num < getNumInputs());
        do_set_input(num, signal);
    }

    Signal getInput(int num) const
    {
        assert(num >= 0 && num < getNumInputs());
        return do_get_input(num);
    }

    Signal getOutput(int num) const
    {
        assert(num >= 0 && num < getNumOutputs());
        return do_get_output(num);
    }

    void calculate()
    {
        assert(canBeCalculated());
        do_calculate();
    }

    virtual void beforeCalculate() = 0; // TODO: delete?
    
    virtual bool canBeCalculated() const = 0;

    virtual void reset() = 0;

    void setName(std::string name) { name_ = std::move(name); }
    const std::string &getName() const { return name_; }

protected:
    virtual void do_calculate() = 0;

    virtual void do_set_input(int num, Signal signal) = 0;
    virtual Signal do_get_input(int num) const = 0;
    virtual Signal do_get_output(int num) const = 0;

private:
    std::string name_;
};

inline std::ostream &operator<<(std::ostream &os, const Node &node)
{
    os << node.getName();
    int num_inputs = node.getNumInputs();
    int num_outputs = node.getNumOutputs();

    os << ": ";

    if (num_inputs > 0)
    {
        os << "in";

        for (int i = 0; i < num_inputs; ++i)
        {
            os << " " << node.getInput(i);
        }
        if (num_outputs > 0)
        {
            os << "; ";
        }
    }

    if (num_outputs > 0)
    {
        os << "out";

        for (int i = 0; i < num_outputs; ++i)
        {
            os << " " << node.getOutput(i);
        }
    }
    return os;
}
