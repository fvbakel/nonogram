# Nonogram

## Build and install

A `CMakeList.txt` file is provided to allow development, build, test, installation and packaging with the CMake tool chain and all IDE's that support CMake.

To build with cmake you need to install cmake. Once installed use the procedure below:

```sh
mkdir build
cd build
cmake ../
cmake --build ./
```

To test after build

```sh
cd qt-gui
./nonogram-qt
```

To make a test installation:

```sh
mkdir build
cd build
cmake ../
cmake --build ./
mkdir test_install
cmake --install ./ --prefix ./test_install
cd test_install
./bin/
```

To make a release package:

```sh
mkdir build
cd build
cmake ../
cmake --build ./
cpack ./
```

## Configuration options

Configuration parameters:

export TESSDATA_PREFIX=~/git/tessdata_best
export MODEL_DIR=~git/nonogram/models

## Small facts

The default detector for parsing images is the DNN method for the nonogram-qt GUI. This should be configurable.

