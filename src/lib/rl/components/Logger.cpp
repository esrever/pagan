#pragma once

#include "Logger.h"

namespace pgn
{
	//-----------------------------------------------------------------------
	template<>
	bool from_json< cLogger>( cLogger& zData, const rapidjson::Value& zRoot)
	{
		from_json(zData.mMaxLines,"MaxLines");
		return true;
	}

	//-----------------------------------------------------------------------
	template<>
	void to_json< cLogger>( const cLogger& zData, JsonWriter& zRoot){}

	/*
		I need functions with text modification:
			void cTextModifier(const string& intext, std::vector<chtype>& outText, "damage")
		
		cTextModifier::Apply("You take",line);
		cTextModifier::Apply(boost::str(boost::format("%d damage")%dmg), line2)
		cTextModifier::Apply("\n",line3);
		cTextModifier::Join(lineOut, line1, line2, line3);
		void WriteToLog(cLogger& zLog, lineOut )
	*/

	void Log(cLogger& zLog, const std::string zMsg)
	{

	}
}