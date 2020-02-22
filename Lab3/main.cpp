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
    graph.output_adjacency_list();

    graph.create_triangulation_tree();
    graph.output();
    graph.output_adjacency_list();
    graph.output_triangulation_list(0);

    std::vector<int> region = graph.find_point(graph_space::TPoint(5, 3));
    graph.output_point_indexes(region);

    return 0;
}
