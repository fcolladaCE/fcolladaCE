cmake_minimum_required(VERSION 3.15)

project(FCOLLADA)

option(FCOLLADA_UNICODE "Build with unicode support" OFF)

if(FCOLLADA_UNICODE)
	add_definitions(-DUNICODE)
endif()

add_definitions(-DFCOLLADA_INTERNAL)

add_subdirectory(FCollada)
add_subdirectory(FColladaPlugins)

install(DIRECTORY "${FCOLLADA_SOURCE_DIR}/FCollada/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h"
	PATTERN "*.hpp"
	PATTERN "StdAfx.h" EXCLUDE
)
install(TARGETS fcollada ARCHIVE LIBRARY RUNTIME)

if(NOT BUILD_SHARED_LIBS)
	install(TARGETS farchivexml ARCHIVE LIBRARY RUNTIME)
endif()
