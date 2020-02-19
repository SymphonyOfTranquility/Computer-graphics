//
// Created by art on 2/19/20.
//


#include "Vertex.h"

namespace graph_space
{
    Vertex::Vertex(const Vertex &v) noexcept
    {
        edge_id = v.edge_id;
        next_v = v.next_v;
    }

    Vertex& Vertex::operator=(const Vertex &v) noexcept
    {
        if (&v == this)
            return *this;
        edge_id = v.edge_id;
        next_v = v.next_v;
        return *this;
    }

    Vertex::Vertex(Vertex &&v) noexcept
    {
        next_v = v.next_v;
        edge_id = std::exchange(v.edge_id, nullptr);
    }

    Vertex &Vertex::operator=(Vertex &&v) noexcept
    {
        if (&v == this)
            return *this;
        next_v = v.next_v;
        edge_id = std::exchange(v.edge_id, nullptr);
        return *this;
    }

    Vertex::~Vertex() noexcept
    {

    }
}