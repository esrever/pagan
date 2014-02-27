#pragma once

#include "SystemBase.h"

namespace pgn
{
	class cExampleSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual void to_json(rapidjson::Value& zRoot) const;
		virtual bool from_json(const rapidjson::Value& zRoot);
	};
}
