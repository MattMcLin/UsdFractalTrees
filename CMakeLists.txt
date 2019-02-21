cmake_minimum_required (VERSION 3.0)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(UsdFractalTrees VERSION 1.0.0)
set (PROJECT_NAME ${CMAKE_PROJECT_NAME})
# The version number.
set (${PROJECT_NAME}_VERSION_MAJOR 0)
set (${PROJECT_NAME}_VERSION_MINOR 1)
set (${PROJECT_NAME}_VERSION_PATCH 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/Version.h.in"
  "${PROJECT_BINARY_DIR}/Version.h"
  )
  
 # add the binary tree to the search path for include files
# so that we will find Version.h
include_directories("${PROJECT_BINARY_DIR}")
  
set (${PROJECT_NAME}_SOURCES
	src/main.cpp
)

project (UsdFractalTrees)
add_executable(UsdFractalTrees ${${PROJECT_NAME}_SOURCES})

install (FILES "${PROJECT_BINARY_DIR}/Version.h"        
         DESTINATION include)
