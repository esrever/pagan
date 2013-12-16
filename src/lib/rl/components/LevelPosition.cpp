#include "LevelPosition.h"

#include <ecs/ecs.h>
#include <ecs/EntityMgr.h>

namespace pgn
{
	cmp::cLevelPosition::cLevelPosition() :mPos(0, 0), mLevel(ECS.mEntityMgr->GetEntityData().end()){}
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cLevelPosition>( cmp::cLevelPosition& zData, const rapidjson::Value& zRoot)
	{
        from_json( zData.mPos, zRoot["Position"]);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cLevelPosition>( const cmp::cLevelPosition& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        JsonWriter_AddMember("Position", zData.mPos, zRoot);
		zRoot.EndObject();
	}
}