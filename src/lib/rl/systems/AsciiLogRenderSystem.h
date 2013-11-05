#pragma once

#include <ecs/SystemBase.h>

namespace pgn
{
	class cAsciiLogRenderSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual bool from_json(const rapidjson::Value& zRoot);
	private:
		
		cQueryExpressionSptr mQuery;
	};
}