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
			class cRenderLog : public cBase
			{
			public:
				bool operator()();
				void SetWindow(cSDLWindow * win) { mWindow = win; }
				void SetArea(const cBox2i& area) { mArea = area; }
			private:
				void RenderLine(const std::string& s);
			private:

				cSDLWindow * mWindow;
				cBox2i mArea;
			};
		}
	}
}