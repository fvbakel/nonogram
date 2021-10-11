cmake_minimum_required(VERSION 3.0.0)
project(nonogram VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(libsolvercore)
add_executable(nonogram ./solver/main.cpp)
target_link_libraries(nonogram PUBLIC libsolvercore)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# install rules
install(TARGETS nonogram DESTINATION bin)


add_test(NAME Runs COMMAND nonogram ./puzzle/tiger.non)