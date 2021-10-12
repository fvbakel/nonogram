cmake_minimum_required(VERSION 3.10)

# set the project name
project(imgsolver)

set(OpenCV_STATIC ON)
#set (OpenCV_DIR /usr/share/OpenCV)
set (OpenCV_DIR /usr/lib/x86_64-linux-gnu/cmake/opencv4)

#find_package(OpenCV REQUIRED core highgui)
find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(imgsolver main.cpp)
target_include_directories(imgsolver PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
target_link_libraries(imgsolver libsolvercore ${OpenCV_LIBS})

install(TARGETS imgsolver DESTINATION bin)

enable_testing()
add_test(NAME Runimgsolver COMMAND imgsolver)

