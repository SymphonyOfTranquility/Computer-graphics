#include "Graph.h"

#include <iostream>
#include <string>
int main()
{
    std::string file_name = "../input.txt";
    graph_space::Graph graph;
    graph.input_graph(file_name);
    graph.pre_processing();
    graph.output();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
