project(Pagan)

cmake_minimum_required(VERSION 2.8.6)
message(STATUS "Compiling ${CMAKE_PROJECT_NAME}")
message(STATUS "Using cmake version ${CMAKE_VERSION}")

if( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    set( ARCH "x86")
else()
    set( ARCH "x64")
endif()

#-------------------------------------------------------------------------------
# Specify the default install path
if (NOT DEFINED CMAKE_INSTALL_PREFIX)
    SET( CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/ )
endif()

if (NOT DEFINED CMAKE_INCDIR_BASE)
    set( CMAKE_INCDIR_BASE include )
endif()

if (NOT DEFINED CMAKE_BINDIR_BASE)
    set( CMAKE_BINDIR_BASE bin )
endif()

if (NOT DEFINED CMAKE_LIBDIR_BASE)
    set( CMAKE_LIBDIR_BASE lib )
endif()

if (NOT DEFINED CMAKE_PLUGINDIR_BASE)
    set( CMAKE_PLUGINDIR_BASE plugin )
endif()

# Allow install path to be overridden for cross-compile builds
if(LIBRARY_OUTPUT_PATH_ROOT)
    SET( CMAKE_INSTALL_PREFIX ${LIBRARY_OUTPUT_PATH_ROOT}/ )
endif()

# Set the directory where the executables will be stored.
set(EXECUTABLE_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Directory where executables will be stored"
)

# Specify the list of directories to search for cmake modules.
set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#-------------------------------------------------------------------------------
# Detect Clang (until a cmake version provides built-in variables)
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_COMPILER_IS_CLANGCC 1)
endif()

set(OSD_COMPILER_FLAGS)

# Disable spurrious warnings in gcc builds and clang
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANGCC)

    # Turn on all warnings
    list(APPEND OSD_COMPILER_FLAGS -Wall)

