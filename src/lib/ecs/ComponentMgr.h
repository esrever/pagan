#pragma once

namespace pgn
{
	class cEntity;

	template<class T>
	class cComponentMgr : public cEventReceiver<cEntityCreatedEventData>,
						  public cEventReceiver<cDestroyEntityEventData>
	{
	public:
		// Component-related functions
		template<class T>
		void AddComponent(const cEntity& zEntity, const T& zComponent = T()); 
		template<class T>
		void RemoveComponent(const cEntity& zEntity, const T& zComponent);
	private:

		std::set<T> mEntitiesPerComponent;
	};


	//-----------------------------------------------------------------------
	template<class T>
	void cComponentMgr::AddComponent(const cEntity& zEntity, const T& zComponent)
	{	
		mEntitiesPerComponent.insert(zEntity);
		// TODO: pass ptr of entity instead?
		auto v = std::pair<cEntity, unsigned>(zEntity, T::msMaskIndex); 
		EMIT_EVENT( ComponentAdded, v);
	}

	//----------------------------------------------------------------
	template<class T>
	void cComponentMgr::RemoveComponent(const cEntity& zEntity, const T& zComponent)
	{
		// TODO: pass ptr of entity instead?
		auto v = std::pair<cEntity, unsigned>(zEntity, T::msMaskIndex); 
		EMIT_EVENT( ComponentRemoved, v);

		mEntitiesPerComponent.erase(zEntity);
	}
}
