#pragma once 

#include <memory>

#include <core/util/logstream.h>


namespace pgn
{
	struct cAsciiWindow;

	class cLogCurses : public cLogBase
	{
	public:
			cLogCurses():mWindow(nullptr){}
			void SetWindow(std::shared_ptr<cAsciiWindow> zWin) {mWindow = zWin;}
			virtual void Log(const eLogLevel zLevel, const std::string& zMsg);
	private:

		std::shared_ptr<cAsciiWindow> mWindow;
	};
}