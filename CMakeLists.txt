cmake_minimum_required(VERSION 3.15)

project(FCOLLADA)

add_definitions(-DFCOLLADA_INTERNAL)

add_subdirectory(FCollada)
add_subdirectory(FColladaPlugins)

install(DIRECTORY "${FCOLLADA_SOURCE_DIR}/FCollada/"
	TYPE INCLUDE
	FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
	EXCLUDE PATTERN "StdAfx.h"
)
install(TARGETS fcollada ARCHIVE LIBRARY RUNTIME)
