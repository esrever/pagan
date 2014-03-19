#include "dungen.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#include <rl/path/Astar.h>

#include "constraints.h"
#include "corr.h"
#include "room.h"

namespace pgn
{
	namespace rl
	{
		void cDiagDistMatrix::Resize(size_t elem_num, size_t value)
		{
			mDataElems = elem_num;
			elem_num = elem_num*(1 + elem_num) / 2;
			mData.resize(elem_num, value);
		}

		size_t& cDiagDistMatrix::operator()(size_t x, size_t y)
		{
			if (x > y)
				std::swap(x, y);
			size_t idx0 = y*(1 + y) / 2;
			return mData.at(idx0 + x);
		}

		void cDiagDistMatrix::update_set(size_t a0, size_t a1, size_t corradd)
		{
			auto& obj = *this;
			for (size_t i0 = 0; i0 < mDataElems; ++i0)
			{
				auto d0 = obj(i0, a0);
				if (d0 != cWorkspace::inf)
				{
					for (size_t i1 = 0; i1 < mDataElems; ++i1)
					{
						size_t d1 = obj(i1, a1);
						if (d1 != cWorkspace::inf)
						{
							auto& d = obj(i0, i1);
							d = std::min(d, d0 + d1 + 1 + corradd);
						}
					}
				}
			}
		}

		void cDiagDistMatrix::update_single(size_t c, size_t a1)
		{
			auto& obj = *this;
			auto d0 = obj(c, a1);
			if (d0 != cWorkspace::inf)
			{
				for (size_t i1 = 0; i1 < mDataElems; ++i1)
				{
					size_t d1 = obj(i1, a1);
					if (d1 != cWorkspace::inf)
					{
						auto& d = obj(c, i1);
						d = std::min(d, d0 + d1);
					}
				}
			}
		}

		cWorkspace::cWorkspace()
			:mAstar()
		{}

		void cWorkspace::Init(int w, int h)
		{
			mAreas.clear();
			mMapData.Resize(w, h,0);
			mPerimeterMap.Resize(w, h);
			mConnectionMap.Resize(w, h);

			// set perimeter as blocked
			int v = eMapData::blocked;
			mMapData.CreateView(w,1,0,0).Fill(v);
			mMapData.CreateView(w,1,0,h-1).Fill(v);
			mMapData.CreateView(1,h,0,0).Fill(v);
			mMapData.CreateView(1,h,w-1,0).Fill(v);
		}

		float cWorkspace::MoveCost(const glm::ivec2& p, int invBits, int reqBits) const
		{
			auto& v = mMapData(p);
			bool invCheck = (v & invBits) == 0;
			bool reqCheck = (v & reqBits) == reqBits;
			return (reqCheck && invCheck) ? 1.0f : std::numeric_limits<float>::max();
		}

