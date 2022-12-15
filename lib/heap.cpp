#include "common.hpp"
#include "data-structure.hpp"
#include "node.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

namespace lib {

namespace {

class ValueArgumentScheme : public ArgumentScheme {
  public:
    explicit ValueArgumentScheme() = default;

    QString getName() const override { return "Значение"; }

    bool validateValue(int64_t value) const override {
        static_cast<void>(value);
        return true;
    }

    int getDefaultMinimum() const override { return -10000; }

    int getDefaultMaximum() const override { return 10000; }
};

class InsertRequestScheme : public RequestScheme {
    ValueArgumentScheme valueArgumentScheme;

  public:
    explicit InsertRequestScheme() = default;

    QString getName() const override { return "Вставить"; }

    bool postValidateValues(const int64_t *values) const override {
        static_cast<void>(values);
        return true;
    }

    size_t getArgumentCount() const override { return 1; }

    const ArgumentScheme *getArgumentScheme(size_t idx) const override {
        switch (idx) {
        case 0:
            return &valueArgumentScheme;
        default:
            throw std::runtime_error(
                "InsertRequestScheme::getArgumentScheme: idx > 0");
        }
    }

    bool doesReturn() const override { return false; }
};

class PopRequestScheme : public RequestScheme {
  public:
    explicit PopRequestScheme() = default;

    QString getName() const override { return "Удалить минимум"; }

    bool postValidateValues(const int64_t *values) const override {
        static_cast<void>(values);
        return true;
    }

    size_t getArgumentCount() const override { return 0; }

    const ArgumentScheme *getArgumentScheme(size_t idx) const override {
        static_cast<void>(idx); // UNUSED
        throw std::runtime_error("PopRequestScheme::getArgumentScheme");
    }

    bool doesReturn() const override { return false; }
};

class GetMinRequestScheme : public RequestScheme {
  public:
    explicit GetMinRequestScheme() = default;

    QString getName() const override { return "Найти минимум"; }

    bool postValidateValues(const int64_t *values) const override {
        static_cast<void>(values);
        return true;
    }

    size_t getArgumentCount() const override { return 0; }

    const ArgumentScheme *getArgumentScheme(size_t idx) const override {
        static_cast<void>(idx); // UNUSED
        throw std::runtime_error("GetMinRequestScheme::getArgumentScheme");
    }

    bool doesReturn() const override { return true; }
};

} // namespace

template <typename Comparator = std::less<int>>
class BinaryHeap : public DataStructure {
  public:
    BinaryHeap(const std::vector<int> &arr, Comparator cmp = Comparator{}) {
        cmp_ = std::move(cmp);
        for (const auto &value : arr) {
            insert(value);
        }
    }

    int64_t executeRequest(const RequestScheme *requestScheme,
                           const int64_t *firstValue) override {
        animatedRoot.clear();

        if (requestScheme == &getMinRequestScheme) {
            return getMin();
        }

        if (requestScheme == &insertRequestScheme) {
            int64_t value = *firstValue;
            insert(value);
            return -1;
        }

        if (requestScheme == &popRequestScheme) {
            pop();
            return -1;
        }

        throw std::runtime_error(
            "BinaryHeap::executeReques: invalid requestScheme");
    }

    const RequestScheme *getRequestScheme(size_t idx) const override {
        switch (idx) {
        case 0:
            return &insertRequestScheme;
        case 1:
            return &popRequestScheme;
        case 2:
            return &getMinRequestScheme;
        default:
            throw std::runtime_error("BinaryHeap::getRequestScheme: idx > 2");
        }
    }

    size_t getRequestSchemeCount() const override { return 3; }

    size_t getAnimationStepCount() const override {
        return animatedRoot.size();
    }

    INode *getRoot(size_t animationStep) const override {
        return animatedRoot.at(animationStep).get();
    }

    size_t size() const noexcept { return heap_.size(); }

  private:
    std::vector<int> heap_;
    // code naming suggests, that cmp_ = std::less<int>,
    // but in practice it could be any viable comparator
    Comparator cmp_;
    std::vector<std::unique_ptr<INode>> animatedRoot;

    InsertRequestScheme insertRequestScheme;
    PopRequestScheme popRequestScheme;
    GetMinRequestScheme getMinRequestScheme;

    void visualize(size_t visualized_from = -1) {
        animatedRoot.push_back(visualizeImpl(0, visualized_from));
    }

    std::unique_ptr<INode> visualizeImpl(size_t vertex_id,
                                         size_t visualized_from = -1) const {
        if (vertex_id >= size())
            return nullptr;
        auto lch = getLeftChildIndex(vertex_id);
        auto rch = getRightChildIndex(vertex_id);

        QString content = QString::number(heap_[vertex_id]);
        QBrush brush = (vertex_id == visualized_from ? QBrush(common::ORANGE)
                                                     : QBrush(common::GOLD));

        if (lch < size()) {
            auto leftChildPtr = visualizeImpl(lch, visualized_from);
            auto rightChildPtr = visualizeImpl(rch, visualized_from);
            return MakeNode(std::move(content), std::move(leftChildPtr),
                            std::move(rightChildPtr), std::any(),
                            std::move(brush));
        } else {
            return MakeLeaf(std::move(content), std::any(), std::move(brush));
        }
    }

    static size_t getLeftChildIndex(size_t p) noexcept { return 2 * p + 1; }

    static size_t getRightChildIndex(size_t p) noexcept { return 2 * p + 2; }

    static size_t getParentIndex(size_t ch) noexcept {
        return (ch == 0 ? 0 : (ch - 1) / 2);
    }

    void siftDown(size_t id) {
        if (id >= size())
            return; // just in case
        visualize(id);
        auto lch = getLeftChildIndex(id);
        auto rch = getRightChildIndex(id);

        if (lch < size()) {
            auto smallestChild = lch;
            if (rch < size() && cmp_(heap_[rch], heap_[lch])) {
                smallestChild = rch;
            }

            if (cmp_(heap_[smallestChild], heap_[id])) {
                std::swap(heap_[smallestChild], heap_[id]);
                siftDown(smallestChild);
            }
        }
    }

    void siftUp(size_t id) {
        visualize(id);
        if (id == 0)
            return;
        auto par = getParentIndex(id);
        if (cmp_(heap_[id], heap_[par])) {
            std::swap(heap_[par], heap_[id]);
            siftUp(par);
        }
    }

    int getMin() {
        visualize(0);
        return heap_.at(0);
    }

    // remove minimum from heap and return it.
    // throws with size() == 0
    int pop() {
        auto res = heap_[0];
        std::swap(heap_[0], heap_[size() - 1]);
        heap_.pop_back();
        siftDown(0);
        return res;
    }

    // insert value in heap
    void insert(int value) {
        heap_.push_back(value);
        siftUp(size() - 1);
    }
};

std::unique_ptr<DataStructure> MakeBinaryHeap(const std::vector<int> &arr) {
    return std::unique_ptr<BinaryHeap<std::less<int>>>(new BinaryHeap(arr));
}
} // namespace lib
