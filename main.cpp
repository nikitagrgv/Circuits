#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Graph.h"
#include "Nodes.h"

using namespace std;

int main()
{
    Graph graph;

    const int id_input_1 = graph.createNode<ConstantNode>(Signal::ON());
    auto &input_1 = static_cast<ConstantNode&>(graph.getNode(id_input_1));
    input_1.setName("input1");

    const int id_input_2 = graph.createNode<ConstantNode>(Signal::ON());
    auto &input_2 = static_cast<ConstantNode&>(graph.getNode(id_input_2));
    input_2.setName("input2");

    const int id_and = graph.createNode<AndNode>();
    Node &node_and = graph.getNode(id_and);
    node_and.setName("and");

    const int id_or = graph.createNode<OrNode>();
    Node &node_or = graph.getNode(id_or);
    node_or.setName("or ");

    graph.connect(id_input_1, 0, id_and, 0);
    graph.connect(id_input_2, 0, id_and, 1);

    graph.connect(id_input_1, 0, id_or, 0);
    graph.connect(id_input_2, 0, id_or, 1);


#define LOG_GRAPH(name) \
    cout << "----------" name "----------" << endl << graph << endl;

    LOG_GRAPH("before")

    input_1.setOutput(Signal::OFF());
    input_2.setOutput(Signal::OFF());
    graph.calculate();
    LOG_GRAPH("------")

    input_1.setOutput(Signal::ON());
    input_2.setOutput(Signal::OFF());
    graph.calculate();
    LOG_GRAPH("------")

    input_1.setOutput(Signal::OFF());
    input_2.setOutput(Signal::ON());
    graph.calculate();
    LOG_GRAPH("------")

    input_1.setOutput(Signal::ON());
    input_2.setOutput(Signal::ON());
    graph.calculate();
    LOG_GRAPH("------")

    input_1.setOutput(Signal::UNKNOWN());
    input_2.setOutput(Signal::ON());
    graph.calculate();
    LOG_GRAPH("------")

}

