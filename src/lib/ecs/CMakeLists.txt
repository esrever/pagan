set(SOURCE_FILES
    ComponentBase.cpp
    SystemBase.cpp
    Entity.cpp
    EntityMgr.cpp
    ComponentQuery.cpp
    ComponentBase.cpp
    EntityComponents.cpp
)

set(HEADER_FILES
    SystemBase.h
    Entity.h
    Event.h
    EntityMgr.h
    ecs_config.h
    ComponentBase.h
    #ComponentMgr.h
    ComponentQuery.h
    EntityComponents.h
)

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
    -D_SCL_SECURE_NO_WARNINGS
)

source_group("Sources" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})

include_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PYSTRING_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${Rapidjson_INCLUDE_DIR}
    ${Signal_INCLUDE_DIR}
)

add_library(pagan_ecs STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_ecs PROPERTIES OUTPUT_NAME pagan_ecs CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_ecs
    ${PLATFORM_LIBRARIES}
    ${Boost_LIBRARIES}
)



