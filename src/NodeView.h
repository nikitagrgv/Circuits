#pragma once

#include "Signal.h"

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/String.hpp"
#include <SFML/Graphics/Drawable.hpp>

class Node;

namespace View
{

class NodeView : public sf::Drawable
{
public:
    NodeView(const Node &node);

    void construct();
    void updateIOStates();

    void setNumInputs(int num_inputs);
    void setNumOutputs(int num_outputs);

    sf::Vector2f getInputPosition(int input) const;
    sf::Vector2f getOutputPosition(int output) const;

    int getInputUnderPosition(sf::Vector2f position) const;
    int getOutputUnderPosition(sf::Vector2f position) const;

    sf::Color getInputColor(int input) const;
    sf::Color getOutputColor(int output) const;

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const { return main_shape_.getPosition(); }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void set_input_signal(int input, const Signal &signal);
    void set_output_signal(int output, const Signal &signal);

    void set_name(const sf::String &string);

    void update_transforms();


private:
    const Node &node_;

    sf::RectangleShape main_shape_;
    std::vector<sf::CircleShape> inputs_;
    std::vector<sf::CircleShape> outputs_;

    sf::Text name_text_{};
};

} // namespace View