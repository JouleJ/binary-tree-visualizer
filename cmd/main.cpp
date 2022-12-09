#include "gui/main-window.hpp"

#include <QApplication>

int main(int argc, char **argv) {
    const int64_t array[] = {-1, 2, 3, 4, 5, 6, 7};
    auto segtree =
        lib::MakeSegmentTree(&array[0], sizeof(array) / sizeof(array[0]));

    const auto app = QApplication(argc, argv);

    MainWindow mw;
    mw.show();

    return app.exec();
}
