//
// Created by art on 3/18/20.
//

#ifndef LAB9_BEACHLINE_H
#define LAB9_BEACHLINE_H

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
        Arc,
        Edge
    };

    struct TreeItem : std::enable_shared_from_this<TreeItem>
    {
        std::shared_ptr<TreeItem> parent, left, right;
        std::shared_ptr<Lokus> lokus;
        
        TreeItem(): parent(nullptr), left(nullptr), right(nullptr) {}
        void set_left(std::shared_ptr<TreeItem> new_left);
        void set_right(std::shared_ptr<TreeItem> new_right);
        void set_parent_from_item(std::shared_ptr<TreeItem> item);
    };

    class BeachLine
    {
        std::shared_ptr<TreeItem> root;
    };

}
#endif //LAB9_BEACHLINE_H
