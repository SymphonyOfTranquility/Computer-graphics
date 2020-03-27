//
// Created by art on 3/18/20.
//

#include "BeachLine.h"

namespace vor
{

    void TreeItem::set_left(std::shared_ptr<TreeItem> new_left)
    {
        left = new_left;
        new_left->parent = shared_from_this();
    }

    void TreeItem::set_right(std::shared_ptr<TreeItem> new_right)
    {
        right = new_right;
        new_right->parent = shared_from_this();
    }

    void TreeItem::set_parent_from_item(std::shared_ptr<TreeItem> item)
    {
        if (item == nullptr)
            return;
        if (item->parent == nullptr)
        {
            parent = nullptr;
            return;
        }

        if (item->parent->left == item)
            item->parent->set_left(shared_from_this());
        else
            item->parent->set_right(shared_from_this());
    }


}