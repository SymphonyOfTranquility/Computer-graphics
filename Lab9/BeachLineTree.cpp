//
// Created by art on 3/18/20.
//

#include <cassert>
#include "BeachLineTree.h"

#define assertm(exp, msg) assert(((void)msg, exp))

namespace vor
{

    void TreeItem::set_arc(std::shared_ptr<Lokus> lokus)
    {
        type = ItemType::Arc;
        this->lokus = lokus;
    }

    void TreeItem::create_edge(std::shared_ptr<TEdge> edge, EdgeType edge_type)
    {
        type = ItemType::Edge;
        this->edges = std::vector<std::shared_ptr<BeachLineEdge> >();
        if (edge != nullptr)
        {
            std::shared_ptr<BeachLineEdge> temp = std::make_shared<BeachLineEdge>(edge, edge_type);
            this->edges.push_back(temp);
        }
    }

    void BeachLineTree::add(std::shared_ptr<Lokus> lokus)
    {
        if (root == nullptr)
        {
            root = std::make_shared<TreeItem>();
            root->set_arc(lokus);
            return;
        }
        root = rec_add(root, lokus);
    }

    std::shared_ptr<TreeItem> BeachLineTree::rec_add(std::shared_ptr<TreeItem> current_root,
                                                     std::shared_ptr<Lokus> lokus)
    {
        if (current_root->type == ItemType::Arc)
        {
            std::shared_ptr<TreeItem> parent = current_root->parent;
            if (parent == nullptr)
            {
                parent = std::make_shared<TreeItem>();
                parent->create_edge(nullptr, EdgeType::NONE);
                parent->children.push_back(current_root);
                current_root->parent = parent;
            }
            std::vector<std::shared_ptr<BeachLineEdge> > new_edge = create_edge(current_root->lokus, lokus);
            if (parent->edges.empty())
            {
                for (int i = 0;i < new_edge.size(); ++i)
                    parent->edges.push_back(new_edge[i]);;
            } else
            {
                int i = 0;
                while (i < parent->children.size())
                {
                    if (parent->children[i] == current_root)
                        break;
                    ++i;
                }
                parent->edges.insert(parent->edges.begin() + i, new_edge.begin(), new_edge.end());
            }
            int root_pos = 0;
            while (root_pos < parent->children.size())
            {
                if (parent->children[root_pos] == current_root)
                    break;
                ++root_pos;
            }
            root_pos++;
            if (new_edge.size() == 2)
            {
                std::shared_ptr<TreeItem> new_center_leaf = std::make_shared<TreeItem>();
                new_center_leaf->set_arc(lokus);
                std::shared_ptr<TreeItem> new_right_leaf = std::make_shared<TreeItem>();
                new_right_leaf->set_arc(current_root->lokus);
                parent->children.insert(parent->children.begin() + root_pos, new_right_leaf);
                parent->children.insert(parent->children.begin() + root_pos, new_center_leaf);
                new_right_leaf->parent = parent;
                new_center_leaf->parent = parent;

                if (current_root->right != nullptr)
                    current_root->right->left = new_right_leaf;

                new_right_leaf->right = current_root->right;
                new_center_leaf->right = new_right_leaf;
                current_root->right = new_center_leaf;
                new_center_leaf->left = current_root;
                new_right_leaf->left = new_center_leaf;
            }
            else
            {
                std::shared_ptr<TreeItem> new_leaf = std::make_shared<TreeItem>();
                new_leaf->set_arc(lokus);
                parent->children.insert(parent->children.begin() + root_pos, new_leaf);
                new_leaf->parent = parent;

                if (current_root->right != nullptr)
                    current_root->right->left = new_leaf;

                new_leaf->right = current_root->right;
                current_root->right = new_leaf;
                new_leaf->left = current_root;
            }
            return parent;
        } else
        {
            bool update = false;
            for (int i = 0;i < current_root->edges.size(); ++i)
            {
                EdgeType position = get_position_depend_on_edge(current_root->edges[i], lokus);
                if (position == EdgeType::LEFT)         //left side
                {
                    current_root = rec_add(current_root->children[i], lokus);
                    update = true;
                    break;
                }
                else if (position == EdgeType::VERTICAL)//lying on it
                {
                    if (current_root->edges[i]->type == EdgeType::RIGHT)
                        current_root = rec_add(current_root->children[i], lokus);
                    else
                        current_root = rec_add(current_root->children[i+1], lokus);
                    update = true;
                    break;
                }
            }
            if (!update)
                current_root = rec_add(current_root->children.back(), lokus);
            if (current_root->children.size() > 3)
            {
                int middle = int(current_root->edges.size()/2);
                std::shared_ptr<TreeItem> parent = current_root->parent;
                if (parent == nullptr)
                {
                    parent = std::make_shared<TreeItem>();
                    parent->create_edge(nullptr, EdgeType::NONE);
                    parent->children.push_back(current_root);
                    current_root->parent = parent;
                }
                int root_pos = 0;
                while (root_pos < parent->children.size())
                {
                    if (parent->children[root_pos] == current_root)
                        break;
                    ++root_pos;
                }
                std::shared_ptr<TreeItem> sibling = std::make_shared<TreeItem>();
                sibling->parent = parent;
                parent->children.insert(parent->children.begin() + root_pos + 1, sibling);
                parent->edges.insert(parent->edges.begin() + root_pos, current_root->edges[middle]);

                sibling->create_edge(nullptr, EdgeType::NONE);
                for (int i = middle+1;i < current_root->children.size(); ++i)
                {
                    if (i+1 < current_root->children.size())
                        sibling->edges.push_back(current_root->edges[i]);
                    sibling->children.push_back(current_root->children[i]);
                    current_root->children[i]->parent = sibling;
                }

                while (current_root->children.size() > middle + 1)
                {
                    current_root->edges.pop_back();
                    current_root->children.pop_back();
                }
                return parent;
            }
            else
            {
                if (current_root->parent == nullptr)
                    return current_root;
                else
                    return current_root->parent;
            }
        }
    }

