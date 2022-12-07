#include "gui/merged-data-structure-widget.hpp"

#include <QApplication>

int main(int argc, char **argv) {
    const int64_t array[] = {-1, 2, 3, 4, 5, 6, 7};
    auto segtree =
        lib::MakeSegmentTree(&array[0], sizeof(array) / sizeof(array[0]));

    const auto app = QApplication(argc, argv);
    auto widget = MergedDataStructureWidget(nullptr);
    widget.show();
    widget.acceptDataStructure(std::move(segtree));
    return app.exec();
}
