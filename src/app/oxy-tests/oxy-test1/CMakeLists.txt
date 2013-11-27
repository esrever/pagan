set(SOURCE_FILES
    src/entry_point.cpp
    src/example.cpp
)

set(HEADER_FILES
    src/example.h
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
    ${Curses_INCLUDE_DIR}
    ${glm_INCLUDE_DIR}
    ${OXYGINE_INCLUDE_DIRS}
)

#link_directories(${OXYGINE_LIBRARY_DIRS})
#target_link_libraries(HelloWorld ${OXYGINE_CORE_LIBS})

add_executable (oxy-test1 
            ${SOURCE_FILES}
            ${HEADER_FILES}
        ) 

#set_target_properties(CMakeTest PROPERTIES OUTPUT_NAME pagan_core CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(oxy-test1
    pagan_core
    pagan_ecs
    pagan_rl
    ${Boost_LIBRARIES}
)

target_link_libraries(oxy-test1
    debug
    ${PYSTRING_LIBRARY_DBG}
    optimized
    ${PYSTRING_LIBRARY}
    ${OXYGINE_CORE_LIBS}
)