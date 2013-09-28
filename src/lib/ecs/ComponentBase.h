#pragma once

#include <iostream>

namespace pgn
{
	class cComponentBase
	{
	public:
		virtual ~cComponentBase(){}

		virtual unsigned short BitId() const {return 0xFFFF;}

		/* stream-style

			virtual std::ostream& operator << (std::ostream& o) const;
			virtual std::istream& operator >> (std::istream& i) const;

			config-style

			ReadJson, WriteJson( document& doc)
		*/
	protected:
		cComponentBase(){}
	};
}