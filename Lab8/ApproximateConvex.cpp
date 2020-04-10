//
// Created by art on 3/15/20.
//

#include "ApproximateConvex.h"

#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace convexhull_space
{
    void ApproximateConvex::input_po
    ints(std::string file_name)
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
    }

    void ApproximateConvex::get_approximate(int number_of_slices)
    {
        int min_id = 0, max_id = 0;
        for (int i = 1; i < vertex_number; ++i)
        {
            if (vertexes[i].x < vertexes[min_id].x)
                min_id = i;
            if (vertexes[i].x > vertexes[max_id].x)
                max_id = i;
        }
        double step = std::abs(vertexes[max_id].x - vertexes[min_id].x)/static_cast<double>(number_of_slices);
        max_min_of_slice.clear();
        max_min_of_slice.resize(number_of_slices+1);
        for (int i = 0;i < vertex_number; ++i)
        {
            if (std::abs(vertexes[i].x-vertexes[min_id].x) < eps)
            {
                max_min_of_slice.front().first.push_back(i);
                continue;
            }
            if (std::abs(vertexes[i].x-vertexes[max_id].x) < eps)
            {
                max_min_of_slice.front().second.push_back(i);
                continue;
            }

            int id_array = static_cast<int>(trunc((vertexes[i].x-vertexes[min_id].x)/step));
            ++id_array;
            if (max_min_of_slice[id_array].first.empty())
            {
                max_min_of_slice[id_array].first.push_back(i);
                max_min_of_slice[id_array].second.push_back(i);
                continue;
            }
            int cur_id_min = max_min_of_slice[id_array].first.front();
            int cur_id_max = max_min_of_slice[id_array].second.front();
            if (std::abs(vertexes[i].y-vertexes[cur_id_min].y) < eps)
                max_min_of_slice[id_array].first.push_back(i);
            else if (vertexes[i].y < vertexes[cur_id_min].y)
            {
                max_min_of_slice[id_array].first.clear();
                max_min_of_slice[id_array].first.push_back(i);
            }

            if (std::abs(vertexes[i].y-vertexes[cur_id_max].y) < eps)
                max_min_of_slice[id_array].second.push_back(i);
            else if (vertexes[i].y > vertexes[cur_id_max].y)
            {
                max_min_of_slice[id_array].second.clear();
                max_min_of_slice[id_array].second.push_back(i);
            }
        }

        get_convex_grehem(number_of_slices);
    }

    void ApproximateConvex::get_convex_grehem(int number_of_slices)
    {
        std::vector<int> slice(0);
        for (int i = 0; i <= number_of_slices; ++i)
        {
            if (!max_min_of_slice[i].first.empty())
            {
                std::copy(max_min_of_slice[i].second.begin(), max_min_of_slice[i].second.end(),
                          std::back_inserter(slice));
                std::copy(max_min_of_slice[i].first.begin(), max_min_of_slice[i].first.end(),
                          std::back_inserter(slice));
            }
        }
        std::sort(slice.begin(), slice.end());
        slice.erase(std::unique(slice.begin(),slice.end()), slice.end());
        std::sort(slice.begin(), slice.end(),
                [this](const int &a,  const int &b) -> bool
                {
                    if (std::abs(vertexes[a].x - vertexes[b].x) < eps)
                        return vertexes[a].y < vertexes[b].y;
                    return vertexes[a].x < vertexes[b].x;
                });
        grehem_part(slice);
    }

    void ApproximateConvex::grehem_part(std::vector<int> points)
    {
        if (points.size() <= 2)
        {
            convex_hull = points;
            return;
        }
        TPoint start_p = vertexes[points.front()], last_p = vertexes[points.back()];
        TPoint cur_p = vertexes[points.front()], prev_p = {cur_p.x, -2*std::abs(cur_p.y)};
        std::vector<int> up_conv, down_conv;
        up_conv.push_back(points.front());
        down_conv.push_back(points.front());
        for (int i = 1;i < points.size(); ++i)
        {
            if (i == points.size() - 1 || get_side(start_p, last_p, vertexes[points[i]]))
            {
                while (up_conv.size() >= 2 &&
                       !get_side(vertexes[*(up_conv.end() - 2)], vertexes[points[i]], vertexes[up_conv.back()]))
                    up_conv.pop_back();
                up_conv.push_back(points[i]);
            }
            if (i == points.size() - 1 || get_side(last_p, start_p, vertexes[points[i]]))
            {
                while (down_conv.size() >= 2 &&
                       !get_side(vertexes[points[i]], vertexes[*(down_conv.end() - 2)], vertexes[down_conv.back()]))
                    down_conv.pop_back();
                down_conv.push_back(points[i]);
            }
        }
        convex_hull.clear();
        std::copy(up_conv.begin(), up_conv.end(), std::back_inserter(convex_hull));
        if (down_conv.size() > 2)
            std::copy(down_conv.rbegin()+1, down_conv.rend()-1, std::back_inserter(convex_hull));
    }

    bool ApproximateConvex::get_side(TPoint a, TPoint b, TPoint u)
    {
        double val = (b.x - a.x) * (u.y - a.y) - (b.y - a.y) * (u.x - a.x);
        return std::abs(val) < eps || val > 0;
    }

    void ApproximateConvex::output()
    {
        std::cout << "Number of slices: " << max_min_of_slice.size()-1 << '\n';
        for (int i = 0;i < convex_hull.size(); ++i)
            std::cout << convex_hull[i] << ' ';
        std::cout << '\n';
    }

}