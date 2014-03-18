file(GLOB_RECURSE SYSTEM_SRCS
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/systems/*.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/systems/*.h)

set(SOURCE_FILES
    ecs.cpp
    ComponentBase.cpp
    EntityData.cpp
    serialize_ecs.cpp
    ecs-config.cpp
    system.cpp
    componentquery.cpp
    systems/InputKey.cpp
)

set(HEADER_FILES
    ecs.h
    ecs-config.h
    Component.h
    ComponentBase.h
    EntityData.h
    system.h
    componentquery.h
    events.h
    systems/InputKey.h
    
    ${SYSTEM_SRCS}
)

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
)

source_group("Sources" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})
source_group("_Systems" FILES ${SYSTEM_SRCS})

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
