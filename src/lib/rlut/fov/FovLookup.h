#pragma once

#include <vector>
#include <memory>

namespace pgn
{
    namespace rlut
    {
		typedef std::function< void(const glm::ivec2&, float)> on_visible_func;
        template<class F>
		class cFovLookup
        {
            public:
                typedef F fov_type;
                
                fov_type& Get(size_t los);
            private:
                std::vector<F> mFovLuts;
        };
        
        //------------------------------------------------------------
		template<class T>
		typename cFovLookup<T>::fov_type& cFovLookup<T>::Get(size_t los)
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