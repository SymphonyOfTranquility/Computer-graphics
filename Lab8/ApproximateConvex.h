//
// Created by art on 3/15/20.
//

#ifndef LAB8_APPROXIMATECONVEX_H
#define LAB8_APPROXIMATECONVEX_H

#include <vector>
#include <string>

namespace convexhull_space
{
    struct TPoint
    {
        double x, y;

        TPoint() : x(0), y(0) {};

        TPoint(double x, double y) : x(x), y(y) {};
    };

    class ApproximateConvex
    {
        const double eps = 0.0000001;
        int vertex_number;
        std::vector<TPoint> vertexes;
        std::vector<int> convex_hull;
        std::vector<std::pair<std::vector<int>,std::vector<int> > > max_min_of_slice;

        bool get_side(TPoint a, TPoint b, TPoint c);

        void grehem_part(std::vector<int> vec);
        void get_convex_grehem(int number_of_slices);
    public:
        ApproximateConvex()
        {
            vertexes.clear();
            convex_hull.clear();
            max_min_of_slice.clear();
        }

        void input_points(std::string file_name);

        void get_approximate(int number_of_slices);

        void output();
    };
}

#endif //LAB8_APPROXIMATECONVEX_H
