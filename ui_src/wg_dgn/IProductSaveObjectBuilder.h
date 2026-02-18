#pragma once

#include "TProductTypeStruct.h"

#include "HeaderPre.h"

class IDgnPerformDataBase;

namespace dgn
{
    namespace def
    {
        enum class enMemb;
    }
}

namespace dgn
{
    namespace converter
    {
        class CProductDBDocHelper;
        class IProductDataBaseHelper;
        class IProductResultTracker;
        class IProductResultInitializer;
        class IProductResultSaver;

        class __MY_EXT_CLASS__ IProductSaveObjectBuilder
        {
        public:
            IProductSaveObjectBuilder();
            virtual ~IProductSaveObjectBuilder();

        public:
            IProductDataBaseHelper*    CreateDataBaseHelper(IDgnPerformDataBase* pIDataBase) const;
            IProductResultTracker*     CreateResultTracker(const CProductDBDocHelper* pDBDocHelper) const;
            IProductResultInitializer* CreateInitializer() const;
            IProductResultSaver*       CreateSaver(const TProductMembType& TMembType, const CProductDBDocHelper* pDBDocHelper, const IProductDataBaseHelper* pIDataBaseHelper) const;

        protected:
            virtual IProductDataBaseHelper*    DoCreateDataBaseHelper(IDgnPerformDataBase* pIDataBase) const = 0;
            virtual IProductResultTracker*     DoCreateResultTracker(const CProductDBDocHelper* pDBDocHelper) const = 0;
            virtual IProductResultInitializer* DoCreateInitializer() const = 0;
            virtual IProductResultSaver*       DoCreateSaver(const TProductMembType& TMembType, const CProductDBDocHelper* pDBDocHelper, const IProductDataBaseHelper* pIDataBaseHelper) const = 0;
        };
    }
}

#include "HeaderPost.h"
