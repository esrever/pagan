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
	private:

		cQueryExpressionSptr mQuery;

		wchar_t mCurChar;
		
	};
}