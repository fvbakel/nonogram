# Nonogram

A `CMakeList.txt` file is provided to allow development, build, test, installation and packaging with the CMake tool chain and all IDE's that support CMake.

To build with cmake you need to install cmake. Once installed use the procedure below:

```sh
mkdir build
cd build
cmake ../
cmake --build ./
```

To make a test installation and run the program:

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
