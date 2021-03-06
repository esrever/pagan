#pragma once

#include <deque>
#include <string>
#include <iostream>
#include <cstdint>

#include <boost/format.hpp>

namespace pgn 
{
	enum class eLogLevel : std::int8_t 
	{
		MERR=0,
		WRN,
		INF,
		DBG,
		num
	};

	class cLogBase
	{
	public:
		cLogBase();
		virtual ~cLogBase(){}
		
		void SetName(const std::string& zName) { mName = zName;}
		void SetDisplayLevel(const eLogLevel zDispLevel) { mDisplayLevel = zDispLevel;}
		void SetFormat(const boost::format& zFmt);

		virtual void Log(const eLogLevel zLevel, const std::string& zMsg){}
		void Dbg(const std::string& zMsg) {Log(eLogLevel::DBG, zMsg);}
		void Inf(const std::string& zMsg) {Log(eLogLevel::INF, zMsg);}
		void Wrn(const std::string& zMsg) {Log(eLogLevel::WRN, zMsg);}
		void Err(const std::string& zMsg) {Log(eLogLevel::MERR, zMsg);}

	protected:
		std::string 	mName;
		eLogLevel 		mDisplayLevel;
		std::string 	mLevelNames[eLogLevel::num];
		boost::format 	mFormat;
		std::ostream * 	mChannel;
	};
	
	class cLogStream : public cLogBase
	{
		public:
            cLogStream();
			void SetChannel(std::ostream * zChannel) { mChannel = zChannel;}
			void Log(const eLogLevel zLevel, const std::string& zMsg);
			static cLogStream& Default() {return mDefault;}
		
		private:
			static cLogStream mDefault;
			std::ostream * 	mChannel;
	};

	class cLogString : public cLogBase
	{
	public:
		cLogString(size_t zMaxLines = 0xFFFFFFFF) :cLogBase(){ SetMaxLines(zMaxLines); }
		void SetMaxLines(size_t zMaxLines);
		void Log(const eLogLevel zLevel, const std::string& zMsg);
		const std::deque<std::string>& Data() const { return mLogLines; }
		static cLogString& Default() { return mDefault; }

	private:
		static cLogString mDefault;
		std::deque<std::string> mLogLines;
		size_t mMaxLines;
	};
}