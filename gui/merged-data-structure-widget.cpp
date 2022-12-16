#include "merged-data-structure-widget.hpp"

#include <algorithm>
#include <iostream>

#include <QSizePolicy>

MergedDataStructureWidget::MergedDataStructureWidget(QWidget *parent)
    : QFrame(parent) {}

void MergedDataStructureWidget::buildLayout() {
    delete layout;
    layout = new QGridLayout(this);

    layout->addWidget(requester, 0, 0, 3, 1);
    layout->addWidget(viewer, 0, 1, 1, 2);
    layout->addWidget(animationProgressBar, 1, 1, 1, 2);
    layout->addWidget(animationBackwardsButton, 2, 1);
    layout->addWidget(animationForwardButton, 2, 2);
    layout->addWidget(animationPlayButton, 3, 1);
    layout->addWidget(animationStopButton, 3, 2);
}

void MergedDataStructureWidget::acceptDataStructure(
    std::unique_ptr<lib::DataStructure> ds) {
    qDeleteAll(children());

    dataStructure = std::move(ds);

    animationProgressBar = new QProgressBar(this);

    requester = new Requester(this, dataStructure.get());
    viewer = new DataStructureViewer(this, dataStructure.get());

    QObject::connect(requester, &Requester::requestExecuted, viewer,
                     &DataStructureViewer::onRequestExecuted);
    QObject::connect(viewer, &DataStructureViewer::animationStepSignal, this,
                     &MergedDataStructureWidget::onAnimationStep);

    QObject::connect(viewer, &DataStructureViewer::animationStepCountChanged,
                     animationProgressBar, &QProgressBar::setMaximum);
    QObject::connect(viewer, &DataStructureViewer::animationStepChanged,
                     animationProgressBar, &QProgressBar::setValue);

    animationForwardButton = new QPushButton("> Вперед", this);
    animationBackwardsButton = new QPushButton("< Назад", this);
    animationPlayButton = new QPushButton("[▶] Воспроизвести", this);
    animationStopButton = new QPushButton("[⏹] Стоп", this);

    QObject::connect(animationForwardButton, &QAbstractButton::clicked, viewer,
                     &DataStructureViewer::onAnimationMustGoForward);
    QObject::connect(animationBackwardsButton, &QAbstractButton::clicked,
                     viewer, &DataStructureViewer::onAnimationMustGoBackwards);
    QObject::connect(animationPlayButton, &QAbstractButton::clicked, viewer,
                     &DataStructureViewer::onAnimationMustPlay);
    QObject::connect(animationStopButton, &QAbstractButton::clicked, viewer,
                     &DataStructureViewer::onAnimationMustStop);

    buildLayout();

    repaintTimer = new QTimer(this);
    QObject::connect(repaintTimer, &QTimer::timeout, this,
                     &MergedDataStructureWidget::onAnimationStep);
    repaintTimer->start(getRepaintTimeout());

    viewer->handleAnimation();
}

int MergedDataStructureWidget::getRepaintTimeout() const { return 133; }

QSize MergedDataStructureWidget::sizeHint() const {
    if (layout != nullptr) {
        return layout->minimumSize();
    }

    return QSize(256, 256);
}

QSize MergedDataStructureWidget::minimumSizeHint() const { return sizeHint(); }

void MergedDataStructureWidget::onAnimationStep() {
    buildLayout();

    update();
    resize(sizeHint());
    update();

    QWidget *parent = parentWidget();
    if (parent != nullptr) {
        parent->update();
        parent->resize(parent->sizeHint());
        parent->update();
    }
}
