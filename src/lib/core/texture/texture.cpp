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
	void SerializeOut(node_type& writer, const cSubTexture& value)
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
		SerializeOut(writer, outname);
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, cSubTexture& value)
	{
		auto * app_ptr = &pgn::mainapp();
		std::string name;
		size_t ok = SerializeIn(reader, name);
		if (!ok) return 0;

		std::string lib, tex, subtex;
		cTexture::SplitName(name, lib, tex, subtex);
		auto * tlib = mainapp()->Resources<cTextureLib>(lib);
		if (!tlib) return 0;
		
		auto tex_atlas = tlib->FindByName(lib + ":" + tex);
		if (tex_atlas == tlib->Textures().end()) return 0;
		
		auto rect_ptr = tex_atlas->second->Rect(subtex);
		if (!rect_ptr) return 0;

		value = cSubTexture(tex_atlas->first, *rect_ptr);
		return ok;
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const SDL_Rect& value)
	{
		detail::WriteAttribute(node, "x", value.x);
		detail::WriteAttribute(node, "y", value.y);
		detail::WriteAttribute(node, "w", value.w);
		detail::WriteAttribute(node, "h", value.h);
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, SDL_Rect& value)
	{
		size_t ret = 0;
		ret += detail::ReadAttribute(node, "x", value.x);
		ret += detail::ReadAttribute(node, "y", value.y);
		ret += detail::ReadAttribute(node, "w", value.w);
		ret += detail::ReadAttribute(node, "h", value.h);
		return ret;
	}
}