    std::vector< std::shared_ptr<BeachLineEdge> > BeachLineTree::create_edge(const std::shared_ptr<Lokus>& root_lokus,
                                                                             const std::shared_ptr<Lokus>& current_lokus)
    {
        std::vector<std::shared_ptr<BeachLineEdge> > edge_of_beach;
        std::shared_ptr<TPoint> intersect_point;
        if (std::abs(root_lokus->point->y - current_lokus->point->y) >= eps)
        {
            intersect_point = get_parabola_line_intersection(root_lokus->point, current_lokus->point);
            std::shared_ptr<TEdge> edge = std::make_shared<TEdge>(intersect_point, root_lokus, current_lokus);
            edge_of_beach.push_back(std::make_shared<BeachLineEdge>(edge, EdgeType::LEFT));
            edge_of_beach.push_back(std::make_shared<BeachLineEdge>(edge, EdgeType::RIGHT));
        }
        else
        {
            intersect_point = std::make_shared<TPoint>((root_lokus->point->x + current_lokus->point->x)/2,
                                                       1/eps*std::abs(root_lokus->point->y));
            std::shared_ptr<TEdge> edge = std::make_shared<TEdge>(intersect_point, root_lokus, current_lokus);
            edge_of_beach.push_back(std::make_shared<BeachLineEdge>(edge, EdgeType::VERTICAL));
        }
        root_lokus->edges.push_back(edge_of_beach[0]->edge);
        current_lokus->edges.push_back(edge_of_beach[0]->edge);
        return edge_of_beach;
    }

    std::shared_ptr<TPoint>
    BeachLineTree::get_parabola_line_intersection(std::shared_ptr<TPoint> focus, std::shared_ptr<TPoint> lokus)
    {
        std::shared_ptr<TPoint> point = std::make_shared<TPoint>();
        point->x = lokus->x;
        point->y = ((lokus->x - focus->x)*(lokus->x - focus->x) + focus->y*focus->y - lokus->y*lokus->y)/
                    (2*(focus->y - lokus->y));
        return point;
    }

    void BeachLineTree::del(std::vector<std::shared_ptr<Lokus> > lokuses, double sweep_line)
    {
        rec_del(root, lokuses, sweep_line);
    }

    EdgeType BeachLineTree::get_position_depend_on_edge(std::shared_ptr<BeachLineEdge> edge, std::shared_ptr<Lokus> lokus)
    {
        std::vector<double> x_point = get_parabolas_intersection_on_axis_X(edge, lokus->point->y);

        if (edge->type == EdgeType::VERTICAL)
        {
            if (lokus->point->x > x_point[0])
                return EdgeType::RIGHT;
            else
                return EdgeType::LEFT;
        } else
        {
            double x1 = x_point[0], x2 = x_point[1];
            if (edge->edge->start != nullptr)
            {
                if (edge->type == EdgeType::LEFT)
                    x2 = std::min(x2, edge->edge->start->x);
                else
                    x1 = std::max(x1, edge->edge->start->x);
            }
            if (lokus->point->x < x1)
                return EdgeType::LEFT;
            else if (x2 < lokus->point->x)
                return EdgeType::RIGHT;
            else
                return EdgeType::VERTICAL;
        }
    }

