#include "behavior.h"

#include <algorithm>

#include <core/serialize/serialize.h>

namespace pgn
{
	namespace bt
	{
		size_t cBehavior::SerializeIn(const pugi::xml_node& node, cBehavior*& ptr)
		{
			const std::string name = node.name();
			if (name == "Repeat")
				ptr = new cRepeat();
			else if (name == "Sequence")
				ptr = new cSequence();
			else if (name == "Selector")
				ptr = new cSelector();
			else if (name == "PrioritySelector")
				ptr = new cPrioritySelector();
			else if (name == "Parallel")
				ptr = new cParallel();
			else if (name == "ActiveSelector")
				ptr = new cActiveSelector();
			else if (name == "Action")
				ptr = new cAction();
			else if (name == "Condition")
				ptr = new cCondition();
			if (ptr)
				return ptr->SerializeIn(node);
			else
				return 0;
		}

		// ============================================================================
		cBehavior::cBehavior(const cBehavior& v)
			:mStatus(v.mStatus)
			, mPriority(v.mPriority)
			, mName(v.mName)
		{
		}

		eStatus cBehavior::Tick(cBlackBoard& bb)
		{
			if (mStatus != eStatus::Running)
			{
				OnInitialize();
			}

			mStatus = Update(bb);

			if (mStatus != eStatus::Running)
			{
				OnTerminate(mStatus);
			}
			return mStatus;
		}

		void cBehavior::Reset()
		{
			mStatus = eStatus::Invalid;
		}

		void cBehavior::Abort()
		{
			OnTerminate(eStatus::Aborted);
			mStatus = eStatus::Aborted;
		}

		bool cBehavior::IsTerminated() const
		{
			return mStatus == eStatus::Success || mStatus == eStatus::Failure;
		}

		bool cBehavior::IsRunning() const
		{
			return mStatus == eStatus::Running;
		}

		eStatus cBehavior::GetStatus() const
		{
			return mStatus;
		}

		size_t cBehavior::SerializeIn(const pugi::xml_node& node)
		{ 
			return pgn::SerializeIn( node, "name", mName);
		}

		void cBehavior::SerializeOut(pugi::xml_node& node)
		{
			pgn::SerializeOut(node, "name", mName);
		}

		// ============================================================================
		cDecorator::cDecorator(const cDecorator& v)
			:cBehavior(v)
			, mChild(v.mChild ? v.mChild->Clone() : nullptr)
		{
		}

		size_t cDecorator::SerializeIn(const pugi::xml_node& node)
		{
			size_t ret = 0;
			ret += cBehavior::SerializeIn(*node.begin(), mChild);
			if (!ret) return 0;
			return ret + cBehavior::SerializeIn(node);
		}
		void cDecorator::SerializeOut(pugi::xml_node& node)
		{
			cBehavior::SerializeOut(node);
			mChild->SerializeOut(node);
		}
		// ============================================================================
		cRepeat::cRepeat(const cRepeat& v)
			: cDecorator(v)
			, mCounter(v.mCounter)
			, mCount(v.mCount)
		{
		}

		void cRepeat::OnInitialize()
		{
			mCounter = 0;
		}

		eStatus cRepeat::Update(cBlackBoard& bb)
		{
			for (;;)
			{
				GetChild()->Tick(bb);
				if (GetChild()->GetStatus() == eStatus::Running) break;
				if (GetChild()->GetStatus() == eStatus::Failure) return eStatus::Failure;
				if (++mCounter == mCount) return eStatus::Success;
				GetChild()->Reset();
			}
			return eStatus::Invalid;
		}

		size_t cRepeat::SerializeIn(const pugi::xml_node& node)
		{
			size_t ret = 0;
			ret += cDecorator::SerializeIn(node);
			if (!ret) return false;
			return ret + pgn::SerializeIn(node, "count", mCount);
		}
		void cRepeat::SerializeOut(pugi::xml_node& node)
		{
			cDecorator::SerializeOut(node);
			pgn::SerializeOut(node, "count", mCount);
		}

		// ============================================================================
		cComposite::cComposite(const cComposite& v)
			:cBehavior(v)
		{
			for (const auto c : v.mChildren)
				mChildren.push_back(c ? c->Clone() : nullptr);
		}

		size_t cComposite::SerializeIn(const pugi::xml_node& node)
		{
			size_t ret = 0;
			ret += cBehavior::SerializeIn(node);
			if (!ret) return 0;
			for (pugi::xml_node::iterator it = node.children().begin(); it != node.children().end(); ++ it)
			{
				mChildren.push_back(nullptr);
				if (!cBehavior::SerializeIn(*it, mChildren.back()))
					return 0;
			}
			return 1;
		}
		void cComposite::SerializeOut(pugi::xml_node& node)
		{
			cBehavior::SerializeOut(node);
			for (const auto& c : mChildren)
				c->SerializeOut(node.append_child(c->Type()));
		}

		// ============================================================================

		cSequence::cSequence(const cSequence& v)
			: cComposite(v)
			, mCurrentChild(mChildren.end())
		{
		}

		void cSequence::OnInitialize()
		{
			mCurrentChild = mChildren.begin();
		}

