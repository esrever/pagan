#pragma once

#include "behavior.h"

namespace pgn
{
	namespace bt
	{
		void RegisterActions();

		eStatus ApproachTarget(cBlackBoard&);
		eStatus FleeTarget(cBlackBoard&);
		eStatus Wander(cBlackBoard&);
		eStatus MeleeAttackTarget(cBlackBoard&);
		eStatus SelectTarget(cBlackBoard&);
		eStatus SenseHostiles(cBlackBoard&);
		eStatus DistanceToTarget(cBlackBoard&);

				/*
					type:		action
					name:		ApproachTarget
					*/

					/*
						type:		action
						name:		Wander
						*/

						/*
							type:		action
							name:		MeleeAttackTarget
							pseudo:		ch = Get<cEntityWithData>( "target")
							evt::Attack( actor, ch)
							*/

							/*
								type:		action
								name:		SelectTarget
								life:		temporary
								write_to:	"target" : EntityWithData
								pseudo:		hd = Get<cEntityWithData>( "hostiles_detected")
								select from *hd closest to actor
								*/
								/*
									type:		action
									name:		SenseHostiles
									life:		temporary
									write_to:	"hostiles" : vector<EntityWithData>
									pseudo:		hl = Get<vector<EntityWithData>>("hostiles");
									for each point in LoS
									if map(point) contains actor
									if relation(this,actor) == hostile
									hl.push_back( actor )
									*/

									/*
										type:		action
										name:		DistanceToTarget
										life:		temporary
										write_to:	"target_distance" : float
										*/
	}
}