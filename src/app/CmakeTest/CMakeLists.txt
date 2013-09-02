set(SOURCE_FILES
    main.cpp
)

set(HEADER_FILES
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


# TODO: what's the lib dir? depends on the configuration that I want! how can i specify that flexibly? from find_package? if I detect the generator for x86/64 then in the script?

add_executable (CMakeTest 
            ${SOURCE_FILES}
            ${HEADER_FILES}
        ) 

#message("${PLATFORM_LIBRARIES}")
#message("${Boost_LIBRARIES}")
#message("${PYSTRING_LIBRARY}")

#set_target_properties(CMakeTest PROPERTIES OUTPUT_NAME pagan_core CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(CMakeTest
    pagan_core
    ${Boost_LIBRARIES}
)

target_link_libraries(CMakeTest
    debug
    ${PYSTRING_LIBRARY_DBG}
    optimized
    ${PYSTRING_LIBRARY}
)