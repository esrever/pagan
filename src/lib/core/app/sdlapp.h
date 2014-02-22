#pragma once

#include <memory>

namespace pgn
{
	class cSDLWindow;

	class cSDLApp
	{
	public:
		cSDLApp(int argc, char ** argv);
		virtual ~cSDLApp();
		virtual void Run();
	private:
		std::shared_ptr<cSDLWindow> mMainWindow;
	};
}