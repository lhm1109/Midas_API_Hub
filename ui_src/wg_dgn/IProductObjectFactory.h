#pragma once

#include "TProductTypeStruct.h"

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        class IProductDgnLcomSetter;
        class IProductMembKeySeparator;
        class IProductSaveObjectBuilder;

        class __MY_EXT_CLASS__ IProductObjectFactory
        {
        public:
            IProductObjectFactory(enResultDatabaseType Type);
            virtual ~IProductObjectFactory();

        public:
            enResultDatabaseType GetResultDBType() const;

            const IProductDgnLcomSetter*     CreateDgnLcomSetter() const;
            const IProductMembKeySeparator*  CreateMembKeySeparator() const;
            const IProductSaveObjectBuilder* CreateSaveObjectBuilder() const;

        protected:
            virtual const IProductDgnLcomSetter*     DoCreateDgnLcomSetter() const = 0;
            virtual const IProductMembKeySeparator*  DoCreateMembKeySeparator() const = 0;
            virtual const IProductSaveObjectBuilder* DoCreateSaveObjectBuilder() const = 0;

        private:
            const enResultDatabaseType m_enResultDBType;
        };
    }
}

#include "HeaderPost.h"
