#include "StdAfx.h"

#include "../dgnengine/idesign/DGN_lib/TemporaryObject.h"

#include "../dgnengine/idesign/DGN_link/IDgnDoc.h"
#include "../dgnengine/idesign/DGN_link/IDgnDocCtrl.h"
#include "../dgnengine/idesign/DGN_link/IDgnPerform.h"

#include "../dgnengine/idesign/DGN_def/DgnRevisionDef.h"
#include "../dgnengine/idesign/DGN_def/DgnResultBundleDef.h"
#include "../dgnengine/idesign/DGN_def/DgnUnitUtil.h"
#include "../dgnengine/idesign/DGN_def/RebarBeam.h"
#include "../dgnengine/idesign/DGN_def/ResultUnitOut.h"
#include "../dgnengine/idesign/DGN_def/DgnSectorDef.h"
#include "../dgnengine/idesign/DGN_def/MemberRebarBeamDef.h"
#include "../dgnengine/idesign/DGN_def/MemberRebarColumnDef.h"
#include "../dgnengine/idesign/DGN_def/MemberRebarWallDef.h"
#include "../dgnengine/idesign/DGN_def/SectionRebar2DDef.h"
#include "../dgnengine/idesign/DGN_def/SectionRebar2DAddDef.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_db/AttrCtrl2.h"
#include "../wg_db/MatlDB.h"
#include "../wg_db/UnitCtrl.h"
#include "../wg_db/PostCtrl.h"
#include "../wg_db/MembCtrl.h"

#include "TEngineResult.h"
#include "DgnTypeConverter.h"
#include "ProductResultSaverEnumDef.h"
#include "TDBDataFunctor.h"
#include "TKeyData.h"
#include "DgnDataCtrl.h"
#include "TSafeDeleteFunctor.h"
#include "ProductDBDocHelperWrapper.h"

#include "ProductDBDocHelper.h"
#include "../dgnengine/idesign/DGN_def/RebarColumn.h"
#include "../dgnengine/idesign/DGN_def/SectionRebarPlateDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::link;
using namespace dgn::def;
using namespace dgn::converter;

CProductDBDocHelper::CProductDBDocHelper(CDBDoc* pDBDoc) :
    m_pDBDoc(pDBDoc),
    m_pWrapper(new CProductDBDocHelperWrapper())
{
    if ( !m_pDBDoc )
    {
        ASSERT(0);
    }
}

CProductDBDocHelper::~CProductDBDocHelper()
{
    TSafeDeleteFunctor()(m_pWrapper);
}

CDBDoc* CProductDBDocHelper::GetDBDoc() const
{
    return m_pDBDoc;
}

CPostCtrl* CProductDBDocHelper::GetPostCtrl() const
{
    auto* pPostCtrl = GetDBDoc()->m_pPostCtrl;
    if ( !pPostCtrl )
    {
        ASSERT(0); return nullptr;
    }

    return pPostCtrl;
}

CAttrCtrl* CProductDBDocHelper::GetAttrCtrl() const
{
    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;
    if ( !pAttrCtrl )
    {
        ASSERT(0); return nullptr;
    }

    return pAttrCtrl;
}

CAttrCtrl2* CProductDBDocHelper::GetAttrCtrl2() const
{
    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl2;
    if ( !pAttrCtrl )
    {
        ASSERT(0); return nullptr;
    }

    return pAttrCtrl;
}

CMembCtrl* CProductDBDocHelper::GetMembCtrl() const
{
    return CMembCtrl::GetMembCtrl();
}

CUnitCtrl* CProductDBDocHelper::GetUnitCtrl() const
{
    auto* pUnitCtrl = GetDBDoc()->m_pUnitCtrl;
    if ( !pUnitCtrl )
    {
        ASSERT(0); return nullptr;
    }

    return pUnitCtrl;
}

CDgnDataCtrl* CProductDBDocHelper::GetDgnDataCtrl() const
{
    auto* pDgnDataCtrl = GetDBDoc()->m_pDgnDataCtrl;
    if ( !pDgnDataCtrl )
    {
        ASSERT(0); return nullptr;
    }

    return pDgnDataCtrl;
}

CMatlDB* CProductDBDocHelper::GetMatlDB() const
{
    auto* pMatlDB = GetDBDoc()->m_pMatlDB;
    if ( !pMatlDB )
    {
        ASSERT(0); return nullptr;
    }

    return pMatlDB;
}

IDgnDoc* CProductDBDocHelper::GetIDgnDoc() const
{
    auto* const pIDgnDoc = IDgnDocCtrl::Instance()->GetDgnDoc(GetDBDoc());
    if ( !pIDgnDoc )
    {
        ASSERT(0); return nullptr;
    }

    return pIDgnDoc;
}

IDgnPerform* CProductDBDocHelper::GetIDgnPerform() const
{
    auto* const pIDgnPerform = GetIDgnDoc()->GetDgnPerform();
    if ( !pIDgnPerform )
    {
        ASSERT(0); return nullptr;
    }

    return pIDgnPerform;
}

TProductDgnType CProductDBDocHelper::GetTDgnType(T_KEY MembKey) const
{
    const auto PDgnType = GetPDgnType(MembKey);
    const auto EDgnType = GetEDgnType(MembKey);

    return TProductDgnType(PDgnType, EDgnType);
}

TProductDgnType CProductDBDocHelper::GetTRepDgnType(const std::vector<T_KEY>& vMembKey) const
{
    if ( vMembKey.empty() )
    {
        ASSERT(0); return lib::TemporaryObject<TProductDgnType>();
    }

#ifdef _DEBUG
    std::vector<TProductDgnType> vTDgnType;
    vTDgnType.clear();
    for ( const auto& MembKey : vMembKey )
    {
        vTDgnType.push_back(GetTDgnType(MembKey));
    }

    if ( !Macro::Equal(vTDgnType) )
    {
        ASSERT(0); return lib::TemporaryObject<TProductDgnType>();
    }

    return vTDgnType.front();
#else
    return GetTDgnType(vMembKey.front());
#endif
}

TProductDgnType CProductDBDocHelper::GetTRepDgnType(const T_KEY_LIST& aMembKey) const
{
    std::vector<T_KEY> vMembKey;
    vMembKey.clear();

#ifdef _DEBUG
    const auto nMembKeySize = static_cast<int>(aMembKey.GetSize());
    for ( int nIdx = 0; nIdx < nMembKeySize; ++nIdx )
    {
        vMembKey.push_back(aMembKey[nIdx]);
    }
#else
    vMembKey.push_back(aMembKey[0]);
#endif

    return GetTRepDgnType(vMembKey);
}

enResultDatabaseType CProductDBDocHelper::GetResultDBType(const T_KEY_LIST& aMembKey) const
{
    std::vector<T_KEY> vMembKey;
    vMembKey.clear();

#ifdef _DEBUG
    const auto nMembKeySize = static_cast<int>(aMembKey.GetSize());
    for ( int nIdx = 0; nIdx < nMembKeySize; ++nIdx )
    {
        vMembKey.push_back(aMembKey[nIdx]);
    }
#else
    vMembKey.push_back(aMembKey[0]);
#endif

    return GetResultDBType(vMembKey);
}

enResultDatabaseType CProductDBDocHelper::GetResultDBType(const std::vector<T_KEY>& vMembKey) const
{
    if ( vMembKey.empty() )
    {
        ASSERT(0); return lib::TemporaryObject<enResultDatabaseType>();
    }

#ifdef _DEBUG
    std::vector<enResultDatabaseType> vResultDBType;
    vResultDBType.clear();
    for ( const auto& MembKey : vMembKey )
    {
        vResultDBType.push_back(GetResultDBType(MembKey));
    }

    if ( !Macro::Equal(vResultDBType) )
    {
        ASSERT(0); return lib::TemporaryObject<enResultDatabaseType>();
    }

    return vResultDBType.front();
#else
    return GetResultDBType(vMembKey.front());
#endif
}

