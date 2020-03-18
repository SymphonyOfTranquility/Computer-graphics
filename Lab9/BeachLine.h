//
// Created by art on 3/18/20.
//

#ifndef LAB9_BEACHLINE_H
#define LAB9_BEACHLINE_H

#include <memory>
#include "TPoint.h"
#include "TEdge.h"
#include "TArc.h"

namespace vor
{
    enum class ItemType
    {
        None,
        Arc,
        Edge
    };

    struct BeachLineItem : std::enable_shared_from_this<BeachLineItem>
    {
        ItemType type;
        union
        {
            std::shared_ptr<TArc> arc;
            std::shared_ptr<TEdge> edge;
        };
        std::shared_ptr<BeachLineItem> parent, left, right;
        BeachLineItem():parent(nullptr), left(nullptr), right(nullptr), type(ItemType::None), arc(nullptr) {}
        void set_left(std::shared_ptr<BeachLineItem> new_left);
        void set_right(std::shared_ptr<BeachLineItem> new_right);
        void set_parent_from_item(std::shared_ptr<BeachLineItem> item);
    };

    class BeachLine
    {
    };

}
#endif //LAB9_BEACHLINE_H
