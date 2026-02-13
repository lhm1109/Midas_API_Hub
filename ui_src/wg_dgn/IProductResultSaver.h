#pragma once

#include "..\dgnengine\idesign\DGN_link\IDgnResultController.h"

#include "ProductResultSaverEnumDef.h"
#include "TProductTypeStruct.h"

#include "HeaderPre.h"

class CDesignResult;
class IDgnPerformDataBase;
struct _DGN_LCOM;

namespace idgn
{
    typedef	DWORD_PTR FIterator;
}

namespace dgn
{
    namespace def
    {
        enum class DataType : unsigned int;
        class CDgnRevision;
        class CDgnLesultBundle;
    }

    namespace link
    {
        struct TEngineResultInfo;
    }
}

namespace dgn
{
    namespace item
    {
        enum class enDgnResultItem : unsigned int;
    }

    namespace converter
    {
        struct TEngineResult;
        class  CProductDBDocHelper;
        class  IProductDataBaseHelper;

        class __MY_EXT_CLASS__ IProductResultSaver
        {
            typedef def::DataType         DataType;
            typedef def::CDgnRevision     CDgnRevision;
            typedef idgn::FIterator       FIterator;
        public:
            IProductResultSaver(const CProductDBDocHelper* pDBDocHelper, 
                                const TProductDgnType& TDgnType,
                                const TProductMembType& TMembType);
            virtual ~IProductResultSaver();

        public:
            TProductDgnType GetTDgnType() const;
            TProductMembType GetTMembType() const;

        public:
            bool Initialize(T_KEY MembKey);
            bool GetEngineResultRepInfo(dgn::link::TEngineResultInfo& rInfo) const;

            bool SaveDgnResult(const TEngineResult& crEResultAll, IDgnPerformDataBase* pIDataBase) const;
            bool SaveDgnForce(const TEngineResult& crEResultAll, IDgnPerformDataBase* pIDataBase) const;
            bool SetResultFlag() const;
            bool WriteFileDgnResult(IDgnPerformDataBase* pIDataBase) const;
            bool WriteFileDgnForce(IDgnPerformDataBase* pIDataBase) const;
            bool MakePDgnLcomKeyMap();
            bool SetLcomResult(const UINT& iMemb, const dgn::def::CDgnLesultBundle* pLesultBundle, IDgnPerformDataBase* pIDataBase);

        public:
            virtual bool DoInitialize(T_KEY MembKey) = 0;
            virtual bool SetLcomResult(idgn::FIterator EDgnLcomItr, idgn::FIterator itrRefer, const dgn::item::enDgnResultItem& EItem, const std::vector<double>& crData)  {  ASSERT(0); return true;  }
            
        protected:
            virtual bool IsEnableDesignMember() const = 0;
            virtual bool IsDesignMember(T_KEY MembKey) const = 0;

            
            virtual bool DoGetEngineResultRepInfo(dgn::link::TEngineResultInfo& rInfo) const = 0;

            virtual bool DoSaveDgnResult(const TEngineResult& crEResultAll, IDgnPerformDataBase* pIDataBase) const = 0;
            virtual bool DoSaveDgnForce(const TEngineResult& crEResultAll, IDgnPerformDataBase* pIDataBase) const = 0;
            virtual bool DoSetResultFlag() const = 0;
            virtual bool DoWriteFileDgnResult(IDgnPerformDataBase* pIDataBase) const = 0;
            virtual bool DoWriteFileDgnForce(IDgnPerformDataBase* pIDataBase) const = 0;

            virtual const IProductDataBaseHelper* GetDataBaseHelper() const = 0;

        protected:
            CMap<T_LCOM_K, T_LCOM_K, _DGN_LCOM, _DGN_LCOM&>* GetLcomList() const;
            bool InitializeDefault(T_KEY MembKey);

            bool IsPLType(FIterator EItrRefer, DataType Type) const;
            bool IsPLPointType(FIterator EItrRefer) const;
            bool IsPLPositionType(FIterator EItrRefer) const;
            bool IsPL2DForceType(FIterator EItrRefer) const;
            bool IsPLCutline(FIterator EItrRefer) const;

            bool  SetCurMembKey(T_KEY MembKey);
            T_KEY GetCurMembKey() const;

            const CProductDBDocHelper* GetDBDocHelper() const;
            enProductDgnMode GetCurDgnMode() const;
            enProductDgnMode GetCurDgnMode(T_KEY MembKey) const;
            bool SetDgnMode(T_KEY MembKey);

            UINT GetPDgnLcomKey(idgn::FIterator itr) const;

        private:
            const CProductDBDocHelper* m_pDBDocHelper;
            const TProductDgnType m_TDgnType;
            const TProductMembType m_TMembType;
            T_KEY m_MembKey;
            enProductDgnMode m_DgnMode;
            std::map<idgn::FIterator, UINT> mPDgnLcomKeyMap;
        };
    }
}

#include "HeaderPost.h"
