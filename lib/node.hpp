#pragma once

#include <QString>
#include <QtGui/QBrush>

#include <memory>

namespace lib {
class INode {
  public:
    virtual ~INode() = default;

    virtual QString getContent() const = 0;
    virtual const INode *getLeftChild() const = 0;
    virtual const INode *getRightChild() const = 0;

    virtual QBrush getBrush() const = 0;
};

std::unique_ptr<INode> MakeNode(QString content, std::unique_ptr<INode> left,
                                std::unique_ptr<INode> right);
std::unique_ptr<INode> MakeLeaf(QString content);
} // namespace lib
