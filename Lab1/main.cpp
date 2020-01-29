#include "Polygon.h"

#include <iostream>
#include <zconf.h>

int main() {

   /* FILE* file = fopen("kkk.tete","wb+");

    fclose(file);*/

    polygon_space::Polygon polygon;
    polygon.input_from_file("../points.dat");
    polygon_space::TPoint point = {0.000, -1.0000};
    std::cout << polygon.check_point_in_polygon(point);

    return 0;
}
