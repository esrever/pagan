#include "example.h"

#include <rl/GameApp.h>
#include <rl/events/events.h>


using namespace oxygine;

/*
	Notes:
		order of addChild matters! otherwise stuff like onClick do not work

*/

void cApplication::Init()
{
	GAME.Init();
}

void cApplication::Update()
{
	//auto& ecs = ECS;
	//ecs.mSystemMgr->UpdateFrame();
	GAME.UpdateFrame();
}

void cApplication::Destroy()
{
	GAME.Destroy();
}