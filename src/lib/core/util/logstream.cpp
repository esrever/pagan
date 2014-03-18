#include "logstream.h"

#include <boost/format.hpp>

namespace pgn
{
	//----------------------------------------------------------------------------
    cLogBase::cLogBase()
        :mName("default")
        ,mDisplayLevel( eLogLevel::DBG )
        ,mFormat( boost::format("%s(%s): %s\n") )
    {
        mLevelNames[0] = "ERR";
        mLevelNames[1] = "WRN";
        mLevelNames[2] = "INF";
        mLevelNames[3] = "DBG";
    }

	//----------------------------------------------------------------------------
    cLogStream::cLogStream()
        :mChannel( nullptr )
    {
    }

	//----------------------------------------------------------------------------
	cLogStream cLogStream::mDefault;

	//----------------------------------------------------------------------------
    void cLogBase::SetFormat(const boost::format& zFmt)
    {
        mFormat = zFmt;
    }

	//----------------------------------------------------------------------------
    void cLogStream::Log(const eLogLevel zLevel, const std::string& zMsg)
    {
        if(mChannel && (int(zLevel) <= int(mDisplayLevel)))
            (*mChannel) << ( mFormat % mName % mLevelNames[int(zLevel)] % zMsg);
    }


	//----------------------------------------------------------------------------
	cLogString cLogString::mDefault;

	//----------------------------------------------------------------------------
	void cLogString::Log(const eLogLevel zLevel, const std::string& zMsg)
	{
		if (int(zLevel) <= int(mDisplayLevel))
		{
			if (mLogLines.size() == mMaxLines)
				mLogLines.pop_back();
			mLogLines.push_front(boost::str(mFormat % mName % mLevelNames[int(zLevel)] % zMsg));
		}
	}

	//----------------------------------------------------------------------------
	void cLogString::SetMaxLines(size_t zMaxLines)
	{
		mMaxLines = zMaxLines;
		if (mLogLines.size() > zMaxLines)
			mLogLines.resize(zMaxLines);
	}
}