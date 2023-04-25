#include "GraphView.h"

#include "LineShape.h"
#include "NodeView.h"

#include <SFML/Graphics/RenderTarget.hpp>

GraphView::GraphView(const Graph &graph)
    : graph_(graph)
{
    createNodes();
    updateIOStates();
}

void GraphView::createNodes()
{
    // nodes
    node_views_.clear();
    for (const int &id : graph_.getNodesIds())
    {
        NodeView node_view{};
        const sf::Vector2f offset = {0, 0};
        //            const sf::Vector2f offset = {20.f, 20.f};
        node_view.setPosition(getPosition() + offset
                              + sf::Vector2f{(float)(id % 5) * 150.f, (float)(id / 5) * 150.f});

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

void GraphView::updateIOStates()
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
