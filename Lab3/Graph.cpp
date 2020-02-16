//
// Created by art on 2/16/20.
//

#include "Graph.h"


#include <fstream>
#include <algorithm>
#include <memory>
#include <iostream>
#include <cmath>

namespace graph_space
{

    std::vector<int> Graph::sort_points()
    {
        std::vector<std::pair<TPoint, int> > temp_vertexes;
        for (int i = 0; i < vertex_number; ++i)
            temp_vertexes.emplace_back(vertexes[i], i);

        std::sort(temp_vertexes.begin(), temp_vertexes.end(),
                  [](const std::pair<TPoint, int> &a, const std::pair<TPoint, int> &b) -> bool {
                      return a.first.y < b.first.y || (a.first.y == b.first.y && a.first.x < b.first.x);
                  });
        std::vector<std::pair<int, int> > new_result;
        vertexes.clear();
        for (int i = 0; i < vertex_number; ++i)
        {
            new_result.push_back({temp_vertexes[i].second, i});
            vertexes.push_back(temp_vertexes[i].first);
        }
        std::sort(new_result.begin(), new_result.end());
        std::vector<int> sorted;
        for (int i = 0;i < vertex_number; ++i)
            sorted.push_back(new_result[i].second);
        return sorted;
    }

    double Graph::get_angle(TPoint a, TPoint b, TPoint u)
    {
        TPoint vec_a = {a.x - u.x, a.y - u.y};
        TPoint vec_b = {b.x - u.x, b.y - u.y};
        double dot = vec_a.x*vec_b.x + vec_a.y*vec_b.y;
        double det = vec_a.x*vec_b.y - vec_a.y*vec_b.x;
        return atan2(det, dot)*180.0/M_PI;
    }

    void Graph::input_graph(std::string file_name)
    {
        std::ifstream file;
        file.open(file_name);
        file >> vertex_number;
        for (int i = 0; i < vertex_number; ++i)
        {
            TPoint p;
            file >> p.x >> p.y;
            vertexes.push_back(p);
        }

        file >> edges_number;
        for (int i = 0; i < edges_number; ++i)
        {
            TEdge e;
            file >> e.v1 >> e.v2;
            edges.push_back(e);
        }
    }

    void Graph::pre_processing()
    {
        std::vector<int> new_indexes = sort_points();
        for (int i = 0;i < new_indexes.size(); ++i)
            std::cout << new_indexes[i] << ' ';
        std::cout << '\n';
        for (int i = 0; i < edges_number; ++i)
        {
            edges[i].v1 = new_indexes[edges[i].v1];
            edges[i].v2 = new_indexes[edges[i].v2];
        }
        adjacency_list.clear();
        adjacency_list.resize(vertex_number);
        for (auto &edge : edges)
        {
            if (edge.v1 > edge.v2)
                std::swap(edge.v1, edge.v2);
            std::shared_ptr<int> weight = std::make_shared<int>(1);
            adjacency_list[edge.v2].emplace_back(edge.v1, weight);
            adjacency_list[edge.v1].emplace_back(edge.v2, weight);
        }
        for (int i = 0; i < vertex_number; ++i)
        {
            std::sort(adjacency_list[i].begin(), adjacency_list[i].end(),
                      [this](const Vertex &a, const Vertex &b) -> bool {
                          return a.next_v < b.next_v;
                      });
        }
    }

    std::vector<int> Graph::find_point(TPoint x)
    {
        return std::vector<int>();
    }

    void Graph::output_point_indexes(std::vector<int> indexes)
    {
        for (int i = 0;i < indexes.size(); ++i)
            std::cout << indexes[i] << ' ';
        std::cout << '\n';
        for (int i = 0;i < indexes.size(); ++i)
            std::cout << vertexes[indexes[i]].x << ' ' << vertexes[indexes[i]].y << '\n';
    }

    void Graph::output()
    {
        std::cout << vertex_number << '\n';
        for (int i = 0;i < vertex_number; ++i)
            std::cout << vertexes[i].x << ' ' << vertexes[i].y << '\n';
        std::cout << "\n\n" << edges_number << '\n';
        for (int i = 0;i < edges_number; ++i)
            std::cout << edges[i].v1 << ' ' << edges[i].v2 << '\n';
    }

    void Graph::output_adjacency_list()
    {
        std::cout << "\n\n";
        for (int i = 0;i < adjacency_list.size(); ++i)
        {
            std::cout << i << " : ";
            for (auto & item : adjacency_list[i])
            {
//                std::cout << item.next_v << ", ";
                std::cout << "{ " << item.next_v;
                std::cout << ", " << *(item.weight) << " },";
            }
            std::cout << '\n';
        }
    }

}