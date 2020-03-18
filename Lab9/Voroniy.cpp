//
// Created by art on 3/18/20.
//

#include <fstream>
#include "Voroniy.h"

namespace vor
{

    void Voroniy::input_points(std::string file_name)
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

    void Voroniy::fortune_algorithm()
    {
        lokuses.clear();
        for (int i = 0;i < points_number; ++i)
        {

        }
    }

    std::vector<Lokus> Voroniy::get_lokuses()
    {
        return lokuses;
    }
}