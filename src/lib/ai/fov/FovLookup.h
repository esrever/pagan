#pragma once

#include <vector>
#include <memory>

namespace pgn
{
    namespace ai
    {
    
        template<class F>
        class cFovLutSet
        {
            public:
                typedef F fov_type;
                
                const F& Get(size_t los) const;
            private:
                std::vector<F> mFovLuts;
        };
        
        //------------------------------------------------------------
        const typename cFovLookup::fov_type& cFovLutSet::Get(size_t los)
        {
            if (los > mFovLuts.size())
                mFovLuts.resize(los);
            auto& flut = mFovLuts.at(los - 1);
            if(!flut.HasInit())
                flut.Init(los);
            return flut;
        }
    }
}