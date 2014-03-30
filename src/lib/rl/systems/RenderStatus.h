#pragma once

#include <memory>

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/ecs.h>
#include <core/app/sdlwin.h>
#include <rl/event/events.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cRenderStatus : public cBase
			{
			public:
				cRenderStatus();
				bool operator()();
				void SetWindow(cSDLWindow * win) { mWindow = win; }
				void SetArea(const cBox2i& area) { mArea = area; }
				void OnMouseOverCell(const glm::ivec2& pos);
			private:

			private:
				DECL_EVT_MEMBER(MouseOverCell);

				glm::ivec2 mMouseOverCell;

				cSDLWindow * mWindow;
				cBox2i mArea;
			};
		}
	}
}