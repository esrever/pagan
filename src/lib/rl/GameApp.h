#pragma once

#include <string>

#include <rl/resources/GameResources.h>
#include <rl/io/GameIO.h>

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

			//! Get the data path
			const std::string& GetDataPath() const { return mDataPath; }

			//! Get the game resources
			const cGameResources& GetResources() const { return mResources; }

			//! Get the game i/o
			const cGameIO& GetIO() const { return mIO; }

		private:
			cGameIO			mIO;
			cGameResources	mResources;
			std::string		mDataPath;
	};
}