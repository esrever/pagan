#include "TileLib.h"

#include <rl/GameApp.h>

using namespace oxygine;

namespace pgn
{
	// Load xml file with all tiles and descriptors
	void cTileLib::Init(const std::string& zResName, const std::string& zDescName)
	{
		mTileRes.loadXML(zResName);
		auto tilemap = mTileRes.getResAnim("tilemap");

		pugi::xml_document doc;
		if (doc.load_file(zDescName.c_str(), pugi::parse_default | pugi::parse_comments))
		{
			//[code_modify_base_node
			for (auto it : doc)
			{
				auto name = it.attribute("name").as_string();
				auto row = it.attribute("row").as_uint();
				auto col = it.attribute("col").as_uint();

				spSprite sprite = new Sprite();
				sprite->setAnimFrame(tilemap->getFrame(col, row));
				sprite->setName(std::string(name));
				mSprites[std::string(name)] = sprite;
			}
		}
	}

	void cTileLib::Destroy()
	{
		mSprites.clear();
		mTileRes.free();
	}

	spSprite cTileLib::GetSprite(const std::string& name) const
	{
		auto it = mSprites.find(name);
		return it != mSprites.end() ? it->second : nullptr;
	}

	template<>
	bool from_json< cTileLib >(cTileLib & zData, const rapidjson::Value& zRoot)
	{
		std::string res, desc;
		from_json(res, zRoot["res"]);
		from_json(desc, zRoot["desc"]);
		zData.Init(GAME.GetDataPath() + res, GAME.GetDataPath() + desc);
		return true;
	}

	template<>
	void to_json< cTileLib >(const cTileLib & zData, JsonWriter& zRoot)
	{

	}
}