    void BeachLineTree::rec_del(std::shared_ptr<TreeItem> current_root, std::vector<std::shared_ptr<Lokus> > lokuses, double sweep_line)
    {
        if (current_root->type == ItemType::Arc)
        {
            if (current_root->lokus == lokuses[0] &&
                current_root->right != nullptr && current_root->right->lokus == lokuses[1] &&
                current_root->right->right != nullptr && current_root->right->right->lokus == lokuses[2])
                current_root = current_root->right;

            if (current_root->lokus == lokuses[2] &&
                current_root->left != nullptr && current_root->left->lokus == lokuses[1] &&
                current_root->left->left != nullptr && current_root->left->left->lokus == lokuses[0])

                current_root = current_root->left;

            if (!(current_root->lokus == lokuses[1] &&
                  current_root->right != nullptr && current_root->right->lokus == lokuses[2] &&
                  current_root->left != nullptr && current_root->left->lokus == lokuses[0]))
                return;

            if (current_root == nullptr || current_root->lokus != lokuses[1])
                return;

            std::shared_ptr<Lokus> lokus = lokuses[1];
            std::shared_ptr<TreeItem> parent = current_root->parent;
            std::pair<std::shared_ptr<TreeItem>, std::vector<int> > near_left_struct = get_near(current_root, 1);       //move on the left
            std::pair<std::shared_ptr<TreeItem>, std::vector<int> > near_right_struct = get_near(current_root, -1);     //move on the right
            int height_left = near_left_struct.second[0], height_right = near_right_struct.second[0];
            int pos_left = near_left_struct.second[1], pos_right = near_right_struct.second[1];
            std::shared_ptr<TreeItem> near_left = near_left_struct.first;
            std::shared_ptr<TreeItem> near_right = near_right_struct.first;
            std::shared_ptr<Lokus> near_left_lokus = current_root->left->lokus;
            std::shared_ptr<Lokus> near_right_lokus = current_root->right->lokus;
            std::shared_ptr<TPoint> intersect_point = get_parabolas_intersection(near_left_lokus, near_right_lokus,
                                                                                 lokus, sweep_line);
            if (near_left->edges[pos_left]->edge->start == nullptr)
                near_left->edges[pos_left]->edge->start = intersect_point;
            else
                near_left->edges[pos_left]->edge->end = intersect_point;

            if (near_right->edges[pos_right]->edge->start == nullptr)
                near_right->edges[pos_right]->edge->start = intersect_point;
            else
                near_right->edges[pos_right]->edge->end = intersect_point;
            std::vector<std::shared_ptr<BeachLineEdge> > new_edge = create_edge(near_left_lokus, near_right_lokus);
            new_edge[0]->edge->start = intersect_point;
            std::shared_ptr<TreeItem> top_item;
            int top_pos;
            if (height_left > height_right)
            {
                top_item = near_left;
                top_pos = pos_left;
            } else
            {
                top_item = near_right;
                top_pos = pos_right;
            }

            if (new_edge.size() == 1)
            {
                insert_edge(top_item, top_pos, new_edge[0]);
            } else
            {
                std::vector<double> x_point = get_parabolas_intersection_on_axis_X(new_edge[0], lokus->point->y - 100);
                std::vector<double> y_point = get_parabolas_intersection_on_axis_Y(new_edge[0], x_point);
                if (y_point[0] < intersect_point->y)
                    insert_edge(top_item, top_pos, new_edge[0]);
                else
                    insert_edge(top_item, top_pos, new_edge[1]);
            }
            if (height_left > height_right)
                del_edge(near_right, current_root, pos_right);
            else
                del_edge(near_left, current_root, pos_left);

            std::shared_ptr<TreeItem> left_friend = current_root->left;
            std::shared_ptr<TreeItem> right_friend = current_root->right;
            if (left_friend != nullptr)
                left_friend->right = right_friend;
            if (right_friend != nullptr)
                right_friend->left = left_friend;
        } else
        {
            bool update = false;
            for (int i = 0;i < current_root->edges.size(); ++i)
            {
                EdgeType position = get_position_depend_on_edge(current_root->edges[i], lokuses[1]);
                if (position == EdgeType::LEFT)         //left side
                {
                    rec_del(current_root->children[i], lokuses, sweep_line);
                    update = true;
                    break;
                }
                else if (position == EdgeType::VERTICAL)//lying on it
                {
                    if (current_root->edges[i]->type == EdgeType::RIGHT)
                        rec_del(current_root->children[i], lokuses, sweep_line);
                    else
                        rec_del(current_root->children[i+1], lokuses, sweep_line);
                    update = true;
                    break;
                }
            }
            if (!update)
                rec_del(current_root->children.back(), lokuses, sweep_line);
        }
    }

