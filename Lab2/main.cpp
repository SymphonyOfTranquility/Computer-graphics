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
    graph.rebalance_weights();
    graph.output_adjacency_list();
    graph.create_chains();
    graph.output_chains();
    graph.output_adjacency_list();

  //  std::vector<graph_space::TPoint> region = graph.find_point(graph_space::TPoint(8, 3));
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