		void cWorkspace::Print()
		{
			FILE * fp = fopen("dungen.ppm", "wb");
			fprintf(fp, "P6\n%d %d\n255\n", mMapData.Width(), mMapData.Height());

			std::ostringstream sout;
			std::string line;
			line.resize((mMapData.Width() + 1) * mMapData.Height());
			size_t lo = 0;

			static const size_t randc = 200;
			cArray2D<glm::ivec3> cols;
			cols.Resize(mMapData.Width(), mMapData.Height());

			for (size_t i = 0; i < mMapData.Height(); ++i)
			{
				for (size_t j = 0; j < mMapData.Width(); ++j)
				{
					int elem = mMapData(j, mMapData.Height() - i - 1);
					char c = '?';
					if (elem == eMapData::unused)
						c = ' ';
					else if (elem & eMapData::conn)
						c = '+';
					else if (elem & eMapData::entry)
						c = '>';
					else if (elem & eMapData::exit)
						c = '<';
					//else if (elem & eMapData::dbg1)
					//	c = '@';
					else if (elem & eMapData::blocked)
						c = 'X';
					else if (elem & eMapData::room)
						c = '.';
					else if (elem & eMapData::corridor)
						c = ',';
					else if (elem & eMapData::perimeter)
						c = '#';
					else
						c = 'a' + elem;

					auto& col = cols(j, i);
					if (elem & eMapData::corridor)
						col += glm::ivec3(255, 255, 0);
					if (elem & eMapData::room)
						col += glm::ivec3(0, 255, 255);
					if (elem & eMapData::blocked)
						col -= glm::ivec3(16, 16, 16);
					if (elem & eMapData::perimeter)
						col += glm::ivec3(64, 64, 64);
					// conn -> green
					if (elem & eMapData::conn)
						col += glm::ivec3(255, -255, -255);

					if (elem & (eMapData::entry | eMapData::exit))
						col = glm::ivec3(0, 0, 255);

					//col = glm::clamp(col, glm::ivec3(0, 0, 0), glm::ivec3(255-randc, 255 - randc, 255 - randc));
					col = glm::clamp(col, glm::ivec3(0, 0, 0), glm::ivec3(255, 255, 255));

					line.at(lo++) = c;
				}
				line.at(lo++) = '\n';
			}


			glm::ivec3 c2(randc, randc, randc);
			static int x_idx[] = { 0, 0, 0, 0, 0, 1, 1 };
			static int y_idx[] = { 0, 1, 0, 1, 0, 1, 1 };
			static int z_idx[] = { 1, 0, 0, 1, 1, 0, 1 };
			for (size_t i = 0; i < mIslands.size(); ++i)
			{
				size_t i7 = i % 7;
				glm::ivec3 c;
				c.x = rand() % randc;
				c.y = rand() % randc;
				c.z = rand() % randc;

				for (size_t j = 0; j < mIslands[i].size(); ++j)
				{
					auto& area = mAreas[mIslands[i][j]];
					if (area->mIsRoom)
					{
						auto room = std::dynamic_pointer_cast<cRoom, cArea>(area);
						for (int y = 0; y < room->mDims.y; ++y)
						for (int x = 0; x < room->mDims.x; ++x)
						{
							glm::ivec2 pt = room->mStart + glm::ivec2(x, y);
							cols(pt.x, mMapData.Height() - pt.y - 1) -= randc;
							cols(pt.x, mMapData.Height() - pt.y - 1) += c;
							//cols(pt.x, mMapData.Height() - pt.y - 1) += c2*glm::ivec3(x_idx[i7], y_idx[i7], z_idx[i7]);
						}
					}
				}

				if ((i + 1) % 7 == 0)
					c2 /= 2;
			}
			for (size_t i = 0; i < mMapData.Height(); ++i)
			{
				for (size_t j = 0; j < mMapData.Width(); ++j)
				{
					auto i2 = mMapData.Height() - i - 1;
					fwrite(&cols(j, i).x, 1, 1, fp);
					fwrite(&cols(j, i).y, 1, 1, fp);
					fwrite(&cols(j, i).z, 1, 1, fp);
				}
			}

			fclose(fp);
			std::cout << line;
			std::ofstream outf("dungen.txt");
			outf << line;
			outf.close();
		}

		size_t cWorkspace::randu(const glm::ivec2& v)
		{
			assert(v.y >= v.x);
			auto vd = v.y - v.x;
			return v.x + (vd ? std::rand() % vd : 0);
		}

