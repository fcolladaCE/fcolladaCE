cmake_minimum_required(VERSION 2.6)

project(FCollada)

SET(TARGET "OMKFCollada")

option(BUILD_SHARED_LIBS "build a shared library" OFF)
if(BUILD_SHARED_LIBS)
	# "DLL" is not only used on Win32 platforms!
	ADD_DEFINITIONS(-DFCOLLADA_DLL)
endif(BUILD_SHARED_LIBS)

# Let the sources know, that we're building FCollada now
ADD_DEFINITIONS(-DFCOLLADA_INTERNAL)

# Backwards compatibility with the old non-standard parameters
if(DEFINED LIBXML2_INC)
	set(LIBXML2_INCLUDE_DIR ${LIBXML2_INC})
endif(DEFINED LIBXML2_INC)

if(DEFINED LIBXML2_LIBS AND DEFINED LIBXML2_LIBDIR)
	foreach(libName ${LIBXML2_LIBS})
		find_library(libxml2_${libName} NAMES ${libName} PATHS ${LIBXML2_LIBDIR})
		if(libxml2_${libName})
			list(APPEND LIBXML2_LIBRARIES ${libxml2_${libName}})
		endif(libxml2_${libName})
	endforeach(libName ${LIBXML2_LIBS})
endif(DEFINED LIBXML2_LIBS AND DEFINED LIBXML2_LIBDIR)

find_package(LibXml2 REQUIRED)

option(LIBXML_STATIC_LINKING "link libxml2 statically" OFF)
if(LIBXML_STATIC_LINKING)
	# (if you try to use it with a dynamic linking, you may experience problems
	# with the _xmlfree symbol not resolved)
	list(APPEND LIBXML2_DEFINITIONS LIBXML_STATIC_LINKING)
endif(LIBXML_STATIC_LINKING)

mark_as_advanced(LIBXML_STATIC_LINKING)


IF(UNIX)
  IF(CMAKE_COMPILER_IS_GNUCXX)
    # Workarounds to avoid removing of unused functions...
    EXECUTE_PROCESS(COMMAND "g++" "-dumpversion"
                    OUTPUT_VARIABLE GNUCXX_VERSION
                    RESULT_VARIABLE GNUCXX_UNFOUND)

    IF(GNUCXX_UNFOUND)
        MESSAGE(FATAL_ERROR "g++ cannot be found!")
    ENDIF(GNUCXX_UNFOUND)

    IF(${GNUCXX_VERSION} VERSION_LESS "4.3")
        # With older versions of g++, flag "-fno-inline-functions" have to be used
        SET(CMAKE_CXX_FLAGS_RELEASE "-fno-inline-functions ${CMAKE_CXX_FLAGS_RELEASE}")
    ELSEIF(${GNUCXX_VERSION} VERSION_LESS "4.4")
        # With g++ at version 4.3, flag "-fno-inline-small-functions" have to be used
        SET(CMAKE_CXX_FLAGS_RELEASE "-fno-inline-small-functions ${CMAKE_CXX_FLAGS_RELEASE}")
    ELSEIF(${GNUCXX_VERSION} VERSION_EQUAL "4.4" OR ${GNUCXX_VERSION} VERSION_GREATER "4.4")
        #With g++ at version 4.4, there is probably a flag more relevant... but let us use -O1
        SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O1")
    ENDIF(${GNUCXX_VERSION} VERSION_LESS "4.3")

  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDIF(UNIX)

SET(MODULE_FOLDERS
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada
  ${CMAKE_CURRENT_SOURCE_DIR}/FColladaPlugins/FArchiveXML
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FCDocument
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FMath
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FUtils
)

