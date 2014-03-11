#pragma once

#include <string>
#include <pugixml.hpp>

namespace pgn
{
	namespace detail
	{
		template<class T>
		struct is_simple { static const bool value = false; };

		template<>struct is_simple<int> { static const bool value = true; };
		template<>struct is_simple<unsigned> { static const bool value = true; };
		template<>struct is_simple<float> { static const bool value = true; };
		template<>struct is_simple<double> { static const bool value = true; };
		template<>struct is_simple<std::string> { static const bool value = true; };

		//! Attribute writers must support std::to_string
		template<class T>
		void WriteAttribute(pugi::xml_node& node, const T& value)
		{

		}
	}
}