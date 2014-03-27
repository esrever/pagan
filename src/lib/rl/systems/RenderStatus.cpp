#include "RenderStatus.h"

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
			bool cRenderStatus::operator()()
			{
				static const int sTextHeight = 24;
				pgn::cSDLFont font(mWindow->Renderer(), PROJECT_ROOT "data\\fonts\\SourceSansPro\\SourceSansPro-Regular.otf", 32);
				//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\PT-Sans\\PTN57F.ttf", 62);
				//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\Nobile\\Nobile-Regular.ttf", 32);

				std::vector<std::string> lines;
				auto hero = mainecs()->TagusToEntities("Player")->second;
				const auto& herodata = hero->second.Component<ecs::cmp::cCreature>()->mCreatureData;
				lines.push_back(format("HP: %d/%d", herodata.mCurHealth, herodata.mMaxHealth));
				lines.push_back(format("XP: %d", herodata.mXP));
				lines.push_back(format("Level: %d", herodata.mLevel));
				lines.push_back(format("Str: %d", herodata.mCurStats[Enum2Int(rl::eStats::Str)]));
				lines.push_back(format("Dex: %d", herodata.mCurStats[Enum2Int(rl::eStats::Dex)]));
				lines.push_back(format("Con: %d", herodata.mCurStats[Enum2Int(rl::eStats::Con)]));
				


				SDL_Rect textRect;
				size_t i = 0;
				for (const auto& line : lines)
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