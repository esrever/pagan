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
	/*
		// Create the text style
		// TODO: keep a fontLib, its own xml
		//mStyle.font = cFontLib::Get("system") -> fontResources.getResFont("system")->getFont(); 
		mStyle.vAlign = TextStyle::VALIGN_TOP;
		mStyle.color = Color(0xffffffff);
		mStyle.multiline = true;

		// Create the bg sprite -- Attach to window
		mBg = new ColorRectSprite();
		mBg->attachTo(this);
		mBg->setColor(Color(64, 64, 64, 255));
		mBg->setSize(mSize);
		mBg->setPosition(mPos);

		// Create the text sprite -- Attach to bg
		mText = new TextActor;
		mText->attachTo(mBg);
		mText->setStyle(mStyle);
		mText->setSize(mSize);
		mText->setHtmlText("");
	*/

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

		zData.mBg->attachTo(&zData);
		zData.mText->attachTo(zData.mBg);
		zData.mText->setStyle(zData.mStyle);
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