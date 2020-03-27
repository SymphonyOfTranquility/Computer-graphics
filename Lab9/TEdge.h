//
// Created by art on 3/18/20.
//

#ifndef LAB9_TEDGE_H
#define LAB9_TEDGE_H

#include <cmath>
#include <memory>
#include "TPoint.h"
#include "Lokus.h"

namespace vor
{
    struct Lokus;

    struct TLine
    {
        double a, b, c;
        void set_vals_by_points(TPoint q, TPoint p)
        {
            a = p.y - q.y;
            b = q.x - p.x;
            c = p.x*q.y - q.x*p.y;
            if (std::abs(a) >= eps && a < 0)
                a = -a, b = -b, c = -c;
        }
        TLine(): a(0), b(0),c(0) {};
    };

    class TEdge
    {
    public:
        std::shared_ptr<TPoint> start, direction, end;
        std::shared_ptr<Lokus> left, right;
        TLine line;
        std::shared_ptr<TEdge> neighbour;

        TEdge(std::shared_ptr<TPoint> center_point, std::shared_ptr<Lokus> a, std::shared_ptr<Lokus> b);
    };
}
#endif //LAB9_TEDGE_H