		bool cWorkspace::is_perimeter_corner(const glm::ivec2& v) const
		{
			const int offs[] = { -1, 1 };
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					const glm::ivec2 corner(v.x + offs[j], v.y + offs[i]);
					if (mMapData.InRange(corner))
					{
						const glm::ivec2 p1(v.x + offs[j], v.y);
						const glm::ivec2 p2(v.x, v.y + offs[i]);
						auto perims = mMapData(p1) & mMapData(p2) & mMapData(v) & eMapData::perimeter;
						auto room = mMapData(corner) & (eMapData::corridor | eMapData::room);
						if (perims && room)
							return true;
					}
				}
			}
			return false;
		}

		void cWorkspace::clear_perimeter(const cArea& area)
		{
			// declare list of points to remove
			std::vector<glm::ivec2> to_remove;

			// for each perimeter point shared with an area
			for (const auto& ppt : area.mPerimeter)
			{
				auto& perim_set = mPerimeterMap(ppt);
				for (int i = 0; i < int(perim_set.size()); ++i)
				{
					if (perim_set[i] == area.mId)
					{
						perim_set.erase(perim_set.begin() + i);
						--i;
					}
				}

				if (perim_set.empty())
				{
					// erase from the dense map
					mMapData(ppt) -= 1 << (eMapData::perimeter - 1);
				}
			}
		}

		void cWorkspace::generate_dungeon(size_t numRooms)
		{
			// Create the rooms first
			static const int max_consecutive_failures = 100;
			int failed = 0;
			size_t created_rooms = 0;
			while ((created_rooms < numRooms) && (failed < max_consecutive_failures))
			{
				auto room_ptr = cRoom::create(*this);
				if (room_ptr)
				{
					room_ptr->Init();
					room_ptr->mMaxConns = randu(mConstraints.mDoorsPerRoom);
					++created_rooms;
					failed = 0;
				}
				else
					failed++;
			}

			//---------------------------------------------
			// main loop
			for (size_t i = 0; i < mAreas.size(); ++i)
			{
				auto area_ptr = mAreas[i];
				if (!area_ptr->mIsRoom) continue; //! # TODO: control if we want subcorridors!
				size_t iConnSlot = 0;
				for (iConnSlot = 0; iConnSlot < area_ptr->mAvailConnectionSlots.size(); ++iConnSlot)
				{
					if (area_ptr->mConnections.size() >= area_ptr->mMaxConns)
						break;
					if (cArea::add_connection(area_ptr, iConnSlot))
						iConnSlot = 0;
				}
			}

			//-------------------------------------
			// Post
			/*
			for (size_t i = 0; i < mAreas.size();++i)
			for (size_t j = i + 1; j < mAreas.size(); ++j)
			std::cout << "(" << i << ", " << j << ") = " << mAreaConnDistanceMap(i, j) << std::endl;

			std::cout << "##################################################################\n";
			*/
			connect_islands2();

			/*
			std::cout << "##################################################################\n";
			for (size_t i = 0; i < mAreas.size(); ++i)
			for (size_t j = i + 1; j < mAreas.size(); ++j)
			std::cout << "(" << i << ", " << j << ") = " << mAreaConnDistanceMap(i, j) << std::endl;
			std::cout << "##################################################################\n";
			*/
		}


		typedef std::tuple<cAreaPtr, cAreaPtr, size_t> aad_t;

		void cWorkspace::calc_area_to_area_distances()
		{
			//std::cout << "###################Start! " << std::endl;
			std::vector<aad_t> aad(mAreas.size());
			for (size_t i = 0; i < mAreas.size(); ++i)
			{
				auto& aadi = aad.at(i);
				std::get<0>(aadi) = std::get<1>(aadi) = mAreas[i];
				std::get<2>(aadi) = 0;
			}

			cAreaPtr src, tgt;
			size_t d;

			while (!aad.empty())
			{
				std::tie(src, tgt, d) = aad.back();
				aad.pop_back();

				bool search_nb = false;
				auto& din = mAreaDistances(glm::ivec2(src->mId, tgt->mId));
				//std::cout << "Comp: " << src->mId << " - " << tgt->mId << " = " << d << " " << din << " " << ((d < din) ? "(true)" : "(false)") << std::endl;
				if (d <= din)
				{
					search_nb = true;
					din = d;
					mAreaDistances(glm::ivec2(tgt->mId, src->mId)) = d;
				}



				if (search_nb)
				{
					for (const auto& conn : tgt->mConnections)
					{
						auto aconn = mAreas[conn.second];
						//set_area_to_area_distance(src, conn.second, d + 1);
						if (aconn->mId != src->mId)
						{
							aad.push_back(std::make_tuple(src, aconn, d + 1));
						}
					}
				}
			}
		}
		/*
		void cWorkspace::calc_area_to_area_distances2()
		{
		for (size_t i = 0; i < mAreas.size(); ++i)
		{
		mAreaDistances(i, i) = 0;
		for (const auto& conn : mAreas.at(i)->mConnections)
		{
		auto j = conn.second->mId - 1;
		mAreaDistances(i, j) = 1;
		mAreaDistances(j, i) = 1;
		}
		}

		size_t n = mAreas.size();
		for (size_t k = 0; k < n;++k)
		for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < n; ++j)
		{
		mAreaDistances(i, j) = std::min(mAreaDistances(i, j), mAreaDistances(i, k) + mAreaDistances(k, j));
		}

		}
		*/

		void cWorkspace::calc_islands()
		{
			mAreasVisited.clear();
			mAreasVisited.resize(mAreas.size());
			for (auto it0 = mAreas.begin(); it0 != mAreas.end(); ++it0)
			{
				const auto& area0 = *it0;
				auto it1 = it0;
				++it1;
				for (it1; it1 != mAreas.end(); ++it1)
				{
					const auto& area1 = *it1;
					auto d = mAreaDistances(glm::ivec2(area0->mId, area1->mId));
					if (d == inf)
					{
						if (!mAreasVisited.at(area0->mId))
						{
							mIslands.push_back(std::vector<size_t>());
							add_to_isle(area0, mIslands.back());
						}
						if (!mAreasVisited.at(area1->mId))
						{
							mIslands.push_back(std::vector<size_t>());
							add_to_isle(area1, mIslands.back());
						}

					}
				}
			}
			if (mIslands.empty())
			{
				cIsle isle;
				for (size_t i = 0; i < mAreas.size(); ++i)
					isle.push_back(mAreas[i]->mId);
				mIslands.push_back(isle);
			}
		}

		void cWorkspace::add_to_isle(const cAreaPtr& area, cIsle& isle)
		{
			// if not visited, add it, set visited, and add_to_isle its neighbours
			std::vector<size_t> todo;
			todo.push_back(area->mId);
			while (!todo.empty())
			{
				auto cur = todo.back();
				todo.pop_back();
				auto visited = mAreasVisited.at(cur);
				if (!visited)
				{
					mAreasVisited.at(cur) = true;
					isle.push_back(cur);
					auto acur = mAreas[cur];
					for (const auto& x : acur->mConnections)
						todo.push_back(x.second);
				}
			}
		}

		void cWorkspace::calc_islands2()
		{
			mConstraints.OverrideMaxConns(true);
			mIslands.clear();

			std::vector<bool> visited(mAreas.size(), false);
			for (size_t i = 0; i < mAreas.size(); ++i)
			{
				// area and friends in island
				if (!visited.at(i))
				{
					mIslands.push_back(cIsle());
					auto& isle = mIslands.back();
					for (size_t j = 0; j < mAreas.size(); ++j)
					{
						if (mAreaConnDistanceMap(i, j) != inf)
						{
							isle.push_back(mAreas[j]->mId);
							assert(!visited.at(j));
							visited.at(j) = true;
						}
					}
				}
			}
		}

		void cWorkspace::connect_islands2()
		{
			calc_islands2();
			if (!mConstraints.mConnectIslands)
				return;
			std::sort(mIslands.begin(), mIslands.end(), [](const cIsle& i0, const cIsle& i1) { return i0.size() > i1.size(); });
			std::cout << mIslands.size() << " ";
			while (mIslands.size() > 1)
			{
				bool conn_sth = false;
				for (size_t i = 0; i < mIslands.size(); ++i)
				{
					bool res = connect_island(mIslands[i]);
					if (res)
					{
						conn_sth = true;
						break;
					}
				}
				if (!conn_sth)
					break;

				calc_islands2();
				std::cout << mIslands.size() << " ";
			}
		}

		void cWorkspace::connect_islands()
		{
			mAreaDistances.Resize(mAreas.size(), mAreas.size());
			mAreaDistances.Fill(inf);
			calc_area_to_area_distances();
			calc_islands();
			if (!mConstraints.mConnectIslands)
				return;
			std::sort(mIslands.begin(), mIslands.end(), [](const cIsle& i0, const cIsle& i1) { return i0.size() > i1.size(); });
			std::cout << mIslands.size() << " ";
			while (mIslands.size() > 1)
			{
				bool conn_sth = false;
				for (size_t i = 0; i < mIslands.size(); ++i)
				{
					bool res = connect_island(mIslands[i]);
					if (res)
					{
						conn_sth = true;
						break;
					}
				}
				if (!conn_sth)
					break;

				mIslands.clear();
				mAreasVisited.clear();

				mAreaDistances.Resize(mAreas.size(), mAreas.size());
				mAreaDistances.Fill(inf);
				calc_area_to_area_distances();
				calc_islands();
				std::cout << mIslands.size() << " ";
			}
		}

		bool cWorkspace::connect_island(cIsle& i0)
		{
			std::deque<glm::ivec2> found_path;
			std::set<size_t> exclude_ids;
			for (const auto& area : i0)
			{
				exclude_ids.insert(area);
			}

			std::vector< std::pair<cAreaPtr, size_t>> area_conns;
			area_conns.reserve(2000);
			for (const auto& area_id : i0)
			{
				auto area_ptr = mAreas[area_id];
				for (size_t iConnSlot = 0; iConnSlot < area_ptr->mAvailConnectionSlots.size(); ++iConnSlot)
				{
					area_conns.push_back(std::make_pair(area_ptr, iConnSlot));
				}
			}

			std::random_shuffle(area_conns.begin(), area_conns.end());

			bool connected = false;
			for (const auto& acpair : area_conns)
			{
				auto area_ptr = acpair.first;
				auto iConnSlot = acpair.second;

				connected = cArea::add_connection(area_ptr, iConnSlot, exclude_ids);

				if (connected)
					break;
			}
			return connected;
		}

		void cWorkspace::add_feature_entry_exit()
		{
			// pick the largest island
			size_t largest_isle_idx = 0;
			for (size_t i = 1; i < mIslands.size(); ++i)
			{
				if (mIslands[i].size() > mIslands[largest_isle_idx].size())
					largest_isle_idx = i;
			}

			// pick a room
			const cIsle& isle = mIslands[largest_isle_idx];
			auto idx = randu(glm::ivec2(0, isle.size()));
			auto it = isle.begin();
			std::advance(it, idx);
			auto area = mAreas[*it];
			while (!area->mIsRoom)
			{
				idx = randu(glm::ivec2(0, isle.size()));
				it = isle.begin();
				std::advance(it, idx);
				area = mAreas[*it];
			}

			auto area_tgt = area;
			auto area_src = area_tgt;
			size_t dmax = 0;
			if (!mConstraints.mRandomEntryExit)
			{

				// find target room
				for (const auto & islearea_id : isle)
				{
					auto islearea = mAreas[islearea_id];
					size_t d = mAreaConnDistanceMap(area->mId, islearea->mId);
					if (islearea->mIsRoom && (d > dmax))
					{
						area_tgt = islearea;
						dmax = d;
					}

				}

				// find source room
				area_src = area_tgt;
				dmax = 0;
				for (const auto & islearea_id : isle)
				{
					auto islearea = mAreas[islearea_id];
					size_t d = mAreaConnDistanceMap(area_tgt->mId, islearea->mId);
					if (islearea->mIsRoom && (d > dmax))
					{
						area_src = islearea;
						dmax = d;
					}

				}
			}
			else
			{
				auto idx = randu(glm::ivec2(0, isle.size()));
				auto it = isle.begin();
				std::advance(it, idx);
				area_src = mAreas[*it];
				while (!area_src->mIsRoom)
				{
					idx = randu(glm::ivec2(0, isle.size()));
					it = isle.begin();
					std::advance(it, idx);
					area_src = mAreas[*it];
				}
			}

			// sample locations in rooms
			bool ok = false;
			std::shared_ptr<cRoom> room = std::dynamic_pointer_cast<cRoom, cArea>(area_src);
			glm::ivec2 epos;
			while (!ok)
			{
				epos.x = randu(glm::ivec2(room->mStart.x, room->mStart.x + room->mDims.x));
				epos.y = randu(glm::ivec2(room->mStart.y, room->mStart.y + room->mDims.y));
				ok = !(mMapData(epos) & (eMapData::entry | eMapData::exit));
			}
			mMapData(epos) |= eMapData::entry;

			ok = false;
			room = std::dynamic_pointer_cast<cRoom, cArea>(area_tgt);
			while (!ok)
			{
				epos.x = randu(glm::ivec2(room->mStart.x, room->mStart.x + room->mDims.x));
				epos.y = randu(glm::ivec2(room->mStart.y, room->mStart.y + room->mDims.y));
				ok = !(mMapData(epos) & (eMapData::entry | eMapData::exit));
			}
			mMapData(epos) |= eMapData::exit;

			std::cout << "Distance : " << dmax << std::endl;
		}

	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const char* key, const rl::cWorkspace & value)
	{
		assert(false);
		//auto& child = writer.append_child(key.c_str());
		//SerializeOut(child, "Name", value.mName);
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, rl::cWorkspace & value)
	{
		size_t roomNum;
		glm::ivec2 dims;
		size_t ret = 0;
		ret += SerializeIn(reader, "ConnectIslands", value.mConstraints.mConnectIslands, true);
		ret += SerializeIn(reader, "DoorsPerRoom", value.mConstraints.mDoorsPerRoom, glm::ivec2(1, 4));
		ret += SerializeIn(reader, "DoorToDoorMinDistance", value.mConstraints.mDoorToDoorMinDistance, glm::ivec2(2, 2));
		ret += SerializeIn(reader, "OverrideMaxConns", value.mConstraints.mOverrideMaxConns, true);
		ret += SerializeIn(reader, "RandomEntryExit", value.mConstraints.mRandomEntryExit, false);
		ret += SerializeIn(reader, "RoomRectH", value.mConstraints.mRoomRectH, glm::ivec2(3, 12));
		ret += SerializeIn(reader, "RoomRectW", value.mConstraints.mRoomRectW, glm::ivec2(3, 12));
		ret += SerializeIn(reader, "StoreRoomConnectivity", value.mConstraints.mStoreRoomConnectivity, true);
		ret += SerializeIn(reader, "Rooms", roomNum, size_t(1000000));
		ret += SerializeIn(reader, "Dims", dims, glm::ivec2(80, 40));
		

		value.Init(dims.x, dims.y);
		//ws.Init(150, 50);
		//ws.Init(10, 20);
		//initscr();ws.Init(COLS-1, LINES-1);
		//timeBeginPeriod(1);
		//auto t0 = timeGetTime();
		value.generate_dungeon(roomNum);

		value.add_feature_entry_exit();

		return ret;
	}
}