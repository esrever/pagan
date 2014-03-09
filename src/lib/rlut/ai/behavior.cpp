#include "behavior.h"

#include <algorithm>

#include <pugixml.hpp>

namespace pgn
{
	namespace bt
	{
		bool cBehavior::SerializeIn(const pugi::xml_node& node, cBehavior*& ptr)
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
				return false;
		}

		// ============================================================================
		cBehavior::cBehavior(const cBehavior& v)
			:mStatus(v.mStatus)
			, mPriority(v.mPriority)
		{
		}

		eStatus cBehavior::Tick(cDict& dict)
		{
			if (mStatus != eStatus::Running)
			{
				OnInitialize();
			}

			mStatus = Update(dict);

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

		bool cBehavior::SerializeIn(const pugi::xml_node& node)
		{ 
			mName = node.attribute("name").as_string();
			return true;
		}

		void cBehavior::SerializeOut(pugi::xml_node& node)
		{
			node.append_attribute("name").set_value(mName.c_str());
		}

		// ============================================================================
		cDecorator::cDecorator(const cDecorator& v)
			:cBehavior(v)
			, mChild(v.mChild)
		{
		}

		bool cDecorator::SerializeIn(const pugi::xml_node& node)
		{
			if (!cBehavior::SerializeIn(*node.begin(), mChild))
				return false;
			return cBehavior::SerializeIn(node);
		}
		void cDecorator::SerializeOut(pugi::xml_node& node)
		{
			cBehavior::SerializeOut(node);
			mChild->SerializeOut(node.append_child(mChild->Type()));
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

		eStatus cRepeat::Update(cDict& dict)
		{
			for (;;)
			{
				GetChild()->Tick(dict);
				if (GetChild()->GetStatus() == eStatus::Running) break;
				if (GetChild()->GetStatus() == eStatus::Failure) return eStatus::Failure;
				if (++mCounter == mCount) return eStatus::Success;
				GetChild()->Reset();
			}
			return eStatus::Invalid;
		}

		bool cRepeat::SerializeIn(const pugi::xml_node& node)
		{
			if (!cDecorator::SerializeIn(node))
				return false;
			mCount = node.attribute("count").as_int();
			return true;
		}
		void cRepeat::SerializeOut(pugi::xml_node& node)
		{
			cDecorator::SerializeOut(node);
			node.append_attribute("count").set_value(mCount);
		}

		// ============================================================================
		cComposite::cComposite(const cComposite& v)
			:cBehavior(v)
			, mChildren(v.mChildren)
		{
		}

		bool cComposite::SerializeIn(const pugi::xml_node& node)
		{
			if (!cBehavior::SerializeIn(node))
				return false;
			for (pugi::xml_node::iterator it = node.children().begin(); it != node.children().end(); ++ it)
			{
				mChildren.push_back(nullptr);
				if (!cBehavior::SerializeIn(*it, mChildren.back()))
					return false;
			}
			return true;
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

		eStatus cSequence::Update(cDict& dict)
		{
			// Keep going until a child behavior says it's running.
			for (;;)
			{
				eStatus s = (*mCurrentChild)->Tick(dict);

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


		eStatus cSelector::Update(cDict& dict)
		{
			// Keep going until a child behavior says its running.
			for (;;)
			{
				eStatus s = (*mCurrent)->Tick(dict);

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

		eStatus cPrioritySelector::Update(cDict& dict)
		{
			// TODO: no need to do it if priorities not changed
			std::sort(mChildren.begin(), mChildren.end(), [](const cBehavior * lhs, const cBehavior * rhs){ return lhs->GetPriority() > rhs->GetPriority();  });
			return cSelector::Update(dict);
		}

		// ============================================================================
		cParallel::cParallel(const cParallel& v)
			: cComposite(v)
			, mSuccessPolicy(v.mSuccessPolicy)
			, mFailurePolicy(v.mFailurePolicy)
		{
		}

		eStatus cParallel::Update(cDict& dict)
		{
			size_t iSuccessCount = 0, iFailureCount = 0;

			for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
			{
				cBehavior& b = **it;
				if (!b.IsTerminated())
				{
					b.Tick(dict);
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

		bool cParallel::SerializeIn(const pugi::xml_node& node)
		{
			if (!cComposite::SerializeIn(node))
				return false;
			mSuccessPolicy = strcmp( node.attribute("success").as_string(), "any") == 0 ? ePolicy::RequireOne : ePolicy::RequireAll;
			mFailurePolicy = strcmp(node.attribute("fail").as_string(), "any") == 0 ? ePolicy::RequireOne : ePolicy::RequireAll;
			return true;
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

		eStatus cActiveSelector::Update(cDict& dict)
		{
			auto previous = mCurrent;

			cSelector::OnInitialize();
			eStatus result = cSelector::Update(dict);

			if (previous != mChildren.end() && mCurrent != previous)
			{
				(*previous)->OnTerminate(eStatus::Aborted);
			}
			return result;
		}

		// ============================================================================
		cAction::cAction(const cAction& v)
			: cBehavior(v)
			, mAction(v.mAction)
		{
		}

		bool cAction::SerializeIn(const pugi::xml_node& node)
		{
			if (!cBehavior::SerializeIn(node))
				return false;
			//! TODO: function type retrieval
			return true;
		}
		void cAction::SerializeOut(pugi::xml_node& node)
		{
			cBehavior::SerializeOut(node);
			//! TODO: function type export
		}

		// ============================================================================
		cCondition::cCondition(const cCondition& v)
			: cBehavior(v)
			, mCondition(v.mCondition)
		{
		}

		bool cCondition::SerializeIn(const pugi::xml_node& node)
		{
			if (!cBehavior::SerializeIn(node))
				return false;
			//! TODO: function type retrieval
			return true;
		}
		void cCondition::SerializeOut(pugi::xml_node& node)
		{
			cBehavior::SerializeOut(node);
			//! TODO: function type export
		}
	}
}
