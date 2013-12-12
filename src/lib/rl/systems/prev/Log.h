#pragma once

#include <ecs/SystemBase.h>

#include <rl/events/events.h>

namespace pgn
{
	namespace cmp
	{
		struct cLog;
		struct cTextWindow;
		struct cOutStream;
	}

	namespace sys
	{
		class cLog : public cSystemBase
		{
		public:
			cLog();

		private:
			cEventHandler<evt::cLog> mOnLog;
		private:
			//! Receiving functions
			void OnLog(const std::string& zLoggerName, const std::string& zString);

			//! Default: blow if I don't know how to handle the log listener
			template<class T>
			void ApplyLog(T& zLogListener, const std::string& zLog) { assert(false); }

		private:
			void ApplyLogTextWin(cmp::cTextWindow& zLogListener, const std::string& zLog);
			void ApplyLogOutStream(cmp::cOutStream& zLogListener, const std::string& zLog);

		private:

			//! Find all logs
			cQueryExpressionSptr mQueryLog;

			//! Find all log listeners of type
			cQueryExpressionSptr mQueryLogListener;
		};
	}

	//---------------------------------------------------------------------------------------
	template<>
	inline void sys::cLog::ApplyLog<cmp::cTextWindow>(cmp::cTextWindow& zLogListener, const std::string& zLog)
	{
		ApplyLogTextWin(zLogListener, zLog);
	}

	//---------------------------------------------------------------------------------------
	template<>
	inline void sys::cLog::ApplyLog<cmp::cOutStream>(cmp::cOutStream& zLogListener, const std::string& zLog)
	{
		ApplyLogOutStream(zLogListener, zLog);
	}
}