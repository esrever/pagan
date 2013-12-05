#pragma once

#include <ecs/SystemBase.h>

#include <rl/events/events.h>

namespace pgn
{
	namespace cmp
	{
		struct cLog;
		struct cTextWin;
		struct cOutStream;
	}

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
			void OnLog(const std::string& zLoggerName, const std::string& zString);

			//! Default: blow if I don't know how to handle the log listener
			template<class T>
			void ApplyLog(T& zLogListener, const cmp::cLog& zLog) { assert(false); }

		private:
			void ApplyLogTextWin(cmp::cTextWin& zLogListener, const cmp::cLog& zLog);
			void ApplyLogOutStream(cmp::cOutStream& zLogListener, const cmp::cLog& zLog);

		private:

			//! Find all logs
			cQueryExpressionSptr mQueryLog;

			//! Find all log listeners of type
			cQueryExpressionSptr mQueryLogListener;
		};
	}

	//---------------------------------------------------------------------------------------
	template<>
	inline void sys::cLog::ApplyLog<cmp::cTextWin>(cmp::cTextWin& zLogListener, const cmp::cLog& zLog)
	{
		ApplyLogTextWin(zLogListener, zLog);
	}

	//---------------------------------------------------------------------------------------
	template<>
	inline void sys::cLog::ApplyLog<cmp::cOutStream>(cmp::cOutStream& zLogListener, const cmp::cLog& zLog)
	{
		ApplyLogOutStream(zLogListener, zLog);
	}
}