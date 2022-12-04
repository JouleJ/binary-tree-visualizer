#pragma once

#include "lib/data-structure.hpp"

#include <QWidget>

#include <vector>

class DataStructureViewer : public QWidget {
    Q_OBJECT

  private:
    const lib::DataStructure *dataStructure;

    void refreshNodes();
    // calculate NodeView parameters
    // return: x-coordinate of node (0 if node == nullptr)
    qreal recurseTree(const lib::INode *node, size_t row_id);

    struct NodeView {
        qreal x, y;
        qreal width, height;
        QString text;
        QBrush brush;

        QPointF getCenter() { return QPointF(x + width / 2, y + height / 2); }
    };

    struct Row {
        std::vector<NodeView> nodes;
    };

    qreal currentLeafX;
    std::vector<Row> rows;
    std::vector<QLineF> edges;
    qreal width, height;

  public:
    explicit DataStructureViewer(QWidget *parent,
                                 const lib::DataStructure *_dataStructure);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    qreal getNodeWidth() const;
    qreal getNodeHeight() const;
    qreal getVertGap() const;
    qreal getHorGap() const;

    qreal getRowHeight(size_t row_id) const;

  public slots:
    void onRequestExecuted();

  protected:
    void paintEvent(QPaintEvent *event) override;
};
