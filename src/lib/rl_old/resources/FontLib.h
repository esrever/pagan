#pragma once

#include <oxygine-framework.h>

#include <core/util/json_conversions.h>

namespace pgn
{
	class cFontLib
	{
	public:
		oxygine::Font * Get(const std::string& zFontName) const; 
		void Init(const std::string& zResFile);
		void Destroy();
	private:
		DECL_JSON_PGN_FRIEND

		oxygine::Resources mFontResources;
	};

	DECL_JSON_PGN(cFontLib)
}