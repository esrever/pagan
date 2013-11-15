#include "LoggerListener.h"
#include <core/util/jsonutils.h>

namespace pgn
{
	//-----------------------------------------------------------------------
	template<>
	bool from_json< cLoggerListener>( cLoggerListener& zData, const rapidjson::Value& zRoot)
	{
		return read_json_vector(zData.mLogTags, zRoot["logtags"]);
	}

	//-----------------------------------------------------------------------
	template<>
	void to_json< cLoggerListener>( const cLoggerListener& zData, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("logtags", zData.mLogTags, zRoot);
		zRoot.EndObject();
	}
}
