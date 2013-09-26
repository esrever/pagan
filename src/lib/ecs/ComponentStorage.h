#pragma once

namespace pgn
{
	template<class T>
	class cComponentStorage
	{
		public:
			 // TODO: copy is bad?
			void AddComponent(const cEntity& zEntity, const T& zComponent = T());
			void RemoveComponent(const cEntity& zEntity);
			const T& Component(const cEntity& zEntity) const;
			T& Component(const cEntity& zEntity);

	private:
		std::map<cEntity, T> mComponents;
	};
}