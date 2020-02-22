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
        used = v.used;
    }

    Vertex& Vertex::operator=(const Vertex &v) noexcept
    {
        if (&v == this)
            return *this;
        edge_id = v.edge_id;
        next_v = v.next_v;
        used = v.used;
        return *this;
    }

    Vertex::Vertex(Vertex &&v) noexcept
    {
        next_v = v.next_v;
        edge_id = std::exchange(v.edge_id, nullptr);
        used = v.used;
    }

    Vertex &Vertex::operator=(Vertex &&v) noexcept
    {
        if (&v == this)
            return *this;
        next_v = v.next_v;
        edge_id = std::exchange(v.edge_id, nullptr);
        used = v.used;
        return *this;
    }

    Vertex::~Vertex() noexcept
    {

    }
}