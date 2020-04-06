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
        int time;
        EventType type;
        std::shared_ptr<Lokus> lokus;
        std::vector<std::shared_ptr<Lokus> > to_delete;
        SweepEvent():time(0), lokus(nullptr), type(EventType::None) { to_delete.clear(); };
        SweepEvent(EventType type, std::shared_ptr<Lokus> lokus, int time):type(type), lokus(lokus), time(time) { to_delete.clear(); };
    };

    class Voroniy
    {
        std::vector<std::shared_ptr<TPoint> > points;
        std::vector<std::shared_ptr<Lokus> > lokuses;
        int points_number;
        std::shared_ptr<TPoint> get_the_most_bottom_circle_dot(std::shared_ptr<TPoint> z0,
                                                               std::shared_ptr<TPoint> z1,
                                                               std::shared_ptr<TPoint> z2);

        bool check_intersection_for_3_parabolas(std::shared_ptr<TPoint> z0,
                                                std::shared_ptr<TPoint> z1,

                                                std::shared_ptr<TPoint> z2);
    public:
        Voroniy(){points.clear(), lokuses.clear(), points_number = 0;}
        void input_points(std::string file_name);
        void fortune_algorithm();
        std::vector<std::shared_ptr<Lokus> > get_lokuses();
        void output();
    };
}


#endif //LAB9_VORONIY_H
