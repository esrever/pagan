set(SOURCE_FILES
    path/Astar.cpp
    fov/fov0.cpp
    fov/fov1.cpp
    
    dungen/room.cpp
    dungen/area.cpp
    dungen/corr.cpp
    dungen/dungen.cpp
    dungen/constraints.cpp
    dungen/util.cpp
)

set(HEADER_FILES
    path/Astar.h
    utils/shape/ShapeCalc.h
    fov/FovLookup.h
    fov/fov0.h
    fov/fov1.h
    
    dungen/room.h
    dungen/area.h
    dungen/corr.h
    dungen/dungen.h
    dungen/constraints.h
    dungen/util.h
)

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
)

source_group("Sources" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})

include_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PYSTRING_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${glm_INCLUDE_DIR}
)

add_library(pagan_rlut STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_rlut PROPERTIES OUTPUT_NAME pagan_rlut CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_rlut
    pagan_core
    #${PLATFORM_LIBRARIES} what is this?
    ${Boost_LIBRARIES}
    ${PYSTRING_LIBRARY}
)



