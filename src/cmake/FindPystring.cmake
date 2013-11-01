# Try to find GLEW library and include path.
# Once done this will define
#
# PYSTRING_FOUND
# PYSTRING_INCLUDE_DIR
# PYSTRING_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( PYSTRING_INCLUDE_DIR
        NAMES
            pystring.h
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/pystring
            DOC "The directory where pystring.h resides" )

    find_library( PYSTRING_LIBRARY
        NAMES
            pystring
        PATHS
            #${PROJECT_SOURCE_DIR}/contrib/pystring/lib/${ARCH}/debug
            ${PROJECT_SOURCE_DIR}/contrib/pystring/lib/${ARCH}/release

            DOC "The pystring library")
            
    find_library( PYSTRING_LIBRARY_DBG
        NAMES
            pystring
        PATHS
            #${PROJECT_SOURCE_DIR}/contrib/pystring/lib/${ARCH}/debug
            ${PROJECT_SOURCE_DIR}/contrib/pystring/lib/${ARCH}/debug

            DOC "The pystring library")
endif ()

find_package_handle_standard_args(PYSTRING DEFAULT_MSG
    PYSTRING_INCLUDE_DIR
    PYSTRING_LIBRARY
    PYSTRING_LIBRARY_DBG
)

mark_as_advanced( PYSTRING_FOUND )
