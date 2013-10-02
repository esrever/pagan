#include "json_conversions.h"

namespace pgn
{
	/*
	void json_level_to_text(const rapidjson::Value& val, std::string& text, int depth)
	{
		auto prefix = repeat_string("   ",depth);
		switch( val.GetType())
		{
			case rapidjson::Type::kObjectType:
				for (auto itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr) 
				{
					auto tmpname = itr->name.GetString();
					auto is_block = ((itr->value.GetType() == rapidjson::Type::kObjectType) || (itr->value.GetType() == rapidjson::Type::kArrayType) );
					auto brace_in = "";is_block ? "{ " : "";
					auto brace_out = is_block ? "}" : "";
					auto suffix = is_block ? "\n" : "";
					text += prefix + brace_in + tmpname + ": " + suffix;
					json_level_to_text(itr->value, text, depth+1);
					//text += prefix + brace_out + "\n";
					text += "\n";
				};
			break;
			case rapidjson::Type::kArrayType:
				{
					//text += prefix + "[\n";
					int c=0;
					for (auto itr = val.Begin(); itr != val.End(); ++itr) 
					{
						text += prefix;
						json_level_to_text(*itr, text, depth+1);
						text += ",\n";
					};
					text.pop_back();text.pop_back();
					//text += prefix + "]\n";
				}
				break;
			case rapidjson::Type::kStringType:
				text += "\"" + std::string(val.GetString())+"\"";
				break;
			case rapidjson::Type::kTrueType:
				text += "true";
				break;
			case rapidjson::Type::kFalseType:
                text += "false";
				break;
			case rapidjson::Type::kNumberType:
                if(val.IsInt()) text += to_string(val.GetInt());//mCfgData.Set(runningKey, val.GetInt());
				else if(val.IsUint()) text += to_string(val.GetUint());//mCfgData.Set(runningKey, val.GetUint());
				else if(val.IsInt64()) text += to_string(val.GetInt64());//mCfgData.Set(runningKey, val.GetInt64());
				else if(val.IsUint64()) text += to_string(val.GetUint64());//mCfgData.Set(runningKey, val.GetUint64());
				else if(val.IsDouble()) text += to_string(val.GetDouble());//mCfgData.Set(runningKey, val.GetDouble());
				else assert(false);
				break;
			case rapidjson::Type::kNullType:
				text += "null";
				break;
			default :
				assert(false);
				break;
		}
	}
	*/
}