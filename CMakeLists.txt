cmake_minimum_required(VERSION 3.0.0)
project(nonogram_all VERSION 0.1.0)
#https://gitlab.com/CLIUtils/modern-cmake/-/tree/master/examples/extended-project

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(CTest)
enable_testing()

#add_subdirectory(hello)
add_subdirectory(solvercore)
add_subdirectory(imgsolver)
add_subdirectory(qt-gui)

add_subdirectory(solver)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
   add_subdirectory(test_imgsolver)
endif()

install(FILES ./tools/process_dir.sh DESTINATION bin)
install(FILES ./models/tessdata/nono-model.traineddata DESTINATION models/tessdata)
install(FILES ./models/tensorflow/nono-model-graph.pb DESTINATION models/tensorflow)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

