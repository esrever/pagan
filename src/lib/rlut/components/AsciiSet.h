#pragma once
#include <cstdint>
#include <ecs/ecs-config.h>
#include <vector>
#include <string>

namespace pgn{
    namespace rl{
        namespace cmp{
            
            struct cAsciiSet
            {
                cAsciiSet(const std::vector< std::string >& arg0 = std::vector< std::string >()):
                        mSymbols(arg0){}
                
                size_t SerializeIn(const node_type&);
                void SerializeOut(node_type&) const;
                
                //!ASCII representation
                std::vector< std::string > mSymbols;
                
            };
        
        }
    }
}
