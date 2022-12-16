# binary-tree-visualizer
Визуализатор структур данных, основанных на бинарных деревьях, и алгоритмов над ними.

# Как собрать (Ubuntu 22.04)
Установите зависимости:
```bash
sudo apt update
sudo apt install qt6-base make cmake ctest
```
Склонируйте репозиторий. В папке с репозиторием:
```bash
mkdir build
cd build
cmake ..
make
```
Запустите приложение с помощью `./binary_tree_visualizer`
