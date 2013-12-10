#pragma once

#include <ecs/EntityData.h>
#include <core/container/Array2D.h>

namespace pgn
{
	//--------------------------------------------------------------
	template<class C, class Visitor>
	void VisitArrayComponent(cArray2D<cEntityWithData>& data, Visitor& v)
	{
		for (auto i = 0; i < data.Height(); ++i)
		{
			for (auto j = 0; j < data.Width(); ++i)
			{
				std::shared_ptr<C> ptr;
				data(j, i)->second.mComponents.GetComponent(ptr);
				v.visit(j, i, ptr->mData);
			}
		}
		
	}
}