enResultDatabaseType CProductDBDocHelper::GetResultDBType(T_KEY MembKey) const
{
    auto TDgnType = GetTDgnType(MembKey);
    int nMembType = GetAttrCtrl()->GetMemberType(MembKey);
    switch ( TDgnType.GetEDgnType() )
    {
    case enDgn::RCS:
        {   //겁나 구리지만 Shell 때문에.. 방도가 없네..
#if defined(_MGEN)
            switch ( nMembType )
            {
            case D_MBTP_BEAM:
            case D_MBTP_COLUMN:
            case D_MBTP_BRACE:
            case D_MBTP_WALL:
            case D_MBTP_SHELL:
            case D_MBTP_ETC:
                return enResultDatabaseType::Rcs;
            default:
                break;
            }
#elif defined(_CIVIL)
            switch ( nMembType )
            {
            case D_MBTP_BEAM:
            case D_MBTP_COLUMN:
            case D_MBTP_BRACE:
            case D_MBTP_PLATEBEAM:
            case D_MBTP_PLATECOLM:
                return enResultDatabaseType::Rcsc;
            case D_MBTP_SHELL:
                return enResultDatabaseType::Rcs;
            default:
                break;
            }
#endif
        }
        break;
    case enDgn::STL:
        return enResultDatabaseType::Acs;
    default:
        break;
    }
    ASSERT(0);
    return enResultDatabaseType::Rcs;
}

bool CProductDBDocHelper::CreateRebarData(T_KEY MembKey) const
{
    const auto EDgnType = GetEDgnType(MembKey);
    const auto* pUnitType = GetUnitType(EDgnType);

    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);
    const auto nForce  = CDgnUnitUtil::GetFUnitForce(*pUnitType);

    GetMatlDB()->Create_RebarData(nLength, nForce, true);

    return true;
}

bool CProductDBDocHelper::ChangeCurUnitIdx(T_KEY MembKey) const
{
    const auto EDgnType = GetEDgnType(MembKey);
    const auto* pUnitType = GetUnitType(EDgnType);

    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);
    const auto nForce  = CDgnUnitUtil::GetFUnitForce(*pUnitType);

    T_UNIT_INDEX ChangeIndex;
    ChangeIndex.nBase_Length = nLength;
    ChangeIndex.nBase_Force  = nForce;
    GetUnitCtrl()->SetUnitIndexCurrent(ChangeIndex);

    return true;
}

bool CProductDBDocHelper::IsStrengthLcom(int LcomType,
                                         T_KEY LcomKey) const
{
    T_LCOM_D LcomD;
    if ( !GetAttrCtrl()->GetLcom(LcomType, LcomKey, LcomD) )
    {
        ASSERT(0); return false;
    }

    switch ( LcomD.nActive )
    {
    case EN_DGN_STRN:
    case EN_DGN_SPEC:
    case EN_DGN_VERT:
    case EN_DGN_STRE:
    case EN_DGN_UG_STRN:
    case EN_DGN_UG_SPEC:
        return true;
    case EN_DGN_NONE:
    case EN_DGN_SERV:
    case EN_DGN_UG_SERV:
        return false;
    default:
        ASSERT(0); return false;
    }
}

bool CProductDBDocHelper::HasDgnResultByItrRefer(const TResultVItem& EItemDataList, FIterator ItrRefer) const
{
    std::vector<TResultVMemb> vData;
    if ( !GetDgnResultList(EItemDataList, vData) )
    {
        ASSERT(0); return false;
    }

    for ( const auto& Data : vData )
    {
        if ( Data.itrRefer == ItrRefer )
        {
            return true;
        }
    }

    return false;
}

int CProductDBDocHelper::GetDgnResultList(const TResultVItem& EItemDataList,
                                          std::vector<TResultVMemb>& rDataList) const
{
    return GetIDgnPerform()->GetDesignResults(EItemDataList, rDataList);
}

bool CProductDBDocHelper::GetDgnResultListBySector(const TResultVItem& EItemDataList,
                                                   enSector Sector,
                                                   std::vector<TResultVMemb>& rDataList) const
{
    return GetIDgnPerform()->GetDesignResults(EItemDataList, Sector, rDataList);
}

bool CProductDBDocHelper::GetDgnResultByCriticalSector(const TResultVItem& EItemDataList,
                                                       enSector Sector,
                                                       TResultVMemb& rData) const
{
    return GetIDgnPerform()->GetDesignResultCritical(EItemDataList, Sector, rData);
}

bool CProductDBDocHelper::GetDgnResultByItrRefer(const TResultVItem& EItemDataList,
                                                 FIterator ItrRefer,
                                                 TResultVMemb& rData) const
{
    std::vector<TResultVMemb> vData;
    if ( !GetDgnResultList(EItemDataList, vData) )
    {
        ASSERT(0); return false;
    }

    for ( const auto& Data : vData )
    {
        if ( Data.itrRefer == ItrRefer )
        {
            rData = Data;
            return true;
        }
    }

    return false;
}

int CProductDBDocHelper::GetCutlineResultItrList(std::vector<FIterator>& vItr) const
{
    return GetIDgnPerform()->GetCuttingLineResultItrList(vItr);
}

double CProductDBDocHelper::GetRebarArea(const CString& strRebar) const
{
    return GetMatlDB()->Get_RebarArea(strRebar);
}

