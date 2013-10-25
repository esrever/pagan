#pragma once

#include "GameLog.h"

namespace pgn
{
	//-----------------------------------------------------------------------
	template<>
	bool from_json< cGameLog>( cGameLog& zData, const rapidjson::Value& zRoot){return true;}

	//-----------------------------------------------------------------------
	template<>
	void to_json< cGameLog>( const cGameLog& zData, rapidjson::Value& zRoot){}

	/*
		I need functions with text modification:
			void cTextModifier(const string& intext, std::vector<chtype>& outText, "damage")
		
		cTextModifier::Apply("You take",line);
		cTextModifier::Apply(boost::str(boost::format("%d damage")%dmg), line2)
		cTextModifier::Apply("\n",line3);
		cTextModifier::Join(lineOut, line1, line2, line3);
		void WriteToLog(cGameLog& zLog, lineOut )
	*/
}