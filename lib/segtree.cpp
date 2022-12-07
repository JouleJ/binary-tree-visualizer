#include "data-structure.hpp"

#include <any>
#include <cassert>
#include <iostream> // For debug purposes
#include <set>
#include <stdexcept>
#include <vector>

namespace lib {

namespace {
class PositionArgumentScheme : public ArgumentScheme {
    size_t count;

  public:
    explicit PositionArgumentScheme(size_t _count) : count(_count) {}

    QString getName() const override { return "Позиция"; }

    bool validateValue(int64_t value) const override {
        return (value >= 0) && (value < static_cast<int64_t>(count));
    }
};

class NewValueArgumentScheme : public ArgumentScheme {
  public:
    explicit NewValueArgumentScheme() = default;

    QString getName() const override { return "Новое значение"; }

    bool validateValue(int64_t value) const override {
        static_cast<void>(value);
        return true;
    }
};

class UpdateRequestScheme : public RequestScheme {
    PositionArgumentScheme positionArgumentScheme;
    NewValueArgumentScheme newValueArgumentScheme;

  public:
    explicit UpdateRequestScheme(size_t count)
        : positionArgumentScheme(count), newValueArgumentScheme() {}

    QString getName() const override { return "Изменение в точке"; }

    bool postValidateValues(const int64_t *values) const override {
        static_cast<void>(values);
        return true;
    }

    size_t getArgumentCount() const override { return 2; }

    const ArgumentScheme *getArgumentScheme(size_t idx) const override {
        switch (idx) {
        case 0:
            return &positionArgumentScheme;
        case 1:
            return &newValueArgumentScheme;
        default:
            throw std::runtime_error(
                "UpdateRequestScheme::getArgumentScheme: idx > 1");
        }
    }

    bool doesReturn() const override { return false; }
};

class LeftBoundArgumentScheme : public ArgumentScheme {
    size_t count;

  public:
    explicit LeftBoundArgumentScheme(size_t _count) : count(_count) {}

    QString getName() const override { return "Левая граница"; }

    bool validateValue(int64_t value) const override {
        return (value >= 0) && (value < static_cast<int64_t>(count));
    }
};

class RightBoundArgumentScheme : public ArgumentScheme {
    size_t count;

  public:
    explicit RightBoundArgumentScheme(size_t _count) : count(_count) {}

    QString getName() const override { return "Правая граница"; }

    bool validateValue(int64_t value) const override {
        return (value >= 0) && (value < static_cast<int64_t>(count));
    }
};

class GetSumRequestScheme : public RequestScheme {
    LeftBoundArgumentScheme leftBoundArgumentScheme;
    RightBoundArgumentScheme rightBoundArgumentScheme;

  public:
    explicit GetSumRequestScheme(size_t count)
        : leftBoundArgumentScheme(count), rightBoundArgumentScheme(count) {}

    QString getName() const override { return "Сумма на отрезке"; }

    bool postValidateValues(const int64_t *values) const override {
        return values[0] <= values[1];
    }

    size_t getArgumentCount() const override { return 2; }

    const ArgumentScheme *getArgumentScheme(size_t idx) const override {
        switch (idx) {
        case 0:
            return &leftBoundArgumentScheme;
        case 1:
            return &rightBoundArgumentScheme;
        default:
            throw std::runtime_error(
                "GetSumRequestScheme::getArgumentScheme: idx > 1");
        }
    }

    bool doesReturn() const override { return true; }
};

class SegmentTree : public DataStructure {
    size_t count;
    std::vector<int64_t> vertices;
    std::set<size_t> initializedVertices;

    UpdateRequestScheme updateRequestScheme;
    GetSumRequestScheme getSumRequestScheme;

    std::vector<std::unique_ptr<INode>> animatedRoot;

    std::unique_ptr<INode> visualize(size_t vertex_id, size_t left_bound,
                                     size_t right_bound) const {
        if (vertex_id >= vertices.size()) {
            return nullptr;
        }

        QString content;
        if (initializedVertices.count(vertex_id) > 0) {
            content = QString::number(vertices[vertex_id]);
        } else {
            content = "None";
        }

        if (left_bound == right_bound) {
            return MakeLeaf(std::move(content),
                            std::make_any<size_t>(left_bound));
        }

        const size_t mid = getMid(left_bound, right_bound);
        auto leftNode = visualize(2 * vertex_id + 1, left_bound, mid);
        auto rightNode = visualize(2 * vertex_id + 2, mid + 1, right_bound);

        return MakeNode(std::move(content), std::move(leftNode),
                        std::move(rightNode));
    }

