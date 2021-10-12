cmake_minimum_required(VERSION 3.0.0)
project(nonogram_all VERSION 0.1.0)
#https://gitlab.com/CLIUtils/modern-cmake/-/tree/master/examples/extended-project
include(CTest)
enable_testing()

#add_subdirectory(hello)
add_subdirectory(libsolvercore)
add_subdirectory(imgsolver)
add_subdirectory(solver)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

