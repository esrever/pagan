#pragma once

namespace pgn
{
	template<class T>
	class cComponentStorage
	{
		public:
			void AddComponent(cEntityPtr zEntity, const T& zComponent = T());
			void RemoveComponent(cEntityPtr);
			//const T& Component(cEntityPtr zEntity) const;
			//T& Component(cEntityPtr zEntity);

			const std::map<cEntityPtr, T>& Components(cEntityPtr zEntity) const { return mComponents; }

	private:
		std::map<cEntity, T> mComponents;
	};

	//------------------------------------------------------------------------------
	template<class T>
	void cComponentStorage<T>::AddComponent(cEntityPtr zEntity, const T& zComponent)
	{
		// TODO: looks dodgy. where are the events?
		if( mComponents.find(zEntity) != mComponents.end())
			RemoveComponent(zEntity);
		mComponents[zEntity] = zComponent;
	}

	//------------------------------------------------------------------------------
	template<class T>
	void cComponentStorage<T>::RemoveComponent(cEntityPtr zEntity)
	{
		mComponents.erase(zEntity);
	}
}