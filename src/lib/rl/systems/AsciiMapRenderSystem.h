#pragma once

#include <ecs/SystemBase.h>

namespace pgn
{
	class cAsciiMapRenderSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual bool from_json(const rapidjson::Value& zRoot);
	private:

		cQueryExpressionSptr mQueryWin;
		cQueryExpressionSptr mQueryLvl;
		cQueryExpressionSptr mQueryChar;
		
	};
}