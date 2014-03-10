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

add_executable (ai-test1
            ${SOURCE_FILES}
            ${HEADER_FILES}
        ) 

copy_dlls(bt-test)

target_link_libraries(ai-test1
    ${APP_LIBS}
)

target_link_libraries(ai-test1 debug
    ${APP_LIBS_DBG})
    
target_link_libraries(ai-test1 optimized
    ${APP_LIBS_OPT})