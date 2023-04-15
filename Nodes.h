#pragma once

#include "Node.h"

#include <algorithm>
#include <vector>

class ClassicNode : public Node
{
public:
    explicit ClassicNode(int num_inputs = 2) { inputs_.resize(num_inputs); }

    int getNumInputs() const override { return inputs_.size(); }
    int getNumOutputs() const override { return 1; }

    void reset() override
    {
        for (Signal &input : inputs_)
        {
            input.reset();
        }
        output_.reset();
    }

    bool canBeCalculated() const override
    {
        return !std::any_of(inputs_.begin(), inputs_.end(),
            [](Signal signal) { return signal == Signal::UNKNOWN(); });
    }

    void beforeCalculate() override { reset(); }

protected:
    void do_set_input(int num, Signal signal) override { inputs_[num] = signal; }
    Signal do_get_input(int num) const override { return inputs_[num]; }
    Signal do_get_output(int num) const override { return output_; }

protected:
    std::vector<Signal> inputs_;
    Signal output_;
};

class AndNode final : public ClassicNode
{
public:
    explicit AndNode(int num_inputs = 2)
        : ClassicNode(num_inputs)
    {}

protected:
    void do_calculate() override
    {
        output_ = Signal::fromBool(std::all_of(inputs_.begin(), inputs_.end(),
            [](Signal signal) { return signal.toBool(); }));
    }
};

class OrNode final : public ClassicNode
{
public:
    explicit OrNode(int num_inputs = 2)
        : ClassicNode(num_inputs)
    {}

protected:
    void do_calculate() override
    {
        output_ = Signal::fromBool(std::any_of(inputs_.begin(), inputs_.end(),
            [](Signal signal) { return signal.toBool(); }));
    }
};

class XorNode final : public ClassicNode
{
public:
    explicit XorNode(int num_inputs = 2)
        : ClassicNode(num_inputs)
    {}

protected:
    void do_calculate() override
    {
        bool out = false;
        for (const Signal signal : inputs_)
        {
            out ^= signal.toBool();
        }
        output_ = Signal::fromBool(out);
    }
};

class NotNode final : public ClassicNode
{
public:
    explicit NotNode()
        : ClassicNode(1)
    {}

protected:
    void do_calculate() override
    {
        output_ = inputs_[0] == Signal::ON() ? Signal::OFF() : Signal::ON();
    }
};

class ConstantNode final : public Node
{
public:
    ConstantNode(Signal signal)
        : output_(signal)
    {}

    void setOutput(Signal signal) { output_ = signal; }

    int getNumInputs() const override { return 0; }
    int getNumOutputs() const override { return 1; }

    bool canBeCalculated() const override { return true; }
    void reset() override {}
    void beforeCalculate() override {}

protected:
    void do_set_input(int num, Signal signal) override {}
    Signal do_get_input(int num) const override { return {}; }
    Signal do_get_output(int num) const override { return output_; }

    void do_calculate() override {}

private:
    Signal output_;
};
