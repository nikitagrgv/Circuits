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

    const int id_not_1 = graph.createNode<NotNode>();
    auto &not_1 = graph.getNode(id_not_1);
    not_1.setName("not1");

    const int id_not_2 = graph.createNode<NotNode>();
    auto &not_2 = graph.getNode(id_not_2);
    not_2.setName("not2");

    graph.connect(id_input_1, 0, id_not_1, 0);
    graph.connect(id_not_1, 0, id_not_2, 0);

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

