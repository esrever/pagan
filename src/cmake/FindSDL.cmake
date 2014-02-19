# Try to find GLEW library and include path.
# Once done this will define
#
# PYSTRING_FOUND
# PYSTRING_INCLUDE_DIR
# PYSTRING_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( SDL_INCLUDE_DIR
        NAMES
            SDL.h
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/SDL2/include
            DOC "The directory where SDL.h resides" )

    find_library( SDL_LIBRARY
        NAMES
            SDL2
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/SDL2/lib/${ARCH}

            DOC "The SDL library")
endif ()

find_package_handle_standard_args(SDL DEFAULT_MSG
    SDL_INCLUDE_DIR
    SDL_LIBRARY
)

mark_as_advanced( SDL_FOUND )
