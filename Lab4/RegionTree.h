//
// Created by art on 2/25/20.
//

#ifndef LAB4_REGIONTREE_H
#define LAB4_REGIONTREE_H


#include <set>
#include <vector>

namespace region_tree
{
    struct TPoint
    {
        double x, y;

        TPoint() : x(0), y(0) {};

        TPoint(double x, double y) : x(x), y(y) {};
    };

    class RegionTree
    {
        struct TVertex{
            std::vector<int> axisY;
            double start, end;
        };

        std::vector<TPoint> points;
        std::vector<TVertex> tree;
        std::vector<int> differ_axis_x;
        int pointsNumber;
        int widthTree;
        constexpr static const double eps = 0.0000001;

        void build(int vertex, int left, int right);
        int binary_search(int vertex, TPoint find, bool up);
        int find_number_in_area(int vertex, TPoint a, TPoint b);
    public:
        RegionTree() : pointsNumber(0), widthTree(0)
        {
            points.clear(), tree.clear();
            differ_axis_x.clear();
        };

        void input_graph(std::string file_name);

        void pre_processing();

        int find_number_in_area(TPoint a, TPoint b);

        void output_tree();
        void output_points();
        void output_differ();
    };
}

#endif //LAB4_REGIONTREE_H
