//
// Created by art on 3/18/20.
//

#ifndef LAB9_VORONIY_H
#define LAB9_VORONIY_H


#include <vector>
#include <memory>
#include "TPoint.h"
#include "Lokus.h"

namespace vor
{
    class Voroniy
    {
        std::vector<std::shared_ptr<TPoint> > points;
        std::vector<Lokus> lokuses;
        int points_number;
    public:
        Voroniy(){points.clear(), lokuses.clear(), points_number = 0;}
        void input_points(std::string file_name);
    };
}


#endif //LAB9_VORONIY_H
