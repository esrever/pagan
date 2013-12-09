#include "example.h"

#include <rl/GameApp.h>


using namespace oxygine;

//it is our resources
//in real project you would have more than one Resources declarations. 
//It is important on mobile devices with limited memory and you would load/unload them
Resources gameResources;

void cTextWindow::onMotion(Event * evt)
{
	TouchEvent * evt2 = reinterpret_cast<TouchEvent *>(evt);
	auto pos = evt2->getPointer()->getPosition();
	auto s = boost::str(boost::format("%f %f") % pos.x%pos.y);
	text->setText(s.c_str());
}

void cTextWindow::Init(Vector2 start, Vector2 size)
{
	spColorRectSprite bg;

	// Create the text style
	TextStyle st;
	st.font = gameResources.getResFont("system")->getFont();
	st.vAlign = TextStyle::VALIGN_TOP;
	st.color = Color(0xffffffff);
	st.multiline = true;

	// Create the bg sprite -- Attach to window
	bg = new ColorRectSprite();
	bg->attachTo(this);
	bg->setColor(Color(64, 64, 64, 255));
	bg->setSize(size);
	bg->setPosition(start);

	// Create the text sprite -- Attach to bg
	text = new TextActor;
	text->attachTo(bg);
	text->setStyle(st);
	text->setSize(size);
	text->setHtmlText("This is <div color='ab0000'> colored text </div>supports with broken \nline. -- Hello this is a first message -- Hello this is a first message -- Hello this is a first message -- Hello this is a first message -- \nLine new\nMore line");
}

/*
	Notes:
		order of addChild matters! otherwise stuff like onClick do not work

*/

void cApplication::Init()
{
	GAME.Init();

	//load xml file with resources definition
	gameResources.loadXML("res.xml");
	
	//##########################
	// General

	//auto win = new Actor;
	//win->setPosition(Vector2(0, 0));
	//win->setSize(mWinWidth, mWinHeight);
	//getRoot()->addChild(win);

	//##########################
	// STATUS & LOG

	auto statusBoxPosition = Vector2(mTileSize*mTileCols, 0);
	auto statusBoxSize = Vector2(mWinWidth - mTileSize*mTileCols, mWinHeight);
		
	auto logBoxPosition = Vector2(0, mTileSize*mTileRows);
	auto logBoxSize = Vector2(mWinWidth, mWinHeight-mTileSize*mTileRows);

	spTextWindow status = new cTextWindow();
	getRoot()->addChild(status);
		
	mTextWin = new cTextWindow();
	getRoot()->addChild(mTextWin);

	status->Init(statusBoxPosition, statusBoxSize);
	mTextWin->Init(logBoxPosition, logBoxSize);

	//##########################
	// MAP
	const auto& tilelib = GAME.GetResources().mTileLib;
	//tilelib.Init(gameResources.getResAnim("tilemap"));

	//spFontAtlas fontmap = new cFontAtlas();
	//fontmap->Init(gameResources.getResAnim("font_atlas"),mTileSize);

	auto it = tilelib.GetMap().begin();
	for (int i = 0; i < mTileRows;++i)
		for (int j = 0; j < mTileCols; ++j)
		{
			while (it != tilelib.GetMap().end())
			{
				//if (pystring::find(it->first, "floor") != -1)
				{
					spSprite sprite = it->second;
					mSprites.push_back(sprite);
					getRoot()->addChild(sprite);
					//set sprite initial position
					Vector2 sprite_pos(j*mTileSize, i*mTileSize);
					sprite->setPosition(sprite_pos);
					++it;
					break;
				}
				++it;
			}
		}
}

void cApplication::Update()
{
	//auto& ecs = ECS;
	//ecs.mSystemMgr->UpdateFrame();
	GAME.UpdateFrame();

	for (auto x : mSprites)
	{
		auto iso = x->getOvered();
		if (iso)
			mTextWin->text->setText(x->getName());
	}
}

void cApplication::Destroy()
{
	gameResources.free();

	GAME.Destroy();
}