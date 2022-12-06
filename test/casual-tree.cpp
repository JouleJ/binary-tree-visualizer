#include "lib/data-structure.hpp"

#include <initializer_list>
#include <vector>

int main() {
    auto tree =
        lib::MakeBinaryTree({{1, 2, 3}, {2, -1, 4}, {3, -1, -1}, {4, -1, -1}});
    assert(tree->getRoot()->getContent() == QString::number(1));
    assert(tree->getRoot()->getLeftChild()->getContent() == QString::number(2));
    assert(tree->getRoot()->getRightChild()->getContent() ==
           QString::number(3));
    assert(tree->getRoot()->getLeftChild()->getRightChild()->getContent() ==
           QString::number(4));

    return 0;
}