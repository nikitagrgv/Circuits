#include "Globals.h"
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
    NodeView()
    {
        main_shape_.setFillColor({38, 46, 108});
        name_text_.setFont(Globals::getFont());
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

    sf::Vector2f getInputPosition(int input) const
    {
        assert(input >= 0 && input < inputs_.size());
        return inputs_[input].getPosition();
    }

    sf::Vector2f getOutputPosition(int output) const
    {
        assert(output >= 0 && output < outputs_.size());
        return outputs_[output].getPosition();
    }

    sf::Color getInputColor(int input) const
    {
        assert(input >= 0 && input < inputs_.size());
        return inputs_[input].getFillColor();
    }

    sf::Color getOutputColor(int output) const
    {
        assert(output >= 0 && output < outputs_.size());
        return outputs_[output].getFillColor();
    }

    void setInputSignal(int input, const Signal &signal)
    {
        assert(input >= 0 && input < inputs_.size());
        inputs_[input].setFillColor(color_from_signal(signal));
    }

    void setOutputSignal(int output, const Signal &signal)
    {
        assert(output >= 0 && output < outputs_.size());
        outputs_[output].setFillColor(color_from_signal(signal));
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
    static sf::Color color_from_signal(const Signal &signal)
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
        createNodes();
        updateIOStates();
    }

    void createNodes()
    {
        // nodes
        node_views_.clear();
        for (const int &id : graph_.getNodesIds())
        {
            NodeView node_view{};
            node_view.setPosition(getPosition()
                + sf::Vector2f{20.f + (float)(id % 5) * 130.f, 20.f + (float)(id / 5) * 130.f});

            const Node &node = graph_.getNode(id);
            node_view.setName(node.getName());
            node_view.setNumInputs(node.getNumInputs());
            node_view.setNumOutputs(node.getNumOutputs());
            node_views_.insert({id, node_view});
        }

        // connections
        connection_views.clear();
        for (const Graph::Connection &connection : graph_.getAllConnections())
        {
            const int from = connection.from;
            const int output = connection.output;
            const int to = connection.to;
            const int input = connection.input;

            const sf::Vector2f p0 = node_views_[from].getOutputPosition(output);
            const sf::Vector2f p1 = node_views_[to].getInputPosition(input);

            LineShape view = LineShape{p0, p1, 2};

            ConnectionWithView con_with_view;
            con_with_view.connection = connection;
            con_with_view.view = view;
            connection_views.push_back(con_with_view);
        }
    }

    void updateIOStates()
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

        for (ConnectionWithView &con_with_view : connection_views)
        {
            const int from = con_with_view.connection.from;
            const int output = con_with_view.connection.output;
            con_with_view.view.setFillColor(node_views_[from].getOutputColor(output));
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
        for (const ConnectionWithView &con_with_view : connection_views)
        {
            target.draw(con_with_view.view, states);
        }
    }

private:
    sf::Transform transform_{sf::Transform::Identity};
    const Graph &graph_{};
    std::unordered_map<int, NodeView> node_views_;

    struct ConnectionWithView
    {
        Graph::Connection connection;
        LineShape view;
    };
    std::vector<ConnectionWithView> connection_views;
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
    int n1 = graph.addNode(std::move(node_1));

    auto node_2 = std::make_unique<SumNode>(4);
    node_2->setName("node_2");
    int n2 = graph.addNode(std::move(node_2));

    auto node_3 = std::make_unique<NotNode>();
    node_3->setName("node_3");
    int n3 = graph.addNode(std::move(node_3));

    auto node_4 = std::make_unique<NotNode>();
    node_4->setName("node_4");
    int n4 = graph.addNode(std::move(node_4));

    auto node_5 = std::make_unique<XorNode>(3);
    node_5->setName("node_5");
    int n5 = graph.addNode(std::move(node_5));

    auto node_6 = std::make_unique<ConstantNode>(Signal{false});
    node_6->setName("node_6");
    int n6 = graph.addNode(std::move(node_6));

    auto node_7 = std::make_unique<ConstantNode>(Signal{-1.f});
    node_7->setName("node_7");
    int n7 = graph.addNode(std::move(node_7));

    auto node_8 = std::make_unique<ConstantNode>(Signal{0.5f});
    node_8->setName("node_8");
    int n8 = graph.addNode(std::move(node_8));


    graph.connect(n1, 0, n2, 0);
    graph.connect(n6, 0, n2, 1);
    graph.connect(n7, 0, n2, 2);
    graph.connect(n8, 0, n2, 3);

    graph.connect(n2, 0, n3, 0);
    graph.connect(n3, 0, n4, 0);


    GraphView graph_view{graph};

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    graph.update();
                    graph_view.updateIOStates();
                }
            }
        }

        window.clear();
        window.draw(graph_view);
        window.display();
    }

    return 0;
}