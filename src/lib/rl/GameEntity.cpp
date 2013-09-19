#include "GameEntity.h"

#include <cassert>

template<> 
void read_json<cGameEntity>(cGameEntity& obj, const rapidjson::Document& doc)
{
    assert(false);
}