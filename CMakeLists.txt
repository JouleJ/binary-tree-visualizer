cmake_minimum_required(VERSION 3.16)
project(binary-tree-visualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

include(CTest)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(lib lib/node.cpp lib/segtree.cpp lib/heap.cpp lib/request.cpp lib/casual-tree.cpp lib/common.hpp)
target_link_libraries(lib PUBLIC Qt::Core Qt::Gui)

add_executable(binary-tree-visualizer
    cmd/main.cpp
    gui/data-structure-viewer.cpp
    gui/request-form.cpp
    gui/merged-data-structure-widget.cpp
    gui/int-array-edit.cpp
    gui/main-window.cpp)

target_link_libraries(binary-tree-visualizer PUBLIC Qt::Core Qt::Gui Qt::Widgets lib)

add_executable(test-node test/node.cpp)
target_link_libraries(test-node PUBLIC Qt::Core lib)

add_executable(test-segtree test/segtree.cpp)
target_link_libraries(test-segtree PUBLIC Qt::Core lib)

add_executable(test-casual test/casual-tree.cpp)
target_link_libraries(test-casual PUBLIC Qt::Core lib)

add_test(NAME TestNode COMMAND ./test-node)
add_test(NAME TestSegmentTree COMMAND ./test-segtree)
add_test(NAME TestCasualTree COMMAND ./test-casual)
