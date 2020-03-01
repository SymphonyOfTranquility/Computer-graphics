//
// Created by art on 2/25/20.
//

#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "RegionTree.h"

namespace region_tree
{
    void RegionTree::input_graph(std::string file_name)
    {
        std::ifstream file;
        file.open(file_name);
        file >> pointsNumber;
        for (int i = 0; i < pointsNumber; ++i)
        {
            TPoint p;
            file >> p.x >> p.y;
            points.push_back(p);
        }
    }

    void RegionTree::pre_processing()
    {
        std::sort(points.begin(), points.end(),[](const TPoint &a, const TPoint &b)->bool
        {
            double eps = 0.0000001;
            if (std::abs(a.x-b.x) < eps)
                return  a.y < b.y;
            return a.x < b.x;
        });

        int counter = 1;
        differ_axis_x.push_back(0);
        for (int i = 1; i < pointsNumber; ++i)
        {
            if (std::abs(points[i].x - points[i - 1].x) >= eps)
            {
                ++counter;
                differ_axis_x.push_back(i);
            }
        }
        tree.resize(4 * counter);
        widthTree = differ_axis_x.size();
        build(1, 0, widthTree-1);
    }

    void RegionTree::build( int vertex, int left, int right)
    {
        if (left == right)
        {
            int new_left = differ_axis_x[left], new_right = new_left;
            tree[vertex].start = tree[vertex].end = points[new_left].x;
            while (new_right < pointsNumber && std::abs(points[new_left].x-points[new_right].x) < eps)
            {
                tree[vertex].axisY.push_back(new_right);
                ++new_right;
            }
        }
        else
        {
            int mid = (left+right)/2;
            build(2*vertex, left, mid);
            build(2*vertex+1, mid+1, right);
            tree[vertex].start = tree[vertex*2].start;
            tree[vertex].end = tree[vertex*2+1].end;
            for (int i = 0,j = 0;i < tree[vertex*2].axisY.size() || j < tree[vertex*2+1].axisY.size(); ++i, ++j)
            {
                if (i >= tree[vertex*2].axisY.size())
                    tree[vertex].axisY.push_back(tree[vertex*2+1].axisY[j]);
                else if (j >= tree[vertex*2+1].axisY.size())
                    tree[vertex].axisY.push_back(tree[vertex*2].axisY[i]);
                else
                {
                    int i_ind = tree[vertex*2].axisY[i], j_ind = tree[vertex*2+1].axisY[j];
                    if ((std::abs(points[i_ind].y-points[j_ind].y) < eps && points[i_ind].x < points[j_ind].x) ||
                        points[i_ind].y < points[j_ind].y)
                    {
                        tree[vertex].axisY.push_back(i_ind);
                        --j;
                    }
                    else
                    {
                        tree[vertex].axisY.push_back(j_ind);
                        --i;
                    }
                }
            }
        }
    }

    int RegionTree::find_number_in_area(int vertex, TPoint a, TPoint b)
    {
        if (a.x > b.x || tree[vertex].end < a.x || tree[vertex].start > b.x )
            return 0;
        if ((a.x <= tree[vertex].start && tree[vertex].end <= b.x) ||
            (std::abs(a.x - tree[vertex].start) < eps && tree[vertex].end <= b.x) ||
            (a.x <= tree[vertex].start && std::abs(b.x - tree[vertex].end) < eps) ||
            (std::abs(a.x - tree[vertex].start) < eps && std::abs(b.x - tree[vertex].end) < eps))
        {
            int it_low = binary_search(vertex, a, 0);
            int it_up = binary_search(vertex, b, 1);

            return std::max(it_up-it_low, 0);
        }
        else
        {
            TPoint new_a = a;
            new_a.x = std::max(tree[vertex*2+1].start, a.x);
            TPoint new_b = b;
            new_b.x = std::min(tree[vertex*2].end, b.x);
            return find_number_in_area(2*vertex, a, new_b) +
                    find_number_in_area(2*vertex+1, new_a, b);
        }
    }

    int RegionTree::binary_search(int vertex, TPoint find, bool up)
    {
        int left = -1, right = tree[vertex].axisY.size();
        while (right - left > 1)
        {
            int mid = (right+left)/2;
            int cur_p = tree[vertex].axisY[mid];
            if ((up && std::abs(points[cur_p].y - find.y) < eps) || points[cur_p].y < find.y)
                left = mid;
            else
                right = mid;
        }
        return left;
    }

    int RegionTree::find_number_in_area(TPoint a, TPoint b)
    {
        return find_number_in_area(1, a, b);
    }

    void RegionTree::output_points()
    {
        for (int i = 0;i < pointsNumber; ++i)
            std::cout << i << ' ' << points[i].x << ' '<<  points[i].y << '\n';
    }

    void RegionTree::output_tree()
    {
        int step = 1;
        for (int i = 1;i < tree.size(); ++i)
        {
            if (i%step == 0)
            {
                std::cout << '\n';
                step *= 2;
            }
            std::cout << "{ " << i << " : ";
            for (int j = 0;j < tree[i].axisY.size(); ++j)
            {
                std::cout << tree[i].axisY[j] << ", ";
            }
            std::cout << " } ";
        }
        std::cout << '\n';
    }

    void RegionTree::output_differ()
    {
        for (int i = 0;i < differ_axis_x.size(); ++i)
        {
            std:: cout << differ_axis_x[i] << ' ';
        }
        std::cout << '\n';
    }
}