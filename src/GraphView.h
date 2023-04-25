#pragma once

#include "Graph.h"
#include "LineShape.h"
#include "NodeView.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

class GraphView
    : public sf::Drawable
    , public sf::Transformable
{
public:
    explicit GraphView(const Graph &graph);
    void createNodes();
    void updateIOStates();

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void draw_connections(sf::RenderTarget &target, sf::RenderStates states) const;

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
