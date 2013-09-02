# Try to find GLEW library and include path.
# Once done this will define
#
# JSONSPIRIT_FOUND
# JSONSPIRIT_INCLUDE_DIR
# JSONSPIRIT_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( Rapidjson_INCLUDE_DIR
        NAMES
            rapidjson/rapidjson.h
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/rapidjson/include
            DOC "The directory where the json headers reside" )
endif ()

find_package_handle_standard_args(Rapidjson DEFAULT_MSG
    Rapidjson_INCLUDE_DIR
)

mark_as_advanced( Rapidjson_FOUND )
