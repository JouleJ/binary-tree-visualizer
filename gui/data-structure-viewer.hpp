#pragma once

#include "lib/data-structure.hpp"

#include <QPainter>
#include <QWidget>

#include <any>
#include <iostream>
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
        std::any meta;
        QColor textColor;
        QColor metaColor;

        QPointF getCenter() { return QPointF(x + width / 2, y + height / 2); }

        void paintNodeBody(QPainter &p, const QRectF &border) const {
            p.setPen(Qt::black);
            p.setBrush(brush);
            p.drawEllipse(border);
        }

        // TODO - Calculate font size based on window size
        void paintNodeText(QPainter &p, const QRectF &border) const {
            p.setPen(textColor);
            p.setFont(QFont("Arial", 18, QFont::DemiBold));
            p.drawText(border, Qt::AlignCenter, text);
        }

        void paintNodeMeta(QPainter &p, const QRectF &border) const {
            if (meta.type().name() == typeid(size_t).name()) {
                p.setPen(metaColor);
                p.setFont(QFont("Arial", 12, QFont::Normal));
                p.drawText(border, Qt::AlignBottom,
                           QString::number(std::any_cast<size_t>(meta)));
            }
        }

        void paintNode(QPainter &p) const {
            const auto rect = QRectF(x, y, width, height);
            paintNodeBody(p, rect);
            paintNodeText(p, rect);
            paintNodeMeta(p, rect);
        }
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
