#include "node.hpp"

#include <QtGui/QBrush>

#include <any>

namespace lib {

namespace {
class NodeImpl : public INode {
    QString content;
    std::any meta;
    std::unique_ptr<INode> left, right;

  public:
    NodeImpl(QString _content, std::unique_ptr<INode> _left,
             std::unique_ptr<INode> _right, std::any meta)
        : content(std::move(_content)), left(std::move(_left)),
          right(std::move(_right)), meta(meta) {}

    QString getContent() const override { return content; }

    const INode *getLeftChild() const override { return left.get(); }

    const INode *getRightChild() const override { return right.get(); }

    QBrush getBrush() const override { return QBrush(Qt::yellow); }

    std::any getMeta() const override { return meta; }
};

class LeafImpl : public INode {
    QString content;
    std::any meta;

  public:
    LeafImpl(QString _content, std::any meta)
        : content(std::move(_content)), meta(meta) {}

    QString getContent() const override { return content; }

    const INode *getLeftChild() const override { return nullptr; }

    const INode *getRightChild() const override { return nullptr; }

    QBrush getBrush() const override { return QBrush(Qt::yellow); }

    std::any getMeta() const override { return meta; }
};
} // namespace

std::unique_ptr<INode> MakeNode(QString content, std::unique_ptr<INode> left,
                                std::unique_ptr<INode> right, std::any meta) {
    return std::make_unique<NodeImpl>(std::move(content), std::move(left),
                                      std::move(right), meta);
}

std::unique_ptr<INode> MakeLeaf(QString content, std::any meta) {
    return std::make_unique<LeafImpl>(std::move(content), meta);
}
} // namespace lib
