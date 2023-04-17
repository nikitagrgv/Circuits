#include "Graph.h"

#include <functional>
#include <unordered_set>

int Graph::addNode(std::unique_ptr<Node> node)
{
    const int id = generate_id();
    nodes_[id] = std::move(node);
    return id;
}

Node &Graph::getNode(int node)
{
    const auto it = nodes_.find(node);
    assert(it != nodes_.end());
    return *it->second;
}

const Node &Graph::getNode(int node) const
{
    const auto it = nodes_.find(node);
    assert(it != nodes_.end());
    return *it->second;
}

void Graph::removeNode(int node)
{
    nodes_.erase(node);
}

void Graph::connect(int from, int output, int to, int input)
{
    assert(output >= 0 && output < getNode(from).getNumOutputs());
    assert(input >= 0 && input < getNode(to).getNumInputs());

    disconnectInput(to, input);

    Connection connection;
    connection.from = from;
    connection.output = output;
    connection.to = to;
    connection.input = input;

    connections_.push_back(connection);
}

void Graph::disconnectInput(int node, int input)
{
    connections_.erase(std::remove_if(connections_.begin(), connections_.end(),
                           [node, input](const Connection &connection) {
                               return connection.to == node && connection.input == input;
                           }),
        connections_.end());
}

void Graph::update()
{
    for (const auto &it : nodes_)
    {
        it.second->beforeCalculate();
    }

    std::unordered_set<int> calculated;

    const auto is_calculated = [&calculated](int node) {
        return calculated.find(node) != calculated.end();
    };

    const auto mark_calculated = [&calculated](int node) {
        calculated.insert(node);
    };

    std::function<void(int node)> calculate;
    calculate = [this, &calculate, &is_calculated, &mark_calculated](int node_id) {
        Node &node = getNode(node_id);
        node.calculate();
        mark_calculated(node_id);

        for (const int idx : get_connections_from(node_id))
        {
            const Connection &connection = connections_[idx];
            const int output = connection.output;
            const int to = connection.to;
            const int input = connection.input;

            Node &node_to = getNode(to);
            node_to.setInput(input, node.getOutput(output));

            if (!is_calculated(to) && node_to.canBeCalculated())
            {
                calculate(to);
            }
        }
    };

    for (const auto &it : nodes_)
    {
        const int node_id = it.first;
        const Node &node = *it.second;

        if (!is_calculated(node_id) && node.canBeCalculated())
        {
            calculate(node_id);
        }
    }
}

std::vector<int> Graph::getNodesIds() const
{
    std::vector<int> ids;
    ids.reserve(nodes_.size());
    for (const auto &it : nodes_)
    {
        ids.push_back(it.first);
    }
    return ids;
}

std::vector<int> Graph::get_connections_from(int node_from) const
{
    std::vector<int> connections_from;
    for (int i = 0, count = connections_.size(); i < count; ++i)
    {
        const Connection &connection = connections_[i];
        if (connection.from == node_from)
        {
            connections_from.push_back(i);
        }
    }
    return connections_from;
}

int Graph::generate_id() const
{
    int id = 0;
    while (nodes_.find(id) != nodes_.end())
    {
        ++id;
    }
    return id;
}
