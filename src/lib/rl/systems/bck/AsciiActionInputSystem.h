#pragma once

#include <ecs/SystemBase.h>
#include <ecs/QueryExpression.h>

namespace pgn
{
	class cAsciiActionInputSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual bool from_json(const rapidjson::Value& zRoot);
		virtual void to_json(JsonWriter& zRoot) const;
	private:

		cQueryExpressionSptr mQuery;

		wchar_t mCurChar;
		
	};
}