# Get the header files
FOREACH(folder ${MODULE_FOLDERS})
  FILE(GLOB tmpHFILES
    ${folder}/*.h
    ${folder}/*.hpp
  )
  SET(H_FILES ${H_FILES} ${tmpHFILES})
ENDFOREACH(folder)

# Get the source files
FOREACH(folder ${MODULE_FOLDERS})
  FILE(GLOB tmpCPPFILES 
    ${folder}/*.cpp
    ${folder}/*.c
  )
  SET(CPPFILES ${CPPFILES} ${tmpCPPFILES})
ENDFOREACH(folder)
SET(CPPFILES ${H_FILES} ${CPPFILES})

INCLUDE_DIRECTORIES(
  ${LIBXML2_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada
  ${CMAKE_CURRENT_SOURCE_DIR}/FColladaPlugins
  ${CMAKE_CURRENT_SOURCE_DIR}/FColladaPlugins/FArchiveXML
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FCDocument
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FMath
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FUtils
)

SOURCE_GROUP(FColladaPlugins REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FColladaPlugins/FArchiveXML/* )
SOURCE_GROUP(FCDocument      REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FCollada/FCDocument/* )
SOURCE_GROUP(FMath           REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FCollada/FMath/* )
SOURCE_GROUP(FUtils          REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FCollada/FUtils/* )

ADD_LIBRARY(${TARGET} ${CPPFILES})

#link with external libs
TARGET_LINK_LIBRARIES(${TARGET} ${LIBXML2_LIBRARIES})

IF(WIN32 AND NOT DEFINED CMAKE_DEBUG_POSTFIX)
  IF(MSVC71)
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES DEBUG_POSTFIX "_vc71_d" RELEASE_POSTFIX "_vc71")
  ELSEIF(MSVC80)
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES DEBUG_POSTFIX "_vc80_d" RELEASE_POSTFIX "_vc80")
  ELSEIF(MSVC90)
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES DEBUG_POSTFIX "_vc90_d" RELEASE_POSTFIX "_vc90")
  ELSE(MSVC71)
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES DEBUG_POSTFIX "_d")
  ENDIF(MSVC71)
ENDIF(WIN32 AND NOT DEFINED CMAKE_DEBUG_POSTFIX)

# Avoid transitive linking of dynamic libraries.
# If B depends on A, C on B, then C will only depend on B (not A).
# This setting doesn't have any effect on static libraries
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_INTERFACE_LIBRARIES "")

if(DEFINED PROJECT_VERSION_FULL)
	SET_TARGET_PROPERTIES(${TARGET} PROPERTIES VERSION ${PROJECT_VERSION_FULL})
endif(DEFINED PROJECT_VERSION_FULL)

FILE(GLOB H_ROOT_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FCollada/*.h FCollada/*.hpp)
FILE(GLOB H_FARCHIVEXML_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FColladaPlugins/FArchiveXML/*.h FColladaPlugins/FArchiveXML/*.hpp)
FILE(GLOB H_FCDOCUMENT_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FCollada/FCDocument/*.h FCollada/FCDocument/*.hpp)
FILE(GLOB H_FMATH_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FCollada/FMath/*.h FCollada/FCDocument/*.hpp)
FILE(GLOB H_FUTILS_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FCollada/FUtils/*.h FCollada/FUtils/*.hpp)

IF(UNIX AND DEFINED INSTALLPREFIX)
  SET(OMKFCOLLADA_INC_DIR ${INSTALLPREFIX}/include/OMKFCollada)
	set(OMKFCOLLADA_LIB_DIR ${INSTALLPREFIX}/lib)
	set(OMKFCOLLADA_BIN_DIR ${INSTALLPREFIX}/bin)
ELSE(UNIX AND DEFINED INSTALLPREFIX)
  SET(OMKFCOLLADA_INC_DIR include/OMKFCollada)
	set(OMKFCOLLADA_LIB_DIR lib)
	set(OMKFCOLLADA_BIN_DIR bin)
ENDIF(UNIX AND DEFINED INSTALLPREFIX)

#install headers
INSTALL(FILES ${H_ROOT_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR})
INSTALL(FILES ${H_FARCHIVEXML_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR}/FArchiveXML)
INSTALL(FILES ${H_FCDOCUMENT_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR}/FCDocument)
INSTALL(FILES ${H_FMATH_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR}/FMath)
INSTALL(FILES ${H_FUTILS_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR}/FUtils)
    
#install lib
INSTALL(TARGETS ${TARGET}
	ARCHIVE DESTINATION ${OMKFCOLLADA_LIB_DIR}
	LIBRARY DESTINATION ${OMKFCOLLADA_LIB_DIR}
	RUNTIME DESTINATION ${OMKFCOLLADA_BIN_DIR}
)

#generate OMKFcollada.pc which will move at install
IF(UNIX)
  FILE(WRITE ${INSTALL_LIB_BUILD}/OMKFCollada.pc
    "prefix=${INSTALLPREFIX}\n"
    "exec_prefix=\${prefix}\n"
    "libdir=\${prefix}/lib\n"
    "bindir=\${prefix}/bin\n"
    "includedir=\${prefix}/include/OMKFCollada\n"
  )

  IF(CMAKE_BUILD_TYPE MATCHES Debug)
    FILE(APPEND ${INSTALL_LIB_BUILD}/OMKFCollada.pc "buildtype=Debug")
  ELSE(CMAKE_BUILD_TYPE MATCHES Debug)
    FILE(APPEND ${INSTALL_LIB_BUILD}/OMKFCollada.pc "buildtype=Release")
  ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

  FILE(APPEND ${INSTALL_LIB_BUILD}/OMKFCollada.pc
    "\n\n"
    "Name: OMKFCollada\n"
    "Description: FCollada for OpenMASK\n"
    "Version: ${PROJECT_VERSION_FULL}\n"
    "Libs: -L\${libdir} -lOMKFCollada\n"
    "Requires.private: libxml-2.0 >= 2.6.29\n"
    "Cflags: -I\${includedir}\n"
  )
  INSTALL(FILES ${INSTALL_LIB_BUILD}/OMKFCollada.pc
    DESTINATION ${INSTALLPREFIX}/lib/pkgconfig
  )
ENDIF(UNIX)
