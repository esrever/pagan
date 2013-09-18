# Try to find GLEW library and include path.
# Once done this will define
#
# JSONSPIRIT_FOUND
# JSONSPIRIT_INCLUDE_DIR
# JSONSPIRIT_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( Curses_INCLUDE_DIR
        NAMES
            curses.h
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/pdcurses
            DOC "The directory where PDCurses resides" )
    find_library( Curses_LIBRARY
        NAMES
            pdcurses
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/pdcurses

            DOC "The PDCurses library")
    find_file( Curses_DLL
        NAMES
            pdcurses.dll
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/pdcurses

            DOC "The PDCurses dll")
endif ()

find_package_handle_standard_args(Curses DEFAULT_MSG
    Curses_INCLUDE_DIR
    Curses_LIBRARY
)

mark_as_advanced( Curses_FOUND )
