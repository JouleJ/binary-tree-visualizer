#include "lib/data-structure.hpp"

#include <initializer_list>
#include <vector>

int main() {
    const auto tree =
        lib::MakeBinaryTree({{1, 2, 3}, {2, -1, 4}, {3, -1, -1}, {4, -1, -1}});

    const auto lastAnimationStep = tree->getAnimationStepCount() - 1;
    const auto root = tree->getRoot(lastAnimationStep);

    assert(root->getContent() == QString::number(1));
    assert(root->getLeftChild()->getContent() == QString::number(2));
    assert(root->getRightChild()->getContent() == QString::number(3));
    assert(root->getLeftChild()->getRightChild()->getContent() ==
           QString::number(4));

    return 0;
}
