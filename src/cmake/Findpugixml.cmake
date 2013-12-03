# Try to find GLEW library and include path.
# Once done this will define
#
# JSONSPIRIT_FOUND
# JSONSPIRIT_INCLUDE_DIR
# JSONSPIRIT_LIBRARY
#

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_path( pugixml_INCLUDE_DIR
        NAMES
            pugixml.hpp
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/pugixml/src
            DOC "The directory where pugixml headers reside" )
    find_library( pugixml_LIBRARY
        NAMES
            pugixml
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/pugixml/lib/${ARCH}/debug

            DOC "The pugixml library")
            
    find_library( pugixml_LIBRARY_DBG
        NAMES
            pugixml
        PATHS
            ${PROJECT_SOURCE_DIR}/contrib/pugixml/lib/${ARCH}/release

            DOC "The pugixml library")
endif ()

find_package_handle_standard_args(pugixml DEFAULT_MSG
    pugixml_INCLUDE_DIR
    pugixml_LIBRARY
    pugixml_LIBRARY_DBG
)

mark_as_advanced( pugixml_FOUND )
