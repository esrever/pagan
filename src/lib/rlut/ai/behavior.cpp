#include "behavior.h"

#include <algorithm>

namespace pgn
{
	namespace bt
	{
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

		// ============================================================================
		cDecorator::cDecorator(const cDecorator& v)
			:cBehavior(v)
			, mChild(v.mChild)
		{
		}

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

		// ============================================================================
		cComposite::cComposite(const cComposite& v)
			:cBehavior(v)
			, mChildren(v.mChildren)
		{
		}

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

		// ============================================================================
		cCondition::cCondition(const cCondition& v)
			: cBehavior(v)
			, mCondition(v.mCondition)
		{
		}
	}
}
