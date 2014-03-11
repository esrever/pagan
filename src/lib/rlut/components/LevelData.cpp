
#include "LevelData.h"

// cmpextra
#include <rlut/dungen/dungen.h>

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cLevelData::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Id", mId);

				//cmpextra
				pgn::rlut::cWorkspace ws;
				pgn::SerializeIn(reader, "DunGen", ws);
				std::map<std::string, std::string> tiles;
				pgn::SerializeIn(reader, "Tiles", tiles);

                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cLevelData::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Id", mId);
            }
        }
    }
}

