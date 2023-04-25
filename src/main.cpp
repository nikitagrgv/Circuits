#include "Globals.h"
#include "Graph.h"
#include "GraphView.h"
#include "LineShape.h"
#include "MathUtils.h"
#include "Nodes.h"

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



        if (mouse_scroll)
            std::cout << " scroll \n";

        const sf::Vector2f mouse_coords = window.mapPixelToCoords(sf::Mouse::getPosition(window), main_view);
        const float tuned_mouse_scroll = -0.1f * (float)mouse_scroll;
        const float resize_coef = mouse_scroll >= 0 ? (1.f + (float)tuned_mouse_scroll)
                                                    : (1.f / (1.f - (float)tuned_mouse_scroll));
        main_view.zoom(resize_coef);

        const sf::Vector2f view_center = main_view.getCenter();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            std::cout << "mouse: " << mouse_coords.x << " " << mouse_coords.y << "\n";

        const sf::Vector2f old_rel_pos = view_center - mouse_coords;
        const sf::Vector2f new_rel_pos = old_rel_pos * resize_coef;
        const sf::Vector2f scroll_move = (new_rel_pos - old_rel_pos);
        main_view.move(scroll_move);

        // draw main
        window.setView(main_view);
        window.clear();
        window.draw(graph_view);

        // draw gui
        window.setView(gui_view);
        window.draw(info_text);
        window.display();
    }

    return 0;
}