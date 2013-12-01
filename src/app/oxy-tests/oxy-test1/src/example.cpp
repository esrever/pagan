#include "oxygine-framework.h"
#include "example.h"
#include "FontAtlas.h"

#include <rapidjson/prettywriter.h>	// for stringify JSON
#include <rapidjson/filestream.h>	// wrapper of C stream for prettywriter as output

#include <ecs/ecs.h>
#include <rl/EntityMgrRL.h>
#include <rl/SystemMgrRL.h>
#include <rl/ActionMgrRL.h>

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

//DECLARE_SMART is helper, it does forward declaration and declares intrusive_ptr typedef for your class
DECLARE_SMART(MainActor, spMainActor);

class MainActor: public Actor
{
public:
	spTextActor _text;

	MainActor()
	{	
		//create Button actor
		spButton button = new Button();
		//add it as child to current actor
		addChild(button);

		//you will find 'button' resource definition in res.xml
		//button has 3 columns for each state: Normal, Pressed, Overed
		button->setResAnim(gameResources.getResAnim("button"));
		//centered button at screen	
		Vector2 pos = getRoot()->getSize()/2 - button->getSize()/2;
		button->setPosition(pos);
		button->setInputChildrenEnabled(false);

		//handle click to button
		EventCallback cb = CLOSURE(this, &MainActor::displayClicked);
		button->addEventListener(TouchEvent::CLICK, cb);

		//second part

		//create Actor with Text and it to button as child
		spTextActor text = new TextActor();
		text->attachTo(button);
		//centered in button
		text->setPosition(button->getSize()/2);

		//initialize text style
		//it would be centered and colored
		TextStyle style;
		style.font = gameResources.getResFont("main")->getFont();
		// TODO: color doesn't work? aha might be related to html? better do html colouring
		style.color = Color(72, 61, 139, 255);
		style.vAlign = TextStyle::VALIGN_MIDDLE;
		style.hAlign = TextStyle::HALIGN_CENTER;

		text->setStyle(style);
		text->setHtmlText("click\nme!");
	}

	void displayClicked(Event *event)
	{
		//lets create and run sprite with animation
		runSprite();
	}

	void runSprite()
	{
		spSprite sprite = new Sprite();		
		addChild(sprite);

		int duration = 500;//500 ms 
		int loops = -1;//infinity loops

		//animation has 7 columns, check res.xml
		ResAnim *animation = gameResources.getResAnim("anim");

		//add animation tween to sprite
		//TweenAnim would change animation frames
		sprite->addTween(TweenAnim(animation), duration, loops);

		Vector2 destPos = getRoot()->getSize() - sprite->getSize();
		Vector2 srcPos = Vector2(0, destPos.y);
		//set sprite initial position
		sprite->setPosition(srcPos);		

		//add another tween: TweenQueue
		//TweenQueue is a collection of tweens
		spTweenQueue tweenQueue = new TweenQueue();
		tweenQueue->setDelay(1500);
		//first, move sprite to dest position
		tweenQueue->add(Sprite::TweenPosition(destPos), 1500, 1);
		//then fade it out smoothly
		tweenQueue->add(Sprite::TweenAlpha(0), 1000, 1);
		
		sprite->addTween(tweenQueue);

		//and remove sprite from tree when tweenQueue is empty
		//if you don't hold any references to sprite it would be deleted automatically
		tweenQueue->setDetachActor(true);		
	}
};

void cApplication::Init()
{
	auto& ecs = ECS;
	ecs.mLog = std::dynamic_pointer_cast<pgn::cLogBase>(std::make_shared<pgn::cLogString>(pgn::cLogString()));
	ecs.Init();

	//! Init log stuff. TODO: Do I need a proper entity? SysLogWindow e.g.
	ecs.mLog = std::dynamic_pointer_cast<pgn::cLogBase>(std::make_shared<pgn::cLogString>(pgn::cLogString()));

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
	spFontAtlas fontmap = new cFontAtlas();
	//win->addChild(fontmap); // TODO: do I need to attach it to the scene graph?
	fontmap->Init(gameResources.getResAnim("font_atlas"),mTileSize);
	for (int i = 0; i < mTileRows;++i)
		for (int j = 0; j < mTileCols; ++j)
		{
			spSprite sprite = fontmap->Get(32 + ((i * mTileCols + j) % 96));
			win->addChild(sprite);

			//set sprite initial position
			Vector2 sprite_pos(j*mTileSize, i*mTileSize);
			sprite->setPosition(sprite_pos);
		}
		

		


	//lets create our client code simple actor
	//prefix 'sp' here means it is intrusive Smart Pointer
	//it would be deleted automatically when you lost ref to it	
	spMainActor actor = new MainActor;
	//and add it to RootActor as child
	getRoot()->addChild(actor);
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