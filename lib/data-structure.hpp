#pragma once

#include "lib/common.hpp"
#include "lib/node.hpp"
#include "lib/request.hpp"

#include <cstdint>

namespace lib {
class DataStructure {
  public:
    virtual ~DataStructure() = default;

    virtual size_t getAnimationStepCount() const = 0;
    virtual INode *getRoot(size_t animationStep) const = 0;

    virtual size_t getRequestSchemeCount() const = 0;
    virtual const RequestScheme *getRequestScheme(size_t idx) const = 0;

    virtual int64_t executeRequest(const RequestScheme *requestScheme,
                                   const int64_t *firstValue) = 0;

    virtual size_t getAnimationDelay() const {
        return common::defaultAnimationDelay;
    }

    virtual size_t getSizeUnit() const { return common::defaultSizeUnit; }
};

std::unique_ptr<DataStructure> MakeSegmentTree(const int64_t *first,
                                               size_t count);

std::unique_ptr<DataStructure>
MakeBinaryTree(const std::vector<std::initializer_list<int>> &conf);
} // namespace lib
