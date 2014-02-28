set(SOURCE_FILES
    src/main.cpp
    src/cmptest.cpp
    src/RegisterComponents.cpp
)

set(HEADER_FILES
    src/cmptest.h
    src/RegisterComponents.h
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

add_executable (ecs-test
            ${SOURCE_FILES}
            ${HEADER_FILES}
        ) 

copy_dlls(ecs-test)

#set_target_properties(CMakeTest PROPERTIES OUTPUT_NAME pagan_core CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(ecs-test
    ${APP_LIBS}
)

target_link_libraries(ecs-test debug
    ${APP_LIBS_DBG})
    
target_link_libraries(ecs-test optimized
    ${APP_LIBS_OPT})