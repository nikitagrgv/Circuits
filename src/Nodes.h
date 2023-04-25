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
            input.invalidate();
        }
        output_.invalidate();
    }

    bool canBeCalculated() const override
    {
        bool has_invalid = std::any_of(inputs_.begin(), inputs_.end(),
            [](Signal signal) { return !signal.isValid(); });
        return !has_invalid;
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
    DECLARE_OBJECT_TYPE(AndNode);

    explicit AndNode(int num_inputs = 2)
        : ClassicNode(num_inputs)
    {}

protected:
    void do_calculate() override
    {
        output_ = Signal(std::all_of(inputs_.begin(), inputs_.end(),
            [](Signal signal) { return signal.getBool(); }));
    }
};

class OrNode final : public ClassicNode
{
public:
    DECLARE_OBJECT_TYPE(OrNode);

    explicit OrNode(int num_inputs = 2)
        : ClassicNode(num_inputs)
    {}

protected:
    void do_calculate() override
    {
        output_ = Signal(std::any_of(inputs_.begin(), inputs_.end(),
            [](Signal signal) { return signal.getBool(); }));
    }
};

class XorNode final : public ClassicNode
{
public:
    DECLARE_OBJECT_TYPE(XorNode);

    explicit XorNode(int num_inputs = 2)
        : ClassicNode(num_inputs)
    {}

protected:
    void do_calculate() override
    {
        bool out = false;
        for (const Signal signal : inputs_)
        {
            out ^= signal.getBool();
        }
        output_ = Signal(out);
    }
};

class NotNode final : public ClassicNode
{
public:
    DECLARE_OBJECT_TYPE(NotNode);

    explicit NotNode()
        : ClassicNode(1)
    {}

protected:
    void do_calculate() override { output_ = Signal(!inputs_[0].getBool()); }
};

class NegateNode final : public ClassicNode
{
public:
    DECLARE_OBJECT_TYPE(NegateNode);

    explicit NegateNode()
        : ClassicNode(1)
    {}

protected:
    void do_calculate() override { output_ = Signal(-inputs_[0].getFloat()); }
};

class ReciprocalNode final : public ClassicNode
{
public:
    DECLARE_OBJECT_TYPE(ReciprocalNode);

    explicit ReciprocalNode()
        : ClassicNode(1)
    {}

protected:
    void do_calculate() override { output_ = Signal(1.f / inputs_[0].getFloat()); }
};

class SumNode final : public ClassicNode
{
public:
    DECLARE_OBJECT_TYPE(SumNode);

    explicit SumNode(int num_inputs = 2)
        : ClassicNode(num_inputs)
    {}

protected:
    void do_calculate() override
    {
        float sum = 0.0f;
        for (const Signal signal : inputs_)
        {
            sum += signal.getFloat();
        }
        output_ = Signal(sum);
    }
};

class MultiplicationNode final : public ClassicNode
{
public:
    DECLARE_OBJECT_TYPE(MultiplicationNode);

    explicit MultiplicationNode(int num_inputs = 2)
        : ClassicNode(num_inputs)
    {}

protected:
    void do_calculate() override
    {
        float product = 1.0f;
        for (const Signal signal : inputs_)
        {
            product *= signal.getFloat();
        }
        output_ = Signal(product);
    }
};

class ConstantNode final : public Node
{
public:
    DECLARE_OBJECT_TYPE(ConstantNode);

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

class TriangleSignalNode final : public Node
{
public:
    DECLARE_OBJECT_TYPE(TriangleSignalNode);

    TriangleSignalNode(float min, float max, float start, float delta)
        : min_(min)
        , max_(max)
        , cur_(start)
        , delta_(delta)
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

    void do_calculate() override
    {
        if (cur_ < min_)
        {
            dir_ = Direction::Up;
        }
        else if (cur_ > max_)
        {
            dir_ = Direction::Down;
        }

        if (dir_ == Direction::Up)
        {
            cur_ += delta_;
        }
        else
        {
            cur_ -= delta_;
        }
        output_.setValue(cur_);
    }

private:
    enum class Direction
    {
        Up,
        Down
    } dir_{Direction::Up};

    float cur_{};

    float min_{};
    float max_{};
    float delta_{};

    Signal output_;
};

class MemoryNode final : public Node
{
public:
    DECLARE_OBJECT_TYPE(MemoryNode);

    MemoryNode()
    {}

    void clearMemory() { memory_.clear(); }
    const std::vector<Signal> &getMemory() const { return memory_; }

    int getNumInputs() const override { return 1; }
    int getNumOutputs() const override { return 0; }

    bool canBeCalculated() const override { return true; }
    void reset() override {}
    void beforeCalculate() override {}

protected:
    void do_set_input(int num, Signal signal) override { cur_signal_ = signal; }
    Signal do_get_input(int num) const override { return cur_signal_; }
    Signal do_get_output(int num) const override { return {}; }

    void do_calculate() override { memory_.push_back(cur_signal_); }

private:
    Signal cur_signal_;
    std::vector<Signal> memory_;
};