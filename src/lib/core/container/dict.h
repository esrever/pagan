#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <cassert>

namespace pgn
{
    //! stores functions for allocation/deletion of arbitrary objects
    struct cObjectMemoryMan
    {
        typedef std::function<void( void *&)> func_del;
        typedef std::function< void* (const void *)> func_alloc;

        template<class T>
        static cObjectMemoryMan Create()
        {
            cObjectMemoryMan outp;

            outp.mAllocator = [](const void* p){ return (void *)(new T(*(T *)p)); };
            outp.mDeleter = [](void *& p){ delete (T *&)p; p = nullptr; };

            return outp;
        }

        func_del   mDeleter;
        func_alloc mAllocator;
    };

    struct cObjectMap
    {
        typedef std::string key_type;
        typedef void * value_type;
        typedef std::map< key_type, value_type> map_type;
        typedef std::pair<map_type, cObjectMemoryMan> map_and_man_type;

        cObjectMap(){}
        cObjectMap(const cObjectMap&){ assert(mData.first.empty()); }
        ~cObjectMap()
        {
            for (auto& v : mData.first)
                mData.second.mDeleter(v.second);
        }

        template<class T>
        T * Get(const std::string& name)
        {
            auto iter = mData.first.find(name);
            return (iter == mData.first.end()) ? nullptr : (T *)iter->second;
        }

        map_and_man_type mData;

        private:
            cObjectMap& operator = (const cObjectMap&) {}
    };


    //! TODO: how to copy/clone dicts around? allow it or not? if yes, the logic should clearly be in ObjectMap
    class cDict
    {
        public:
            typedef std::string key_type;
            typedef void * value_type;
            typedef std::map< key_type, value_type> map_type;
            typedef std::pair<map_type, cObjectMemoryMan> map_and_man_type;

			cDict(){}
			cDict(const cDict& d){}
			cDict& operator = (const cDict& d){ return *this; }

			/**
			* Gets a value of the given type, if not found return nullptr
			* @name key key name
			*/
            template<class T>
            T * Get(const std::string& name)
            {
                const auto& ti = typeid(T);
                auto iter = mPerTypeData.find(ti);
                if (iter == mPerTypeData.end())
                    return nullptr;
                else
                    return iter->second.Get<T>(name);
            }


			/**
			* Inserts a new value at the given key
			* @name key key name
			* @name value value
			*/
            template<class T>
            void Insert(const std::string& name, const T& value)
            {
                const auto& ti = typeid(value);
                auto iter = mPerTypeData.find(ti);
                
                map_and_man_type * valp;
                if (iter == mPerTypeData.end())
                {
                    auto& ref = mPerTypeData[ti];
                    ref.mData.second = cObjectMemoryMan::Create<T>();
                    valp = &ref.mData;
                }
                else
                    valp = &iter->second.mData;
                // get map/man pair
                auto& val = *valp;
                void * vptr = (void *)&value;
                void * newobj = val.second.mAllocator(vptr);
                val.first.insert(std::make_pair(name, newobj));
            }

			/**
			* Erases a value in a given key
			* @name key key name
			*/
            template<class T>
            void Erase(const std::string& name)
            {
                const auto& ti = typeid(T);
                auto iter = mPerTypeData.find(ti);

                if (iter != mPerTypeData.end())
                {
                    auto& submap = iter->second.mData;
                    auto iter2 = submap.first.find(name);
                    if (iter2 != submap.first.end())
                    {
                        submap.second.mDeleter(iter2->second);
                        submap.first.erase(iter2);
                    }
                }
            }

			/**
			* Applies a function to the key-value pairs of a given type
			* @name func a function with the signature void(const string&, const T&)
			*/
            template<class T>
            void Visit(std::function<void(const std::string& key, const T& val)> func)
            {
                const auto& submap = mPerTypeData[typeid(T)];
                for (const auto& kv : submap.mData.first)
                {
                    func(kv.first, *(T *)kv.second);
                }
            }
            
        private:
            std::map<std::type_index, cObjectMap> mPerTypeData;
    };
}

