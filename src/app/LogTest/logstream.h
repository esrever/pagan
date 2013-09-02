#include <string>
#include <iostream>
#include <cstdint>

#include <boost/format.hpp>

namespace gel 
{
	enum class eLogLevel : std::int8_t 
	{
		ERR=0,
		WRN,
		INF,
		DBG,
		num
	};
	
	class cLogStream
	{
		public:
            cLogStream();
			void SetName(const std::string& zName) { mName = zName;}
			void SetDisplayLevel(const eLogLevel zDispLevel) { mDisplayLevel = zDispLevel;}
			void SetChannel(std::ostream * zChannel) { mChannel = zChannel;}
			void SetFormat(const std::string (&zLevelNames)[eLogLevel::num], const boost::format& zFmt);
			
			void Log(const eLogLevel zLevel, const std::string& zMsg);
			void Dbg(const std::string& zMsg) {Log(eLogLevel::DBG, zMsg);}
			void Inf(const std::string& zMsg) {Log(eLogLevel::INF, zMsg);}
			void Wrn(const std::string& zMsg) {Log(eLogLevel::WRN, zMsg);}
			void Err(const std::string& zMsg) {Log(eLogLevel::ERR, zMsg);}
		
		private:
			std::string 	mName;
			eLogLevel 		mDisplayLevel;
			std::string 	mLevelNames[eLogLevel::num];
			boost::format 	mFormat;
			std::ostream * 	mChannel;
	};
}