#include "cfgtree.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace std;

namespace pgn
{
	static void process_level( const rapidjson::Value& val, int depth);

	//--------------------------------------------------------------
	cCfgTree::cCfgTree(const std::string& zJsonFileName)
	{
		FILE * fp = fopen(zJsonFileName.c_str(),"rt");
		std::vector<char> jsontext;
		if(fp)
		{
			fseek(fp,0,SEEK_END);
			jsontext.resize(ftell(fp)+1);
			rewind(fp);
			fread(&jsontext.front(),1, jsontext.size()-1,fp);
			fclose(fp);
			jsontext.back() = '\0';
		}
		
		rapidjson::Document document;
		if (document.Parse<0>(&jsontext.front()).HasParseError())
			return ;
		assert(document.IsObject());
        process_level( document, 0);
	}

	//--------------------------------------------------------------
	cCfgTree::cCfgTree(const cCfgData& zCfgData)
	{
		mCfgData = zCfgData;
	}

	void cCfgTree::BuildTreeLevel(const rapidjson::Value& val, int depth, std::string& zRunningName)
	{
		string outp;
		ostringstream prefix;
		for(int i=0;i<depth;++i)
			prefix << "\t";
		switch( val.GetType())
		{
			case rapidjson::Type::kObjectType:
				for (auto itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr) 
				{
					cout<< prefix.str() << itr->name.GetString() << endl ;
					process_level( itr->value, depth+1);
				};
			break;
			case rapidjson::Type::kArrayType:
				for (auto itr = val.Begin(); itr != val.End(); ++itr) 
				{
					static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
					cout<< prefix.str() << kTypeNames[itr->GetType()] << endl ;
					process_level( *itr, depth+1);
				};
				break;
			case rapidjson::Type::kStringType:
				cout<<prefix.str() <<val.GetString()<<endl;
				break;
			case rapidjson::Type::kTrueType:
			case rapidjson::Type::kFalseType:
				outp = "bool";
				break;
			case rapidjson::Type::kNumberType:
				outp = val.IsInt() ? "int" : "real";
				break;
			case rapidjson::Type::kNullType:
				outp = "null";
				break;
			default :
				outp = "BEEEEEP";
				break;
		}
	}

	static void process_level( const rapidjson::Value& val, int depth)
	{
		string outp;
		ostringstream prefix;
		for(int i=0;i<depth;++i)
			prefix << "\t";
		switch( val.GetType())
		{
			case rapidjson::Type::kObjectType:
				for (auto itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr) 
				{
					cout<< prefix.str() << itr->name.GetString() << endl ;
					process_level( itr->value, depth+1);
				};
			break;
			case rapidjson::Type::kArrayType:
				for (auto itr = val.Begin(); itr != val.End(); ++itr) 
				{
					static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
					cout<< prefix.str() << kTypeNames[itr->GetType()] << endl ;
					process_level( *itr, depth+1);
				};
				break;
			case rapidjson::Type::kStringType:
				cout<<prefix.str() <<val.GetString()<<endl;
				break;
			case rapidjson::Type::kTrueType:
			case rapidjson::Type::kFalseType:
				outp = "bool";
				break;
			case rapidjson::Type::kNumberType:
				outp = val.IsInt() ? "int" : "real";
				break;
			case rapidjson::Type::kNullType:
				outp = "null";
				break;
			default :
				outp = "BEEEEEP";
				break;
		}
	}
}