#pragma once

#include "TProductTypeStruct.h"

namespace dgn
{
    namespace lib
    {
        class CDgnProgObserver;
    }
}

namespace dgn
{
    namespace converter
    {
        class  CProductDBDocHelper;
        class  IProductMembKeySeparator;
        class  IProductSaveObjectBuilder;
        class  CProductResultSaveController;

        class IEngineResultConvert final
        {
        public:
            IEngineResultConvert(const CProductDBDocHelper* pDBDocHelper, enResultDatabaseType enDBType, bool bRecalc);
            ~IEngineResultConvert();

        public:
            bool Initialize(const T_KEY_LIST& SuperMembKeyList, IDgnPerformDataBase* pIDataBase);
            int  ConvertResults(lib::CDgnProgObserver* pProgObserver, const T_KEY_LIST& SuperMembKeyList, IDgnPerformDataBase* pIDataBase);

        private:
            bool ConvertResult(T_KEY SuperMembKey, IDgnPerformDataBase* pIDataBase) const;
            bool IsValidMembKey(T_KEY MembKey) const;

        private:
            const CProductDBDocHelper*       m_pDBDocHelper;
            const IProductMembKeySeparator*  m_pSeparator;
            const IProductSaveObjectBuilder* m_pBuilder;
            CProductResultSaveController*    m_pController;
			bool m_bRecalc;
        };
    }
}