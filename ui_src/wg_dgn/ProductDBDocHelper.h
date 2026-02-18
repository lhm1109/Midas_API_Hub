#pragma once

//빈번하게 불리우는 제품 및 DGN_link의 Class들을 사용하기 위한 Helper Class

#include "../dgnengine/idesign/DGN_lib/Macro.h"
#include "../dgnengine/idesign/DGN_def/Rebar2D.h"

#include "TKeyData.h"
#include "TProductTypeStruct.h"

#include "ProductDBDocHelperPreDecl.h"

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        enum class enProductDBDocHelperWrapper;
        class CProductDBDocHelperWrapper;

        class __MY_EXT_CLASS__ CProductDBDocHelper final
        {
            typedef idgn::FIterator                           FIterator;
            typedef link::IDgnDoc                             IDgnDoc;
            typedef link::Pepe                                IDgnPerform;
            typedef def::DataType                             DataType;
            typedef def::enMemb                               enMemb;
            typedef def::enDgn                                enDgn;
            typedef def::enUnitType                           enUnitType;
            typedef def::enSector                             enSector;
            typedef def::CRebarBeam                           CRebarBeam;
            typedef def::CDgnRevision                         CDgnRevision;
            typedef def::CResultUnitOut                       CResultUnitOut;
            typedef def::TResultVItem                         TResultVItem;
            typedef def::TResultVMemb                         TResultVMemb;
            typedef item::enDgnResultItem                     enDgnResultItem;
            typedef std::pair<int, enMemb>                    MembTypePair;
            typedef std::pair<CString, enDgn>                 DgnTypePair;
            typedef TKeyData<T_KEY, T_WALL_D>                 TWallKeyData;
            typedef Macro::TNumber<def::enRebar2D>            Rebar2DNumb;
            typedef std::array<tRebar2D, 4>                   T_RBMS_D_List;
            typedef std::pair<T_SLAB_BAR, T_SLAB_BAR>         T_SLAB_BAR_Pair;
            typedef enProductDBDocHelperWrapper               WrapperEnum;

        public:
            CProductDBDocHelper(CDBDoc* pDBDoc);
            ~CProductDBDocHelper();

        public:
            CDBDoc*       GetDBDoc() const;
            CPostCtrl*    GetPostCtrl() const;
            CAttrCtrl*    GetAttrCtrl() const;
            CAttrCtrl2*   GetAttrCtrl2() const;
            CMembCtrl*    GetMembCtrl() const;
            CUnitCtrl*    GetUnitCtrl() const;
            CDgnDataCtrl* GetDgnDataCtrl() const;
            CMatlDB*      GetMatlDB() const;

            IDgnDoc*     GetIDgnDoc() const;
            IDgnPerform* GetIDgnPerform() const;

        public:
            TProductDgnType GetTDgnType(T_KEY MembKey) const;
            TProductDgnType GetTRepDgnType(const std::vector<T_KEY>& vMembKey) const;
            TProductDgnType GetTRepDgnType(const T_KEY_LIST& aMembKey) const;
            enResultDatabaseType GetResultDBType(const T_KEY_LIST& aMembKey) const;
            enResultDatabaseType GetResultDBType(const std::vector<T_KEY>& vMembKey) const;
            enResultDatabaseType GetResultDBType(T_KEY MembKey) const;

            bool CreateRebarData(T_KEY MembKey) const;
            bool ChangeCurUnitIdx(T_KEY MembKey) const;
            bool IsStrengthLcom(int LcomType, T_KEY LcomKey) const;

            bool HasDgnResultByItrRefer(const TResultVItem& EItemDataList, FIterator ItrRefer) const;

            int  GetDgnResultList(const TResultVItem& EItemDataList, std::vector<TResultVMemb>& rvDataList) const;
            bool GetDgnResultListBySector(const TResultVItem& EItemDataList, enSector Sector, std::vector<TResultVMemb>& rData) const;
            bool GetDgnResultByCriticalSector(const TResultVItem& EItemDataList, enSector Sector, TResultVMemb& rData) const;
            bool GetDgnResultByItrRefer(const TResultVItem& EItemDataList, FIterator ItrRefer, TResultVMemb& rData) const;

            int GetCutlineResultItrList(std::vector<FIterator>& vItr) const;

            double GetRebarArea(const CString& strRebar) const;

            const T_ELEM_D* GetElemData(T_KEY MembKey) const;
            const T_REBB_D* GetRebarBeamData(T_KEY MembKey, enProductDgnMode CurDgnMode) const;
            const T_REBC_D* GetRebarColumnData(T_KEY MembKey, enProductDgnMode CurDgnMode) const;
            const T_RCHK_BEAM GetRebarBeamDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const;
            const T_RCHK_COLM GetRebarColumnDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const;
            const T_REBT_D GetRebarGeneralColumnDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const;
            const T_SBDO_D GetRebarPlateDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const;
            const T_RPCM_D GetRebarPlateColumnDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const;
            const std::vector<T_RBSL_D> GetRebarShellData(T_KEY MembKey, enProductDgnMode CurDgnMode) const;

            const T_REBR_D* GetRebarBraceData(T_KEY MembKey, enProductDgnMode CurDgnMode) const;
            const T_REBW_D* GetRebarWallData(T_KEY MembKey, enProductDgnMode CurDgnMode) const;
            const T_RBMS_D_List* GetRebarMeshedSlabFlexDataList(T_KEY ElemKey, enProductDgnMode CurDgnMode) const;

            int GetLcomKeyList(int LcomType, T_LCOM_K_LIST& rData) const;
            int GetDesignRebarRevList(T_KEY MembKey, std::vector<CDgnRevision>& rvRebarRev) const;
            int GetDesignRebarRevListByType(DataType Type, T_KEY MembKey, std::vector<CDgnRevision>& rvRebarRev) const;

            CDesignResult* GetDesignResultPtr() const;

            UINT GetInternalWallID(T_KEY MembKey) const;
            const TWallKeyData* GetWallKeyData(T_KEY MembKey) const;
            const T_WALL_K* GetWallKeyUnion(UINT InternalWallID) const;
            const T_STOR_D* GetStoryData(T_STOR_K StoryKey) const;
            const T_WMAK_D* GetWallMarkData(T_WALL_KEY WallKey) const;

            T_KEY GetRepMembKeyInSbdoMember(T_KEY CurElemKey) const;

            double GetLengthFactorNU(enDgn EDgnType) const;
            double GetAreaFactorNU(enDgn EDgnType) const;
            double GetStressFactorNU(enDgn EDgnType) const;
            double GetForceFactorNU(enDgn EDgnType) const;
            double GetUnitMomentFactorNU(enDgn EDgnType) const;
            double GetUnitForceFactorNU(enDgn EDgnType) const;
            double GetUnitWidth(enDgn EDgnType) const;

            int GetLengthUnitType(enDgn EDgnType) const;

            CResultUnitOut CreateResultUnitOut(enDgn EDgnType, enDgnResultItem EItem, const TResultVMemb& crData) const;

            UINT GetUpperColmNo(UINT NodeK) const;
            UINT GetLowerColmNo(UINT NodeK) const;

            UINT GetElemNodeNum(UINT ElemK) const;

            bool GetUserWallThick(const UINT& StoryID, const UINT& WallID, double& dThick) const;

        private:
            bool ConvertBeamRebar(double dLengthFactorNU, const CRebarBeam& crERebar, T_REBB_BASE& rTop, T_REBB_BASE& rBot) const;
            bool GetRebarSlabDataListByRebarRevList(T_KEY ElemKey, const std::vector<CDgnRevision>& vRebarRev, T_RBMS_D_List& rData) const;

            enDgn   GetEDgnType(T_KEY MembKey) const;
            CString GetPDgnType(T_KEY MembKey) const;
            const T_MATD_D* GetMatdData(T_KEY MembKey) const;

            const enUnitType* GetUnitType(enDgn EDgnType) const;

        private:
            CProductDBDocHelperWrapper* GetWrapper() const;

        private:
            CDBDoc* m_pDBDoc;
            CProductDBDocHelperWrapper* m_pWrapper;
        };
    }
}

#include "HeaderPost.h"
