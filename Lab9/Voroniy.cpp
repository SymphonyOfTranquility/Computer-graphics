//
// Created by art on 3/18/20.
//

#include <fstream>
#include <queue>
#include <iostream>
#include "Voroniy.h"
#include "BeachLineTree.h"

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
        auto compare = [](const SweepEvent &a, const SweepEvent &b) -> bool
                        {
                            if (std::abs(a.lokus->point->y - b.lokus->point->y) < eps)
                            {
                                if (std::abs(a.lokus->point->x - b.lokus->point->x) < eps)
                                    return a.type == EventType::DeleteLokus;
                                else
                                    return a.lokus->point->x > b.lokus->point->x;
                            }
                            else
                                return a.lokus->point->y < b.lokus->point->y;
                        };
        std::priority_queue<SweepEvent, std::vector<SweepEvent>, decltype(compare)> queue(compare);
        for (int i = 0;i < points_number; ++i)
        {
            std::shared_ptr<Lokus> new_lokus = std::make_shared<Lokus>(points[i]);
            queue.push({EventType::NewLokus, new_lokus});
            lokuses.push_back(new_lokus);
        }
        BeachLineTree tree;
        while (!queue.empty())
        {
            SweepEvent top = queue.top();
            if (top.type == EventType::NewLokus)
            {
                tree.add(top.lokus);
                std::vector<std::shared_ptr<Lokus> > arcs = tree.get(top.lokus);
                for (int i = 0;i < arcs.size() - 2; ++i)
                {
                    if (arcs[i] != nullptr && arcs[i+1] != nullptr && arcs[i+2] != nullptr && arcs[i] != arcs[i+2])
                    {
                        std::shared_ptr<TPoint> circle_dot = get_the_most_bottom_circle_dot(arcs[i]->point,
                                                                                            arcs[i+1]->point,
                                                                                            arcs[i+2]->point);
                        std::shared_ptr<Lokus> lokus_for_del = std::make_shared<Lokus>(circle_dot);
                        SweepEvent new_event(EventType::DeleteLokus, lokus_for_del);
                        new_event.to_delete = arcs[i+1];
                        queue.push(new_event);
                    }
                }
            }
            else
                tree.del(top.to_delete, top.lokus->point->y);
        }
    }

    std::vector<std::shared_ptr<Lokus> > Voroniy::get_lokuses()
    {
        return lokuses;
    }
}