#include "TextWindow.h"

using namespace oxygine;

namespace pgn
{
	void cmp::cTextWindow::SetText(const std::string& zText)
	{
		mText->setHtmlText(zText);
	}

	//----------------------------------------------------------------------------------
	void cmp::cTextWindow::Init()
	{
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
		mBg->setPosition(mStart);

		// Create the text sprite -- Attach to bg
		mText = new TextActor;
		mText->attachTo(mBg);
		mText->setStyle(mStyle);
		mText->setSize(mSize);
		mText->setHtmlText("");
	}

	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cmp::cTextWindow>( cmp::cTextWindow& zData, const rapidjson::Value& zRoot)
	{
		
		zData.Init();
        return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cmp::cTextWindow>( const cmp::cTextWindow& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
        
		zRoot.EndObject();
	}
}