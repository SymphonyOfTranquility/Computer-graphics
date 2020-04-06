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
                                    return a.time > b.time;
                                else
                                    return a.lokus->point->x > b.lokus->point->x;
                            }
                            else
                                return a.lokus->point->y < b.lokus->point->y;
                        };
        std::priority_queue<SweepEvent, std::vector<SweepEvent>, decltype(compare)> queue(compare);
        for (int i = 0;i < points_number; ++i)
        {//check_intersection_for_3_parabolas
            std::shared_ptr<Lokus> new_lokus = std::make_shared<Lokus>(points[i]);
            queue.push({EventType::NewLokus, new_lokus, 0});
            lokuses.push_back(new_lokus);
        }
        BeachLineTree tree;
        int time = 1;
        while (!queue.empty())
        {
            SweepEvent top = queue.top();
            queue.pop();
            std::cout << top.lokus->point->x << ' ' << top.lokus->point->y << '\n';
            if (top.type == EventType::DeleteLokus)
            {
                for (int i = 0;i < top.to_delete.size(); ++i)
                    std::cout << "\tDel " << top.to_delete[i]->point->x << ' ' << top.to_delete[i]->point->y << '\n';
            }
            std::cout << '\n';
            if (top.type == EventType::NewLokus)
                tree.add(top.lokus);
            else
                tree.del(top.to_delete, top.lokus->point->y);



            std::vector<std::shared_ptr<Lokus> > arcs = tree.get_can_cross();
            for (int i = 0;i < arcs.size() - 2 && arcs.size() >= 3; ++i)
            {
                if (arcs[i] != nullptr && arcs[i+1] != nullptr && arcs[i+2] != nullptr && arcs[i] != arcs[i+2])
                {
                    if ((std::abs(arcs[i]->point->x - arcs[i+1]->point->x) < eps &&
                         std::abs(arcs[i+1]->point->x - arcs[i+2]->point->x) < eps) ||
                        (std::abs(arcs[i]->point->y - arcs[i+1]->point->y) < eps &&
                         std::abs(arcs[i+1]->point->y - arcs[i+2]->point->y) < eps))
                        continue;

                    std::shared_ptr<TPoint> circle_dot = get_the_most_bottom_circle_dot(arcs[i]->point,
                                                                                        arcs[i+1]->point,
                                                                                        arcs[i+2]->point);
                    if (std::abs(circle_dot->y - top.lokus->point->y) < eps && top.type == EventType::DeleteLokus)
                        continue;
                    if (circle_dot->y > top.lokus->point->y)
                        continue;
                    if (!check_intersection_for_3_parabolas(arcs[i]->point, arcs[i+1]->point, arcs[i+2]->point))
                        continue;
                    std::shared_ptr<Lokus> lokus_for_del = std::make_shared<Lokus>(circle_dot);
                    SweepEvent new_event(EventType::DeleteLokus, lokus_for_del, time);
                    new_event.to_delete.push_back(arcs[i]);
                    new_event.to_delete.push_back(arcs[i+1]);
                    new_event.to_delete.push_back(arcs[i+2]);
                    queue.push(new_event);
                    std::cout << "Add to queue to del " << circle_dot->x << ' ' << circle_dot->y << ' ';
                    std::cout << "Lokus is " << arcs[i+1]->point->x << ' ' << arcs[i+1]->point->y << '\n';
                    std::cout << "  Near is " << arcs[i]->point->x << ' ' << arcs[i]->point->y << '\n';
                    std::cout << "  Near is " << arcs[i+2]->point->x << ' ' << arcs[i+2]->point->y << '\n';
                }
            }
            std::cout << "----------------------------------------------------\n";
            output();
            ++time;
        }
    }

    std::vector<std::shared_ptr<Lokus> > Voroniy::get_lokuses()
    {
        return lokuses;
    }

    std::shared_ptr<TPoint>
    Voroniy::get_the_most_bottom_circle_dot(std::shared_ptr<TPoint> z0, std::shared_ptr<TPoint> z1,
                                            std::shared_ptr<TPoint> z2)
    {
        double a1 = -2*(z0->x - z1->x), b1 = -2*(z0->y - z1->y),
               c1 = z0->x*z0->x - z1->x*z1->x + z0->y*z0->y - z1->y*z1->y;
        double a2 = -2*(z0->x - z2->x), b2 = -2*(z0->y - z2->y),
               c2 = z0->x*z0->x - z2->x*z2->x + z0->y*z0->y - z2->y*z2->y;
        double p = (c2*b1 - c1*b2)/(b2*a1 - a2*b1), q = (a2*c1 - c2*a1)/(b2*a1 - b1*a2),
               r = sqrt((z1->x - p)*(z1->x - p) + (z1->y - q)*(z1->y - q));
        return std::make_shared<TPoint>(p, q-r);
    }

    void Voroniy::output()
    {
        for (int i = 0;i < lokuses.size(); ++i)
        {
            std::cout << "Lokus: " << lokuses[i]->point->x << ", " << lokuses[i]->point->y << '\n';
            for (int j = 0;j < lokuses[i]->edges.size(); ++j)
            {
                if (lokuses[i]->edges[j]->start != nullptr)
                    std::cout << "\tStart " << lokuses[i]->edges[j]->start->x << ' ' << lokuses[i]->edges[j]->start->y << '\n';
                else
                    std::cout << "\tStart null\t";

                if (lokuses[i]->edges[j]->end != nullptr)
                    std::cout << "\tEnd " << lokuses[i]->edges[j]->end->x << ' ' << lokuses[i]->edges[j]->end->y << '\n';
                else
                    std::cout << "\tEnd null\t";

                std::cout << "\tLine: ";
                std::cout << lokuses[i]->edges[j]->line.a << ' ' << lokuses[i]->edges[j]->line.b << ' ' << lokuses[i]->edges[j]->line.c << '\n';
                std::cout << '\n';
            }
        }
    }

    bool Voroniy::check_intersection_for_3_parabolas(std::shared_ptr<TPoint> z0, std::shared_ptr<TPoint> z1,
                                                     std::shared_ptr<TPoint> z2)
    {
        std::shared_ptr<TPoint> bottom_dot = z0, high_dot1 = z1, high_dot2 = z2;
        if (bottom_dot->y > z1->y)
            std::swap(high_dot1, bottom_dot);
        if (bottom_dot->y > z2->y)
            std::swap(bottom_dot, high_dot2);

        if (std::abs(high_dot1->x - high_dot2->x) < eps)
        {
            if (high_dot1->y < high_dot2->y)
                std::swap(high_dot1, high_dot2);
        } else if (high_dot1->x < high_dot2->x)
            std::swap(high_dot2, high_dot1);

        double p1 = (high_dot1->y - bottom_dot->y)/2.0, h1 = high_dot1->x, k1 = (high_dot1->y + bottom_dot->y)/2.0;
        double p2 = (high_dot2->y - bottom_dot->y)/2.0, h2 = high_dot2->x, k2 = (high_dot2->y + bottom_dot->y)/2.0;

        double y1 = std::pow(bottom_dot->x - h1, 2)/(4*p1) + k1;
        double y2 = std::pow(bottom_dot->x - h2, 2)/(4*p2) + k2;

        if (std::abs(y1 - y2) < eps)
        {

        }
        return false;
    }
}