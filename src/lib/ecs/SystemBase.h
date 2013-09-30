#pragma once

#include "ecs_config.h"
#include "Event.h"

namespace pgn
{
	DECL_EVENT(SystemActivity, bool);

	class cSystemBase
	{
		public:
			cSystemBase();
			virtual ~cSystemBase();
			virtual void Process()=0;

			void SetActive(bool zActive);
			bool Active() const {return mActive;}
		protected:
			virtual void ProcessOne(cEntity zE)=0;

		private:
			bool mActive;
	};
}