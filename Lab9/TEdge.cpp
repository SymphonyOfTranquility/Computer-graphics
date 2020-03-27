//
// Created by art on 3/27/20.
//


#include "TEdge.h"

namespace vor
{

    TEdge::TEdge(std::shared_ptr<TPoint> center_point, std::shared_ptr<Lokus> a, std::shared_ptr<Lokus> b)
    {
        this->start = nullptr;
        this->end = nullptr;
        this->left = a;
        this->right = b;
        this->neighbour = nullptr;
        direction = std::make_shared<TPoint>(b->point->y - a->point->y, a->point->x - b->point->x);
        if (center_point != nullptr)
            line.set_vals_by_points(*center_point, {center_point->x + direction->x, center_point->y + direction->y});
    }
}

