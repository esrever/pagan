#include "FontAtlas.h"

using namespace oxygine;

void cFontAtlas::Init(ResAnim * anim)
{
	mFontSprites.resize( anim->getTotalFrames());
	for (int i = 0; i < 66;++i)
		for (int j = 0; j < 16; ++j)
		{
			int o = i * 16 + j;
			spSprite& sprite = mFontSprites.at(o);
			sprite = new Sprite();
			addChild(sprite);
			sprite->setAnimFrame(anim->getFrame(j, i));
		}
}