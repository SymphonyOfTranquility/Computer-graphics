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

    std::vector<int> region = graph.find_point(graph_space::TPoint(5, 3));
    graph.output_point_indexes(region);
    region = graph.find_point(graph_space::TPoint(3, 3));
    graph.output_point_indexes(region);
    region = graph.find_point(graph_space::TPoint(5, 0));
    graph.output_point_indexes(region);
    region = graph.find_point(graph_space::TPoint(0, 0));
    graph.output_point_indexes(region);
    region = graph.find_point(graph_space::TPoint(4, -4));
    graph.output_point_indexes(region);
    
    region = graph.find_point(graph_space::TPoint(1, 4));
    graph.output_point_indexes(region);
    
    return 0;
}
