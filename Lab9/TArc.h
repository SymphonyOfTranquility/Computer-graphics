//
// Created by art on 3/18/20.
//

#ifndef LAB9_TARC_H
#define LAB9_TARC_H

#include <memory>
#include "TPoint.h"

namespace vor
{
    class SweepEvent;
    class TArc
    {
    public:
        std::shared_ptr<TPoint> focus;
        std::shared_ptr<SweepEvent> line;
        TArc(): focus(nullptr), line(nullptr){};
        TArc(std::shared_ptr<TPoint> focus, std::shared_ptr<SweepEvent> line): focus(focus), line(line) {};
    };

}

#endif //LAB9_TARC_H
