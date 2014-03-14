file(GLOB_RECURSE COMPONENT_SRCS
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/components/*.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/components/*.h)

set(SOURCE_FILES
    app/rlapp.cpp
    path/Astar.cpp
    path/Astar-impl.cpp
    path/difi.cpp
    fov/fov_rsc.cpp
    
    #tile/tileinfo.cpp
    tile/layout.cpp
    
    event/events.cpp
    input/actionmap.cpp
    
    dungen/room.cpp
    dungen/area.cpp
    dungen/corr.cpp
    dungen/dungen.cpp
    dungen/constraints.cpp
    dungen/util.cpp
    
    ai/btree.cpp
    ai/behavior.cpp
    ai/actions.cpp
    ai/conditions.cpp
)

set(HEADER_FILES
    app/rlapp.h
    path/Astar.h
    path/difi.h
    utils/shape/ShapeCalc.h
    fov/FovLookup.h
    fov/fov_rsc.h
    
    dungen/room.h
    dungen/area.h
    dungen/corr.h
    dungen/dungen.h
    dungen/constraints.h
    dungen/util.h
    
    #tile/tileinfo.h
    tile/layout.h
    input/actionmap.h
    
    event/events.h
    
    ai/btree.h
    ai/behavior.h
    ai/blackboard.h
    ai/actions.h
    ai/conditions.h
    
    ${COMPONENT_SRCS}
)

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
)

source_group("Sources" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})
source_group("_Components" FILES ${COMPONENT_SRCS})


include_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PYSTRING_INCLUDE_DIR}
    ${pugixml_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${glm_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${SDL_image_INCLUDE_DIR}
    ${SDL_ttf_INCLUDE_DIR}
    ${Signal_INCLUDE_DIR}
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
