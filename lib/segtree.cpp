#include "data-structure.hpp"

#include <stdexcept>
#include <vector>

namespace lib {

    namespace {
        class SegmentTree : public DataStructure {
            size_t count;
            std::vector<int64_t> vertices;

            size_t getMid(size_t left_bound, size_t right_bound) const {
                return left_bound + (right_bound - left_bound) / 2;
            }

            void build(size_t vertex_id, size_t left_bound, size_t right_bound, const int64_t* values) {
                if (vertex_id >= vertices.size()) {
                    vertices.resize(vertex_id + 1);
                }

                if (left_bound == right_bound) {
                    vertices[vertex_id] = values[left_bound];
                    return;
                }

                const size_t mid = getMid(left_bound, right_bound);

                build(2 * vertex_id + 1, left_bound, mid, values);
                build(2 * vertex_id + 2, mid + 1, right_bound, values);
                vertices[vertex_id] = vertices[2 * vertex_id + 1] + vertices[2 * vertex_id + 2];
            }

            std::unique_ptr<Node> visualize(size_t vertex_id, size_t left_bound, size_t right_bound) const {
                QString content = QString::number(vertices[vertex_id]);

                if (left_bound == right_bound) {
                    return MakeLeaf(std::move(content));
                }

                const size_t mid = getMid(left_bound, right_bound);
                auto leftNode = visualize(2 * vertex_id + 1, left_bound, mid);
                auto rightNode = visualize(2 * vertex_id + 2, mid + 1, right_bound);

                return MakeNode(std::move(content), std::move(leftNode), std::move(rightNode));
            }

        public:
            SegmentTree(const int64_t* first, size_t _count): count(_count) {
                if (count == 0) {
                    throw std::runtime_error("SegmentTree: count must be greater than zero");
                }

                build(0, 0, count - 1, first);
            }

            std::unique_ptr<Node> getRoot() const override {
                return visualize(0, 0, count - 1);
            }
        };    
    }

    std::unique_ptr<DataStructure> MakeSegmentTree(const int64_t* first, size_t count) {
        return std::make_unique<SegmentTree>(first, count);
    }
}
