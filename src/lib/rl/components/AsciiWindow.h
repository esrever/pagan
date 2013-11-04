#pragma once

#include <string>
#include <vector>

#include <curses.h>
#include <glm/glm.hpp>

#include <core/util/json_conversions.h>

namespace pgn
{
	struct cAsciiWindow
	{
	public:
		cAsciiWindow();
		~cAsciiWindow() {Destroy();}

		void Destroy();
		void Init();

		std::shared_ptr<WINDOW *>	   mWindow;
		glm::ivec2				   mStart;
		glm::uvec2				   mSize;
	};

	//-------------------------------------------------------------------
	template<>
	bool from_json<cAsciiWindow>(cAsciiWindow& zObj, const rapidjson::Value& zRoot);
}