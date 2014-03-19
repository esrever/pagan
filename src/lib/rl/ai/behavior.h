#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include <map>
#include <functional>

#define CLONE_DECL(T) virtual cBehavior * clone() const { return new T (*this);};
//#define CLONE_DECL(T)

#define btvar(T,N) private:\
	T m##N;

#define btget(T,N) public:\
	const T & Get##N##() const { return m##N; }

#define btset(T,N) public:\
	void Set##N##(const T & v) { m##N = v; }

#define btgetset(T,N) \
	btvar( T , N )\
	btget( T , N )\
	btset(T, N)

#define btcdtor(T, U) \
typedef U super;\
virtual ~ T (){}\
virtual cBehavior * Clone() const { return new T (*this);}\
virtual const char * Type() const { return #T; }\
T (const T &);\
T ()

#define btser \
virtual size_t SerializeIn(const pugi::xml_node& node); \
virtual void SerializeOut(pugi::xml_node& node); 

namespace pugi
{
	class xml_node;
}

namespace pgn
{

	struct cBlackBoard;

	namespace bt
	{

		// ============================================================================
		enum class eStatus : int8_t
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
		class cBehavior
			/**
			* Base class for actions, conditions and composites.
			*/
		{
		public:
			cBehavior() : mStatus(eStatus::Invalid), mPriority(0.0f){};
			cBehavior(const cBehavior&);
			virtual ~cBehavior(){}
			virtual const char * Type() const { return "Behavior"; };
			btser

			virtual eStatus Update(cBlackBoard& bb) { return eStatus::Invalid; };
			virtual void OnInitialize()			{}
			virtual void OnTerminate(eStatus)	{}
			cBehavior * AddChild(cBehavior*) { return this; }

			eStatus Tick(cBlackBoard& bb);
			void Reset();
			void Abort();
			bool IsTerminated() const;
			bool IsRunning() const;
			eStatus GetStatus() const;
			virtual cBehavior * Clone() const { return nullptr; };

			btgetset(float, Priority);
			btgetset(std::string, Name);


			static size_t SerializeIn(const pugi::xml_node& node, cBehavior*& ptr);

		private:
			eStatus mStatus;
		};

		typedef cBehavior* cBehavior_ptr;
		typedef std::shared_ptr<cBehavior> cBehavior_sptr;
		typedef std::vector<cBehavior_ptr> cBehavior_ptrs;

		// ============================================================================

		class cDecorator : public cBehavior
		{
		public:
			btcdtor(cDecorator, cBehavior) :mChild(nullptr){}
			btser
			btgetset(cBehavior_ptr, Child);
		};

		// ============================================================================
		class cRepeat : public cDecorator
		{
		public:
			btcdtor(cRepeat, cDecorator) :mCount(-1), mCounter(0){}
			btser
			btgetset(int, Count);
			btgetset(int, Counter);

			void OnInitialize();
			eStatus Update(cBlackBoard& bb);
		};

		// ============================================================================

		class cComposite : public cBehavior
		{
		public:
			btcdtor(cComposite, cBehavior){}
			btser
			cBehavior * AddChild(cBehavior* child) { mChildren.push_back(child); return this; }
		protected:
			cBehavior_ptrs mChildren;
		};

		// ============================================================================
		class cSequence : public cComposite
		{
		public:
			btcdtor(cSequence, cComposite){}
		protected:
			virtual void OnInitialize();
			virtual eStatus Update(cBlackBoard& bb);

		protected:
			cBehavior_ptrs::iterator mCurrentChild;
		};

		// ============================================================================

		class cSelector : public cComposite
		{
		public:
			btcdtor(cSelector, cComposite){}
		protected:
			virtual void OnInitialize();
			virtual eStatus Update(cBlackBoard& bb);
		protected:
			cBehavior_ptrs::iterator mCurrent;
		};

		// ============================================================================
		class cPrioritySelector : public cSelector
		{
		public:
			btcdtor(cPrioritySelector, cSelector){}
		protected:
			virtual eStatus Update(cBlackBoard& bb);
		};

		// ============================================================================
		class cParallel : public cComposite
		{
		public:
			enum ePolicy
			{
				RequireOne,
				RequireAll,
			};

			btcdtor(cParallel, cComposite) :mSuccessPolicy(RequireOne), mFailurePolicy(RequireAll){}
			btser
			btgetset(ePolicy, SuccessPolicy);
			btgetset(ePolicy, FailurePolicy);

		protected:
			virtual eStatus Update(cBlackBoard& bb);
			virtual void OnTerminate(eStatus);
		};

		// ============================================================================
		class cActiveSelector : public cSelector
		{
		public:
			btcdtor(cActiveSelector, cSelector){}

		protected:
			virtual void OnInitialize();
			virtual eStatus Update(cBlackBoard& bb);
		};



		// ============================================================================
		class cAction : public cBehavior
		{
		public:
			typedef std::function<eStatus(cBlackBoard&)> func_type;
			btcdtor(cAction, cBehavior){}
			btser
			btgetset(func_type, Action);

			static void Register(const std::string& s, func_type f) { msFuncs[s] = f; }
			const std::map<std::string, func_type>& Funcs() const { return msFuncs; }

		protected:
			virtual eStatus Update(cBlackBoard& bb) { return mAction(bb); }
		private:
			static std::map<std::string,func_type> msFuncs;
		};

		class cCondition : public cBehavior
		{
		public:
			typedef std::function<bool(cBlackBoard&)> func_type;
			btcdtor(cCondition, cBehavior){}
			btser
			btgetset(func_type, Condition);

			static void Register(const std::string& s, func_type f) { msFuncs[s] = f; }
			const std::map<std::string, func_type>& Funcs() const { return msFuncs; }

		protected:
			virtual eStatus Update(cBlackBoard& bb) { return mCondition(bb) ? eStatus::Success : eStatus::Failure; }
		private:
			static std::map<std::string, func_type> msFuncs;
		};
	}
}
