cmake_minimum_required (VERSION 3.0)

# CMP0048 required so that we can specify the version of our project
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(UsdFractalTrees VERSION 0.1.0)
set (PROJECT_NAME ${CMAKE_PROJECT_NAME})
set (${PROJECT_NAME}_VERSION_MAJOR 0)
set (${PROJECT_NAME}_VERSION_MINOR 1)
set (${PROJECT_NAME}_VERSION_PATCH 0)

 # FIXME: you will need to update the hard-coded paths to your USD, boost and Python include directories
set(USD_LIB_DIR "d:/git/3rdparty/USD/lib")
set(PYTHON_LIB_DIR "d:/Python27/libs")
set(USD_INCLUDE_DIR "d:/git/3rdparty/USD/include")
set(BOOST_INCLUDE_DIR "d:/git/3rdparty/USD/include/boost-1_61")
set(PYTHON_INCLUDE_DIR "d:/Python27/include")

if (WIN32)
	add_compile_definitions(_WIN64=1)
	add_compile_definitions(NOMINMAX)
	add_compile_definitions(TBB_USE_THREADING_TOOLS=1)
	add_compile_definitions(TBB_USE_ASSERT=0)
endif()

# create a header file that contains our version info
configure_file (
  "${PROJECT_SOURCE_DIR}/src/Version.h.in"
  "${PROJECT_BINARY_DIR}/Version.h"
  )
  
 # add the binary tree to the search path for include files so that we will find Version.h
include_directories(
	${PROJECT_BINARY_DIR}
	${USD_INCLUDE_DIR}
	${BOOST_INCLUDE_DIR}
	${PYTHON_INCLUDE_DIR}
	)
	
link_directories(
	${USD_LIB_DIR}
	${PYTHON_LIB_DIR}
)
  
set (${PROJECT_NAME}_SOURCES
	src/main.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME} usd sdf tf)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin)

# install the various binaries needed for the application bundle
install(FILES 
	${USD_LIB_DIR}/libusd.dll
	${USD_LIB_DIR}/libarch.dll
	${USD_LIB_DIR}/libar.dll
	${USD_LIB_DIR}/libgf.dll
	${USD_LIB_DIR}/libjs.dll
	${USD_LIB_DIR}/libkind.dll
	${USD_LIB_DIR}/libpcp.dll
	${USD_LIB_DIR}/libplug.dll
	${USD_LIB_DIR}/libsdf.dll
	${USD_LIB_DIR}/libtf.dll
	${USD_LIB_DIR}/libtrace.dll
	${USD_LIB_DIR}/libvt.dll
	${USD_LIB_DIR}/libwork.dll
	${USD_LIB_DIR}/boost_python-vc140-mt-1_61.dll
	DESTINATION ${CMAKE_BINARY_DIR}/bin)

