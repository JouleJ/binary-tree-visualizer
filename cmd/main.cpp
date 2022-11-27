#include "gui/data-structure-viewer.hpp"

#include <QApplication>

int main(int argc, char **argv) {
    const int64_t array[] = {1, 2, 3, 4};
    auto segtree = lib::MakeSegmentTree(&array[0], sizeof(array) / sizeof(array[0]));

    const auto app = QApplication(argc, argv);
    auto widget = DataStructureViewer(nullptr, segtree.get());
    widget.show();
    return app.exec();
}
