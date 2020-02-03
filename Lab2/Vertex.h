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
        std::shared_ptr<bool> real;

        Vertex() noexcept : next_v(-1), weight(nullptr), real(nullptr) {};

        Vertex(int next_v, std::shared_ptr<int> weight, std::shared_ptr<bool> real) noexcept : next_v(next_v),
                                                                                               weight(std::move(
                                                                                                       weight)),
                                                                                               real(std::move(real)) {};

        Vertex(const Vertex &v) noexcept;

        Vertex &operator=(const Vertex &v) noexcept;

        Vertex(Vertex &&v) noexcept;

        Vertex &operator=(Vertex &&v) noexcept;

        ~Vertex() noexcept;
    };
}
#endif //LAB2_VERTEX_H
