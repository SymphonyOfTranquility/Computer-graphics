//
// Created by art on 3/18/20.
//

#ifndef LAB9_SWEEPEVENT_H
#define LAB9_SWEEPEVENT_H

#include "TPoint.h"
#include "BeachLineTree.h"

namespace vor
{
    enum class SweepLineType
    {
        None,
        NewPoint,
        EdgeIntersection
    };

    struct NewPointEvent
    {
        TPoint point;
    };

    struct EdgeIntersectionEvent
    {
        TPoint intersection;
        std::shared_ptr<TreeItem> arc;
        bool is_valid;
    };

    class SweepEvent
    {
    public:
        double y_axis;
        SweepLineType type;
        union
        {
            NewPointEvent new_point;
            EdgeIntersectionEvent edge_intersect;
        };
    };
}
#endif //LAB9_SWEEPEVENT_H
