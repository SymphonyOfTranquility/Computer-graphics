#include "Polygon.h"

#include <iostream>

int main()
{
    polygon_space::Polygon polygon;
    polygon.input_from_file("../points.dat");

    polygon_space::TPoint point = {-1, 1.};

    std::cout << polygon.check_point_in_polygon(point);

    return 0;
}
