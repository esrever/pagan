set(SOURCE_FILES
    ComponentBase.cpp
    SystemBase.cpp
    #Entity.cpp
    Event.cpp
    ecs_config.cpp
    EntityMgr.cpp
    #ComponentQuery.cpp
    ComponentBase.cpp
    EntityComponents.cpp
    ecs.cpp
    ExampleComponent.cpp
    ExampleSystem.cpp
    SystemMgr.cpp
    ComponentHelpers.cpp
    #TagQuery.cpp
    #QueryBase.cpp
    QueryExpression.cpp
    ActionMgr.cpp
)

set(HEADER_FILES
    Archetype.h
    Exemplar.h
    Component.h
    SystemBase.h
    Entity.h
    Event.h
    EntityMgr.h
    ecs_config.h
    ComponentBase.h
    #ComponentQuery.h
    EntityComponents.h
    ecs.h
    ExampleComponent.h
    ExampleSystem.h
    SystemMgr.h
    ActionMgr.h
    ComponentHelpers.h
    #TagQuery.h
    #QueryBase.h
    QueryExpression.h
    #EventHandlerQuery.h
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
    ${glm_INCLUDE_DIR}
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



