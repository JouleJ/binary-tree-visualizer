#include "gui/data-structure-viewer.hpp"

#include <QPainter>
#include <QTimer>

#include <algorithm>

// TODO: calculate this based on widget size
qreal DataStructureViewer::getNodeWidth() const { return 64; }

qreal DataStructureViewer::getNodeHeight() const { return 64; }

qreal DataStructureViewer::getVertGap() const { return 16; }

qreal DataStructureViewer::getHorGap() const { return 32; }

qreal DataStructureViewer::getRowHeight(size_t row_id) const {
    return row_id * getNodeHeight() + (row_id + 1) * getVertGap();
}

DataStructureViewer::DataStructureViewer(
    QWidget *parent, const lib::DataStructure *_dataStructure)
    : QWidget(parent), dataStructure(_dataStructure) {
    refreshNodes();
    QObject::connect(this, &DataStructureViewer::widgetCreated, this,
                     &DataStructureViewer::onRequestExecuted);
    emit widgetCreated();
}

void DataStructureViewer::refreshNodes() {
    rows.clear();
    edges.clear();
    width = 0.;
    height = 0.;
    currentLeafX = getHorGap();

    const auto root = dataStructure->getRoot(animationStep);
    recurseTree(root, 0);

    width = currentLeafX;
    height = getRowHeight(rows.size());
}

qreal DataStructureViewer::recurseTree(const lib::INode *node, size_t row_id) {
    if (node == nullptr) {
        return 0;
    }

    if (row_id >= rows.size()) {
        rows.resize(row_id + 1);
    }

    NodeView out_node;
    out_node.text = node->getContent();
    out_node.height = getNodeHeight();
    out_node.width = getNodeWidth();
    out_node.brush = node->getBrush();
    out_node.y = getRowHeight(row_id);
    out_node.meta = node->getMeta();
    out_node.textColor = node->getTextColor();
    out_node.metaColor = node->getMetaColor();

    auto xl = recurseTree(node->getLeftChild(), row_id + 1);
    auto xr = recurseTree(node->getRightChild(), row_id + 1);
    qreal curX = 0;
    if (xl == 0 && xr == 0) {
        // leaf
        curX = currentLeafX;
        currentLeafX += getNodeWidth() + getHorGap();
    } else {
        int non_zero_cnt = (xl != 0) + (xr != 0);
        curX = (xl + xr) / non_zero_cnt;
    }
    out_node.x = curX;

    // this will break if node.width and node.height
    // is different for nodes in the same tree
    // we can return NodeView * from children
    if (xl != 0) {
        QPointF pt1 = out_node.getCenter();
        QPointF pt2(xl + getNodeWidth() / 2,
                    getRowHeight(row_id + 1) + getNodeHeight() / 2);
        edges.emplace_back(pt1, pt2);
    }
    if (xr != 0) {
        QPointF pt1 = out_node.getCenter();
        QPointF pt2(xr + getNodeWidth() / 2,
                    getRowHeight(row_id + 1) + getNodeHeight() / 2);
        edges.emplace_back(pt1, pt2);
    }

    rows[row_id].nodes.emplace_back(std::move(out_node));
    return curX;
}

QSize DataStructureViewer::minimumSizeHint() const {
    return QSize(static_cast<int>(width), static_cast<int>(height));
}

QSize DataStructureViewer::sizeHint() const { return minimumSizeHint(); }

void DataStructureViewer::paintEvent(QPaintEvent *event) {
    static_cast<void>(event); // UNUSED

    refreshNodes();
    QPainter painter(this);
    for (const auto &edge : edges) {
        painter.drawLine(edge);
    }

    for (const auto &row : rows) {
        for (const auto &node : row.nodes) {
            node.paintNode(painter);
        }
    }

    const size_t animationStepCount = dataStructure->getAnimationStepCount();
    std::cout << "animation: " << animationStep << "/" << animationStepCount
              << "\n";
    if (animationStep + 1 < animationStepCount) {
        ++animationStep;
    }
}

size_t DataStructureViewer::getAnimationDelayMsec() const { return 2500; }

void DataStructureViewer::onAnimationStep() {
    update();
    resize(minimumSizeHint());
}

void DataStructureViewer::onRequestExecuted() {
    animationStep = 0;
    const size_t animationStepCount = dataStructure->getAnimationStepCount();
    const size_t delay = getAnimationDelayMsec();

    for (size_t step_id = 0; step_id < animationStepCount; ++step_id) {
        QTimer::singleShot(delay * (step_id + 1), this,
                           &DataStructureViewer::onAnimationStep);
    }
}
