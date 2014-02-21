set(SOURCE_FILES
    
)

set(HEADER_FILES
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

add_library(pagan_input STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_input PROPERTIES OUTPUT_NAME pagan_input CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_input
    pagan_core
    #${PLATFORM_LIBRARIES} what is this?
    ${Boost_LIBRARIES}
    ${PYSTRING_LIBRARY}
)



