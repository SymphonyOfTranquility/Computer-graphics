//
// Created by art on 3/18/20.
//

#include <fstream>
#include "Voroniy.h"

void vor::Voroniy::input_points(std::string file_name)
{
    std::ifstream file;
    file.open(file_name);
    file >> points_number;
    for (int i = 0; i < points_number; ++i)
    {
        std::shared_ptr<TPoint> p = std::make_shared<TPoint>();
        file >> p->x >> p->y;
        points.push_back(p);
    }
}
