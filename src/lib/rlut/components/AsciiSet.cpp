
#include "AsciiSet.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cAsciiSet::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Symbols", mSymbols);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cAsciiSet::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Symbols", mSymbols);
            }
        }
    }
}