    std::vector<double> BeachLineTree::get_parabolas_intersection_on_axis_X(std::shared_ptr<BeachLineEdge> edge,
                                                                         double sweep_value)
    {
        std::vector<double> ans(0);
        if (edge->type == EdgeType::VERTICAL)
        {
            double x_point = - edge->edge->line.c/edge->edge->line.a;
            ans.push_back(x_point);
        } else
        {
            std::shared_ptr<TPoint> point_a = edge->edge->right->point, point_b = edge->edge->left->point;

            double p1 = (point_a->y - sweep_value) / 2, k1 = (point_a->y + sweep_value) / 2, h1 = point_a->x,
                    p2 = (point_b->y - sweep_value) / 2, k2 = (point_b->y + sweep_value) / 2, h2 = point_b->x;

            double a = p2 - p1, b = (p2 * h1 - p1 * h2), c = p2 * h1 * h1 - p1 * h2 * h2 + 4 * p1 * p2 * (k1 - k2);
            double x1 = (b - sqrt(b * b - a * c)) / a, x2 = (b + sqrt(b * b - a * c)) / a;
            if (x1 > x2)
                std::swap(x1, x2);
            ans.push_back(x1);
            ans.push_back(x2);
        }
        return ans;
    }

    std::vector<double> BeachLineTree::get_parabolas_intersection_on_axis_Y(std::shared_ptr<BeachLineEdge> edge,
                                                                            std::vector<double> x_points)
    {
        TLine line = edge->edge->line;
        std::vector<double> y_points;
        y_points.push_back((-line.a*x_points[0]-line.c)/line.b);
        y_points.push_back((-line.a*x_points[1]-line.c)/line.b);
        return y_points;
    }

    std::shared_ptr<TPoint> BeachLineTree::get_parabolas_intersection(std::shared_ptr<Lokus> near_left_lokus,
                                                                      std::shared_ptr<Lokus> near_right_lokus,
                                                                      std::shared_ptr<Lokus> middle_lokus,
                                                                      double sweep_line)
    {
        std::vector<TPoint> intersect_left_right = get_2_parabolas_intersection(near_left_lokus, near_right_lokus, sweep_line);
        std::vector<TPoint> intersect_middle_right = get_2_parabolas_intersection(middle_lokus, near_right_lokus, sweep_line);
        if ((std::abs(intersect_left_right[0].x - intersect_middle_right[0].x) < eps &&
            std::abs(intersect_left_right[0].y - intersect_middle_right[0].y) < eps) ||
            (std::abs(intersect_left_right[0].x - intersect_middle_right[1].x) < eps &&
            std::abs(intersect_left_right[0].y - intersect_middle_right[1].y) < eps))
            return std::make_shared<TPoint>(intersect_left_right[0]);
        else
            return std::make_shared<TPoint>(intersect_left_right[1]);
    }

    std::vector<TPoint> BeachLineTree::get_2_parabolas_intersection(std::shared_ptr<Lokus> lokus_a,
                                                                    std::shared_ptr<Lokus> lokus_b,
                                                                    double sweep_line)
    {
        std::vector<TPoint> ans(0);
        std::shared_ptr<TPoint> point_a = lokus_a->point, point_b = lokus_b->point;

        double p1 = (point_a->y - sweep_line) / 2, k1 = (point_a->y + sweep_line) / 2, h1 = point_a->x,
               p2 = (point_b->y - sweep_line) / 2, k2 = (point_b->y + sweep_line) / 2, h2 = point_b->x;

        double a = p2 - p1, b = (p2 * h1 - p1 * h2), c = p2 * h1 * h1 - p1 * h2 * h2 + 4 * p1 * p2 * (k1 - k2);
        double x;
        if (std::abs(a) > eps)
        {
            x = (b - sqrt(b * b - a * c)) / a;
            ans.emplace_back(x, 0);
            x = (b + sqrt(b * b - a * c)) / a;
            ans.emplace_back(x, 0);
        } else
        {
            x = c/(2*b);
            ans.emplace_back(x, 0);
        }
        for (auto & item : ans)
            item.y = std::pow(item.x - h1, 2) / (4.0 * p1) + k1;
        return ans;
    }

