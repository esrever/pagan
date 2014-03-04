#include "texture.h"

namespace pgn
{
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
	template<> void SerializeOut<cTexture>(node_type& writer, const std::string& key, const cTexture& value)
	{
		// export a dump of the object
		SerializeOut(writer, key.c_str(), value.Name());
	}

	//---------------------------------------------------------------------------------------------------
	template<> bool SerializeIn<cTexture>(const node_type& reader, cTexture& value)
	{
		// TODO: read string, get app and fetch the appropriate texture
		assert(false);
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