#include "json_conversions.h"

#include <rl/GameApp.h>

namespace pgn
{
	template<>
	bool from_json< oxygine::Color>(oxygine::Color & zData, const rapidjson::Value& zRoot)
	{
		std::string col="0xFFFFFFFF";
		from_json(col, zRoot);
		zData = oxygine::hex2color(col.c_str());
		return true;
	}

	template<>
	void to_json< oxygine::Color>(const oxygine::Color& zData, JsonWriter& zRoot)
	{
		to_json(oxygine::color2hex(zData), zRoot);
	}

	template<>
	bool from_json< oxygine::TextStyle >(oxygine::TextStyle & zData, const rapidjson::Value& zRoot)
	{
		std::string fontname = "system";
		from_json(fontname, zRoot["font"]);
		zData.font = GAME.GetResources().mFontLib.Get(fontname);

		from_json(zData.color, zRoot["color"]);
		from_json(zData.hAlign, zRoot["halign"]);
		from_json(zData.vAlign, zRoot["valign"]);
		from_json(zData.fontSize2Scale, zRoot["fontSize2Scale"]);
		from_json(zData.linesOffset, zRoot["linesOffset"]);
		from_json(zData.kerning, zRoot["kerning"]);
		from_json(zData.multiline, zRoot["multiline"]);
		return true;
	}

	template<>
	void to_json< oxygine::TextStyle >(const oxygine::TextStyle & zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("font", zData.font->getName(), zRoot);
		JsonWriter_AddMember("color", zData.color, zRoot);
		JsonWriter_AddMember("halign", zData.hAlign, zRoot);
		JsonWriter_AddMember("valign", zData.vAlign, zRoot);
		JsonWriter_AddMember("fontSize2Scale", zData.fontSize2Scale, zRoot);
		JsonWriter_AddMember("linesOffset", zData.linesOffset, zRoot);
		JsonWriter_AddMember("kerning", zData.kerning, zRoot);
		JsonWriter_AddMember("multiline", zData.multiline, zRoot);
		zRoot.EndObject();
	}

	template<>
	bool from_json< oxygine::ColorRectSprite >(oxygine::ColorRectSprite & zData, const rapidjson::Value& zRoot)
	{
		oxygine::Color col;
		oxygine::Vector2 size, pos;
		from_json(col, zRoot["color"]) ? zData.setColor(col) : 0;
		from_json(size, zRoot["size"]) ? zData.setSize(size) : 0;
		from_json(pos, zRoot["position"]) ? zData.setPosition(pos) : 0;
		
		return true;
	}

	template<>
	void to_json< oxygine::ColorRectSprite >(const oxygine::ColorRectSprite & zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("color", zData.getColor(), zRoot);
		JsonWriter_AddMember("size", zData.getSize(), zRoot);
		JsonWriter_AddMember("position", zData.getPosition(), zRoot);
		zRoot.EndObject();
	}

	template<>
	bool from_json< oxygine::TextActor >(oxygine::TextActor & zData, const rapidjson::Value& zRoot)
	{
		std::string text;
		oxygine::Vector2 size;
		from_json(size, zRoot["size"]) ? zData.setSize(size) : 0;
		from_json(text, zRoot["text"]) ? zData.setText(text) : 0;

		return true;
	}

	template<>
	void to_json< oxygine::TextActor >(const oxygine::TextActor & zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("size", zData.getSize(), zRoot);
		JsonWriter_AddMember("text", zData.getText(), zRoot);
		zRoot.EndObject();
	}
}