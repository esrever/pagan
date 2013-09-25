#pragma once

namespace pgn
{
	struct cEntity;

	class cSystemBase
	{
		public:
			virtual ~cSystemBase(){}
			virtual void Process();
		protected:
			virtual void ProcessOne(cEntity zE);

		private:
	};
}