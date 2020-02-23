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
        first_layer();
        int layer = 0;
        while (triangulation_lists[layer].size() > 3)
        {
            next_layer(layer);
            ++layer;
        }
//        for (int i = 0;i < triangulation_tree.size(); ++i)
//        {
//            std::sort(triangulation_tree[i].begin(), triangulation_tree[i].end(),
//                    [](const TTreeVertex &a, const TTreeVertex &b)->bool
//            {
//                for (int i = 0;i < 3;++i)
//                {
//                    if (a.vertex_id[i] < b.vertex_id[i])
//                        return true;
//                    if (a.vertex_id[i] > b.vertex_id[i])
//                        return false;
//                }
//            });
//        }

        output_all_triangulation_layers();
        for (int i = 0;i < triangulation_lists.size()-1; ++i)
            layers_connection(i);
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

    void Graph::sort_counter_clockwise(const std::vector<int> &indeces, std::vector<std::vector<Vertex> > &cur_list)
    {
        for (int i = 0; i < indeces.size(); ++i)
        {
            std::sort(cur_list[i].begin(), cur_list[i].end(),
                    [i, indeces, this](const Vertex &a, const Vertex &b)-> bool {
                TPoint point_a = vertexes[indeces[a.next_v]];
                TPoint point_b = vertexes[indeces[b.next_v]];
                TPoint end_point = vertexes[indeces[i]];
                TPoint master_vector = TPoint(vertexes[indeces[i]].x - 10, vertexes[indeces[i]].y);
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

    std::vector<TEdge> Graph::create_monotonous_polygon(std::vector<int> points, std::vector<std::vector<Vertex> > cur_list)
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
                cur_edges.emplace_back(i, points.size()-1, false);
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
                cur_edges.emplace_back(i, points.size()-1, false);
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
        vertexes.pop_back();
        return new_edges;
    }

    void Graph::add_new_edges(std::vector<std::vector<Vertex> > &cur_list, std::vector<TEdge> &cur_edges)
    {
        for (auto & cur_edge : cur_edges)
        {
            std::shared_ptr<int> weight = std::make_shared<int>(-1);
            cur_list[cur_edge.v1].emplace_back(cur_edge.v2, weight);
            cur_list[cur_edge.v2].emplace_back(cur_edge.v1, weight);
        }
    }

    void Graph::output_triangulation_list(int index)
    {
        for (int i = 0;i < triangulation_vertexes[index].size(); ++i)
        {
            std::cout << triangulation_vertexes[index][i] << " : ";
            for (auto & item : triangulation_lists[index][i])
            {
                std::cout << triangulation_vertexes[index][item.next_v] << ", ";
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    void Graph::triangulate(const std::vector<int> &points, std::vector<std::vector<Vertex> > &cur_list)
    {
        sort_counter_clockwise(points, cur_list);
        std::vector<std::vector<std::pair<int,int > > > polygons = get_all_polygons(points, cur_list);
        for (int i = 0; i < polygons.size(); ++i)
            divide_on_triangles(polygons[i], points, cur_list);
    }

    std::vector<std::vector<std::pair<int, int> > > Graph::get_all_polygons(const std::vector<int> &points, std::vector<std::vector<Vertex> > &cur_list)
    {
        std::vector<std::vector<std::pair<int, int> > > polygons;
        for (int start_v = 0; start_v < points.size(); ++start_v)
        {
            for (int j = 0;j < cur_list[start_v].size(); ++j)
            {
                std::vector<std::pair<int, int> > cur_poly(points.size(), {-1, -1});
                int cur_v = start_v, next_v = cur_list[start_v][j].next_v;
                if (cur_list[start_v][j].used)
                    continue;
                cur_poly[cur_v].second = next_v;
                cur_poly[next_v].first = cur_v;
                cur_list[start_v][j].used = true;
                while (next_v != start_v)
                {
                    for (int i = 0;i < cur_list[next_v].size(); ++i)
                    {
                        if (cur_list[next_v][i].next_v == cur_v)
                        {
                            cur_v = next_v;
                            next_v = cur_list[cur_v][(i+1)%cur_list[cur_v].size()].next_v;
                            cur_poly[cur_v].second = next_v;
                            cur_poly[next_v].first = cur_v;
                            cur_list[cur_v][(i+1)%cur_list[cur_v].size()].used = true;
                            break;
                        }
                    }
                }
                polygons.push_back(cur_poly);
            }
        }
        return polygons;
    }

    void Graph::output_polygons(const std::vector<std::vector<std::pair<int, int> > > &polygons)
    {
        std::cout << "Number of polygons: " << polygons.size() << '\n';
        for (int i = 0;i < polygons.size(); ++i)
        {
            std::cout << i << " ::: \n";
            for (int j = 0;j < polygons[i].size(); ++j)
            {
                if (j < 10)
                    std::cout << ' ';
                std::cout << j << ' ';
            }
            std::cout << '\n';
            for (int j = 0;j < polygons[i].size(); ++j)
            {
                if (polygons[i][j].first < 10 && polygons[i][j].first> -1)
                    std::cout << ' ';
                std::cout << polygons[i][j].first << ' ';
            }
            std::cout << '\n';
            for (int j = 0;j < polygons[i].size(); ++j)
            {
                if (polygons[i][j].second < 10 && polygons[i][j].second > -1)
                    std::cout << ' ';
                std::cout << polygons[i][j].second << ' ';
            }
            std::cout << '\n';
        }
    }

    void Graph::divide_on_triangles(std::vector<std::pair<int, int> > &polygon,
                                    const std::vector<int> &points,
                                    std::vector<std::vector<Vertex> > &cur_list)
    {
        std::vector<int> vertex_side = set_side_for_edges(polygon, points);
        std::vector<int> stack;
        std::vector<int> used_points;
        for (int i = 0; i < polygon.size(); ++i)
            if (polygon[i].first != -1)
            {
                used_points.push_back(i);
                if (polygon[i].first > polygon[i].second)
                    std::swap(polygon[i].first, polygon[i].second);
            }
        stack.push_back(used_points[0]);
        stack.push_back(used_points[1]);

        for (int i = 2;i < used_points.size()-1; ++i)
        {
            int cur_vertex = used_points[i];
            int stack_top = *(stack.end()-1);
            stack.pop_back();
            if (vertex_side[stack_top] == vertex_side[cur_vertex])
            {
                int elem = -1;
                while (!stack.empty())
                {
                    elem = *(stack.end()-1);
                    stack.pop_back();
                    if (check_angle_side(elem, stack_top, cur_vertex, vertex_side[cur_vertex], points))
                    {
                        std::shared_ptr<int> id = std::make_shared<int>(-1);
                        cur_list[elem].emplace_back(cur_vertex, id);
                        cur_list[cur_vertex].emplace_back(elem, id);
                        stack_top = elem;
                    } else {
                        stack.push_back(elem);
                        break;
                    }
                }
            }
            else
            {
                int elem = -1;
                stack.push_back(stack_top);
                while (!stack.empty())
                {
                    elem = *(stack.end()-1);
                    stack.pop_back();
                    if (elem != polygon[cur_vertex].first)
                    {
                        std::shared_ptr<int> id = std::make_shared<int>(-1);
                        cur_list[elem].emplace_back(cur_vertex, id);
                        cur_list[cur_vertex].emplace_back(elem, id);
                    }
                }
            }
            stack.push_back(stack_top);
            stack.push_back(cur_vertex);
        }

        int last_vertex = *(used_points.end()-1);
        int elem = -1;
        while (!stack.empty())
        {
            elem = *(stack.end()-1);
            stack.pop_back();
            if (elem != polygon[last_vertex].first && elem != polygon[last_vertex].second)
            {
                std::shared_ptr<int> id = std::make_shared<int>(-1);
                cur_list[elem].emplace_back(last_vertex, id);
                cur_list[last_vertex].emplace_back(elem, id);
            }
        }
    }

    std::vector<int> Graph::set_side_for_edges(const std::vector<std::pair<int, int> > &polygon,
                                               const std::vector<int> &points)
    {
        std::vector<int> ans(polygon.size(), -1);                      //-1 - not in polygon
        int j = 0;
        while (polygon[j].first == -1) ++j;
        int left_vertex = j, right_vertex = j;
        ans[left_vertex] = 2;
        left_vertex = polygon[j].first;
        right_vertex = polygon[j].second;
        TPoint point_a = vertexes[points[left_vertex]],
               point_b = vertexes[points[right_vertex]],
               point_mid = vertexes[points[j]],
               set_point(point_mid.y - std::abs(2 * point_mid.y), point_mid.y);

        double angle1 = get_angle(set_point, point_a, point_mid);
        double angle2 = get_angle(set_point, point_b, point_mid);
        if (angle1 > angle2)
            std::swap(left_vertex, right_vertex);
        set_side_for_chains(polygon, ans, left_vertex, 1);          //1 - left
        set_side_for_chains(polygon, ans, right_vertex, 2);         //2 - right
        return ans;
    }

    void Graph::set_side_for_chains(const std::vector<std::pair<int, int> > &polygon, std::vector<int> &chains_side, int vertex, int type)
    {
        chains_side[vertex] = type;
        while (polygon[vertex].first > vertex || polygon[vertex].second > vertex)
        {
            vertex = std::max(polygon[vertex].first, polygon[vertex].second);
            chains_side[vertex] = type;
        }
    }

    bool Graph::check_angle_side(int last_vertex, int middle_vertex, int current_vertex, int vertex_side,
                                 const std::vector<int> &points)
    {
        TPoint point_last = vertexes[points[last_vertex]];
        TPoint point_mid = vertexes[points[middle_vertex]];
        TPoint point_cur = vertexes[points[current_vertex]];
        double angle = get_angle(point_last, point_cur, point_mid);
        if (std::abs(angle) < 0.00001 || std::abs(angle - 180.0) < 0.00001)
            return false;
        return (angle < 0 && vertex_side == 1) || (angle > 0 && vertex_side == 2);
    }

    void Graph::next_layer(int layer_id)
    {
        std::vector<int> points = triangulation_vertexes[layer_id];
        std::vector<std::pair<int, int> > number_of_adjacency;
        for (int i = 0; i < points.size(); ++i)
            number_of_adjacency.emplace_back(triangulation_lists[layer_id][i].size(), i);
        std::sort(number_of_adjacency.begin(), number_of_adjacency.end(), std::greater<>());
        std::vector<std::vector<Vertex> > new_list;
        std::vector<int> new_points;
        std::vector<int> vertexes_to_delete;
        std::vector<bool> used(points.size(), false);
        for (auto & i : number_of_adjacency)
        {
            int cur_v = i.second;
            if (cur_v != 0 && cur_v != 1 && cur_v != points.size()-1 && !used[cur_v])
            {
                vertexes_to_delete.push_back(cur_v);
                used[cur_v] = true;
                for (int j = 0; j < triangulation_lists[layer_id][cur_v].size(); ++j)
                    used[triangulation_lists[layer_id][cur_v][j].next_v] = true;
            }
        }
        sort(vertexes_to_delete.begin(), vertexes_to_delete.end());
        std::vector<int> interception(points.size());
        for (int j = 0, k = 0;j < points.size(); ++j)
        {
            if (k < vertexes_to_delete.size() && points[vertexes_to_delete[k]] == points[j])
            {
                ++k;
                interception[j] = -1;
            } else{
                interception[j] = new_points.size();
                new_points.push_back(points[j]);
            }
        }
        new_list.resize(points.size()-vertexes_to_delete.size());
        for (int i = 0;i < triangulation_lists[layer_id].size(); ++i)
        {
            if (interception[i] != -1)
            {
                for (int j = 0;j < triangulation_lists[layer_id][i].size(); ++j)
                {
                    int next_v = triangulation_lists[layer_id][i][j].next_v;
                    if (interception[next_v] != -1)
                    {
                        if (interception[next_v] > interception[i])
                        {
                            int new_next_v = interception[next_v], new_cur_v = interception[i];
                            std::shared_ptr<int> id = std::make_shared<int>(-1);
                            new_list[new_cur_v].emplace_back(new_next_v, id);
                            new_list[new_next_v].emplace_back(new_cur_v, id);
                        }
                    }
                }
            }
        }
        //triangulation

        ++layer_id;
        triangulation_vertexes.push_back(new_points);
        triangulation_lists.push_back(new_list);
        sort_counter_clockwise(triangulation_vertexes[layer_id], triangulation_lists[layer_id]);

        std::vector<TEdge> new_edges = create_monotonous_polygon(triangulation_vertexes[layer_id],
                triangulation_lists[layer_id]);
        add_new_edges(triangulation_lists[layer_id], new_edges);
        triangulate(triangulation_vertexes[layer_id], triangulation_lists[layer_id]);

        int triangles_number = count_triangles(layer_id);
    }

    int Graph::count_triangles(int layer_id)
    {
        sort_counter_clockwise(triangulation_vertexes[layer_id], triangulation_lists[layer_id]);
        int size = triangulation_vertexes[layer_id].size();
        std::vector<TTreeVertex> layer;
        for (int start_v = 0; start_v < size; ++start_v)
        {
            for (int j = 0;j < triangulation_lists[layer_id][start_v].size(); ++j)
            {
                if (triangulation_lists[layer_id][start_v][j].triangle_id != -1)
                    continue;
                triangulation_lists[layer_id][start_v][j].triangle_id = layer.size();
                TTreeVertex cur_triangle;
                int next_v = triangulation_lists[layer_id][start_v][j].next_v;
                cur_triangle.vertex_id[0] = start_v;
                cur_triangle.vertex_id[1] = next_v;
                for (int i = 0;i < triangulation_lists[layer_id][next_v].size(); ++i)
                {
                    if (triangulation_lists[layer_id][next_v][i].next_v == start_v)
                    {
                        int next_v_size = triangulation_lists[layer_id][next_v].size();
                        cur_triangle.vertex_id[2] = triangulation_lists[layer_id][next_v][(i+1)%next_v_size].next_v;
                        triangulation_lists[layer_id][next_v][(i+1)%next_v_size].triangle_id = layer.size();
                        break;
                    }
                }
                int cur_v = next_v; next_v = cur_triangle.vertex_id[2];
                for (int i = 0;i < triangulation_lists[layer_id][next_v].size(); ++i)
                {
                    if (triangulation_lists[layer_id][next_v][i].next_v == cur_v)
                    {
                        int next_v_size = triangulation_lists[layer_id][next_v].size();
                        triangulation_lists[layer_id][next_v][(i+1)%next_v_size].triangle_id = layer.size();
                        break;
                    }
                }
                layer.push_back(cur_triangle);
            }
        }
        triangulation_tree.push_back(layer);
        return layer.size();
    }

    void Graph::first_layer()
    {
        add_triangle();
        std::vector<int> indeces;
        for (int i = 0;i < vertex_number; ++i)
            indeces.push_back(i);

        triangulation_vertexes.push_back(indeces);
        triangulation_lists.push_back(adjacency_list);
        sort_counter_clockwise(indeces, triangulation_lists[0]);

        std::vector<TEdge> new_edges = create_monotonous_polygon(triangulation_vertexes[0], triangulation_lists[0]);
        add_new_edges(triangulation_lists[0], new_edges);
        triangulate(triangulation_vertexes[0], triangulation_lists[0]);

        int triangles_number = count_triangles(0);
    }


    void Graph::output_triangulation_vertexes(int index)
    {
        for (int i = 0;i < triangulation_vertexes[index].size(); ++i)
            std::cout << triangulation_vertexes[index][i] << ' ';
        std::cout << '\n';
    }

    void Graph::output_all_triangulation_layers()
    {
        for (int i = 0;i < triangulation_lists.size(); ++i)
        {
            std::cout << "Layer : " << i << '\n';
            output_triangulation_vertexes(i);
            output_triangulation_list(i);
        }
    }

    void Graph::output_triangles(int index)
    {
        for (int i = 0;i < triangulation_tree[index].size(); ++i)
        {
            TTreeVertex v = triangulation_tree[index][i];
            std::cout << i << ": " <<  triangulation_vertexes[index][v.vertex_id[0]] << ' '
                << triangulation_vertexes[index][v.vertex_id[1]] << ' '
                << triangulation_vertexes[index][v.vertex_id[2]];
            if (triangulation_tree[index][i].next_ids.size() > 0)
            {
                std::cout << " ::: ";
                for (int j = 0; j < triangulation_tree[index][i].next_ids.size(); ++j)
                {
                    std::cout << triangulation_tree[index][i].next_ids[j] << ' ';
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    void Graph::output_tree()
    {
        for (int i = 0;i < triangulation_tree.size(); ++i)
        {
            std::cout << "Layer : " << i << '\n';
            output_triangles(i);
        }
    }

    void Graph::layers_connection(int layer_id)
    {
        std::cout << "Nya : " << layer_id << '\n';
        output_triangles(layer_id);
        int next_layer_id = layer_id+1;
        std::vector<int> difference = get_difference(layer_id);
        std::vector<int> cur_points = triangulation_vertexes[layer_id], next_points = triangulation_vertexes[next_layer_id];
        std::vector<int> interception(cur_points.size());
        std::vector<int> back_interception(cur_points.size() - difference.size());
        for (int i = 0, k = 0;i < cur_points.size(); ++i)
        {
            if (cur_points[i] != next_points[k])
                interception[i] = -1;
            else
            {
                interception[i] = k;
                back_interception[k] = i;
                ++k;
            }
        }
        for (int i = 0, dif = 0;i < cur_points.size(); ++i)
        {
            if (dif >= difference.size() || cur_points[i] != difference[dif])
                continue;
            ++dif;
            std::vector<int> polygon, triangle_ids;
            for (int j = 0; j < triangulation_lists[layer_id][i].size(); ++j)
            {
                polygon.push_back(triangulation_lists[layer_id][i][j].next_v);
                triangle_ids.push_back(triangulation_lists[layer_id][i][j].triangle_id);
            }
            std::vector<int> sorted_by_ind_polygon = polygon;
            std::sort(sorted_by_ind_polygon.begin(), sorted_by_ind_polygon.end());

            for (int j = 0; j < sorted_by_ind_polygon.size(); ++j)
            {
                int cur_v = interception[sorted_by_ind_polygon[j]];
                if (cur_v == -1)
                    continue;
                for (int k = 0; k < triangulation_lists[next_layer_id][cur_v].size(); ++k)
                {
                    int next_v = triangulation_lists[next_layer_id][cur_v][k].next_v;
                    auto pos = std::lower_bound(sorted_by_ind_polygon.begin(), sorted_by_ind_polygon.end(),
                                                back_interception[next_v]);
                    if (pos == sorted_by_ind_polygon.end() || *pos != back_interception[next_v] || next_v < cur_v)
                        continue;

                    int triangle_id = triangulation_lists[next_layer_id][cur_v][k].triangle_id;
                    TTreeVertex triangle = triangulation_tree[next_layer_id][triangle_id];
                    int last_v = triangle.vertex_id[0];
                    if (cur_v == last_v || next_v == last_v)
                        last_v = triangle.vertex_id[1];
                    if (cur_v == last_v || next_v == last_v)
                        last_v = triangle.vertex_id[2];
                    pos = std::lower_bound(sorted_by_ind_polygon.begin(), sorted_by_ind_polygon.end(),
                                           back_interception[last_v]);
                    if (pos == sorted_by_ind_polygon.end() || *pos != back_interception[last_v])
                        continue;
                    std::pair<int, int> zero_triangles = get_bounds(polygon, i, triangle.vertex_id[0], triangle.vertex_id[2], layer_id);
                    std::pair<int, int> first_triangles = get_bounds(polygon, i, triangle.vertex_id[1], triangle.vertex_id[2], layer_id);
                    std::pair<int, int> second_triangles = get_bounds(polygon, i, triangle.vertex_id[0], triangle.vertex_id[1], layer_id);

                    std::vector<int> temp1(0), temp2(0), temp3(0), temp4(0);
                    triangle.next_ids.clear();
                    for (int v = zero_triangles.first; v != zero_triangles.second; v = (v+1)%polygon.size())
                        temp1.push_back(triangle_ids[v]);
                    for (int v = first_triangles.first; v != first_triangles.second; v = (v+1)%polygon.size())
                        temp2.push_back(triangle_ids[v]);
                    for (int v = second_triangles.first; v != second_triangles.second; v = (v+1)%polygon.size())
                        temp3.push_back(triangle_ids[v]);

                    std::sort(temp1.begin(), temp1.end());
                    std::sort(temp2.begin(), temp2.end());
                    std::sort(temp3.begin(), temp3.end());
                    std::set_union(temp1.begin(), temp1.end(), temp2.begin(), temp2.end(),
                            std::back_inserter(temp4));

                    std::sort(temp4.begin(), temp4.end());
                    std::set_union(temp4.begin(), temp4.end(), temp3.begin(), temp3.end(),
                            std::back_inserter(triangle.next_ids));

                    triangulation_tree[next_layer_id][triangle_id] = triangle;
                }
            }
        }
        std::vector<std::pair<TTreeVertex, std::pair<int, int> > > all_unused;
        for (int i = 0;i < triangulation_tree[next_layer_id].size(); ++i)
            if (triangulation_tree[next_layer_id][i].next_ids.empty())
                all_unused.push_back({triangulation_tree[next_layer_id][i], {i, next_layer_id}});
        for (int i = 0;i < triangulation_tree[layer_id].size(); ++i)
            all_unused.push_back({triangulation_tree[layer_id][i], {i, layer_id}});

        std::sort(all_unused.begin(), all_unused.end(),
                [this](const std::pair<TTreeVertex, std::pair<int, int> > &a,
                        std::pair<TTreeVertex, std::pair<int, int> > &b) -> bool
                        {
                            int a_layer = a.second.second, b_layer = b.second.second;
                            int a_index = a.second.first, b_index = b.second.first;
                            TTreeVertex a_triang = a.first, b_triang = b.first;
                            for (int i = 0;i < 3; ++i)
                            {
                                if (triangulation_vertexes[a_layer][a_triang.vertex_id[i]] <
                                    triangulation_vertexes[b_layer][b_triang.vertex_id[i]])
                                    return true;
                                if (triangulation_vertexes[a_layer][a_triang.vertex_id[i]] >
                                    triangulation_vertexes[b_layer][b_triang.vertex_id[i]])
                                    return false;
                            }
                            return a_layer > b_layer;
                        });

        for (int i = 0;i < all_unused.size()-1; ++i)
        {
            int layer = all_unused[i].second.second;
            int next_layer = all_unused[i+1].second.second;
            TTreeVertex current_triag = all_unused[i].first, next_triag = all_unused[i+1].first;;
            if (triangulation_vertexes[layer][current_triag.vertex_id[0]] ==
                    triangulation_vertexes[next_layer][next_triag.vertex_id[0]] &&
                triangulation_vertexes[layer][current_triag.vertex_id[1]] ==
                    triangulation_vertexes[next_layer][next_triag.vertex_id[1]]  &&
                triangulation_vertexes[layer][current_triag.vertex_id[2]] ==
                    triangulation_vertexes[next_layer][next_triag.vertex_id[2]] )
            {
                triangulation_tree[layer][all_unused[i].second.first].next_ids.
                    push_back(all_unused[i+1].second.first);
            }
        }
    }

    std::vector<int> Graph::get_difference(int layer_id)
    {
        std::vector<int> cur_points = triangulation_vertexes[layer_id], next_points = triangulation_vertexes[layer_id+1];
        std::vector<int> difference(cur_points);
        difference.insert(difference.end(), next_points.begin(), next_points.end());
        std::sort(difference.begin(), difference.end());
        std::vector<int> temp;
        for (int i = 0;i < difference.size()-1; ++i)
        {
            if (difference[i] != difference[i+1])
                temp.push_back(difference[i]);
            else
                ++i;
        }
        return temp;
    }

    int Graph::check_in_triangle(TTreeVertex triangle, int layer, int vertex)
    {
        TPoint a = vertexes[triangulation_vertexes[layer+1][triangle.vertex_id[0]]];
        TPoint b = vertexes[triangulation_vertexes[layer+1][triangle.vertex_id[1]]];
        TPoint c = vertexes[triangulation_vertexes[layer+1][triangle.vertex_id[2]]];
        TPoint p = vertexes[triangulation_vertexes[layer][vertex]];
        double angle_a = get_angle(a, b, c), angle_point_a = get_angle(a, p, c);
        double angle_b = get_angle(b, c, a), angle_point_b = get_angle(b, p, a);
        double angle_c = get_angle(c, a, b), angle_point_c = get_angle(c, p, b);
        double eps = 0.00001;
        if (std::abs(angle_a - angle_point_a) < eps)
            return 1;
        if (std::abs(angle_b - angle_point_b) < eps)
            return 2;
        if (std::abs(angle_c - angle_point_c) < eps)
            return 3;
        if (angle_a*angle_point_a > 0 && angle_b*angle_point_b > 0 && angle_c*angle_point_c > 0)
            return 0;
        return -1;
    }

    std::pair<int, int> Graph::get_bounds(const std::vector<int>& polygon, int center, int start_v, int end_v, int layer_id)
    {
        TPoint a = vertexes[triangulation_vertexes[layer_id+1][start_v]];
        TPoint b = vertexes[triangulation_vertexes[layer_id+1][end_v]];
        TPoint c = vertexes[triangulation_vertexes[layer_id][center]];
        double eps = 0.000001;
        double temp_angle = get_angle(a, b, c);
        if (temp_angle < 0)
            std::swap(start_v, end_v);


        auto compare = [polygon, layer_id, center, this](int mid, int find)-> int
        {
            TPoint find_p = vertexes[triangulation_vertexes[layer_id+1][find]];
            TPoint center_p = vertexes[triangulation_vertexes[layer_id][center]];
            TPoint mid_p = vertexes[triangulation_vertexes[layer_id][polygon[mid]]];
            TPoint temp_p = {center_p.x-2*(center_p.x), center_p.y};
            double angle1 = get_angle(temp_p, find_p, center_p);
            double angle2 = get_angle(temp_p, mid_p, center_p);
            if (angle1 < 0)
                angle1 = std::abs(angle1);
            else
                angle1 = 360.0 - angle1;
            if (angle2 < 0)
                angle2 = std::abs(angle2);
            else
                angle2 = 360.0 - angle2;
            double eps = 0.0000001;
            if (std::abs(angle1-angle2) < eps)
                return 0;
            if (angle1 > angle2)
                return 1;
            else
                return -1;
        };
        std::pair<int, int> ans(-1, -1);
        int l = -1, r = polygon.size();
        while (r - l > 1)
        {
            int mid = (r+l)/2;
            if (compare(mid, start_v) > 0)
                r = mid;
            else
                l = mid;
        }
        ans.first = (r+polygon.size())%polygon.size();

        l = -1, r = polygon.size();
        while (r - l > 1)
        {
            int mid = (r+l)/2;
            if (compare(mid, end_v) > 0)
                r = mid;
            else
                l = mid;
        }
        ans.second = (r+polygon.size())%polygon.size();

        return ans;
    }

}
