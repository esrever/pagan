
#include "TextureSet.h"

namespace pgn{
    namespace ecs{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cTextureSet::SerializeIn(const node_type& reader)
            {
                size_t ret = 0;
                ret += pgn::SerializeIn(reader,  "Sprites", mSprites);
                return ret;
            }
            
            //----------------------------------------------------------------------------------------------
            void cTextureSet::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Sprites", mSprites);
            }
        }
    }
}

