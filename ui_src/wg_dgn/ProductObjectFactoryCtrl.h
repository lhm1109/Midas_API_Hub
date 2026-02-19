#pragma once

#include <map>

#include "TProductTypeStruct.h"

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        class IProductObjectFactory;
        class IProductDgnLcomSetter;
        class IProductMembKeySeparator;
        class IProductSaveObjectBuilder;
        enum enResultDatabaseType;

        class __MY_EXT_CLASS__ CProductObjectFactoryCtrl final
        {
        public:
            CProductObjectFactoryCtrl();
            ~CProductObjectFactoryCtrl();

        public:
            static bool AddObjectFactory(const IProductObjectFactory* pFactory);

            static const IProductDgnLcomSetter*     CreateDgnLcomSetter(enResultDatabaseType enDBType);
            static const IProductMembKeySeparator*  CreateMembKeySeparator(enResultDatabaseType enDBType);
            static const IProductSaveObjectBuilder* CreateSaveObjectBuilder(enResultDatabaseType enDBType);

        private:
            static const IProductObjectFactory* GetObjectFactory(enResultDatabaseType enDBType);

        private:
            static std::map<enResultDatabaseType, const IProductObjectFactory*> m_mObjectFactory;
        };
    }
}

#include "HeaderPost.h"
