#pragma once

#include <curses.h>
#include <core/util/jsonutils.h>

struct cAsciiRep
{
    cAsciiRep();
    chtype mSymbol;
    chtype mAttribs;
    int    mColor;
};


template<>
void read_json<cAsciiRep>(cAsciiRep& obj, const rapidjson::Document& doc);