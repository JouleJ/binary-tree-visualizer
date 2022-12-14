#include "common.hpp"
#include "data-structure.hpp"
#include "node.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace lib {

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
        return 0;
    }

    const RequestScheme *getRequestScheme(size_t idx) const override {
        return nullptr;
    }

    size_t getRequestSchemeCount() const override { return 0; }

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
        if (id == 0)
            return;
        visualize(id);
        auto par = getParentIndex(id);
        if (cmp_(heap_[par], heap_[id])) {
            std::swap(heap_[par], heap_[id]);
            siftUp(par);
        }
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
