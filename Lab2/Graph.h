//
// Created by art on 2/2/20.
//

#ifndef LAB2_GRAPH_H
#define LAB2_GRAPH_H

#include <vector>
#include <string>

namespace graph_space
{
    struct TPoint
    {
        double x, y;

        TPoint() : x(0), y(0) {};

        TPoint(double x, double y) : x(x), y(y) {};
    };

    struct TVertex
    {
        int next_v, weight;

        TVertex(): next_v(-1), weight(0) {};
        TVertex(int next_v, int weight): next_v(next_v), weight(weight) {};
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
        std::vector<std::vector<TVertex> > list_in;
        std::vector<std::vector<TVertex> > list_out;
        int vertex_number, edges_number;

        Graph() : vertex_number(0),
                  edges_number(0) { vertexes.clear(), edges.clear(), list_in.clear(), list_out.clear() };

        void input_graph(std::string file_name);
    };
}


#endif //LAB2_GRAPH_H
