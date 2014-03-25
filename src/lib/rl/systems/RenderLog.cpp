#include "RenderLog.h"

#include <core/texture/texturelib.h>
#include <core/texture/subtexturelib.h>
#include <core/sdl/util.h>
#include <core/sdl/font.h>

#include <rl/event/events.h>
#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cRenderLog::operator()()
			{
				static const int sTextHeight = 24;
				pgn::cSDLFont font(mWindow->Renderer(), PROJECT_ROOT "data\\fonts\\SourceSansPro\\SourceSansPro-Regular.otf", 32);
				//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\PT-Sans\\PTN57F.ttf", 62);
				//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\Nobile\\Nobile-Regular.ttf", 32);

				SDL_Rect textRect;
				auto& gamelog = pgn::mainapp()->GameLog();
				size_t i = 0;
				for (const auto& line : gamelog.Data())
				{
					int yo = mArea.mStart.y + i*sTextHeight;
					if ((yo + sTextHeight) > (mWindow->Rect().h - 5))
						break;

					auto texf = font.CreateText(line, &textRect);
					float ar = textRect.w / float(textRect.h);

					SDL_Rect rect = { mArea.mStart.x, yo, int(ar * sTextHeight), sTextHeight };
					mWindow->Render(texf.get(), &rect);
					++i;
				}

				return true;
			}
		}
	}
}