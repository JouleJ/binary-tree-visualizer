#include "lib/node.hpp"

#include <cassert>

int main() {
    const auto root = lib::MakeNode(
        "A",
        lib::MakeNode(
            "C",
            lib::MakeLeaf("D"),
            lib::MakeLeaf("E")
        ),
        lib::MakeLeaf("B")
    );

    assert(root->getContent() == "A");
    assert(root->getLeftChild()->getContent() == "C");
    assert(root->getLeftChild()->getLeftChild()->getContent() == "D");
    assert(root->getLeftChild()->getRightChild()->getContent() == "E");
    assert(root->getRightChild()->getContent() == "B");

    return 0;
}
