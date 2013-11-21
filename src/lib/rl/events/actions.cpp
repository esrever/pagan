#include "actions.h"
#include "ecs/ActionMgr.h"

namespace pgn
{
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
}
