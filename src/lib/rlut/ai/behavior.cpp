#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include <functional>

#define CLONE_DECL(T) virtual Behavior * clone() const { return new T (*this);};
//#define CLONE_DECL(T)

#define btvar(T,N) private:\
	T m##N;

#define btget(T,N) public:\
	const T & get##N##() const { return m##N; }

#define btset(T,N) public:\
	void set##N##(const T & v) { m##N = v; }

#define btgetset(T,N) \
	btvar( T , N )\
	btget( T , N )\
	btset(T, N)

#define btcdtor(T, U) \
typedef U super;\
virtual ~ T (){}\
virtual Behavior * clone() const { return new T (*this);}\
T (const T &);\
T ()

class cDict;

// ============================================================================
enum Status
	/**
	* Return values of and valid states for behaviors.
	*/
{
	BH_INVALID,
	BH_SUCCESS,
	BH_FAILURE,
	BH_RUNNING,
	BH_ABORTED,
};

namespace pugi
{
	class xml_node;
}

// ============================================================================
class Behavior
	/**
	* Base class for actions, conditions and composites.
	*/
{
public:
	Behavior() : mStatus(BH_INVALID), mPriority(0.0f){};
	Behavior(const Behavior&);
	virtual ~Behavior(){}

	virtual Status update(cDict& dict) { return BH_INVALID; };
	virtual void onInitialize()			{}
	virtual void onTerminate(Status)	{}
	Behavior * addChild(Behavior*) { return this; }

	Status tick(cDict& dict);
	void reset();
	void abort();
	bool isTerminated() const;
	bool isRunning() const;
	Status getStatus() const;
	virtual Behavior * clone() const { return nullptr; };

	btgetset(float, Priority);

private:
	Status mStatus;
};

typedef Behavior* Behavior_ptr;
typedef std::shared_ptr<Behavior> Behavior_sptr;
typedef std::vector<Behavior_ptr> Behavior_ptrs;

// ============================================================================

class Decorator : public Behavior
{
public:
	btcdtor(Decorator, Behavior):mChild(nullptr){}
	btgetset(Behavior_ptr, Child);
};

// ============================================================================
class Repeat : public Decorator
{
public:
	btcdtor(Repeat, Decorator) :mCount(-1),mCounter(0){}
	btgetset(int, Count);
	btgetset(int, Counter);

	void onInitialize();
	Status update(cDict& dict);
};

// ============================================================================

class Composite : public Behavior
{
public:
	btcdtor(Composite, Behavior){}
	Behavior * addChild(Behavior* child) { mChildren.push_back(child); return this; }
protected:
	Behavior_ptrs mChildren;
};

// ============================================================================
class Sequence : public Composite
{
public:
	btcdtor(Sequence, Composite){}
protected:
	virtual void onInitialize();
	virtual Status update(cDict& dict);

protected:
	Behavior_ptrs::iterator mCurrentChild;
};

// ============================================================================

class Selector : public Composite
{
public:
	btcdtor(Selector, Composite){}
protected:
	virtual void onInitialize();
	virtual Status update(cDict& dict);
protected:
	Behavior_ptrs::iterator mCurrent;
};

// ============================================================================
class PrioritySelector : public Selector
{
public :
	btcdtor(PrioritySelector, Selector){}
protected:
	virtual Status update(cDict& dict);
};

// ============================================================================
class Parallel : public Composite
{
public:
	enum Policy
	{
		RequireOne,
		RequireAll,
	};

	btcdtor(Parallel, Composite):mSuccessPolicy(RequireOne),mFailurePolicy(RequireAll){}
	btgetset(Policy, SuccessPolicy);
	btgetset(Policy, FailurePolicy);

protected:	
	virtual Status update(cDict& dict);
	virtual void onTerminate(Status);
};

// ============================================================================
class ActiveSelector : public Selector
{
public:
	btcdtor(ActiveSelector, Selector){}

protected:
	virtual void onInitialize();
	virtual Status update(cDict& dict);
};



// ============================================================================
class Action : public Behavior
{
public:
	typedef std::function<Status(cDict&)> func_type;
	btcdtor(Action, Behavior){}
	btgetset(func_type, Action)
protected:
	virtual Status update(cDict& dict) { return mAction(dict); }
};

class Condition : public Behavior
{
public:
	typedef std::function<bool(cDict&)> func_type;
	btcdtor(Condition, Behavior){}
	btgetset(func_type, Condition)
protected:
	virtual Status update(cDict& dict) { return mCondition(dict) ? BH_SUCCESS : BH_FAILURE; }
	
};
