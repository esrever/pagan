#pragma once
#include <vector>

#include "oxygine-framework.h"
#include <Actor.h>

using namespace oxygine;

struct cRootActor : public RootActor
{
	cRootActor()
	{
		//each mobile application should handle focus lost
		//and free/restore GPU resources
		addEventListener(RootActor::DEACTIVATE, CLOSURE(this, &cRootActor::onDeactivate));
		addEventListener(RootActor::ACTIVATE, CLOSURE(this, &cRootActor::onActivate));
	}

	void onDeactivate(Event *)
	{
		core::reset();
	}

	void onActivate(Event *)
	{
		core::restore();

	}
};

struct cApplication
{
	
	void Init();
	void Update();
	void Destroy();

	static const int mWinWidth = 1024;
	static const int mWinHeight = 640;
	//static const int mWinWidth = 2528;
	//static const int mWinHeight = 1312;
};
