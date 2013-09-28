#pragma once

namespace pgn
{
	class cComponentBase
	{
	public:
		virtual ~cComponentBase(){}

		virtual unsigned short BitId() const {return 0xFFFF;}
	protected:
		cComponentBase(){}
	};
}