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

    struct TTreeVertex
    {
        int vertex_id[3];
        std::vector<int> next_ids;

        TTreeVertex() { vertex_id[0] = vertex_id[1] = vertex_id[2] = -1; next_ids.clear(); };
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
        std::vector<std::vector<std::vector<Vertex> > > triangulation_lists;
        std::vector<std::vector<int> > triangulation_vertexes;
        std::vector<std::vector<TTreeVertex> > triangulation_tree;

        std::vector<int> sort_points();

        double get_angle(TPoint a, TPoint b, TPoint u);

        // this part get triangles from graph
        int get_side_dot(int side); //-1 - left, 1 right
        void add_triangle();

        void sort_counter_clockwise(const std::vector<int> &indeces, std::vector<std::vector<Vertex> > &cur_list);

        void add_new_edges(std::vector<std::vector<Vertex> > &cur_list, std::vector<TEdge> &cur_edges);

        std::vector<std::vector<std::pair<int, int> > >
        get_all_polygons(const std::vector<int> &points, std::vector<std::vector<Vertex> > &cur_list);

        bool check_angle_side(int last_vertex, int middle_vertex, int current_vertex, int vertex_side,
                              const std::vector<int> &points);
        void set_side_for_chains(const std::vector<std::pair<int, int> > &polygon, std::vector<int> &chains_side, int start, int type);
        std::vector<int> set_side_for_edges(const std::vector<std::pair<int, int> > &polygon,
                                            const std::vector<int> &points);
        void divide_on_triangles(std::vector<std::pair<int, int> > &polygon,
                const std::vector<int> &points, std::vector<std::vector<Vertex> > &cur_list);
        void triangulate(const std::vector<int> &points, std::vector<std::vector<Vertex> > &cur_list);
        std::vector<TEdge> create_monotonous_polygon(std::vector<int> points, std::vector<std::vector<Vertex> > cur_list);

        void output_polygons(const std::vector<std::vector<std::pair<int, int> > > &polygons);

        // get next layer of triangles
        int count_triangles(int layer_id);
        void first_layer();
        void next_layer(int layer_id);
        //connect layers
        std::vector<int> get_difference(int layer_id);
        void layers_connection(int layer_id);

        std::pair<int, int> get_bounds(const std::vector<int>& polygon, int center, int start_v, int end_v, int layer_id);
        int check_in_triangle(TTreeVertex triangle, int layer, TPoint vertex);

    public:
        Graph() : vertex_number(0), edges_number(0)
        {
            vertexes.clear(), edges.clear(), adjacency_list.clear();
        };

        void input_graph(std::string file_name);

        void pre_processing();

        std::vector<TTreeVertex> find_point(TPoint x);

        void create_triangulation_tree();


        // all outputs
        void output_point_indexes(std::vector<TTreeVertex> indexes);
        void output();
        void output_adjacency_list();
        void output_triangulation_list(int index);
        void output_triangulation_vertexes(int index);
        void output_all_triangulation_layers();
        void output_triangles(int index);
        void output_tree();
    };
}


#endif //LAB3_GRAPH_H
