FILE(GLOB components_CPP "components/*.cpp")
FILE(GLOB components_H "components/*.h")

set(SOURCE_FILES
    EntityMgrRL.cpp
    ${components_CPP}
)




set(HEADER_FILES
    EntityMgrRL.h
    ${components_H}
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
    ${Rapidjson_INCLUDE_DIR}
    ${Curses_INCLUDE_DIR}
    ${Signal_INCLUDE_DIR}
    ${glm_INCLUDE_DIR}
)

add_library(pagan_rl STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_rl PROPERTIES OUTPUT_NAME pagan_rl CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_rl
    ${PLATFORM_LIBRARIES}
    ${Boost_LIBRARIES}
)



