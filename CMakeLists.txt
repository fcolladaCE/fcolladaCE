cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(FCOLLADA)

find_package(LibXml2 REQUIRED)

include_directories(${LIBXML2_INCLUDE_DIR})
add_definitions(-DFCOLLADA_DLL)

add_subdirectory(FCollada)
add_subdirectory(FColladaPlugins)

