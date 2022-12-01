#include "lib/data-structure.hpp"

#include <cassert>

int main() {
    const int64_t array[] = {1, 2, 3, 4};
    const size_t array_size = sizeof(array) / sizeof(array[0]);

    auto segtree = lib::MakeSegmentTree(&array[0], array_size);
    auto root = segtree->getRoot();

    assert(root->getContent() == "10");
    assert(root->getLeftChild()->getContent() == "3");
    assert(root->getRightChild()->getContent() == "7");
    assert(root->getLeftChild()->getLeftChild()->getContent() == "1");
    assert(root->getLeftChild()->getRightChild()->getContent() == "2");
    assert(root->getRightChild()->getLeftChild()->getContent() == "3");
    assert(root->getRightChild()->getRightChild()->getContent() == "4");

    assert(root->getLeftChild()->getLeftChild()->getLeftChild() == nullptr);
    assert(root->getLeftChild()->getLeftChild()->getRightChild() == nullptr);

    assert(root->getLeftChild()->getRightChild()->getLeftChild() == nullptr);
    assert(root->getLeftChild()->getRightChild()->getRightChild() == nullptr);

    assert(root->getRightChild()->getLeftChild()->getLeftChild() == nullptr);
    assert(root->getRightChild()->getLeftChild()->getRightChild() == nullptr);

    assert(root->getRightChild()->getRightChild()->getLeftChild() == nullptr);
    assert(root->getRightChild()->getRightChild()->getRightChild() == nullptr);

    return 0;
}
