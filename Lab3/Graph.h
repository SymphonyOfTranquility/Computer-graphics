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
        bool used;

        TEdge() : v1(-1), v2(-1), used(false) {};

        TEdge(int v1, int v2, bool used) : v1(v1), v2(v2), used(used) {};
    };
    class Graph
    {
        enum VType
        {
            START,
            END,
            NORMAL,
            SPLIT,
            MERGE
        };
        std::vector<TPoint> vertexes;
        std::vector<TEdge> edges;
        std::vector<std::vector<Vertex> > adjacency_list;
        std::vector<std::pair<int, int> > nearest_edges;
        int vertex_number, edges_number;

        std::vector<int> sort_points();

        double get_angle(TPoint a, TPoint b, TPoint u);
        int get_side_dot(int side); //-1 - left, 1 right
        void add_triangle();
        void sort_counter_clockwise();

        std::vector<TEdge> triangulate(std::vector<int> points, std::vector<std::vector<Vertex> > cur_list);

    public:
        Graph() : vertex_number(0), edges_number(0)
        {
            vertexes.clear(), edges.clear(), adjacency_list.clear();
        };

        void input_graph(std::string file_name);
        void pre_processing();
        std::vector<int> find_point(TPoint x);

        void create_triangulation_tree();

        void output_point_indexes(std::vector<int> indexes);
        void output();
        void output_adjacency_list();
    };
}


#endif //LAB3_GRAPH_H
