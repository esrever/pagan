#pragma once

namespace pgn
{
	namespace bt
	{
		void RegisterConditions();

		//! Later on, some complex conditions might write their result to the blackboard

		/*
			type:		condition
			name:		hostiles_detected
			pseudo:		hl = Get<vector<EntityWithData>>("hostiles");
						return !hl->empty()
		*/

		/*
			type:		condition
			name:		melee_distance_to_hostile
		*/
	}
}