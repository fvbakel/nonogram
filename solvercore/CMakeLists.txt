cmake_minimum_required(VERSION 3.5...3.28)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

FILE(GLOB MyCppSources ./classes/*.cpp)
#FILE(GLOB_RECURSE MyCppheaders ${nonogram_all_SOURCE_DIR}/include/*.h)
file(GLOB MyCppheaders CONFIGURE_DEPENDS "${nonogram_all_SOURCE_DIR}/include/solvercore/*.h")
add_library(solvercore ${MyCppSources} ${MyCppheaders})
target_include_directories(solvercore PUBLIC ../include
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/solvercore()
)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${MyCppheaders})


# install rules
install(TARGETS solvercore DESTINATION lib)
install(FILES ${MyCppheaders} DESTINATION include/solvercore)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#if(BUILD_TESTING)
add_executable(test_nonogram ./test/test_nonogram.cpp)
target_link_libraries(test_nonogram PUBLIC solvercore)
target_include_directories(test_nonogram PUBLIC ./include)
#endif()

add_test(NAME test_solvercore COMMAND test_nonogram)
