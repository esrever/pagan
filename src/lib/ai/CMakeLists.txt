set(SOURCE_FILES
    path/Astar.cpp
)

set(HEADER_FILES
    path/Astar.h
    utils/shape/ShapeCalc.h
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

add_library(pagan_ai STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_ai PROPERTIES OUTPUT_NAME pagan_ai CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_ai
    pagan_core
    #${PLATFORM_LIBRARIES} what is this?
    ${Boost_LIBRARIES}
    ${PYSTRING_LIBRARY}
)



