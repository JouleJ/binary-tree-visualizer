#pragma once

#include <QBrush>
#include <QColor>
#include <QString>

#include <any>
#include <memory>

#include "lib/common.hpp"

namespace lib {
class INode {
  public:
    virtual ~INode() = default;

    virtual QString getContent() const = 0;
    virtual const INode *getLeftChild() const = 0;
    virtual const INode *getRightChild() const = 0;

    virtual QBrush getBrush() const = 0;
    virtual std::any getMeta() const { return std::any(); }

    virtual QColor getTextColor() const { return common::defaultTextColor; }

    virtual QColor getMetaColor() const { return common::defaultMetaColor; }
};

std::unique_ptr<INode> MakeNode(QString content, std::unique_ptr<INode> left,
                                std::unique_ptr<INode> right,
                                std::any meta = std::any(),
                                QBrush brush = QBrush(common::GOLD));

std::unique_ptr<INode> MakeLeaf(QString content, std::any meta = std::any(),
                                QBrush brush = QBrush(common::GOLD));
} // namespace lib
