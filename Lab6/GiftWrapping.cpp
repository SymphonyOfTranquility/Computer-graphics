//
// Created by art on 3/1/20.
//

#include <fstream>
#include <cmath>
#include <iostream>

#include "GiftWrapping.h"


namespace gift_wrapping_space
{

    void GiftWrapping::input_graph(std::string file_name)
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

    void GiftWrapping::jarvis_convex_hull()
    {
        int min_id = 0;
        for (int i = 1; i < vertex_number; ++i)
        {
            if (std::abs(vertexes[i].x - vertexes[min_id].x) < eps)
            {
                if (vertexes[i].y < vertexes[min_id].y)
                    min_id = i;
            }
            if (vertexes[i].x < vertexes[min_id].x)
                min_id = i;
        }
        convex_hull.push_back(min_id);
        TPoint cur_p = vertexes[min_id], prev_p = {cur_p.x, -2*std::abs(cur_p.y)};
        int next_id = min_id, cur_id = min_id;
        do
        {
            double next_angle = -1;
            for (int i = 0;i < vertex_number; ++i)
            {
                if (i == cur_id)
                    continue;
                double angle = get_angle(prev_p, vertexes[i], cur_p);
                if (std::abs(angle - next_angle) < eps)
                {
                    double dist1 = get_distance_between_points(cur_p, vertexes[next_id]);
                    double dist2 = get_distance_between_points(cur_p, vertexes[i]);
                    if (dist2 > dist1)
                    {
                        next_angle = angle;
                        next_id = i;
                    }
                }
                if (next_angle < angle)
                {
                    next_angle = angle;
                    next_id = i;
                }
            }
            prev_p = cur_p;
            cur_p = vertexes[next_id];
            cur_id = next_id;
            convex_hull.push_back(next_id);
        }
        while (next_id != min_id);
        convex_hull.pop_back();
    }

    double GiftWrapping::get_angle(TPoint a, TPoint b, TPoint u)
    {
        TPoint vec_a = {a.x - u.x, a.y - u.y};
        TPoint vec_b = {b.x - u.x, b.y - u.y};
        double dot = vec_a.x * vec_b.x + vec_a.y * vec_b.y;
        double det = vec_a.x * vec_b.y - vec_a.y * vec_b.x;
        return atan2(det, dot) * 180.0 / M_PI;
    }

    void GiftWrapping::output_convex_hull()
    {
        std::cout << "Size:" << convex_hull.size() << '\n';
        for (int i = 0; i < convex_hull.size(); ++i)
            std::cout << convex_hull[i] << ' ';
        std::cout << '\n';
    }

    double GiftWrapping::get_distance_between_points(TPoint a, TPoint b)
    {
        return std::sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
    }
}