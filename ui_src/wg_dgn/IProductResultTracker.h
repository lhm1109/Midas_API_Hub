#pragma once

#include <map>

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        enum class enProductResultSaveFlag
        {
            None,
            Done,
            Number
        };

        class CProductDBDocHelper;
        class IProductResultSaver;

        class __MY_EXT_CLASS__ IProductResultTracker
        {
        private:
            typedef enProductResultSaveFlag Flag;
            typedef std::pair<T_KEY, TProductMembType> PairKeyType;

        public:
            IProductResultTracker(const CProductDBDocHelper* pDBDocHelper);
            virtual ~IProductResultTracker();

            bool IsSkipCondition(T_KEY MembKey, const IProductResultSaver* pISaver) const;
            bool SetDone(T_KEY MembKey, const IProductResultSaver* pISaver);

        protected:
            virtual PairKeyType DoCreateKeySet(T_KEY MembKey, const IProductResultSaver* pISaver) const = 0;

        protected:
            const CProductDBDocHelper* GetDBDocHelper() const;
            const CProductDBDocHelper* m_pDBDocHelper;

        private:
            std::map<PairKeyType, Flag> m_mKeyFlag;
        };
    }
}

#include "HeaderPost.h"
