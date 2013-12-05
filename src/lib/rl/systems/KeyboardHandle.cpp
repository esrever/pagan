#include "KeyboardHandle.h"

#include <Input.h>

#include <ecs/Event.h>
#include <ecs/Component.h>
#include <ecs/EntityData.h>

#include <rl/components/KeyActionMapper.h>

namespace pgn
{
	namespace sys
	{
		cKeyboardHandle::cKeyboardHandle()
		:mOnKey(Simple::slot(this, &cKeyboardHandle::OnKey))
		{

		}

		void cKeyboardHandle::OnKey(const int key, const oxygine::cKeyState& state)
		{ 

			// Debug key presses
			//std::cout << boost::str(boost::format("%s - %s - %d\n")% (state.mPressed ? "pressed" : "released")%(state.mRepeat ? "repeat" : "no-repeat")%key );

			for (auto e : mQueryKeyActionMapper->Entities())
			{
				// Get the action mapper
				std::shared_ptr< cComponent<cmp::cKeyActionMapper>> kam_ptr;
				auto ec = ECS.mEntityMgr->GetEntityData().find(e);
				assert(ec != ECS.mEntityMgr->GetEntityData().end());
				ec->second.mComponents.GetComponent(kam_ptr);
				cmp::cKeyActionMapper& kam = kam_ptr->mData;
			}

			for (auto e : mQueryDevConsole->Entities())
			{
				// TODO: ... 
			}
		}

		void cKeyboardHandle::Process()
		{
			
		}

		bool cKeyboardHandle::from_json(const rapidjson::Value& zRoot)
		{
			cSystemBase::from_json(zRoot);

			auto b0 = LoadQuery(mQueryKeyActionMapper, zRoot, "QueryKeyActionMapper");
			auto b1 = LoadQuery(mQueryDevConsole, zRoot, "QueryDevConsole");
			return b0;
		}

		void cKeyboardHandle::to_json(JsonWriter& zRoot) const
		{
			zRoot.StartObject();
			zRoot.String("Base");
			cSystemBase::to_json(zRoot);
			JsonWriter_AddMember("QueryKeyActionMapper", mQueryKeyActionMapper, zRoot);
			JsonWriter_AddMember("QueryDevConsole", mQueryDevConsole, zRoot);
			zRoot.EndObject();
		}
	}
}