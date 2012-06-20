include_directories(${FCOLLADA_SOURCE_DIR}/FCollada
                    ${FCOLLADA_BINARY_DIR})

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

add_library(farchivexml SHARED ${FARCHIVEXML_SOURCES})
