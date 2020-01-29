//
// Created by art on 1/29/20.
//

#ifndef LAB1_POLYGON_H
#define LAB1_POLYGON_H

#include <vector>
#include <string>

struct TPoint {
    double x, y;
};


class Polygon {
    std::vector<TPoint> points;
    int size;
public:
    Polygon() = default;
    Polygon(std::string pass_to_file);
    void input_from_file(std::string pass_to_input_file);
};


#endif //LAB1_POLYGON_H
