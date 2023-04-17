#pragma once

#include "Node.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>

class Graph
{
public:
    struct Connection
    {
        int from{-1};
        int output{-1};

        int to{-1};
        int input{-1};
    };

    template<class T, class... Args>
    int createNode(Args &&...args)
    {
        std::unique_ptr<Node> node = std::make_unique<T>(std::forward<Args>(args)...);
        return addNode(std::move(node));
    }

    int addNode(std::unique_ptr<Node> node);
    Node& getNode(int node);
    const Node& getNode(int node) const;
    void removeNode(int node);

    void connect(int from, int output, int to, int input);
    void disconnectInput(int node, int input);

    void calculate();

    std::vector<int> getNodesIds() const;

    const std::vector<Connection> &getAllConnections() const { return connections_; }

private:
    std::vector<int> get_connections_from(int node_from) const;

private:
    int generate_id() const;

private:
    std::unordered_map<int, std::unique_ptr<Node>> nodes_;
    std::vector<Connection> connections_;
};

inline std::ostream &operator<<(std::ostream &os, const Graph &graph)
{
    for (const int id: graph.getNodesIds())
    {
        os << graph.getNode(id) << std::endl;
    }
    return os;
}
