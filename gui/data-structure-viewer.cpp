#include "gui/data-structure-viewer.hpp"

#include <QPainter>
#include <QTimer>

#include <algorithm>

qreal DataStructureViewer::getNodeWidth() const {
    return 4 * dataStructure->getSizeUnit();
}

qreal DataStructureViewer::getNodeHeight() const {
    return 4 * dataStructure->getSizeUnit();
}

qreal DataStructureViewer::getVertGap() const {
    return 1 * dataStructure->getSizeUnit();
}

qreal DataStructureViewer::getHorGap() const {
    return 2 * dataStructure->getSizeUnit();
}

qreal DataStructureViewer::getRowHeight(size_t row_id) const {
    return row_id * getNodeHeight() + (row_id + 1) * getVertGap();
}

DataStructureViewer::DataStructureViewer(
    QWidget *parent, const lib::DataStructure *_dataStructure)
    : QWidget(parent), dataStructure(_dataStructure) {
    QObject::connect(this, &DataStructureViewer::widgetCreated, this,
                     &DataStructureViewer::onRequestExecuted);
    handleAnimation();
    emit widgetCreated();
}

void DataStructureViewer::refreshNodes() {
    if (dataStructure->getAnimationStepCount() != 0) {
        rows.clear();
        edges.clear();
        width_ = 0.;
        height_ = 0.;
        currentLeafX = getHorGap();

        const auto root = dataStructure->getRoot(animationStep);
        recurseTree(root, 0);

        height_ = getRowHeight(rows.size()) + getVertGap();
    }
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
    width_ = std::max<qreal>(width_, out_node.x + out_node.width + getHorGap());

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
    return QSize(static_cast<int>(width_ + 1.), static_cast<int>(height_ + 1.));
}

QSize DataStructureViewer::sizeHint() const { return minimumSizeHint(); }

void DataStructureViewer::paintEvent(QPaintEvent *event) {
    static_cast<void>(event); // UNUSED

    refreshNodes();
    QPainter painter(this);
    painter.drawRect(QRectF(0., 0., width_, height_));
    for (const auto &edge : edges) {
        painter.drawLine(edge);
    }

    for (const auto &row : rows) {
        for (const auto &node : row.nodes) {
            node.paintNode(painter);
        }
    }
}

size_t DataStructureViewer::getAnimationDelayMsec() const {
    return dataStructure->getAnimationDelay();
}

void DataStructureViewer::handleAnimation() {
    refreshNodes();
    resize(minimumSizeHint());

    const size_t animationStepCount = dataStructure->getAnimationStepCount();

    emit animationStepSignal();
    emit animationStepChanged(animationStep + 1);
    emit animationStepCountChanged(animationStepCount);
}

void DataStructureViewer::onAnimationMustGoForward() {
    const size_t animationStepCount = dataStructure->getAnimationStepCount();
    if (animationStep + 1 < animationStepCount) {
        ++animationStep;
        handleAnimation();
    }
}

void DataStructureViewer::onAnimationMustGoBackwards() {
    if (animationStep > 0) {
        --animationStep;
        handleAnimation();
    }
}

void DataStructureViewer::onRequestExecuted() {
    animationStep = 0;
    handleAnimation();
}

void DataStructureViewer::onAnimationMustPlay() {
    if (animationTimer != nullptr) {
        animationTimer = new QTimer(this);
        QObject::connect(animationTimer, &QTimer::timeout, this,
                         &DataStructureViewer::onAnimationMustGoForward);
        animationTimer->start(getAnimationDelayMsec());
    }
}

void DataStructureViewer::onAnimationMustStop() { delete animationTimer; }
