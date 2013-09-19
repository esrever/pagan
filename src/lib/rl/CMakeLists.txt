set(SOURCE_FILES
    asciirep.cpp
    GameEntity.cpp
    tilelib.cpp
)

set(HEADER_FILES
    asciirep.h
    GameEntity.h
    tilelib.h
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



