
#include "TextureSet.h"

namespace pgn{
    namespace rl{
        namespace cmp{
            //----------------------------------------------------------------------------------------------
            bool cTextureSet::SerializeIn(const node_type& reader)
            {
                pgn::SerializeIn(reader.child("Sprites"), mSprites);
                return true;
            }
            
            //----------------------------------------------------------------------------------------------
            void cTextureSet::SerializeOut(node_type& writer, const std::string& key) const
            {
                writer.append_attribute("value").set_value(key.c_str());
                pgn::SerializeOut(writer,"Sprites", mSprites);
            }
        }
    }
    
    
}

