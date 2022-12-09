#pragma once

#include "gui/data-structure-viewer.hpp"
#include "gui/request-form.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QTimer>

class MergedDataStructureWidget : public QFrame {
    Q_OBJECT

  private:
    std::unique_ptr<lib::DataStructure> dataStructure;

    DataStructureViewer *viewer = nullptr;
    Requester *requester = nullptr;

    QHBoxLayout *hboxLayout = nullptr;
    QTimer *repaintTimer = nullptr;

    int getRepaintTimeout() const;
    int getGap() const;

  public:
    MergedDataStructureWidget(QWidget *parent);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void acceptDataStructure(std::unique_ptr<lib::DataStructure> ds);

  public slots:
    void onAnimationStep();
};
