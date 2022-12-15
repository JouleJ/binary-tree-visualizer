#include "gui/main-window.hpp"

MainWindow::MainWindow() {
    mergedDataStructureWidget = new MergedDataStructureWidget(this);

    createDataStructureMenu = menuBar()->addMenu("Создать структуру данных");

    createSegmentTreeAction =
        createDataStructureMenu->addAction("Создать дерево отрезков");
    createHeapAction = createDataStructureMenu->addAction("Создать кучу");

    QObject::connect(createSegmentTreeAction, &QAction::triggered, this,
                     &MainWindow::askUserForSegTreeArray);

    QObject::connect(createHeapAction, &QAction::triggered, this, &MainWindow::askUserForHeapArray);
}

void MainWindow::askUserForSegTreeArray() {
    IntArrayEdit *intArrayEdit = new IntArrayEdit(nullptr);
    QObject::connect(intArrayEdit, &IntArrayEdit::userFinished, this,
                     &MainWindow::onUserPromptedSegTreeArray);
    QObject::connect(intArrayEdit, &IntArrayEdit::userFinished, this,
                     &MainWindow::closeDialogWidgets);

    dialogWidgets.push_back(intArrayEdit);
    intArrayEdit->show();
}

void MainWindow::onUserPromptedSegTreeArray(std::vector<int64_t> array) {
    auto segtree = lib::MakeSegmentTree(array.data(), array.size());
    mergedDataStructureWidget->acceptDataStructure(std::move(segtree));
    callbackAfterTreeChoice();
}

void MainWindow::askUserForHeapArray() {
    IntArrayEdit *intArrayEdit = new IntArrayEdit(nullptr);
    QObject::connect(intArrayEdit, &IntArrayEdit::userFinished, this, &MainWindow::onUserPromptedHeapArray);
    QObject::connect(intArrayEdit, &IntArrayEdit::userFinished, this, &MainWindow::closeDialogWidgets);
    dialogWidgets.push_back(intArrayEdit);
    intArrayEdit->show();
}

void MainWindow::onUserPromptedHeapArray(std::vector<int64_t> array) {
    auto heap = lib::MakeBinaryHeap(std::vector<int>(array.begin(), array.end()));
    mergedDataStructureWidget->acceptDataStructure(std::move(heap));
    callbackAfterTreeChoice();
}

void MainWindow::callbackAfterTreeChoice() {
    resize(sizeHint());
    menuBar()->clear();
}

void MainWindow::closeDialogWidgets() {
    for (QWidget *widget : dialogWidgets) {
        widget->deleteLater();
    }

    dialogWidgets.clear();
}

QSize MainWindow::sizeHint() const {
    return mergedDataStructureWidget->sizeHint();
}

QSize MainWindow::minimumSizeHint() const {
    return mergedDataStructureWidget->minimumSizeHint();
}
