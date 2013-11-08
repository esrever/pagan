#pragma once

#include <ecs/SystemBase.h>

namespace pgn
{
	class cAsciiStatusRenderSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual bool from_json(const rapidjson::Value& zRoot);
		virtual void to_json(JsonWriter& zRoot) const;
	private:

		cQueryExpressionSptr mQuery;
		
	};
}