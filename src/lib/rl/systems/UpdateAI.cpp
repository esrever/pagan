#include "UpdateAI.h"

#include <core/serialize/util.h>

#include <rl/event/events.h>
#include <rl/components/components.h>
#include <rl/systems/UpdateLayout.h>
#include <rl/ai/behavior.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//--------------------------------------------------------------------------
			cUpdateAI::cUpdateAI()
			{
				mAIQuery.Require<cmp::cControllerAI>();
				mAIQuery.SetOnEntityAdded(std::bind(&cUpdateAI::InitializeBt, this , std::placeholders::_1));
			}

			//--------------------------------------------------------------------------
			bool cUpdateAI::operator()(ecs::cEntityWithData ed)
			{
				auto& btree = ed->second.Component<cmp::cControllerAI>()->mBtree;
				auto status = btree.Tick();
				return status != bt::eStatus::Failure;
			}

			//--------------------------------------------------------------------------
			void cUpdateAI::InitializeBt(ecs::cEntityWithData ed)
			{
				auto& btree = ed->second.Component<cmp::cControllerAI>()->mBtree;
				btree.BlackBoard().mDictPerm.Insert("me", ed);
			}

			//--------------------------------------------------------------------------
			void cUpdateAI::LoadBehaviorDb(const char * fname)
			{
				pugi::xml_document doc;
				if (LoadXML(doc, fname, std::cout))
				{
					for (const auto& c : doc.children())
					{
						bt::cBehavior * behavior;
						bt::cBehavior::SerializeIn(c, behavior);
						if (behavior)
						{
							mBehaviorDb[c.attribute("name").as_string()] = behavior;
						}

					}
				}
			}

			//--------------------------------------------------------------------------
			bt::cBehavior * cUpdateAI::GetBehavior(const std::string& name) const
			{
				auto it = mBehaviorDb.find(name);
				if (it != mBehaviorDb.end())
				{
					return it->second->Clone();
				}
				else
					return nullptr;
			}
		}
	}
}