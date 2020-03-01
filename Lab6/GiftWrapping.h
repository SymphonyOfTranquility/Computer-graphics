//
// Created by art on 3/1/20.
//

#ifndef LAB6_GIFTWRAPPING_H
#define LAB6_GIFTWRAPPING_H

#include <vector>
#include <string>

namespace gift_wrapping_space
{
    struct TPoint
    {
        double x, y;

        TPoint() : x(0), y(0) {};

        TPoint(double x, double y) : x(x), y(y) {};
    };

    class GiftWrapping
    {
        const double eps = 0.0000001;
        int vertex_number;
        std::vector<TPoint> vertexes;
        std::vector<int> convex_hull;

        double get_angle(TPoint a, TPoint b, TPoint u);
        double get_distance_between_points(TPoint a, TPoint b);
    public:
        GiftWrapping() : vertex_number(0)
        {
            vertexes.clear();
            convex_hull.clear();
        }

        void input_graph(std::string file_name);

        void jarvis_convex_hull();

        void output_convex_hull();
    };
}


#endif //LAB6_GIFTWRAPPING_H
