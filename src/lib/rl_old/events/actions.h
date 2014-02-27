#pragma once


namespace pgn
{
    class cActionMgr;
    void RegisterAllActions(cActionMgr& zMgr);

	//! Action prototypes
	namespace evt
	{
		// Player-specific actions
		void PlayerIdle();
		void PlayerMoveN();
		void PlayerMoveS();
		void PlayerMoveE();
		void PlayerMoveW();
		void PlayerMoveNE();
		void PlayerMoveSE();
		void PlayerMoveNW();
		void PlayerMoveSW();

		// Tile-specific actions?

		// App specific actions
		void AppQuit();
		void AppHelp();
	}
	//! ~Action prototypes
}
