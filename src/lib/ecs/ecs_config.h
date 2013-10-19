#pragma once
#include <memory>
#include <tuple>
#include <bitset>

namespace pgn
{
	typedef unsigned cEntity;

#define MAX_COMPONENTS 64
typedef std::bitset<MAX_COMPONENTS> component_mask_type;
#define DECL_PTRTYPE(T) typedef std::shared_ptr< T > T##Sptr;\
						typedef std::weak_ptr< T >   T##Wptr;\
						typedef const T &    T##Cref;\
						typedef T &    T##Ref;

#define DECL_EVENT0(N) typedef void N##_datatype; struct c##N##EventData{\
	typedef void data_type; \
	static void emit() { pgn::emit_event< pgn::c##N##EventData >( ); }};

#define DECL_EVENT1(N, A) typedef std::tuple< A > N##_datatype; DECL_EVENT( N , N##_datatype )
#define DECL_EVENT2(N, A, B) typedef std::tuple< A , B > N##_datatype; DECL_EVENT( N , N##_datatype )
#define DECL_EVENT3(N, A, B, C) typedef std::tuple< A , B , C> N##_datatype; DECL_EVENT( N , N##_datatype )

#define DECL_EVENT(N, T) struct c##N##EventData{ \
	typedef T data_type; \
	T data; \
	c##N##EventData(){} \
	c##N##EventData(const T &v):data(v){} \
	static void emit(const T & val ) { pgn::emit_event< pgn::c##N##EventData >(val); }\
};
//#define EMIT_EVENT(N,V) pgn::emit_event< pgn::c##N##EventData >(V);

}