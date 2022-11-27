#pragma once

#include "lib/node.hpp"

#include <cstdint>

namespace lib {
    class DataStructure {
    public:
        virtual ~DataStructure() = default;

        virtual std::unique_ptr<Node> getRoot() const = 0;
    };

    std::unique_ptr<DataStructure> MakeSegmentTree(const int64_t* first, size_t count);
}
