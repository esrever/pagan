#pragma once

#include <memory>

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/ecs.h>
#include <core/app/sdlwin.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cRenderMainWin : public cBase
			{
			public:
				bool operator()();
				void SetWindow(cSDLWindow * win) { mWindow = win; }
			private:
			private:

				cSDLWindow * mWindow;
			};
		}
	}
}