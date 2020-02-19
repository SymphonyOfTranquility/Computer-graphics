//
// Created by art on 2/16/20.
//

#include "Graph.h"


#include <fstream>
#include <algorithm>
#include <memory>
#include <iostream>
#include <cmath>
#include <set>

namespace graph_space
{

    std::vector<int> Graph::sort_points()
    {
        std::vector<std::pair<TPoint, int> > temp_vertexes;
        for (int i = 0; i < vertex_number; ++i)
            temp_vertexes.emplace_back(vertexes[i], i);

        std::sort(temp_vertexes.begin(), temp_vertexes.end(),
                  [](const std::pair<TPoint, int> &a, const std::pair<TPoint, int> &b) -> bool {
                      return a.first.y > b.first.y || (a.first.y == b.first.y && a.first.x > b.first.x);
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
            std::shared_ptr<int> edge_id = std::make_shared<int>(-1);
            adjacency_list[edge.v2].emplace_back(edge.v1, edge_id);
            adjacency_list[edge.v1].emplace_back(edge.v2, edge_id);
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
                std::cout << item.next_v << ", ";
            }
            std::cout << '\n';
        }
    }

    void Graph::create_triangulation_tree()
    {
        add_triangle();
        sort_counter_clockwise();
        output();
        output_adjacency_list();
        std::vector<int> vert;
        for (int i = 0;i < vertex_number; ++i)
            vert.push_back(i);
        std::vector<TEdge> new_edges = triangulate(vert, adjacency_list);
        std::cout << "JJJJJJJJJJJJJJJJJ \n";
        for (int i = 0;i < new_edges.size(); ++i)
        {
            std::cout << new_edges[i].v1 << ' ' << new_edges[i].v2 << '\n';
        }
        std::cout << '\n';

    }

    int Graph::get_side_dot(int side)
    {
        int ans = 0;
        double eps = 0.000001;
        for (int i = 1;i < vertex_number; ++i)
        {
            if (std::abs(vertexes[i].x*side - vertexes[ans].x) > eps)
            {
                if (vertexes[i].x*side > vertexes[ans].x*side)
                {
                    ans = i;
                }
            }
        }
        return ans;
    }

    void Graph::add_triangle()
    {
        int most_left_dot, most_right_dot, highest_dot = 0, lowest_dot = vertex_number-1;
        most_left_dot = get_side_dot(-1);    //-1 - left
        most_right_dot = get_side_dot(1);    //1 - right
        double step = 2.0;
        std::vector<TPoint> triangle = {
                TPoint(vertexes[most_right_dot].x + step +
                       (vertexes[highest_dot].y-vertexes[lowest_dot].y),
                       vertexes[highest_dot].y + step),
                TPoint(vertexes[most_left_dot].x - step, vertexes[highest_dot].y + step),
                TPoint(vertexes[most_left_dot].x - step,
                       vertexes[lowest_dot].y - step -
                       (vertexes[most_right_dot].x - vertexes[most_left_dot].x))};
        vertexes.insert(vertexes.begin(), triangle.begin(), triangle.end()-1);
        vertexes.push_back(triangle[2]);
        vertex_number += 3;
        for (int j = 0; j < edges_number; ++j)
        {
            edges[j].v1 += 2;
            edges[j].v2 += 2;
        }
        edges.emplace_back(0, most_right_dot+2, false);
        edges.emplace_back(1, highest_dot+2, false);
        edges.emplace_back(vertex_number-1, lowest_dot+2, false);

        edges.emplace_back(0, 1, false);
        edges.emplace_back(1, vertex_number-1, false);
        edges.emplace_back(vertex_number-1, 0, false);
        edges_number += 6;

        adjacency_list.insert(adjacency_list.begin(), std::vector<Vertex>());
        adjacency_list.insert(adjacency_list.begin(), std::vector<Vertex>());
        adjacency_list.emplace_back();

        for (auto & i : adjacency_list)
        {
            for (auto & j : i)
                j.next_v += 2;
        }


        std::shared_ptr<int> edge_id = std::make_shared<int>(-1);
        adjacency_list[0].emplace_back(most_right_dot+2, edge_id);
        adjacency_list[most_right_dot+2].emplace_back(0, edge_id);
        edge_id = std::make_shared<int>(-1);
        adjacency_list[1].emplace_back(highest_dot+2, edge_id);
        adjacency_list[highest_dot+2].emplace_back(1, edge_id);
        edge_id = std::make_shared<int>(-1);
        adjacency_list[vertex_number-1].emplace_back(lowest_dot+2,edge_id);
        adjacency_list[lowest_dot+2].emplace_back(vertex_number-1, edge_id);
        edge_id = std::make_shared<int>(-1);
        adjacency_list[0].emplace_back(1, edge_id);
        adjacency_list[1].emplace_back(0, edge_id);
        edge_id = std::make_shared<int>(-1);
        adjacency_list[1].emplace_back(vertex_number-1, edge_id);
        adjacency_list[vertex_number-1].emplace_back(1, edge_id);
        edge_id = std::make_shared<int>(-1);
        adjacency_list[vertex_number-1].emplace_back(0, edge_id);
        adjacency_list[0].emplace_back(vertex_number-1, edge_id);
    }

