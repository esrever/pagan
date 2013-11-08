#include "QueryExpression.h"

#include <algorithm>

#include <boost/format.hpp>
#include <pystring.h>

#include <core/util/jsonutils.h>

#include "ecs.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cQueryExpression::cQueryExpression()
		: mHash(0)
		, mTags()
		, mTagsNot()
		, mMask()
		, mMaskNot()
		, mOnEntityCreated(Simple::slot(this, &cQueryExpression::OnEntityCreated))
		, mOnEntityDestroy(Simple::slot(this, &cQueryExpression::OnEntityDestroy))
		, mOnComponentAdded(Simple::slot(this, &cQueryExpression::OnComponentAdded))
		, mOnComponentRemove(Simple::slot(this, &cQueryExpression::OnComponentRemove))
		, mOnEntityTagged(Simple::slot(this, &cQueryExpression::OnEntityTagged))
		, mOnEntityUntag(Simple::slot(this, &cQueryExpression::OnEntityUntag))
		, mOnTagRemove(Simple::slot(this, &cQueryExpression::OnTagRemove))
	{
		
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::Reset()
	{
		mTags.clear();
		mTagsNot.clear();
		mMask.reset();
		mMaskNot.reset();
		mEntities.clear();
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::GenerateHash()
	{
		std::vector<std::string> allstrings;

		std::string mask_str = mMask.to_string();
		std::string masknot_str = mMaskNot.to_string();

		allstrings.insert(allstrings.end(), "tags");
		allstrings.insert(allstrings.end(), mTags.begin(), mTags.end());
		allstrings.insert(allstrings.end(), "tagsnot");
		allstrings.insert(allstrings.end(), mTagsNot.begin(), mTagsNot.end());
		allstrings.insert(allstrings.end(), "mask");
		allstrings.insert(allstrings.end(), mask_str);
		allstrings.insert(allstrings.end(), "masknot");
		allstrings.insert(allstrings.end(), masknot_str);
		mHash = container_hash<std::vector<std::string>>()(allstrings);
	}

	//------------------------------------------------------------------------------
	bool cQueryExpression::operator < (const cQueryExpression &rhs) const
	{ 
		return mHash<  rhs.mHash;
	}

	//------------------------------------------------------------------------------
	bool cQueryExpression::operator == (const cQueryExpression &rhs) const
	{ 
		//return container_hash<std::set<cEntity>>()(Entities()) == container_hash<std::set<cEntity>>()(rhs.Entities()); 
		return mHash == rhs.mHash;
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::Merge(const cQueryExpression& zQ, bool zNot)
	{
		mMask |= zNot ? zQ.mMaskNot : zQ.mMask;
		mMaskNot |= zNot ? zQ.mMask : zQ.mMaskNot;
		const auto& moreTags = zNot ? zQ.mTagsNot : zQ.mTags;
		const auto& moreTagsNot = zNot ? zQ.mTags : zQ.mTagsNot;
		mTags.insert(mTags.end(), moreTags.cbegin(), moreTags.cend());
		mTagsNot.insert(mTagsNot.end(), moreTagsNot.cbegin(), moreTagsNot.cend());
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::ScanEntities()
	{
		// Fetch all components that match the mask, from cEntityMgr
		for (auto x : ECS.mEntityMgr->GetComponents())
		{
			if (Qualify(x.first))
				mEntities.insert(x.first);
		}
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::OnComponentAdded(cEntityWithComponents ec, unsigned short)
	{
		const auto& e = ec->first;
		if (Qualify(e)) 
			mEntities.insert(e); 
		else 
			mEntities.erase(e);
	}
	//------------------------------------------------------------------------------
	void cQueryExpression::OnComponentRemove(cEntityWithComponents ec, unsigned short cid)
	{
		const auto& e = ec->first;
		if (Qualify(e))
			mEntities.insert(e);
		else
			mEntities.erase(e);
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::OnEntityCreated(cEntity e)
	{
		if (Qualify(e))
			mEntities.insert(e);
		else
			mEntities.erase(e);

	}
	//------------------------------------------------------------------------------
	void cQueryExpression::OnEntityDestroy(cEntity e)
	{
		mEntities.erase(e);
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::OnEntityTagged(cEntity e, const std::string& zTag)
	{
		if (Qualify(e))
			mEntities.insert(e);
		else
			mEntities.erase(e);
	}
	//------------------------------------------------------------------------------
	void cQueryExpression::OnEntityUntag(cEntity e, const std::string& zTag)
	{
		if (Qualify(e))
			mEntities.insert(e);
		else
			mEntities.erase(e);
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::OnTagRemove(const std::string& zTag)
	{
		// If the tag is in one of our list, what happens? issue a warning, as compos will forever be stripped!
		assert(false);
	}

	//------------------------------------------------------------------------
	bool cQueryExpression::Qualify(cEntity e) const
	{
		bool ret = true;

		const auto& tagged_ents = ECS.mEntityMgr->TaggedEntities();
		// TAGS
		// TODO: perhaps store iterators to the entity sets in ecs mgr for the selected tags?
		for (const auto& x : mTags)
		{
			auto itset = tagged_ents.find(x);
			if (itset != tagged_ents.end())
			{
				auto itent = itset->second.find(e);
				if (itent == itset->second.end())
					return false;
			}
		}

		// TAGS_NOT
		for (const auto& x : mTags)
		{
			auto itset = tagged_ents.find(x);
			if (itset != tagged_ents.end())
			{
				auto itent = itset->second.find(e);
				if (itent != itset->second.end())
					return false;
			}
		}
		
		const auto& entity_components = ECS.mEntityMgr->GetComponents();
		const auto& it = entity_components.find(e);
		if (it == entity_components.end())
			return false;
		
		// MASK
		const component_mask_type& allmask = it->second.Mask();
		if (!is_subset(allmask,mMask))
			return false;
		// MASK_NOT
		if (!is_subset(allmask, mMaskNot))
			return false;

		return true;
	}

	//------------------------------------------------------------------------------
	static bool read_component_mask(component_mask_type& zMask, const std::vector<std::string>& zCompos)
	{
		bool err = false;
		for (const auto& s : zCompos)
		{
			auto idx = EMGR->GetComponentTypeIndex(s);
			if (idx != 0xFFFFFFFF)
				zMask.set(idx);
			else
			{
				ECS.mLog->Err(boost::str(boost::format("cQueryExpression::from_json: Component type \"%s\" does not exist")%s));
				err = true;
			};
		}
		return !err;
	}

	//------------------------------------------------------------------------------
	bool cQueryExpression::ArrayFormatMergeSingle(const rapidjson::Value& zRoot)
	{
		if (!zRoot.IsString()) return false;
		std::string name = zRoot.GetString();
		bool zNot = false;
		if (name[0] == '!')
		{
			zNot = true;
			name.erase(name.begin());
		}
		
		std::vector<std::string> results;
		if (pystring::startswith(name, "tag_"))
		{
			pystring::split(name, results, "_", 1);
			if (zNot)
				mTagsNot.push_back(results[1]);
			else
				mTags.push_back(results[1]);
		}
		else if (pystring::startswith(name, "cmp_"))
		{
			pystring::split(name, results, "_", 1);
			std::vector<std::string> components;
			components.push_back(results[1]);
			read_component_mask(zNot ? mMaskNot : mMask, components);
		}
		else
		{
			// no arraymode composite for now
			auto it = ECS.mSystemMgr->GetQueries().find(name);
			assert(it != ECS.mSystemMgr->GetQueries().end());
			*this = *it->second;
		}
		return true;
	}

	//------------------------------------------------------------------------
	template<>
	bool from_json<cQueryExpression>(cQueryExpression& zObj, const rapidjson::Value& zRoot)
	{
		zObj.Reset();

		// Verbose mode
		if (zRoot.IsObject())
		{
			std::vector<std::string> compos, composNot, composites, compositesNot;
			if (zRoot.HasMember("HasTags"))
				read_json_vector(zObj.mTags, zRoot["HasTags"]);
			if (zRoot.HasMember("HasNotTags"))
				read_json_vector(zObj.mTagsNot, zRoot["HasNotTags"]);
			if (zRoot.HasMember("HasComponents"))
				read_json_vector(compos, zRoot["HasComponents"]);
			if (zRoot.HasMember("HasNotComponents"))
				read_json_vector(composNot, zRoot["HasNotComponents"]);
			if (zRoot.HasMember("HasComposites"))
				read_json_vector(composites, zRoot["HasComposites"]);
			if (zRoot.HasMember("HasNotComposites"))
				read_json_vector(compositesNot, zRoot["HasNotComposites"]);

			const auto& allq = ECS.mSystemMgr->GetQueries();
			for (const auto& x : composites)
			{
				const auto it = allq.find(x);
				if (it != allq.end())
					zObj.Merge(*it->second, false);
			}
			for (const auto& x : compositesNot)
			{
				const auto it = allq.find(x);
				if (it != allq.end())
					zObj.Merge(*it->second, true);
			}

			bool ok = true;
			ok = ok && read_component_mask(zObj.mMask, compos);
			ok = ok && read_component_mask(zObj.mMaskNot, composNot);

			if (!ok)
				zObj.Reset();

			if (ok)
				zObj.Finalize();
			return ok;
		}
		// Array mode
		else if (zRoot.IsArray())
		{
			for (auto itr = zRoot.Begin(); itr != zRoot.End(); ++itr)
			{
				zObj.ArrayFormatMergeSingle(*itr);
			}
			zObj.Finalize();
			return true;
		}
		else
		{
			auto ok = zObj.ArrayFormatMergeSingle(zRoot);
			if( ok) 
				zObj.Finalize();
			return ok;
		}
	}

	//------------------------------------------------------------------------
	template<>
	void to_json<cQueryExpression>(const cQueryExpression& zMgr, JsonWriter& writer)
	{
		writer.StartObject();
		JsonWriter_AddMember("Tags", zMgr.mTags,writer);
		JsonWriter_AddMember("Tags-not", zMgr.mTagsNot, writer);
		JsonWriter_AddMember("Mask", zMgr.mMask, writer);
		JsonWriter_AddMember("Mask-not", zMgr.mMaskNot, writer);
		writer.EndObject();
	}
}