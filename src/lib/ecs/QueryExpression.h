#pragma once

#include "QueryBase.h"
#include "EntityComponents.h"

namespace pgn
{
	class cQueryExpressionBase : public cQueryBase
	{
	public:
		
		virtual ~cQueryExpressionBase(){}
		
		//! TODO: Get all the entities that match the expression
		virtual void Evaluate() {}
		
	};
}