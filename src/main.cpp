#include <SFML/Graphics.hpp>
#include <iostream>


class NodeView : public sf::Drawable
{
public:
    NodeView()
    {
        main_shape_.setFillColor({38, 46, 108});

        if (!font_.loadFromFile("arial.ttf"))
        {
            std::cout << "Font not found" << std::endl;
        }

        name_text_.setFont(font_);
        name_text_.setFillColor({131, 131, 175});
        name_text_.setCharacterSize(66);
    }

    void setNumInputs(int num_inputs)
    {
        inputs_.clear();
        for (int i = 0; i < num_inputs; ++i)
        {
            sf::CircleShape input;
            input.setRadius(POINT_RADIUS);
            input.setOrigin({POINT_RADIUS / 2, POINT_RADIUS / 2});
            input.setFillColor(sf::Color::Green);
            inputs_.push_back(input);
        }
        update_transforms();
    }

    void setNumOutputs(int num_outputs)
    {
        outputs_.clear();
        for (int i = 0; i < num_outputs; ++i)
        {
            sf::CircleShape output;
            output.setRadius(POINT_RADIUS);
            output.setOrigin({POINT_RADIUS / 2, POINT_RADIUS / 2});
            output.setFillColor(sf::Color::Red);
            outputs_.push_back(output);
        }
        update_transforms();
    }

    void setPosition(sf::Vector2f pos)
    {
        main_shape_.setPosition(pos);
        update_transforms();
    }

    void setName(sf::String string)
    {
        name_text_.setString(string);
        update_transforms();
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
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

private:
    void update_transforms()
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
        name_text_.setOrigin({text_bounds.width / 2, text_bounds.height / 2});
        name_text_.setPosition(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
    }

private:
    sf::RectangleShape main_shape_{
        sf::Vector2f{MAIN_SHAPE_SIZE_X, MAIN_SHAPE_SIZE_Y}
    };
    std::vector<sf::CircleShape> inputs_;
    std::vector<sf::CircleShape> outputs_;

    sf::Text name_text_{};

    sf::Font font_;

    constexpr static float POINT_RADIUS = 10.0f;
    constexpr static float MAIN_SHAPE_SIZE_X = 50.0f;
    constexpr static float MAIN_SHAPE_SIZE_Y = 100.0f;
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Circuits");

    NodeView node_view;
    node_view.setPosition({300, 500});
    node_view.setName("node");
    node_view.setNumInputs(3);
    node_view.setNumOutputs(7);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(node_view);
        window.display();
    }

    return 0;
}