cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(FCOLLADA)

if(BUILD_SHARED_LIBS)
	add_definitions(-DFCOLLADA_DLL)
endif(BUILD_SHARED_LIBS)

add_definitions(-DFCOLLADA_INTERNAL)

add_subdirectory(FCollada)
add_subdirectory(FColladaPlugins)

