project(FCOLLADA)

cmake_minimum_required(VERSION 2.4)
cmake_policy(VERSION 2.4)

find_package(LibXml2 REQUIRED)

include_directories(${LIBXML2_INCLUDE_DIR})
add_definitions(-DFCOLLADA_DLL)

add_subdirectory(FCollada)
add_subdirectory(FColladaPlugins)

