#include "merged-data-structure-widget.hpp"

#include <algorithm>

#include <QSizePolicy>

MergedDataStructureWidget::MergedDataStructureWidget(QWidget *parent)
    : QFrame(parent) {
    repaintTimer = new QTimer(this);
    QObject::connect(repaintTimer, &QTimer::timeout, this,
                     &MergedDataStructureWidget::onAnimationStep);
    repaintTimer->start(getRepaintTimeout());
}

void MergedDataStructureWidget::acceptDataStructure(
    std::unique_ptr<lib::DataStructure> ds) {
    if (dataStructure) {
        viewer->deleteLater();
        requester->deleteLater();
        hboxLayout->deleteLater();
    }

    dataStructure = std::move(ds);
    hboxLayout = new QHBoxLayout(this);

    requester = new Requester(this, dataStructure.get());
    viewer = new DataStructureViewer(this, dataStructure.get());

    QObject::connect(requester, &Requester::requestExecuted, viewer,
                     &DataStructureViewer::onRequestExecuted);
    QObject::connect(viewer, &DataStructureViewer::animationStepSignal, this,
                     &MergedDataStructureWidget::onAnimationStep);

    requester->setSizePolicy(
        QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    hboxLayout->addWidget(requester);
    hboxLayout->addWidget(viewer);

    setLayout(hboxLayout);
}

int MergedDataStructureWidget::getGap() const { return 35; }

int MergedDataStructureWidget::getRepaintTimeout() const { return 133; }

QSize MergedDataStructureWidget::sizeHint() const {
    if (viewer == nullptr || requester == nullptr) {
        return QSize(256, 256);
    }

    const auto viewerSize = viewer->sizeHint();
    const auto requesterSize = requester->sizeHint();
    const auto width =
        viewerSize.width() + requesterSize.width() + getGap();
    const auto height =
        std::max(viewerSize.height(), requesterSize.height()) + getGap();

    return QSize(width, height);
}

QSize MergedDataStructureWidget::minimumSizeHint() const {
    return sizeHint();
}

void MergedDataStructureWidget::onAnimationStep() {
    QWidget *parent = parentWidget();
    if (parent != nullptr) {
        parent->update();
        parent->resize(parent->sizeHint());
        parent->update();
    }

    update();
    resize(sizeHint());
    update();
}