elseif(MSVC)

    # Turn on all warnings
    list(APPEND OSD_COMPILER_FLAGS /Wall)
    
    # MSVC is unfortunately not standard conforming with regards to
    # the alternative names for logical and bitwise operators:
    # http://stackoverflow.com/questions/555505/c-alternative-tokens
    # http://stackoverflow.com/questions/6006526/c-writing-or-instead-of
    #
    # This can be solved by including iso646.h, but that is a rather
    # unsatisfactory solution since we then always have to remember to
    # include this header file.  Instead we define these operators
    # ourselves as command line arguments to cl.exe.
    #
    # An alternative would be to compile with the /Za option
    # (but unfortunately that breaks other code):
    # http://msdn.microsoft.com/en-us/library/0k0w269d.aspx
    list(APPEND OSD_COMPILER_FLAGS
                    /Dand=&&
                    /Dand_eq=&=
                    /Dbitand=&
                    /Dbitor=|
                    /Dcompl=~
                    /Dnot=!
                    /Dnot_eq=!=
                    /Dor=||
                    /Dor_eq=|=
                    # nvcc does not seem to like a caret being the last character
                    # in a command line defined preprocessor symbol, so add an
                    # empty trailing comment to avoid this.
                    /Dxor=^/**/
                    /Dxor_eq=^=
                    /D_SCL_SECURE_NO_WARNINGS
    )
    

    list(APPEND OSD_COMPILER_FLAGS
                    /W3     # Use warning level recommended for production purposes.
                    /WX     # Treat all compiler warnings as errors.

                    # these warnings are being triggered from inside VC's header files
                    # warning C4350: behavior change: 'member1' called instead of 'member2'
                    /wd4350
                    # warning C4548: expression before comma has no effect; expected expression with side-effect
                    /wd4548
                    
                    # Make sure WinDef.h does not define min and max macros which
                    # will conflict with std::min() and std::max().
                    /DNOMINMAX
                    
                    # Make sure the constants in <math.h> get defined.
                    /D_USE_MATH_DEFINES
                    
                    # Do not enforce MSVC's safe CRT replacements.
                    /D_CRT_SECURE_NO_WARNINGS
                    
                    # Disable checked iterators and iterator debugging.  Visual Studio
                    # 2008 does not implement std::vector::data(), so we need to take the
                    # address of std::vector::operator[](0) to get the memory location of
                    # a vector's underlying data storage.  This does not work for an empty
                    # vector if checked iterators or iterator debugging is enabled.
                     
		    # XXXX manuelk : we can't force SECURE_SCL to 0 or client code has
		    # problems linking against OSD if their build is not also
		    # overriding SSCL to the same value.
		    # See : http://msdn.microsoft.com/en-us/library/vstudio/hh697468.aspx
                    #/D_SECURE_SCL=0
                    #/D_HAS_ITERATOR_DEBUGGING=0
    )

    # Turn off a duplicate LIBCMT linker warning 
    set(CMAKE_EXE_LINKER_FLAGS 
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
    set(CMAKE_SHARED_LINKER_FLAGS 
        "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")

endif()

add_definitions(${OSD_COMPILER_FLAGS})

set(CMAKE_SUPPRESS_REGENERATION TRUE)

##############################################
#-- Add an Option to toggle the generation of the API documentation
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR 
      "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  #-- Configure the Template Doxyfile for our specific project
  configure_file(Doxyfile.in 
                 ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
  #-- Add a custom target to run Doxygen when ever the project is built
  add_custom_target (Docs ALL 
  										COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
  										SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
  # IF you do NOT want the documentation to be generated EVERY time you build the project
  # then leave out the 'ALL' keyword from the above command.
endif()

##############################################
# First for the generic no-config case (e.g. with mingw)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${EXECUTABLE_OUTPUT_PATH} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${EXECUTABLE_OUTPUT_PATH} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${EXECUTABLE_OUTPUT_PATH} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

##############################################

macro(copy_dlls TGT)
    set (DLLPATH "${CMAKE_SOURCE_DIR}/bin/${ARCH}")
    file(GLOB DLL_LIST
        "${DLLPATH}/*.dll"
    )
    foreach( dll ${DLL_LIST})
        add_custom_command(TARGET ${TGT} POST_BUILD        # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
            "${dll}"                                   # <--this is in-file
            ${EXECUTABLE_OUTPUT_PATH})                 # <--this is out-file path
    endforeach(dll)
endmacro(copy_dlls)

##############################################

macro(using_lib INC LIB)
    set (APP_LIBS
         ${APP_LIBS} LIB)
    set (APP_INCLUDES
         ${APP_INCLUDES} INC)
endmacro(using_lib)

add_definitions(-DPROJECT_ROOT="${CMAKE_SOURCE_DIR}/")

#################################################################
# Packages
SET(CMAKE_PREFIX_PATH $ENV{QTDIR})
if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.11)
    cmake_policy(SET CMP0020 OLD)
endif()
#SET(ENV{PATH} $ENV{QTDIR}bin $ENV{PATH}) # Still, from VS2012 I can't run it
find_package(Pystring)
SET(ENV{BOOST_ROOT} ${PROJECT_SOURCE_DIR}/contrib/boost_1_54_0)
find_package(BOOST 1.54.0 REQUIRED COMPONENTS program_options)
#find_package(Rapidjson)
#find_package(Curses)
find_package(pugixml)
find_package(Signal)
find_package(GLM)
find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)

#add_subdirectory(${CMAKE_SOURCE_DIR}/contrib/oxygine-framework ${CMAKE_CURRENT_BINARY_DIR}/contrib/oxygine-framework)
#add_definitions(${OXYGINE_DEFINITIONS})
#include_directories(${OXYGINE_INCLUDE_DIRS})
#link_directories(${OXYGINE_LIBRARY_DIRS})

# Sub-projects
add_subdirectory(lib)
add_subdirectory(app)