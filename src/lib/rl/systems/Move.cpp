#include "Move.h"

#include <ecs/Event.h>
#include <ecs/Component.h>
#include <ecs/EntityData.h>

#include <rl/components/Movement.h>
#include <rl/components/MapSprite.h>
#include <rl/components/LevelPosition.h>

static const char * dir2text_full[] = { "south-west", "south", "south-east", "west", "nowhere", "east", "north-west", "north", "north-east" };

namespace pgn
{
	namespace sys
	{
		cMove::cMove()
		:mOnActionMoveAdj(Simple::slot(this, &cMove::OnActionMoveAdj))
		,mOnActionIdle(Simple::slot(this, &cMove::OnActionIdle))
		{

		}

		void cMove::OnActionMoveAdj(cEntityWithData ed, const glm::ivec2& vin)
		{ 
			glm::ivec2 v = vin;

			// Get entity data
			std::shared_ptr< cComponent<cmp::cMovement>> move_ptr;
			std::shared_ptr< cComponent<cmp::cMapSprite>> sprite_ptr;
			std::shared_ptr< cComponent<cmp::cLevelPosition>> pos_ptr;
			ed->second.mComponents.GetComponent(move_ptr);
			ed->second.mComponents.GetComponent(sprite_ptr);
			ed->second.mComponents.GetComponent(pos_ptr);

			// TODO: apply logic to check if we can move, apply movepoint reduction etc.

			// TODO: temp!
			pos_ptr->mData.mPos += v;
			auto curspritepos = sprite_ptr->mData.mSprite->getPosition();
			sprite_ptr->mData.mSprite->setPosition(curspritepos + oxygine::Vector2(32 * v.x, -32 * v.y));

			// Log
			int udir = v.x + 1 + (v.y + 1) * 3;
			evt::cLog::mSig.emit("game_log", boost::str(boost::format("%s moved %s") % ed->second.mName.c_str() % dir2text_full[udir]));

		}

		void cMove::OnActionIdle(cEntityWithData ed)
		{
			evt::cLog::mSig.emit("game_log", boost::str(boost::format("%s moved %s") % ed->second.mName.c_str() % dir2text_full[4]));
		}

		void cMove::Process()
		{
			
		}

		bool cMove::from_json(const rapidjson::Value& zRoot)
		{
			cSystemBase::from_json(zRoot);

			auto b0 = LoadQuery(mQueryLog, zRoot, "QueryLog");
			return b0 ;
		}

		void cMove::to_json(JsonWriter& zRoot) const
		{
			zRoot.StartObject();
			zRoot.String("Base");
			cSystemBase::to_json(zRoot);
			JsonWriter_AddMember("QueryLog", mQueryLog, zRoot);
			zRoot.EndObject();
		}
	}
}