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
    ${APP_INCLUDES}
)

#link_directories(${OXYGINE_LIBRARY_DIRS})
#target_link_libraries(HelloWorld ${OXYGINE_CORE_LIBS})

add_executable (bt-test
            ${SOURCE_FILES}
            ${HEADER_FILES}
        ) 

copy_dlls(bt-test)

#set_target_properties(CMakeTest PROPERTIES OUTPUT_NAME pagan_core CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(bt-test
    ${APP_LIBS}
)

target_link_libraries(bt-test debug
    ${APP_LIBS_DBG})
    
target_link_libraries(bt-test optimized
    ${APP_LIBS_OPT})