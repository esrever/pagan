#include <algorithm>
#include <vector>
#include <map>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <iostream>

#include "myevent.h"

using boost::uuids::uuid;
using namespace std;

/*
how do you define the archetypes for your entities?
	an entity archetype is a set of defaults for components
	entity archetype hierarchy?
		lvl 3 monster
		lvl 8 orc
		lvl 4 orc shaman
how do you instantiate multiple new entities from a single archetype?
how do you STORE in-memory entities so that they can be re-instantiated later on?

how do you tag an entity with an easily referencable name?
	map< refname, entity> 
*/

/*
// Components
struct Compo1{static const int cid = 0; int a;};
struct Compo2{static const int cid = 1; int b;};
struct Compo3{static const int cid = 2; int c;};
struct Compo4{static const int cid = 3; int d;};

// Entities
struct Entity
{
	uuid	mUuid;
	bitset  mCompoMask;

	// Initialize entity to default():
	Entity( cmask = 0):mUuid(boost::uuids::random_generator()())
	{
		for each component in mask, initialize
	}

	// Initialize entity from RAM data():
		
	// Initialize entity from config:

	// Initialize entity from blueprint:
};

class EntityManager
{
	// Template forest?
	Entity Create( e = null)
	Entity Create( cmask = 0, e = null); // augment
	Entity Create( conf = 0, e = null);  // augment
	Entity Create( bin = 0, e = null);   // augment

	void Tag(entity, name)
	void Untag(entity, name)

	// Go through the list of the subsystems and add/remove entity refs if conditions are met
	template<class T>
	void AddComponent(entity, T component = T()) 
	template<class T>
	void RemoveComponent(entity)

	template<class T>
	bool GetComponent(entity, T& component)

	// Get all components from an entity ( mainly for I/O)
	// E.g. read_json, write_json, etc. Anything that needs to be applied to all components that are currently used, or'ed with the input mask
	template<class F>
	void ApplyFunctiontoEntityComponents(entity, F func, bitset mask= 0xFFFF...);

	// TODO: entities_with_component(  cmask )

}

struct World
{
	// Get all entities that have a component
	map<Compo1, Entity> mCompo1toEntity;
	map<Compo2, Entity> mCompo2toEntity;
	map<Compo3, Entity> mCompo3toEntity;

	map<string, set<Entity> > mTags;

	map<System, set<entity> > mSystoEntities;
};



class SystemManager
{
	void init()
	void addSystem(sys, pri)
	void removeSystem(sysname)
	void clearSystems()
	void destroy()
}


class SystemBase
{
	virtual void Init() {};
	virtual void Process( entity ) {};
	virtual bitset EntityMask() const =0;
    virtual string SystemName() const=0;
    
    virtual void ClearDeleted() const=0;
    // TODO: deletes marked E/C for deletion. Runs after process. what about created? E.g. fireball
}

class System1 : public SystemBase
{
	// processing logic ... 
}


class EventBase
{
	System *		// system that emitted the event. For debug purposes
	map<string, entity>	// entities that are coupled to the event. String is used for easier linking, but we could also pass a vector and the derived class could provide the enums-indices
}

class EventDer : public EventBase
{
	// event-specific data ... 
}
*/
/*
	EventSys:

	Who subscribes/listens?
	types (immediate)
		AI actions. All these use a list of entities as an argument
		Debug checks make sure that the passed entities are of the correct type (IsPC, IsItem, etc.)
			OnUseItem event <E, Eitem>
			OnApplySkill event <E, Es>
			OnSacrifice event <E0, E1>
			OnRest event <E0, stopCrit>
			OnPickup event <E0, itm>
			OnDrop event <E0, itm>
			OnEquip event <E0, itm>
			OnUnequip event <E0, itm>
			OnPray event <E0, god>
			OnMove event <E0, Etile>
			OnMelee event <E0, E1>
			OnRanged event <E0, E1>
			OnSpell
				OnSpellTarget event <E0, E1>
				OnSpellArea event <E0, ...>
				OnSpellSelf event <E0>
				Bah! Better!
			OnSwap event <E0, E1>
			OnChat event <E0, E1>
			OnKick event <E0, E1>
			OnThrow event <E0, E1, Eproj>
		ResetLevel event <L, ...> 
		
		// General ones
		EntityCreated <E>
		RemoveEntity <E>
		ComponentAdded <E,C>
        RemoveComponent <E,C>
        SystemAdded<S>
        RemoveSystem<S>
        ComponentModified <E,C>
        EntityModified <E>
        SystemModified <S>
        
	
	Definite difference of immediate and timed events?
		immediate like QT
		timed are for running in the end of a loop/frame.
		similar iface to both!
        or make the delayListener a system, running at the end of each system?
        
    mMarkedDeleted for components/entities? 
        set as not to be further processed
        
*/

using namespace pgn;

struct EvtData1 { int boom; float blam;};

struct Printer1 : public cEventReceiver<int>, public cEventReceiver<float>
{
public:
	void Receive( const int& zData) { std::cout<<"IntPrinter1 "<<zData<<std::endl; }
	void Receive( const float& zData) { std::cout<<"FloatPrinter1 "<<zData<<std::endl; }
};

struct Printer2 : public cEventReceiver<int>, public cEventReceiver<EvtData1>
{
public:
	void Receive( const int& zData) { std::cout<<"IntPrinter2 "<<zData<<std::endl; }
	void Receive( const EvtData1& zData) { std::cout<<"EvtPrinter2 "<<zData.blam<<" "<<zData.boom<<std::endl; }
};



int main()
{
	EvtData1 evtdata;
	evtdata.blam=5.0f;
	evtdata.boom=7;

	Printer1 p1; p1.cEventReceiver<int>::Subscribe(); p1.cEventReceiver<float>::Subscribe();
	{
		Printer2 p2; p2.cEventReceiver<int>::Subscribe(); p2.cEventReceiver<EvtData1>::Subscribe();

		cEvent<int>::emit( 10);
		cEvent<float>::emit( 20.0f);
		cEvent<EvtData1>::emit( evtdata);
	}

	std::cout<<"-----"<<std::endl;
	cEvent<int>::emit( 10);
	cEvent<float>::emit( 20.0f);
	cEvent<EvtData1>::emit( evtdata);

	cEvent<int>::emit.Clear();

	std::cout<<"-----"<<std::endl;
	cEvent<int>::emit( 10);
	cEvent<float>::emit( 20.0f);
	cEvent<EvtData1>::emit( evtdata);

	/*
	size_t c1 = sizeof(Compo1);
	size_t cb = sizeof(BaseCompo);
	size_t cd1 = sizeof(CompoDer1);
	*/
	return 0;
}