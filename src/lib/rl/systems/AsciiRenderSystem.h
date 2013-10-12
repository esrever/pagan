#pragma once

#include <ecs/SystemBase>

namespace pgn
{
	class cAsciiRenderSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual void from_json(const rapidjson::Value& zRoot);
	private:
		
	}
}