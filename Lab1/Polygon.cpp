//
// Created by art on 1/29/20.
//

#include "Polygon.h"

#include <cmath>
#include <fstream>

namespace polygon_space
{
    TVector::TVector(double x1, double y1, double x2, double y2)
    {
        direction = {x1 - x2, y1 - y2};
    }

    TVector::TVector(TPoint u, TPoint v)
    {
        direction = {u.x - v.x, u.y - v.y};
    }

    double TVector::get_angle(TVector vec)
    {
        double dot = direction.x*vec.direction.x + direction.y*vec.direction.y;
        double det = direction.x*vec.direction.y - direction.y*vec.direction.x;
        return atan2(det, dot)*180.0/M_PI;
    }

    bool TVector::is_on_left_side(TVector mid, TVector p)
    {
        double angle_mid = get_angle(mid), angle_point = get_angle(p), eps = 0.0000001;
        if (std::abs(angle_point) < eps || (std::abs(angle_mid-angle_point ) < eps))
            return true;
        if (angle_mid < 0.0 && angle_point > 0.0)
            return false;
        if (angle_mid > 0.0 && angle_point < 0.0)
            return true;
        return (angle_point - angle_mid) >= 0;
    }

    bool TVector::is_on_same_side(TVector center, TVector point)
    {
        double angle_center = get_angle(center), angle_point = get_angle(point), eps = 0.0000001;
        if (std::abs(angle_point) < eps || (std::abs(angle_center-angle_point ) < eps))
            return true;
        return angle_center * angle_point >= 0;
    }

    Polygon::Polygon(std::string pass_to_input_file)
    {
        input_from_file(pass_to_input_file);
    }

    void Polygon::input_from_file(std::string pass_to_input_file)
    {
        std::ifstream file;
        file.open(pass_to_input_file);
        file >> size;
        points.clear();
        for (int i = 0; i < size; ++i)
        {
            TPoint point;
            file >> point.x >> point.y;
            points.push_back(point);
        }
    }

    bool Polygon::check_point_in_polygon(const TPoint &point)
    {
        TPoint center = mid_dot(mid_dot(points[0], points[1]), points[2]);

        int l = 0, r = size;
        while (r - l > 1)
        {
            int mid = (r + l) / 2;
            if (check_rays(l, mid, center, point))
                r = mid;
            else
                l = mid;
        }
        return check_last_edge(l, r, center, point);
    }

    bool Polygon::check_rays(int left_ray, int mid_ray, const TPoint &center, const TPoint &point)
    {
        TVector left_vector = TVector(points[left_ray], center), mid_vector = TVector(points[mid_ray], center);
        TVector point_vector = TVector(point, center);
        return left_vector.is_on_left_side(mid_vector, point_vector);
    }

    TPoint Polygon::mid_dot(TPoint a, TPoint b)
    {
        return {(a.x + b.x) / 2.0, (a.y + b.y) / 2.0};
    }

    bool Polygon::check_last_edge(int left_point, int right_point, const TPoint &center, const TPoint &point)
    {
        TVector edge_vector = TVector(points[left_point], points[right_point%size]);
        TVector center_vector = TVector(center, points[right_point%size]);
        TVector point_vector = TVector(point, points[right_point%size]);
        return edge_vector.is_on_same_side(center_vector, point_vector);
    }
}