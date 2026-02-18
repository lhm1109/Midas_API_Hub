#pragma once

#include "HeaderPre.h"

class IDgnPerformDataBase;

namespace dgn
{
    namespace converter
    {
        class CProductDBDocHelper;

        class __MY_EXT_CLASS__ IProductDgnLcomSetter
        {
        public:
            IProductDgnLcomSetter();
            virtual ~IProductDgnLcomSetter();

        public:
            bool SetDgnLcomList(IDgnPerformDataBase* pIDataBase) const;

        protected:
            virtual bool DoSetDgnLcomList(IDgnPerformDataBase* pIDataBase) const = 0;

        protected:
            const CProductDBDocHelper* GetHelper() const;
            const CProductDBDocHelper* m_pHelper;
        };
    }
}

#include "HeaderPost.h"
