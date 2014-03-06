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

namespace pgn
{

	class cDict;

	// ============================================================================
	enum class eBehaviorStatus : int8_t
		/**
		* Return values of and valid states for behaviors.
		*/
	{
		Invalid,
		Success,
		Failure,
		Running,
		Aborted,
	};

	// ============================================================================
	class Behavior
		/**
		* Base class for actions, conditions and composites.
		*/
	{
	public:
		Behavior() : mStatus(eBehaviorStatus::Invalid), mPriority(0.0f){};
		Behavior(const Behavior&);
		virtual ~Behavior(){}

		virtual eBehaviorStatus update(cDict& dict) { return eBehaviorStatus::Invalid; };
		virtual void onInitialize()			{}
		virtual void onTerminate(eBehaviorStatus)	{}
		Behavior * addChild(Behavior*) { return this; }

		eBehaviorStatus Tick(cDict& dict);
		void reset();
		void abort();
		bool isTerminated() const;
		bool isRunning() const;
		eBehaviorStatus getStatus() const;
		virtual Behavior * clone() const { return nullptr; };

		btgetset(float, Priority);

	private:
		eBehaviorStatus mStatus;
	};

	typedef Behavior* Behavior_ptr;
	typedef std::shared_ptr<Behavior> Behavior_sptr;
	typedef std::vector<Behavior_ptr> Behavior_ptrs;

	// ============================================================================

	class Decorator : public Behavior
	{
	public:
		btcdtor(Decorator, Behavior) :mChild(nullptr){}
		btgetset(Behavior_ptr, Child);
	};

	// ============================================================================
	class Repeat : public Decorator
	{
	public:
		btcdtor(Repeat, Decorator) :mCount(-1), mCounter(0){}
		btgetset(int, Count);
		btgetset(int, Counter);

		void onInitialize();
		eBehaviorStatus update(cDict& dict);
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
		virtual eBehaviorStatus update(cDict& dict);

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
		virtual eBehaviorStatus update(cDict& dict);
	protected:
		Behavior_ptrs::iterator mCurrent;
	};

	// ============================================================================
	class PrioritySelector : public Selector
	{
	public:
		btcdtor(PrioritySelector, Selector){}
	protected:
		virtual eBehaviorStatus update(cDict& dict);
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

		btcdtor(Parallel, Composite) :mSuccessPolicy(RequireOne), mFailurePolicy(RequireAll){}
		btgetset(Policy, SuccessPolicy);
		btgetset(Policy, FailurePolicy);

	protected:
		virtual eBehaviorStatus update(cDict& dict);
		virtual void onTerminate(eBehaviorStatus);
	};

	// ============================================================================
	class ActiveSelector : public Selector
	{
	public:
		btcdtor(ActiveSelector, Selector){}

	protected:
		virtual void onInitialize();
		virtual eBehaviorStatus update(cDict& dict);
	};



	// ============================================================================
	class Action : public Behavior
	{
	public:
		typedef std::function<eBehaviorStatus(cDict&)> func_type;
		btcdtor(Action, Behavior){}
		btgetset(func_type, Action)
	protected:
		virtual eBehaviorStatus update(cDict& dict) { return mAction(dict); }
	};

	class Condition : public Behavior
	{
	public:
		typedef std::function<bool(cDict&)> func_type;
		btcdtor(Condition, Behavior){}
		btgetset(func_type, Condition)
	protected:
		virtual eBehaviorStatus update(cDict& dict) { return mCondition(dict) ? eBehaviorStatus::Success : eBehaviorStatus::Failure; }

	};
}
