#pragma once

#include <map>
#include <memory>
#include <string>

#include <core/container/box2.h>

namespace pgn
{
	class cSubTextureLib
	{
		public:
			virtual ~cSubTextureLib(){}
			void AddRect(const std::string& s, const cBox2i & r);
			void AddRect(const cBox2i & r);

			const cBox2i * Rect() const { return nullptr; }
			const cBox2i * Rect(const std::string& s) const;
			const std::map<std::string, cBox2i>& Rects() const { return mRects; }
			
		private:
			std::map<std::string, cBox2i> mRects;
	};

	typedef std::shared_ptr<cSubTextureLib> cSubTextureLib_sptr;
}