const T_ELEM_D* CProductDBDocHelper::GetElemData(T_KEY MembKey) const
{
    typedef WrapperEnum TEnum;
    typedef T_KEY       TKey;
    typedef T_ELEM_D    TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::Element);

    if ( pKeyData->GetKey() != MembKey )
    {
        pKeyData->Init();
        pKeyData->Key = MembKey;
        auto& rData = pKeyData->Data;

        if ( !GetAttrCtrl()->GetElem(MembKey, rData) )
        {
            ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

const T_REBB_D* CProductDBDocHelper::GetRebarBeamData(T_KEY MembKey,
                                                      enProductDgnMode CurDgnMode) const
{
    typedef WrapperEnum TEnum;
    typedef T_KEY       TKey;
    typedef T_REBB_D    TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::RebarBeam);

    if ( pKeyData->GetKey() != MembKey )
    {
        pKeyData->Init();
        pKeyData->Key = MembKey;
        auto& rData = pKeyData->Data;

        switch ( CurDgnMode )
        {
        case enProductDgnMode::Design:
            {
                std::vector<CDgnRevision> vRebarRev;
                if ( !GetDesignRebarRevList(MembKey, vRebarRev) )
                {
                    ASSERT(0); return nullptr;
                }

                const auto& RebarDataType =
                    CDataTypeUtil::GetDPoolType(DataType::MemberRebarBeam);

                Macro::Filter(vRebarRev, 
                    [&RebarDataType] (const CDgnRevision& RebarRev)
                {
                    const auto& CurDataType = RebarRev.iDataType;
                    return (CurDataType == RebarDataType);
                });

                if ( vRebarRev.size() != std::size_t(1) )
                {
                    ASSERT(0); return nullptr;
                }

                const auto& RebarRev = vRebarRev.front();
                const auto& CurItrRefer = RebarRev.itrRefer;

                const auto* pDBData = TDBDataFunctor<CMemberRebarBeam>()(CurItrRefer);
                const auto& RebarI = pDBData->GetRebar(enSector::I);
                const auto& RebarM = pDBData->GetRebar(enSector::M);
                const auto& RebarJ = pDBData->GetRebar(enSector::J);

                //const auto& EDgnType = GetEDgnType(MembKey);
                //const auto dLengthFactorNU = GetLengthFactorNU(EDgnType);
                const auto dLengthFactorNU = 1.0;

                rData.SubRebarName = RebarI.Shear.Rebar.Dia.strName.c_str();
                rData.strSkinName  = RebarI.Skin.Data.Rebar.Dia.strName.c_str();
                rData.dB = RebarI.aBot[0].dCenterCover * dLengthFactorNU;
                rData.dT = RebarI.aTop[0].dCenterCover * dLengthFactorNU;

                ConvertBeamRebar(dLengthFactorNU, RebarI, rData.Top_I, rData.Bot_I);
                ConvertBeamRebar(dLengthFactorNU, RebarM, rData.Top_C, rData.Bot_C);
                ConvertBeamRebar(dLengthFactorNU, RebarJ, rData.Top_J, rData.Bot_J);
            }
            break;
        case enProductDgnMode::Checking:
            {
			    //중립단위계
			    CCurUnitSaver Save(TRUE);
			    T_UNIT_INDEX ChangeIndex;
			    ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
			    ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
                GetDBDoc()->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);

                auto* pAttrCtrl = GetAttrCtrl();
                const auto RebarKey = pAttrCtrl->GetRebbByElemK(MembKey);
                if ( !pAttrCtrl->GetRebb(RebarKey, rData) )
                {
                    ASSERT(0); return nullptr;
                }
            }
            break;
        default: ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

const T_REBC_D* CProductDBDocHelper::GetRebarColumnData(T_KEY MembKey,
                                                        enProductDgnMode CurDgnMode) const
{
    typedef WrapperEnum TEnum;
    typedef T_KEY       TKey;
    typedef T_REBC_D    TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::RebarColumn);

    if ( pKeyData->GetKey() != MembKey )
    {
        pKeyData->Init();
        pKeyData->Key = MembKey;
        auto& rData = pKeyData->Data;

        switch ( CurDgnMode )
        {
        case enProductDgnMode::Design:
            {
                std::vector<CDgnRevision> vRebarRev;
                if ( !GetDesignRebarRevList(MembKey, vRebarRev) )
                {
                    ASSERT(0); return nullptr;
                }

                const auto& RebarDataType =
                    CDataTypeUtil::GetDPoolType(DataType::MemberRebarColumn);

                Macro::Filter(vRebarRev,
                              [&RebarDataType] (const CDgnRevision& RebarRev)
                {
                    const auto& CurDataType = RebarRev.iDataType;
                    return (CurDataType == RebarDataType);
                });

                if ( vRebarRev.size() != std::size_t(1) )
                {
                    ASSERT(0); return nullptr;
                }

                const auto& RebarRev = vRebarRev.front();
                const auto& CurItrRefer = RebarRev.itrRefer;

                const auto* pDBData = TDBDataFunctor<CMemberRebarColumn>()(CurItrRefer);
                const auto& EndBar  = pDBData->GetRebar(enSector::End);
                const auto& MidBar  = pDBData->GetRebar(enSector::Mid);

                rData.MainRebarName = CW2A(EndBar.aMain[0].Rebar.Dia.strName);
                if ( EndBar.aMain[0].nCol == 0 ) // Circle
                {
                    rData.nQrb = EndBar.aMain[0].nRow;
                }
                else // Rectangle
                {
                    rData.nQrb = EndBar.aMain[0].nRow * 2 + EndBar.aMain[0].nCol * 2 - 4;
                    rData.nRow = EndBar.aMain[0].nRow;
                }

                //const auto& EDgnType = GetEDgnType(MembKey);
                //const auto dLengthFactorNU = GetLengthFactorNU(EDgnType);
                const auto dLengthFactorNU = 1.0;

                rData.dConcCenter          = EndBar.aMain[0].dCenterCover * dLengthFactorNU;
                rData.bSameRebarEndNCenter = FALSE;
                rData.nHoopType            = 1; // 1 = Tied  2 = Spiral
                rData.SubRebarName         = EndBar.Shear.Rebar.Dia.strName.c_str();
                rData.dSubRebarSpace       = EndBar.Shear.Rebar.dSpace * dLengthFactorNU;
                rData.iSubRebarNum[0]      = EndBar.Shear.nLegHor;
                rData.iSubRebarNum[1]      = EndBar.Shear.nLegVer;
                rData.SubRebarName2        = MidBar.Shear.Rebar.Dia.strName.c_str();
                rData.dSubRebarSpace2      = MidBar.Shear.Rebar.dSpace * dLengthFactorNU;
                rData.iSubRebarNum2[0]     = MidBar.Shear.nLegHor;
                rData.iSubRebarNum2[1]     = MidBar.Shear.nLegVer;
                rData.bUseCornerRebar      = FALSE;
                rData.CornerRebarName.Empty();

                rData.iBCJointRebarNum     = 0;
            }
            break;
        case enProductDgnMode::Checking:
            {
			    //중립단위계
			    CCurUnitSaver Save(TRUE);
			    T_UNIT_INDEX ChangeIndex;
			    ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
			    ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
			    GetDBDoc()->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);

                auto* pAttrCtrl = GetAttrCtrl();
                const auto RebarKey = pAttrCtrl->GetRebcByElemK(MembKey);
                if ( !pAttrCtrl->GetRebc(RebarKey, rData) )
                {
                    ASSERT(0); return nullptr;
                }
            }
            break;
        default: ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

const T_RCHK_BEAM CProductDBDocHelper::GetRebarBeamDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const
{
    T_RCHK_D rData;

    auto _l_ConvertBeamRebar = [](double dLengthFactorNU, const CRebarBeam& crERebar, T_RCHK_BEAM& Beam, int nIMJ) -> bool
    {
        if ( !crERebar.IsValidData() )
        {
            ASSERT(0); return false;
        }

        Beam.strTopBarNa1[nIMJ][0] = crERebar.aTop[0].Rebar.Dia.strName.c_str();
        Beam.iTopBarNum[nIMJ][0] = crERebar.aTop[0].nNum;
        Beam.strBotBarNa1[nIMJ][0] = crERebar.aBot[0].Rebar.Dia.strName.c_str();
        Beam.iBotBarNum[nIMJ][0] = crERebar.aBot[0].nNum;
        Beam.strSubBarNa[nIMJ] = crERebar.Shear.Rebar.Dia.strName.c_str();
        Beam.dSubBarNum[nIMJ] = crERebar.Shear.nLeg;
        Beam.dSubBarDist[nIMJ] = crERebar.Shear.Rebar.dSpace * dLengthFactorNU;

        return true;
    };

    switch ( CurDgnMode )
    {
    case enProductDgnMode::Design:
        {
            std::vector<CDgnRevision> vRebarRev;
            if ( !GetDesignRebarRevList(MembKey, vRebarRev) )
            {
                ASSERT(0); return rData.BEAM;
            }

            const auto& RebarDataType =
                CDataTypeUtil::GetDPoolType(DataType::MemberRebarBeam);

            Macro::Filter(vRebarRev,
                [&RebarDataType] (const CDgnRevision& RebarRev)
            {
                const auto& CurDataType = RebarRev.iDataType;
                return (CurDataType == RebarDataType);
            });

            if ( vRebarRev.size() != std::size_t(1) )
            {
                ASSERT(0); return rData.BEAM;
            }

            const auto& RebarRev = vRebarRev.front();
            const auto& CurItrRefer = RebarRev.itrRefer;

            const auto* pDBData = TDBDataFunctor<CMemberRebarBeam>()(CurItrRefer);
            const auto& RebarI = pDBData->GetRebar(enSector::I);
            const auto& RebarM = pDBData->GetRebar(enSector::M);
            const auto& RebarJ = pDBData->GetRebar(enSector::J);

            //const auto& EDgnType = GetEDgnType(MembKey);
            //const auto dLengthFactorNU = GetLengthFactorNU(EDgnType);
            const auto dLengthFactorNU = 1.0;

            _l_ConvertBeamRebar(dLengthFactorNU, RebarI, rData.BEAM, 0);
            _l_ConvertBeamRebar(dLengthFactorNU, RebarM, rData.BEAM, 1);
            _l_ConvertBeamRebar(dLengthFactorNU, RebarJ, rData.BEAM, 2);
        }
        break;
    case enProductDgnMode::Checking:
        {
            auto* pAttrCtrl = GetAttrCtrl();
            auto RchkK = pAttrCtrl->GetRchkByElemK(MembKey);
            if ( !pAttrCtrl->GetRchk(RchkK, rData) )
            {
                ASSERT(0); return rData.BEAM;
            }
        }
        break;
    default: ASSERT(0); return rData.BEAM;
    }

    return rData.BEAM;
}

const T_RCHK_COLM CProductDBDocHelper::GetRebarColumnDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const
{
    T_RCHK_D rData;

    auto _l_ConvertColumnRebar = [] (double dLengthFactorNU, const CRebarColumn& crERebar, T_RCHK_COLM& Column) -> bool
    {
        //이거 어떻게 꼽아줘야되지. 
        for ( int i = 0; i<3; ++i )
        {
            Column.strBarNa1[i][0][0] = crERebar.aMain[0].Rebar.Dia.strName.c_str();
            Column.iBarNum[i][0][0] = crERebar.aMain[0].nRow;
            Column.iBarNum[i][1][0] = crERebar.aMain[0].nCol;

            Column.strSubBarNa[i] = crERebar.Shear.Rebar.Dia.strName.c_str();
            Column.strSubBarNa_y[i] = crERebar.Shear.Rebar.Dia.strName.c_str();
            Column.strSubBarNa_z[i] = crERebar.Shear.Rebar.Dia.strName.c_str();
            Column.dSubBarNum[i] = min(crERebar.Shear.nLegHor, crERebar.Shear.nLegVer);
            Column.dSubBarNum_y[i] = crERebar.Shear.nLegHor;
            Column.dSubBarNum_z[i] = crERebar.Shear.nLegVer;
            Column.dSubBarDist[i] = crERebar.Shear.Rebar.dSpace * dLengthFactorNU;
        }

        return true;
    };

    switch ( CurDgnMode )
    {
    case enProductDgnMode::Design:
        {
            std::vector<CDgnRevision> vRebarRev;
            if ( !GetDesignRebarRevList(MembKey, vRebarRev) )
            {
                ASSERT(0); return rData.COLM;
            }

            const auto& RebarDataType =
                CDataTypeUtil::GetDPoolType(DataType::MemberRebarColumn);

            Macro::Filter(vRebarRev,
                [&RebarDataType] (const CDgnRevision& RebarRev)
            {
                const auto& CurDataType = RebarRev.iDataType;
                return (CurDataType == RebarDataType);
            });

            if ( vRebarRev.size() != std::size_t(1) )
            {
                ASSERT(0); return rData.COLM;
            }

            const auto& RebarRev = vRebarRev.front();
            const auto& CurItrRefer = RebarRev.itrRefer;

            const auto* pDBData = TDBDataFunctor<CMemberRebarColumn>()(CurItrRefer);
            const auto& Rebar = pDBData->GetRebar(enSector::End);

            //const auto& EDgnType = GetEDgnType(MembKey);
            //const auto dLengthFactorNU = GetLengthFactorNU(EDgnType);
            const auto dLengthFactorNU = 1.0;

            _l_ConvertColumnRebar(dLengthFactorNU, Rebar, rData.COLM);
        }
        break;
    case enProductDgnMode::Checking:
        {
            auto* pAttrCtrl = GetAttrCtrl();
            auto RchkK = pAttrCtrl->GetRchkByElemK(MembKey);
            if ( !pAttrCtrl->GetRchk(RchkK, rData) )
            {
                ASSERT(0); return rData.COLM;
            }
        }
        break;
    default: ASSERT(0); return rData.COLM;
    }

    return rData.COLM;
}

const T_REBT_D CProductDBDocHelper::GetRebarGeneralColumnDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const
{
    T_REBT_D rData;

    switch ( CurDgnMode )
    {
    case enProductDgnMode::Design:
        {
            ASSERT(0);
        }
        break;
    case enProductDgnMode::Checking:
        {
            auto* pAttrCtrl = GetAttrCtrl();
            T_ELEM_D ElemD;
            if ( !pAttrCtrl->GetElem(MembKey, ElemD) )
            {
                ASSERT(0); return rData;
            }

            T_REBT_D RebtD;
            if ( !pAttrCtrl->GetRebt(ElemD.elpro, rData) )
            {
                ASSERT(0); return rData;
            }
        }
        break;
    default: ASSERT(0); return rData;
    }

    return rData;
}

const T_SBDO_D CProductDBDocHelper::GetRebarPlateDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const
{
    T_SBDO_D rData;

    auto _l_ConvertPlateRebar = [] (double dLengthFactorNU, const CRebarPlate& crERebar, T_SBDO_D& Plate) -> bool
    {
        Plate.strTopRebarName_X = crERebar.Rebar2D.aTopX[0].Rebar.Dia.strName.c_str();
        Plate.dTopRebarSpace_X = crERebar.Rebar2D.aTopX[0].dSpace;
        Plate.strTopRebarName_Y = crERebar.Rebar2D.aTopY[0].Rebar.Dia.strName.c_str();
        Plate.dTopRebarSpace_Y = crERebar.Rebar2D.aTopY[0].dSpace;

        Plate.strBotRebarName_X = crERebar.Rebar2D.aBotX[0].Rebar.Dia.strName.c_str();
        Plate.dBotRebarSpace_X = crERebar.Rebar2D.aBotX[0].dSpace;
        Plate.strBotRebarName_Y = crERebar.Rebar2D.aBotY[0].Rebar.Dia.strName.c_str();
        Plate.dBotRebarSpace_Y = crERebar.Rebar2D.aBotY[0].dSpace;
        
        return true;
    };


    switch ( CurDgnMode )
    {
    case enProductDgnMode::Design:
        {
            std::vector<CDgnRevision> vRebarRev;
            if ( !GetDesignRebarRevList(MembKey, vRebarRev) )
            {
                ASSERT(0); return rData;
            }

            const auto& RebarDataType =
                CDataTypeUtil::GetDPoolType(DataType::SectionRebarPlate);

            Macro::Filter(vRebarRev,
                [&RebarDataType] (const CDgnRevision& RebarRev)
            {
                const auto& CurDataType = RebarRev.iDataType;
                return (CurDataType == RebarDataType);
            });

            if ( vRebarRev.size() != std::size_t(1) )
            {
                ASSERT(0); return rData;
            }

            const auto& RebarRev = vRebarRev.front();
            const auto& CurItrRefer = RebarRev.itrRefer;

            const auto* pDBData = TDBDataFunctor<CSectionRebarPlate>()(CurItrRefer);
            const auto& Rebar = pDBData->GetRebar();

            //const auto& EDgnType = GetEDgnType(MembKey);
            //const auto dLengthFactorNU = GetLengthFactorNU(EDgnType);
            const auto dLengthFactorNU = 1.0;

            _l_ConvertPlateRebar(dLengthFactorNU, Rebar, rData);
        }
        break;
    case enProductDgnMode::Checking:
        {
            auto* pMembCtrl = GetMembCtrl();
            if ( !pMembCtrl->GetSbdoByElem(MembKey, rData) )
            {
                ASSERT(0); return rData;
            }
        }
        break;
    default: ASSERT(0); return rData;
    }

    return rData;
}

const T_RPCM_D CProductDBDocHelper::GetRebarPlateColumnDataCivil(T_KEY MembKey, enProductDgnMode CurDgnMode) const
{
    T_RPCM_D rData;

    switch ( CurDgnMode )
    {
    case enProductDgnMode::Design:
        {
            rData.Initialize();
        }
        break;
    case enProductDgnMode::Checking:
        {
            auto* pMembCtrl = GetMembCtrl();
            auto RipcK = pMembCtrl->GetRipcByElem(MembKey);
            auto* pAttrCtrl = GetAttrCtrl2();
            T_RIPC_D RipcD;
            if ( !pAttrCtrl->GetRipc(RipcK, RipcD) )
            {
                ASSERT(0); return rData;
            }

            return RipcD.MainRebar;
        }
        break;
    default: ASSERT(0); return rData;
    }

    return rData;
}

const std::vector<T_RBSL_D> CProductDBDocHelper::GetRebarShellData(T_KEY MembKey, enProductDgnMode CurDgnMode) const
{
    std::vector<T_RBSL_D> vRbslD;
    vRbslD.clear();
    vRbslD.reserve(4);

    auto _l_ConvertPlateRebar = [] (double dLengthFactorNU, const CRebarPlate& crERebar, std::vector<T_RBSL_D>& aPlate) -> bool
    {
        aPlate.resize(4);
        aPlate[0].strBasicRebarName = crERebar.Rebar2D.aTopX[0].Rebar.Dia.strName.c_str();
        aPlate[0].dBasicRebarSpace = crERebar.Rebar2D.aTopX[0].dSpace * dLengthFactorNU;
        aPlate[0].nLayerType = 0;
        aPlate[0].dCover = crERebar.Rebar2D.aTopX[0].dCenterCover * dLengthFactorNU;

        aPlate[1].strBasicRebarName = crERebar.Rebar2D.aTopY[0].Rebar.Dia.strName.c_str();
        aPlate[1].dBasicRebarSpace = crERebar.Rebar2D.aTopY[0].dSpace * dLengthFactorNU;
        aPlate[1].nLayerType = 1;
        aPlate[1].dCover = crERebar.Rebar2D.aTopY[0].dCenterCover * dLengthFactorNU;

        aPlate[2].strBasicRebarName = crERebar.Rebar2D.aBotX[0].Rebar.Dia.strName.c_str();
        aPlate[2].dBasicRebarSpace = crERebar.Rebar2D.aBotX[0].dSpace * dLengthFactorNU;
        aPlate[2].nLayerType = 2;
        aPlate[2].dCover = crERebar.Rebar2D.aBotX[0].dCenterCover * dLengthFactorNU;

        aPlate[3].strBasicRebarName = crERebar.Rebar2D.aBotY[0].Rebar.Dia.strName.c_str();
        aPlate[3].dBasicRebarSpace = crERebar.Rebar2D.aBotY[0].dSpace * dLengthFactorNU;
        aPlate[3].nLayerType = 3;
        aPlate[3].dCover = crERebar.Rebar2D.aBotY[0].dCenterCover * dLengthFactorNU;

        return true;
    };

    switch ( CurDgnMode )
    {
    case enProductDgnMode::Design:
        {
            std::vector<CDgnRevision> vRebarRev;
            if ( !GetDesignRebarRevListByType(DataType::Elem, MembKey, vRebarRev) )
            {
                ASSERT(0); return std::move(vRbslD);
            }

            const auto& RebarDataType =
                CDataTypeUtil::GetDPoolType(DataType::SectionRebarPlate);

            Macro::Filter(vRebarRev,
                [&RebarDataType] (const CDgnRevision& RebarRev)
            {
                const auto& CurDataType = RebarRev.iDataType;
                return (CurDataType == RebarDataType);
            });

            if ( vRebarRev.size() != std::size_t(1) )
            {
                ASSERT(0); return std::move(vRbslD);
            }

            const auto& RebarRev = vRebarRev.front();
            const auto& CurItrRefer = RebarRev.itrRefer;

            const auto* pDBData = TDBDataFunctor<CSectionRebarPlate>()(CurItrRefer);
            const auto& Rebar = pDBData->GetRebar();

            //const auto& EDgnType = GetEDgnType(MembKey);
            //const auto dLengthFactorNU = GetLengthFactorNU(EDgnType);
            const auto dLengthFactorNU = 1.0;

            _l_ConvertPlateRebar(dLengthFactorNU, Rebar, vRbslD);
        }
        break;
    case enProductDgnMode::Checking:
        {
            auto* pAttrCtrl = GetAttrCtrl();
            for ( int i = 0; i<4; ++i )
            {
                T_RBSL_D RbslD;
                if ( !pAttrCtrl->GetRbslByElemK(MembKey, i, RbslD) )
                {
                    ASSERT(0); return std::move(vRbslD);
                }

                vRbslD.push_back(RbslD);
            }
        }
        break;
    default: ASSERT(0); return std::move(vRbslD);
    }

    return std::move(vRbslD);
}

const T_REBR_D* CProductDBDocHelper::GetRebarBraceData(T_KEY MembKey,
                                                       enProductDgnMode CurDgnMode) const
{
    typedef WrapperEnum TEnum;
    typedef T_KEY       TKey;
    typedef T_REBR_D    TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::RebarBrace);

    if ( pKeyData->GetKey() != MembKey )
    {
        pKeyData->Init();
        pKeyData->Key = MembKey;
        auto& rData = pKeyData->Data;

        switch ( CurDgnMode )
        {
        case enProductDgnMode::Design:
            {
                std::vector<CDgnRevision> vRebarRev;
                if ( !GetDesignRebarRevList(MembKey, vRebarRev) )
                {
                    ASSERT(0); return nullptr;
                }

                const auto& RebarDataType =
                    CDataTypeUtil::GetDPoolType(DataType::MemberRebarColumn);

                Macro::Filter(vRebarRev,
                              [&RebarDataType] (const CDgnRevision& RebarRev)
                {
                    const auto& CurDataType = RebarRev.iDataType;
                    return (CurDataType == RebarDataType);
                });

                if ( vRebarRev.size() != std::size_t(1) )
                {
                    ASSERT(0); return nullptr;
                }

                const auto& RebarRev = vRebarRev.front();
                const auto& CurItrRefer = RebarRev.itrRefer;

                const auto* pDBData = TDBDataFunctor<CMemberRebarColumn>()(CurItrRefer);
                const auto& EndBar  = pDBData->GetRebar(enSector::End);

                rData.MainRebarName = EndBar.aMain[0].Rebar.Dia.strName.c_str();
                if ( EndBar.aMain[0].nCol == 0 )
                {
                    // Circle
                    rData.nQrb = EndBar.aMain[0].nRow;
                }
                else
                {
                    // Rectangle
                    rData.nQrb = EndBar.aMain[0].nRow * 2 + EndBar.aMain[0].nCol * 2 - 4;
                    rData.nRow = EndBar.aMain[0].nRow;
                }

                //const auto& EDgnType = GetEDgnType(MembKey);
                //const auto dLengthFactorNU = GetLengthFactorNU(EDgnType);
                const auto dLengthFactorNU = 1.0;

                rData.dConcCenter = EndBar.aMain[0].dCenterCover * dLengthFactorNU;

                rData.nHoopType = 1; // 1 = Tied  2 = Spiral
                rData.SubRebarName = EndBar.Shear.Rebar.Dia.strName.c_str();
                rData.dSubRebarSpace = EndBar.Shear.Rebar.dSpace * dLengthFactorNU;
                rData.iSubRebarNum[0] = EndBar.Shear.nLegHor;
                rData.iSubRebarNum[1] = EndBar.Shear.nLegVer;
            }
            break;
        case enProductDgnMode::Checking:
            {
		        //중립단위계
		        CCurUnitSaver Save(TRUE);
		        T_UNIT_INDEX ChangeIndex;
		        ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
		        ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
		        GetDBDoc()->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);

                auto* pAttrCtrl = GetAttrCtrl();
                const auto RebarKey = pAttrCtrl->GetRebrByElemK(MembKey);
                if ( !pAttrCtrl->GetRebr(RebarKey, rData) )
                {
                    ASSERT(0); return nullptr;
                }
            }
            break;
        default: ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

const T_REBW_D* CProductDBDocHelper::GetRebarWallData(T_KEY MembKey,
                                                      enProductDgnMode CurDgnMode) const
{
    typedef WrapperEnum TEnum;
    typedef T_KEY       TKey;
    typedef T_REBW_D    TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::RebarWall);

    if ( pKeyData->GetKey() != MembKey )
    {
        pKeyData->Init();
        pKeyData->Key = MembKey;
        auto& rData = pKeyData->Data;

        switch ( CurDgnMode )
        {
        case enProductDgnMode::Design:
            {
                std::vector<CDgnRevision> vRebarRev;
                if ( !GetDesignRebarRevList(MembKey, vRebarRev) )
                {
                    ASSERT(0); return nullptr;
                }

                const auto& RebarDataType =
                    CDataTypeUtil::GetDPoolType(DataType::MemberRebarWall);

                Macro::Filter(vRebarRev,
                              [&RebarDataType] (const CDgnRevision& RebarRev)
                {
                    const auto& CurDataType = RebarRev.iDataType;
                    return (CurDataType == RebarDataType);
                });

                if ( vRebarRev.size() != std::size_t(1) )
                {
                    ASSERT(0); return nullptr;
                }

                const auto& RebarRev = vRebarRev.front();
                const auto& CurItrRefer = RebarRev.itrRefer;

                const auto* pDBData = TDBDataFunctor<CMemberRebarWall>()(CurItrRefer);
                const auto Rebar = pDBData->GetRebar();

                //const auto& EDgnType = GetEDgnType(MembKey);
                //const auto& dLengthFactorNU = GetLengthFactorNU(EDgnType);
                const auto dLengthFactorNU = 1.0;
                const auto& InternalWallID = GetInternalWallID(MembKey);
                const auto* pWallKeyUnion = GetWallKeyUnion(InternalWallID);
                const auto* pWallKeyData = GetWallKeyData(MembKey);
                const auto& WallData = pWallKeyData->GetData();

                rData.VerticalRebarName = Rebar.VerBar.Rebar.Dia.strName.c_str();
                rData.EndRebarName = Rebar.EndBar.Rebar.Dia.strName.c_str();
                rData.HorizonRebarName = Rebar.HorBar.Rebar.Dia.strName.c_str();
                rData.BEHorizonRebarName = Rebar.BEHorBar.Rebar.strDia.c_str();

                rData.dVerticalRebarSpace = Rebar.VerBar.dSpace * dLengthFactorNU;
                rData.dHorizonRebarSpace = Rebar.HorBar.dSpace * dLengthFactorNU;
                rData.dBEHorizonRebarSpace = Rebar.BEHorBar.dSpace * dLengthFactorNU;
                rData.dEndRebarSpace = Rebar.EndBar.dSpace * dLengthFactorNU;

                rData.nNumEndRebar = Rebar.EndBar.unColNum * Rebar.EndBar.unRowNum;

                rData.dDe = Rebar.VerBar.dCenterCover * dLengthFactorNU;
                rData.dDw = Rebar.HorBar.dCenterCover * dLengthFactorNU;

                rData.dThickness = WallData.dThickness;
                rData.dBELength = 0.0; //TODO 이건 뭘까요

                rData.aStorK.Add(pWallKeyUnion->key.storid);
            }
            break;
        case enProductDgnMode::Checking:
            {
			    //중립단위계
			    CCurUnitSaver Save(TRUE);
			    T_UNIT_INDEX ChangeIndex;
			    ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
			    ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
			    GetDBDoc()->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);

                const auto* pWallKeyData = GetWallKeyData(MembKey);
                const auto& WallKey = pWallKeyData->GetKey();

                T_REBW_K RebarKey;
                RebarKey.keymap = WallKey;

                auto* pAttrCtrl = GetAttrCtrl();
                if ( !pAttrCtrl->GetRebw(RebarKey, rData) )
                {
                    ASSERT(0); return nullptr;
                }
            }
            break;
        default: ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

const std::array<enRebar2D, Macro::TNumber<enRebar2D>::Number> caRebarSlabLayerType =
{
    enRebar2D::TopX,
    enRebar2D::BotX,
    enRebar2D::TopY,
    enRebar2D::BotY,
};

const CProductDBDocHelper::T_RBMS_D_List* CProductDBDocHelper::GetRebarMeshedSlabFlexDataList(T_KEY ElemKey,
                                                                                              enProductDgnMode CurDgnMode) const
{
    typedef WrapperEnum   TEnum;
    typedef T_KEY         TKey;
    typedef T_RBMS_D_List TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::RebarMeshedSlab);

    if ( pKeyData->GetKey() != ElemKey )
    {
        pKeyData->Init();
        pKeyData->Key = ElemKey;
        auto& rData = pKeyData->Data;

        switch ( CurDgnMode )
        {
        case enProductDgnMode::Design:
            {
                std::vector<CDgnRevision> vRebarRev;
                if ( !GetDesignRebarRevListByType(DataType::Elem, ElemKey, vRebarRev) )
                {
                    ASSERT(0); return nullptr;
                }

                if ( vRebarRev.empty() )
                {
                    ASSERT(0); return nullptr;
                }

                const auto& type_SRebar2D = CDataTypeUtil::GetDPoolType(DataType::SectionRebar2D);
                const auto& type_SRebar2DAdd = CDataTypeUtil::GetDPoolType(DataType::SectionRebar2DAdd);

                Macro::Filter(vRebarRev, 
                    [&type_SRebar2D, &type_SRebar2DAdd] (const CDgnRevision& RebarRev)
                {
                    const auto& CurDataType = RebarRev.iDataType;
                    return ( CurDataType == type_SRebar2D ||
                             CurDataType == type_SRebar2DAdd );
                });

                if ( !GetRebarSlabDataListByRebarRevList(ElemKey, vRebarRev, rData) )
                {
                    ASSERT(0); return nullptr;
                }
            }
            break;
        case enProductDgnMode::Checking:
            {
                auto* pAttrCtrl = GetAttrCtrl();

                for ( const auto& Dir : caRebarSlabLayerType )
                {
                    const auto nDir = Macro::Ordinal(Dir);
                    if ( !pAttrCtrl->GetRbmsByElemK(ElemKey, nDir, rData[0].aRebar[nDir]) )
                    {
                        ASSERT(0); return nullptr;
                    }
                }
                for (int i = 1; i < 4; ++i)
                {
                    rData[i] = rData[0];
                }
            }
            break;
        default: ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

int CProductDBDocHelper::GetLcomKeyList(int LcomType,
                                        T_LCOM_K_LIST& rData) const
{
    GetAttrCtrl()->GetLcomKeyList(LcomType, rData);
    return static_cast<int>(rData.GetSize());
}

int CProductDBDocHelper::GetDesignRebarRevList(T_KEY MembKey,
                                               std::vector<CDgnRevision>& rvRebarRev) const
{
    return GetIDgnPerform()->GetDesignRebarRevList(MembKey, rvRebarRev);
}

int CProductDBDocHelper::GetDesignRebarRevListByType(DataType Type, 
                                                     T_KEY MembKey, 
                                                     std::vector<CDgnRevision>& rvRebarRev) const
{
    return GetIDgnPerform()->GetDesignRebarRevList(Type, MembKey, rvRebarRev);
}

CDesignResult* CProductDBDocHelper::GetDesignResultPtr() const
{
    return GetPostCtrl()->GetDesignResult();
}

UINT CProductDBDocHelper::GetInternalWallID(T_KEY MembKey) const
{
    UINT nWallID;
    if ( !GetPostCtrl()->GetInternalWallId(MembKey, nWallID) )
    {
        ASSERT(0); return UINT();
    }

    return nWallID;
}

const CProductDBDocHelper::TWallKeyData* CProductDBDocHelper::GetWallKeyData(T_KEY MembKey) const
{
    typedef WrapperEnum  TEnum;
    typedef T_KEY        TKey;
    typedef TWallKeyData TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::WallKeyData);

    if ( pKeyData->GetKey() != MembKey )
    {
        pKeyData->Init();
        pKeyData->Key = MembKey;
        auto& rData = pKeyData->Data;

        if ( !GetPostCtrl()->GetWallDataFromElemK(MembKey, rData.Data, &rData.Key) )
        {
            ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

const T_WALL_K* CProductDBDocHelper::GetWallKeyUnion(UINT InternalWallID) const
{
    typedef WrapperEnum TEnum;
    typedef UINT        TKey;
    typedef T_WALL_K    TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::WallKeyUnion);

    if ( pKeyData->GetKey() != InternalWallID )
    {
        pKeyData->Init();
        pKeyData->Key = InternalWallID;
        auto& rData = pKeyData->Data;

        if ( !GetPostCtrl()->GetWallKey(InternalWallID, rData.keymap) )
        {
            ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

const T_STOR_D* CProductDBDocHelper::GetStoryData(T_STOR_K StoryKey) const
{
    typedef WrapperEnum TEnum;
    typedef T_STOR_K    TKey;
    typedef T_STOR_D    TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::Story);

    if ( pKeyData->GetKey() != StoryKey )
    {
        pKeyData->Init();
        pKeyData->Key = StoryKey;
        auto& rData = pKeyData->Data;

        if ( !GetAttrCtrl()->GetStor(StoryKey, rData) )
        {
            ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

const T_WMAK_D* CProductDBDocHelper::GetWallMarkData(T_WALL_KEY WallKey) const
{
    typedef WrapperEnum TEnum;
    typedef T_KEY       TKey;
    typedef T_WMAK_D    TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::WallMark);

    if ( pKeyData->GetKey() != WallKey )
    {
        pKeyData->Init();
        pKeyData->Key = WallKey;
        auto& rData = pKeyData->Data;

        if ( !GetDgnDataCtrl()->Get_DgnWalWmak(WallKey, rData) )
        {
            ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

T_KEY CProductDBDocHelper::GetRepMembKeyInSbdoMember(T_KEY CurElemKey) const
{
    typedef std::map<T_KEY, T_KEY> mapKeyPair;
    typedef WrapperEnum TEnum;
    typedef T_KEY       TKey;
    typedef mapKeyPair  TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::RepMembKeyOnSbdoMember);

    auto& rmElemKey_RepMembKey = pKeyData->Data;

    if ( rmElemKey_RepMembKey.empty() )
    {
        rmElemKey_RepMembKey.clear();

        auto* const pAttrCtrl = GetAttrCtrl();
        std::vector<T_SBDO_K> vSbdoKey;
        pAttrCtrl->GetSbdoKeyList(vSbdoKey);
        if ( vSbdoKey.empty() )
        {
            ASSERT(0); return T_KEY();
        }

        for ( const auto& SbdoKey : vSbdoKey )
        {
            CArray<T_ELEM_K, T_ELEM_K> aElemKey;
            int nSize = pAttrCtrl->GetElemListSbdoK(SbdoKey, aElemKey);
            if(nSize==0) continue;
            const auto& SlabMembKey = aElemKey[0];
            for ( int nIdx = 0; nIdx < nSize; ++nIdx )
            {
                const auto& ElemKey = aElemKey[nIdx];
                rmElemKey_RepMembKey[ElemKey] = SlabMembKey;
            }
        }
    }

    const auto& itr = rmElemKey_RepMembKey.find(CurElemKey);
    if ( itr == rmElemKey_RepMembKey.end() )
    {
        ASSERT(0); return T_KEY();
    }

    return itr->second;
}


double CProductDBDocHelper::GetLengthFactorNU(enDgn EDgnType) const
{
    const auto* pUnitType = GetUnitType(EDgnType);

    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);
    const auto nForce  = CDgnUnitUtil::GetFUnitForce(*pUnitType);

    const auto& ForceIdx    = D_UNITSYS_FORCE_INDEX_N;
    const auto& LengthIdx   = D_UNITSYS_LENGTH_INDEX_M;
    const auto& USystemType = D_UNITSYS_BASE_LENGTH;

    return GetUnitCtrl()->ConvertSrcUnitData2TgtUnit(nForce, nLength,
                                                     ForceIdx, LengthIdx, USystemType, 1.0);
}

double CProductDBDocHelper::GetAreaFactorNU(enDgn EDgnType) const
{
    const auto* pUnitType = GetUnitType(EDgnType);

    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);
    const auto nForce  = CDgnUnitUtil::GetFUnitForce(*pUnitType);

    const auto& ForceIdx    = D_UNITSYS_FORCE_INDEX_N;
    const auto& LengthIdx   = D_UNITSYS_LENGTH_INDEX_M;
    const auto& USystemType = D_UNITSYS_BASE_AREA;

    return GetUnitCtrl()->ConvertSrcUnitData2TgtUnit(nForce, nLength,
        ForceIdx, LengthIdx, USystemType, 1.0);
}

double CProductDBDocHelper::GetForceFactorNU(enDgn EDgnType) const
{
    const auto* pUnitType = GetUnitType(EDgnType);

    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);
    const auto nForce = CDgnUnitUtil::GetFUnitForce(*pUnitType);

    const auto& ForceIdx = D_UNITSYS_FORCE_INDEX_N;
    const auto& LengthIdx = D_UNITSYS_LENGTH_INDEX_M;
    const auto& USystemType = D_UNITSYS_BASE_FORCE;

    return GetUnitCtrl()->ConvertSrcUnitData2TgtUnit(nForce, nLength,
        ForceIdx, LengthIdx, USystemType, 1.0);
}

double CProductDBDocHelper::GetStressFactorNU(enDgn EDgnType) const
{
    const auto* pUnitType = GetUnitType(EDgnType);

    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);
    const auto nForce = CDgnUnitUtil::GetFUnitForce(*pUnitType);

    const auto& ForceIdx = D_UNITSYS_FORCE_INDEX_N;
    const auto& LengthIdx = D_UNITSYS_LENGTH_INDEX_M;
    const auto& USystemType = D_UNITSYS_BASE_STRESS;

    return GetUnitCtrl()->ConvertSrcUnitData2TgtUnit(nForce, nLength,
        ForceIdx, LengthIdx, USystemType, 1.0);
}

double CProductDBDocHelper::GetUnitMomentFactorNU(enDgn EDgnType) const
{
    const auto* pUnitType = GetUnitType(EDgnType);

    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);
    const auto nForce  = CDgnUnitUtil::GetFUnitForce(*pUnitType);

    const auto& ForceIdx    = D_UNITSYS_FORCE_INDEX_N;
    const auto& LengthIdx   = D_UNITSYS_LENGTH_INDEX_M;
    const auto& USystemType = D_UNITSYS_BASE_UNITMOMENT;

    return GetUnitCtrl()->ConvertSrcUnitData2TgtUnit(nForce, nLength,
                                                     ForceIdx, LengthIdx, USystemType, 1.0);
}

double CProductDBDocHelper::GetUnitForceFactorNU(enDgn EDgnType) const
{
    const auto* pUnitType = GetUnitType(EDgnType);

    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);
    const auto nForce  = CDgnUnitUtil::GetFUnitForce(*pUnitType);

    const auto& ForceIdx    = D_UNITSYS_FORCE_INDEX_N;
    const auto& LengthIdx   = D_UNITSYS_LENGTH_INDEX_M;
    const auto& USystemType = D_UNITSYS_BASE_UNITFORCE;

    return GetUnitCtrl()->ConvertSrcUnitData2TgtUnit(nForce, nLength,
                                                     ForceIdx, LengthIdx, USystemType, 1.0);
}

double CProductDBDocHelper::GetUnitWidth(enDgn EDgnType) const
{
    const auto* pUnitType = GetUnitType(EDgnType);
    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);

    switch ( nLength )
    {
    case D_UNITSYS_LENGTH_INDEX_MM:
        return 1000.0;
	case D_UNITSYS_LENGTH_INDEX_CM:
	    return 100.0;
    case D_UNITSYS_LENGTH_INDEX_IN:
        return 12.0;
    default:
        return 1.0;
    }
}

int CProductDBDocHelper::GetLengthUnitType(enDgn EDgnType) const
{
    const auto* pUnitType = GetUnitType(EDgnType);
    return CDgnUnitUtil::GetFUnitLength(*pUnitType);
}

CResultUnitOut CProductDBDocHelper::CreateResultUnitOut(enDgn EDgnType,
                                                        enDgnResultItem EItem,
                                                        const TResultVMemb& crData) const
{
    const auto& aResult = crData.aResult;
    const auto& nSize = aResult.GetSize();
    std::vector<double> vd(nSize);
    for ( int nIdx = 0; nIdx < nSize; ++nIdx )
    {
        vd[nIdx] = aResult[nIdx];
    }

    const auto* pUnitType = GetUnitType(EDgnType);

    const auto nLength = CDgnUnitUtil::GetFUnitLength(*pUnitType);
    const auto nForce  = CDgnUnitUtil::GetFUnitForce(*pUnitType);

    return CResultUnitOut(GetIDgnDoc(), nForce, nLength, EItem, vd);
}

bool CProductDBDocHelper::ConvertBeamRebar(double dLengthFactorNU,
                                           const CRebarBeam& crERebar,
                                           T_REBB_BASE& rTop,
                                           T_REBB_BASE& rBot) const
{
    if ( !crERebar.IsValidData() )
    {
        ASSERT(0); return false;
    }

    rTop.RebarName = crERebar.aTop[0].Rebar.Dia.strName.c_str();
    rTop.nRebar1 = crERebar.aTop[0].nNum;
    rBot.RebarName = crERebar.aBot[0].Rebar.Dia.strName.c_str();
    rBot.nRebar1 = crERebar.aBot[0].nNum;
    rTop.iSubRebarNum = crERebar.Shear.nLeg;
    rTop.dSubRebarSpace = crERebar.Shear.Rebar.dSpace * dLengthFactorNU;

    if ( crERebar.aTop.size() == Macro::Ordinal(enProductRebarLayer::Double) )
    {
        rTop.nArrayType = crERebar.aTop.size();
        rTop.nRebar2 = crERebar.aTop[1].nNum;
    }

    if ( crERebar.aBot.size() == Macro::Ordinal(enProductRebarLayer::Double) )
    {
        rBot.nArrayType = crERebar.aBot.size();
        rBot.nRebar2 = crERebar.aBot[1].nNum;
    }

    rTop.nSkin = crERebar.Skin.Data.nNum;

    return true;
}

bool CProductDBDocHelper::GetRebarSlabDataListByRebarRevList(T_KEY ElemKey,
                                                             const std::vector<CDgnRevision>& vRebarRev,
                                                             T_RBMS_D_List& rData) const
{
    for ( const auto& RebarRev : vRebarRev )
    {
        const auto& CurDataType = RebarRev.iDataType;
        const auto& CurItrRefer = RebarRev.itrRefer;

        const auto& type_SRebar2D =
            CDataTypeUtil::GetDPoolType(DataType::SectionRebar2D);
        if ( CurDataType == type_SRebar2D )
        {
            const auto* pDBData = TDBDataFunctor<CSectionRebar2D>()(CurItrRefer);

            for ( const auto& Type : caRebarSlabLayerType )
            {
                const auto nType = Macro::Ordinal(Type);

                vMain2D vMainRebar;
                if ( !pDBData->GetRebar().GetMainRebarList(Type, vMainRebar) )
                {
                    ASSERT(0); return false;
                }

                //const auto& EDgnType = GetEDgnType(ElemKey);
                //const auto dLengthFactorNU = GetLengthFactorNU(EDgnType);
                const auto dLengthFactorNU = 1.0;
                for (int j = 0; j < vMainRebar.size(); ++j)
                {
                    const auto& RepRebar = vMainRebar[j];
                    rData[j].aRebar[nType].strBasicRebarName = RepRebar.Rebar.Dia.strName.c_str();
                    rData[j].aRebar[nType].dBasicRebarSpace = RepRebar.dSpace * dLengthFactorNU;
                    rData[j].aRebar[nType].dCover = RepRebar.dCenterCover * dLengthFactorNU;
                    rData[j].aRebar[nType].nLayerType = nType;
                }
            }

            continue;
        }

        const auto& type_SRebar2DAdd =
            CDataTypeUtil::GetDPoolType(DataType::SectionRebar2DAdd);
        if ( CurDataType == type_SRebar2DAdd )
        {
            const auto* pDBData = TDBDataFunctor<CSectionRebar2DAdd>()(CurItrRefer);

            for ( const auto& Type : caRebarSlabLayerType )
            {
                const auto nType = Macro::Ordinal(Type);

                vMain2D vMainRebar;
                if ( !pDBData->GetRebar().GetMainRebarList(Type, vMainRebar) )
                {
                    ASSERT(0); return false;
                }

                if ( !vMainRebar.size() )
                {
                    ASSERT(0); return false;
                }

                //const auto& EDgnType = GetEDgnType(ElemKey);
                //const auto dLengthFactorNU = GetLengthFactorNU(EDgnType);
                const auto dLengthFactorNU = 1.0;

                for (int j = 0; j < vMainRebar.size(); ++j)
                {
                    const auto& AddRebar = vMainRebar[j];

                    if (AddRebar.Rebar.Dia.strName.IsEmpty()) continue;

                    rData[j].aRebar[nType].strAdd1RebarName = AddRebar.Rebar.Dia.strName.c_str();
                    rData[j].aRebar[nType].dAdd1RebarSpace = AddRebar.dSpace * dLengthFactorNU;
                    rData[j].aRebar[nType].strAdd2RebarName = AddRebar.Rebar.Sub.strName.c_str();
                    rData[j].aRebar[nType].dAdd2RebarSpace = AddRebar.dSpace * dLengthFactorNU;
                    rData[j].aRebar[nType].dCover = AddRebar.dCenterCover * dLengthFactorNU;
                    rData[j].aRebar[nType].nLayerType = nType;
                }
            }

            continue;
        }

        ASSERT(0);
        return false;
    }

    return true;
}

enDgn CProductDBDocHelper::GetEDgnType(T_KEY MembKey) const
{
    const auto& PDgnType = GetPDgnType(MembKey);
    
    if ( PDgnType == _T("C") )
    {
        return enDgn::RCS;
    }

    if ( PDgnType == _T("S") )
    {
        return enDgn::STL;
    }

    ASSERT(0);
    return enDgn();
}

CString CProductDBDocHelper::GetPDgnType(T_KEY MembKey) const
{
    const auto* pMatdAttr = GetMatdData(MembKey);
    return pMatdAttr->Type;
}

const T_MATD_D* CProductDBDocHelper::GetMatdData(T_KEY MembKey) const
{
    //이 함수 IProductDataBaseHelper에도 중복으로 있는데,
    //DataBase 내의 MemberCtrl 함수로 추가적인 확인 하는 함수 인듯ㅠㅠ
    //DgnType 꺼내오는 용도로는 큰 문제 없으니 여기서만 사용함
    //private는 풀지 말 것.
    typedef WrapperEnum TEnum;
    typedef T_KEY       TKey;
    typedef T_MATD_D    TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::Material_AttrOnly);

    if ( pKeyData->GetKey() != MembKey )
    {
        pKeyData->Init();
        pKeyData->Key = MembKey;
        auto& rData = pKeyData->Data;

        if ( !GetAttrCtrl()->GetDgnGenMatd(MembKey, rData) )
        {
            ASSERT(0); return nullptr;
        }
    }

    return &pKeyData->GetData();
}

const enUnitType* CProductDBDocHelper::GetUnitType(enDgn EDgnType) const
{
    typedef WrapperEnum TEnum;
    typedef enDgn       TKey;
    typedef enUnitType  TData;

    auto* pKeyData =
        CastTKeyDataByWrapper<TEnum, TKey, TData>(GetWrapper(), TEnum::CodeUnitType);

    if ( pKeyData->GetKey() != EDgnType )
    {
        pKeyData->Init();
        pKeyData->Key = EDgnType;
        auto& rData = pKeyData->Data;

        rData = GetIDgnPerform()->GetCodeUnit(EDgnType);
    }

    return &pKeyData->GetData();
}

CProductDBDocHelperWrapper* CProductDBDocHelper::GetWrapper() const
{
    return m_pWrapper;
}
UINT CProductDBDocHelper::GetLowerColmNo(UINT NodeK) const
{
    UINT ElemK = 0;

    CArray<T_ELEM_K, T_ELEM_K> arElemList;
    GetAttrCtrl()->GetConnectedElem(NodeK, arElemList);  // 절점에 연결된 element의 key를 가져온다.

    T_NODE_D JointNode;
    GetAttrCtrl()->GetNode(NodeK, JointNode);

    int nElem = arElemList.GetSize();
    for (int IEL = 0; IEL < nElem; IEL++)
    {
        T_ELEM_K KeyElem = arElemList.GetAt(IEL);
        int iCBM = GetAttrCtrl()->GetMemberType(KeyElem);
        if (iCBM == D_MBTP_COLUMN || iCBM == D_MBTP_BRACE) // Seungjun-20140218 : 펀칭 검토시 truss도 포함
        {
            T_ELEM_D elemD; elemD.Initialize();
            GetAttrCtrl()->GetElem(KeyElem, elemD);

            T_NODE_D OppsiNode; OppsiNode.Initialize();
            if (elemD.elnod[0] == NodeK)
            {
                GetAttrCtrl()->GetNode(elemD.elnod[1], OppsiNode);
            }
            else
            {
                GetAttrCtrl()->GetNode(elemD.elnod[0], OppsiNode);
            }

            if (JointNode.z > OppsiNode.z)
            {
                ElemK = arElemList[IEL];
                if (iCBM == D_MBTP_COLUMN || IEL == (nElem - 1)) // brace보다 column 우선
                {
                    break;
                }
            }
        }
    }

    return ElemK;
}

UINT CProductDBDocHelper::GetElemNodeNum(UINT ElemK) const
{
    T_ELEM_D ElemD;
    if ( !GetAttrCtrl()->GetElem(ElemK, ElemD) )
    {
        ASSERT(0); return 0U;
    }
    for ( int nIdx = 0; nIdx < D_ELEM_MAXNOD; ++nIdx )
    {
        const auto& PL_ENodeKey = ElemD.elnod[nIdx];
        if ( PL_ENodeKey == 0 )
        {
            return nIdx;
        }
    }

    ASSERT(0);
    return 0U;
}

UINT CProductDBDocHelper::GetUpperColmNo(UINT NodeK) const
{
    UINT ElemK = 0;

    CArray<T_ELEM_K, T_ELEM_K> arElemList;
    GetAttrCtrl()->GetConnectedElem(NodeK, arElemList);  // 절점에 연결된 element의 key를 가져온다.

    T_NODE_D JointNode;
    GetAttrCtrl()->GetNode(NodeK, JointNode);

    int nElem = arElemList.GetSize();
    for (int IEL = 0; IEL < nElem; IEL++)
    {
        T_ELEM_K KeyElem = arElemList.GetAt(IEL);
        int iCBM = GetAttrCtrl()->GetMemberType(KeyElem);
        if (iCBM == D_MBTP_COLUMN || iCBM == D_MBTP_BRACE) // Seungjun-20140218 : 펀칭 검토시 truss도 포함
        {
            T_ELEM_D elemD; elemD.Initialize();
            GetAttrCtrl()->GetElem(KeyElem, elemD);

            T_NODE_D OppsiNode; OppsiNode.Initialize();
            if (elemD.elnod[0] == NodeK)
            {
                GetAttrCtrl()->GetNode(elemD.elnod[1], OppsiNode);
            }
            else
            {
                GetAttrCtrl()->GetNode(elemD.elnod[0], OppsiNode);
            }

            if (JointNode.z < OppsiNode.z)
            {
                ElemK = arElemList[IEL];
                if (iCBM == D_MBTP_COLUMN || IEL == (nElem - 1)) // brace보다 column 우선
                    break;
            }
        }
    }

    return ElemK;
}

bool CProductDBDocHelper::GetUserWallThick(const UINT& StoryID, const UINT& WallID, double& dThick) const
{
	T_REBW_K RebwK;
	RebwK.key.storid = StoryID;
	RebwK.key.wallid = WallID;
	T_REBW_D RebwD;
	if(GetAttrCtrl()->GetRebw(RebwK, RebwD))
	{
        if(!RebwD.bUseModelThik)
        {
            dThick = RebwD.dThickness;
            return true;
        }
	}

    return false;
}