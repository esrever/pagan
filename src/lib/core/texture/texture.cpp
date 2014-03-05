#include "texture.h"

#include <algorithm>

#include <pystring.h>

#include <core/app/sdlapp.h>
#include <core/texture/texturelib.h>

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void cTexture::SplitName(const std::string& name, std::string& lib, std::string& tex, std::string& subtex)
	{
		std::vector<std::string> chunks;
		pystring::split(name, chunks,":", 2);

		lib = (chunks.size() > 0) ? chunks[0] : "";
		tex = (chunks.size() > 1) ? chunks[1] : "";
		subtex = (chunks.size() > 2) ? chunks[2] : "";
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const std::string& key, const cSubTexture& value)
	{
		// split to chunks
		std::string lib, tex, subtex;
		cTexture::SplitName(value.first->Name(), lib, tex, subtex);
		
		// get tex/subtexlib
		auto it = mainapp()->Resources<cTextureLib>(lib)->FindByName(value.first->Name());

		// get rect name
		const auto& rects = it->second->Rects();
		auto itr = std::find_if(rects.begin(), rects.end(), [&](const std::map<std::string, SDL_Rect>::value_type& v) {return v.second == value.second; });
		subtex = itr->first;
		
		// write out
		std::string outname = value.first->Name() + ":" + subtex;
		SerializeOut(writer, key, outname);
	}

	//---------------------------------------------------------------------------------------------------
	bool SerializeIn(const node_type& reader, cSubTexture& value)
	{
		auto * app_ptr = &pgn::mainapp();
		std::string name;
		SerializeIn(reader, name);
		std::string lib, tex, subtex;
		cTexture::SplitName(name, lib, tex, subtex);
		auto tex_atlas = mainapp()->Resources<cTextureLib>(lib)->FindByName(lib + ":" + tex);
		value = cSubTexture(tex_atlas->first, *tex_atlas->second->Rect(subtex));
		return true;
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const std::string& key, const SDL_Rect& value)
	{
		// export a dump of the object
		auto& child = writer.append_child(key.c_str());
		const char * s = "xywh";
		const int * val = &value.x;
		for (int i = 0; i < 4; ++i)
		{
			child.append_attribute(std::string(1, s[i]).c_str()).set_value(std::to_string(val[i]).c_str());
		}
	}

	//---------------------------------------------------------------------------------------------------
	bool SerializeIn(const node_type& reader, SDL_Rect& value)
	{
		const char * s = "xywh";
		int * val = &value.x;
		for (int i = 0; i < 4; ++i)
		{
			const char * v = reader.attribute(std::string(1,s[i]).c_str()).value();
			std::stringstream(v) >> val[i];
		}
		return true;
	}
}