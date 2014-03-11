
#include "TextureSet.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            size_t cTextureSet::SerializeIn(const node_type& reader)
            {
                return pgn::SerializeIn(reader,"Sprites", mSprites);
            }
            
            //----------------------------------------------------------------------------------------------
            void cTextureSet::SerializeOut(node_type& writer) const
            {
                pgn::SerializeOut(writer,"Sprites", mSprites);
            }
        }
    }
    
    
}

