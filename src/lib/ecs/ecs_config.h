#pragma once
#include <memory>
#include <bitset>

namespace pgn
{

#define MAX_COMPONENTS 64
typedef std::bitset<MAX_COMPONENTS> component_mask_type;
#define DECL_PTRTYPE(T) typedef std::shared_ptr< T > T##Sptr;\
						typedef std::weak_ptr< T >   T##Wptr;\
						typedef const T &    T##Cref;\
						typedef T &    T##Ref;

#define DECL_EVENT(N, T) struct c##N##EventData{ \
	typedef T data_type; \
	T data; \
	c##N##EventData(){} \
	c##N##EventData(const T &v):data(v){} \
	static void emit(const T& val ) { pgn::emit_event< pgn::c##N##EventData >(val); }\
};

#define DECL_EVENT_TUP1(N, T) typedef std::tuple< T > N##_datatype; DECL_EVENT_TUP(N, N##_datatype )
#define DECL_EVENT_TUP2(N, T, U) typedef std::tuple< T , U > N##_datatype; DECL_EVENT_TUP(N, N##_datatype )
#define DECL_EVENT_TUP3(N, A, B, C) typedef std::tuple< A , B , C> N##_datatype; DECL_EVENT_TUP(N, N##_datatype )

#define DECL_EVENT_TUP(N, T) struct c##N##EventData{ \
	typedef T data_type; \
	T data; \
	c##N##EventData(){} \
	c##N##EventData(const T &v):data(v){} \
	static void emit(const T& val ) { pgn::emit_event< pgn::c##N##EventData >(val); }\
};
//#define EMIT_EVENT(N,V) pgn::emit_event< pgn::c##N##EventData >(V);

}