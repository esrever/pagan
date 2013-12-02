#pragma once

#include "TileLib.h"

using namespace oxygine;

	// Load xml file with all tiles and descriptors
void cTileLib::Init(Resources& res)
{
	auto tilemap = res.getResAnim("tilemap");
	pugi::xml_document doc;
	if (doc.load_file("tilemap.xml", pugi::parse_default | pugi::parse_comments))
	{
		//[code_modify_base_node
		for (auto it : doc)
		{
			auto name = it.attribute("name").as_string();
			auto row = it.attribute("name").as_uint();
			auto col = it.attribute("name").as_uint();

			spSprite sprite = new Sprite();
			sprite->setAnimFrame(tilemap->getFrame(col, row));
			mSprites[std::string(name)] = sprite;
		}
	}
}