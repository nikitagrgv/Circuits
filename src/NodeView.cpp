#include "NodeView.h"

#include "Globals.h"
#include "Node.h"

#include "SFML/Graphics/CircleShape.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/String.hpp>

namespace
{
constexpr float POINT_RADIUS = 6.0f;
constexpr float MAIN_SHAPE_SIZE_X = 100.0f;
constexpr float MAIN_SHAPE_SIZE_Y = 100.0f;

constexpr float MAX_SIGNAL_VALUE = 1.f;
constexpr float MIN_SIGNAL_VALUE = -1.f;
} // namespace

NodeView::NodeView()
{
    main_shape_.setSize(sf::Vector2f{MAIN_SHAPE_SIZE_X, MAIN_SHAPE_SIZE_Y});
    main_shape_.setFillColor({38, 46, 108});
    name_text_.setFont(Globals::getFont());
    name_text_.setFillColor({150, 173, 235});
    name_text_.setCharacterSize(12);
}

void NodeView::setNumInputs(int num_inputs)
{
    inputs_.clear();
    for (int i = 0; i < num_inputs; ++i)
    {
        inputs_.push_back(create_io_shape());
    }
    update_transforms();
}

void NodeView::setNumOutputs(int num_outputs)
{
    outputs_.clear();
    for (int i = 0; i < num_outputs; ++i)
    {
        outputs_.push_back(create_io_shape());
    }
    update_transforms();
}

sf::Vector2f NodeView::getInputPosition(int input) const
{
    assert(input >= 0 && input < inputs_.size());
    return inputs_[input].getPosition();
}

sf::Vector2f NodeView::getOutputPosition(int output) const
{
    assert(output >= 0 && output < outputs_.size());
    return outputs_[output].getPosition();
}

sf::Color NodeView::getInputColor(int input) const
{
    assert(input >= 0 && input < inputs_.size());
    return inputs_[input].getFillColor();
}

sf::Color NodeView::getOutputColor(int output) const
{
    assert(output >= 0 && output < outputs_.size());
    return outputs_[output].getFillColor();
}

void NodeView::setInputSignal(int input, const Signal &signal)
{
    assert(input >= 0 && input < inputs_.size());
    inputs_[input].setFillColor(color_from_signal(signal));
}

void NodeView::setOutputSignal(int output, const Signal &signal)
{
    assert(output >= 0 && output < outputs_.size());
    outputs_[output].setFillColor(color_from_signal(signal));
}

void NodeView::setPosition(sf::Vector2f pos)
{
    main_shape_.setPosition(pos);
    update_transforms();
}

int NodeView::getInputUnderPosition(sf::Vector2f position) const
{
    for (int i = 0, count = inputs_.size(); i < count; i++)
    {
        if (inputs_[i].getGlobalBounds().contains(position))
        {
            return i;
        }
    }
    return -1;
}

int NodeView::getOutputUnderPosition(sf::Vector2f position) const
{
    for (int i = 0, count = outputs_.size(); i < count; i++)
    {
        if (outputs_[i].getGlobalBounds().contains(position))
        {
            return i;
        }
    }
    return -1;
}

void NodeView::setName(const sf::String &string)
{
    name_text_.setString(string);
    update_transforms();
}

sf::CircleShape NodeView::create_io_shape()
{
    sf::CircleShape output;
    output.setRadius(POINT_RADIUS);
    output.setFillColor(sf::Color::Blue);
    output.setOutlineThickness(1);
    output.setOutlineColor(sf::Color::White);
    const sf::FloatRect bounds = output.getLocalBounds();
    output.setOrigin({bounds.width / 2, bounds.height / 2});
    return output;
}

sf::Color NodeView::color_from_signal(const Signal &signal)
{
    if (!signal.isValid())
    {
        return sf::Color::Blue;
    }
    const float value = signal.getFloat();
    if (value > 0)
    {
        return {0, (unsigned char)(value / MAX_SIGNAL_VALUE * 255), 0};
    }
    else if (value < 0)
    {
        return {(unsigned char)(value / MIN_SIGNAL_VALUE * 255), 0, 0};
    }
    else
    {
        return sf::Color::White;
    }
}

void NodeView::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(main_shape_, states);
    for (const sf::CircleShape &input : inputs_)
    {
        target.draw(input, states);
    }
    for (const sf::CircleShape &output : outputs_)
    {
        target.draw(output, states);
    }
    target.draw(name_text_, states);
}

void NodeView::update_transforms()
{
    const sf::FloatRect bounds = main_shape_.getGlobalBounds();

    const float delta_y_inputs = bounds.height / ((float)inputs_.size() + 1.0f);
    for (int i = 0; i < inputs_.size(); ++i)
    {
        inputs_[i].setPosition(bounds.left, bounds.top + delta_y_inputs * ((float)i + 1.0f));
    }

    const float delta_y_outputs = bounds.height / ((float)outputs_.size() + 1.0f);
    for (int i = 0; i < outputs_.size(); ++i)
    {
        outputs_[i].setPosition(bounds.left + bounds.width,
            bounds.top + delta_y_outputs * ((float)i + 1.0f));
    }

    const sf::FloatRect text_bounds = name_text_.getLocalBounds();
    name_text_.setOrigin(
        {text_bounds.left + text_bounds.width / 2, text_bounds.top + text_bounds.height / 2});
    name_text_.setPosition(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
}
