#pragma once
#include <memory>
#include <bitset>

#define MAX_COMPONENTS 64
typedef std::bitset<MAX_COMPONENTS> component_mask_type;
#define DECL_PTRTYPE(T) typedef std::shared_ptr< T > T##Sptr;\
						typedef std::weak_ptr< T >   T##Wptr;\
						typedef const T &    T##Cref;\
						typedef T &    T##Ref;