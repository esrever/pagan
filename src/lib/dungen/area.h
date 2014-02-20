#pragma once

#include <memory>
#include <map>
#include <set>
#include <vector>

#include "constraints.h"

namespace pgn
{
	namespace dungen
	{

		struct cWorkspace;
		struct cConstraints;

		struct cArea
		{
			typedef std::shared_ptr<cArea> ptr_type;
			typedef std::weak_ptr<cArea> wptr_type;
			// ctor
			cArea(cWorkspace& ws);
			virtual ~cArea(){}

			// virtuals
			virtual void Init() = 0;
			virtual void update_perimeter();
			virtual void calc_avail_connection_slots() = 0;

			static bool add_connection(ptr_type area, size_t slot_idx, const std::set<size_t>& excludeIds = std::set<size_t>());

			// Members
			size_t							mMaxConns;
			bool							mIsRoom;
			cWorkspace&						mWorkspace;
			size_t							mId;
			std::map<glm::ivec2, size_t>	mConnections;			// subset of perimeter
			std::set<glm::ivec2>			mPerimeter;
			std::vector<glm::ivec2>			mAvailConnectionSlots;	// subset of perimeter
		};

		class area_finder_wptr {
		public:
			area_finder_wptr(size_t id) :mId(id){}
			bool operator()(const std::weak_ptr<cArea>& area) const
			{
				auto ptr = area.lock();
				return ptr ? ptr->mId == mId : false;
			}
		private:
			size_t mId;
		};

		class area_finder_ptr {
		public:
			area_finder_ptr(size_t id) :mId(id){}
			bool operator()(const std::shared_ptr<cArea>& ptr) const
			{
				return ptr ? ptr->mId == mId : false;
			}
		private:
			size_t mId;
		};

	}
}

namespace std {
	template <> struct less<std::weak_ptr<pgn::dungen::cArea>>
	{
		bool operator()(const std::weak_ptr<pgn::dungen::cArea> & lhs, const std::weak_ptr<pgn::dungen::cArea> & rhs) const
		{
			auto p0 = lhs.lock();
			auto p1 = rhs.lock();
			return p0->mId < p1->mId;
		}
	};
}


namespace std {
	template <> struct less<pgn::dungen::cArea>
	{
		bool operator()(const pgn::dungen::cArea & lhs, const pgn::dungen::cArea & rhs) const
		{
			return lhs.mId < rhs.mId;
		}
	};
}

inline bool operator < (const std::shared_ptr<pgn::dungen::cArea> & lhs, const std::shared_ptr<pgn::dungen::cArea> & rhs)
{
	return lhs->mId < rhs->mId;
}