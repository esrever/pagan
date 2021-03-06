#pragma once

#include <memory>
#include <tuple>
#include <bitset>
#include <type_traits>

#include <core/util/json_conversions.h>

#define foreach( it, cont) for( auto it = cont##.begin(); it != cont##.end(); ++##it )

#define DECL_ENUMSTR(T, V) template<> \
struct Enum2String< T , size_t( T##::##V )>{\
	static const char * Get() { return #V; }};


namespace pgn
{
	template<typename E>
	const typename std::underlying_type<E>::type to_integral(E e)
	{
		return static_cast<typename std::underlying_type<E>::type>(e);
	}

	template<class T, size_t N>
	struct Enum2String{};


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

namespace evt
{
	//! Allow 256 values for the eBasicECSEvent
	enum class eBasicECS : size_t {
		ENTITY_CREATED = 0,
		ENTITY_DESTROY,
		ENTITY_TAGGED,
		ENTITY_UNTAG,
		TAG_REMOVE,
		COMPONENT_ADDED,
		COMPONENTS_ADDED,
		COMPONENT_REMOVE,
		SYSTEM_ACTIVE,
		UPDATE,
		num = 256
	};
}

//------------------------------------------------------------------------
//! At the moment I'm using these for entities that have location. Not sure when I'll be cloning monsters
template<class T>
T Clone(const T& val) { return val; }

}