#include "AsciiRep.h"

#include <glm/glm.hpp>

#include <core/util/jsonutils.h>

#include <rl/curses/color.h>

namespace pgn
{
	static glm::vec3 read_color( const rapidjson::Value& zRoot)
	{
		glm::vec3 vcol;
		std::string col_str;
		if(!from_json(col_str, zRoot))
		{
			std::vector<unsigned> col;
			read_json_vector(col, zRoot);
			if(col.size() == 3)
				for(unsigned i=0;i<3;++i)
					vcol[i] = col[i]/255.0f;
		}
		else if(!from_json(col_str, zRoot))
		{
			std::vector<float> col;
			read_json_vector(col, zRoot);
			if(col.size() == 3)
				for(unsigned i=0;i<3;++i)
					vcol[i] = col[i];
		}
		else
		{
			unsigned col_num;
			std::stringstream ss;
			ss << std::hex << col_str;
			ss >> col_num;
			vcol.z = (col_num&0xFF)/255.0f;
			vcol.y = ((col_num>>8)&0xFF)/255.0f;
			vcol.x = ((col_num>>16)&0xFF)/255.0f;
		}
		return vcol;
	}

	//----------------------------------------------------------------------------------
	template<>
	bool from_json<cAsciiRep>( cAsciiRep& zData, const rapidjson::Value& zRoot)
	{		
		char c;
		from_json( c, zRoot["char"]);
		
		std::vector<std::string> attrs;
		read_json_vector(attrs, zRoot["attribs"]);

		glm::vec3 vcol = read_color(zRoot["color"]);
		glm::vec3 vcolbg = read_color(zRoot["color_bg"]);

		auto cpair = curses::cColor::CreateColorPairIndex( vcol, vcolbg);

		zData.mChar = c + (0<<PDC_ATTR_SHIFT) + (cpair<<PDC_COLOR_SHIFT);

		return true;
	}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<cAsciiRep>( const cAsciiRep& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		//JsonWriter_AddMember("char", zData.mChar, zRoot);
		zRoot.EndObject();
	}
}