#include "TextWindow.h"
#include <rl/util/json_conversions.h>

using namespace oxygine;

namespace pgn
{
	void cmp::cTextWindow::SetText(const std::string& zText)
	{
		mText->setHtmlText(zText);
	}

	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cTextWindow>( cmp::cTextWindow& zData, const rapidjson::Value& zRoot)
	{
		zData.mBg = new ColorRectSprite();
		zData.mText = new TextActor;

		from_json(zData.mStyle, zRoot["style"]);
		from_json(zData.mBg, zRoot["bg"]);
		from_json(zData.mText, zRoot["text"]);
		from_json(zData.mPos, zRoot["pos"]);
		from_json(zData.mSize, zRoot["size"]);

		oxygine::getRoot()->addChild(zData.mBg);
		zData.mText->attachTo(zData.mBg);
		zData.mText->setStyle(zData.mStyle);
		
		zData.mText->setSize(zData.mSize);
		zData.mBg->setSize(zData.mSize);
		zData.mBg->setPosition(zData.mPos);
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cTextWindow>( const cmp::cTextWindow& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("style", zData.mStyle, zRoot);
		JsonWriter_AddMember("bg", zData.mBg, zRoot);
		JsonWriter_AddMember("text", zData.mText, zRoot);
		JsonWriter_AddMember("pos", zData.mPos, zRoot);
		JsonWriter_AddMember("size", zData.mSize, zRoot);
		zRoot.EndObject();
	}
}