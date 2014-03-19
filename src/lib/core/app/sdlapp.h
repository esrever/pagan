#pragma once

#include <cassert>
#include <memory>
#include <string>

#include <SDL.h>
#undef main

#include <core/util/logstream.h>

namespace pgn
{
	class cSDLWindow;
	namespace ecs{ class cECS;  }

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

		cLogStream& SysLog() { return mSysLog; }
		cLogString& GameLog() { return mGameLog; }

		template<class T>
		T * Resources(const std::string& name = "");

		const std::shared_ptr<cSDLWindow>& MainWindow() const { return mMainWindow; }
	private:
		std::shared_ptr<cSDLWindow> mMainWindow;
		cLogStream mSysLog;
		cLogString mGameLog;
	};
    
    //----------------------------------------------------------------------
    /* 
        derive from SDLApp
    */
    struct cMain
    {
        cSDLApp * mApp;
		ecs::cECS * mECS;
    };
    
	cSDLApp *& mainapp();
	ecs::cECS *& mainecs();
}

