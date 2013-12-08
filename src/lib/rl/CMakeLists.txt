#FILE(GLOB components_CPP "components/*.cpp")
#FILE(GLOB components_H "components/*.h")
#FILE(GLOB systems_CPP "systems/*.cpp")
#FILE(GLOB systems_H "systems/*.h")

file(GLOB_RECURSE COMPONENT_SRCS
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/components/*.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/components/*.h)
         
file(GLOB_RECURSE SYSTEM_SRCS
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/systems/*.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/systems/*.h)
         
file(GLOB_RECURSE EVENT_SRCS
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/events/*.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/events/*.h)

set(SOURCE_FILES
    EntityMgrRL.cpp
    SystemMgrRL.cpp
    ActionMgrRL.cpp
    GameApp.cpp
    util/FormatString.cpp
    util/json_conversions.cpp
    io/GameIO.cpp
    io/Keyboard.cpp
    resources/GameResources.cpp
    resources/FontLib.cpp
    resources/TileLib.cpp
)

set(HEADER_FILES
    EntityMgrRL.h
    SystemMgrRL.h
    ActionMgrRL.h
    GameApp.h
    util/FormatString.h
    util/json_conversions.h
    io/GameIO.h
    io/Keyboard.h
    resources/GameResources.h
    resources/FontLib.h
    resources/TileLib.h
    ${COMPONENT_SRCS}
    ${SYSTEM_SRCS}
    ${EVENT_SRCS}
    
)

add_definitions(
    ${PLATFORM_COMPILE_FLAGS}
)

source_group("Sources" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})
source_group("_Components"   FILES ${COMPONENT_SRCS})
source_group("_Systems"      FILES ${SYSTEM_SRCS})
source_group("_Events"      FILES ${EVENT_SRCS})

include_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PYSTRING_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${Rapidjson_INCLUDE_DIR}
    ${Curses_INCLUDE_DIR}
    ${Signal_INCLUDE_DIR}
    ${glm_INCLUDE_DIR}
)

add_library(pagan_rl STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_rl PROPERTIES OUTPUT_NAME pagan_rl CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(pagan_rl
    ${PLATFORM_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Curses_LIBRARY}
)



