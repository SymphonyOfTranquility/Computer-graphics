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
    enum class EventType
    {
        None,
        NewLokus,
        DeleteLokus
    };

    struct SweepEvent
    {
        EventType type;
        std::shared_ptr<Lokus> lokus;
        std::shared_ptr<Lokus> to_delete;
        SweepEvent(): lokus(nullptr), to_delete(nullptr), type(EventType::None) {};
        SweepEvent(EventType type, std::shared_ptr<Lokus> lokus):type(type), lokus(lokus), to_delete(nullptr) {};
    };

    class Voroniy
    {
        std::vector<std::shared_ptr<TPoint> > points;
        std::vector<std::shared_ptr<Lokus> > lokuses;
        int points_number;
    public:
        Voroniy(){points.clear(), lokuses.clear(), points_number = 0;}
        void input_points(std::string file_name);
        void fortune_algorithm();
        std::vector<std::shared_ptr<Lokus> > get_lokuses();
    };
}


#endif //LAB9_VORONIY_H
