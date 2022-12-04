#include "node.hpp"

#include <QtGui/QBrush>

namespace lib {

namespace {
class NodeImpl : public INode {
    QString content;
    std::unique_ptr<INode> left, right;

  public:
    NodeImpl(QString _content, std::unique_ptr<INode> _left,
             std::unique_ptr<INode> _right)
        : content(std::move(_content)), left(std::move(_left)),
          right(std::move(_right)) {}

    QString getContent() const override { return content; }

    const INode *getLeftChild() const override { return left.get(); }

    const INode *getRightChild() const override { return right.get(); }

    QBrush getBrush() const override { return QBrush(Qt::yellow); }
};

class LeafImpl : public INode {
    QString content;

  public:
    LeafImpl(QString _content) : content(std::move(_content)) {}

    QString getContent() const override { return content; }

    const INode *getLeftChild() const override { return nullptr; }

    const INode *getRightChild() const override { return nullptr; }

    QBrush getBrush() const override { return QBrush(Qt::yellow); }
};
} // namespace

std::unique_ptr<INode> MakeNode(QString content, std::unique_ptr<INode> left,
                                std::unique_ptr<INode> right) {
    return std::make_unique<NodeImpl>(std::move(content), std::move(left),
                                      std::move(right));
}

std::unique_ptr<INode> MakeLeaf(QString content) {
    return std::make_unique<LeafImpl>(std::move(content));
}
} // namespace lib
