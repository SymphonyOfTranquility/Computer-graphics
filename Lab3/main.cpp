#include "Graph.h"
#include <iostream>
#include <string>

int main()
{
    std::string file_name = "../input.txt";
    graph_space::Graph graph;
    graph.input_graph(file_name);
    graph.pre_processing();

    graph.create_triangulation_tree();
    graph.output();
    graph.output_adjacency_list();

    graph.output_all_triangulation_layers();
    graph.output_tree();
    int n = 6;
    graph_space::TPoint points[n];
    points[0] = {8, 13};
    points[1] = {5, 13};
    points[2] = {-10,1};
    points[3] = {9, 12};
    points[4] = {11, 20};
    points[5] = {4, 13};
    std::vector<graph_space::TTreeVertex> path[n];
    for (int i = 0;i < n; ++i)
    {
        path[i] = graph.find_point(points[i]);
        std::cout << "\nPoint : { " << points[i].x << ", " << points[i].y << " }\n";
        graph.output_point_indexes(path[i]);
    }

    return 0;
}
