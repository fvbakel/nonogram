cmake_minimum_required(VERSION 3.10)

#set(OpenCV_STATIC ON)
#set (OpenCV_DIR /usr/share/OpenCV)
set (OpenCV_DIR /usr/lib/x86_64-linux-gnu/cmake/opencv4)

#find_package(OpenCV REQUIRED core highgui)
find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(test_imgsolver test_imgsolver.cpp)
target_include_directories(test_imgsolver PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
target_link_libraries(test_imgsolver imgsolver solvercore ${OpenCV_LIBS})

enable_testing()
add_test(NAME run_test_imgsolver COMMAND test_imgsolver)

