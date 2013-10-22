#pragma once

#include "SystemBase.h"

namespace pgn
{
	class cExampleSystem : public cSystemBase
	{
	public:
		virtual void Process();
		virtual void to_json(rapidjson::Value& zRoot) const;
		virtual void from_json(const rapidjson::Value& zRoot);
	};
}
