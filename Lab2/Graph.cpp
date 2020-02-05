//
// Created by art on 2/2/20.
//

#include "Graph.h"

#include <fstream>
#include <algorithm>
#include <memory>
#include <iostream>

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

    void Graph::dfs(std::vector<std::vector<Vertex> > &list, int v, int num_chain)
    {
        chains[num_chain].push_back(v);
        std::vector<int> cur_chain = chains[num_chain];
        bool flag = false;
        for (int i = 0; i < list[v].size(); ++i)
        {
            int next_v = list[v][i].next_v;
            if (!flag)
            {
                dfs(list, next_v, num_chain);
                flag = true;
            }
            else
            {
                chains.push_back(cur_chain);
                dfs(list, next_v, static_cast<int>(chains.size())-1);
            }
        }
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

    void Graph::rebalance_weights()
    {
        for (int i = 1; i < vertex_number-1; ++i)
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
                *(adjacency_list[i][id_most_left].weight) += all_weight_in - counter_out;
            }
        }
        for (int i = vertex_number - 2; i > 0; --i)
        {
            int all_weight_in = 0, all_weight_out = 0, id_most_left = -1;
            for (int j = 0; j < adjacency_list[i].size(); ++j)
            {
                if (adjacency_list[i][j].next_v > i)
                    all_weight_in += *(adjacency_list[i][j].weight);
                else
                {
                    if (id_most_left == -1)
                        id_most_left = j;
                    all_weight_out += *(adjacency_list[i][j].weight);
                }
            }
            if (all_weight_out < all_weight_in)
            {
                *(adjacency_list[i][id_most_left].weight) += all_weight_in - all_weight_out;
            }
        }
    }


    void Graph::create_chains()
    {
        std::vector<std::vector<Vertex> > temp_list(vertex_number);
        for (int i = 0;i < vertex_number-1; ++i)
        {
            for (int j = 0;j < adjacency_list[i].size(); ++j)
                if (adjacency_list[i][j].next_v > i)
                    temp_list[i].push_back(adjacency_list[i][j]);

            std::sort(temp_list[i].begin(), temp_list[i].end(),
                    [this](const Vertex &a, const Vertex &b) -> bool
                    {
                        TPoint point_a = vertexes[a.next_v], point_b = vertexes[b.next_v];
                        return point_a.x < point_b.x || (point_a.x == point_b.x && point_a.y < point_b.y);
                    });
        }

        chains_number = 0;
        chains.clear();
        chains.resize(1);
        dfs(temp_list, 0, 0);
        chains_number = chains.size();
    }


    std::vector<TPoint> Graph::find_point(TPoint x)
    {
        int l_chain = -1, r_chain = chains_number;
        while (r_chain-l_chain > 1)
        {
            int mid = (r_chain + l_chain) >> 1;
            //if (above)
        }
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
                std::cout << "  w  " << *(item.weight) << " },";
            }
            std::cout << '\n';
        }
    }

    void Graph::output_chains()
    {
        for (int i = 0;i < chains_number; ++i)
        {
            std::cout << "Chain " << i << ": ";
            for (int j = 0; j < chains[i].size(); ++j)
                std::cout << chains[i][j] << ' ';
            std::cout << '\n';
        }
    }
}