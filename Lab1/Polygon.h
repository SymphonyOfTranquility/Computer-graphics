//
// Created by art on 1/29/20.
//

#ifndef LAB1_POLYGON_H
#define LAB1_POLYGON_H

#include <vector>
#include <string>

namespace polygon_space
{

    struct TPoint
    {
        double x, y;

        TPoint() = default;

        TPoint(double x, double y) : x(x), y(y) {};
    };

    struct TVector
    {
        TPoint direction;

        TVector(TPoint a):direction(a){};

        TVector(double x1, double y1, double x2, double y2);

        TVector(TPoint u, TPoint v);

        double get_angle(TVector a);
        bool is_on_left_side(TVector mid, TVector p);
    };

    class Polygon
    {
        std::vector<TPoint> points;
        int size;

        bool check_rays(int left_ray, int mid_ray, const TPoint &center, const TPoint &point);

        TPoint get_check_point(int left_point, int mid_point);

        TPoint mid_dot(TPoint a, TPoint b);

    public:
        Polygon():size(0){};

        Polygon(std::string pass_to_file);

        void input_from_file(std::string pass_to_input_file);

        bool check_point_in_polygon(const TPoint &point);
    };
}

#endif //LAB1_POLYGON_H
