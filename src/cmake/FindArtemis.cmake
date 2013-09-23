# Try to find GLEW library and include path.
# Once done this will define
#
# PYSTRING_FOUND
# PYSTRING_INCLUDE_DIR
# PYSTRING_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( ARTEMIS_INCLUDE_DIR
        NAMES
            Artemis.h
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/artemis/include
            DOC "The directory where artemis.h resides" )

    find_library( ARTEMIS_LIBRARY
        NAMES
            artemis
        PATHS
            #${PROJECT_SOURCE_DIR}/contrib/pystring/lib/${ARCH}/debug
            ${PROJECT_SOURCE_DIR}/contrib/artemis/lib

            DOC "The artemis library")
            
    find_library( ARTEMIS_LIBRARY_DBG
        NAMES
            artemisd
        PATHS
            #${PROJECT_SOURCE_DIR}/contrib/pystring/lib/${ARCH}/debug
            ${PROJECT_SOURCE_DIR}/contrib/artemis/lib

            DOC "The artemis library")
endif ()

find_package_handle_standard_args(ARTEMIS DEFAULT_MSG
    ARTEMIS_INCLUDE_DIR
    ARTEMIS_LIBRARY
    ARTEMIS_LIBRARY_DBG
)

mark_as_advanced( ARTEMIS_FOUND )
