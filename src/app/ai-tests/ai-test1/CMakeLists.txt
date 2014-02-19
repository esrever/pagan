set(SOURCE_FILES
    src/main.cpp
)

set(HEADER_FILES
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
    ${glm_INCLUDE_DIR}
    #${Rapidjson_INCLUDE_DIR}
    #${Signal_INCLUDE_DIR}
    #${Curses_INCLUDE_DIR}
    #${glm_INCLUDE_DIR}
    #${pugixml_INCLUDE_DIR}
    #${OXYGINE_INCLUDE_DIRS}
)

#link_directories(${OXYGINE_LIBRARY_DIRS})
#target_link_libraries(HelloWorld ${OXYGINE_CORE_LIBS})

add_executable (ai-test1 
            ${SOURCE_FILES}
            ${HEADER_FILES}
        ) 

#set_target_properties(CMakeTest PROPERTIES OUTPUT_NAME pagan_core CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(ai-test1
    pagan_core
    pagan_ai
    ${Boost_LIBRARIES}
    ${PYSTRING_LIBRARY}
    ${pugixml_LIBRARY}
)