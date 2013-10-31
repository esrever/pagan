#pragma once

#include <ecs/SystemBase.h>

namespace pgn
{
	class cAsciiRenderSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual void ProcessSingle(const std::map< cEntity, cEntityComponents>::const_iterator& zEc);
		virtual bool from_json(const rapidjson::Value& zRoot);
	private:
		
	};
}