    void BeachLineTree::del_edge(std::shared_ptr<TreeItem> parent, std::shared_ptr<TreeItem> child, int pos_to_del)
    {
        int child_pos = 0;
        if (parent->children[pos_to_del] == child)
            child_pos = pos_to_del;
        else if (pos_to_del > 0 && parent->children[pos_to_del - 1] == child)
            child_pos = pos_to_del - 1;
        else if (pos_to_del + 1 < parent->children.size() && parent->children[pos_to_del + 1] == child)
            child_pos = pos_to_del + 1;
        for (int i = pos_to_del;i < parent->edges.size()-1; ++i)
            parent->edges[i] = parent->edges[i+1];
        parent->edges.pop_back();

        for (int i = child_pos; i < parent->children.size() - 1; ++i)
            parent->children[i] = parent->children[i+1];
        parent->children.pop_back();
        if (parent->edges.empty())
        {
            std::shared_ptr<TreeItem> grandpa = parent->parent;
            std::shared_ptr<TreeItem> last_child = parent->children[0];
            assertm(grandpa != nullptr, "grandpa is null");
            for (int i = 0;i < grandpa->children.size(); ++i)
                if (grandpa->children[i] == parent)
                {
                    grandpa->children[i] = last_child;
                    last_child->parent = grandpa;
                    break;
                }
            parent->parent = parent->right = parent->left = nullptr;
            parent->children.clear();
            parent->edges.clear();
            parent->type = ItemType::None;
        }
    }

    void BeachLineTree::insert_edge(std::shared_ptr<TreeItem> parent, int pos, std::shared_ptr<BeachLineEdge> edge)
    {
        parent->edges[pos] = edge;
    }

    std::pair<std::shared_ptr<TreeItem>, std::vector<int> >
    BeachLineTree::get_near(std::shared_ptr<TreeItem> current_root, int val)
    {
        assertm(current_root->parent != nullptr, "get near parent of root is null");
        std::shared_ptr<TreeItem> parent = current_root->parent;
        int root_pos = -1;
        for (int i = 0;i < parent->children.size(); ++i)
        {
            if (parent->children[i] == current_root)
            {
                root_pos = i;
                break;
            }
        }
        std::pair<std::shared_ptr<TreeItem>, std::vector<int> > ans;
        if (root_pos != int(val < 0)*(parent->children.size()-1))
        {
            ans.first = parent;
            ans.second.push_back(1);
            ans.second.push_back(root_pos-(val > 0));
        } else
        {
            ans = get_near(parent, val);
            ans.second[0]++;
        }
        return ans;
    }

    std::vector<std::shared_ptr<Lokus> > BeachLineTree::get(std::shared_ptr<Lokus> lokus)
    {
        return rec_get(root, lokus);
    }

    std::vector<std::shared_ptr<Lokus> > BeachLineTree::rec_get(std::shared_ptr<TreeItem> current_root, std::shared_ptr<Lokus> lokus)
    {
        std::vector<std::shared_ptr<Lokus> > arcs;
        if (current_root->type == ItemType::Arc)
        {
            if (lokus != current_root->lokus)
            {
                for (int i = 0;i < 5; ++i)
                    arcs.push_back(nullptr);
                return arcs;
            }
            if (current_root->left == nullptr)
            {
                arcs.push_back(nullptr);
                arcs.push_back(nullptr);
            }
            else
            {
                if (current_root->left->left != nullptr)
                    arcs.push_back(current_root->left->left->lokus);
                else
                    arcs.push_back(nullptr);
                arcs.push_back(current_root->left->lokus);
            }
            arcs.push_back(current_root->lokus);
            if (current_root->right == nullptr)
            {
                arcs.push_back(nullptr);
                arcs.push_back(nullptr);
            } else
            {
                arcs.push_back(current_root->right->lokus);
                if (current_root->right->right != nullptr)
                    arcs.push_back(current_root->right->right->lokus);
                else
                    arcs.push_back(nullptr);
            }
        } else
        {
            bool update = false;
            for (int i = 0;i < current_root->edges.size(); ++i)
            {
                EdgeType position = get_position_depend_on_edge(current_root->edges[i], lokus);
                if (position == EdgeType::LEFT)         //left side
                {
                    arcs = rec_get(current_root->children[i], lokus);
                    update = true;
                    break;
                }
                else if (position == EdgeType::VERTICAL)//lying on it
                {
                    if (current_root->edges[i]->type == EdgeType::RIGHT)
                        arcs = rec_get(current_root->children[i], lokus);
                    else
                        arcs = rec_get(current_root->children[i + 1], lokus);
                    update = true;
                    break;
                }
            }
            if (!update)
                arcs = rec_get(current_root->children.back(), lokus);
        }
        return arcs;
    }
}