#pragma once

#include <bitset>
#include <functional>
#include <memory>
#include <core/serialize/serialize.h>

#define MAX_COMPONENTS 256

namespace pgn
{
	typedef size_t cEntity;
	typedef std::bitset<MAX_COMPONENTS> cComponentMask;
	typedef std::function<void(std::vector<cEntity>&)> cQueryFunc;
}

// Some defines
#define DECL_PTRTYPE(T)\
typedef std::shared_ptr< T > T##_sptr;\
typedef std::weak_ptr< T > T##_wptr;\
typedef std::unique_ptr< T > T##_uptr;