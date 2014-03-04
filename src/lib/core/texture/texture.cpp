#include "texture.h"

#include <pystring.h>

#include <core/app/sdlapp.h>
#include <core/texture/texturelib.h>

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void cTexture::SplitName(const std::string& name, std::string& lib, std::string& tex, std::string& subtex)
	{
		std::vector<std::string> chunks;
		pystring::split(name, chunks,":", 3);

		lib = (chunks.size() > 0) ? chunks[0] : "";
		tex = (chunks.size() > 1) ? chunks[1] : "";
		subtex = (chunks.size() > 2) ? chunks[2] : "";
	}

	//---------------------------------------------------------------------------------------------------
	cSubTexture cTexture::SubTexture(const SDL_Rect * rect) 
	{ 
		SDL_Rect rdef = { 0, 0, 0, 0 };
		return cSubTexture(this, rect ? *rect : rdef);
	}

	//---------------------------------------------------------------------------------------------------
	void cTexture::Load(texture_loadfunc_type func, const std::string& fname, const char * desc)
	{
		mTexture = func(fname);
		mName = desc ? desc : fname;
	}

	//---------------------------------------------------------------------------------------------------
	template<> void SerializeOut<cTexture_ptr>(node_type& writer, const std::string& key, const cTexture_ptr& value)
	{
		// export a dump of the object
		SerializeOut(writer, key.c_str(), value? value->Name() : "nullptr");
	}

	//---------------------------------------------------------------------------------------------------
	template<> bool SerializeIn<cTexture_ptr>(const node_type& reader, cTexture_ptr& value)
	{
		std::string name;
		SerializeIn(reader, name);
		std::string lib, tex, subtex;
		cTexture::SplitName(name, lib, tex, subtex);
		//value = mainapp()->Resources<cTextureLib>(lib)->FindByName(tex)->SubTexture(subtex);
		return false;
	}

	//---------------------------------------------------------------------------------------------------
	template<> void SerializeOut<SDL_Rect>(node_type& writer, const std::string& key, const SDL_Rect& value)
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
	template<> bool SerializeIn<SDL_Rect>(const node_type& reader, SDL_Rect& value)
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