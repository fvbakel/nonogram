# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
FILE(GLOB MyCppSources ./*.cpp)
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${nonogram_all_SOURCE_DIR}/include/imgsolver/*.h")

#set(OpenCV_STATIC ON)
#set (OpenCV_DIR /usr/share/OpenCV)
#set (OpenCV_DIR /usr/lib/x86_64-linux-gnu/cmake/opencv4)

#find_package(OpenCV REQUIRED core highgui)
find_package(OpenCV REQUIRED)
find_package( PkgConfig REQUIRED)
pkg_search_module( TESSERACT REQUIRED tesseract )
#pkg_search_module( LEPTONICA REQUIRED lept )

include_directories( ${OpenCV_INCLUDE_DIRS} )

# Make an automatic library - will be static or dynamic based on user setting
add_library(imgsolver ${MyCppSources} ${HEADER_LIST})

target_link_libraries( imgsolver solvercore)
target_link_libraries( imgsolver ${OpenCV_LIBS})
target_link_libraries( imgsolver ${TESSERACT_LIBRARIES} )
target_link_libraries( imgsolver ${LEPTONICA_LIBRARIES} )


# We need this directory, and users of our library will need it too
target_include_directories(imgsolver PUBLIC ../include)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

install(TARGETS imgsolver DESTINATION lib)
install(FILES ${HEADER_LIST} DESTINATION include/imgsolver)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)