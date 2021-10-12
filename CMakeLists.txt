cmake_minimum_required(VERSION 3.0.0)
project(nonogram VERSION 0.1.0)
#https://gitlab.com/CLIUtils/modern-cmake/-/tree/master/examples/extended-project
include(CTest)
enable_testing()

#add_subdirectory(hello)
add_subdirectory(libsolvercore)
add_subdirectory(imgsolver)
add_executable(nonogram ./solver/main.cpp)
target_link_libraries(nonogram PUBLIC libsolvercore)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# install rules
install(TARGETS nonogram DESTINATION bin)

add_test(NAME Runs_nonogram COMMAND nonogram -f ../puzzles/tiger.non)
set_tests_properties(Runs_nonogram PROPERTIES
  PASS_REGULAR_EXPRESSION "Solved with rules and constraints only."
)

add_test(NAME Runs_help COMMAND nonogram -h)
set_tests_properties(Runs_help PROPERTIES
  PASS_REGULAR_EXPRESSION "Display this help text"
)

add_test(NAME nonogram_wrong_file COMMAND nonogram -f dummy.non)
set_tests_properties(nonogram_wrong_file PROPERTIES
  FAIL_REGULAR_EXPRESSION "Solved successfully"
  PASS_REGULAR_EXPRESSION "ERROR: Unable to open file"
)