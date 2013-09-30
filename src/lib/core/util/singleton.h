#pragma once

namespace pgn
{
	template<class T>
	class cSingleton
	{
	public:
		static T& Instance()  // return reference.
        {
            static T inst;
            return inst;
        }
	};
}