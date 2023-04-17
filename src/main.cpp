#include "Graph.h"
#include "LineShape.h"
#include "Nodes.h"

#include <SFML/Graphics.hpp>
#include <cassert>
#include <cmath>
#include <iostream>

class NodeView : public sf::Drawable
{
public:
    NodeView(const sf::Font &font)
        : font_(font)
    {
        main_shape_.setFillColor({38, 46, 108});
        name_text_.setFont(font);
        name_text_.setFillColor({150, 173, 235});
        name_text_.setCharacterSize(12);
    }

    void setNumInputs(int num_inputs)
    {
        inputs_.clear();
        for (int i = 0; i < num_inputs; ++i)
        {
            sf::CircleShape input;
            input.setRadius(POINT_RADIUS);
            input.setOrigin({POINT_RADIUS / 2, POINT_RADIUS / 2});
            input.setFillColor(sf::Color::Blue);
            input.setOutlineThickness(1);
            input.setOutlineColor(sf::Color::White);
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
            output.setFillColor(sf::Color::Blue);
            output.setOutlineThickness(1);
            output.setOutlineColor(sf::Color::White);
            outputs_.push_back(output);
        }
        update_transforms();
    }

    sf::Vector2f getInputPosition(int input)
    {
        assert(input >= 0 && input < inputs_.size());
        return inputs_[input].getPosition();
    }

    sf::Vector2f getOutputPosition(int output)
    {
        assert(output >= 0 && output < outputs_.size());
        return outputs_[output].getPosition();
    }

    void setInputSignal(int input, const Signal &signal)
    {
        assert(input >= 0 && input < inputs_.size());
        set_color_from_signal(inputs_[input], signal);
    }

    void setOutputSignal(int output, const Signal &signal)
    {
        assert(output >= 0 && output < outputs_.size());
        set_color_from_signal(outputs_[output], signal);
    }

    void setPosition(sf::Vector2f pos)
    {
        main_shape_.setPosition(pos);
        update_transforms();
    }

    sf::Vector2f getPosition() const { return main_shape_.getPosition(); }

    void setName(const sf::String &string)
    {
        name_text_.setString(string);
        update_transforms();
    }

protected:
    static void set_color_from_signal(sf::Shape &shape, const Signal &signal)
    {
        if (!signal.isValid())
        {
            shape.setFillColor(sf::Color::Blue);
            return;
        }
        const float value = signal.getFloat();
        if (signal.getFloat() >= 0)
        {
            shape.setFillColor({0, (unsigned char)(value / MAX_SIGNAL_VALUE * 255), 0});
        }
        else
        {
            shape.setFillColor({(unsigned char)(value / MIN_SIGNAL_VALUE * 255), 0, 0});
        }
    }

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

    const sf::Font &font_{};

    constexpr static float POINT_RADIUS = 6.0f;
    constexpr static float MAIN_SHAPE_SIZE_X = 100.0f;
    constexpr static float MAIN_SHAPE_SIZE_Y = 100.0f;

    constexpr static float MAX_SIGNAL_VALUE = 1.f;
    constexpr static float MIN_SIGNAL_VALUE = -1.f;
};


class GraphView
    : public sf::Drawable
    , public sf::Transformable
{
public:
    GraphView(const Graph &graph)
        : graph_(graph)
    {
        if (!font_.loadFromFile("consolas.ttf"))
        {
            std::cout << "Font not found" << std::endl;
        }

        placeNodes();
        updateIO();
    }

    void placeNodes()
    {
        node_views_.clear();
        for (const int &id : graph_.getNodesIds())
        {
            NodeView node_view{font_};
            node_view.setPosition(getPosition()
                + sf::Vector2f{20.f + (float)(id % 5) * 130.f, 20.f + (float)(id / 5) * 130.f});

            const Node &node = graph_.getNode(id);
            node_view.setName(node.getName());
            node_view.setNumInputs(node.getNumInputs());
            node_view.setNumOutputs(node.getNumOutputs());
            node_views_.insert({id, node_view});
        }
    }

    void updateIO()
    {
        for (auto &it : node_views_)
        {
            const int id = it.first;
            NodeView &node_view = it.second;
            const Node &node = graph_.getNode(id);

            for (int i = 0, count = node.getNumInputs(); i < count; i++)
            {
                node_view.setInputSignal(i, node.getInput(i));
            }
            for (int i = 0, count = node.getNumOutputs(); i < count; i++)
            {
                node_view.setOutputSignal(i, node.getOutput(i));
            }
        }
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        for (const auto &it : node_views_)
        {
            target.draw(it.second, states);
        }
        draw_connections(target, states);
    }

    void draw_connections(sf::RenderTarget &target, sf::RenderStates states) const
    {
        const std::vector<Graph::Connection> &connections = graph_.getAllConnections();
    }

private:
    sf::Font font_;

    sf::Transform transform_{sf::Transform::Identity};
    const Graph &graph_{};
    std::unordered_map<int, NodeView> node_views_;
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Circuits");

    //    NodeView node_view;
    //    node_view.setPosition({300, 500});
    //    node_view.setName("node");
    //    node_view.setNumInputs(3);
    //    node_view.setNumOutputs(7);

    Graph graph;

    auto node_1 = std::make_unique<ConstantNode>(Signal{true});
    node_1->setName("node_1");
    graph.addNode(std::move(node_1));

    auto node_2 = std::make_unique<AndNode>(4);
    node_2->setName("node_2");
    graph.addNode(std::move(node_2));

    auto node_3 = std::make_unique<OrNode>(3);
    node_3->setName("node_3");
    graph.addNode(std::move(node_3));

    auto node_4 = std::make_unique<NotNode>();
    node_4->setName("node_4");
    graph.addNode(std::move(node_4));

    auto node_5 = std::make_unique<XorNode>(3);
    node_5->setName("node_5");
    graph.addNode(std::move(node_5));

    auto node_6 = std::make_unique<ConstantNode>(Signal{false});
    node_6->setName("node_6");
    graph.addNode(std::move(node_6));

    auto node_7 = std::make_unique<ConstantNode>(Signal{-1.f});
    node_7->setName("node_7");
    graph.addNode(std::move(node_7));


    GraphView graph_view{graph};

    LineShape line{
        {100, 200},
        {300, 500},
        2, sf::Color::Red
    };

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(graph_view);
        window.draw(line);
        window.display();
    }

    return 0;
}