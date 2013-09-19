#pragma once

#include <string>

#include <core/util/jsonutils.h>

#include "asciirep.h"

class cGameEntity
{
    friend void read_json(cGameEntity& obj, const rapidjson::Document& doc);
    
    public:
        // 
        cGameEntity(){}
        virtual ~cGameEntity(){}
    
    protected:
        std::string mName;
        std::string mDesc;
        std::string mLongDesc;
        cAsciiRep   mAsciiRep;
};

template<>
void read_json<cGameEntity>(cGameEntity& obj, const rapidjson::Document& doc);