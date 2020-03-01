//
// Created by art on 3/1/20.
//

#ifndef LAB5_QUICKHULL_H
#define LAB5_QUICKHULL_H


#include <vector>
#include <string>

namespace quickhull_space
{
    struct TPoint
    {
        double x, y;

        TPoint() : x(0), y(0) {};

        TPoint(double x, double y) : x(x), y(y) {};
    };

    class QuickHull
    {
        const double eps = 0.0000001;
        int vertex_number;
        std::vector<TPoint> vertexes;
        std::vector<int> convex_hull;

        double get_angle(int a, int b, int u);
        std::vector<int> get_up(int start, int end, std::vector<int> points);
        double get_distance(int a, int b, int u);

        void quickhull(int start, int end, std::vector<int> points);
    public:
        QuickHull()
        {
            vertexes.clear();
            convex_hull.clear();
        }

        void input_graph(std::string file_name);

        void create_convex_hull();

        void output_convex_hull();
    };
}


#endif //LAB5_QUICKHULL_H
