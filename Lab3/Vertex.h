//
// Created by art on 2/16/20.
//

#ifndef LAB3_VERTEX_H
#define LAB3_VERTEX_H


#include <memory>

namespace graph_space
{
    class Vertex
    {
    public:
        int next_v;
        std::shared_ptr<int> weight;

        Vertex() noexcept : next_v(-1), weight(nullptr) {};

        Vertex(int next_v, std::shared_ptr<int> weight) noexcept : next_v(next_v), weight(std::move(weight)) {};

        Vertex(const Vertex &v) noexcept;

        Vertex &operator=(const Vertex &v) noexcept;

        Vertex(Vertex &&v) noexcept;

        Vertex &operator=(Vertex &&v) noexcept;

        ~Vertex() noexcept;
    };
}


#endif //LAB3_VERTEX_H
