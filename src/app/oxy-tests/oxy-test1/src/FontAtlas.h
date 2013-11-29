#pragma once

#include <vector>

#include "oxygine-framework.h"

class cFontAtlas : public oxygine::Actor
{
public:
	void Init(oxygine::ResAnim * res);

private:

	std::vector<oxygine::spSprite>	mFontSprites;
};