#pragma once

#include <map>
#include <string>
#include <set>

#include "Entity.h"

namespace pgn
{
	class cEntityMgr : public cEventReceiver<cEntityCreatedEventData>,
					   public cEventReceiver<cDestroyEntityEventData>
	{
		public:
			// Entity creation functions

			//! From an existing entity
			cEntity Create( const cEntity& zEntity = cEntity());
			cEntity Create( const cEntity::component_mask_type& zMask, 
							const cEntity& zEntity = cEntity()); // augment
			//cEntity Create( conf = 0, e = null);  // augment
			//cEntity Create( bin = 0, e = null);   // augment

			// Entity marking/unmarking functions
			void Tag(const cEntity& zEntity, const std::string& zTag);
			void Untag(const cEntity& zEntity, const std::string& zTag);
			void Untag(const std::string& zTag);
			void Untag(const cEntity& zEntity);

			// Receiving functions
			void Receive( const cEntityCreatedEventData& zData);
			void Receive( const cDestroyEntityEventData& zData);

		private:
			std::map<std::string, std::set<cEntity>> mTaggedEntities;
	};

	
}