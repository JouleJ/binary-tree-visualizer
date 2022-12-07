#include "node.hpp"

#include <QtGui/QBrush>

#include <any>

namespace lib {

namespace {
class NodeImpl : public INode {
    QString content;
    std::any meta;
    std::unique_ptr<INode> left, right;
    QBrush brush;

  public:
    NodeImpl(QString _content, std::unique_ptr<INode> _left,
             std::unique_ptr<INode> _right, std::any meta, QBrush brush)
        : content(std::move(_content)), meta(meta), left(std::move(_left)),
          right(std::move(_right)), brush(brush) {}

    QString getContent() const override { return content; }

    const INode *getLeftChild() const override { return left.get(); }

    const INode *getRightChild() const override { return right.get(); }

    QBrush getBrush() const override { return brush; }

    void setBrush(QBrush newBrush) { brush = newBrush; }

    std::any getMeta() const override { return meta; }
};

class LeafImpl : public INode {
    QString content;
    std::any meta;
    QBrush brush;

  public:
    LeafImpl(QString _content, std::any meta, QBrush brush)
        : content(std::move(_content)), meta(meta), brush(brush) {}

    QString getContent() const override { return content; }

    const INode *getLeftChild() const override { return nullptr; }

    const INode *getRightChild() const override { return nullptr; }

    QBrush getBrush() const override { return brush; }

    void setBrush(QBrush newBrush) { brush = newBrush; }

    std::any getMeta() const override { return meta; }
};
} // namespace

std::unique_ptr<INode> MakeNode(QString content, std::unique_ptr<INode> left,
                                std::unique_ptr<INode> right, std::any meta,
                                QBrush brush) {
    return std::make_unique<NodeImpl>(std::move(content), std::move(left),
                                      std::move(right), meta, brush);
}

std::unique_ptr<INode> MakeLeaf(QString content, std::any meta, QBrush brush) {
    return std::make_unique<LeafImpl>(std::move(content), meta, brush);
}
} // namespace lib
