#include "node.hpp"

namespace lib {

    namespace {
        class NodeImpl : public Node {
            QString content;
            std::unique_ptr<Node> left, right;

        public:
            NodeImpl(QString _content,
                     std::unique_ptr<Node> _left,
                     std::unique_ptr<Node> _right):
                content(std::move(_content)),
                left(std::move(_left)),
                right(std::move(_right)) {}

            QString getContent() const override {
                return content;
            }

            const Node* getLeftChild() const override {
                return left.get();
            }

            const Node* getRightChild() const override {
                return right.get();
            }
        };

        class LeafImpl : public Node {
            QString content;

        public:
            LeafImpl(QString _content): content(std::move(_content)) {}

            QString getContent() const override {
                return content;
            }

            const Node* getLeftChild() const override {
                return nullptr;
            }

            const Node* getRightChild() const override {
                return nullptr;
            }
        };
    }

    std::unique_ptr<Node> MakeNode(QString content, std::unique_ptr<Node> left, std::unique_ptr<Node> right) {
        return std::make_unique<NodeImpl>(std::move(content), std::move(left), std::move(right));
    }

    std::unique_ptr<Node> MakeLeaf(QString content) {
        return std::make_unique<LeafImpl>(std::move(content));
    }
}
