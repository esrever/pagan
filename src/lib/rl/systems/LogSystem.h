#pragma once

#include <ecs/SystemBase.h>

#include <rl/events/events.h>

namespace pgn
{
	namespace sys
	{
		class cLog : public cSystemBase
		{
		public:
			cLog();
			virtual void Process();
			virtual bool from_json(const rapidjson::Value& zRoot);
			virtual void to_json(JsonWriter& zRoot) const;

		private:
			cEventHandler<evt::cLog> mOnLog;
		private:
			//! Receiving functions
			void OnLog(const std::string& zString);
		private:

			cQueryExpressionSptr mQuery;
		};
	}
}