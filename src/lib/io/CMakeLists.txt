set(SOURCE_FILES
    image/utils.cpp
)

set(HEADER_FILES
    image/utils.h
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
    ${SDL_INCLUDE_DIR}
    ${SDL_image_INCLUDE_DIR}
)

add_library(pagan_io STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_io PROPERTIES OUTPUT_NAME pagan_io CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_io
    pagan_core
    #${PLATFORM_LIBRARIES} what is this?
    ${Boost_LIBRARIES}
    ${PYSTRING_LIBRARY}
)



