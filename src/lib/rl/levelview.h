#pragma once

namespace pgn
{
    struct cTile;
    
    template<class T>
    cArray2D<T::view_data_type> ConvertLevel(const cLevel& zLevel);
    
    //----------------------------------------------------------------------
    //! View for movement cost of a map
    struct cLevelViewMovement
    {
        // typedefs
        typedef int view_data_type;
        // statics
        static const view_data_type  msDefaultValue = 1;
        
        view_data_type ConvertTile( const cTile& zTile );
    };
    
    //----------------------------------------------------------------------
    //! View for rendering as ASCII
    struct cLevelViewAscii
    {
        // typedefs
        typedef char view_data_type;
        // statics
        static const view_data_type  msDefaultValue = ' ';
        
        view_data_type ConvertTile( const cTile& zTile );
    };
    
    //----------------------------------------------------------------------
    //! View for rendering as 32-bit bitmap
    struct cLevelViewBitmap
    {
        // typedefs
        typedef unsigned view_data_type;
        // statics
        static const view_data_type  msDefaultValue = 0;
        
        view_data_type ConvertTile( const cTile& zTile );
    };
    
    #include "levelview.hpp"
};
