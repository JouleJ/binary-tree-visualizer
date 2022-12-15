#pragma once

#include "gui/int-array-edit.hpp"
#include "gui/merged-data-structure-widget.hpp"

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

class MainWindow : public QMainWindow {
    Q_OBJECT

  private:
    QMenu *createDataStructureMenu = nullptr;

    QAction *createSegmentTreeAction = nullptr;
    QAction *createHeapAction = nullptr;

    MergedDataStructureWidget *mergedDataStructureWidget = nullptr;

    std::vector<QWidget *> dialogWidgets;

    void callbackAfterTreeChoice();

  private slots:
    void askUserForSegTreeArray();
    void onUserPromptedSegTreeArray(std::vector<int64_t> array);

    void askUserForHeapArray();
    void onUserPromptedHeapArray(std::vector<int64_t> array);

    void closeDialogWidgets();

  public:
    MainWindow();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};
