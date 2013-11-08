#pragma once

#include <set>
#include <vector>
#include <string>

#include <core/util/predicates.h>
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

		void Reset();
		bool Qualify(cEntity e) const;

		//! Access data
		size_t Hash() const { return mHash; }
		const std::set<cEntity>& Entities() const { return mEntities; }

		bool operator < (const cQueryExpression &rhs) const;
		bool operator == (const cQueryExpression &rhs) const;

	private:
		bool ArrayFormatMergeSingle(const rapidjson::Value& zRoot);

		void Merge(const cQueryExpression& zQ, bool zNot);

		//! Scans all entities that fulfill query criteria
		void ScanEntities();

		//! Generate hash from all tags/masks
		void GenerateHash();

		//! Scan entities and generate hash
		void Finalize() { ScanEntities(); GenerateHash(); }

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
		template<class T>
		friend void to_json(const T& zObj, JsonWriter& zRoot);

		std::set<cEntity> mEntities;
		
		std::vector<std::string> mTags;
		std::vector<std::string> mTagsNot;
		component_mask_type mMask;
		component_mask_type mMaskNot;

		size_t mHash;
	};

	DECL_PTRTYPE(cQueryExpression);

	//------------------------------------------------------------------------
	inline bool is_subset(const component_mask_type& zAll, const component_mask_type& zSub) { return (zAll & zSub) == zSub; }

	//------------------------------------------------------------------------
	template<>
	bool from_json<cQueryExpression>(cQueryExpression& zObj, const rapidjson::Value& zRoot);

	//------------------------------------------------------------------------
	template<>
	void to_json<cQueryExpression>(const cQueryExpression& zMgr, JsonWriter& writer);
}