#include "gui/data-structure-viewer.hpp"

#include <QPainter>

#include <algorithm>

DataStructureViewer::DataStructureViewer(
    QWidget *parent, const lib::DataStructure *_dataStructure)
    : QWidget(parent), dataStructure(_dataStructure) {
    refreshNodes();
}

void DataStructureViewer::refreshNodes() {
    rows.clear();
    width = 0.;
    height = 0.;

    const auto root = dataStructure->getRoot();
    recurseTree(root.get(), 0);

    const qreal vert_gap = 16.; // TODO: Убрать магические числа
    const qreal hor_gap = 48.; // TODO: Убрать магические числа

    for (auto &row : rows) {
        qreal rowWidth = 0.;
        qreal rowHeight = 0.;

        for (const auto &node : row.nodes) {
            rowWidth += node.width + hor_gap;
            rowHeight = std::max<qreal>(rowHeight, node.height);
        }

        rowWidth += hor_gap;

        qreal lastNodeX = 0.;
        qreal lastNodeWidth = 0.;
        for (auto &node : row.nodes) {
            node.x = lastNodeX + lastNodeWidth + hor_gap;
            node.y = height;

            lastNodeX = node.x;
            lastNodeWidth = node.width;
        }

        width = std::max<qreal>(width, rowWidth);
        height += rowHeight + vert_gap;
    }

    for (auto &row : rows) {
        const size_t nodeCount = row.nodes.size();

        if (nodeCount > 0) {
            const qreal xMin = row.nodes[0].x;
            const qreal xMax =
                row.nodes[nodeCount - 1].x + row.nodes[nodeCount - 1].width;
            const qreal xSpan = xMax - xMin;
            const qreal padding = 0.5 * (width - xSpan);

            for (auto &node : row.nodes) {
                node.x += padding - xMin;
            }
        }
    }
}

#include <iostream>

void DataStructureViewer::recurseTree(const lib::Node *node, size_t row_id) {
    if (node == nullptr) {
        return;
    }

    if (row_id >= rows.size()) {
        rows.resize(row_id + 1);
    }

    Node out_node;
    out_node.text = node->getContent();
    out_node.height = 64.; // TODO: Убрать магические числа
    out_node.width =
        32. * out_node.text.length(); // TODO: Убрать магические числа
    rows[row_id].nodes.emplace_back(std::move(out_node));

    recurseTree(node->getLeftChild(), row_id + 1);
    recurseTree(node->getRightChild(), row_id + 1);
}

QSize DataStructureViewer::minimumSizeHint() const {
    return QSize(static_cast<int>(width), static_cast<int>(height));
}

QSize DataStructureViewer::sizeHint() const { return minimumSizeHint(); }

void DataStructureViewer::paintEvent(QPaintEvent *event) {
    static_cast<void>(event); // UNUSED

    refreshNodes();
    QPainter painter(this);
    for (const auto &row : rows) {
        for (const auto &node : row.nodes) {
            const auto rect = QRectF(node.x, node.y, node.width, node.height);
            painter.drawRect(rect);
            painter.drawText(rect, node.text);
        }
    }
}

void DataStructureViewer::onRequestExecuted() {
    update();
    resize(minimumSizeHint());
}
