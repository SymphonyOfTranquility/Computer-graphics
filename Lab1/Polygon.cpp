//
// Created by art on 1/29/20.
//

#include "Polygon.h"

#include <fstream>

Polygon::Polygon(std::string pass_to_input_file) {
    input_from_file(pass_to_input_file);
}

void Polygon::input_from_file(std::string pass_to_input_file) {
    std::ifstream file;
    file.open(pass_to_input_file);
    file >> size;
    points.clear();
    for (int i = 0;i < size; ++i)
    {
        TPoint point;
        file >> point.x >> point.y;
        points.push_back(point);
    }
}
