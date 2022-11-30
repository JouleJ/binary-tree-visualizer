#pragma once

#include <QString>

#include <memory>

namespace lib {
class Node {
public:
  virtual ~Node() = default;

  virtual QString getContent() const = 0;
  virtual const Node *getLeftChild() const = 0;
  virtual const Node *getRightChild() const = 0;
};

std::unique_ptr<Node> MakeNode(QString content, std::unique_ptr<Node> left,
                               std::unique_ptr<Node> right);
std::unique_ptr<Node> MakeLeaf(QString content);
} // namespace lib