    void Graph::sort_counter_clockwise()
    {
        for (int i = 0;i < vertex_number; ++i)
        {
            std::sort(adjacency_list[i].begin(), adjacency_list[i].end(),
                    [i, this](const Vertex &a,const Vertex &b)-> bool {
                TPoint point_a = vertexes[a.next_v];
                TPoint point_b = vertexes[b.next_v];
                TPoint end_point = vertexes[i];
                TPoint master_vector = TPoint(vertexes[i].x-10,vertexes[i].y);
                double angle_a = get_angle(master_vector, point_a, end_point);
                double angle_b = get_angle(master_vector, point_b, end_point);
                double eps = 0.0001;
                if (std::abs(angle_a) < eps)
                    return false;
                if (std::abs(angle_b) < eps)
                    return true;
                if (angle_a*angle_b > 0)
                    return angle_a < angle_b;
                else
                    return angle_a > 0;
            });
        }
    }

    std::vector<TEdge> Graph::triangulate(std::vector<int> points, std::vector<std::vector<Vertex> > cur_list)
    {
        std::vector<VType> types;
        types.push_back(VType::START);
        for (int i = 1;i < points.size()-1; ++i)
        {
            int counter = 0;
            for (int j = 0;j <cur_list[i].size(); ++j)
            {
                if (cur_list[i][j].next_v > i)
                    ++counter;
            }
            if (cur_list[i].size() - counter == 0)
                types.push_back(VType::SPLIT); // normal types
            else if (cur_list[i].size() - counter == cur_list[i].size())
                types.push_back(VType::MERGE);
            else
                types.push_back(VType::NORMAL);
        }
        types.push_back(VType::END);

        std::vector<TEdge> cur_edges;
        for (int i = 0;i < cur_list.size(); ++i)
        {
            for (int j = 0;j < cur_list[i].size(); ++j)
            {
                if (i < cur_list[i][j].next_v)
                {
                    *(cur_list[i][j].edge_id) = cur_edges.size();
                    cur_edges.emplace_back(i, cur_list[i][j].next_v, false);
                }
            }
        }
        std::vector<int> helper(cur_edges.size(), -1);

        points.emplace_back(vertexes.size());
        vertexes.emplace_back(INFINITY, INFINITY);
        auto compare = [&cur_list, &points, &cur_edges, this](const int &a, const int &b)->bool
        {
            TEdge a_edge = cur_edges[a], b_edge = cur_edges[b];
            TPoint a_top = vertexes[points[a_edge.v1]], a_bottom = vertexes[points[a_edge.v2]];
            TPoint b_top = vertexes[points[b_edge.v1]], b_bottom = vertexes[points[b_edge.v2]];
            double eps = 0.00001;
            if (a_edge.v1 == b_edge.v1)
            {
                TPoint temp(-100000, a_top.y);
                double angle1 = get_angle(temp, a_bottom, a_top);
                double angle2 = get_angle(temp, b_bottom, a_top);
                return angle1 < angle2;
            }
            if (a_edge.v2 == b_edge.v1)
                return false;
            if (b_edge.v2 == a_edge.v1)
                return true;
            if (std::abs(a_top.y-b_top.y) < eps)
                return a_bottom.x < b_bottom.x;
            else if (a_top.y > b_top.y)
                return get_angle(a_top, b_top, a_bottom) < 0;
            else
                return get_angle(b_top, a_top, b_bottom) > 0;
        };
        std::set<int, decltype(compare)> tree(compare);
        std::vector<TEdge> new_edges;
        for (int i = 0;i < points.size()-1; ++i)
        {
            if (types[i] == VType::START)
            {
                for (int j = 0;j < cur_list[i].size(); ++j)
                {
                    tree.insert(*(cur_list[i][j].edge_id));
                    helper[*(cur_list[i][j].edge_id)] = i;
                }
            }
            else if (types[i] == VType::END)
            {
                for (int j = 0;j < cur_list[i].size(); ++j)
                {
                    int cur_edge = *(cur_list[i][j].edge_id);
                    if (helper[cur_edge] != -1)
                    {
                        if (types[helper[cur_edge]] == VType::MERGE)
                        {
                            new_edges.emplace_back(i, cur_list[i][j].next_v, false);
                        }
                    }
                    tree.erase(*(cur_list[i][j].edge_id));
                }
            }
            else if (types[i] == VType::SPLIT)
            {
                int temp_to_find = *(cur_list[i].begin()->edge_id);
                std::set<int>::iterator it_low = tree.lower_bound(temp_to_find);
                int cur_edge = *it_low;
                if (it_low == tree.begin())
                {
                    if (compare(*it_low, temp_to_find))
                        cur_edge = -1;
                }
                if (cur_edge != -1)
                {
                    new_edges.emplace_back(helper[cur_edge], i, false);
                    helper[cur_edge] = i;
                }
                for (int j = 0;j < cur_list[i].size(); ++j)
                {
                    if (cur_list[i][j].next_v > i)
                    {
                        tree.insert(*(cur_list[i][j].edge_id));
                        helper[*(cur_list[i][j].edge_id)] = i;
                    }
                }
            }
            else if (types[i] == VType::MERGE)
            {
                for (int j = 0;j < cur_list[i].size(); ++j)
                {
                    int cur_edge = *(cur_list[i][j].edge_id);
                    if (helper[cur_edge] != -1 && types[helper[cur_edge]] == VType::MERGE)
                        new_edges.emplace_back(helper[cur_edge], i, false);

                    tree.erase(cur_edge);
                }
                cur_edges.emplace_back(i, vertexes.size()-1, false);
                std::set<int>::iterator it_low = tree.lower_bound(cur_edges.size()-1);
                int cur_edge = *it_low;
                if (it_low == tree.begin())
                {
                    if (compare(*it_low, cur_edges.size()-1))
                        cur_edge = -1;
                }
                cur_edges.pop_back();
                if (cur_edge != -1)
                {
                    if (helper[*it_low] != -1 && types[helper[*it_low]] == VType::MERGE)
                        new_edges.emplace_back(helper[*it_low], i, false);
                    helper[*it_low] = i;
                }
            }
            else if (types[i] == VType::NORMAL)
            {
                for (int j = 0;j < cur_list[i].size(); ++j)
                {
                    int cur_edge = *(cur_list[i][j].edge_id), next_v = cur_list[i][j].next_v;
                    if (next_v < i)
                    {
                        if (helper[cur_edge] != -1 && types[helper[cur_edge]] == VType::MERGE)
                            new_edges.emplace_back(helper[cur_edge], i, false);
                        tree.erase(cur_edge);
                    }
                }
                cur_edges.emplace_back(i, vertexes.size()-1, false);
                std::set<int>::iterator it_low = tree.lower_bound(cur_edges.size()-1);
                int cur_edge = *it_low;
                if (it_low == tree.begin())
                {
                    if (compare(*it_low, cur_edges.size()-1))
                        cur_edge = -1;
                }
                cur_edges.pop_back();
                if (cur_edge != -1)
                {
                    if (helper[*it_low] != -1 && types[helper[*it_low]] == VType::MERGE)
                        new_edges.emplace_back(helper[*it_low], i, false);
                    helper[*it_low] = i;
                }
                for (int j = 0;j < cur_list[i].size(); ++j)
                {
                    if (cur_list[i][j].next_v > i)
                    {
                        tree.insert(*(cur_list[i][j].edge_id));
                        helper[*(cur_list[i][j].edge_id)] = i;
                    }
                }
            }
        }
        return new_edges;
    }

}
