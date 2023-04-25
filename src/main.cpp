#include "Globals.h"
#include "Graph.h"
#include "GraphView.h"
#include "LineShape.h"
#include "MathUtils.h"
#include "Nodes.h"
#include "ViewCommon.h"

#include <SFML/Graphics.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>


int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Circuits");
    window.setVerticalSyncEnabled(true);

    //    NodeView node_view;
    //    node_view.setPosition({300, 500});
    //    node_view.setName("node");
    //    node_view.setNumInputs(3);
    //    node_view.setNumOutputs(7);

    Graph graph;

    auto n1 = graph.createNode<TriangleSignalNode>(-0.5f, 0.5f, 0.f, 0.09f);
    auto &node_1 = graph.getNode(n1);
    node_1.setName("node_1");

    auto n2 = graph.createNode<SumNode>(4);
    auto &node_2 = graph.getNode(n2);
    node_2.setName("node_2");

    auto n3 = graph.createNode<NegateNode>();
    auto &node_3 = graph.getNode(n3);
    node_3.setName("node_3");

    auto n4 = graph.createNode<ReciprocalNode>();
    auto &node_4 = graph.getNode(n4);
    node_4.setName("node_4");

    auto n5 = graph.createNode<MemoryNode>();
    auto &node_5 = graph.getNode(n5);
    node_5.setName("node_5");

    auto n6 = graph.createNode<TriangleSignalNode>(-1.f, 1.f, 0.f, 0.1f);
    auto &node_6 = graph.getNode(n6);
    node_6.setName("node_6");

    auto n7 = graph.createNode<MultiplicationNode>(2);
    auto &node_7 = graph.getNode(n7);
    node_7.setName("node_7");

    auto n8 = graph.createNode<ConstantNode>(Signal{0.5f});
    auto &node_8 = graph.getNode(n8);
    node_8.setName("node_8");

    graph.connect(n1, 0, n2, 0);
    graph.connect(n6, 0, n2, 1);
    graph.connect(n7, 0, n2, 2);
    graph.connect(n8, 0, n2, 3);

    graph.connect(n2, 0, n3, 0);
    graph.connect(n3, 0, n4, 0);

    graph.connect(n1, 0, n7, 0);
    graph.connect(n6, 0, n7, 1);

    graph.connect(n4, 0, n5, 0);

    View::GraphView graph_view{graph};

    sf::Text info_text{"", Globals::getFont(), 30};
    int iteration = 0;

    const auto update_info = [&]() {
        sf::String str;
        str += "Iteration: ";
        str += std::to_string(iteration);
        info_text.setString(str);
        info_text.setOrigin(Math::getBottomRight(info_text));
        // dont need transforms because gui_view has same coordinates as window
        info_text.setPosition((sf::Vector2f)window.getSize());
    };
    update_info();

    sf::View gui_view = window.getDefaultView();
    sf::View main_view = window.getDefaultView();

    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    while (window.isOpen())
    {
        sf::Vector2i mouse_delta;
        int mouse_scroll = 0;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    graph.iterate();
                    graph_view.updateIOStates();
                    iteration++;
                    update_info();
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                mouse_scroll += event.mouseWheelScroll.delta;
            }
            if (event.type == sf::Event::Resized)
            {
                gui_view.setSize({(float)event.size.width, (float)event.size.height});
                const float height = main_view.getSize().y;
                const float aspect_ratio = (float)event.size.width / (float)event.size.height;
                const float width = height * aspect_ratio;
                main_view.setSize(width, height);
            }
            if (event.type == sf::Event::MouseMoved)
            {
                sf::Vector2i new_mouse_pos(event.mouseMove.x, event.mouseMove.y);
                mouse_delta = new_mouse_pos - mouse_pos;
                mouse_pos = new_mouse_pos;
            }
        }
        sf::Vector2f dir;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            dir.x -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            dir.x += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            dir.y -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            dir.y += 1;
        }
        main_view.move(dir * 1.f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
        {
            sf::Vector2f mouse_delta_coords = window.mapPixelToCoords(mouse_delta, main_view) - window.mapPixelToCoords({}, main_view);
            main_view.move(-mouse_delta_coords);
        }


        const sf::Vector2f mouse_coords = window.mapPixelToCoords(sf::Mouse::getPosition(window), main_view);
        const float tuned_mouse_scroll = -0.1f * (float)mouse_scroll;
        const float resize_coef = mouse_scroll >= 0 ? (1.f + (float)tuned_mouse_scroll)
                                                    : (1.f / (1.f - (float)tuned_mouse_scroll));
        main_view.zoom(resize_coef);

        const sf::Vector2f view_center = main_view.getCenter();

        const sf::Vector2f old_rel_pos = view_center - mouse_coords;
        const sf::Vector2f new_rel_pos = old_rel_pos * resize_coef;
        const sf::Vector2f scroll_move = (new_rel_pos - old_rel_pos);
        main_view.move(scroll_move);

        // draw main
        window.setView(main_view);
        window.clear({60, 63, 65});
        window.draw(graph_view);


        std::vector<sf::Vertex> vertices;
        const std::vector<Signal> &memory = object_cast<MemoryNode>(&node_5)->getMemory();
        for (int i = 0, count = memory.size(); i < count; i++)
        {
            const Signal &signal = memory[i];
            if (!signal.isValid())
            {
                continue;
            }

            sf::Vertex v;
            v.position.y = signal.getFloat() * 100;
            v.position.x = (float)i;
            v.color = View::colorFromSignal(signal);
            vertices.push_back(v);
        }
        window.draw(vertices.data(), vertices.size(), sf::LineStrip);

        // draw gui
        window.setView(gui_view);
        window.draw(info_text);
        window.display();
    }

    return 0;
}