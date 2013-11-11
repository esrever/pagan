#pragma once

#include <string>

namespace pgn
{
	// Normal text handler
	template<class C>
	void TextMod(std::wstring& zOut)
	{
		const std::wstring tag_end_open(L"</");
		auto idx_end_1 = zOut.find(tag_end_open);
		size_t idx_start = 0;
		size_t idx_end_2 = 0;
		while (idx_end_1 != std::string::npos)
		{
			idx_end_2 = zOut.find(L'>', idx_end_1);
			auto fmt = zOut.substr(idx_end_1 + 2, idx_end_2 - idx_end_1 - 2);
			idx_start = zOut.find(fmt, idx_start);
			auto idx_start_text = idx_start + fmt.size() + 1;
			auto text_size = idx_end_1 - idx_start_text;
			
			idx_end_2 += C::Apply(zOut, idx_start_text, text_size, fmt.size());

			idx_start = idx_end_2;
			idx_end_1 = zOut.find(tag_end_open, idx_end_2);
		}
	}

	//--------------------------------------------------------
	class cTextModConverterPassthrough
	{
		public:
			// Create zOut from the range specified in zIn
			static int Apply(std::wstring& zOut, size_t textOffset, size_t textSize, size_t fmtSize)
			{
				return 0;
			}
	};

	//--------------------------------------------------------
	class cTextModConverterIgnore
	{
	public:
		// Create zOut from the range specified in zIn
		static int Apply(std::wstring& zOut, size_t textOffset, size_t textSize, size_t fmtSize)
		{
			int lenStart = int(zOut.size());
			zOut.replace(textOffset - fmtSize - 2,
				2 * fmtSize + 5 + textSize,
				zOut,
				textOffset,
				textSize);
			int lenEnd = int(zOut.size());
			return lenEnd - lenStart;
		}
	};
}