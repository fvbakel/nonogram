cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(nonogram-qt main.cpp NonogramQt.cpp NonogramQt.h)
target_link_libraries(nonogram-qt Qt5::Widgets)

install(TARGETS nonogram-qt DESTINATION bin)