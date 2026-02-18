#pragma once

#include "..\dgnengine\idesign\DGN_link\IDgnResultController.h"

#include "TProductTypeStruct.h"

class IDgnPerformDataBase;

namespace dgn
{
    namespace def
    {
        enum class enMemb;
        enum class enDgn;
    }

    namespace link
    {
        struct TEngineResultInfo;
    }

    namespace converter
    {
        class  CProductLcomResultController;
        class  IProductSaveObjectBuilder;
        class  CProductDBDocHelper;
        class  IProductDataBaseHelper;
        class  IProductResultSaver;
        struct TEngineResult;
        class  IProductResultTracker;

        class CProductResultSaveController : public dgn::link::IDgnResultController
        {
        public:
            CProductResultSaveController(const CProductDBDocHelper* pDBDocHelper, const IProductSaveObjectBuilder* pBuilder);
            ~CProductResultSaveController();

        protected:
            //여기는 Lcom 결과 control 하는애들
            virtual bool Initialize() override;
            virtual bool GetEngineResultRepInfo(const UINT& iMemb, dgn::link::TEngineResultInfo& rInfo) const override;
            virtual bool SetLcomResult(const UINT& iMemb, idgn::FIterator EDgnLcomItr, idgn::FIterator itrRefer, const dgn::item::enDgnResultItem& EItem, const std::vector<double>& crData) override;
            virtual bool SetLcomResult(const UINT& iMemb, const dgn::def::CDgnLesultBundle* pLesultBundle) override;

            //여기 아래는 Critical 결과 control 하는애들
        public:
            bool Play(T_KEY MembKey, IDgnPerformDataBase* pIDataBase) const;
            bool MakeISaverList(T_KEY MembKey, IDgnPerformDataBase* pIDataBase) const;

        private:
            bool Initialize(T_KEY MembKey, IDgnPerformDataBase* pIDataBase) const;
            bool SaveData_WriteFile(T_KEY MembKey, IDgnPerformDataBase* pIDataBase) const;

        private:
            bool GetEngineResult(const dgn::link::TEngineResultInfo& Info, TEngineResult& rData) const;
            int GetTMembTypeList(T_KEY MembKey, IDgnPerformDataBase* pIDataBase, std::vector<TProductMembType>& rvType) const;

        private:
            int GetISaverList(T_KEY MembKey, IDgnPerformDataBase* pIDataBase, std::vector<IProductResultSaver*>& rvpISaver) const;
            const IProductDataBaseHelper* GetIDataBaseHelper(T_KEY MembKey, IDgnPerformDataBase* pIDataBase) const;
            IProductDataBaseHelper* GetIDataBaseHelper(TProductDgnType DgnType) const;
            IProductResultTracker* GetTracker() const;

            bool HasIDataBaseHelper(const TProductDgnType& TDgnType) const;
            bool HasISaver(const TProductMembType& TMembType) const;

            const CProductDBDocHelper* GetDBDocHelper() const;
            const IProductSaveObjectBuilder* GetBuilder() const;

        private:
            const CProductDBDocHelper*       m_pDBDocHelper;
            const IProductSaveObjectBuilder* m_pBuilder;

        private:
            std::map<TProductDgnType, const IProductDataBaseHelper*> m_mTypeDataBaseHelper;
            std::map<TProductMembType, IProductResultSaver*>         m_mTypeSaver;
            IProductResultTracker* m_pTracker;
        };
    }
}