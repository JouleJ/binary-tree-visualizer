#include "gui/main-window.hpp"

#include <iostream>
#include <string>

#include <QApplication>

bool DEBUG = false;

void parse_args(int argc, char **argv) {
    for (size_t i = 1; i < argc; i++) {
        std::string arg{argv[i]};
        if (arg == "-d" || arg == "--debug") {
            DEBUG = true;
        } else
        if (arg == "-h" || arg == "--help") {
            std::cout << "Binary tree visualizer:\n"
                         "(-d | --debug): debug mode\n"
                         "(-h | --help): print this message and exit\n";
            exit(0);
        } else {
            std::cout << "Unknown option. Print "
                      << std::string(argv[0])
                      << " -h for help\n";
            exit(0);
        }
    }
}

int main(int argc, char **argv) {
    parse_args(argc, argv);
    if (!DEBUG) {
        const auto app = QApplication(argc, argv);

        MainWindow mw;
        mw.show();
        return app.exec();
    } else {
        const int64_t array[] = {-1, 2, 3, 4, 5, 6, 7};
        auto segtree =
            lib::MakeSegmentTree(&array[0], sizeof(array) / sizeof(array[0]));

        const auto app = QApplication(argc, argv);
        auto widget1 = DataStructureViewer(nullptr, segtree.get());
        auto widget2 = Requester(nullptr, segtree.get());
        QObject::connect(&widget2, &Requester::requestExecuted, &widget1,
                        &DataStructureViewer::onRequestExecuted);

        widget1.show();
        widget2.show();
        return app.exec();
    }
}
