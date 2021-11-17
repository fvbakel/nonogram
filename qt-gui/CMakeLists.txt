cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(nonogram-qt main.cpp NonogramQt.cpp NonogramQt.h)
target_link_libraries(nonogram-qt Qt5::Widgets)
target_link_libraries(nonogram-qt imgsolver solvercore ${OpenCV_LIBS})

install(TARGETS nonogram-qt DESTINATION bin)