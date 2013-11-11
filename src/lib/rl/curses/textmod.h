#pragma once

#include <curses.h>

#include "fontpreset.h"

namespace pgn
{
	template<class C>
	void TextMod(std::vector<chtype>& zOut, const std::wstring& zIn)
	{
		std::wstring outText = zIn;
		std::wstring outAttr(zIn.size(), L'\0');
		const std::wstring tag_end_open(L"</");
		auto idx_end_1 = outText.find(tag_end_open);
		size_t idx_start = 0;
		size_t idx_end_2 = 0;
		while (idx_end_1 != std::string::npos)
		{
			idx_end_2 = outText.find(L'>', idx_end_1);
			auto fmt = outText.substr(idx_end_1 + 2, idx_end_2 - idx_end_1 - 2);
			idx_start = outText.find(fmt, idx_start);
			auto idx_start_text = idx_start + fmt.size() + 1;
			auto text_size = idx_end_1 - idx_start_text;

			idx_end_2 += C::Apply(outText, outAttr, idx_start_text, text_size, fmt.size());

			idx_start = idx_end_2;
			idx_end_1 = outText.find(tag_end_open, idx_end_2);
		}

		// merge
		zOut.resize(outText.size()+1);
		for (size_t i = 0; i < outText.size(); ++i)
		{
			zOut.at(i) = outText.at(i) | (outAttr.at(i) << 16);
		}
		zOut.back() = L'\0';
	}

	//--------------------------------------------------------
	class cTextModConverterCurses
	{
	public:
		// Create zOut from the range specified in zIn
		static int Apply(std::wstring& zOutText, std::wstring& zOutAttr, size_t textOffset, size_t textSize, size_t fmtSize)
		{
			const size_t i0 = textOffset - fmtSize - 2;
			const size_t len0 = 2 * fmtSize + 5 + textSize;
			const size_t i1 = textOffset;
			const size_t len1 = textSize;

			const auto fmt = zOutText.substr(i0 + 1, fmtSize);
			/*
				TODO: 
					- grab fmt. TODO: from where??
					- look up fmt presets (ushort elem)

			*/
			wchar_t elem = curses::cFontPreset::Get(fmt);
			std::fill(zOutAttr.begin() + i1, zOutAttr.begin() + i1 + len1, elem);
			cTextModConverterIgnore::Apply(zOutText, textOffset, textSize, fmtSize);
			return cTextModConverterIgnore::Apply(zOutAttr, textOffset, textSize, fmtSize);
		}
	};
}