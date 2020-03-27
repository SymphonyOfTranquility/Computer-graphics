//
// Created by art on 3/18/20.
//

#ifndef LAB9_LOKUS_H
#define LAB9_LOKUS_H

#include <vector>
#include "TEdge.h"
#include "TPoint.h"

namespace vor
{
    class TEdge;

    struct Lokus
    {
        std::shared_ptr<TPoint> point;
        std::vector<std::shared_ptr<TEdge> > edges;

        Lokus(std::shared_ptr<TPoint> point)
        {
            this->point = point;
            edges.clear();
        }
    };
}

#endif //LAB9_LOKUS_H
