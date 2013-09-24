#pragma once

namespace pgn
{
	class cEventMgr
	{
		public:
	};

	class cEvent
	{
		public:
	};

	template<class T>
	class cEventListener<T>
	{
		public:
			cEventListener() 
			{
				// subscribe to emitted events T
				// #define Connect( a, signal, b, slot ) a.signal.Connect( &b, &slot )
				Connect( b, update1, l, cEventListener<T>::Receive ); 
			}
			virtual ~cEventListener(){}
	}
}