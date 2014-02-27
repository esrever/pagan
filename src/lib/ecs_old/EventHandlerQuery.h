#pragma once

#include <ecs/Event.h>
#include <ecs/QueryExpression.h>

namespace pgn
{
	//! Base class for event handler queries
	class cEventHandlerQueryBase
	{
	public:
		virtual  ~cEventHandlerQueryBase(){}
		void SetQuery(cQueryExpressionSptr q) { mQuery = q; }
		virtual std::shared_ptr<cEventHandlerQueryBase> clone() const
		{
			return std::make_shared<cEventHandlerQueryBase>(*this);
		}
	protected:
		cQueryExpressionSptr mQuery;
	};
}