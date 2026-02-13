#pragma once

//[P] : Product, [E] : Engine
//Engine Type은 코드작성할때 가독성을 위해 넣어둠ㅠㅠ

#include "HeaderPre.h"

namespace dgn
{
    namespace def
    {
        enum class enMemb;
        enum class enDgn;
    }
}

namespace dgn
{
    namespace converter
    {
        enum class enProductSubType
        {
            None,
            Flexural,
            OneWayFlexural,
            PunchingShear,
            OneWayShear,
            Serviceabilty,
            Stress,
            Shear,
            Number,
        };

        struct __MY_EXT_CLASS__ TProductMembType final
        {
            typedef def::enMemb enMemb;
            typedef enProductSubType enPSubType;

            TProductMembType();
            TProductMembType(int PMembType, enPSubType PSubType, enMemb EMembType);

            bool operator < (const TProductMembType& src) const;
            bool operator == (const TProductMembType& src) const;

            bool IsValid() const;

            int        GetPMembType() const;
            enPSubType GetPSubType() const;
            enMemb     GetEMembType() const;

        private:
            const int        PMembType; //ref DB_ST_DN.h : MembType Macro
            const enPSubType PSubType;
            const enMemb     EMembType;
        };

        struct __MY_EXT_CLASS__ TProductDgnType final
        {
            typedef def::enDgn enDgn;

            TProductDgnType();
            TProductDgnType(const CString& PDgnType, enDgn EDgnType);

            bool operator < (const TProductDgnType& src) const;
            bool operator == (const TProductDgnType& src) const;

            bool IsValid() const;

            CString GetPDgnType() const;
            enDgn GetEDgnType() const;

        private:
            const CString PDgnType;
            const enDgn   EDgnType;
        };

        enum enResultDatabaseType
        {
            Rcs,
            Rcsc,
            Acs,
        };

        class __MY_EXT_CLASS__ CResultDataBaseTypeUtil
        {
        public:
            CResultDataBaseTypeUtil();
            virtual ~CResultDataBaseTypeUtil();

        public:
            static enResultDatabaseType GetResultDataBaseType(dgn::def::enDgn nDgn, dgn::def::enMemb nMemb);
        };
    }
}

#include "HeaderPost.h"
