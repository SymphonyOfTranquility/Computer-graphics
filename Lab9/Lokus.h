//
// Created by art on 3/18/20.
//

#ifndef LAB9_LOKUS_H
#define LAB9_LOKUS_H

#include "TEdge.h"
#include "TPoint.h"

namespace vor
{

    class Lokus
    {
        std::shared_ptr<TPoint> point;
        std::shared_ptr<Edge> edge;

        Lokus(std::shared_ptr<TPoint> point)
        {
            this->point = point;
            edge = nullptr;
        }
    };
}

#endif //LAB9_LOKUS_H
