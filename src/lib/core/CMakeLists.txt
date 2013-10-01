set(SOURCE_FILES
    util/cmdopt.cpp
    util/logstream.cpp
    util/cfgtree.cpp
    util/cfgdata.cpp
    util/conversions.cpp
    util/json_conversions.cpp
    app/application.cpp
)

set(HEADER_FILES
    util/cmdopt.h
    util/logstream.h
    util/cfgtree.h
    util/conversions.h
    util/json_conversions.h
    app/application.h
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
)

add_library(pagan_core STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_core PROPERTIES OUTPUT_NAME pagan_core CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_core
    ${PLATFORM_LIBRARIES}
    ${Boost_LIBRARIES}
)



