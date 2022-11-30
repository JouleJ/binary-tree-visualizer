#pragma once

#include "lib/node.hpp"
#include "lib/request.hpp"

#include <cstdint>

namespace lib {
    class DataStructure {
    public:
        virtual ~DataStructure() = default;

        virtual std::unique_ptr<Node> getRoot() const = 0;

        virtual size_t getRequestSchemeCount() const = 0;
        virtual const RequestScheme* getRequestScheme(size_t idx) const = 0;

        virtual int64_t executeRequest(const RequestScheme* requestScheme, const int64_t* firstValue) = 0;
    };

    std::unique_ptr<DataStructure> MakeSegmentTree(const int64_t* first, size_t count);
}
