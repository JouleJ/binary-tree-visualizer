#pragma once

#include "lib/data-structure.hpp"

#include <QWidget>

#include <vector>

class DataStructureViewer : public QWidget {
    Q_OBJECT

private:
    const lib::DataStructure* dataStructure;

    void refreshNodes();
    void recurseTree(const lib::Node* node, size_t row_id);

    struct Node {
        qreal x, y;
        qreal width, height;
        QString text;
    };

    struct Row {
        std::vector<Node> nodes;
    };

    std::vector<Row> rows;
    qreal width, height;

public:
    explicit DataStructureViewer(QWidget* parent, const lib::DataStructure* _dataStructure);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
};
