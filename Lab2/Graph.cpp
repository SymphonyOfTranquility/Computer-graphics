//
// Created by art on 2/2/20.
//

#include "Graph.h"

#include <fstream>
#include <algorithm>
#include <memory>

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
        std::vector<int> result;
        vertexes.clear();
        for (int i = 0; i < vertex_number; ++i)
        {
            result.push_back(temp_vertexes[i].second);
            vertexes.push_back(temp_vertexes[i].first);
        }
        return result;
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
        for (int i = 0; i < edges_number; ++i)
        {
            edges[i].v1 = new_indexes[edges[i].v1];
            edges[i].v2 = new_indexes[edges[i].v2];
        }

        for (auto &edge : edges)
        {
            if (edge.v1 > edge.v2)
                std::swap(edge.v1, edge.v2);
            std::shared_ptr<int> weight = std::make_shared<int>(1);
            std::shared_ptr<bool> real = std::make_shared<bool>(true);
            adjacency_list[edge.v2].emplace_back(edge.v1, weight, real);
            adjacency_list[edge.v1].emplace_back(edge.v2, weight, real);
        }
        for (int i = 0; i < vertex_number; ++i)
        {
            std::sort(adjacency_list[i].begin(), adjacency_list[i].end(),
                      [this](const Vertex &a, const Vertex &b) -> bool {
                          TPoint point_a = vertexes[a.next_v];
                          TPoint point_b = vertexes[b.next_v];
                          double eps = 0.000001;
                          if (std::abs(point_a.x - point_b.x) < eps)
                              return point_a.y < point_b.y;
                          return point_a.x < point_b.x;
                      });
        }
    }

    void Graph::regularization()
    {

    }

    void Graph::rebalance_weights()
    {
        for (int i = 1; i < vertex_number; ++i)
        {
            int all_weight_in = 0, counter_out = 0, id_most_left = -1;
            for (int j = 0; j < adjacency_list[i].size(); ++j)
            {
                if (adjacency_list[i][j].next_v < i)
                    all_weight_in += *(adjacency_list[i][j].weight);
                else
                {
                    if (id_most_left == -1)
                        id_most_left = j;
                    ++counter_out;
                }
            }
            if (counter_out < all_weight_in)
            {
                *(adjacency_list[i][id_most_left].weight) = all_weight_in - counter_out;
            }
        }
        for (int i = vertex_number - 2; i >= 0; --i)
        {
            int all_weight_in = 0, counter_out = 0, id_most_left = -1;
            for (int j = 0; j < adjacency_list[i].size(); --j)
            {
                if (adjacency_list[i][j].next_v > i)
                    all_weight_in += *(adjacency_list[i][j].weight);
                else
                {
                    if (id_most_left == -1)
                        id_most_left = j;
                    ++counter_out;
                }
            }
            if (counter_out < all_weight_in)
            {
                *(adjacency_list[i][id_most_left].weight) = all_weight_in - counter_out;
            }
        }
    }
}