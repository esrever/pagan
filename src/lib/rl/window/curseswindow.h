#pragma once

#include <glm/glm.hpp>

namespace pgn
{
	class cCursesWindow
	{
	public:
		cCursesWindow():mWindow(nullptr){}
		~cCursesWindow() {Destroy();}

		void Destroy();
		void Init(const glm::ivec2& zStart, const glm::uvec2& zSize);

	private:
		WINDOW * mWindow;
		glm::ivec2 mStart;
		glm::uvec2 mSize;
	}
}