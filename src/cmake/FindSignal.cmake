# Try to find GLEW library and include path.
# Once done this will define
#
# JSONSPIRIT_FOUND
# JSONSPIRIT_INCLUDE_DIR
# JSONSPIRIT_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( Signal_INCLUDE_DIR
        NAMES
            Signal.h
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/ssignals
            DOC "The directory where Signal resides" )
endif ()

find_package_handle_standard_args(Signal DEFAULT_MSG
    Signal_INCLUDE_DIR
)

mark_as_advanced( Curses_FOUND )
