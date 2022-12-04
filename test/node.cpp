#include "lib/node.hpp"

#include <QtGui/QBrush>
#include <cassert>

int main() {
    const auto root = lib::MakeNode(
        "A", lib::MakeNode("C", lib::MakeLeaf("D"), lib::MakeLeaf("E")),
        lib::MakeLeaf("B"));

    assert(root->getContent() == "A");
    assert(root->getLeftChild()->getContent() == "C");
    assert(root->getLeftChild()->getLeftChild()->getContent() == "D");
    assert(root->getLeftChild()->getRightChild()->getContent() == "E");
    assert(root->getRightChild()->getContent() == "B");
    assert(root->getBrush() == QBrush(Qt::yellow));

    return 0;
}
