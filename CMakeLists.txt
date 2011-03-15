SET(TARGET "OMKFCollada")

# "DLL" is not only used on Win32 platforms!
ADD_DEFINITIONS("-DFCOLLADA_DLL")
# I'm beggining to take drugs...
ADD_DEFINITIONS("-DFCOLLADA_INTERNAL")
# If you need to link statically libxml2 with OMKFCollada, uncomment the
# following line
# (if you try to use it with a dynamic linking, you may experience problems
# with the _xmlfree symbol not resolved)
#ADD_DEFINITIONS("-DLIBXML_STATIC_LINKING")

IF(UNIX)
  IF(CMAKE_COMPILER_IS_GNUCXX)
    # Workarounds to avoid removing of unused functions...
    EXECUTE_PROCESS(COMMAND "g++" "-dumpversion"
                    OUTPUT_VARIABLE GNUCXX_VERSION
                    RESULT_VARIABLE GNUCXX_UNFOUND)

    IF(GNUCXX_UNFOUND)
        MESSAGE(FATAL_ERROR "g++ cannot be found!")
    ENDIF(GNUCXX_UNFOUND)

    # We want to be compatible with CMake 2.4 but previous to version 2.6,
    # CMake does not support VERSION_LESS. So, Cmake version checking is
    # required and STRLESS will be used. STRLESS can miscompare strings with
    # versions!
    IF(${CMAKE_MAJOR_VERSION} STREQUAL "2" AND ${CMAKE_MINOR_VERSION} STRGREATER "4")
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
    ELSE(${CMAKE_MAJOR_VERSION} STREQUAL "2" AND ${CMAKE_MINOR_VERSION} STRGREATER "4")
        IF(${GNUCXX_VERSION} STRLESS "4.3")
            # With older versions of g++, flag "-fno-inline-functions" have to be used
            SET(CMAKE_CXX_FLAGS_RELEASE "-fno-inline-functions ${CMAKE_CXX_FLAGS_RELEASE}")
        ELSEIF(${GNUCXX_VERSION} STREQUAL "4.3" OR ${GNUCXX_VERSION} STRGREATER "4.3")
            IF (${GNUCXX_VERSION} STRLESS "4.4")
                # With g++ at version 4.3, flag "-fno-inline-small-functions" have to be used
                SET(CMAKE_CXX_FLAGS_RELEASE "-fno-inline-small-functions ${CMAKE_CXX_FLAGS_RELEASE}")
            ELSE(${GNUCXX_VERSION} STRLESS "4.4")
                #With g++ at version 4.4, there is probably a flag more relevant... but let us use -O1
                SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O1")
            ENDIF(${GNUCXX_VERSION} STRLESS "4.4")
        ENDIF(${GNUCXX_VERSION} STRLESS "4.3")
    ENDIF(${CMAKE_MAJOR_VERSION} STREQUAL "2" AND ${CMAKE_MINOR_VERSION} STRGREATER "4")

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

MESSAGE(STATUS ">>>>>> Project: ${PROJECT_NAME}")
MESSAGE(STATUS ">>> Files: ${CPPFILES}")

INCLUDE_DIRECTORIES(
  ${LIBXML2_INC}
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada
  ${CMAKE_CURRENT_SOURCE_DIR}/FColladaPlugins
  ${CMAKE_CURRENT_SOURCE_DIR}/FColladaPlugins/FArchiveXML
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FCDocument
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FMath
  ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FUtils
)

