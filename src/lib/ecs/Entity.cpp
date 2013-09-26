#include "Entity.h"

namespace pgn
{
	cEntity::cEntity():mId(boost::uuids::random_generator()()){}
}