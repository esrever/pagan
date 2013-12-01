#pragma once

#include <vector>

#include "oxygine-framework.h"

DECLARE_SMART(cFontAtlas, spFontAtlas);

class cFontAtlas : public oxygine::Actor
{
public:
	void Init(oxygine::ResAnim * res, size_t tileSize);
	oxygine::spSprite Get(const char c) const;

private:

	std::vector<oxygine::spSprite>	mFontSprites;
};