#pragma once

#include <memory>

#include <ecs/ecs-config.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//! The system base. Does not do much. Registers type & creator to ECS
			/*
				map<string, shared_ptr<SystemBase> >					mSystems;
				Base& SystemBase(const std::string& s)					{ return mSystems[s]}
				template<class T> System(const std::string& s)			{return cast<T>( SystemBase(s))}
			*/
			class cBase
			{
				SUPPORT_DERIVED(cBase);
				public:
					cBase() :mActive(true){}
					void SetActive(bool zValue) { mActive = zValue; }
					bool Active() const			{ return mActive; }
				private:
					bool mActive;
			};

			typedef std::shared_ptr<cBase> cBase_sptr;
		}
	}
}