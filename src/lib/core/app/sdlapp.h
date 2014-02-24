#pragma once

#include <memory>

#include <SDL.h>

namespace pgn
{
	class cSDLWindow;

	class cSDLApp
	{
	public:
		cSDLApp(int argc, char ** argv);
		virtual ~cSDLApp();
		void Run();

		virtual void Init(){}
		virtual void Render(){}
		virtual bool Update(){ return false; }
		virtual void Destroy(){}
		
		bool HandleEvents();

		const std::shared_ptr<cSDLWindow>& MainWindow() const { return mMainWindow; }
	private:
		std::shared_ptr<cSDLWindow> mMainWindow;
	};
}