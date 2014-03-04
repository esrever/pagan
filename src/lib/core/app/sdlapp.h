#pragma once

#include <cassert>
#include <memory>
#include <string>

#include <SDL.h>
#undef main

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

		template<class T>
		T * Resources(const std::string& name = "");// { assert(false); return nullptr; } // TODO: why does this work?

		const std::shared_ptr<cSDLWindow>& MainWindow() const { return mMainWindow; }
	private:
		std::shared_ptr<cSDLWindow> mMainWindow;
	};
    
    //----------------------------------------------------------------------
    /* 
        derive from SDLApp
    */
    struct cMain
    {
        cSDLApp * mApp;
    };
    
    static inline cSDLApp *& mainapp()
    {
        static cMain mainapp_inst;
        return mainapp_inst.mApp;
    }
}

