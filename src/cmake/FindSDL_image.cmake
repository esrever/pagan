# Try to find GLEW library and include path.
# Once done this will define
#
# PYSTRING_FOUND
# PYSTRING_INCLUDE_DIR
# PYSTRING_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( SDL_image_INCLUDE_DIR
        NAMES
            SDL_image.h
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/SDL2_image/include
            DOC "The directory where SDL_image.h resides" )

    find_library( SDL_image_LIBRARY
        NAMES
            SDL2_image
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/SDL2_image/lib/${ARCH}

            DOC "The SDL_image library")
endif ()

find_package_handle_standard_args(SDL_image DEFAULT_MSG
    SDL_image_INCLUDE_DIR
    SDL_image_LIBRARY
)

mark_as_advanced( SDL_image_FOUND )
