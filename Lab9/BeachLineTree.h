//
// Created by art on 3/18/20.
//

#ifndef LAB9_BEACHLINETREE_H
#define LAB9_BEACHLINETREE_H

#include <memory>
#include "TPoint.h"
#include "TEdge.h"
#include "TArc.h"
#include "Lokus.h"

namespace vor
{
    enum class ItemType
    {
        None,
        Edge,
        Arc
    };

    enum class EdgeType
    {
        NONE,
        LEFT,
        RIGHT,
        VERTICAL,
    };

    struct BeachLineEdge
    {
        std::shared_ptr<TEdge> edge;
        EdgeType type;
        BeachLineEdge() : edge(nullptr), type(EdgeType::NONE) {};
        BeachLineEdge(std::shared_ptr<TEdge> edge, EdgeType type) : edge(edge), type(type) {}
    };

    class TreeItem : std::enable_shared_from_this<TreeItem>
    {
    public:
        std::shared_ptr<TreeItem> parent;
        std::vector<std::shared_ptr<TreeItem> > children;
        std::shared_ptr<TreeItem> left, right;
        ItemType type;
        std::shared_ptr<Lokus> lokus;
        std::vector<std::shared_ptr<BeachLineEdge> > edges;

        TreeItem(): parent(nullptr), lokus(nullptr), type(ItemType::None), left(nullptr), right(nullptr)
        { children.clear(); }
        void set_arc(std::shared_ptr<Lokus> lokus);
        void create_edge(std::shared_ptr<TEdge> edge, EdgeType edge_type);
    };

    class BeachLineTree
    {
        std::shared_ptr<TreeItem> root;

        std::vector< std::shared_ptr<BeachLineEdge> > create_edge(const std::shared_ptr<Lokus>& root_lokus,
                                                                  const std::shared_ptr<Lokus>& current_lokus);

        EdgeType get_position_depend_on_edge(std::shared_ptr<BeachLineEdge> edge, std::shared_ptr<Lokus> lokus);

        std::shared_ptr<TreeItem> rec_add(std::shared_ptr<TreeItem> current_root, std::shared_ptr<Lokus> lokus);

        void del_edge(std::shared_ptr<TreeItem> parent, std::shared_ptr<TreeItem> child_to_del, int pos_to_del);
        void insert_edge(std::shared_ptr<TreeItem> parent, int pos, std::shared_ptr<BeachLineEdge> edge);

        void rec_del(std::shared_ptr<TreeItem> current_root, std::vector<std::shared_ptr<Lokus> > lokuses, double sweep_line);

        std::shared_ptr<TPoint> get_parabola_line_intersection(std::shared_ptr<TPoint> focus, std::shared_ptr<TPoint> lokus);

        std::vector<double> get_parabolas_intersection_on_axis_X(std::shared_ptr<BeachLineEdge> edge, double sweep_value);

        std::vector<double> get_parabolas_intersection_on_axis_Y(std::shared_ptr<BeachLineEdge> edge, std::vector<double> x_points);

        std::pair<std::shared_ptr<TreeItem>, std::vector<int> > get_near(std::shared_ptr<TreeItem> current_root, int val);

        std::vector<TPoint> get_2_parabolas_intersection(std::shared_ptr<Lokus> a, std::shared_ptr<Lokus> b,
                                                         double sweep_line);

        std::shared_ptr<TPoint> get_parabolas_intersection(std::shared_ptr<Lokus> near_left_lokus,
                                                           std::shared_ptr<Lokus> near_right_lokus,
                                                           std::shared_ptr<Lokus> middle_lokus,
                                                           double sweep_line);

        std::vector<std::shared_ptr<Lokus> > rec_get(std::shared_ptr<TreeItem> current_root, std::shared_ptr<Lokus> lokus);

    public:
        BeachLineTree():root(nullptr){};
        void add(std::shared_ptr<Lokus> lokus);
        void del(std::vector<std::shared_ptr<Lokus> > lokuses, double sweep_line);
        std::vector<std::shared_ptr<Lokus> > get(std::shared_ptr<Lokus> lokus);
    };

}
#endif //LAB9_BEACHLINETREE_H
