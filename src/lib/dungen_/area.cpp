#include "area.h"

#include <iostream>

#include "util.h"
#include "dungen.h"
#include "corr.h"
#include "room.h"

namespace pgn
{
	namespace dungen
	{

		cArea::cArea(cWorkspace& ws) :mWorkspace(ws)
		{
			mId = ws.mAreas.size();
			ws.mAreaConnDistanceMap.Resize(mId + 1, cWorkspace::inf);
			ws.mAreaConnDistanceMap(mId, mId) = 0;
		}

		//-----------------------------------------------------
		void cArea::update_perimeter()
		{
			mWorkspace.clear_perimeter(*this);
			mPerimeter.clear();
		}

		bool cArea::add_connection(ptr_type area, size_t slot_idx, const std::set<size_t>& excludeIds)
		{
			bool ret = false;
			auto& ws = area->mWorkspace;
			const auto& conn = area->mAvailConnectionSlots.at(slot_idx);
			
			cAnyGoalFirst agf(ws, conn, area->mId, excludeIds);
			auto cross = ai::cShapeCalc<ai::cDiamondDistance>::Get(1, 1);
			auto gfunc = GenerateMoveCostFunction(ws, 0xFFFFFFFF, 0);
			bool res = ws.mAstar.Search(conn, agf, ws.mAstarPath, ws.mMapData.View().Shape(), gfunc, cross);
			if (res)
			{
				if (ws.mAstarPath.size() == 1)
				{
					// No corridor, but shared connection
					const auto& area_ids = ws.mConnectionMap(conn);
					assert(area_ids.size() == 2);
					auto area0 = area->mWorkspace.mAreas[area_ids[0]];
					auto area1 = area->mWorkspace.mAreas[area_ids[1]];
					auto id0 = area0->mId;
					auto id1 = area1->mId;
					area0->mConnections[conn] = id1;
					area1->mConnections[conn] = id0;
					ws.mMapData(conn) |= eMapData::conn;
					area0->calc_avail_connection_slots();
					area1->calc_avail_connection_slots();
					assert(area0->mId != area1->mId);
					ret = true;

					ws.mAreaConnDistanceMap(id1, id0) = 1;
					ws.mAreaConnDistanceMap.update_set(id0, id1);
				}

				else if (ws.mAstarPath.size() <= 2)
				{
					std::cout << "Astar path = 2" << std::endl;
				}

				else
				{
					const auto conn_nb = ws.mAstarPath.back();
					ws.mMapData(conn) |= eMapData::conn;
					ws.mMapData(conn_nb) |= eMapData::conn;

					ws.mAstarPath.pop_back();
					ws.mAstarPath.pop_front();
					auto corr_ptr = cCorr::ptr_type(new  cCorr(ws, ws.mAstarPath));
					ws.mAreas.push_back(corr_ptr);
					auto area0 = area;

					const auto& area_ids = ws.mConnectionMap(conn_nb);
					assert(area_ids.size() == 1);
					auto area1 = area->mWorkspace.mAreas[area_ids[0]];
					corr_ptr->Init();
					area0->mConnections[conn] = corr_ptr->mId;
					area1->mConnections[conn_nb] = corr_ptr->mId;
					corr_ptr->mConnections[conn] = area0->mId;
					corr_ptr->mConnections[conn_nb] = area1->mId;

					//std::cout << "Connection: " << room->mId << ", " << room_nb->mId << std::endl;
					area0->calc_avail_connection_slots();
					area1->calc_avail_connection_slots();

					assert(area0->mId != area1->mId);

					ret = true;

					auto id0 = area0->mId;
					auto id1 = area1->mId;
					auto idc = corr_ptr->mId;
					ws.mAreaConnDistanceMap(idc, id0) = 1;
					ws.mAreaConnDistanceMap(idc, id1) = 1;
					ws.mAreaConnDistanceMap.update_single(idc, id0);
					ws.mAreaConnDistanceMap.update_single(idc, id1);
					ws.mAreaConnDistanceMap.update_set(id0, id1, 1);
				}
			}
			return ret;
		}
	}
}