IF(_WIN32)
    SOURCE_GROUP( FColladaPlugins REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FColladaPlugins/FArchiveXML/* )
    SOURCE_GROUP( FCDocument      REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FCollada/FCDocument/* )
    SOURCE_GROUP( FMath           REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FCollada/FMath/* )
    SOURCE_GROUP( FUtils          REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/FCollada/FCollada/FUtils/* )

    # not needed on Unix because we find libraries ourselves
    LINK_DIRECTORIES(
      ${LIBXML2_LIBDIR}
    )
ENDIF(_WIN32)

ADD_LIBRARY(${TARGET} SHARED ${CPPFILES})

#link with external libs
IF(_WIN32)
  TARGET_LINK_LIBRARIES(${TARGET} ${LIBXML2_LIBS})

  IF(MSVC71)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_vc71_d" RELEASE_POSTFIX "_vc71")
  ELSEIF(MSVC80)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_vc80_d" RELEASE_POSTFIX "_vc80")
  ELSEIF(MSVC90)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_vc90_d" RELEASE_POSTFIX "_vc90")
  ELSE(MSVC71)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
  ENDIF(MSVC71)
ELSE(_WIN32)
  MACRO(FIND_AND_LINK_LIBRARIES LIB_TARGET LIBS LIBDIR)
    FOREACH(lib ${LIBS})
      # try to find the library only in the paths provided
      FIND_LIBRARY(var${lib}
        NAMES ${lib}
        PATHS ${LIBDIR}
        NO_DEFAULT_PATH )
      IF(EXISTS ${var${lib}})
        TARGET_LINK_LIBRARIES(${LIB_TARGET} ${var${lib}})
      ELSE(EXISTS ${var${lib}})
        # try one more time but with a wider scope to find the library
        FIND_LIBRARY(var${lib}
          NAMES ${lib}
          PATHS ${LIBDIR} )
        IF(EXISTS ${var${lib}})
          TARGET_LINK_LIBRARIES(${LIB_TARGET} ${var${lib}})
        ELSE(EXISTS ${var${lib}})
          MESSAGE(FATAL_ERROR "ERROR: could not find ${lib} on path: ${LIBDIR}")
        ENDIF(EXISTS ${var${lib}})
      ENDIF(EXISTS ${var${lib}})
    ENDFOREACH(lib)
  ENDMACRO(FIND_AND_LINK_LIBRARIES LIB_TARGET LIBS LIBDIR)

  # libxml2
  FIND_AND_LINK_LIBRARIES("${TARGET}" "${LIBXML2_LIBS}" "${LIBXML2_LIBDIR}")
ENDIF(_WIN32)

# the following code is only supported by CMake versions above 2.4
IF(${CMAKE_MAJOR_VERSION} STREQUAL "2" AND ${CMAKE_MINOR_VERSION} STRGREATER "4")
  # Avoid transitive linking of dynamic libraries.
  # If B depends on A, C on B, then C will only depend on B (not A).
  # This setting doesn't have any effect on static libraries
  SET_TARGET_PROPERTIES(${MODULE_NAME}
      PROPERTIES
      LINK_INTERFACE_LIBRARIES ""
  )
ENDIF(${CMAKE_MAJOR_VERSION} STREQUAL "2" AND ${CMAKE_MINOR_VERSION} STRGREATER "4")

SET_TARGET_PROPERTIES(${TARGET} PROPERTIES VERSION ${PROJECT_VERSION_FULL})

FILE(GLOB H_ROOT_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FCollada/*.h FCollada/*.hpp)
FILE(GLOB H_FARCHIVEXML_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FColladaPlugins/FArchiveXML/*.h FColladaPlugins/FArchiveXML/*.hpp)
FILE(GLOB H_FCDOCUMENT_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FCollada/FCDocument/*.h FCollada/FCDocument/*.hpp)
FILE(GLOB H_FMATH_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FCollada/FMath/*.h FCollada/FCDocument/*.hpp)
FILE(GLOB H_FUTILS_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FCollada/FUtils/*.h FCollada/FUtils/*.hpp)

IF(UNIX)
  #install headers
  SET(OMKFCOLLADA_INC_DIR ${INSTALLPREFIX}/include/OMKFCollada)
  #for root headers
  FOREACH(file ${H_ROOT_FILES} )
    INSTALL(FILES ${file} DESTINATION ${OMKFCOLLADA_INC_DIR})
  ENDFOREACH(file)
  #for FArchiveXML
  FOREACH(file ${H_FARCHIVEXML_FILES} )
    INSTALL(FILES ${file} DESTINATION ${OMKFCOLLADA_INC_DIR}/FArchiveXML/)
  ENDFOREACH(file)
  #for FCDocument
  FOREACH(file ${H_FCDOCUMENT_FILES} )
    INSTALL(FILES ${file} DESTINATION ${OMKFCOLLADA_INC_DIR}/FCDocument/)
  ENDFOREACH(file)
  #for FMath
  FOREACH(file ${H_FMATH_FILES} )
    INSTALL(FILES ${file} DESTINATION ${OMKFCOLLADA_INC_DIR}/FMath/)
  ENDFOREACH(file)
  #for FUtils
  FOREACH(file ${H_FUTILS_FILES} )
    INSTALL(FILES ${file} DESTINATION ${OMKFCOLLADA_INC_DIR}/FUtils/)
  ENDFOREACH(file)

  #install lib
  INSTALL(TARGETS ${TARGET} LIBRARY DESTINATION ${INSTALLPREFIX}/lib/)

ELSE(UNIX)
  #install headers
  SET(OMKFCOLLADA_INC_DIR include/OMKFCollada)
  INSTALL(FILES ${H_ROOT_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR})
  INSTALL(FILES ${H_FARCHIVEXML_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR}/FArchiveXML)
  INSTALL(FILES ${H_FCDOCUMENT_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR}/FCDocument)
  INSTALL(FILES ${H_FMATH_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR}/FMath)
  INSTALL(FILES ${H_FUTILS_FILES} DESTINATION ${OMKFCOLLADA_INC_DIR}/FUtils)
    
  #install lib
  INSTALL(TARGETS ${TARGET} DESTINATION lib)

ENDIF(UNIX)

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
