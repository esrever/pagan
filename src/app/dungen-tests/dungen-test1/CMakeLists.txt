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

add_executable (dungen-test1 
            ${SOURCE_FILES}
            ${HEADER_FILES}
        ) 

# TODO: implement sth like "using_library" which copies DLLs and sets some common include/libs
copy_dlls(dungen-test1)
        
#set_target_properties(CMakeTest PROPERTIES OUTPUT_NAME pagan_core CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(dungen-test1
    ${APP_LIBS}
)

target_link_libraries(dungen-test1 debug
    ${APP_LIBS_DBG})
    
target_link_libraries(dungen-test1 optimized
    ${APP_LIBS_OPT})