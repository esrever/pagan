#pragma once

#include <ecs/SystemBase.h>

namespace pgn
{
	class cAsciiActionInputSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual bool from_json(const rapidjson::Value& zRoot);
		virtual void ProcessSingle(const std::map< cEntity, cEntityComponents>::const_iterator& zEc);
	private:

		wchar_t mCurChar;
		
	};
}