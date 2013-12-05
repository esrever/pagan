#include "example.h"

#include <iostream>
#include "FontAtlas.h"
#include "TileLib.h"

#include <core/util/xml_conversions.h>

#include <rapidjson/prettywriter.h>	// for stringify JSON
#include <rapidjson/filestream.h>	// wrapper of C stream for prettywriter as output

#include <ecs/ecs.h>
#include <rl/EntityMgrRL.h>
#include <rl/SystemMgrRL.h>
#include <rl/ActionMgrRL.h>
#include <rl/oxygine/KeyboardMgr.h>

using namespace oxygine;

//it is our resources
//in real project you would have more than one Resources declarations. 
//It is important on mobile devices with limited memory and you would load/unload them
Resources gameResources;

void cTextWindow::Init(Vector2 start, Vector2 size)
{
	spColorRectSprite bg;
	spTextActor text;

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
	auto& ecs = ECS;
	ecs.mLog = std::dynamic_pointer_cast<pgn::cLogBase>(std::make_shared<pgn::cLogString>(pgn::cLogString()));
	ecs.Init();

	KeyboardInstance;

	//! Init log stuff. TODO: Do I need a proper entity? SysLogWindow e.g.
	//ecs.mLog = std::dynamic_pointer_cast<pgn::cLogBase>(std::make_shared<pgn::cLogString>(pgn::cLogString()));
	auto log_ptr = std::make_shared<pgn::cLogStream>(pgn::cLogStream());
	log_ptr->SetChannel(&std::cout);
	ecs.mLog = std::dynamic_pointer_cast<pgn::cLogBase>(log_ptr);

	//! Init ecs
	ecs.Init();
	ecs.mSystemMgr = std::shared_ptr<pgn::cSystemMgr>(new pgn::cSystemMgrRL());
	ecs.mEntityMgr = std::shared_ptr<pgn::cEntityMgr>(new pgn::cEntityMgrRL());
	ecs.mActionMgr = std::shared_ptr<pgn::cActionMgr>(new pgn::cActionMgrRL());

	//! register custom types
	ecs.mEntityMgr->RegisterComponentTypes();

	//! register system types
	ecs.mSystemMgr->RegisterSystemTypes();

	//! register action types
	ecs.mActionMgr->RegisterActionTypes();

	//! Read ecs configuration
	//ecs.SetDataPath("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\rl\\data\\");
	ecs.from_json(*pgn::file_to_json(ecs.GetDataPath() + "ecs.json"));

	//###############################
	//# Demo
	// Create 2 entities, a rat and a human
	ecs.mEntityMgr->InstantiateExemplar("rat_common");
	ecs.mEntityMgr->InstantiateExemplar("human_common");

	FILE * fp = fopen("ecs_export.txt", "wt");
	rapidjson::StringBuffer strbuf;
	JsonWriter writer(strbuf);
	to_json(ecs, writer);
	const char * jsontext = strbuf.GetString();
	fwrite(jsontext, 1, strlen(jsontext), fp);
	fclose(fp);


	//load xml file with resources definition
	gameResources.loadXML("res.xml");

	
	//##########################
	// General

	auto win = new Actor;
	win->setPosition(Vector2(0, 0));
	win->setSize(mWinWidth, mWinHeight);
	getRoot()->addChild(win);

	//##########################
	// STATUS & LOG

	auto statusBoxPosition = Vector2(mTileSize*mTileCols, 0);
	auto statusBoxSize = Vector2(mWinWidth - mTileSize*mTileCols, mWinHeight);
		
	auto logBoxPosition = Vector2(0, mTileSize*mTileRows);
	auto logBoxSize = Vector2(mWinWidth, mWinHeight-mTileSize*mTileRows);

	spTextWindow status = new cTextWindow();
	win->addChild(status);
		
	spTextWindow log = new cTextWindow();
	win->addChild(log);

	status->Init(statusBoxPosition, statusBoxSize);
	status->Init(logBoxPosition, logBoxSize);

	//##########################
	// MAP
	static cTileLib tilelib;
	tilelib.Init(gameResources.getResAnim("tilemap"));

	spFontAtlas fontmap = new cFontAtlas();
	fontmap->Init(gameResources.getResAnim("font_atlas"),mTileSize);

	auto it = tilelib.GetMap().begin();
	for (int i = 0; i < mTileRows;++i)
		for (int j = 0; j < mTileCols; ++j)
		{
			while (it != tilelib.GetMap().end())
			{
				if (pystring::find(it->first, "floor") != -1)
				{
					spSprite sprite = it->second;
					win->addChild(sprite);
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
	auto& ecs = ECS;
	ecs.mSystemMgr->UpdateFrame();
}

void cApplication::Destroy()
{
	auto& ecs = ECS;

	gameResources.free();

	ecs.Destroy();
}