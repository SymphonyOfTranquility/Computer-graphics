//
// Created by art on 3/18/20.
//

#ifndef LAB9_TPOINT_H
#define LAB9_TPOINT_H

namespace vor
{
    const double eps = 0.000001;
    struct TPoint
    {
        double x, y;

        TPoint() : x(0), y(0) {};

        TPoint(double x, double y) : x(x), y(y) {};
    };

}

#endif //LAB9_TPOINT_H
