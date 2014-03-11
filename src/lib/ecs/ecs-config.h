#pragma once

#include <bitset>
#include <functional>
#include <memory>
#include <core/serialize/serialize.h>

#define MAX_COMPONENTS 32

namespace pgn
{
	typedef size_t cEntity;
	
	typedef std::bitset<MAX_COMPONENTS> cComponentMask;
	DECL_SERIALIZE_INTERFACE(cComponentMask);

	typedef std::function<void(std::vector<cEntity>&)> cQueryFunc;
}

// Some defines
#define DECL_PTRTYPE(T)\
typedef std::shared_ptr< T > T##_sptr;\
typedef std::weak_ptr< T > T##_wptr;\
typedef std::unique_ptr< T > T##_uptr;


#define SUPPORT_DERIVED(T)\
	public:\
	virtual ~ T (){}\
	protected:\
	virtual void SerializeOut(node_type& writer) const {  }\
	virtual size_t SerializeIn(const node_type& reader)  { return 0; };\
	friend void SerializeOut(node_type& writer, const T & value);\
	friend size_t SerializeIn(const node_type& reader, T & value);\
	private: