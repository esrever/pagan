#pragma once

#include <memory>
#include <tuple>
#include <bitset>

#include <core/util/json_conversions.h>

namespace pgn
{
//! Declare constants
#define MAX_COMPONENTS 64

//! Declare types
typedef std::bitset<MAX_COMPONENTS> component_mask_type;
template<>
void to_json<component_mask_type>(const component_mask_type& zObj, JsonWriter& zRoot);

typedef unsigned cEntity;

//! Declare pointer types
#define DECL_PTRTYPE(T) typedef std::shared_ptr< T > T##Sptr;\
						typedef std::weak_ptr< T >   T##Wptr;\
						typedef const T &    T##Cref;\
						typedef T &    T##Ref;

//! Allow 256 values for the eBasicECSEvent
enum class eBasicECSEvent : size_t {
	ENTITY_CREATED = 0,
	ENTITY_DESTROY,
	ENTITY_TAGGED,
	ENTITY_UNTAG,
	TAG_REMOVE,
	COMPONENT_ADDED,
	COMPONENT_REMOVE,
	SYSTEM_ACTIVE,
	num = 256
};


}