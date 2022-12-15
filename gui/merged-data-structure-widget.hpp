#pragma once

#include "gui/data-structure-viewer.hpp"
#include "gui/request-form.hpp"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>

class MergedDataStructureWidget : public QFrame {
    Q_OBJECT

  private:
    std::unique_ptr<lib::DataStructure> dataStructure;

    DataStructureViewer *viewer = nullptr;
    Requester *requester = nullptr;

    QGridLayout *layout = nullptr;

    QPushButton *animationForwardButton = nullptr;
    QPushButton *animationBackwardsButton = nullptr;

    QProgressBar *animationProgressBar = nullptr;

    QTimer *repaintTimer = nullptr;

    int getRepaintTimeout() const;
    void buildLayout();

  public:
    MergedDataStructureWidget(QWidget *parent);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void acceptDataStructure(std::unique_ptr<lib::DataStructure> ds);

  public slots:
    void onAnimationStep();
};
