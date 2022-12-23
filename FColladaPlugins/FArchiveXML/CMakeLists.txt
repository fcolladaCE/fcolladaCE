find_package(LibXml2 REQUIRED)

include_directories(${FCOLLADA_SOURCE_DIR}/FCollada ${LibXml2_INCLUDE_DIR})

set(FARCHIVEXML_SOURCES
	FAXGeometryExport.cpp
	FAXPhysicsImport.cpp
	FAXAnimationImport.cpp
	FAXCameraImport.cpp
	FAXSceneImport.cpp
	FAXColladaParser.cpp
	FAXCameraExport.cpp
	FAXEntityExport.cpp
	FAXEmitterExport.cpp
	FAXControllerImport.cpp
	FAXControllerExport.cpp
	FAXSceneExport.cpp
	FAXImportLinking.cpp
	StdAfx.cpp
	FAXInstanceImport.cpp
	FAXPhysicsExport.cpp
	FAXAnimationExport.cpp
	FAXForceFieldImport.cpp
	FAXGeometryImport.cpp
	FAXColladaWriter.cpp
	FAXMaterialExport.cpp
	FAXMaterialImport.cpp
	FArchiveXML.cpp
	FAXLightImport.cpp
	FAXLightExport.cpp
	FAXEntityImport.cpp
	FAXForceFieldExport.cpp
	FAXInstanceExport.cpp
	FAXEmitterImport.cpp
)

add_library(farchivexml STATIC ${FARCHIVEXML_SOURCES})

target_link_libraries(fcollada LibXml2::LibXml2)