    size_t getMid(size_t left_bound, size_t right_bound) const {
        return left_bound + (right_bound - left_bound) / 2;
    }

    void build(size_t vertex_id, size_t left_bound, size_t right_bound,
               const int64_t *values) {
        if (vertex_id >= vertices.size()) {
            vertices.resize(vertex_id + 1, -1);
        }

        if (left_bound == right_bound) {
            vertices[vertex_id] = values[left_bound];
            initializedVertices.insert(vertex_id);
            animatedRoot.push_back(visualize(0, 0, count - 1));
            return;
        }

        const size_t mid = getMid(left_bound, right_bound);

        build(2 * vertex_id + 1, left_bound, mid, values);
        build(2 * vertex_id + 2, mid + 1, right_bound, values);
        vertices[vertex_id] =
            vertices[2 * vertex_id + 1] + vertices[2 * vertex_id + 2];

        initializedVertices.insert(vertex_id);
        animatedRoot.push_back(visualize(0, 0, count - 1));
    }

    void update(size_t vertex_id, size_t left_bound, size_t right_bound,
                size_t position, int64_t new_value) {
        if (left_bound == right_bound) {
            vertices[vertex_id] = new_value;
            animatedRoot.push_back(visualize(0, 0, count - 1));
            return;
        }

        const size_t mid = getMid(left_bound, right_bound);
        if (position <= mid) {
            update(2 * vertex_id + 1, left_bound, mid, position, new_value);
        } else {
            update(2 * vertex_id + 2, mid + 1, right_bound, position,
                   new_value);
        }

        vertices[vertex_id] =
            vertices[2 * vertex_id + 1] + vertices[2 * vertex_id + 2];

        animatedRoot.push_back(visualize(0, 0, count - 1));
    }

    int64_t getSum(size_t vertex_id, size_t left_bound, size_t right_bound,
                   size_t req_left_bound, size_t req_right_bound) {
        assert(left_bound <= req_left_bound);
        assert(right_bound >= req_right_bound);

        if (left_bound == req_left_bound && right_bound == req_right_bound) {
            return vertices[vertex_id];
        }

        const size_t mid = getMid(left_bound, right_bound);

        if (req_right_bound <= mid) {
            return getSum(2 * vertex_id + 1, left_bound, mid, req_left_bound,
                          req_right_bound);
        }

        if (req_left_bound > mid) {
            return getSum(2 * vertex_id + 2, mid + 1, right_bound,
                          req_left_bound, req_right_bound);
        }

        return getSum(2 * vertex_id + 1, left_bound, mid, req_left_bound, mid) +
               getSum(2 * vertex_id + 2, mid + 1, right_bound, mid + 1,
                      req_right_bound);
    }

  public:
    SegmentTree(const int64_t *first, size_t _count)
        : count(_count), updateRequestScheme(_count),
          getSumRequestScheme(_count) {

        if (count == 0) {
            throw std::runtime_error(
                "SegmentTree: count must be greater than zero");
        }

        build(0, 0, count - 1, first);
    }

    size_t getAnimationStepCount() const override {
        return animatedRoot.size();
    }

    INode *getRoot(size_t animationStep) const override {
        return animatedRoot.at(animationStep).get();
    }

    size_t getRequestSchemeCount() const override { return 2; }

    const RequestScheme *getRequestScheme(size_t idx) const override {
        switch (idx) {
        case 0:
            return &updateRequestScheme;
        case 1:
            return &getSumRequestScheme;
        default:
            throw std::runtime_error("SegmentTree::getRequestScheme: idx > 1");
        }
    }

    int64_t executeRequest(const RequestScheme *requestScheme,
                           const int64_t *firstValue) override {
        if (requestScheme == &updateRequestScheme) {
            const size_t position = static_cast<size_t>(firstValue[0]);
            const int64_t newValue = firstValue[1];
            animatedRoot.clear();
            update(0, 0, count - 1, position, newValue);
            return 0;
        }

        if (requestScheme == &getSumRequestScheme) {
            const size_t leftBound = static_cast<size_t>(firstValue[0]);
            const size_t rightBound = static_cast<size_t>(firstValue[1]);
            return getSum(0, 0, count - 1, leftBound, rightBound);
        }

        throw std::runtime_error(
            "SegmentTree::executeRequest: bad requestScheme");
    }
};
} // namespace

std::unique_ptr<DataStructure> MakeSegmentTree(const int64_t *first,
                                               size_t count) {
    return std::make_unique<SegmentTree>(first, count);
}
} // namespace lib
