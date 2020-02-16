//
// Created by art on 2/16/20.
//

#ifndef LAB3_GRAPH_H
#define LAB3_GRAPH_H

#include "Vertex.h"

#include <vector>
#include <string>
#include <memory>

namespace graph_space
{
    struct TPoint
    {
        double x, y;

        TPoint() : x(0), y(0) {};

        TPoint(double x, double y) : x(x), y(y) {};
    };

    struct TEdge
    {
        int v1, v2;

        TEdge() : v1(-1), v2(-1) {};

        TEdge(int v1, int v2) : v1(v1), v2(v2) {};
    };

    class Graph
    {
        std::vector<TPoint> vertexes;
        std::vector<TEdge> edges;
        std::vector<std::vector<Vertex> > adjacency_list;
        std::vector<std::vector<int> > chains;
        int vertex_number, edges_number, chains_number;

        std::vector<int> sort_points();

        double get_angle(TPoint a, TPoint b, TPoint u);

    public:
        Graph() : vertex_number(0), edges_number(0)
        {
            vertexes.clear(), edges.clear(), adjacency_list.clear();
            chains.clear();
        };

        void input_graph(std::string file_name);
        void pre_processing();
        std::vector<int> find_point(TPoint x);


        void output_point_indexes(std::vector<int> indexes);
        void output();
        void output_adjacency_list();
    };
}


#endif //LAB3_GRAPH_H
