#include "cfgtree.h"

#include <pystring.h>

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
        //process_level( document, 0);
		std::vector<std::string> rn;
		BuildTreeLevel(document,rn);
	}

	//--------------------------------------------------------------
	cCfgTree::cCfgTree(const cCfgData& zCfgData)
	{
		mCfgData = zCfgData;
	}

	void cCfgTree::BuildTreeLevel(const rapidjson::Value& val, std::vector<std::string> zRunningName)
	{
		std::string runningKey = pystring::join(".",zRunningName);
		std::vector<std::string> tmpName;
		switch( val.GetType())
		{
			case rapidjson::Type::kObjectType:
				for (auto itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr) 
				{
					tmpName = zRunningName;
                    tmpName.push_back(itr->name.GetString());
					BuildTreeLevel(itr->value, tmpName);
				};
			break;
			case rapidjson::Type::kArrayType:
				{
					int c=0;
					for (auto itr = val.Begin(); itr != val.End(); ++itr) 
					{
						tmpName = zRunningName;
						tmpName.back() += "[" + to_string(c++) +"]";
						BuildTreeLevel(*itr, tmpName);
					};
				}
				break;
			case rapidjson::Type::kStringType:
                mCfgData.Set(runningKey, std::string(val.GetString()));
				break;
			case rapidjson::Type::kTrueType:
			case rapidjson::Type::kFalseType:
                mCfgData.Set(runningKey, val.GetBool());
				break;
			case rapidjson::Type::kNumberType:
                if(val.IsInt()) mCfgData.Set(runningKey, val.GetInt());
				if(val.IsUint()) mCfgData.Set(runningKey, val.GetUint());
				if(val.IsInt64()) mCfgData.Set(runningKey, val.GetInt64());
				if(val.IsUint64()) mCfgData.Set(runningKey, val.GetUint64());
				if(val.IsDouble()) mCfgData.Set(runningKey, val.GetDouble());
				break;
			case rapidjson::Type::kNullType:
				break;
			default :
				assert(false);
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