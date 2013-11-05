#pragma once

#include <set>
#include <vector>
#include <string>

#include <core/util/json_conversions.h>

#include "EntityComponents.h"

namespace pgn
{
	class cQueryExpression
	{
	public:
		//! ctors
		cQueryExpression();
		~cQueryExpression(){}

		//! Receive functions
		void OnComponentAdded(cEntityWithComponents ec, unsigned short cid);
		void OnComponentRemove(cEntityWithComponents ec, unsigned short cid);
		void OnEntityCreated(cEntity e);
		void OnEntityDestroy(cEntity e);
		void OnEntityTagged(cEntity e, const std::string& zTag);
		void OnEntityUntag(cEntity e, const std::string& zTag);
		void OnTagRemove(const std::string& zTag);
		// TODO: add tagremove

		//! Access data
		const std::set<cEntity>& Entities() const { return mEntities; }
		//const std::vector<std::string>& Tags() const { return mTags; }
		//const component_mask_type& Mask() const { return mMask; }

		virtual bool operator < (const cQueryExpression &rhs) const { return container_hash < std::set < cEntity >> ()(Entities()) < container_hash < std::set < cEntity >> ()(rhs.Entities()); }
		virtual bool operator == (const cQueryExpression &rhs) const { return container_hash<std::set<cEntity>>()(Entities()) == container_hash<std::set<cEntity>>()(rhs.Entities()); }

	private:
		cEventHandler<cEntityTaggedEvent> mOnEntityTagged;
		cEventHandler<cEntityUntagEvent> mOnEntityUntag;
		cEventHandler<cEntityCreatedEvent> mOnEntityCreated;
		cEventHandler<cEntityDestroyEvent> mOnEntityDestroy;
		cEventHandler<cComponentAddedEvent> mOnComponentAdded;
		cEventHandler<cComponentRemoveEvent> mOnComponentRemove;
		cEventHandler<cTagRemoveEvent> mOnTagRemove;

	private:
		template<class T>
		friend bool from_json(T& zObj, const rapidjson::Value& zRoot);

		std::set<cEntity> mEntities;
		
		std::vector<std::string> mTags;
		std::vector<std::string> mNotTags;
		component_mask_type mMask;
		component_mask_type mNotMask;
	};

	//------------------------------------------------------------------------
	inline bool is_subset(const component_mask_type& zAll, const component_mask_type& zSub) { return (zAll & zSub) == zSub; }

	//------------------------------------------------------------------------
	template<>
	bool from_json<cQueryExpression>(cQueryExpression& zObj, const rapidjson::Value& zRoot);
}