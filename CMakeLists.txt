cmake_minimum_required(VERSION 3.16)
project(binary-tree-visualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

include(CTest)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(lib lib/node.cpp lib/segtree.cpp)
target_link_libraries(lib PUBLIC Qt::Core)

add_executable(binary-tree-visualizer cmd/main.cpp gui/data-structure-viewer.cpp)
target_link_libraries(binary-tree-visualizer PUBLIC Qt::Core Qt::Gui Qt::Widgets lib)

add_executable(test-node test/node.cpp)
target_link_libraries(test-node PUBLIC Qt::Core lib)

add_executable(test-segtree test/segtree.cpp)
target_link_libraries(test-segtree PUBLIC Qt::Core lib)

add_test(NAME TestNode COMMAND ./test-node)
add_test(NAME TestSegmentTree COMMAND ./test-segtree)
