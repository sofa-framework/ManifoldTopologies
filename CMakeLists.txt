cmake_minimum_required(VERSION 3.12)
project(ManifoldTopologies VERSION 0.1)

sofa_find_package(Sofa.Component.Topology.Container REQUIRED)

set(MANIFOLDTOPOLOGIES_SRC "src/ManifoldTopologies")

set(HEADER_FILES
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldEdgeSetGeometryAlgorithms.h
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldEdgeSetGeometryAlgorithms.inl
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldEdgeSetTopologyContainer.h
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldEdgeSetTopologyModifier.h
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTetrahedronSetTopologyContainer.h
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTopologyObject_double.h
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTopologyObject_float.h
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTriangleSetGeometryAlgorithms.h
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTriangleSetGeometryAlgorithms.inl
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTriangleSetTopologyContainer.h
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTriangleSetTopologyModifier.h
    ${MANIFOLDTOPOLOGIES_SRC}/config.h
)

set(SOURCE_FILES
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldEdgeSetGeometryAlgorithms.cpp
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldEdgeSetTopologyContainer.cpp
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldEdgeSetTopologyModifier.cpp
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTetrahedronSetTopologyContainer.cpp
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTriangleSetGeometryAlgorithms.cpp
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTriangleSetTopologyContainer.cpp
    ${MANIFOLDTOPOLOGIES_SRC}/ManifoldTriangleSetTopologyModifier.cpp
    ${MANIFOLDTOPOLOGIES_SRC}/initManifoldTopologies.cpp
)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Sofa.Component.Topology.Container)

sofa_create_package_with_targets(
    PACKAGE_NAME ${PROJECT_NAME}
    PACKAGE_VERSION ${PROJECT_VERSION}
    TARGETS ${PROJECT_NAME} AUTO_SET_TARGET_PROPERTIES
    INCLUDE_SOURCE_DIR "src"
    RELOCATABLE "plugins"
    )

# Organize projects into folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "plugins")
