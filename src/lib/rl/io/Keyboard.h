#pragma once

#include <core/util/singleton.h>
#include <ecs/Event.h>

namespace oxygine
{
	struct cKeyState;
}

namespace pgn
{
	namespace evt
	{
		typedef cEvent<size_t(eBasicECS::KEY_EVT), const int, const oxygine::cKeyState&> cKey;
	}

	class cKeyboard
	{
		public:
			cKeyboard();
			~cKeyboard();
			const oxygine::cKeyState& KeyState(const int key) const;
			void OnKey(const int key,  const oxygine::cKeyState& evt);

		private:
			std::vector<oxygine::cKeyState> mKeyStates;
	};
}