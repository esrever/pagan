# Try to find GLEW library and include path.
# Once done this will define
#
# JSONSPIRIT_FOUND
# JSONSPIRIT_INCLUDE_DIR
# JSONSPIRIT_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( JSONSPIRIT_INCLUDE_DIR
        NAMES
            json_spirit.h
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/json_spirit_v4.06/json_spirit
            DOC "The directory where the json headers reside" )

    find_library( JSONSPIRIT_LIBRARY
        NAMES
            json_spirit_lib
        PATHS
            #${PROJECT_SOURCE_DIR}/contrib/json_spirit_v4.06/lib/x86/debug
            ${PROJECT_SOURCE_DIR}/contrib/json_spirit_v4.06/lib/x86/release
            #${PROJECT_SOURCE_DIR}/contrib/json_spirit_v4.06/lib/x64/debug
            #${PROJECT_SOURCE_DIR}/contrib/json_spirit_v4.06/lib/x64/release
            DOC "The json spirit library")
endif ()

find_package_handle_standard_args(JSONSPIRIT DEFAULT_MSG
    JSONSPIRIT_INCLUDE_DIR
    JSONSPIRIT_LIBRARY
)

mark_as_advanced( JSONSPIRIT_FOUND )
