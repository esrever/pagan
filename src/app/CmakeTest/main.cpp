#include <iostream>
#include <fstream>
#include <sstream>
#include <pystring.h>
#include <boost/format.hpp>

#include <core/util/cmdopt.h>
#include <core/util/cfgtree.h>


/*
    I need to wrap this up, as well as the cmd opts, so that I can easily ask for an array or a value by get<int>("asd.qwe.etrty")
*/

using namespace std;

static void test_split(const std::string& s, const std::string& sep, const int count=-1)
{
	std::vector<std::string> res;
	pystring::split(s,res,sep,count);
	std::cout<<"splitting "<<s<<std::endl;
	for(auto s : res)
		std::cout<<"\""<<s<<"\""<<std::endl;
}

int main(int argc, char ** argv)
{
	/*
	pgn::cCfgTree tree;
	tree().Set<int>("key0", 32);
	tree().Set("key1", 32.1);
	tree().Set("key2", std::string("32.3"));
	std::vector<int> args;
	args.push_back(1);
	args.push_back(2);
	args.push_back(3);
	tree().Set("key3", args);

	int k0;
	float k1;
	std::string k2;
	tree().Get("key0",k0);
	tree().Get("key1",k1);
	tree().Get("key2",k2);
	std::cout<<" "<<k0<<" "<<k1<<" "<<k2<<std::endl;
	std::vector<float> k3;
	tree().Get("key3", k3);
	for(const auto& v : k3) std::cout<<" "<<v;
	std::cout<<std::endl;
	*/
	// TODO:  write the json importer and fix the std::string() ctor for .Set function

	test_split("a=b","=",1);
	test_split("b",",",1);
	test_split("b,c",",",1);
	test_split("b,c,",",");

    pgn::cCmdOpt cmdopts;
    cmdopts.Init(argc,argv);
	pgn::cCfgTree cfgtree( cmdopts.CfgData());
	auto fjson = cmdopts.CfgData().mData.find("jopt");
    if( fjson != cmdopts.CfgData().mData.end())
    {
        const std::string& fname = fjson->second.front();
		pgn::cCfgTree cfgjson(fname);
		cfgtree.mCfgData.Merge(cfgjson.mCfgData);
    }
	return 0;
}