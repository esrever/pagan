#include "FontLib.h"

#include <rl/GameApp.h>

namespace pgn
{
	oxygine::Font * cFontLib::Get(const std::string& zFontName) const
	{
		auto& res = const_cast<oxygine::Resources &>(mFontResources);
		return res.getResFont(zFontName)->getFont();
	}

	void cFontLib::Init(const std::string& zResFile)
	{
		mFontResources.loadXML(zResFile);
	}
	
	void cFontLib::Destroy()
	{
		mFontResources.free();
	}

	template<>
	bool from_json< cFontLib >(cFontLib & zData, const rapidjson::Value& zRoot)
	{
		std::string res;
		from_json(res, zRoot["res"]);
		zData.Init(GAME.GetDataPath() + res);
		return true;
	}

	template<>
	void to_json< cFontLib >(const cFontLib & zData, JsonWriter& zRoot)
	{

	}
	
}