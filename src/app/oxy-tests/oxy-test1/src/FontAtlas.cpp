#include "FontAtlas.h"

using namespace oxygine;

void cFontAtlas::Init(ResAnim * anim, size_t tileSize)
{
	mFontSprites.resize( anim->getTotalFrames());
	for (int i = 0; i < 6;++i)
		for (int j = 0; j < 16; ++j)
		{
			int o = i * 16 + j;
			spSprite& sprite = mFontSprites.at(o);
			sprite = new Sprite();
			addChild(sprite);
			sprite->setAnimFrame(anim->getFrame(j, i));
			Vector2 frame_size = sprite->getAnimFrame().getFrameSize();
			sprite->setScaleX(tileSize / frame_size.x);
			sprite->setScaleY(tileSize / frame_size.y);
		}
}

oxygine::spSprite cFontAtlas::Get(const char c) const
{
	return mFontSprites.at(c-32)->clone();
}