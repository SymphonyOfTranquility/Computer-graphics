//
// Created by art on 2/2/20.
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
        if (v.real != nullptr)
            real = std::make_shared<bool>(*v.real);
        else
            real = nullptr;
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
        if (v.real != nullptr)
            real = std::make_shared<bool>(*v.real);
        else
            real = nullptr;
        next_v = v.next_v;
        real = v.real;
        return *this;
    }

    Vertex::Vertex(Vertex &&v) noexcept
    {
        next_v = v.next_v;
        weight = std::exchange(v.weight, nullptr);
        real = std::exchange(v.real, nullptr);
    }

    Vertex &Vertex::operator=(Vertex &&v) noexcept
    {
        if (&v == this)
            return *this;
        next_v = v.next_v;
        weight = std::exchange(v.weight, nullptr);
        real = std::exchange(v.real, nullptr);
        return *this;
    }

    Vertex::~Vertex() noexcept
    {

    }


}