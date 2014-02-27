set(SOURCE_FILES
    src/ecs.cpp
    src/ComponentBase.cpp
    src/EntityData.cpp
)

set(HEADER_FILES
    src/ecs.h
    src/ecs-config.h
    src/Component.h
    src/ComponentBase.h
    src/EntityData.h
    
)

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
)

source_group("Sources" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})

include_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PYSTRING_INCLUDE_DIR}
    ${pugixml_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${glm_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${SDL_image_INCLUDE_DIR}
    ${SDL_ttf_INCLUDE_DIR}
)

add_library(pagan_ecs STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_ecs PROPERTIES OUTPUT_NAME pagan_ecs CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_ecs
    pagan_core
    #${PLATFORM_LIBRARIES} what is this?
    #${Boost_LIBRARIES}
    #${PYSTRING_LIBRARY}
)
