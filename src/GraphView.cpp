#include "GraphView.h"

#include "LineShape.h"
#include "NodeView.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
const float DISTANCE_X = 250.f;
const float DISTANCE_Y = 250.f;
}

namespace View
{

GraphView::GraphView(const Graph &graph)
    : graph_(graph)
{
    construct();
    updateIOStates();
}

void GraphView::construct()
{
    // nodes
    node_views_.clear();
    for (const int &id : graph_.getNodesIds())
    {
        const Node &node = graph_.getNode(id);

        NodeView node_view{node};
        const sf::Vector2f offset = {0, 0};
        node_view.setPosition(getPosition() + offset
            + sf::Vector2f{(float)(id % 5) * DISTANCE_X, (float)(id / 5) * DISTANCE_Y});
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

        const sf::Vector2f p0 = node_views_.find(from)->second.getOutputPosition(output);
        const sf::Vector2f p1 = node_views_.find(to)->second.getInputPosition(input);

        LineShape view = LineShape{p0, p1, 3};
        view.setOutlineColor(sf::Color::White);
        view.setOutlineThickness(0.8);
        ConnectionWithView con_with_view;
        con_with_view.connection = connection;
        con_with_view.view = view;
        connection_views.push_back(con_with_view);
    }
}

void GraphView::updateIOStates()
{
    for (auto &it : node_views_)
    {
        it.second.updateIOStates();
    }

    for (ConnectionWithView &con_with_view : connection_views)
    {
        const int from = con_with_view.connection.from;
        const int output = con_with_view.connection.output;
        const sf::Color color = node_views_.find(from)->second.getOutputColor(output);
        con_with_view.view.setFillColor(color);
    }
}

void GraphView::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const auto &it : node_views_)
    {
        target.draw(it.second, states);
    }
    draw_connections(target, states);
}

void GraphView::draw_connections(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const ConnectionWithView &con_with_view : connection_views)
    {
        target.draw(con_with_view.view, states);
    }
}

} // namespace View