#include <iostream>
#include <pystring.h>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include "logstream.h"

using namespace gel;

namespace po = boost::program_options;

cLogStream::cLogStream()
	:mName("default")
	,mDisplayLevel( eLogLevel::DBG )
	,mFormat( boost::format("%s: %s\n") )
	,mChannel( nullptr )
	{
		mLevelNames[0] = "ERR";
		mLevelNames[1] = "WRN";
		mLevelNames[2] = "INF";
		mLevelNames[3] = "DBG";
	}
	
	void cLogStream::SetFormat(const std::string (&zLevelNames)[eLogLevel::num], const boost::format& zFmt)
	{
		for(int i=0;i< int(eLogLevel::num);++i)
			mLevelNames[i] = zLevelNames[i];
		mFormat = zFmt;
	}
	
	void cLogStream::Log(const eLogLevel zLevel, const std::string& zMsg)
	{
		if(mChannel && (int(zLevel) <= int(mDisplayLevel)))
			(*mChannel) << ( mFormat % mLevelNames[int(zLevel)] % zMsg);
	}

int main(int argc, char ** argv)
{
    cLogStream s;
    s.SetChannel(&std::cout);
    s.Dbg("Debug msg");
    s.Inf("Inf msg");
    s.SetDisplayLevel(eLogLevel::ERR);
    s.Wrn("Wrn msg");
    s.Err("Error msg");
    
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("compression", po::value<int>(), "set compression level")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("compression")) {
        std::cout << "Compression level was set to " 
     << vm["compression"].as<int>() << ".\n";
    } else {
        std::cout << "Compression level was not set.\n";
    }
    
	return 0;
}