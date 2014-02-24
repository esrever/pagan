set(SOURCE_FILES
    room.cpp
    area.cpp
    corr.cpp
    dungen.cpp
    constraints.cpp
    util.cpp
)

set(HEADER_FILES
    room.h
    area.h
    corr.h
    dungen.h
    constraints.h
    util.h
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

add_library(pagan_dungen STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_dungen PROPERTIES OUTPUT_NAME pagan_dungen CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_dungen
    pagan_core
    #${PLATFORM_LIBRARIES} what is this?
    ${Boost_LIBRARIES}
    ${PYSTRING_LIBRARY}
)



