set(SOURCE_FILES
#    util/cmdopt.cpp
    app/sdlapp.cpp
    app/sdlwin.cpp
    util/logstream.cpp
    util/color.cpp
    util/rand.cpp
    util/image.cpp
    sdl/util.cpp
    sdl/font.cpp
    texture/texturelib.cpp
    texture/subtexturelib.cpp
    texture/texture.cpp
    texture/textureatlas.cpp
    serialize/util.cpp
    event/event.cpp
    noise/perlin.cpp
#    util/cfgtree.cpp
#    util/cfgdata.cpp
#    util/conversions.cpp
#    util/json_conversions.cpp
#    util/xml_conversions.cpp
#    app/application.cpp
)

set(HEADER_FILES
    app/sdlapp.h
    app/sdlwin.h
    container/ArrayStorage.h
    container/Array2DView.h
    container/Array2D.h
    container/Array2D_utils.h
    container/ArrayShape2D.h
    container/dict.h
    container/box2.h
    #container/dict2.h
    sdl/font.h
    sdl/util.h
    util/logstream.h
    util/color.h
    util/image.h
    util/rand.h
    util/idgen.h
    util/glm_utils.h
    util/string.h
    util/enum.h
    math/norm.h
    texture/subtexturelib.h
    texture/texturelib.h
    texture/texture.h
    texture/textureatlas.h
    serialize/serialize.h
    serialize/util.h
    #serialize/serialize_glm.hpp
    #serialize/serialize_std.hpp
    event/event.h
#    app/application.h
#    util/textmod.h
    noise/perlin.h
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
    ${glm_INCLUDE_DIR}
    ${pugixml_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${SDL_image_INCLUDE_DIR}
    ${SDL_ttf_INCLUDE_DIR}
    ${Signal_INCLUDE_DIR}
)

add_library(pagan_core STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
        )
set_target_properties(pagan_core PROPERTIES OUTPUT_NAME pagan_core CLEAN_DIRECT_OUTPUT 1 DEBUG_POSTFIX "d")
target_link_libraries(pagan_core
    ${PLATFORM_LIBRARIES}
    ${Boost_LIBRARIES}
)



