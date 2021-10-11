cmake_minimum_required(VERSION 3.10)

# set the project name
project(HelloOpenCv)

set(OpenCV_STATIC ON)
#set (OpenCV_DIR /usr/share/OpenCV)
set (OpenCV_DIR /usr/lib/x86_64-linux-gnu/cmake/opencv4)

#find_package(OpenCV REQUIRED core highgui)
find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(hello hello.cpp)
target_include_directories(hello PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
target_link_libraries(hello ${OpenCV_LIBS})

install(TARGETS hello DESTINATION bin)

