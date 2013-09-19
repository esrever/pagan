#include "asciirep.h"

#include <cassert>

cAsciiRep::cAsciiRep():
mSymbol(chtype(' ')),
mAttribs(chtype(0)),
mColor(COLOR_BLACK)
{}

template<>
void read_json<cAsciiRep>(cAsciiRep& obj, const rapidjson::Document& doc)
{
    // TODO: implement
    assert(false);
}