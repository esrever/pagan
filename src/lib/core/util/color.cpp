#include "color.h"

namespace pgn
{
	void GenGrayscaleGradient(std::vector<SDL_Color>& grad, Uint8 start, Uint8 end, size_t num)
	{
		grad.resize(num);
		double gstep = end - start / (num - 1.0);
		for (size_t i = 0; i < num;++i)
		{
			auto& c = grad[i];
			c.r = c.g = c.b = Uint8(std::round(start + (end - start)*gstep*i));
			c.a = 255;
		}
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const SDL_Color& value)
	{
			unsigned x = *(const unsigned *)(&value);
		std::stringstream stream;
		stream << std::hex << x;
		std::string col_hex(stream.str());
		SerializeOut(node, col_hex);
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, SDL_Color& value)
	{
		static const std::map<std::string, SDL_Color> colours{  { "red", { 255, 0, 0, 255 } },
																{ "green", { 0, 255, 0, 255 } },
																{ "blue", { 0, 0, 255, 255 } },
																{ "white", { 255, 255, 255, 255 } },
																{ "black", { 0, 0, 0, 255 } },
																{ "magenta", { 255, 0, 255, 255 } },
																{ "cyan", { 0, 255, 255, 255 } },
																{ "yellow", { 255, 255, 0, 255 } } };
		std::string s;
		size_t ret = SerializeIn(node, s);
		auto it = colours.find(s);
		if (it == colours.end())
		{
			unsigned x = std::stoul(s, nullptr, 16);
			value = *(SDL_Color *)(&x);
		}
		else
		{
			value = it->second;
		}
		return ret;
	}
}

namespace std
{
	const string to_string(const SDL_Color& value)
	{
		std::ostringstream oss;
		unsigned x = *(const unsigned *)(&value);
		oss << std::hex << x;
		return oss.str();
	}

	istream& operator>> (istream &in, SDL_Color& value)
	{
		static const std::map<std::string, SDL_Color> colours{ { "red", { 255, 0, 0, 255 } },
		{ "green", { 0, 255, 0, 255 } },
		{ "blue", { 0, 0, 255, 255 } },
		{ "white", { 255, 255, 255, 255 } },
		{ "black", { 0, 0, 0, 255 } },
		{ "magenta", { 255, 0, 255, 255 } },
		{ "cyan", { 0, 255, 255, 255 } },
		{ "yellow", { 255, 255, 0, 255 } } };
		std::string s;
		in >> s;
		auto it = colours.find(s);
		if (it == colours.end())
		{
			unsigned x = std::stoul(s, nullptr, 16);
			value = *(SDL_Color *)(&x);
		}
		else
		{
			value = it->second;
		}
		return in;
	}
}