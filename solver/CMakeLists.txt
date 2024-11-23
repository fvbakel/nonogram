cmake_minimum_required(VERSION 3.10...3.28)

# set the project name
#project(nonogram)

#set(OpenCV_STATIC ON)
#set (OpenCV_DIR /usr/share/OpenCV)
#set (OpenCV_DIR /usr/lib/x86_64-linux-gnu/cmake/opencv4)

#find_package(OpenCV REQUIRED core highgui)
find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(nonogram main.cpp)
target_include_directories(nonogram PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
target_link_libraries(nonogram imgsolver solvercore ${OpenCV_LIBS})

# install rules
install(TARGETS nonogram DESTINATION bin)

add_test(NAME Runs_nonogram COMMAND nonogram -f ../../puzzles/tiger.non)
set_tests_properties(Runs_nonogram PROPERTIES
  PASS_REGULAR_EXPRESSION "Solved with rules and constraints only."
)

add_test(NAME nonogram_help COMMAND nonogram -h)
set_tests_properties(nonogram_help PROPERTIES
  PASS_REGULAR_EXPRESSION "Display this help text"
)

add_test(NAME nonogram_wrong_file COMMAND nonogram -f dummy.non)
set_tests_properties(nonogram_wrong_file PROPERTIES
  FAIL_REGULAR_EXPRESSION "Solved successfully"
  PASS_REGULAR_EXPRESSION "ERROR: Unable to open file"
)

add_test(NAME nonogram_png_file COMMAND nonogram -f ../../test_data/puzzle-30-nonogram.png)
set_tests_properties(nonogram_png_file PROPERTIES
  PASS_REGULAR_EXPRESSION "Solved with rules only."
)


