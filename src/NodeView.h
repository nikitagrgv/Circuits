#pragma once

#include "Signal.h"

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/String.hpp"
#include <SFML/Graphics/Drawable.hpp>

class NodeView : public sf::Drawable
{
public:
    NodeView();

    void setNumInputs(int num_inputs);
    void setNumOutputs(int num_outputs);

    sf::Vector2f getInputPosition(int input) const;
    sf::Vector2f getOutputPosition(int output) const;

    sf::Color getInputColor(int input) const;
    sf::Color getOutputColor(int output) const;

    void setInputSignal(int input, const Signal &signal);
    void setOutputSignal(int output, const Signal &signal);

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const { return main_shape_.getPosition(); }

    int getInputUnderPosition(sf::Vector2f position) const;
    int getOutputUnderPosition(sf::Vector2f position) const;

    void setName(const sf::String &string);

protected:
    static sf::CircleShape create_io_shape();
    static sf::Color color_from_signal(const Signal &signal);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void update_transforms();

private:
    sf::RectangleShape main_shape_;
    std::vector<sf::CircleShape> inputs_;
    std::vector<sf::CircleShape> outputs_;

    sf::Text name_text_{};
};
