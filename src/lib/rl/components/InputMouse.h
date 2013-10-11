#pragma once

#include <glm/glm.hpp>

namespace pgn
{
	//! Stores mouse input information
	struct cInputMouse
	{
		glm::ivec2 mCoords;
		// TODO: button state
	};
}