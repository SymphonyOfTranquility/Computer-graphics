//
// Created by art on 2/19/20.
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
        bool used;
        int triangle_id;
        std::shared_ptr<int> edge_id;

        Vertex() noexcept : next_v(-1), edge_id(nullptr), used(false), triangle_id(-1) {};

        Vertex(int next_v, std::shared_ptr<int> edge_id) noexcept : next_v(next_v), edge_id(edge_id), used(false), triangle_id(-1) {};

        Vertex(const Vertex &v) noexcept;

        Vertex &operator=(const Vertex &v) noexcept;

        Vertex(Vertex &&v) noexcept;

        Vertex &operator=(Vertex &&v) noexcept;

        ~Vertex() noexcept;
    };
}

#endif //LAB3_VERTEX_H
