set (APP_LIBS 
     pagan_core
     pagan_io
     pagan_ai
     pagan_dungen
     ${Boost_LIBRARIES}
     ${SDL_LIBRARY}
     ${SDL_image_LIBRARY}
     ${SDL_ttf_LIBRARY})
     
set (APP_LIBS_DBG
    ${PYSTRING_LIBRARY_DBG}
    ${pugixml_LIBRARY_DBG})
    
set (APP_LIBS_OPT
    ${PYSTRING_LIBRARY}
    ${pugixml_LIBRARY})
     
set (APP_INCLUDES
    ${PROJECT_SOURCE_DIR}/lib
    ${Boost_INCLUDE_DIRS}
    ${glm_INCLUDE_DIR}
    ${PYSTRING_INCLUDE_DIR}
    ${pugixml_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${SDL_image_INCLUDE_DIR}
    ${SDL_ttf_INCLUDE_DIR})
    

#add_subdirectory(CMakeTest)
#add_subdirectory(roguelike-tests)
#add_subdirectory(ecs-tests)
#add_subdirectory(qt-tests)
#add_subdirectory(oxy-tests)
add_subdirectory(ai-tests)
add_subdirectory(io-tests)
add_subdirectory(dungen-tests)
add_subdirectory(sdl-tests)