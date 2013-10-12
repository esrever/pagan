# Try to find GLEW library and include path.
# Once done this will define
#
# JSONSPIRIT_FOUND
# JSONSPIRIT_INCLUDE_DIR
# JSONSPIRIT_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( glm_INCLUDE_DIR
        NAMES
            glm/glm.hpp
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/glm
            DOC "The directory where GLM resides" )
endif ()

find_package_handle_standard_args(glm DEFAULT_MSG
    glm_INCLUDE_DIR
)

mark_as_advanced( glm_FOUND )
