//
// Created by art on 3/1/20.
//

#include <fstream>
#include <cmath>
#include <iostream>
#include "QuickHull.h"

namespace quickhull_space
{
    void QuickHull::input_graph(std::string file_name)
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


    std::vector<int> QuickHull::get_up(int start, int end, std::vector<int> points)
    {
        std::vector<int> next;
        for (int i = 0;i < points.size(); ++i)
        {
            if (points[i] == start || points[i] == end)
                next.push_back(points[i]);
            else
            {
                int angle = get_angle(start, points[i], end);
                if (std::abs(angle) < eps || angle > 0.0)
                    next.push_back(points[i]);
            }
        }
        return next;
    }

    void QuickHull::create_convex_hull()
    {
        int min_id = 0, max_id = 0;
        std::vector<int> cur_vertexes(1, 0);
        for (int i = 1;i < vertex_number; ++i)
        {
            cur_vertexes.push_back(i);
            if (std::abs(vertexes[i].x - vertexes[max_id].x) < eps)
            {
                if (vertexes[i].y > vertexes[max_id].y)
                    max_id = i;
            }
            else
            {
                if (vertexes[i].x > vertexes[max_id].x)
                    max_id = i;
            }
            if (std::abs(vertexes[i].x - vertexes[min_id].x) < eps)
            {
                if (vertexes[i].y < vertexes[min_id].y)
                    min_id = i;
            }
            else if (vertexes[i].x < vertexes[min_id].x)
                min_id = i;
        }
        convex_hull.push_back(min_id);
        std::vector<int> top = get_up(min_id, max_id, cur_vertexes);
        quickhull(min_id, max_id, top);

        convex_hull.push_back(max_id);
        std::vector<int> bottom = get_up(max_id, min_id, cur_vertexes);
        quickhull(max_id, min_id, bottom);
    }

    double QuickHull::get_angle(int a_id, int b_id, int u_id)
    {
        TPoint a = vertexes[a_id], b = vertexes[b_id], u = vertexes[u_id];
        TPoint vec_a = {a.x - u.x, a.y - u.y};
        TPoint vec_b = {b.x - u.x, b.y - u.y};
        double dot = vec_a.x*vec_b.x + vec_a.y*vec_b.y;
        double det = vec_a.x*vec_b.y - vec_a.y*vec_b.x;
        return atan2(det, dot)*180.0/M_PI;
    }

    void QuickHull::quickhull(int start, int end, std::vector<int> points)
    {
        if (points.size() == 2)
            return;
        if (points.size() == 3)
        {
            int i = 0;
            while (points[i] == end || points[i] == start) ++i;
            convex_hull.push_back(points[i]);
            return;
        }

        int mid = -1; double dist = -1;
        for (int i = 0;i < points.size(); ++i)
        {
            double cur_dist = get_distance(start, end, points[i]);
            if (cur_dist > dist)
            {
                mid = i;
                dist = cur_dist;
            }
        }
        std::vector<int> top = get_up(start, points[mid], points);
        quickhull(start, points[mid], top);
        convex_hull.push_back(points[mid]);
        std::vector<int> bottom = get_up(points[mid], end, points);
        quickhull(points[mid], end, bottom);
    }

    double QuickHull::get_distance(int a_id, int b_id, int u_id)
    {
        TPoint a = vertexes[a_id], b = vertexes[b_id], u = vertexes[u_id];
        return std::abs(((u.x - a.x)*(a.y - b.y) - (u.y - a.y)*(a.x - b.x))/std::sqrt((a.y-b.y)*(a.y-b.y) + (a.x-b.x)*( a.x-b.x)));
    }

    void QuickHull::output_convex_hull()
    {
        std::cout << "Size:" << convex_hull.size() << '\n';
        for (int i = 0;i < convex_hull.size(); ++i)
            std::cout << convex_hull[i] << ' ';
        std::cout << '\n';
    }
}