		eStatus cSequence::Update(cBlackBoard& bb)
		{
			// Keep going until a child behavior says it's running.
			for (;;)
			{
				eStatus s = (*mCurrentChild)->Tick(bb);

				// If the child fails, or keeps running, do the same.
				if (s != eStatus::Success)
				{
					return s;
				}

				// Hit the end of the array, job done!
				if (++mCurrentChild == mChildren.end())
				{
					return eStatus::Success;
				}
			}
		}

		// ============================================================================
		cSelector::cSelector(const cSelector& v)
			: cComposite(v)
			, mCurrent(mChildren.end())
		{
		}

		void cSelector::OnInitialize()
		{
			mCurrent = mChildren.begin();
		}


		eStatus cSelector::Update(cBlackBoard& bb)
		{
			// Keep going until a child behavior says its running.
			for (;;)
			{
				eStatus s = (*mCurrent)->Tick(bb);

				// If the child succeeds, or keeps running, do the same.
				if (s != eStatus::Failure)
				{
					return s;
				}

				// Hit the end of the array, it didn't end well...
				if (++mCurrent == mChildren.end())
				{
					return eStatus::Failure;
				}
			}
		}

		// ============================================================================
		cPrioritySelector::cPrioritySelector(const cPrioritySelector& v)
			: cSelector(v)
		{
		}

		eStatus cPrioritySelector::Update(cBlackBoard& bb)
		{
			// TODO: no need to do it if priorities not changed
			std::sort(mChildren.begin(), mChildren.end(), [](const cBehavior * lhs, const cBehavior * rhs){ return lhs->GetPriority() > rhs->GetPriority();  });
			return cSelector::Update(bb);
		}

		// ============================================================================
		cParallel::cParallel(const cParallel& v)
			: cComposite(v)
			, mSuccessPolicy(v.mSuccessPolicy)
			, mFailurePolicy(v.mFailurePolicy)
		{
		}

		eStatus cParallel::Update(cBlackBoard& bb)
		{
			size_t iSuccessCount = 0, iFailureCount = 0;

			for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
			{
				cBehavior& b = **it;
				if (!b.IsTerminated())
				{
					b.Tick(bb);
				}

				if (b.GetStatus() == eStatus::Success)
				{
					++iSuccessCount;
					if (mSuccessPolicy == RequireOne)
					{
						return eStatus::Success;
					}
				}

				if (b.GetStatus() == eStatus::Failure)
				{
					++iFailureCount;
					if (mFailurePolicy == RequireOne)
					{
						return eStatus::Failure;
					}
				}
			}

			if (mFailurePolicy == RequireAll  &&  iFailureCount == mChildren.size())
			{
				return eStatus::Failure;
			}

			if (mSuccessPolicy == RequireAll  &&  iSuccessCount == mChildren.size())
			{
				return eStatus::Success;
			}

			return eStatus::Running;
		}

		void cParallel::OnTerminate(eStatus)
		{
			for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
			{
				cBehavior& b = **it;
				if (b.IsRunning())
				{
					b.Abort();
				}
			}
		}

		size_t cParallel::SerializeIn(const pugi::xml_node& node)
		{
			if (!cComposite::SerializeIn(node))
				return false;
			mSuccessPolicy = strcmp( node.attribute("success").as_string(), "any") == 0 ? ePolicy::RequireOne : ePolicy::RequireAll;
			mFailurePolicy = strcmp(node.attribute("fail").as_string(), "any") == 0 ? ePolicy::RequireOne : ePolicy::RequireAll;
			return 1;
		}
		void cParallel::SerializeOut(pugi::xml_node& node)
		{
			cComposite::SerializeOut(node);
			node.append_attribute("success").set_value(mSuccessPolicy == ePolicy::RequireOne ? "any" : "all");
			node.append_attribute("fail").set_value(mFailurePolicy == ePolicy::RequireOne ? "any" : "all");
		}


		// ============================================================================
		cActiveSelector::cActiveSelector(const cActiveSelector& v)
			: cSelector(v)
		{
		}

		void cActiveSelector::OnInitialize()
		{
			mCurrent = mChildren.end();
		}

		eStatus cActiveSelector::Update(cBlackBoard& bb)
		{
			auto previous = mCurrent;

			cSelector::OnInitialize();
			eStatus result = cSelector::Update(bb);

			if (previous != mChildren.end() && mCurrent != previous)
			{
				(*previous)->OnTerminate(eStatus::Aborted);
			}
			return result;
		}

		// ============================================================================
		std::map<std::string, cAction::func_type> cAction::msFuncs;

		cAction::cAction(const cAction& v)
			: cBehavior(v)
			, mAction(v.mAction)
		{
		}

		size_t cAction::SerializeIn(const pugi::xml_node& node)
		{
			if (!cBehavior::SerializeIn(node))
				return 0;
			mAction = msFuncs[GetName()];
			return 1;
		}
		void cAction::SerializeOut(pugi::xml_node& node)
		{
			cBehavior::SerializeOut(node);
		}

		// ============================================================================
		std::map<std::string, cCondition::func_type> cCondition::msFuncs;
		cCondition::cCondition(const cCondition& v)
			: cBehavior(v)
			, mCondition(v.mCondition)
		{
		}

		size_t cCondition::SerializeIn(const pugi::xml_node& node)
		{
			if (!cBehavior::SerializeIn(node))
				return 0;
			mCondition = msFuncs[GetName()];
			return 1;
		}
		void cCondition::SerializeOut(pugi::xml_node& node)
		{
			cBehavior::SerializeOut(node);
		}
	}
}
