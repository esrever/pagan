#pragma once

#include <cassert>

namespace rapidjson
{
    class Document;
}

template<class T>
void read_json(T& obj, const rapidjson::Document& doc)
{
    asssert(false);
}