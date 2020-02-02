//
// Created by art on 2/2/20.
//

#ifndef LAB2_VERTEX_H
#define LAB2_VERTEX_H

#include <memory>

namespace graph_space
{
    class Vertex
    {
    public:
        int next_v;
        std::shared_ptr<int> weight;

        Vertex() noexcept : next_v(-1), weight(nullptr) {};
        Vertex(int next_v, std::shared_ptr<int> weight) noexcept : next_v(next_v), weight(weight.get()) {};
        Vertex(const Vertex &v) noexcept ;
        Vertex& operator=(const Vertex &v) noexcept ;
        Vertex(Vertex &&v) noexcept;
        Vertex& operator=(Vertex &&v) noexcept ;
        ~Vertex() noexcept ;
    };
}

#endif //LAB2_VERTEX_H
