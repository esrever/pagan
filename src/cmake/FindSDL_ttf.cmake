# Try to find GLEW library and include path.
# Once done this will define
#
# PYSTRING_FOUND
# PYSTRING_INCLUDE_DIR
# PYSTRING_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( SDL_ttf_INCLUDE_DIR
        NAMES
            SDL_ttf.h
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/SDL2_ttf/include
            DOC "The directory where SDL_ttf.h resides" )

    find_library( SDL_ttf_LIBRARY
        NAMES
            SDL2_ttf
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/SDL2_ttf/lib/${ARCH}

            DOC "The SDL_ttf library")
endif ()

find_package_handle_standard_args(SDL_ttf DEFAULT_MSG
    SDL_ttf_INCLUDE_DIR
    SDL_ttf_LIBRARY
)

mark_as_advanced( SDL_ttf_FOUND )
