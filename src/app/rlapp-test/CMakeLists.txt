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

add_executable (rlapp-test
            ${SOURCE_FILES}
            ${HEADER_FILES}
        ) 

copy_dlls(rlapp-test)

set_target_properties(rlapp-test PROPERTIES OUTPUT_NAME rlapp-test CLEAN_DIRECT_OUTPUT 1 DEBUG_POSTFIX "d")
target_link_libraries(rlapp-test
    ${APP_LIBS}
)

foreach(f ${APP_LIBS_DBG}) 
target_link_libraries(rlapp-test debug ${f})
endforeach(f) 

foreach(f ${APP_LIBS_OPT})     
target_link_libraries(rlapp-test optimized ${f})
endforeach(f) 