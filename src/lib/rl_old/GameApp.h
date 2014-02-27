#pragma once

#include <string>

#include <core/util/singleton.h>
#include <core/util/json_conversions.h>

#include <rl/resources/GameResources.h>
#include <rl/io/GameIO.h>

#define GAME pgn::cSingleton<pgn::cGameApp>::Instance()

namespace pgn
{
	//! The main game application
	class cGameApp
	{
		public:
			//! Load the config file and set up everything
			void Init();

			//! Destroy all
			void Destroy();

			//! Runs a single frame update
			void UpdateFrame();

			//! Get the data path
			const std::string& GetDataPath() const { return mDataPath; }

			//! Get the game resources
			const cGameResources& GetResources() const { return mResources; }

			//! Get the game i/o
			const cGameIO& GetIO() const { return mIO; }

		private:
			DECL_JSON_PGN_FRIEND

			cGameIO			mIO;
			cGameResources	mResources;
			std::string		mDataPath;
	};

	DECL_JSON_PGN(cGameApp)
}