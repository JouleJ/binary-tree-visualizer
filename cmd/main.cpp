#include "gui/data-structure-viewer.hpp"
#include "gui/request-form.hpp"

#include <QApplication>

int main(int argc, char **argv) {
    const int64_t array[] = {1, 2, 3, 4};
    auto segtree = lib::MakeSegmentTree(&array[0], sizeof(array) / sizeof(array[0]));

    const auto app = QApplication(argc, argv);
    auto widget1 = DataStructureViewer(nullptr, segtree.get());
    auto widget2 = Requester(nullptr, segtree.get());
    QObject::connect(&widget2, &Requester::requestExecuted, &widget1, &DataStructureViewer::onRequestExecuted);

    widget1.show();
    widget2.show();
    return app.exec();
}
