#pragma once

#include <ecs/SystemBase.h>

#include <rl/events/events.h>

namespace pgn
{
	namespace sys
	{
		class cLogDisplay : public cSystemBase
		{
		public:
			cLogDisplay();
			virtual void Process();
			virtual bool from_json(const rapidjson::Value& zRoot);
			virtual void to_json(JsonWriter& zRoot) const;

		private:
			//! Event handlers
		private:
			//! Receiving functions
		private:
			//! Gets all entities that have a log
			cQueryExpressionSptr mQueryLog;
			
			//! Gets all entities that have a text window AND are tagged with "log:${logname}"
			cQueryExpressionSptr mQueryTextWin;
		};
	}
}