#pragma once

#include <type_traits>

namespace pgn
{
	template <typename Enumeration>
	inline auto Enum2Int(Enumeration const value)
		-> typename std::underlying_type<Enumeration>::type
	{
		return static_cast<typename std::underlying_type<Enumeration>::type>(value);
	}
}