#pragma once

#include <ecs/SystemBase.h>

namespace pgn
{
	class cActionMappingSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual void from_json(const rapidjson::Value& zRoot);
	private:
		
	};
}