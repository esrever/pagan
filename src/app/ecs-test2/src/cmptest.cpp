#include "cmptest.h"

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	bool cCmpTest::SerializeIn(const node_type& reader)
	{
		pgn::SerializeIn(reader.child("DataValue"), mData);
		// Read in entities, etc
		return true;
	}

	//---------------------------------------------------------------------------------------------------
	void cCmpTest::SerializeOut(node_type& writer, const std::string& key) const
	{
		auto& child = writer.append_child(key.c_str());
		// export a dump of the object
		pgn::SerializeOut(child, "DataValue", mData);
	}
}