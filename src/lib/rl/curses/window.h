#pragma once

#include <string>
#include <vector>

#include <curses.h>
#include <glm/glm.hpp>

#include <core/util/json_conversions.h>

namespace pgn
{
	namespace curses
	{
		class cWindow
		{
		public:
			cWindow() :mWindow(nullptr){}
			~cWindow() { Destroy(); }

			void Destroy();
			void Init(const glm::ivec2& zStart, const glm::uvec2& zSize, WINDOW * parent = nullptr);
			void Init(WINDOW * parent = nullptr);

			WINDOW * Ptr() const { return mWindow; }

		private:
			template<class T>
			friend bool from_json(T& zObj, const rapidjson::Value& zRoot);
		private:
			WINDOW *				   mWindow;
			glm::ivec2				   mStart;
			glm::uvec2				   mSize;
			std::string				   mName;
			std::vector<cCursesWindow> mSubWindows;
		};
	}
	//-------------------------------------------------------------------
	template<>
	bool from_json<cCursesWindow>(cCursesWindow& zObj, const rapidjson::Value& zRoot);
}