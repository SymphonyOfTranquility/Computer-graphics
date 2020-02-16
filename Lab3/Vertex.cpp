//
// Created by art on 2/16/20.
//

#include "Vertex.h"

namespace graph_space
{
    Vertex::Vertex(const Vertex &v) noexcept
    {
        if (v.weight != nullptr)
            weight = std::make_shared<int>(*v.weight);
        else
            weight = nullptr;
        next_v = v.next_v;
    }

    Vertex& Vertex::operator=(const Vertex &v) noexcept
    {
        if (&v == this)
            return *this;
        if (v.weight != nullptr)
            weight = std::make_shared<int>(*v.weight);
        else
            weight = nullptr;
        next_v = v.next_v;
        return *this;
    }

    Vertex::Vertex(Vertex &&v) noexcept
    {
        next_v = v.next_v;
        weight = std::exchange(v.weight, nullptr);
    }

    Vertex &Vertex::operator=(Vertex &&v) noexcept
    {
        if (&v == this)
            return *this;
        next_v = v.next_v;
        weight = std::exchange(v.weight, nullptr);
        return *this;
    }

    Vertex::~Vertex() noexcept
    {

    }
}