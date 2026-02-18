// DgnPscDataCtrl.cpp: implementation of the CDgnPscDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "..\..\dgnengine\idesign\DGN_link\IDgnDocCtrl.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnDoc.h"
#include "..\..\dgnengine\idesign\DGN_link\CalcType.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnQuery.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnMemoizationLib.h"

#include "..\..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnReportInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\IRptMembInfo.h"
#include "..\..\dgnengine\idesign\DGN_def\ReportCritDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnRptCriticalInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DesignCaseDef.h"
#include "..\..\dgnengine\idesign\DGN_def\UnbracedLengthDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnItemCompoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionClassificationEnumDef.h"
#include "..\..\dgnengine\idesign\DGN_def\LongitudinalStiffenerDef.h"
#include "..\..\dgnengine\idesign\DGN_def\IStiffenerSingle.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffenerRectangle.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffenerT.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffenerURib.h"
#include "..\..\dgnengine\idesign\DGN_def\RptMembInfoDef.h"

#include "..\..\dgnengine\idesign\DGN_lib\IAlignedShapeTypical.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeRectangle.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeT.h"  
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeURib.h"  


#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"

#include "DgnCsgDataCtrl.h"
#include "CRCForceCtrl.h"
#include "Dgn_CSGManager.h"
#include "Dgn_CSGManager_AASHTO12.h"
#include "Dgn_CSGManager_CSA_S6_14.h"
#include "Dgn_CSGManager_KSCE_LSD15.h"
#include "Dgn_CSGManager_KSSC_2014.h"
#include "Dgn_CSGManager_CS457_R1.h"
#include "Dgn_CSGManager_NR_GN_CIV_025_06.h"
#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"

//#include "DgnDraw_SectViewPannel.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\PlateGirderDesign.h"
//#include "..\wg_db\wg_db_SectUtil.h"
//#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\wg_db_SectUtil.h"
//#include "..\wg_db\DgnDrawBase_DrawView_Impl.h"
//#include "..\wg_base\product.h"
//#include "..\wg_db\DB_ST_DT_SECT.h"

#include "..\wg_base\wg_base_UtilFunc.h"
#include "..\wg_base\wg_base_ArrayUtil.h"

#include "Dgn_DllManager_Base.h"
#include "..\dgnengine\idesign\DGN_link\ITmpDgnDrawLib.h"
#include "..\dgnengine\idesign\DGN_lib\DgnThreadPool.h"

#include <mutex>

#include "DgnCsgDataBase.h"
#include "NationalAnnexTool.h"
#include "DgnProgressDlg.h"
#include "DgnForceStressST.h"

using namespace std;
using namespace dgn::lib;
using namespace dgn::def;
using namespace dgn::link;
using namespace dgn::item;

std::mutex g_csg_mutex;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnCsgDataCtrl::CDgnCsgDataCtrl() : CDgnCsgDataBase()
{ 
    T_CPGD_D CpgdD; CpgdD.Initialize();  
    m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);
    T_RPAS_D RpasD;
    if (!m_pDoc->m_pAttrCtrl2->GetRpas(RpasD)) RpasD.Initialize();
	if ( CDBLib::IsCSGCodeLSD_KR(m_iCivilCode) && CpgdD.nConsiderPv==1 )
    { // LSD 기준에서 곡선교 일 때
        m_nDgnCode = KSSC_2014_CSG;
    }
    else if(m_iCivilCode==STL_KSCE_LSD15 && RpasD.nBridgeType==1)
    {
        m_nDgnCode = KSSC_2014_CSG;
    }
    else
    {
        m_nDgnCode = m_iCivilCode;
    }

    switch (m_nDgnCode)
    {
    case AASHTO_LRFD20_CSG:
    case AASHTO_LRFD17_CSG:
    case AASHTO_LRFD16_CSG:
    case AASHTO_LRFD12_CSG:
    case STL_AASHTO_LRFD12: // CSG Composite Rating
    case STL_AASHTO_LRFD19:
        {
            m_pCsgManager = new CDgn_CSGManager_AASHTO12;
        }
        break;
    case AASHTO_LRFD07_CSG:
        {
            m_pCsgManager = new CDgn_CSGManager_AASHTO07;
        }
        break;
	case KDS_24_14_31_2018_CSG:
	case KSSC_2014_CSG:
        {
            m_pCsgManager = new CDgn_CSGManager_KSSC_2014;
        }
        break;
    case KSCE_LSD15_CSG:
    case STL_KSCE_LSD15:
        {
            m_pCsgManager = new CDgn_CSGManager_KSCE_LSD15;
        }
        break;
    case CSA_S6_19_CSG:
    case CSA_S6_14_CSG:
    case CSA_S6_10_CSG:
        {
            m_pCsgManager = new CDgn_CSGManager_CSA_S6_14;
        }
        break;
    case CS457_R1_CSG:
    case STL_CS454_20:
        {
            m_pCsgManager = new CDgn_CSGManager_CS457_R1;
        }
        break;
    case NR_GN_CIV_025_06_CSG:
    case STL_NR_GN_CIV_025_06:
    {
        m_pCsgManager = new CDgn_CSGManager_NR_GN_CIV_025_06;
    }
    break;
    default:
        m_pCsgManager = new CDgn_CSGManager;
        break;
    }


    m_mElemSpanK.clear();
    m_mElemSubSpanK.clear();
    m_mElemSpanD.clear();
    m_mElemSpanLength.clear();
    m_mCsgMatl.RemoveAll();
    m_mCsgSect.clear();
    m_mCsgTrst.clear();
    m_mInteriorSupport.clear();

    m_nChkLcomSizeUlmf = 5;

	m_nDgnLcomNo = 0;
}

CDgnCsgDataCtrl::~CDgnCsgDataCtrl()
{
    if(m_pCsgManager!=NULL) 
    {
        delete m_pCsgManager;
        m_pCsgManager = NULL;
    }
}

void CDgnCsgDataCtrl::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
{
    CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

    T_CPGD_D CpgdD; CpgdD.Initialize();  
    m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;
	switch ( m_nDgnCode )
    {
    case KDS_24_14_31_2018_CSG:
    case KSCE_LSD15_CSG:
    case KSSC_2014_CSG:
    case KSCE_ASD10_CSG:
    case KSCE_RAIL_ASD11_CSG:
    case CSA_S6_10_CSG:
    case CSA_S6_14_CSG:
    case CSA_S6_19_CSG:
    case CS457_R1_CSG:
	case AS5100_6_17_CSG:
        CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
        CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
        m_pDataCtrl->Set_UnitIndex(CodeUnit);
        break;
    case AASHTO_LRFD20_CSG:
    case AASHTO_LRFD17_CSG:
    case AASHTO_LRFD16_CSG:
    case AASHTO_LRFD12_CSG:
    case AASHTO_LRFD07_CSG:
        {
            int iRptSelUnit = CDBLib::GetReportSelUnit();
            if(iRptSelUnit == CDBLib::EN_REPORT_UNIT_US)
            {
                CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
                CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KIP;
            }
            else
            {
                CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
                CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
            }
            m_pDataCtrl->Set_UnitIndex(CodeUnit);
            break;
        }
    default: ASSERT(0); break;
    }

    InitialData(m_nDgnCode);

}

BOOL CDgnCsgDataCtrl::InitialData(int nDgnCode)
{
    if(!m_pCsgManager->InitialData(DGNE_CODETYPE_CSG, nDgnCode)) return FALSE;

    //   T_PSCD_D PscdD; PscdD.Initialize();
    //   m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
    //   
    //   ConvertPscParamData(PscdD, m_SPscdD);

    return TRUE;
}

void CDgnCsgDataCtrl::SetCsgDgnCode(int nDgnCode)
{
    m_nDgnCode = nDgnCode;
}

void CDgnCsgDataCtrl::SetChkCsgDgnElem(ArrElemPairKey& raElemK)
{
    if (raElemK.GetSize()>0) m_arElemK.Copy(raElemK);
}

BOOL CDgnCsgDataCtrl::Make_CSGDgnData()
{
    int nReturn=0;	

    // Set arElemK	
    m_arElemK.RemoveAll();
    ArrElemPairKey arElemK;
    if(m_pDataCtrl->Get_ElemListForPlateGirder(TRUE, arElemK))
    {
        int nElemSize = arElemK.GetSize();
        for(int i=0; i<nElemSize; ++i)
        {
            auto ElemK = arElemK.GetAt(i);
			BOOL bSaveElemK = FALSE;
            if (ElemK.second == EN_EL_BEAM)
            {
                bSaveElemK = m_pDoc->m_pAttrCtrl->ExistCgdp(ElemK.first);
            }
            else if (ElemK.second == EN_EL_VBEAM)
            {
                bSaveElemK = m_pDoc->m_pAttrCtrl->ExistCgdv(ElemK.first);
            }
            else ASSERT(0);
           
            // Save ElemK.
            if(bSaveElemK)	m_arElemK.Add(ElemK);
        }
        if(m_arElemK.GetSize()==0)  m_arElemK.Copy(arElemK);
    }

    Make_ElemSpanMapKey();
    Make_ElemSpanMapKey_Sub();
    Make_ElemSpanData();


    if(nReturn==0)  nReturn = (Make_CsgMatlDgnData() ? 0 : 1);  
    if(nReturn==0)  nReturn = (Make_CsgSectDgnData() ? 0 : 2);
    if(nReturn==0)  nReturn = (Make_CpgRbarElem()    ? 0 : 3);	
    if(nReturn==0)  nReturn = (Make_CpgSconElem()    ? 0 : 4);//Shear Connector
    if(nReturn==0)  nReturn = (Make_CpgLstiElem()    ? 0 : 5);//Longitudinal Stiffner
    if(nReturn==0)  nReturn = (Make_CsgTrstDgnData() ? 0 : 6);//Transverse Stiffner
    if(nReturn==0)  nReturn = (Make_CpgCpfpElem()    ? 0 : 7);//Fatigue Param
    if(nReturn==0)  nReturn = (Make_CpgCpcbElem()    ? 0 : 8);//Curved Bridge
    //if(nReturn==0)  nReturn = (Make_CpgTsesElem()    ? 0 : 7);
    //if(nReturn==0)  nReturn = (Make_CpgBcdtElem()    ? 0 : 8); // Buckling coefficients for different types of load application.
    //if(nReturn==0)  nReturn = (Make_CpgLtbdElem()    ? 0 : 9); // Lateral Torsional Buckling Data
    //if(nReturn==0)  nReturn = (Make_CpgFrdtElem()    ? 0 : 10); // Fatigue Data
    //if(nReturn==0)  nReturn = (Make_CpgMembElem()    ? 0 : 11); // Member Data
    if(nReturn==0)  nReturn = (Make_CpgDohlElem()    ? 0 : 12); // Deck Overhang Loads

    if(nReturn!=0)  return FALSE;

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CSGDgnData4Rating()
{
    int nReturn=0;	
    if(nReturn==0)  nReturn = (Make_CsgMatlDgnData() ? 0 : 1);  
    if(nReturn==0)  nReturn = (Make_CsgSectDgnData() ? 0 : 2);
    if(nReturn==0)  nReturn = (Make_CpgRbarElem()    ? 0 : 3);	
    if(nReturn==0)  nReturn = (Make_CpgSconElem()    ? 0 : 4);//Shear Connector
    if(nReturn==0)  nReturn = (Make_CpgLstiElem()    ? 0 : 5);//Longitudinal Stiffener
    if(nReturn==0)  nReturn = (Make_CsgTrstDgnData() ? 0 : 6);//Transverse Stiffener
    if(nReturn==0)  nReturn = (Make_CpgCpfpElem()    ? 0 : 7);//Fatigue Param
    if(nReturn==0)  nReturn = (Make_CpgCpcbElem()    ? 0 : 8);//Curved Bridge
    if(nReturn==0)  nReturn = (Make_CpgEbslElem()    ? 0 : 9);//Effective breadth by shear lag
    if(nReturn==0)  nReturn = (Make_CpgLtbdElem()    ? 0 : 10); // Lateral Torsional Buckling Data
    if(nReturn==0)  nReturn = (Make_CpgDwspElem()    ? 0 : 11); // Distorsional warping stress
    if(nReturn==0)  nReturn = (Make_CsgUfraElem()    ? 0 : 12); // U-frame
    if(nReturn==0)  nReturn = (Make_CsgMdfsElem()    ? 0 : 13); // Moment Distribution Factor for Slenderness
    if(nReturn==0)  nReturn = (Make_CsgLshrElem()    ? 0 : 14); // 
    if(nReturn==0)  nReturn = (Make_CsgCrdpData()    ? 0 : 15); // Corrosion Depth 

    if(nReturn!=0)  return FALSE;

    return TRUE;
}

void CDgnCsgDataCtrl::SetMemberResData(CSG_MEMB_RES_KSCE_LSD12_D &rData)
{
    m_pCsgManager->Set_MemberResKSCE_LSD12(rData);
}

BOOL CDgnCsgDataCtrl::MakeMemberResData(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RES_KSCE_LSD12_D &rData)
{
    if (!m_pCsgManager->Check_MemberResKSCE_LSD12(CsgMembD, rData)) return FALSE;
    return TRUE;
}

void CDgnCsgDataCtrl::SetMemberResDataKSSC2014(CSG_MEMB_RES_AASHTO_LRFD_D &rData)
{
    m_pCsgManager->Set_MemberResKSSC_LSD2014(rData);
}

BOOL CDgnCsgDataCtrl::MakeMemberResDataKSSC2014(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData)
{
    if (!m_pCsgManager->Check_MemberResKSSC_LSD2014(CsgMembD, rData)) return FALSE;
    return TRUE;
}

void CDgnCsgDataCtrl::SetMemberResDataAASHTO(CSG_MEMB_RES_AASHTO_LRFD_D &rData)
{
    m_pCsgManager->Set_MemberResAASHTO(rData);
}

BOOL CDgnCsgDataCtrl::MakeMemberResDataAASHTO(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData)
{
    if (!m_pCsgManager->Check_MemberResAASHTO(CsgMembD, rData)) return FALSE;
    return TRUE;
}

void CDgnCsgDataCtrl::SetMemberResultData(CSG_MEMB_RESULT_D &rData)
{
    m_pCsgManager->Set_MemberResult(rData);
}

BOOL CDgnCsgDataCtrl::MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData)
{
    ASSERT(0);
    return TRUE;
}


BOOL CDgnCsgDataCtrl::Make_ElemSpanMapKey()
{
    m_mElemSpanK.clear();

    CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
    m_pDoc->m_pAttrCtrl->GetSpanKeyList(aSpanKeyList);

    // Span key로 돌리면서 요소를 map에 setting...
    int nSpanKeySize = aSpanKeyList.GetSize();  
    T_SPAN_D SpanD; 
    for(int i=0; i<nSpanKeySize; ++i)
    {    
        SpanD.Initialize();
        T_SPAN_K SpanK = aSpanKeyList[i]; 
        m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);
        int nSpanElemNum = SpanD.aElemBase.GetSize();
        UINT iL_count = 1;
        CArray<double,double> aElemSpan;

        for(int j=0; j<nSpanElemNum; ++j)
        {    
            const T_SPAN_BASE &SpanBase = SpanD.aElemBase[j];
            T_ELEM_K ElemK2 = SpanBase.ElemK;
            ElemPairK EPairK2(ElemK2, EN_EL_BEAM);
			auto iter = m_mElemSpanK.find(EPairK2);
            if (iter == m_mElemSpanK.end())
            {
                m_mElemSpanK.insert({ EPairK2, SpanK });
            }
        }
    }

    CArray<T_SPAV_K, T_SPAV_K> aSpavKeyList;
    m_pDoc->m_pAttrCtrl->GetSpavKeyList(aSpavKeyList);

    T_SPAV_D SpavD;
    for (int i=0; i<aSpavKeyList.GetSize(); ++i)
    {
        SpavD.Initialize();
        T_SPAV_K SpavK = aSpavKeyList[i];
        m_pDoc->m_pAttrCtrl->GetSpav(SpavK, SpavD);
        int nSpanElemNum = SpavD.aElemBase.GetSize();

        for (int j = 0; j < nSpanElemNum; ++j)
        {
            const T_SPAN_BASE& SpanBase = SpavD.aElemBase[j];
            T_ELEM_K ElemK2 = SpanBase.ElemK;
            ElemPairK EPairK2(ElemK2, EN_EL_VBEAM);
            auto iter = m_mElemSpanK.find(EPairK2);
            if (iter == m_mElemSpanK.end())
            {
                m_mElemSpanK.insert({ EPairK2, SpavK });
            }
        }
    }

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_ElemSpanMapKey_Sub()
{
    m_mElemSubSpanK.clear();

    CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
    m_pDoc->m_pAttrCtrl->GetSpanKeyList(aSpanKeyList);

    // Span key로 돌리면서 요소를 map에 setting...
    int nSpanKeySize = aSpanKeyList.GetSize();  
    T_SPAN_D SpanD; 
    T_SPAN_K SubSpanK = 0;

    for(int i=0; i<nSpanKeySize; ++i)
    {    
        SpanD.Initialize();
        T_SPAN_K SpanK = aSpanKeyList[i]; 
        m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);
        int nSpanElemNum = SpanD.aElemBase.GetSize();

        int nSubSpanNum = SpanD.aSpan.GetSize();

        for(int j=0; j<nSpanElemNum; ++j)
        {    
            T_SPAN_BASE &SpanBase = SpanD.aElemBase[j];
            T_ELEM_K ElemK2 = SpanBase.ElemK;      
            if(j==0){SubSpanK +=1;}
            if(j!=0 && j!= nSpanElemNum - 1 && SpanBase.nSupport!=0) 
            {
                SubSpanK +=1;
            }
            ElemPairK EPairK2(ElemK2, EN_EL_BEAM);
            auto iter = m_mElemSubSpanK.find(EPairK2);
			if(iter == m_mElemSubSpanK.end())
			{
				m_mElemSubSpanK.insert({ EPairK2, SubSpanK });
			}
        }
    }

    CArray<T_SPAV_K, T_SPAV_K> aSpavKeyList;
    m_pDoc->m_pAttrCtrl->GetSpavKeyList(aSpavKeyList);

    T_SPAV_D SpavD;
    T_SPAV_K SubSpavK = 0;

    for (int i = 0; i < aSpavKeyList.GetSize(); ++i)
    {
        SpavD.Initialize();
        T_SPAV_K SpavK = aSpavKeyList[i];
        m_pDoc->m_pAttrCtrl->GetSpav(SpavK, SpavD);
        int nSpanElemNum = SpavD.aElemBase.GetSize();

        int nSubSpanNum = SpavD.aSpan.GetSize();

        for (int j = 0; j < nSpanElemNum; ++j)
        {
            const T_SPAN_BASE& SpanBase = SpavD.aElemBase[j];
            T_ELEM_K ElemK2 = SpanBase.ElemK;
            if (j == 0) { SubSpavK += 1; }
            if (j != 0 && j != nSpanElemNum - 1 && SpanBase.nSupport != 0)
            {
                SubSpavK += 1;
            }
            ElemPairK EPairK2(ElemK2, EN_EL_VBEAM);
            auto iter = m_mElemSubSpanK.find(EPairK2);
            if (iter == m_mElemSubSpanK.end())
            {
                m_mElemSubSpanK.insert({ EPairK2, SubSpavK });
            }
        }
    }

    return TRUE;
}


BOOL CDgnCsgDataCtrl::Make_ElemSpanData()
{
    m_mElemSpanD.clear();
    m_mElemSpanLength.clear();
    m_mInteriorSupport.clear();

    CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
    m_pDoc->m_pAttrCtrl->GetSpanKeyList(aSpanKeyList);
    if (aSpanKeyList.GetSize() > 0)
    {
        Make_ElemSpanDataBase(aSpanKeyList, EN_EL_BEAM);
    }

    CArray<T_SPAV_K, T_SPAV_K> aSpavKeyList;
    m_pDoc->m_pAttrCtrl->GetSpavKeyList(aSpavKeyList);
    if (aSpavKeyList.GetSize() > 0)
    {
        Make_ElemSpanDataBase(aSpavKeyList, EN_EL_VBEAM);
    }

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_ElemSpanDataBase(const CArray<T_SPAN_K, T_SPAN_K>& aSpanKeyList, const EN_BEAM_DGN_TYPE& enBeamType)
{

    T_SPAN_K TotalSubSpanK=0;
    T_SPAN_D SpanD; 
    for(int i=0; i<aSpanKeyList.GetSize(); ++i)
    {    
        int nSpanElemNum = 0;
        int nSpanLenNum = 0;
        T_SPAN_K SpanK = aSpanKeyList[i];
        SpanD.Initialize();
        if (enBeamType == EN_EL_BEAM)
        {            
            m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);
            UINT iL_count = 1;
        }
        else if (enBeamType == EN_EL_VBEAM)
        {
            m_pDoc->m_pAttrCtrl->GetSpav(SpanK, SpanD);
        }
        else ASSERT(0);

        nSpanElemNum = SpanD.aElemBase.GetSize();
        nSpanLenNum = SpanD.aSpan.GetSize();

        for(int j=0; j<nSpanElemNum; ++j)
        {    
            const T_SPAN_BASE &SpanBase = SpanD.aElemBase[j];
            T_ELEM_K ElemK2 = SpanBase.ElemK;  
			ElemPairK EPairK2(ElemK2, enBeamType);

            m_mElemSpanD.insert({ EPairK2, SpanBase });
            //      T_SPAN_K SpanValK=0;
            //       if (m_mElemSpanK.Lookup(SpanBase.ElemK, SpanValK)==FALSE)
            //       {
            //         m_mElemSpanK.SetAt(ElemK2, SpanK);
            //       }      

            CSG_INTERIOR_SUPPORT IntSupport;
            if ( SpanBase.nSupport!=0 )
            {
                bool bSupport = true;
                if(j==0 )
                { 
                    if ( SpanBase.nSupport == 2 )
                    {
                        IntSupport.bSupport[1] = true;
                        m_mInteriorSupport.insert({ EPairK2, IntSupport });
                    }
                }
                else if (j == nSpanElemNum-1 )
                {
                    if ( SpanBase.nSupport == 1 )
                    {
                        IntSupport.bSupport[0] = true;
                        m_mInteriorSupport.insert({ EPairK2, IntSupport });
                    }
                }
                else
                {
                    IntSupport.bSupport[0] = SpanBase.nSupport==1;
                    IntSupport.bSupport[1] = SpanBase.nSupport==2;
                    m_mInteriorSupport.insert({ EPairK2, IntSupport });
                }
            }
        }

        // SubSpan key
        int nSubSpanNum = 0;
        if (enBeamType == EN_EL_BEAM)
        {
            nSubSpanNum = m_pDoc->m_pAttrCtrl->GetCountSubSpan(SpanK);
		}
		else if (enBeamType == EN_EL_VBEAM)
		{
			nSubSpanNum = m_pDoc->m_pAttrCtrl->GetCountSubSpav(SpanK);
		}
		else ASSERT(0);        
        
        double dSpanLengthFromSupport = 0.0;
        double dSubSpanStart = 0.0;
        T_SPAN_K SubSpanK=0;
        for ( int j=0; j<nSubSpanNum; ++j )
        {
            SubSpanK      += 1;
            TotalSubSpanK += 1;
            double dSubSpanLength = j<nSpanLenNum ? SpanD.aSpan[j] : 0.0;
            dSpanLengthFromSupport += dSubSpanLength;
            double dLengthFromSupport = 0.0;
            for ( int k=0; k<nSpanElemNum; ++k )
            {
                const T_SPAN_BASE &SpanBase = SpanD.aElemBase[k];
                T_ELEM_K ElemK = SpanBase.ElemK;
                ElemPairK EPairK(ElemK, enBeamType);

                double dElemLength = m_pDataCtrl->GetLengthOfBeam(EPairK);				

                dLengthFromSupport += dElemLength;
                BOOL bInSpan = (dSubSpanStart<dLengthFromSupport &&  dLengthFromSupport<dSpanLengthFromSupport+m_dZero);
                if ( !bInSpan ) continue;

                auto iter = m_mElemSubSpanK.find(EPairK);
                if (iter->second != TotalSubSpanK ) continue;
                if ( dSubSpanLength > 0.0 )
                {
                    m_mElemSpanLength.insert({ EPairK, dSubSpanLength });
                }
                else
                {
                    m_mElemSpanLength.insert({ EPairK, dElemLength });
                }
            }
            dSubSpanStart += dSubSpanLength;
        }
    }

    return TRUE;
}


BOOL CDgnCsgDataCtrl::Make_CsgMatlDgnData()
{
    m_mCsgMatl.RemoveAll();

    CArray<T_MATL_K, T_MATL_K> aMatlKey;
    m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlKey);
    int nMatlKeySize = aMatlKey.GetSize();
    //T_MATD_D MatdD;  

    for(int i=0; i<nMatlKeySize; ++i)
    {
        CPG_MATL_ELEM MatlElem; MatlElem.Initialize();	
        T_MATL_K MatlK = aMatlKey[i];
        m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatlElem.MatdD);

        if (m_nDgnCode==AASHTO_LRFD20_CSG)
        {
            MatlElem.MatdD.Data2.Analysis.Elast = Calc_Ec(m_nDgnCode, MatlElem.MatdD.Data2.Design.C_fc);
        }
        else
        {
            if (MatlElem.MatdD.Data2.CodeName == _T("") || MatlElem.MatdD.Data2.CodeName == _T("None"))
            {
                MatlElem.MatdD.Data2.Analysis.Elast = Calc_Ec(m_nDgnCode, MatlElem.MatdD.Data2.Design.C_fc);
            }
        }

        //if(!m_pDataCtrl->Get_SrcMatd(ElemK, MatdD))	continue;		
        // 		if(m_iCivilCode == EUROCODE4_2_04_CSG)
        // 		{
        //       //!/
        //       // T_MATD_D에서 가져오지 못하는 Data는 CPG_MATL_ELEM에 변수를 추가하고
        //       //여기서 처리한다. 
        //       if(!Get_MatlCpgConc(ElemK, dfck, dfcm, dfctm, dEcm)) continue;
        //       MatlElem.dfck  = dfck;
        //       MatlElem.dfcm  = dfcm;
        //       MatlElem.dfctm = dfctm;
        //       MatlElem.dEcm  = dEcm;
        //       //
        //       CString strMatName = MatdD.Data1.CodeMatlName;
        //       int iMatClass=0;
        //       Get_ClassifiedMatl(strMatName,iMatClass);
        //       MatlElem.iMatClass = iMatClass;
        // 		}
        //     else ASSERT(0);


        CSG_MATL_ELEM CsgMatlD;
        if(!ConvertCsgCalcMatlData(&MatlElem, CsgMatlD)) continue;

        m_mCsgMatl.SetAt(MatlK, CsgMatlD);
    }
    return TRUE;
}

bool CDgnCsgDataCtrl::GetLongitudinalStiffenerType1(int nSectType, const ST_SECT_SECTBASE_D& stSect, const TStiffenerMaterial& stMatl, CLongitudinalStiffener* pData)
{	
	bool bStiffL = true;
	bool bStiffR = true;
	int nSectPart = 4;
	if (IsSecTypeI(nSectType))
	{
		nSectPart = 2;
		switch (stSect.Stiffener.nPosition)
		{
		case 0: { bStiffL = true;  bStiffR = true; } break;
		case 1: { bStiffL = true;  bStiffR = false; } break;
		case 2: { bStiffL = false; bStiffR = true; } break;
		default: ASSERT(0); break;
		}
	}

	std::vector<IStiffenerSingle*> vpSingle;
	for (int nPart = 0; nPart < nSectPart; ++nPart) // 0:Left, 1:Right, 2:Bottom, 3:Top
	{
		CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&> aStiff;

		TStiffenerLocation StiffLoc;
		dgn::def::enPart enSPart;
		switch (nPart)
		{
		case 0:
			{
				if (!bStiffL) continue;
				aStiff.Copy(stSect.Stiffener.aStiffLeft);
				enSPart = nSectType == D_SECT_TYPE_COMPO_I ? enPart::Web : enPart::WebL;
				StiffLoc.Location = enStiffenerLocation::Left;
			}
			break;
		case 1:
			{
				if (!bStiffR) continue;
				aStiff.Copy(stSect.Stiffener.aStiffRight);
				enSPart = nSectType == D_SECT_TYPE_COMPO_I ? enPart::Web : enPart::WebR;
				StiffLoc.Location = enStiffenerLocation::Right;
			}
			break;
		case 2:
			{
				aStiff.Copy(stSect.Stiffener.aStiffBottom);
				enSPart = enPart::Bot;
				StiffLoc.Location = enStiffenerLocation::Bot;
			}
			break;
		case 3:
			{
				aStiff.Copy(stSect.Stiffener.aStiffTop);
				enSPart = enPart::Top;
				StiffLoc.Location = enStiffenerLocation::Top;
			}
			break;
		default:
			ASSERT(0);
			break;
		}

		double dPosY = 0.0;
		double dPosZ = 0.0;
        const int nSize = aStiff.GetSize();
		for (int i = 0; i < nSize; ++i)
		{
			const ST_STIFF_PROFILE_D& Cur = aStiff[i];
			// 보강재의 Stiffness 반영 여부에 관계 없이 보강재 고려
			//if ( ProfileB.bCalc == false )
			//	continue;

			const ST_STIFF_SHAPE_D& Shape = stSect.Stiffener.aStiffShape[Cur.nShape];
            switch (enSPart)
            {
            case enPart::Web:
			case enPart::WebL:
			case enPart::WebR:
				{
					dPosZ = Cur.dSpacing;
				}
				break;
            case enPart::Top:
            case enPart::Bot:
                {
                    dPosY = Cur.dSpacing;
                }
                break;
            default:
                ASSERT(0);
                break;
            }
			
			enTypicalType enType;
			switch (Shape.nType) // 0:I, 1:T, 2:U-Rib, 3:L
			{
			case 0:
				{
					enType = enTypicalType::Rectangle;
					CAlignedShapeRectangle ShapeRect(Shape.dSize[1], Shape.dSize[0], CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(dPosY, dPosZ));
					ShapeRect.SetAlign(AlignToStfn);

					CStiffenerRectangle LSShape(ShapeRect, stMatl, enSPart, StiffLoc);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			case 1:
				{
					enType = enTypicalType::T;
					double dB = Shape.dSize[1];
					double dH = Shape.dSize[0];
					double dtw = Shape.dSize[2];
					double dtf = Shape.dSize[3];
					CAlignedShapeT ShapeT(dB, dH, dtw, dtf, CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(dPosY, dPosZ));
					ShapeT.SetAlign(AlignToStfn);

					CStiffenerT LSShape(ShapeT, stMatl, enSPart, StiffLoc);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			case 2:
				{
					enType = enTypicalType::URib;
					double dB = Shape.dSize[1];
					double dH = Shape.dSize[0];
					double dBb = Shape.dSize[2];
					double dt = Shape.dSize[3];
					double dR = Shape.dSize[4];
					CAlignedShapeURib ShapeU(dB, dH, dBb, dt, dR, CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(dPosY, dPosZ));
					ShapeU.SetAlign(AlignToStfn);

					CStiffenerURib LSShape(ShapeU, stMatl, enSPart, StiffLoc);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			default: ASSERT(0); break;
			}
		}
	}

	pData->Set(vpSingle);

    return true;
}

bool CDgnCsgDataCtrl::GetLongitudinalStiffenerType2(int nSectType, const ST_SECT_SECTBASE_D& stSect, const TStiffenerMaterial& stMatl, CLongitudinalStiffener* pData)
{
    const bool bStlShapeI = (nSectType == DGN_SECT_TYPE_COMPO_STLG_I) ? true : false;
	
    bool bStiffL = true;
	bool bStiffR = true;
	int nSectPart = 4;
	if (bStlShapeI)
	{
		nSectPart = 2;
		switch (stSect.Stiffener.nPosition)
		{
		case 0: { bStiffL = true;  bStiffR = true; } break;
		case 1: { bStiffL = true;  bStiffR = false; } break;
		case 2: { bStiffL = false; bStiffR = true; } break;
		default: ASSERT(0); break;
		}
	}

	std::vector<IStiffenerSingle*> vpSingle;
	for (int nPart = 0; nPart < nSectPart; ++nPart) // 0:Left, 1:Right, 2:Bottom, 3:Top
	{
		CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&> aStiff;

		TStiffenerLocation StiffLoc;
		dgn::def::enPart enSPart;
		switch (nPart)
		{
		case 0:
			{
				if (!bStiffL) continue;
				aStiff.Copy(stSect.Stiffener.aStiffLeft);
				enSPart = bStlShapeI ? enPart::Web : enPart::WebL;
				StiffLoc.Location = enStiffenerLocation::Left;
			}
			break;
		case 1:
			{
				if (!bStiffR) continue;
				aStiff.Copy(stSect.Stiffener.aStiffRight);
				enSPart = bStlShapeI ? enPart::Web : enPart::WebR;
				StiffLoc.Location = enStiffenerLocation::Right;
			}
			break;
		case 2:
			{
				aStiff.Copy(stSect.Stiffener.aStiffBottom);
				enSPart = enPart::Bot;
				StiffLoc.Location = enStiffenerLocation::Bot;
			}
			break;
		case 3:
			{
				aStiff.Copy(stSect.Stiffener.aStiffTop);
				enSPart = enPart::Top;
				StiffLoc.Location = enStiffenerLocation::Top;
			}
			break;
		default:
			ASSERT(0);
			break;
		}

		double dPosY = 0.0;
		double dPosZ = 0.0;
		const int nSize = aStiff.GetSize();
		for (int i = 0; i < nSize; ++i)
		{
			const ST_STIFF_PROFILE_D& Cur = aStiff[i];
			// 보강재의 Stiffness 반영 여부에 관계 없이 보강재 고려
			//if ( ProfileB.bCalc == false )
			//	continue;

			const ST_STIFF_SHAPE_D& Shape = stSect.Stiffener.aStiffShape[Cur.nShape];
			switch (enSPart)
			{
			case enPart::Web:
			case enPart::WebL:
			case enPart::WebR:
				{
					dPosZ = Cur.dSpacing;
				}
				break;
			case enPart::Top:
			case enPart::Bot:
				{
					dPosY = Cur.dSpacing;
				}
				break;
			default:
				ASSERT(0);
				break;
			}

			enTypicalType enType;
			switch (Shape.nType) // 0:I, 1:T, 2:U-Rib, 3:L
			{
			case 0:
				{
					enType = enTypicalType::Rectangle;
					CAlignedShapeRectangle ShapeRect(Shape.dSize[1], Shape.dSize[0], CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(dPosY, dPosZ));
					ShapeRect.SetAlign(AlignToStfn);

					CStiffenerRectangle LSShape(ShapeRect, stMatl, enSPart, StiffLoc);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			case 1:
				{
					enType = enTypicalType::T;
					double dB = Shape.dSize[1];
					double dH = Shape.dSize[0];
					double dtw = Shape.dSize[2];
					double dtf = Shape.dSize[3];
					CAlignedShapeT ShapeT(dB, dH, dtw, dtf, CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(dPosY, dPosZ));
					ShapeT.SetAlign(AlignToStfn);

					CStiffenerT LSShape(ShapeT, stMatl, enSPart, StiffLoc);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			case 2:
				{
					enType = enTypicalType::URib;
					double dB = Shape.dSize[1];
					double dH = Shape.dSize[0];
					double dBb = Shape.dSize[2];
					double dt = Shape.dSize[3];
					double dR = Shape.dSize[4];
					CAlignedShapeURib ShapeU(dB, dH, dBb, dt, dR, CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(dPosY, dPosZ));
					ShapeU.SetAlign(AlignToStfn);

					CStiffenerURib LSShape(ShapeU, stMatl, enSPart, StiffLoc);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			default: ASSERT(0); break;
			}
		}
	}

	pData->Set(vpSingle);

	return true;    
}

BOOL CDgnCsgDataCtrl::Make_CsgSectDgnData()
{
    BOOL bPostMode = m_pDoc->IsPostMode();
    if(!bPostMode) return FALSE;

	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

    m_mCsgSect.clear();
    m_mCsgElemSect.clear();
    ArrElemPairKey aCheckElemK;
    m_pDataCtrl->Get_ModiCheckElemKey(m_arElemK, m_pDataCtrl->m_aChkElemUblmKey, aCheckElemK);
    int nChkElemSize = aCheckElemK.GetSize();
    
    for(int i=0; i<nChkElemSize; ++i)
    {		
        CSG_SECT_ELEM SectElem;
        auto ElemK = aCheckElemK[i];

        if (ElemK.second == EN_EL_BEAM)
        {
            T_ELEM_D ElemD, ElemPreD;
            ElemD.Initialize();
            ElemPreD.Initialize();
            BOOL bGetElem = m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD);
            BOOL bGetElemPre = m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemPreD);

            T_TSGR_K TsgrK = m_pDoc->m_pAttrCtrl->GetElemTsgrKey(ElemK.first);

            T_SECT_K SectK = ElemD.elpro;
            SectPairK SPairK(SectK, EN_EL_BEAM);
            m_mCsgElemSect.insert({ ElemK, SPairK });
            if (TsgrK == 0)
            {
				const auto& itSect = m_mCsgSect.find(SPairK);
                if (itSect != m_mCsgSect.end()) continue; // 있으면...
            }

            if (Get_CsgSectData4Elem(ElemK, CpgdD.bLongTermSectProp, SectElem))
            {
                m_mCsgSect.insert({ SPairK, SectElem });
            }
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            SectPairK SPairK(ElemK.first, EN_EL_VBEAM);
            m_mCsgElemSect.insert({ ElemK, SPairK });

            const auto& itSect = m_mCsgSect.find(SPairK);
            if (itSect != m_mCsgSect.end()) continue; // 있으면...

            if (Get_CsgSectData4Elem(ElemK, CpgdD.bLongTermSectProp, SectElem))
            {
                m_mCsgSect.insert({ SPairK, SectElem });
            }
        }

    }

    if(m_mCsgSect.size()==0) return FALSE;  

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Get_CsgSectData4Elem(ElemPairK ElemK, BOOL bLongTermSectProp, CSG_SECT_ELEM& SectElem)
{
    T_SECT_D SectD; SectD.Initialize();
    T_CRDP_SSM_D CrdpSsmD; CrdpSsmD.Initialize();
    
    if (ElemK.second == EN_EL_BEAM)
    {
        T_ELEM_D ElemD, ElemPreD;
        ElemD.Initialize();
        ElemPreD.Initialize();
        BOOL bGetElem = m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD);
        BOOL bGetElemPre = bGetElem = m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemPreD);
        if (!bGetElem || !bGetElemPre)	return FALSE;

        T_SECT_K SectK = ElemD.elpro;
        T_SECT_K SectPreK = ElemPreD.elpro;

        T_TSGR_K TsgrK = m_pDoc->m_pAttrCtrl->GetElemTsgrKey(ElemK.first);

        if (TsgrK > 0)
        {
            T_SECT_D TSectD;
            m_pDoc->m_pAttrCtrl->GetSect(SectPreK, TSectD);

            T_TSGR_D TsgrD;
            double dTotalLength = 0.0;
            double dPositionI = 0.0;
            double dPositionJ = 0.0;
            if (m_pDoc->m_pAttrCtrl->GetTsgr(TsgrK, TsgrD))
            {
                double dTsgrTotLength = 0.0;
                m_pDoc->m_pAttrCtrl->CalcTsgrTotLength(TsgrD, ElemK.first, dTsgrTotLength, dPositionI);
                //         int nTsgrElemList = TsgrD.aElemList.GetSize();
                //         for(int i=0; i<nTsgrElemList; ++i)
                //         {
                //           if(TsgrD.aElemList[i]==ElemK) dPositionI = dTsgrTotLength;
                //           dTsgrTotLength += m_pDoc->calcLAVElem(TsgrD.aElemList[i]);
                //         }
                dPositionJ = dPositionI + m_pDoc->calcLAVElem(ElemD);

                m_pDoc->m_pSectDB->CalcTaperedSection(TSectD, TsgrD, dTsgrTotLength, dPositionI, dPositionJ, 0, SectD);
            }
        }
        else
        {
            m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD);
            m_pDoc->m_pAttrCtrl2->GetCrdpSsm(SectK, CrdpSsmD);
        }
    }
    else if (ElemK.second == EN_EL_VBEAM)
    {
        T_VBEM_D VbemD; VbemD.Initialize();
        if (m_pDoc->m_pAttrCtrl2->GetVbem(ElemK.first, VbemD))
        {
            T_SECV_D SecvD; SecvD.Initialize();
            m_pDoc->m_pAttrCtrl2->GetSecv(ElemK.first, SecvD); // T_SECV_K = T_VBEM_K
            SecvD.ConvertToSect(SectD);
        }

    }
    else ASSERT(0);

    // 해석에 연관되기 때문에 RPSD 가 아닌 RPSC 사용. 
    //T_RPSC_D RpsdD; RpsdD.Initialize();
    //m_pDoc->m_pAttrCtrl->GetRpsc(SectPreK, RpsdD);
    //SectElem.bCracked = RpsdD.bCracked; 

    // Girder 개수가 2개 이상인 경우는 설계대상에서 제외.
    // 2015.01.07_ 이정우GJ 요청으로 막음.
    //     if(SectD.SectAfter.SectI.BuiltUpFlag>=2)
    //     {
    // 			AfxMessageBox(_T("Composite Design is failed.( Girder Num > 1)"));
    // 			return FALSE;
    //     }    

    int nSectType = 0;
    if (SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_G) nSectType = D_SECT_TYPE_COMPO_G;
    else if (SectD.nStype == D_SECT_TYPE_COMPO_G) nSectType = D_SECT_TYPE_COMPO_G;
    else if (SectD.nStype == D_SECT_TYPE_TAPERED)
    {
        nSectType = SectD.SectBefore.nStype;
    }
    else nSectType = SectD.nStype;

    BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;

    T_SECT_D WSectD = SectD;
    T_STPO_D StpoD;
    if (nSectType == D_SECT_TYPE_COMPO_STLG_I ||
        nSectType == D_SECT_TYPE_COMPO_STLG_B ||
        nSectType == D_SECT_TYPE_COMPO_STLG_TUB)
    {
        CSectUtil::GetStressPointSOD_Stiffener(SectD, StpoD);
    }


    for (int j = 0; j < 2; ++j)
    {
        BOOL bPosiI = (j == 0);
        //if (bTapered==FALSE && j==1) continue;
        T_SECT_SECTBASE_D& SectBaseD = (!bPosiI && bTapered && j == 1) ? SectD.CmpTapJ : SectD.SectBefore.SectI;
        if (!bPosiI && bTapered && j == 1)
        {
            SectBaseD.Stiffener = SectD.SectBefore.SectI.Stiffener;
            SectBaseD.SODStiffener = SectD.SectBefore.SectI.SODStiffener;

            SectD.SectBefore.SectJ.Stiffener = SectD.SectBefore.SectI.Stiffener;
            SectD.SectBefore.SectJ.SODStiffener = SectD.SectBefore.SectI.SODStiffener;
        }

        CSG_SECT_POSD& CsgSPos = SectElem.SectPos[j];
        CsgSPos.SectInfo.bIsTapered = bTapered;
        CsgSPos.SectInfo.dn_LT = SectD.SectBefore.dLongTermESEC;

        // Composite General Section
        if (nSectType == D_SECT_TYPE_COMPO_G)
        {
            ASSERT(0);
        }
        else
        {
            // 0=Rebar 반영한 환산단면
            // 1=부모멘트경우(콘크리트 무시) 환산단면
            T_SECT_STIFFNESS SpscSect[4];
            for (int k = 0; k < 4; ++k) SpscSect[k].Initialize();

            BOOL bSecf = FALSE;
            BOOL bCrack = FALSE;
            BOOL bStiffn = CSectUtil::GetCompoSectCR(TRUE, FALSE, bCrack, 0, TRUE, ElemK, bPosiI, bSecf, SpscSect[0]);
            BOOL bStiff3n = CSectUtil::GetCompoSectCR(TRUE, TRUE, bCrack, 2, TRUE, ElemK, bPosiI, bSecf, SpscSect[2]);
            bCrack = TRUE;
            BOOL bStiffCrack = CSectUtil::GetCompoSectCR(TRUE, FALSE, bCrack, 0, TRUE, ElemK, bPosiI, bSecf, SpscSect[1]);
            BOOL bStiffReba3n = CSectUtil::GetCompoSectCR(TRUE, FALSE, bCrack, 1, TRUE, ElemK, bPosiI, bSecf, SpscSect[3]);

            BOOL bCalcOK = FALSE;
            switch (nSectType)
            {
            case D_SECT_TYPE_COMPO_I:
            case D_SECT_TYPE_COMPO_B:
            case D_SECT_TYPE_COMPO_TUB:
            case D_SECT_TYPE_COMPO_STLG_I:
            case D_SECT_TYPE_COMPO_STLG_B:
            case D_SECT_TYPE_COMPO_STLG_TUB:
            {
                bCalcOK = (bStiffn && bStiffCrack && bStiff3n && bStiffReba3n);
            }
            break;
            case D_SECT_TYPE_STLG_I:
            case D_SECT_TYPE_STLG_B:
            case D_SECT_TYPE_STLG_MCELL:
            case D_SECT_TYPE_REGULAR:
            {
                bCalcOK = TRUE;
            }
            break;
            default: ASSERT(0); break;
            }

            // Set Sect (0=I, 1=J).
            if (bCalcOK)
            {
                ConvertCsgOrgSectData(j, &SectD, CsgSPos);

                ConvertSectStiffnessData(SpscSect[1], CsgSPos.NonSlabStiff); // bCrack !!!
                if (bLongTermSectProp)
                {
                    ConvertSectStiffnessData(SpscSect[3], CsgSPos.NonSlabStiff2); // 
                }
                else
                {
                    ConvertSectStiffnessData(SpscSect[1], CsgSPos.NonSlabStiff2);
                }


                double dYbar_temp = 0.0, dZbar_temp = 0.0;
                if (CDBLib::IsCompositeSect(nSectType))
                {
                    T_SECT_STIFFNESS PureStiff; PureStiff.Initialize();
                    CSectUtil::GetCompoSectStiffnessOfSECT(SectD, bPosiI, PureStiff, dYbar_temp, dZbar_temp);

                    // Modification, MQC, 4596 관련 철근 정보는 단면특성에 반영 안하는 것으로. 
                    // SectElem.SectPosi[j].RebarSect     = SectElem.SectPosi[j].PureSect;
                    // SectElem.SectPosi[j].RbarStiff3n   = SectElem.SectPosi[j].PureSect;  // 수정필요
                    // SectElem.SectPosi[j].NonSlabStiff2 = SpscSect[1]; // 수정 필요 

                    ConvertSectStiffnessData(PureStiff, CsgSPos.PureSect);
                    //PMS: BSC-20140408 이혜연 과장 요청으로 인해 정모멘트일때 슬래브에 배근된 철근은 포함 
                    ConvertSectStiffnessData(PureStiff/*SpscSect[0]*/, CsgSPos.RbarStiffn); // n
                    ConvertSectStiffnessData(SpscSect[2], CsgSPos.RbarStiff3n);   // 3n
                }

                // Stiffener stiffness
                if (nSectType == D_SECT_TYPE_COMPO_STLG_I ||
                    nSectType == D_SECT_TYPE_COMPO_STLG_B ||
                    nSectType == D_SECT_TYPE_COMPO_STLG_TUB)
                {
                    ConvertSodStiffenerData(nSectType, &SectBaseD, StpoD, &SectBaseD.SODStiffener, CsgSPos.SectInfo.Before.Stiffener);
                }
                else
                {
                    CalcStiffnessOfStiffener(SectBaseD.Stiffener, CsgSPos.SectInfo.Before.Stiffener);
                }

                TCHAR sIJ = j == 0 ? 'I' : 'J';
                T_SECT_STIFFNESS* pStiff = CSectUtil::GetProperStiffPosition(WSectD, TRUE, 1, sIJ); // LongTerm
                ConvertCompSectStiffness4Warping(j, WSectD, SectElem.SectPos[j]);
                ConvertSectStiff4Warping(*pStiff, SectElem.SectPos[j].WSlt);

                ConvertReinforcedSectInfo(j, WSectD, SectElem.SectPos[j]);

                ConvertCorrosionSectInfo(j, WSectD, CrdpSsmD, SectElem.SectPos[j]);
            }
        }
    }

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CsgTrstDgnData()
{
    m_mCsgTrst.clear();

    CArray<T_CGTS_SSM_K, T_CGTS_SSM_K> aKeyList;
    m_pDoc->m_pAttrCtrl2->GetCgtsSsmKeyList(aKeyList);

    int aKeySize = aKeyList.GetSize();
    for (int i=0; i<aKeySize; ++i)
    {
        T_CGTS_SSM_D CgtsSSM;
        SectPairK SPairK(aKeyList[i], EN_EL_BEAM);
        CgtsSSM.Initialize();
        if (m_pDoc->m_pAttrCtrl2->GetCgtsSsm(aKeyList[i], CgtsSSM))
        {
            CSG_TRST_SECT TrstD;
            ConvertCsgCalcCgtsData(&CgtsSSM, TrstD);
            m_mCsgTrst.insert({SPairK, TrstD});
        }
    }

    CArray<T_CGTV_SSM_K, T_CGTV_SSM_K> aCgtvKeyList;
    m_pDoc->m_pAttrCtrl2->GetCgtvSsmKeyList(aCgtvKeyList);

    T_CGTV_SSM_D CgtvSSM;
    for (int i = 0; i < aCgtvKeyList.GetSize(); ++i)
    {
        SectPairK SPairK(aCgtvKeyList[i], EN_EL_VBEAM);
        CgtvSSM.Initialize();
        if (m_pDoc->m_pAttrCtrl2->GetCgtvSsm(aCgtvKeyList[i], CgtvSSM))
        {
            T_CGTS_SSM_D CgtsSSM;
			CgtsSSM=CgtvSSM;
            CSG_TRST_SECT TrstD;
            ConvertCsgCalcCgtsData(&CgtsSSM, TrstD);
            m_mCsgTrst.insert({ SPairK, TrstD });
        }
    }
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CsgCrdpData()
{
    CArray<T_CRDP_SSM_K, T_CRDP_SSM_K> aKeyList;
    m_pDoc->m_pAttrCtrl2->GetCrdpSsmKeyList(aKeyList);

    T_CRDP_SSM_D CrdpSSM;
    int aKeySize = aKeyList.GetSize();
    for (int i = 0; i < aKeySize; ++i)
    {
        CrdpSSM.Initialize();
        CSG_SECT_ELEM SectElem;
        SectPairK SPairK(aKeyList[i], EN_EL_BEAM);
		const auto& itSect = m_mCsgSect.find(SPairK);
		if (itSect == m_mCsgSect.end()) continue;

        if (m_pDoc->m_pAttrCtrl2->GetCrdpSsm(aKeyList[i], CrdpSSM))
        {
            SectElem = itSect->second;
			for (int j=0; j<2; ++j)
			{
                ConvertCsgCalcCordData(j, &CrdpSSM, SectElem.SectPos[j].SectInfo.Before);
			}

			m_mCsgSect[itSect->first] = SectElem;

        }
    }

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CpgSconElem()
{
    m_mCsgCpsc.clear();

    CSG_CPSC_ELEM SconElem;
    for(int i=0; i<m_arElemK.GetSize(); i++)
    {
        SconElem.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        if ( m_nDgnCode == CS457_R1_CSG || m_nDgnCode == NR_GN_CIV_025_06_CSG)
        {
            T_SRSC_D SrscD; SrscD.Initialize();
            BOOL bSrsc = m_pDoc->m_pAttrCtrl2->GetSrsc(ElemK.first, SrscD);
            T_SRSV_D SrsvD; SrsvD.Initialize();
            BOOL bSrsv = m_pDoc->m_pAttrCtrl2->GetSrsv(ElemK.first, SrsvD);
            if (!bSrsc && !bSrsv) continue;
			if (bSrsv && ElemK.second==EN_EL_VBEAM) SrscD = SrsvD;
            
            for ( int j=0; j<2; j++ )
            {
                const T_SRSC_STUD& SCon = SrscD.Sconn[j];
                CSG_CPSC_POSD& TfType = SconElem.TfType[j];
                TfType.dPitch    = SCon.dLSpacing;
                TfType.nNum      = SCon.nNumStudWeb;                
                TfType.dFu       = SCon.dPr;     
                TfType.dFrc      = SCon.dPim;
                TfType.dq_qr_ratio = SCon.dMaxRatio;
                TfType.dNa       = SCon.dNa;
            }
        }
        else
        {
            T_CPSC_D CpscD;
            CpscD.Initialize();
            BOOL bCpsc = m_pDoc->m_pAttrCtrl2->GetCpsc(ElemK.first, CpscD);
			T_CPSV_D CpsvD; CpsvD.Initialize();
            BOOL bCpsv = m_pDoc->m_pAttrCtrl2->GetCpsv(ElemK.first, CpsvD);
            if (!bCpsc && !bCpsv) continue;
            if (bCpsv && ElemK.second==EN_EL_VBEAM) CpscD=CpsvD;
            // Set Rbar (0=I, 1=J).
            for ( int j=0; j<2; j++ )
            {
                SconElem.TfType[j].nType     = CpscD.TfType[j].nType;
                SconElem.TfType[j].nCategory = CpscD.TfType[j].nCategory;
                SconElem.TfType[j].dHeight   = CpscD.TfType[j].dHeight;
                SconElem.TfType[j].dPitch    = CpscD.TfType[j].dPitch;
                SconElem.TfType[j].nNum      = CpscD.TfType[j].nNum;
                SconElem.TfType[j].dSpace    = CpscD.TfType[j].dSpace;
                SconElem.TfType[j].dFu       = CpscD.TfType[j].dFu;
                SconElem.TfType[j].dDia      = CpscD.TfType[j].dDia;
                SconElem.TfType[j].dLength   = CpscD.TfType[j].dLength;
                SconElem.TfType[j].nSFCalc   = CpscD.TfType[j].nSFCalc;
                SconElem.TfType[j].dFrc      = CpscD.TfType[j].dFrc;
            }

        }

        // Set
        m_mCsgCpsc.insert({ ElemK, SconElem });
    }
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CpgCpfpElem()
{
    m_mCsgCpfp.clear();
    CSG_CPFP_ELEM CpfpElem;
    for(int i=0; i<m_arElemK.GetSize(); i++)
    {
        CpfpElem.Initialize();
        T_CPFP_D CpfpD;
        CpfpD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        BOOL bCpfp = m_pDoc->m_pAttrCtrl2->GetCpfp(ElemK.first,CpfpD);
        T_CPFV_D CpfvD; CpfvD.Initialize();
        BOOL bCpfv = m_pDoc->m_pAttrCtrl2->GetCpfv(ElemK.first, CpfvD);
        if (!bCpfp && !bCpfv) continue;
        if (bCpfv && ElemK.second == EN_EL_VBEAM) CpfpD=CpfvD;
        for(int j=0; j<2; j++)
        {
            CpfpElem.TfType[j].nDetailCategory  =   CpfpD.TfType[j].nDetailCategory; 
            CpfpElem.TfType[j].dADTT            =   CpfpD.TfType[j].dADTT;      
            CpfpElem.TfType[j].dNum             =   CpfpD.TfType[j].dNum; 
            CpfpElem.TfType[j].nWarpType        =   CpfpD.TfType[j].nWarpType;
            CpfpElem.TfType[j].dTopFlange       =   CpfpD.TfType[j].dTopFlange;
            CpfpElem.TfType[j].dBotFlange       =   CpfpD.TfType[j].dBotFlange;
            //CSA
            CpfpElem.TfType[j].nClassHighway    =   CpfpD.TfType[j].nADTT;
            CpfpElem.TfType[j].dYear            =   CpfpD.TfType[j].dDesignLife;
            CpfpElem.TfType[j].dWeight          =   CpfpD.TfType[j].dWeight;
            CpfpElem.TfType[j].dNd              =   CpfpD.TfType[j].dNd;
            CpfpElem.TfType[j].dp               =   CpfpD.TfType[j].dp;
            CpfpElem.TfType[j].bAutoChk         =   CpfpD.TfType[j].bAutoChk;
            CpfpElem.TfType[j].nLongiMembType   =   CpfpD.TfType[j].nGirderType;
            //AASHTO
            CpfpElem.TfType[j].dTopFlange_Bot   =   CpfpD.TfType[j].dTopFlangeBot;
            CpfpElem.TfType[j].dBotFlange_Top   =   CpfpD.TfType[j].dBotFlangeTop;

            CpfpElem.TfType[j].nDetailCategory2 =   CpfpD.TfType[j].nDetailCategory2;
            CpfpElem.TfType[j].nDetailCategory3 =   CpfpD.TfType[j].nDetailCategory3;
            CpfpElem.TfType[j].nDetailCategory4 =   CpfpD.TfType[j].nDetailCategory4;
        }

        // Set
        m_mCsgCpfp.insert({ ElemK, CpfpElem });
    }
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CpgCpcbElem()
{
    m_mCsgCpcb.clear();
    CSG_CPCB_ELEM CpcbElem;
    for(int i=0; i<m_arElemK.GetSize(); i++)
    {
        CpcbElem.Initialize();
        T_CPCB_D CpcbD;
        CpcbD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        BOOL bCpcb = m_pDoc->m_pAttrCtrl2->GetCpcb(ElemK.first,CpcbD);
        T_CPCV_D CpcvD;
        BOOL bCpcv = m_pDoc->m_pAttrCtrl2->GetCpcv(ElemK.first, CpcvD);
        if (!bCpcb && !bCpcv) continue;
        if (bCpcv && ElemK.second == EN_EL_VBEAM) CpcbD=CpcvD;
        // Set Rbar (0=I, 1=J).
        for(int j=0; j<2; j++)
        {
            CpcbElem.TfType[j].dRadius    = CpcbD.TfType[j].dRadius; 
            CpcbElem.TfType[j].nCurveType = CpcbD.TfType[j].nCurveType; 
            CpcbElem.TfType[j].bIJDirct   = TRUE;  // 여기 ij 방향 가져오는 함수로 ij 방향 받아와야 함!  i->j : ture,  j->i : false
        }

        // Set Rbar.
        m_mCsgCpcb.insert({ ElemK, CpcbElem });
    }
    return TRUE;
}


// Deck Overhang Loads
BOOL CDgnCsgDataCtrl::Make_CpgDohlElem()
{
    m_mCsgDohl.clear();
    CSG_DOHL_ELEM DohlElem;
    for(int i=0; i<m_arElemK.GetSize(); i++)
    {
        DohlElem.Initialize();
        T_DOHL_D DohlD;
        DohlD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        BOOL bDohl = m_pDoc->m_pAttrCtrl2->GetDohl(ElemK.first,DohlD);
        T_DOHV_D DohvD;
        BOOL bDohv = m_pDoc->m_pAttrCtrl2->GetDohv(ElemK.first, DohvD);
        if (!bDohl && !bDohv) continue;
        if (bDohv && ElemK.second == EN_EL_VBEAM) DohlD=DohvD;

        for(int j=0; j<2; j++)
        { 
            DohlElem.DohlPos[j].bUseDohl   =  TRUE;
            DohlElem.DohlPos[j].dDistForce   = DohlD.DohlType[j].dDistForce; 
            DohlElem.DohlPos[j].dConcForce   = DohlD.DohlType[j].dConcForce; 
            DohlElem.DohlPos[j].dEccenLoad   = DohlD.DohlType[j].dEccenLoad; 
        }

        m_mCsgDohl.insert({ ElemK, DohlElem });
    }
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CpgEbslElem()
{
    m_mCsgEbsl.clear();
    T_EBSL_D EbslD;
    for ( INT_PTR i=0; i<m_arElemK.GetSize(); ++i )
    {
        EbslD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        BOOL bEbsl = m_pDoc->m_pAttrCtrl2->GetEbsl(ElemK.first, EbslD);
        T_EBSV_D EbsvD;
        BOOL bEbsv = m_pDoc->m_pAttrCtrl2->GetEbsv(ElemK.first, EbsvD);
        if (!bEbsl && !bEbsv) continue;
        if (bEbsv && ElemK.second == EN_EL_VBEAM) EbslD= EbsvD;

        CSG_EBSL_ELEM EbslElem;
        for ( int j=0; j<2; ++j )
        {
            CSG_EBSL_POSD& EbslPos = EbslElem.EbslPos[j];
            EbslPos.bUse = true;
            EbslPos.dBs  = EbslD.dSlab[j];
            EbslPos.dBt  = EbslD.dTF[j];
            EbslPos.dBb  = EbslD.dBF[j];
        }
        if ( EbslD.bBothIJ )
        {
            EbslElem.EbslPos[1] = EbslElem.EbslPos[0];
        }
        m_mCsgEbsl.insert({ ElemK, EbslElem });
    }
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CpgLtbdElem()
{
    m_mCsgLtbd.clear();
    T_CGLT_D CgltD;
    for ( INT_PTR i=0; i<m_arElemK.GetSize(); ++i )
    {
        CgltD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        BOOL bCglt = m_pDoc->m_pAttrCtrl->GetCglt(ElemK.first, CgltD);
        T_CGLV_D CglvD;
        BOOL bCglv = m_pDoc->m_pAttrCtrl->GetCglv(ElemK.first, CglvD);
        if (!bCglt && !bCglv) continue;
        if (bCglv && ElemK.second == EN_EL_VBEAM) CglvD.ConvertToCglt(CgltD);

        CSG_LTBD_ELEM LtbdElem;
        for ( int j=0; j<2; ++j )
        {
            CSG_LTBD_POSD& LtbdPos = LtbdElem.LtbPos[j];
            LtbdPos.bUse = true;
            LtbdPos.da   = CgltD.LTdata[j].da;
        }
        if ( CgltD.bBothIJ )
        {
            LtbdElem.LtbPos[1] = LtbdElem.LtbPos[0];
        }
        m_mCsgLtbd.insert({ ElemK, LtbdElem });
    }
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CpgDwspElem()
{
    m_mCsgDwsp.clear();
    T_SRSP_D SrspD;
    for ( INT_PTR i=0; i<m_arElemK.GetSize(); ++i )
    {
        SrspD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        BOOL bSrsp = m_pDoc->m_pAttrCtrl2->GetSrsp(ElemK.first, SrspD);
		T_SRVP_D SrvpD;
		BOOL bSrvp = m_pDoc->m_pAttrCtrl2->GetSrvp(ElemK.first, SrvpD);
        if (!bSrsp && !bSrvp) continue;
        if (bSrvp && ElemK.second == EN_EL_VBEAM) SrspD = SrvpD;

        CSG_DWSP_ELEM DwspElem;
        for ( int j=0; j<2; ++j )
        {
            CSG_DWSP_POSD& DwspPos = DwspElem.DwspPos[j];
            DwspPos.bUse = true;
            DwspPos.df_dw_tf = SrspD.TfType[j].dTopFlange;
            DwspPos.df_dw_bf = SrspD.TfType[j].dBotFlange;
        }
        if ( SrspD.bBothIJ )
        {
            DwspElem.DwspPos[1] = DwspElem.DwspPos[0];
        }
        m_mCsgDwsp.insert({ ElemK, DwspElem });
    }
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CsgUfraElem()
{
    m_mCsgUfra.clear();

    auto _L_ElemVector = [&](const T_NODE_D& NoI, const T_NODE_D& NoJ, double EVec[3]) -> void
		{
			EVec[0] = NoJ.x - NoI.x;
			EVec[1] = NoJ.y - NoI.y;
			EVec[2] = NoJ.z - NoI.z;
            CMathFunc::mathNormalize(EVec, EVec);
        };

    CArray<T_ELUF_K, T_ELUF_K> aElufKey;
    m_pDoc->m_pAttrCtrl->GetElufKeyList(aElufKey);
    T_ELUF_D ElufD;
	for (INT_PTR i = 0; i < aElufKey.GetSize(); ++i)
	{
		ElufD.Initialize();
		T_ELUF_K ElufK = aElufKey.GetAt(i);
        ElemPairK ElemK(ElufK, EN_EL_BEAM);
		if (ElemK.second == EN_EL_VBEAM) continue; // Vbeam은 제외

		if (!m_pDoc->m_pAttrCtrl->GetEluf(ElemK.first, ElufD))	continue;

		CSG_UFRA_ELEM UfraElem;

        BOOL bAutoCalcI2 = TRUE; // 자동 판단 해야 되는 경우에 열자
        if (bAutoCalcI2) 
        {
            T_UFMT_D UfmtD; UfmtD.Initialize();
            m_pDoc->m_pAttrCtrl->GetUfmt(ElemK.first, UfmtD);

            const double dAngleFactor = 1.0;
            T_NODE_D NodeI, NodeJ;
            CArray<T_ELEM_K, T_ELEM_K> aElemI;
            T_ELEM_D ElemD;
            if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemD)) continue;
			m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeI);
            m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeJ);

			double MgVec[3] = { 0.0, 0.0, 0.0 };
            _L_ElemVector(NodeI, NodeJ, MgVec);

            for (int IJ = 0; IJ < 2; IJ++)
            {
                CSG_UFRA_POSD& UfraPos = UfraElem.UfraPos[IJ];
                UfraPos.bUse = true;
                UfraPos.bCrossMemberBottomFlange = ElufD.nConnectType == 0 ? true : false;
                UfraPos.dd1 = ElufD.dd1;
                UfraPos.dd2 = ElufD.dd2;
                UfraPos.dB  = ElufD.dB;

                m_pDoc->m_pAttrCtrl->GetConnectedElem(ElemD.elnod[IJ], aElemI);
                if (aElemI.GetSize() > 0 && UfmtD.nMemberType == 0)
                {
                    for (int j = 0; j < aElemI.GetSize(); ++j)
                    {
                        T_ELEM_K CurElemK = aElemI.GetAt(j);
                        if (CurElemK == ElemK.first) continue; // 자기 자신은 제외
                        T_ELEM_D CurElemD;
                        if (!m_pDoc->m_pAttrCtrl->GetElem(CurElemK, CurElemD)) continue;

                        T_NODE_D CurNodeI, CurNodeJ;
                        m_pDoc->m_pAttrCtrl->GetNode(CurElemD.elnod[0], CurNodeI);
                        m_pDoc->m_pAttrCtrl->GetNode(CurElemD.elnod[1], CurNodeJ);

                        double CgVec[3] = { 0.0, 0.0, 0.0 };
                        _L_ElemVector(CurNodeI, CurNodeJ, CgVec);

                        if (fabs(CMathFunc::mathCrossAngle(CgVec, MgVec)) < dAngleFactor) continue;

                        T_SECT_D SectD;   SectD.Initialize();
                        m_pDoc->m_pPostCtrl->GetSectPost(CurElemD.elpro, SectD);

                        UfraPos.dI2 = SectD.SectBefore.SectI.Stiffness.Ryy;
                        UfraPos.dCzp_cg = SectD.SectBefore.SectI.Stiffness.Czp;
                        UfraPos.dCzm_cg = SectD.SectBefore.SectI.Stiffness.Czm;
                        UfraPos.dHcg = UfraPos.dCzp_cg + UfraPos.dCzm_cg;

                        break; // dB는 입력을 받고 있음 자동 계산이 필요할 때 수정

                        double dCGLength = m_pDoc->calcLAVElem(ElemK.first);

                        BOOL bNextConnNodeJ = FALSE;
                        if (EQ(NodeI.x, CurNodeI.x) && EQ(NodeI.y, CurNodeI.y) && EQ(NodeI.z, CurNodeI.z))
                        {
                            bNextConnNodeJ = TRUE;
                        }

                        BOOL bNextFind = TRUE;
                        m_pDataCtrl->CalcCrossGirderLength(CurElemK, bNextConnNodeJ, CgVec, dCGLength, bNextFind);

                        //UfraPos.dB = dCGLength;
                    }
                }

                switch (ElufD.nJoint)
                {
                case 0: UfraPos.enFlexJoint = EN_UF_Flex_Joint_Type_a_0p5; break;
                case 1: UfraPos.enFlexJoint = EN_UF_Flex_Joint_Type_b_0p2; break;
                case 2: UfraPos.enFlexJoint = EN_UF_Flex_Joint_Type_c_0p1; break;
                default:
                    ASSERT(0);
                    break;
                }
            }
        }		
        
        m_mCsgUfra.insert({ ElemK, UfraElem });
	}

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CsgMdfsElem()
{
    m_mCsgMdfs.clear();

	T_MDFS_D MdfsD;
    for (INT_PTR i = 0; i < m_arElemK.GetSize(); ++i)
    {
        MdfsD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        if (!m_pDoc->m_pAttrCtrl->GetMdfs(ElemK.first, MdfsD))	continue;

        CSG_MDFS_ELEM MdfsElem;
        CSG_MDFS_POSD& MdfsPos = MdfsElem.MdfsPos[0];
        MdfsPos.bUse = true;
        MdfsPos.dEta = MdfsD.dLambdaLT;

        MdfsElem.MdfsPos[1] = MdfsElem.MdfsPos[0];
        m_mCsgMdfs.insert({ ElemK, MdfsElem });
    }

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Make_CsgLshrElem()
{
    m_mCsgLshr.clear();

    T_RIVT_D RivtD;
    for (INT_PTR i = 0; i < m_arElemK.GetSize(); ++i)
    {
        RivtD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        if (!m_pDoc->m_pAttrCtrl->GetRivt(ElemK.first, RivtD))	continue;

        CSG_LSHR_ELEM LshrElem;
        for (int j = 0; j < 2; ++j)
        {
			CSG_LSHR_POSD& LshrPos = LshrElem.LshrPos[j];
			LshrPos.bUse = true;
			LshrPos.dSpace = RivtD.RivtSub[j].dLongiSpacing;
            LshrPos.dFy    = RivtD.RivtSub[j].dYieldStre;
            LshrPos.dFu    = RivtD.RivtSub[j].dUltTensStre;
        }
        if (RivtD.bSameIJ)
        {
            LshrElem.LshrPos[1] = LshrElem.LshrPos[0];
        }
        m_mCsgLshr.insert({ ElemK, LshrElem });
    }

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_CompositeSteelGirder()
{  
	bool bCalcEngine = CDgnCodeCtrl::IsIDesignModule(m_iCivilCode);

    IDgnMemoizationLib::Instance()->CreateMemo(IDgnMemoizationLib::EN_MEMO_PLASTIC_MOMENT_NA);

    CPlateGirderDesign *pCheckGirder = m_pDoc->m_pPostCtrl->GetPlateGirder();
    Make_CSGDgnData();

    ArrElemPairKey arCheckElemK; 
    m_pDataCtrl->GetPgdElemKeyList(arCheckElemK);
    int nChkElemSize = arCheckElemK.GetSize();

    m_pDoc->SetDesignStatus(6);

    T_CPGD_D CpgdD; CpgdD.Initialize();  
    m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

    // Pre-Combined Composite Bridge
    BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb(); 

    int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
    int nStrnLcomSize = m_pForcCtrl->Get_LcomNumForStrn();
    int nServLcomSize = m_pForcCtrl->Get_LcomNumForSLS();

    // Span 정보 계산 
	ArrElemPairKey aCheckElemK4Span;
    m_pDataCtrl->Calc_LengthPosition4Span(aCheckElemK4Span);

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);  

    int nLcomK4CS=0;
    double dFactorDeadCS=0.0;
    if (!GetDeadLoadCSMaxFactor(nLcomK4CS, dFactorDeadCS)) dFactorDeadCS = 1.0;

    T_CSLF_CPT Cslf;
    Cslf.dFacDL = dFactorDeadCS;
    m_pForcCtrl->GetErectionCSMaxFactor(StctD, Cslf);

    int nForceBeforeType=1;

    CSG_MEMB_RESULT_D      MembR;
    CSG_CHECK_RESULT_CODES ChkResD;

	if ( !bCalcEngine )
	{
		ClassNewMembResult(MembR);
		ClassNewDgnResult(ChkResD);
	}

	m_vRes.clear();
	m_ElemToLcom.clear();
	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlgCVL(PROGRESS_TYPE_STEEL_COMPOSITE);
	int nProgress = 0;

    DgnBeamPairK nCurUblmK(EN_EL_BEAM, -1);    
    ArrElemPairKey aUblmEPairK;
    ADGNMEMB_FORCE            aDgnMembFor;
    ADGNMEMB_FORCE            aDgnMembFor_LTerm;      //Unbraced LongTerm 부재력
    ADGNMEMB_FORCE            aDgnMembFor_Pre;        // bIsPLCB 일 경우 거더 부재력
    for (int i=0; i<nChkElemSize; ++i)
    {
        CSG_MEMB_POSD_KSCE_LSD MembLcomD;
        MembLcomD.Initialize();
        InitMembResult(MembR);

        //S_CSGD_D CsgD;
        ConvertCGSGlobalData(CpgdD, MembLcomD.Memb.Csgd);
        int nConsiderPv =   MembLcomD.Memb.Csgd.nConsiderPv; // 0:직선, 1:곡선
        BOOL bCurved =  (nConsiderPv==0)? FALSE : TRUE;

        auto ElemK = m_arElemK[i];
        T_ELEM_D ElemD; ElemD.Initialize();
        int nMatlK = 0;        ;
        T_VBEM_D VbemD; VbemD.Initialize();
        if (ElemK.second == EN_EL_BEAM)
        {
            if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD)) continue;
			nMatlK = ElemD.elmat;
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {            
            if (m_pDoc->m_pAttrCtrl2->GetVbem(ElemK.first, VbemD))
            {
                T_MATL_K SlabMatlK = 0, GirderMatlK = 0;
                m_pDoc->m_pAttrCtrl2->GetSecvMatlK(ElemK.first, SlabMatlK, GirderMatlK);
				nMatlK = GirderMatlK;                
            }            
        }
        else ASSERT(0);
        
		SetCheckElem(ElemK.first);
        SetCheckElemPairK(ElemK);

        BOOL bChkIJ[2]={FALSE};
        bChkIJ[0] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 1);
        bChkIJ[1] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 2);

        int nSelUnit	= CDBLib::GetReportSelUnit(); //0:US, 1:SI
        MembLcomD.nSelUnit = nSelUnit;

        MembLcomD.Memb.ChIf.dLength = m_pDataCtrl->GetLengthOfBeam(ElemK);

        // Material         
        if (!m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl))
        {
            CString strErrMsg; 
            strErrMsg.Format(_LS(IDS_DGN_CPG_NO_DGN_MATERIAL), ElemK);
            GSaveHistoryFormatNF(strErrMsg);
            continue; 
        }

        CSG_SECT_ELEM CsgSect;
        GetCsgSectionSupport(ElemK, MembLcomD);

        T_KFAC_D KFacD; KFacD.Initialize();
        BOOL bGetKfac = m_pDataCtrl->GetGenKfactor(ElemK, KFacD);

        // 절점별 데이타
        Get_CsgMembInData(ElemK, MembLcomD);
        int nChkLcomSize=1;
        MembLcomD.aChkFor.SetSize(nChkLcomSize);
        MembLcomD.aUlmFor.SetSize(nChkLcomSize);
        MembLcomD.aReac.SetSize(nChkLcomSize);

        T_STAG_K nGdLastStagK=0;
        int nLastStepNo=0;
        m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);

        T_SPAN_K CurSpanK=0;
        ElemPairK EPairI=ElemK, EPairM=ElemK, EPairJ=ElemK;
        int nPosI=0;
        int nPosM=0; // 임시, 찾아야함. 
        int nPosJ=1;

        BOOL bChkUblm=FALSE;
        BOOL bCantilever=FALSE;
        BOOL bMakeMembFor=FALSE;
        BOOL b2SideBgg=FALSE;
        DgnBeamPairK nUblmK(static_cast<EN_BEAM_DGN_TYPE>(ElemK.second),0);

        m_pDataCtrl->GetUbLengthElemInfo(ElemK, nUblmK, nCurUblmK, EPairI, EPairM, EPairJ, nPosI, nPosM, nPosJ, bChkUblm, bCantilever, bMakeMembFor, b2SideBgg, aUblmEPairK);

        int nUblElemSize = aUblmEPairK.GetSize();
        //MembLcomD.bChkUlmf = bChkUblm;
        MembLcomD.bChkUlmf = nUblElemSize > 0 ? true : false;
        ADGNFORCE_EX aForceLcom_EX;
        ADGNFORCE_EX aForceTnpr_EX; 
        ADGNFORCE_EX aForceTnsc_EX;
        ADGNSHRSADD_EX aStssLcom_EX;
        if (bMakeMembFor==TRUE)
        {
            aDgnMembFor.RemoveAll();
            if (nUblElemSize>0) aDgnMembFor.SetSize(nUblElemSize);
            if (nUblElemSize>0) aDgnMembFor_LTerm.SetSize(nUblElemSize);
            if (nUblElemSize>0) aDgnMembFor_Pre.SetSize(nUblElemSize);

            for (int nUbl=0; nUbl<nUblElemSize; ++nUbl )
            {
                _DGN_FORC_UL_MEMB MembForD;
                _DGN_FORC_UL_MEMB MembForD_LTerm;
                ADGNFORCE_EX aMembForD_NC;
				ElemPairK UblEPairK = aUblmEPairK[nUbl];
                if (!m_pForcCtrl->Get_LcomDataForDesign_EX(UblEPairK, 0, TRUE, TRUE, MembForD.ElemFor, nLcomNum)) return FALSE;
                if (!m_pForcCtrl->Get_LcomDataForDesign_EX(UblEPairK, TRUE, TRUE, aForceLcom_EX, aForceTnpr_EX, aForceTnsc_EX, MembForD_LTerm.ElemFor, aStssLcom_EX, nLcomNum)) return FALSE;
                if(bIsPLCB)  //시공단계가 없어도, Before Force까지 더한 부재력 사용
                {
                    _DGN_FORC_UL_MEMB MembFor_Pre;
                    if(!m_pForcCtrl->Get_BeforeForce4PreCompo_EX(UblEPairK, MembFor_Pre.ElemFor, nLcomNum)) return FALSE;
                    aDgnMembFor_Pre.SetAt(nUbl, MembFor_Pre);
                }
                aDgnMembFor.SetAt(nUbl, MembForD);
                aDgnMembFor_LTerm.SetAt(nUbl, MembForD_LTerm);

            }
        }

        nCurUblmK = nUblmK;
        switch (m_nDgnCode)
        {
        case CSA_S6_10_CSG:
        case CSA_S6_14_CSG: //CSA는 양단 Lateral Restraint 협의 
        case CSA_S6_19_CSG:
            MembLcomD.Memb.ChIf.b2SideLateralRestraint = TRUE;
            break;
        case NR_GN_CIV_025_06_CSG:
        case BS5400_3_00_CSG:
            MembLcomD.Memb.ChIf.enMembType = EN_Chk_Memb_Main_Girder;
            break;
        default:
            MembLcomD.Memb.ChIf.b2SideLateralRestraint = b2SideBgg;
            break;
        }
        
        MembLcomD.aUlmf.SetSize(m_nChkLcomSizeUlmf);        
        Get_UnbracedLengthData(ElemK, bGetKfac, KFacD, MembLcomD.Memb.Ulen);
        Get_CsgUlmSection(EPairI, EPairM, EPairJ, nPosI, nPosM, nPosJ, MembLcomD.Memb.Ulms);


		if ( !bCalcEngine )
		{
			MakeMemberResultData(ElemK, MembLcomD, MembR);
		}      


        //////////////////////////////////////////////////////////////////////////
        // 시공단계중 거더 검토

        MembLcomD.Memb.ChIf.dFactorCS = dFactorDeadCS;

        CStageInfo *pStageInfo = m_pDoc->m_pPostCtrl->GetStageInfo();
        CArray<T_STAG_K,T_STAG_K> aStagK;
        pStageInfo->GetResultSavedStagKeyList(aStagK);
        int nStageSize = aStagK.GetSize();
        CPG_CSIF_D NCStageD;
		if ( nStageSize == 0 && bIsPLCB == TRUE )
		{
			MembLcomD.Memb.ChIf.bIsPLCB = TRUE;
			Check_ConstructionNoStage(ElemK, nLcomK4CS, aUblmEPairK, MembLcomD, aDgnMembFor_Pre, MembR);
		}
		else
		{
			Check_ConstructionStage(ElemK, nLcomK4CS, aUblmEPairK, Cslf, MembLcomD, MembR, NCStageD);
		}

        MembLcomD.Memb.ChIf.bCantilever = bCantilever;

        // 시공중 거더 Force

        ADGNFORCE aPreForceLcom;
        if (bIsPLCB==TRUE)
        {
            m_pForcCtrl->Get_BeforeForce4PreCompo(ElemK, aPreForceLcom, nLcomNum);
        }

        ADGNFORCE aForceLcom;
        ADGNFORCE aForceTnpr;
        ADGNFORCE aForceTnsc;
        ADGNFORCE aForceLTerm;
        ADGNFORCE aForceSTerm;
        ADGNSHRSADD aStssLcom;

        if (!m_pForcCtrl->Get_LcomDataForDesign(ElemK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aForceSTerm, aStssLcom, nLcomNum)) return FALSE;

        int nForceLcomSize = aForceLcom.GetSize(); 

		int nFortype = 0;

		_DGN_STRS_CRC_ELEM StressLcom;
		ADGNSTRESS aStressTnpr;
		if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, StressLcom.aStressLcom, aStressTnpr, nLcomNum) ) { return false; }
		m_mStress[ElemK.first] = std::move(StressLcom);

		nFortype = 2;
		_DGN_STRS_CRC_ELEM StressLcomP2;
		ADGNSTRESS aStressTnprP2;
		if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, StressLcomP2.aStressLcom, aStressTnprP2, nLcomNum) ) { return false; }
		m_mStressP2[ElemK.first] = std::move(StressLcomP2);


        int nMcount[4]={0,};
        double dMmaxRat[4]={0.0,};

        int nVcount[2]={0,};
        double dVmaxRat[2]={0.0,};

        int nScount[4]={0,};
        double dSmaxRat[4]={0.0,};

        int nSCcount[2]={0,};
        double dSCmaxRat[2]={0.0,};
        double dSCmaxfsRat[2]={0.0,};

        int nBScount[2]={0,};
        double dBSmaxRat[2]={0.0,};


        T_CSBR_D CsbrResD;
        T_CSVR_D CsvrResD;
        T_CSAC_D CsacResD;
        T_CSFC_D CsfcResD;
        T_CSCR_D CscrResD;
        T_CSSL_D CsslResD;
        T_BSTF_D BstfResD;
        //
        T_CPBR_D CpbrResD;
        T_CPVR_D CpvrResD;
        T_CPAC_D CpacResD;
        T_CPFC_D CpfcResD;
        T_CASC_D CascResD;
        T_CPLS_D CplsResD;
        T_CPTS_D CptsResD;
        T_CPBS_D CpbsResD;

        CArray<T_STRB_D,T_STRB_D&> aStrbElem;
        CArray<T_STRB_D,T_STRB_D&> aStrbElem_LTerm;
        CArray<T_STRB_D,T_STRB_D&> aStrbElem_Pre;
        if (nUblElemSize>0) aStrbElem.SetSize(nUblElemSize);
        if (nUblElemSize>0) aStrbElem_LTerm.SetSize(nUblElemSize);
        if (nUblElemSize>0) aStrbElem_Pre.SetSize(nUblElemSize);

		T_COMPOSITE_STEEL_GIRDER_RES_D* pRes = new T_COMPOSITE_STEEL_GIRDER_RES_D;
		pRes->vLcomRes.resize(nForceLcomSize);
		m_vRes[ElemK] = pRes;

        int nDgnLcomNo = 0;
        int nOrgLcomNo = 0;
        int nSerLcomCount = 0;
        int nMaxMinType = 0;    
        CString strOrgLcomNa=_T("");

        int nServLcomType=0;

        T_STRB_D MembForNC; 

        for(int j=0; j<nForceLcomSize; ++j)
        {
            int nDgnLcomNo = j+1;
            if(!m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType))
            {
                ASSERT(0);
            }		   
			SetDgnLcomNo(nDgnLcomNo);

            _DGN_FORC_CRC ForceNC;
            m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, StctD, ElemK, NCStageD.StagK, NCStageD.nSerialStepK,  nDgnLcomNo, nForceBeforeType, ForceNC);

            MembForNC.Initialize();
            m_pForcCtrl->Get_GirderMembForce4CSLoadOfLCOM(FALSE, StctD, ElemK, NCStageD.StagK, NCStageD.nSerialStepK, nDgnLcomNo, nForceBeforeType, aUblmEPairK, &MembForNC);

            if ( bIsPLCB==TRUE )
            {
                ForceNC   = aPreForceLcom[j];
            }

            //_DGN_FORC_CRC &ForceLcom   = aForceLcom.GetAt(j);
            _DGN_FORC_CRC ForceST    = aForceLcom[j];
            _DGN_FORC_CRC ForceLT    = aForceLTerm[j];
            _DGN_FORC_CRC ForceSum   = aForceLcom[j];
            if ( bIsPLCB==TRUE )
            {
                ForceST -= ForceNC;
                ForceST -= ForceLT;
            }
            else
            {
                ForceST -= ForceLT;
                ForceLT -= ForceNC;
            }

            BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(nDgnLcomNo);
            BOOL bSLSLcom = m_pForcCtrl->Is_LcomForStrs(nDgnLcomNo);   
            BOOL bFLSLcom = m_pForcCtrl->Is_LcomForFati(nDgnLcomNo);
            if(bSLSLcom) nServLcomType = m_pForcCtrl->Get_ServLcomType(nDgnLcomNo);

            if(bULSLcom == TRUE)    MembLcomD.bIsLcomULS = TRUE;
            if(bSLSLcom == TRUE)    MembLcomD.bIsLcomSLS = TRUE;
            if(bFLSLcom == TRUE)    MembLcomD.bIsLcomFLS = TRUE;

			T_COMPOSITE_STEEL_GIRDER_RES_D* pRes = m_vRes[ElemK];
			pRes->vLcomRes[j].bULSLcom = bULSLcom ? true : false;
			pRes->vLcomRes[j].bSLSLcom = bSLSLcom ? true : false;
			pRes->vLcomRes[j].nDgnLcomNo = nDgnLcomNo;

            _DGN_LCOM DgnLcom;

            if (bFLSLcom) continue;

            MakeConvertCsgMemberForce(bIsPLCB, aUblmEPairK, j,
                aDgnMembFor, aDgnMembFor_LTerm, aDgnMembFor_Pre,
                MembForNC, aStrbElem, aStrbElem_LTerm, aStrbElem_Pre, MembLcomD);

            MembLcomD.aChkFor[0].Initialize();
            CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];
            if      (bULSLcom==TRUE) ComLcomF.nLcomType = 0;
            else if (bSLSLcom==TRUE) 
            {
                ComLcomF.nLcomType = 1;
                ComLcomF.nLcomSub  = nServLcomType;
            }      
            ComLcomF.nMax = nMaxMinType;


            for (int k=0; k<2; ++k) // I,J
            {
                if(bChkIJ[k]==FALSE) continue;
                MembLcomD.Memb.ChIf.nChkPos = k;
				SetCheckPosition(k);
                T_VSEC_K CurVsecK = k==0 ? VbemD.Vsec1 : VbemD.Vsec2;

                InitDgnResult(ChkResD);

                ConvertCsgCalcChkForce(k, ForceNC, ForceLT, ForceST, ForceSum, MembLcomD.aChkFor[0]);

                double dMuy = MembLcomD.aChkFor[0].ForLC.dForce[4]; // ???

				if ( bCalcEngine )
				{
					//thread 별로 현재 Lcom에 대한 정보를 들고 있어야 나중에 input converter에서 해당 lcom에 대한 data를 가져올수 있다.
					//m_ElemToLcom[std::this_thread::get_id()] = std::make_tuple(ElemK, k, iDgnLcomNo);
					SetLcomInfoResultStruct(MembLcomD, pRes->vLcomRes[j]);

					IDgnCodeCheck::Check_Section(false);
					continue;
				}

                // 반력 && Link Force 
                T_ELEM_D ElemD; ElemD.Initialize();
                m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD);

                T_LCOM_D LcomD; LcomD.Initialize();
                m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, nOrgLcomNo, LcomD); // Load Combi을 가져옴
                m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);                       // Load Combi를 셋팅시킴.

                T_REAC_D ReacMaxData;  ReacMaxData.Initialize();
                T_REAC_D ReacMinData;  ReacMinData.Initialize();
                if (ElemK.second == EN_EL_BEAM)
                {
                    m_pDoc->m_pPostCtrl->GetReacNew(ElemD.elnod[k], &ReacMaxData, &ReacMinData, NULL); // Load Combi에 대한 반력 
                }
                else if (ElemK.second == EN_EL_VBEAM)
                {
                    m_pDoc->m_pPostCtrl->GetReacNewVeam(CurVsecK, &ReacMaxData, &ReacMinData, NULL);
                }
                else ASSERT(0);
                

                double dFz = 0.0;
                if (nMaxMinType==0)
                {
                    dFz = ReacMaxData.dblReac[2]; //
                }
                else
                {
                    dFz = (nMaxMinType % 2 == 1) ? ReacMaxData.dblReac[2] : ReacMinData.dblReac[2];
                }
                //

                // Link Force 정보
                if(dFz==0.0)
                {
                    dFz =GetLinkForce(nOrgLcomNo, nMaxMinType, k, ElemD.elnod[k]);
                }
                dFz = fabs(dFz);


                CSG_CHK_RES_DTR_PARAM  DtrR;

                T_CSCH_REF RsltRef;

                // ULS
                if (bULSLcom==TRUE)
                {
                    BOOL bPositive = (dMuy>=0.0);

                    Check_Strength(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);
                    GetMy4Strength(ChkResD, dMuy);
                    double dRatio_F = DtrR.dDTR_F;
                    double dRatio_S = DtrR.dDTR_S;

                    BOOL bPosiM = (dMuy>=0.0);
                    int nIx = (dMuy>=0.0) ? k*2 : k*2+1; // nIx : 0 (i-positive flexure) , 1 (i-negative flexure), 2 (j-positive flexure), 3 (j-negative flexure)

                    RsltRef.pCsbrB = &CsbrResD.CsbrB[nIx];
                    RsltRef.pCsvrB = &CsvrResD.CsvrB[k];
                    //
                    RsltRef.pCpbrB = &CpbrResD.CpbrB[nIx];
                    RsltRef.pCpvrB = &CpvrResD.CpvrB[k];
                    RsltRef.pCptsB = &CptsResD.CptsB[k];

                    if (nMcount[nIx]==0)
                    { 
                        dMmaxRat[nIx] = dRatio_F;
                        SetCsbrBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);
                        ConvertCsgCsbrRes(bPosiM, MembR, ChkResD, MembLcomD, dRatio_F, RsltRef);
                    }
                    else
                    {
                        if(dMmaxRat[nIx]<dRatio_F)
                        { 
                            dMmaxRat[nIx] = dRatio_F;
                            SetCsbrBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);
                            ConvertCsgCsbrRes(bPosiM, MembR, ChkResD, MembLcomD, dRatio_F, RsltRef);
                        }
                    }
                    nMcount[nIx]++;

                    // Shear
                    if (CpgdD.bDesignParameter[1]==TRUE)
                    {
                        if (nVcount[k]==0)
                        { 
                            dVmaxRat[k] = dRatio_S;
                            SetCsvrBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);						
                            ConvertCsgCsvrRes(MembR, ChkResD, MembLcomD, dRatio_S, RsltRef);
                            ConvertCsgCptsResLcom(ChkResD, RsltRef); // CSA 사용
                        }
                        else
                        {
                            if(dVmaxRat[k]<dRatio_S)
                            { 
                                dVmaxRat[k] = dRatio_S;
                                SetCsvrBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);								
                                ConvertCsgCsvrRes(MembR, ChkResD, MembLcomD, dRatio_S, RsltRef);
                                ConvertCsgCptsResLcom(ChkResD, RsltRef); // CSA 사용
                            }              
                        }
                        nVcount[k]++;
                    }
                }

                // Serivice
                if (CpgdD.bDesignParameter[2]==TRUE && bSLSLcom==TRUE)
                {

                    Check_Service(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);
                    double dRatio_Ser = DtrR.dDTR_Ser;

                    int nIx = (dMuy>=0.0) ? k*2 : k*2+1;

                    RsltRef.pCsacB = &CsacResD.CsacB[nIx];
                    //
                    RsltRef.pCpacB = &CpacResD.CpacB[nIx];

                    if (nScount[nIx]==0)
                    { 
                        dSmaxRat[nIx] = dRatio_Ser;
                        SetCsacBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);
                        ConvertCsgCsacRes(MembR, ChkResD, MembLcomD, dRatio_Ser, RsltRef);
                    }
                    else
                    {
                        if(dSmaxRat[nIx]<dRatio_Ser)
                        { 
                            dSmaxRat[nIx] = dRatio_Ser;
                            SetCsacBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);
                            ConvertCsgCsacRes(MembR, ChkResD, MembLcomD, dRatio_Ser, RsltRef);
                        }
                    }
                    nScount[nIx]++;
                }

                if (CpgdD.bDesignParameter[5]==TRUE)
                {
                    // ***************
                    // Stiffener 
                    // ***************

                    RsltRef.pCsslB = &CsslResD.CsslB[k];
                    RsltRef.pCptsB = &CptsResD.CptsB[k];
                    RsltRef.pCplsB = &CplsResD.CplsB[k];
                    RsltRef.pCpbsB = &CpbsResD.CpbsB[k];

                    Check_Stiffener(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

                    double dRatio   = DtrR.dDTR_Sf;
                    double dRatiofs = DtrR.dDTR_Sf_fs;

                    if (nSCcount[k]==0)
                    { 
                        dSCmaxRat[k] = dRatio;
                        dSCmaxfsRat[k] = dRatiofs;  
                        SetCsslBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);							
                        ConvertCsgCsslMembRes(MembR, MembLcomD, dRatio, RsltRef);
                    }
                    else
                    {
                        BOOL bFlag   = dSCmaxRat[k]<dRatio;
                        BOOL bFlagfs = dSCmaxfsRat[k]<dRatiofs;
                        if(bFlag || bFlagfs)
                        { 
                            dSCmaxRat[k]   = bFlag ? dRatio : dSCmaxRat[k];
                            dSCmaxfsRat[k] = bFlagfs ? dRatiofs : dSCmaxfsRat[k];   
                            SetCsslBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);				
                            ConvertCsgCsslMembRes(MembR, MembLcomD, dRatio, RsltRef);
                        }
                    }
                    nSCcount[k]++;

                    // *******************
                    // Bearing Stiffener
                    // *******************

                    RsltRef.pBstfB = &BstfResD.BstfB[k];

                    double dBSRatio = 0.0;
                    if(MembLcomD.Memb.PosD[k].Trst.bUseStifBearing && dFz >0.0) // Bearing Stiffener 체크 && 반력이 있을 때만
                    {
                        if(IsSectTypeWith2Web(MembLcomD.Memb.PosD[k].Sect.SectInfo.nStype)) dFz /=2.0;
                        MembLcomD.aReac[0].dFz = dFz;
                        Check_BearingStiffener(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);
                        dBSRatio = DtrR.dDTR_Bs;

                        if (nBScount[k]==0)
                        {
                            dBSmaxRat[k] = dBSRatio;
                            SetBstfBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);		
                            SetCpbsBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);
                            ConvertCsgBstfMembRes(MembR, MembLcomD, RsltRef);
                        }
                        else
                        {
                            if(dBSmaxRat[k]<dBSRatio)
                            { 
                                dBSmaxRat[k] = dBSRatio;
                                SetBstfBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);							
                                SetCpbsBase(bChkIJ[k], nDgnLcomNo, nOrgLcomNo, nMaxMinType, RsltRef);
                                ConvertCsgBstfMembRes(MembR, MembLcomD, RsltRef);
                            }
                        }
                        nBScount[k]++;
                    }
                }

            } // I/J

        } // LCom

        // 피로검토, 마지막에..
		if ( !bCalcEngine )
		{
			Check_CSG_Fatigue(CpgdD, StctD, bIsPLCB, ElemK, bChkIJ, NCStageD, aUblmEPairK,
				MembLcomD, MembR,
				aForceLcom, aForceLTerm,
				aDgnMembFor, aDgnMembFor_LTerm, aDgnMembFor_Pre,
				aStrbElem, aStrbElem_LTerm, aStrbElem_Pre,
				ChkResD, CsfcResD, CscrResD, CpfcResD);


			// 하중조합 결과가 아닌 것은 한 번만. 
			for ( int k = 0; k < 2; ++k )
			{
				T_CSCH_REF RsltRef;

				RsltRef.pCscrB = &CscrResD.CscrB[k];
				RsltRef.pCascB = &CascResD.CascB[k];
				//
				RsltRef.pCsslB = &CsslResD.CsslB[k];
				RsltRef.pCplsB = &CplsResD.CplsB[k];
				//
				RsltRef.pCptsB = &CptsResD.CptsB[k];

				MembLcomD.Memb.ChIf.nChkPos = k;
				if ( bChkIJ[k] == FALSE ) continue;
				if ( CpgdD.bDesignParameter[5] == TRUE )
				{
					// 전단연결재
					double dRatio_p = 0.0;
					if ( IsSCMembChk() )
					{
						ConvertCsgCscrMembRes(MembR, MembLcomD, dRatio_p, RsltRef);
					}

					// 수평 Stiffner
					double dRatio_ls = 0.0;
					ConvertCsgCsslMembNoLcomRes(MembR, MembLcomD, CsbrResD, RsltRef);

					// Transverse Stiffener,
					// if (m_nDgnCode==CSA_S6_14_CSG || m_nDgnCode==CSA_S6_10_CSG)
					ConvertCsgCptsMembRes(MembR, MembLcomD, RsltRef);
				}
			}
		}

		if ( bCalcEngine )
		{
			CalcCriticalLcomResult();
			WriteResultData();
		}		


		// Write
		if (pCheckGirder != NULL)
		{
			if (IsCSACode(m_nDgnCode))
			{
				if (CpgdD.bDesignParameter[0] == TRUE) pCheckGirder->WriteBendingResistanceCSA_S6(ElemK, CpbrResD);
				if (CpgdD.bDesignParameter[1] == TRUE) pCheckGirder->WriteShearResistanceCSA_S6(ElemK, CpvrResD);
				if (CpgdD.bDesignParameter[2] == TRUE) pCheckGirder->WriteServiceLimitCSA_S6(ElemK, CpacResD);
				if (CpgdD.bDesignParameter[4] == TRUE) pCheckGirder->WriteFatigueLimitCSA_S6(ElemK, CpfcResD);
				if (CpgdD.bDesignParameter[5] == TRUE)
				{
					if (CpgdD.bDesignParameter[0] == FALSE)
						pCheckGirder->WriteBendingResistanceCSA_S6(ElemK, CpbrResD);
					if (CpgdD.bDesignParameter[1] == FALSE)
						pCheckGirder->WriteShearResistanceCSA_S6(ElemK, CpvrResD);

					pCheckGirder->WriteShearConnectorCSA_S6(ElemK, CascResD);
					pCheckGirder->WriteLongiStiffenerCSA_S6(ElemK, CplsResD);
					pCheckGirder->WriteTransStiffenerCSA_S6(ElemK, CptsResD);
					pCheckGirder->WriteBearingStiffenerCSA_S6(ElemK, CpbsResD);
				}
			}
			else
			{
				if (CpgdD.bDesignParameter[0] == TRUE) pCheckGirder->WriteBendingResistanceKSCE_LSD(ElemK, CsbrResD);
				if (CpgdD.bDesignParameter[1] == TRUE) pCheckGirder->WriteShearResistanceKSCE_LSD(ElemK, CsvrResD);
				if (CpgdD.bDesignParameter[2] == TRUE) pCheckGirder->WriteServiceLimitKSCE_LSD(ElemK, CsacResD);
				if (CpgdD.bDesignParameter[4] == TRUE) pCheckGirder->WriteFatigueLimitKSCE_LSD(ElemK, CsfcResD);
				if (CpgdD.bDesignParameter[5] == TRUE)
				{
					pCheckGirder->WriteShearConnectorKSCE_LSD(ElemK, CscrResD);
					pCheckGirder->WriteLongiStiffenerKSCE_LSD(ElemK, CsslResD);
					pCheckGirder->WriteBearingStiffener(ElemK, BstfResD);
				}
			}
		}

		ProDlg.Update_ProgressDlg(nProgress++, nChkElemSize);
    }  // nChkElemSize


	if ( !bCalcEngine )
	{
		Check_CriticalResultOfSpan();
	}

    if (MembR.pAASHTO != NULL) delete MembR.pAASHTO;
    if (MembR.pLSD    != NULL) delete MembR.pLSD;
    if (MembR.pCSA    != NULL) delete MembR.pCSA;

	if ( !bCalcEngine )
	{
		ClassDelDgnResult(ChkResD);
	}
    
	IDgnMemoizationLib::Instance()->DeleteMemo(IDgnMemoizationLib::EN_MEMO_PLASTIC_MOMENT_NA);

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_CSG_Fatigue(T_CPGD_D &CpgdD,
                                        T_STCT_D &StctD,
                                        BOOL bIsPLCB,
                                        ElemPairK ElemK,
                                        BOOL bChkIJ[],
                                        CPG_CSIF_D &NCStageD,
                                        ArrElemPairKey& aUblmElemK,
                                        CSG_MEMB_POSD_KSCE_LSD &MembLcomD,   
                                        CSG_MEMB_RESULT_D &MembRes,
                                        ADGNFORCE &aForceLcom,
                                        ADGNFORCE &aForceLTerm,
                                        ADGNMEMB_FORCE &aDgnMembFor,
                                        ADGNMEMB_FORCE &aDgnMembFor_LTerm,
                                        ADGNMEMB_FORCE &aDgnMembFor_Pre,
                                        CArray<T_STRB_D,T_STRB_D&> &aStrbElem,
                                        CArray<T_STRB_D,T_STRB_D&> &aStrbElem_LTerm,
                                        CArray<T_STRB_D,T_STRB_D&> &aStrbElem_Pre,
                                        CSG_CHECK_RESULT_CODES &ChkResD,
                                        T_CSFC_D &CsfcResD,
                                        T_CSCR_D &CscrResD,
                                        T_CPFC_D &CpfcResD)
{
    if (CpgdD.bDesignParameter[4]==FALSE && CpgdD.bDesignParameter[5]==FALSE) return FALSE;

    CArray<T_LCOM_K,T_LCOM_K> aFatiLcomK;
    m_pForcCtrl->m_aFatiLcomId.GetSortedKeyListforUintK(aFatiLcomK);

    double dMinVal = -1.0*pow(10,21);

    int nLoadTop[2]={0,0}; // 하중유발 
    double dLoadTopRat[2]={0.0, 0.0};

    int nLoadBot[2]={0,0};
    double dLoadBotRat[2]={0.0, 0.0};

    int nLoadStud[2]={0,0};
    double dLoadStudRat[2]={0.0, 0.0};

    int nFlex[2]={0,0};
    double dFlexRat[2]={0.0, 0.0};

    int nWebShear[2]={0,0};
    double dWebShearRat[2]={0.0, 0.0};  

    int nSCcount[2]={0,0};
    double dSCmaxRat[2]={0.0, 0.0};

    int nSCFati[2]={0,0};
    double dSCFatigueRat[2]={0.0, 0.0};


    CSG_FATI_CR_RSLT FatiCrResD;

    CSG_CHECK_RESULT_CODES ChkFatiResD; // CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D SconResAASHTO; 
    ClassNewDgnResult(ChkFatiResD);

    T_STRB_D MembForNC; 

    int nForceBeforeType=1;    
    int nFatiLcomSize = aFatiLcomK.GetSize();
    for (int i=0; i<nFatiLcomSize; ++i)
    {
        T_DGN_LCOM_ID FatiLcomId;
        T_LCOM_K nLcomK = aFatiLcomK[i];
        if (!m_pForcCtrl->m_aFatiLcomId.Lookup(aFatiLcomK[i], FatiLcomId)) continue;  

        int nFatiConcSize = FatiLcomId.aDgnLcomId.GetSize();
        MembLcomD.aChkFor.RemoveAll();
        MembLcomD.aUlmf.RemoveAll();
        MembLcomD.aChkFor.SetSize(nFatiConcSize);
        MembLcomD.aUlmf.SetSize(m_nChkLcomSizeUlmf);

        _DGN_FORC_CRC ForceNC;
        if (nFatiConcSize>0)
        {
            int nDgnLcomId = FatiLcomId.aDgnLcomId[0];
            m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, StctD, ElemK, NCStageD.StagK, NCStageD.nSerialStepK,  nDgnLcomId, nForceBeforeType, ForceNC);

            MembForNC.Initialize();
            m_pForcCtrl->Get_GirderMembForce4CSLoadOfLCOM(FALSE, StctD, ElemK, NCStageD.StagK, NCStageD.nSerialStepK, nDgnLcomId, nForceBeforeType, aUblmElemK, &MembForNC);
        }

        int nOrgLcomNo = 0;
        int nSerLcomCount = 0;
        int nMaxMinType = 0;    
        CString strOrgLcomNa=_T("");
        int nFatiLcomType = 0;

        CSGFORCOMP_ARRAY aChkForceI;
        aChkForceI.SetSize(nFatiConcSize);
        CSGFORCOMP_ARRAY aChkForceJ;
        aChkForceJ.SetSize(nFatiConcSize);
        for (int j=0; j<nFatiConcSize; ++j)
        {
            int nDgnLcomId = FatiLcomId.aDgnLcomId[j];

            if(!m_pForcCtrl->Get_LcomType(nDgnLcomId, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0);}

            int nLcomIndex = nDgnLcomId-1;
            _DGN_FORC_CRC ForceST  = aForceLcom[nLcomIndex];
            _DGN_FORC_CRC ForceLT  = aForceLTerm[nLcomIndex];
            _DGN_FORC_CRC ForceSum = aForceLcom[nLcomIndex];

            ForceST -= ForceLT;
            ForceLT -= ForceNC;

            MakeConvertCsgMemberForce(bIsPLCB, aUblmElemK, j, 
                                      aDgnMembFor, aDgnMembFor_LTerm, aDgnMembFor_Pre,
                                      MembForNC, aStrbElem, aStrbElem_LTerm, aStrbElem_Pre, MembLcomD);

            MembLcomD.aChkFor[j].Initialize();
            CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[j];
            aChkForceJ[j].nLcomType = aChkForceI[j].nLcomType = ComLcomF.nLcomType = 2;
            aChkForceJ[j].nLcomSub  = aChkForceI[j].nLcomSub  = ComLcomF.nLcomSub  = m_pForcCtrl->Get_FatigueType(nDgnLcomId);
            aChkForceJ[j].nMax      = aChkForceI[j].nMax      = ComLcomF.nMax      = nMaxMinType;
            nFatiLcomType = ComLcomF.nLcomSub;

            ConvertCsgCalcChkForce(0, ForceNC, ForceLT, ForceST, ForceSum, aChkForceI[j]);
            ConvertCsgCalcChkForce(1, ForceNC, ForceLT, ForceST, ForceSum, aChkForceJ[j]);
        }

        if (m_nDgnCode==CSA_S6_14_CSG || m_nDgnCode==CSA_S6_10_CSG || m_nDgnCode==CSA_S6_19_CSG)
        {
            T_LCOM_D LcomData;  LcomData.Initialize();
            m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, nLcomK, LcomData);
            int nFatiLcomCount = LcomData.aCombination.GetSize();
            BOOL bIsFatiMVLoad = FALSE;
            for(int ii=0; ii<nFatiLcomCount; ++ii)
            {
                if(LcomData.aCombination[ii].AnalType ==  D_LCOM_MOVING)
                    bIsFatiMVLoad = TRUE;
            }
            MembLcomD.LcomInf.aMVIn.RemoveAll();
            if(bIsFatiMVLoad)
                MembLcomD.LcomInf.aMVIn.SetSize(1);
        }

        int nFatigueII[2] = {0,0};
        int nFatigueII_SC[2] = {0,0};
        int nDgnLcomId=0; // 저장할 필요 없음?
        for (int k=0; k<2; ++k) // I,J
        {
            if(bChkIJ[k]==FALSE) continue;
            BOOL bPosI = (k==0);
            MembLcomD.Memb.ChIf.nChkPos = k;      

            //double dMuy = MembLcomD.aChkFor[0].ForLC.dForce[4]; // ???

            if (k==0) MembLcomD.aChkFor.Copy(aChkForceI);
            else      MembLcomD.aChkFor.Copy(aChkForceJ);

            CSG_CHK_RES_DTR_PARAM DtrR;

            T_CSCH_REF RsltRef; RsltRef.Initialize();

            RsltRef.pCsfcB = &CsfcResD.CsfcB[k];
            RsltRef.pCscrB = &CscrResD.CscrB[k];
            RsltRef.pCpfcB = &CpfcResD.CpfcB[k];

            Check_Fatigue(ElemK, k, MembLcomD, MembRes, ChkResD, DtrR, FatiCrResD);      

            if (IsChkFatigue2(nFatiLcomType, ChkResD)) nFatigueII[k]++;

            double dRati_Fat=0.0;      
            BOOL   bChkInducedLoad = TRUE;
            BOOL   bChkFatigueII = FALSE;

            double dMuy=0.0; 
            int nIx = (dMuy>=0.0) ? k*2 : k*2+1; // 정/부모멘트를 찾아야 하는가?. 아니면 전단처럼?..    
            int nFLoadType = 0;

            FindFatiCrRes4Lcom(nFLoadType, k, bChkIJ,
                nFatiLcomType, nDgnLcomId, nOrgLcomNo,
                MembLcomD, MembRes, nFatigueII,
                nLoadTop, nLoadBot,
                dLoadTopRat, dLoadBotRat,
                DtrR, ChkResD, RsltRef, FatiCrResD);


            //하중유발피로 StudBase CSA추가
            if (IsChkFatiLoadStud())
            {
                if (nLoadStud[k]==0)
                {
                    dLoadStudRat[k] = DtrR.dDTR_F_LS;
                    SetCpfcStudBase(bChkIJ[k], nDgnLcomId, nOrgLcomNo, 0, RsltRef);
                    ConvertCsgFatiStudRes(MembRes, ChkResD, MembLcomD, RsltRef);    //FlexR - StudBase로 사용
                }
                else
                {
                    if(dLoadStudRat[k]<DtrR.dDTR_F_LS)
                    { 
                        dLoadStudRat[k] = DtrR.dDTR_F_LS;
                        SetCpfcStudBase(bChkIJ[k], nDgnLcomId, nOrgLcomNo, 0, RsltRef);
                        ConvertCsgFatiStudRes(MembRes, ChkResD, MembLcomD, RsltRef);
                    }
                }
                nLoadStud[k]++;
            }

            // 변형유발피로 휨 
            if (IsChkFatiTranFlex())
            {
                if (nFlex[k]==0)
                { 
                    dFlexRat[k] = DtrR.dDTR_F_Flex;
                    SetCsfcFlexBase(bChkIJ[k], nDgnLcomId, nOrgLcomNo, ChkResD, RsltRef);
                    ConvertCsgFatiFlexRes(MembRes, ChkResD, MembLcomD, dFlexRat[k], RsltRef);
                }
                else
                {
                    if(dFlexRat[k]<DtrR.dDTR_F_Flex)
                    { 
                        dFlexRat[k] = DtrR.dDTR_F_Flex;;
                        SetCsfcFlexBase(bChkIJ[k], nDgnLcomId, nOrgLcomNo, ChkResD, RsltRef);
                        ConvertCsgFatiFlexRes(MembRes, ChkResD, MembLcomD, dFlexRat[k], RsltRef);
                    }
                }
                nFlex[k]++;

            }

            // 변형유발피로 Web 전단  
            if (nWebShear[k]==0)
            { 
                dWebShearRat[k] = DtrR.dDTR_F_WebShr;
                SetCsfcShearBase(bChkIJ[k], nDgnLcomId, nOrgLcomNo, ChkResD, RsltRef);
                ConvertCsgFatiShearRes(MembRes, ChkResD, MembLcomD, dWebShearRat[k],RsltRef);
            }
            else
            {
                if(dWebShearRat[k]<DtrR.dDTR_F_WebShr)
                { 
                    dWebShearRat[k] = DtrR.dDTR_F_WebShr;
                    SetCsfcShearBase(bChkIJ[k], nDgnLcomId, nOrgLcomNo, ChkResD, RsltRef);
                    ConvertCsgFatiShearRes(MembRes, ChkResD, MembLcomD, dWebShearRat[k],RsltRef);
                }
            }
            nWebShear[k]++;

            // Fatigue Shear connector
            FindFatiSCCrRes4Lcom(ElemK, k, bChkIJ,
                nFatiLcomType, nDgnLcomId, nOrgLcomNo, nMaxMinType, 
                MembLcomD, MembRes, 
                nSCcount, dSCmaxRat,
                DtrR, ChkResD, RsltRef);


            ChkFatiResD.Init();
            double dRati_Fat_SC=0.0;      
            bChkInducedLoad = TRUE;
            bChkFatigueII = FALSE;      

            FindFatiSCLoadCrRes4Lcom(ElemK, k, bChkIJ, 
                nFatiLcomType, nDgnLcomId, nOrgLcomNo, nMaxMinType,
                MembLcomD, MembRes, 
                nFatigueII_SC, nSCFati,
                dSCFatigueRat,
                DtrR, ChkResD, ChkFatiResD, RsltRef, FatiCrResD);

        }
    }

    DecisionFatigueRes(FatiCrResD, CsfcResD, CscrResD);

    if(nFatiLcomSize>0)
    {
		if ( CDBLib::IsCSGCodeLSD_KR(m_nDgnCode) )
        {
            for (int i=0; i<2; ++i)
            {
                CsfcResD.CsfcB[i].bChk  = bChkIJ[i];
                CsfcResD.CsfcB[i].nOK   = (CsfcResD.CsfcB[i].LoadR[0].nOK==1 && 
                    CsfcResD.CsfcB[i].LoadR[1].nOK==1 &&
                    CsfcResD.CsfcB[i].FlexR.nOK   ==1 &&
                    CsfcResD.CsfcB[i].ShearR.nOK  ==1 );
            }
        }
        else if (m_nDgnCode==CSA_S6_14_CSG || m_nDgnCode==CSA_S6_10_CSG || m_nDgnCode==CSA_S6_19_CSG)
        {
            for (int i=0; i<2; ++i)
            {
                CpfcResD.CpfcB[i].bChk  = bChkIJ[i];
                CpfcResD.CpfcB[i].nOK   = (CpfcResD.CpfcB[i].LoadR[0].nOK==1 && 
                    CpfcResD.CpfcB[i].LoadR[1].nOK==1 &&
                    CpfcResD.CpfcB[i].SConnR.nOK==1      );
            }
        }
        else
        {
            for (int i=0; i<2; ++i)
            {
                CsfcResD.CsfcB[i].bChk  = bChkIJ[i];
                CsfcResD.CsfcB[i].nOK   = (CsfcResD.CsfcB[i].LoadR[0].nOK==1 && 
                    CsfcResD.CsfcB[i].LoadR[1].nOK==1 &&
                    /*CsfcResD.CsfcB[i].FlexR.nOK   ==1 &&*/
                    CsfcResD.CsfcB[i].ShearR.nOK  ==1 );
            }
        }
    }

    ClassDelDgnResult(ChkFatiResD);

    return TRUE;
}

BOOL CDgnCsgDataCtrl::FindFatiCrRes4Lcom(int nFLoadType, int nIJ, BOOL *pbChkIJ, 
                                         int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo,
                                         CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                         CSG_MEMB_RESULT_D &MembRes,
                                         int *pnFatigueII,
                                         int *pnLoadTop, int *pnLoadBot,
                                         double *pdLoadTopRat, double *pdLoadBotRat,
                                         CSG_CHK_RES_DTR_PARAM &DtrR,
                                         CSG_CHECK_RESULT_CODES &ChkResD,
                                         T_CSCH_REF &RsltRef,
                                         CSG_FATI_CR_RSLT &FRes)
{
    if (nIJ<0 || nIJ>1) return FALSE;

    // 하중유발피로 Top flange
    int nTopBot = 0;
    if (pnLoadTop[nIJ]==0)
    { 
        pdLoadTopRat[nIJ] = DtrR.dDTR_FL[0];
        SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, RsltRef);
        ConvertCsgFatiLoadRes(nFLoadType, 0, MembRes, ChkResD, MembLcomD, pdLoadTopRat[nIJ], RsltRef);
    }
    else
    {
        if (IsChkFatigueCr(nFatiLcomType, pnFatigueII[nIJ]))
        {
            if(pdLoadTopRat[nIJ]<DtrR.dDTR_FL[0])
            { 
                pdLoadTopRat[nIJ] = DtrR.dDTR_FL[0];
                SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, RsltRef);
                ConvertCsgFatiLoadRes(nFLoadType, 0, MembRes, ChkResD, MembLcomD, pdLoadTopRat[nIJ], RsltRef);
            }
        }
    }
    pnLoadTop[nIJ]++;

    // 하중유발피로 Bottom flange
    nTopBot = 1;
    if (pnLoadBot[nIJ]==0)
    { 
        pdLoadBotRat[nIJ] = DtrR.dDTR_FL[1];
        SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, RsltRef);
        ConvertCsgFatiLoadRes(nFLoadType, 1, MembRes, ChkResD, MembLcomD, pdLoadBotRat[nIJ], RsltRef);
    }
    else
    {
        if (IsChkFatigueCr(nFatiLcomType, pnFatigueII[nIJ]))
        {
            if(pdLoadBotRat[nIJ]<DtrR.dDTR_FL[1])
            { 
                pdLoadBotRat[nIJ] = DtrR.dDTR_FL[1];
                SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, RsltRef);
                ConvertCsgFatiLoadRes(nFLoadType, 1, MembRes, ChkResD, MembLcomD, pdLoadBotRat[nIJ], RsltRef);
            }
        }
    }
    pnLoadBot[nIJ]++;

    return TRUE;
}

BOOL CDgnCsgDataCtrl::FindFatiSCCrRes4Lcom(ElemPairK ElemK, int nIJ, BOOL *pbChkIJ,
                                           int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo, int nMaxMinType,
                                           CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                           CSG_MEMB_RESULT_D &MembRes,
                                           int *pnSCcount,
                                           double *pdSCmaxRat,
                                           CSG_CHK_RES_DTR_PARAM &DtrR,
                                           CSG_CHECK_RESULT_CODES &ChkResD,
                                           T_CSCH_REF &RsltRef)
{
    double dRatio_p= DtrR.dDTR_F_SC_p; 
    if (pnSCcount[nIJ]==0)
    { 
        pdSCmaxRat[nIJ] = dRatio_p;
        ConvertCsgCscrRes(MembRes, ChkResD, MembLcomD, dRatio_p, RsltRef);              

        Check_ShearConnector(ElemK, nIJ, MembLcomD, MembRes, ChkResD, DtrR);
        if (IsSkipChkScon(nFatiLcomType, ChkResD)) return TRUE;
        SetCscrBase(pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nMaxMinType, RsltRef);							
        ConvertCsgCscrMembRes(MembRes, MembLcomD, dRatio_p, RsltRef);
    }
    else
    {
        if(pdSCmaxRat[nIJ]<dRatio_p)
        { 
            pdSCmaxRat[nIJ] = dRatio_p;
            ConvertCsgCscrRes(MembRes, ChkResD, MembLcomD, dRatio_p, RsltRef);              

            Check_ShearConnector(ElemK, nIJ, MembLcomD, MembRes, ChkResD, DtrR);
            if (IsSkipChkScon(nFatiLcomType, ChkResD)) return TRUE;
            SetCscrBase(pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nMaxMinType, RsltRef);								
            ConvertCsgCscrMembRes(MembRes, MembLcomD, dRatio_p, RsltRef);
        }
    }
    pnSCcount[nIJ]++;

    return TRUE;
}

BOOL CDgnCsgDataCtrl::FindFatiSCLoadCrRes4Lcom(ElemPairK ElemK, int nIJ, BOOL *pbChkIJ,
                                               int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo, int nMaxMinType,
                                               CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                               CSG_MEMB_RESULT_D &MembRes,
                                               int *pnFatigueII_SC,
                                               int *pnSCFati,
                                               double *pdSCFatigueRat,
                                               CSG_CHK_RES_DTR_PARAM &DtrR,
                                               CSG_CHECK_RESULT_CODES &ChkMaResD,
                                               CSG_CHECK_RESULT_CODES &ChkResD,                                               
                                               T_CSCH_REF &RsltRef,
                                               CSG_FATI_CR_RSLT &FRes)
{
    Check_ShearConnector(ElemK, nIJ, MembLcomD, MembRes, ChkResD, DtrR);

    if (IsChkFatigue2(nFatiLcomType, ChkResD)) pnFatigueII_SC[nIJ]++;

    int nTopBot = 0;
    int nFLoadType = 1;
    if (pnSCFati[nIJ]==0) 
    { 
        pdSCFatigueRat[nIJ] = DtrR.dDTR_F_SC_f;  //SconResAASHTO.SC_FatiD.Fat_LoadInduceD.df_fat_rat;

        SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, RsltRef);
        ConvertCsgFatiLoadRes(nFLoadType, 0, MembRes, ChkResD, MembLcomD, pdSCFatigueRat[nIJ], RsltRef);
    }
    else
    {
        //if (bChkInducedLoad==TRUE)  : False 라도 ratio에 따라 선택되도록 함.
        {
            BOOL bCheckFati = IsChkFatigueCr(nFatiLcomType, pnFatigueII_SC[nIJ]);
            if (bCheckFati==TRUE)
            {
                if(pdSCFatigueRat[nIJ]< DtrR.dDTR_F_SC_f)
                { 
                    pdSCFatigueRat[nIJ] = DtrR.dDTR_F_SC_f;
                    SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, RsltRef);
                    ConvertCsgFatiLoadRes(nFLoadType, 0, MembRes, ChkResD, MembLcomD, pdSCFatigueRat[nIJ], RsltRef);
                }
            }
        }
    }
    pnSCFati[nIJ]++;   

    return TRUE;
}

void CDgnCsgDataCtrl::DecisionFatigueRes(CSG_FATI_CR_RSLT &FRes, T_CSFC_D &CsfcResD, T_CSCR_D &CscrResD)
{

}

BOOL CDgnCsgDataCtrl::MakeConvertCsgMemberForce(BOOL bIsPLCB,
                                                ArrElemPairKey& aUblmElemK,
                                                int nLcomId,
                                                ADGNMEMB_FORCE &aDgnMembFor,
                                                ADGNMEMB_FORCE &aDgnMembFor_LT,
                                                ADGNMEMB_FORCE &aDgnMembFor_Pre,
                                                T_STRB_D &MembForNC,
                                                CArray<T_STRB_D,T_STRB_D&> &aStrbElem,
                                                CArray<T_STRB_D,T_STRB_D&> &aStrbElem_LT,
                                                CArray<T_STRB_D,T_STRB_D&> &aStrbElem_Pre,
                                                CSG_MEMB_POSD_KSCE_LSD &MembLcomD
                                                ) const
{
    double dMmax = 0.0;
    T_STRB_D MembForD; MembForD.Initialize();
    T_STRB_D MembForD_LTerm; MembForD_LTerm.Initialize();            
    T_STRB_D MembForD_Pre; MembForD_Pre.Initialize();

    for (INT_PTR nEl=0; nEl<aUblmElemK.GetSize(); ++nEl)
    {
        if (nLcomId >= aDgnMembFor[nEl].ElemFor.GetSize()) continue;
        m_pForcCtrl->ConvertDgnForceToStrb(aDgnMembFor[nEl].ElemFor[nLcomId], aStrbElem[nEl]);
        m_pForcCtrl->ConvertDgnForceToStrb(aDgnMembFor_LT[nEl].ElemFor[nLcomId], aStrbElem_LT[nEl]);
        if(bIsPLCB) m_pForcCtrl->ConvertDgnForceToStrb(aDgnMembFor_Pre[nEl].ElemFor[nLcomId], aStrbElem_Pre[nEl]);
    }
    DGN_MY_MAX_B MyMaxLcom;
    DGN_MY_MAX_B MyMaxLTerm;
    DGN_MY_MAX_B MyMaxPre;
    m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem, &MembForD, MyMaxLcom);  //Unbraced 내 요소들에 대해서 I, 1/4, 1/2, 3/4, J 구간의 부재력을 가져옴(PartI, Part2, Part3, Part4)
    m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_LT, &MembForD_LTerm, MyMaxLTerm);
    if(bIsPLCB) m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_Pre, &MembForD_Pre, MyMaxPre);

    ElemPairK EMymaxPairK(MyMaxLcom.ElemK, aUblmElemK[0].second);
    //Get_CsgUlmsData4MyMax(EMymaxPairK, MyMaxLcom.nPosIJ, MembLcomD.Memb.Ulms);

    if (MembForNC.nDOFType==2)
    {
        MembForD.nDOFType       = MembForNC.nDOFType;
        MembForD_LTerm.nDOFType = MembForNC.nDOFType;
        MembForD_Pre.nDOFType   = MembForNC.nDOFType;
    }

    T_STRB_D MembForD_STerm = MembForD;
    T_STRB_D MembFor_LT_tp  = MembForD_LTerm;
    MembFor_LT_tp.Mult(-1.0);
    MembForD_STerm.Add(MembFor_LT_tp);

    T_STRB_D MembForNC_tp = MembForNC;
    MembForNC_tp.Mult(-1.0);
    MembForD_LTerm.Add(MembForNC_tp);  // 완성계 LT에서 NC 빼줌.


    T_STRB_D MembNCForD = bIsPLCB==TRUE ? MembForD_Pre : MembForNC;

    ConvertCsgCalcUblMForce(0, MembForD, MembLcomD.aUlmFor[0].ForD[1].ForLC);   //MembForD의 Part1의 J단을 Ma로 저장
    ConvertCsgCalcUblMForce(1, MembForD, MembLcomD.aUlmFor[0].ForD[2].ForLC);   //MembForD의 Part2의 J단을 Mb로 저장
    ConvertCsgCalcUblMForce(2, MembForD, MembLcomD.aUlmFor[0].ForD[3].ForLC);   //MembForD의 Part3의 J단을 Mc로 저장
    MembLcomD.aUlmFor[0].dMmax = m_pForcCtrl->GetMaxMyByMembForce(&MembForD);
    int nMid = 0;
    int nCom = 0;
    GetUlmMidIndex(MembForD, nMid, nCom);
    ConvertCsgCalcUlmForce(MembForD,       MembLcomD.aUlmf[0]);   //LC (NC+LT+ST)
    ConvertCsgCalcUlmForce(MembNCForD,     MembLcomD.aUlmf[1]);   //NC (_T("Dead Load")의한 부재력 + Erection Dead Before부재력)
    ConvertCsgCalcUlmForce(MembForD_LTerm, MembLcomD.aUlmf[2]);   //LT
    ConvertCsgCalcUlmForce(MembForD_STerm, MembLcomD.aUlmf[3]);   //ST

    ConvertCsgCalcUlmForceCb(MembForD, nMid, nCom, MembLcomD.aUlmf[0]);   //LC (NC+LT+ST)
    ConvertCsgCalcUlmForceCb(MembNCForD, nMid, nCom, MembLcomD.aUlmf[1]);   //NC (_T("Dead Load")의한 부재력 + Erection Dead Before부재력)
    ConvertCsgCalcUlmForceCb(MembForD_LTerm, nMid, nCom, MembLcomD.aUlmf[2]);   //LT
    ConvertCsgCalcUlmForceCb(MembForD_STerm, nMid, nCom, MembLcomD.aUlmf[3]);   //ST


    MembLcomD.Memb.Ulms.nElemK4Mymax = MyMaxLcom.ElemK;
    MembLcomD.Memb.Ulms.nSectK4Mymax = MyMaxLcom.SectK;
    MembLcomD.Memb.Ulms.nPos4Mymax = MyMaxLcom.nPos5;

    int nMyMaxId = 0;
    for (int m = 0; m < aUblmElemK.GetSize(); m++)
    {
        if (aUblmElemK[m].first == MyMaxLcom.ElemK)
        {
            break;
        }
        nMyMaxId++;
    }
        
    //int nForceBeforeType = 1;
    //_DGN_FORC_CRC_EX ForceNC;
    //m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM_EX(FALSE, StctD, EMymaxPairK, CsifD.StagK, CsifD.nSerialStepK, nLcomId, nForceBeforeType, ForceNC);
    //ConvertCsgCalcUblMForce4Pos(MyMaxLcom.nPos5, aStrbElem[nMyMaxId], MembLcomD.aUlmf[0].MyForce);
    //ConvertCsgCalcForce(MyMaxLcom.nPos5, ForceNC, MembLcomD.aUlmf[1].MyForce);
    //ConvertCsgCalcUblMForce4Pos(MyMaxLcom.nPos5, aStrbElem_LT[nMyMaxId], MembLcomD.aUlmf[2].MyForce);
    //ConvertCsgCalcUblMForce4Pos(MyMaxLcom.nPos5, aStrbElem_ST[nMyMaxId], MembLcomD.aUlmf[3].MyForce);
    //ConvertCsgCalcUblMForce4Pos(MyMaxLcom.nPos5, aStrbElem_PV[nMyMaxId], MembLcomD.aUlmf[4].MyForce);
    //ConvertCsgCalcUblMForce4Pos(MyMaxLcom.nPos5, aStrbElem_SV[nMyMaxId], MembLcomD.aUlmf[5].MyForce);

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_CriticalResultOfSpan()
{
    CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
    m_pDoc->m_pAttrCtrl->GetSpanKeyList(aSpanKeyList);
	Check_CriticalResultOfSpanBase(aSpanKeyList, EN_EL_BEAM);
    //
    CArray<T_SPAV_K, T_SPAV_K> aSpavKeyList;
    m_pDoc->m_pAttrCtrl->GetSpavKeyList(aSpavKeyList);
    Check_CriticalResultOfSpanBase(aSpavKeyList, EN_EL_VBEAM);

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_CriticalResultOfSpanBase(const CArray<T_SPAN_K, T_SPAN_K>& aSpanKeyList, const EN_BEAM_DGN_TYPE& enBeamType)
{
	if (aSpanKeyList.GetSize() == 0) return FALSE;

    CPlateGirderDesign* pCheckGirder = m_pDoc->m_pPostCtrl->GetPlateGirder();
    if (pCheckGirder == NULL) return FALSE;

    T_SPAN_K TotalSubSpanK=0;
    int nSpanKeySize = aSpanKeyList.GetSize();  
    T_SPAN_D SpanD; 
    for(int i=0; i<nSpanKeySize; ++i)
    {    
        SpanD.Initialize();
        T_SPAN_K SpanK = aSpanKeyList[i];
        // SubSpan key
        int nSubSpanNum = 0;
        if (enBeamType == EN_EL_BEAM)
        {
            m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);
            nSubSpanNum = m_pDoc->m_pAttrCtrl->GetCountSubSpan(SpanK);
        }
        else if (enBeamType == EN_EL_VBEAM)
        {
            m_pDoc->m_pAttrCtrl->GetSpav(SpanK, SpanD);
            nSubSpanNum = m_pDoc->m_pAttrCtrl->GetCountSubSpav(SpanK);
        }
        else ASSERT(0);
                
        int nSpanElemNum = SpanD.aElemBase.GetSize();    
        int nSpanLenNum  = SpanD.aSpan.GetSize();
        
        //
        double dSpanLengthFromSupport = 0.0;
        double dSubSpanStart = 0.0;
        T_SPAN_K SubSpanK=0;
        for(int j=0; j<nSubSpanNum; ++j)
        {
            SubSpanK      += 1;
            TotalSubSpanK += 1;
			SpanPairK SPairK(TotalSubSpanK, enBeamType);
            ElemPairK nMpme(0, 0), nMnme(0, 0);
            ElemPairK nVme(0, 0);
            ElemPairK nMp_cs(0, 0), nMn_cs(0, 0);
            ElemPairK nV_cs(0, 0);

            ElemPairK nSpme(0, 0), nSnme(0, 0);
            ElemPairK nFpme(0, 0), nFnme(0, 0);
            ElemPairK nSCme(0, 0);
            ElemPairK nLSme(0, 0);

            int nPosMp=0, nPosMn=0;
            int nPosV=0;
            int nPosMp_cs=0, nPosMn_cs=0;
            int nPosV_cs=0;

            int nPosSp=0, nPosSn=0;
            int nPosFp=0, nPosFn=0;
            int nPosSC=0;
            int nPosLS=0;

            double dMpmr=0.0, dMnmr=0.0;
            double dVmr=0.0;
            double dMp_cs=0.0, dMn_cs=0.0;
            double dV_cs=0.0;

            double dSpmr=0.0, dSnmr=0.0;
            double dFpmr=0.0, dFnmr=0.0;
            double dSCmr=0.0;
            double dLSmr=0.0;

            int nMpcount=0, nMncount=0;
            int nVcount=0;
            int nMpcount_cs=0, nMncount_cs=0;
            int nVcount_cs=0;
            int nSpcount=0, nSncount=0;
            int nFpcount=0, nFncount=0;
            int nSCcount=0;
            int nLScount=0;

            double dSubSpanLength = j<nSpanLenNum ? SpanD.aSpan[j] : 0.0;
            dSpanLengthFromSupport += dSubSpanLength;
            double dLengthFromSupport = 0.0;
            for(int k=0; k<nSpanElemNum; ++k)
            {    
                T_SPAN_BASE &SpanBase = SpanD.aElemBase[k];
                ElemPairK EPairK(SpanBase.ElemK, enBeamType);
                double dElemLength = m_pDataCtrl->GetLengthOfBeam(EPairK);

                dLengthFromSupport += dElemLength;
                BOOL bInSpan = (dSubSpanStart<dLengthFromSupport &&  dLengthFromSupport<dSpanLengthFromSupport+m_dZero);
                if ( !bInSpan ) continue;

                //T_SPAN_K SubSpanKTemp;
                //m_mElemSubSpanK.Lookup(EPairK.first, SubSpanKTemp);
                //if(SubSpanKTemp!=TotalSubSpanK) continue;

                T_CSBR_D CsbrD;
                T_CSVR_D CsvrD;
                T_CSAC_D CsacD;
                T_CSFC_D CsfcD;
                T_CSCR_D CscrD;      

                // 휨
                Get_CriticalCsbr(pCheckGirder, EPairK, nMpcount, nMncount, nMpme, nMnme, nPosMp, nPosMn, dMpmr, dMnmr);
                // 전단
                Get_CriticalCsvr(pCheckGirder, EPairK, nVcount, nVme, nPosV, dVmr);
                // 사용성
                Get_CriticalCsac(pCheckGirder, EPairK, nSpcount, nSncount, nSpme, nSnme, nPosSp, nPosSn, dSpmr, dSnmr);
                // 피로
                Get_CriticalCsfc(pCheckGirder, EPairK, nFpcount, nFncount, nFpme, nFnme, nPosFp, nPosFn, dFpmr, dFnmr);
                // 전단연결재
                Get_CriticalCscr(pCheckGirder, EPairK, nSCcount, nSCme, nPosSC, dSCmr);
                // 수평 보강재
                Get_CriticalCssl(pCheckGirder, EPairK, nLScount, nLSme, nPosLS, dLSmr);
                // 시공(휨)
                Get_CriticalScbr(pCheckGirder, EPairK, nMpcount_cs, nMncount_cs, nMp_cs, nMn_cs, nPosMp_cs, nPosMn_cs, dMp_cs, dMn_cs);
                // 시공(전단)
                Get_CriticalScvr(pCheckGirder, EPairK, nVcount_cs, nV_cs, nPosV_cs, dV_cs);
            }

            dSubSpanStart += dSubSpanLength;

            T_CSPR_D CsprD;    
            CsprD.OrgSpanK = SpanK;
            CsprD.SubSpanK = SubSpanK;

            // 휨
            CsprD.Flex[0].ElemK = nMpme.first;
            CsprD.Flex[0].nPos  = nPosMp;
            CsprD.Flex[0].nBeamType = nMpme.second;

            CsprD.Flex[1].ElemK = nMnme.first;
            CsprD.Flex[1].nPos  = nPosMn;
            CsprD.Flex[1].nBeamType = nMnme.second;
            // 전단
            CsprD.Shear.ElemK   = nVme.first;
            CsprD.Shear.nPos    = nPosV;
            CsprD.Shear.nBeamType = nVme.second;

            // 시공(휨)
            CsprD.ConF[0].ElemK = nMp_cs.first;
            CsprD.ConF[0].nPos  = nPosMp_cs;
            CsprD.ConF[0].nBeamType = nMp_cs.second;
            CsprD.ConF[1].ElemK = nMn_cs.first;
            CsprD.ConF[1].nPos  = nPosMn_cs;
            CsprD.ConF[1].nBeamType = nMn_cs.second;
            // 시공(전단)
            CsprD.ConS.ElemK   = nV_cs.first;
            CsprD.ConS.nPos    = nPosV_cs;
            CsprD.ConS.nBeamType = nV_cs.second;

            // SLS
            CsprD.Serv[0].ElemK = nSpme.first;
            CsprD.Serv[0].nPos  = nPosSp;
            CsprD.Serv[0].nBeamType = nSpme.second;
            CsprD.Serv[1].ElemK = nSnme.first;
            CsprD.Serv[1].nPos  = nPosSn;
            CsprD.Serv[1].nBeamType = nSnme.second;
            // Fatigue
            CsprD.Fati[0].ElemK = nFpme.first;
            CsprD.Fati[0].nPos  = nPosFp;
            CsprD.Fati[0].nBeamType = nFpme.second;
            CsprD.Fati[1].ElemK = nFnme.first;
            CsprD.Fati[1].nPos  = nPosFn;
            CsprD.Fati[1].nBeamType = nFnme.second;
            // Shear connector
            CsprD.SCon.ElemK    = nSCme.first;
            CsprD.SCon.nPos     = nPosSC;
            CsprD.SCon.nBeamType = nSCme.second;
            // Long. Stiffener
            CsprD.LStfn.ElemK   = nLSme.first;
            CsprD.LStfn.nPos    = nPosLS;
            CsprD.LStfn.nBeamType = nLSme.second;

            pCheckGirder->WriteCheckSpanKSCE_LSD(SPairK, CsprD);

        } // Sub Span

    } //Span

    return TRUE;
}

void CDgnCsgDataCtrl::Get_CriticalCsbr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nMpcount, int &nMncount, ElemPairK &nMpme, ElemPairK &nMnme,
                                       int &nPosMp, int &nPosMn, double &dMpmr, double &dMnmr)
{
    if (pGirder==NULL) return;

    T_CSBR_D CsbrD;
    if (pGirder->ReadCSBR_LSD12(ElemK, CsbrD))
    {
        for (int k=0; k<4; ++k)
        {
            T_CSBR_BASE &CsbrB = CsbrD.CsbrB[k];        
            if (CsbrB.bChk)
            {
                if (k==0 || k==2) // Positive M
                {
                    nMpcount++;
                    if (nMpcount==1)
                    {
                        nMpme  = ElemK;
                        dMpmr  = CsbrB.dMrat;
                        nPosMp = (k==0) ? 0 : 1;
                    }
                    else
                    {
                        if (dMpmr<CsbrB.dMrat)
                        {
                            nMpme  = ElemK;
                            dMpmr  = CsbrB.dMrat;
                            nPosMp = (k==0) ? 0 : 1;
                        }
                    }
                }
                else 
                {
                    nMncount++;
                    if (nMncount==1)
                    {
                        nMnme  = ElemK;
                        dMnmr  = CsbrB.dMrat;
                        nPosMn = (k==1) ? 0 : 1;
                    }
                    else
                    {
                        if (dMnmr<CsbrB.dMrat)
                        {
                            nMnme  = ElemK;
                            dMnmr  = CsbrB.dMrat;
                            nPosMn = (k==1) ? 0 : 1;
                        }
                    }
                }
            }
        }
    }
}

void CDgnCsgDataCtrl::Get_CriticalCsvr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nVcount, ElemPairK &nVme, int &nPosV, double &dVmr)
{
    if (pGirder==NULL) return;

    T_CSVR_D CsvrD;
    if (pGirder->ReadCSVR_LSD12(ElemK, CsvrD))
    {
        for (int k=0; k<2; ++k)
        {
            T_CSVR_BASE &CsvrB = CsvrD.CsvrB[k];        
            if (CsvrB.bChk)
            {
                if (k==0) // I
                {
                    nVcount++;
                    if (nVcount==1)
                    {
                        nVme  = ElemK;
                        dVmr  = CsvrB.dVrat;
                        nPosV = k;
                    }
                    else
                    {
                        if (dVmr<CsvrB.dVrat)
                        {
                            nVme  = ElemK;
                            dVmr  = CsvrB.dVrat;
                            nPosV = k;
                        }
                    }
                }
                else 
                {
                    nVcount++;
                    if (nVcount==1)
                    {
                        nVme  = ElemK;
                        dVmr  = CsvrB.dVrat;
                        nPosV = k;
                    }
                    else
                    {
                        if (dVmr<CsvrB.dVrat)
                        {
                            nVme  = ElemK;
                            dVmr  = CsvrB.dVrat;
                            nPosV = k;
                        }
                    }
                }
            }
        }
    }
}


void CDgnCsgDataCtrl::Get_CriticalScbr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nMpcount, int &nMncount, ElemPairK &nMpme, ElemPairK &nMnme,
                                       int &nPosMp, int &nPosMn, double &dMpmr, double &dMnmr)
{
    if (pGirder==NULL) return;

    T_SCBR_D ScbrD;
    if (pGirder->ReadSCBR_LSD12(ElemK, ScbrD))
    {
        for (int k=0; k<4; ++k)
        {
            T_SCBR_BASE &ScbrB = ScbrD.ScbrB[k];        
            if (ScbrB.bChk)
            {
                if (k==0 || k==2) // Positive M
                {
                    nMpcount++;
                    if (nMpcount==1)
                    {
                        nMpme  = ElemK;
                        dMpmr  = ScbrB.dRatio;
                        nPosMp = (k==0) ? 0 : 1;
                    }
                    else
                    {
                        if (dMpmr<ScbrB.dRatio)
                        {
                            nMpme  = ElemK;
                            dMpmr  = ScbrB.dRatio;
                            nPosMp = (k==0) ? 0 : 1;
                        }
                    }
                }
                else 
                {
                    nMncount++;
                    if (nMncount==1)
                    {
                        nMnme  = ElemK;
                        dMnmr  = ScbrB.dRatio;
                        nPosMn = (k==1) ? 0 : 1;
                    }
                    else
                    {
                        if (dMnmr<ScbrB.dRatio)
                        {
                            nMnme  = ElemK;
                            dMnmr  = ScbrB.dRatio;
                            nPosMn = (k==1) ? 0 : 1;
                        }
                    }
                }
            }
        }
    }
}

void CDgnCsgDataCtrl::Get_CriticalScvr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nVcount, ElemPairK &nVme, int &nPosV, double &dVmr)
{
    if (pGirder==NULL) return;

    T_SCVR_D ScvrD;
    if (pGirder->ReadSCVR_LSD12(ElemK, ScvrD))
    {
        for (int k=0; k<2; ++k)
        {
            T_SCVR_BASE &ScvrB = ScvrD.ScvrB[k];        
            if (ScvrB.bChk)
            {
                if (k==0) // I
                {
                    nVcount++;
                    if (nVcount==1)
                    {
                        nVme  = ElemK;
                        dVmr  = ScvrB.dRatio;
                        nPosV = k;
                    }
                    else
                    {
                        if (dVmr<ScvrB.dRatio)
                        {
                            nVme  = ElemK;
                            dVmr  = ScvrB.dRatio;
                            nPosV = k;
                        }
                    }
                }
                else 
                {
                    nVcount++;
                    if (nVcount==1)
                    {
                        nVme  = ElemK;
                        dVmr  = ScvrB.dRatio;
                        nPosV = k;
                    }
                    else
                    {
                        if (dVmr<ScvrB.dRatio)
                        {
                            nVme  = ElemK;
                            dVmr  = ScvrB.dRatio;
                            nPosV = k;
                        }
                    }
                }
            }
        }
    }
}

void CDgnCsgDataCtrl::Get_CriticalCsac(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nSpcount, int &nSncount, ElemPairK &nSpme, ElemPairK &nSnme,
                                       int &nPosSp, int &nPosSn, double &dSpmr, double &dSnmr)
{
    if (pGirder==NULL) return;

    T_CSAC_D CsacD;
    if (pGirder->ReadCSAC_LSD12(ElemK, CsacD))
    {
        for (int k=0; k<4; ++k)
        {
            T_CSAC_BASE &CsacB = CsacD.CsacB[k];
            if (CsacB.bChk)
            {
                if (k==0 || k==2) // Positive M
                {
                    nSpcount++;
                    if (nSpcount==1)
                    {
                        nSpme  = ElemK;
                        dSpmr  = CsacB.dfrat;
                        nPosSp = (k==0) ? 0 : 1;
                    }
                    else
                    {
                        if (dSpmr<CsacB.dfrat)
                        {
                            nSpme  = ElemK;
                            dSpmr  = CsacB.dfrat;
                            nPosSp = (k==0) ? 0 : 1;
                        }
                    }
                }
                else 
                {
                    nSncount++;
                    if (nSncount==1)
                    {
                        nSnme = ElemK;
                        dSnmr = CsacB.dfrat;
                        nPosSn = (k==1) ? 0 : 1;
                    }
                    else
                    {
                        if (dSnmr<CsacB.dfrat)
                        {
                            nSnme = ElemK;
                            dSnmr = CsacB.dfrat;
                            nPosSn = (k==1) ? 0 : 1;
                        }
                    }
                }
            }
        }
    }
}

void CDgnCsgDataCtrl::Get_CriticalCsfc(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nFpcount, int &nFncount, ElemPairK &nFpme, ElemPairK &nFnme,
                                       int &nPosFp, int &nPosFn, double &dFpmr, double &dFnmr)
{
    if (pGirder==NULL) return;

    T_CSFC_D CsfcD;
    if (pGirder->ReadCSFC_LSD12(ElemK, CsfcD))
    {
        for (int k=0; k<4; ++k)
        {
            T_CSFC_BASE &CsfcB = CsfcD.CsfcB[k];        
            if (CsfcB.bChk)
            {
                if (k==0 || k==2) // Positive M
                {
                    nFpcount++;
                    if (nFpcount==1)
                    {
                        nFpme  = ElemK;
                        dFpmr  = CsfcB.dFrat;
                        nPosFp = (k==0) ? 0 : 1;
                    }
                    else
                    {
                        if (dFpmr<CsfcB.dFrat)
                        {
                            nFpme  = ElemK;
                            dFpmr  = CsfcB.dFrat;
                            nPosFp = (k==0) ? 0 : 1;
                        }
                    }
                }
                else 
                {
                    nFncount++;
                    if (nFncount==1)
                    {
                        nFnme  = ElemK;
                        dFnmr  = CsfcB.dFrat;
                        nPosFn = (k==1) ? 0 : 1;
                    }
                    else
                    {
                        if (dFnmr<CsfcB.dFrat)
                        {
                            nFnme  = ElemK;
                            dFnmr  = CsfcB.dFrat;
                            nPosFn = (k==1) ? 0 : 1;
                        }
                    }
                }
            }
        }
    }
}

void CDgnCsgDataCtrl::Get_CriticalCscr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nSCcount, ElemPairK &nSCme, int &nPosSC, double &dSCmr)
{
    if (pGirder==NULL) return;

    T_CSCR_D CscrD;
    if (pGirder->ReadCSCR_LSD12(ElemK, CscrD))
    {
        for (int k=0; k<2; ++k)
        {
            T_CSCR_BASE &CscrB = CscrD.CscrB[k];        
            if (CscrB.bChk)
            {
                if (k==0) // I
                {
                    nSCcount++;
                    if (nSCcount==1)
                    {
                        nSCme  = ElemK;
                        dSCmr  = CscrB.dSCrat;
                        nPosSC = k;
                    }
                    else
                    {
                        if (dSCmr<CscrB.dSCrat)
                        {
                            nSCme  = ElemK;
                            dSCmr  = CscrB.dSCrat;
                            nPosSC = k;
                        }
                    }
                }
                else 
                {
                    nSCcount++;
                    if (nSCcount==1)
                    {
                        nSCme  = ElemK;
                        dSCmr  = CscrB.dSCrat;
                        nPosSC = k;
                    }
                    else
                    {
                        if (dSCmr<CscrB.dSCrat)
                        {
                            nSCme  = ElemK;
                            dSCmr  = CscrB.dSCrat;
                            nPosSC = k;
                        }
                    }
                }
            }
        }
    }
}

void CDgnCsgDataCtrl::Get_CriticalCssl(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nLScount, ElemPairK &nLSme, int &nPosLS, double &dLSmr)
{
    if (pGirder==NULL) return;

    T_CSSL_D CsslD;
    if (pGirder->ReadCSSL_LSD12(ElemK, CsslD))
    {
        for (int k=0; k<2; ++k)
        {
            T_CSSL_BASE &CsslB = CsslD.CsslB[k];        
            if (CsslB.bChk)
            {
                if (k==0) // I
                {
                    nLScount++;
                    if (nLScount==1)
                    {
                        nLSme  = ElemK;
                        dLSmr  = CsslB.dSFrat;
                        nPosLS = k;
                    }
                    else
                    {
                        if (dLSmr<CsslB.dSFrat)
                        {
                            nLSme  = ElemK;
                            dLSmr  = CsslB.dSFrat;
                            nPosLS = k;
                        }
                    }
                }
                else 
                {
                    nLScount++;
                    if (nLScount==1)
                    {
                        nLSme  = ElemK;
                        dLSmr  = CsslB.dSFrat;
                        nPosLS = k;
                    }
                    else
                    {
                        if (dLSmr<CsslB.dSFrat)
                        {
                            nLSme  = ElemK;
                            dLSmr  = CsslB.dSFrat;
                            nPosLS = k;
                        }
                    }
                }
            }
        }
    }
}

BOOL CDgnCsgDataCtrl::Check_ConstructionNoStage(ElemPairK ElemK, int nLcomK4CS,
                                                ArrElemPairKey& aUblmElemK,
                                                CSG_MEMB_POSD_KSCE_LSD &MembLcomD, 
                                                ADGNMEMB_FORCE &aMembForPre,
                                                CSG_MEMB_RESULT_D &rMembR)
{

    CPlateGirderDesign *pCheckGirder = m_pDoc->m_pPostCtrl->GetPlateGirder();


    BOOL bChkIJ[2]={FALSE};
    bChkIJ[0] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 1);
    bChkIJ[1] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 2);

    CSG_CHECK_RESULT_CODES ChkConsR;
    ClassNewDgnResult(ChkConsR);

    int nMcount[4]={0};
    double dMmaxRat[4]={0.0};

    int nVcount[2]={0};
    double dVmaxRat[2]={0.0};

    int nScount[4]={0};
    double dSmaxRat[4]={0.0};

    T_ELEM_D ElemD;
    T_SECT_D SectD;
    m_pDoc->m_pAttrCtrl->GetElem(ElemK.first,ElemD);	  
    m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro,SectD);

    int nSType = GetSectType(SectD);

    T_SCBR_D ScbrR;
    T_SCVR_D ScvrR;
    T_CSAC_D CsacR;

    T_CCBR_D CcbrR;
    T_CCVR_D CcvrR;

    int nUblElemSize = aUblmElemK.GetSize();

    CArray<T_STRB_D,T_STRB_D&> aStrbElem_Pre;
    if (nUblElemSize>0) aStrbElem_Pre.SetSize(nUblElemSize);

    int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
    ADGNFORCE aPreForceLcom;
    m_pForcCtrl->Get_BeforeForce4PreCompo(ElemK, aPreForceLcom, nLcomNum);

    int nLcomSise = aPreForceLcom.GetSize();

    double dFactorCS = 1.0; //MembLcomD.Memb.ChIf.dFactorCS;
    _DGN_FORC_CRC ForceNC;
    for(int i=0; i< nLcomSise; ++i)
    {
        ForceNC = aPreForceLcom[i];

        // Span을 구성하는 I,M,J Force 
        double dMmax = 0.0;
        T_STRB_D MembForD_Pre; MembForD_Pre.Initialize();
        for (int nEl=0; nEl<nUblElemSize; ++nEl)
        {
            m_pForcCtrl->ConvertDgnForceToStrb(aMembForPre[nEl].ElemFor[i], aStrbElem_Pre[nEl]);
        }
        DGN_MY_MAX_B MyMaxT; 
        m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_Pre, &MembForD_Pre, MyMaxT);

        MembForD_Pre.Mult(dFactorCS);
        ConvertCsgCalcUblMForce(0, MembForD_Pre, MembLcomD.aUlmFor[0].ForD[1].ForNC);   //MembForD의 Part1의 J단을 Ma로 저장
        ConvertCsgCalcUblMForce(1, MembForD_Pre, MembLcomD.aUlmFor[0].ForD[2].ForNC);   //MembForD의 Part2의 J단을 Mb로 저장
        ConvertCsgCalcUblMForce(2, MembForD_Pre, MembLcomD.aUlmFor[0].ForD[3].ForNC);   //MembForD의 Part3의 J단을 Mc로 저장
        MembLcomD.aUlmFor[0].dMmax = m_pForcCtrl->GetMaxMyByMembForce(&MembForD_Pre);
        int nMid = 0;
        int nCom = 0;
        GetUlmMidIndex(MembForD_Pre, nMid, nCom);
        ConvertCsgCalcUlmForce(MembForD_Pre, MembLcomD.aUlmf[0]); // LC
        ConvertCsgCalcUlmForce(MembForD_Pre, MembLcomD.aUlmf[1]); // NC

        ConvertCsgCalcUlmForceCb(MembForD_Pre, nMid, nCom, MembLcomD.aUlmf[0]); // LC
        ConvertCsgCalcUlmForceCb(MembForD_Pre, nMid, nCom, MembLcomD.aUlmf[1]); // NC

        //     int nPosI = 0;
        //     int nPosM = 0;
        //     int nPosJ = 0;
        //     _DGN_FORC_CRC SpanForI = ForceNC;  SpanForI *= dFactorCS;
        //     _DGN_FORC_CRC SpanForM = ForceNC;  SpanForM *= dFactorCS;
        //     _DGN_FORC_CRC SpanForJ = ForceNC;  SpanForJ *= dFactorCS;
        //     ConvertCsgCalcUlmForce(nPosI,nPosM,nPosJ, SpanForI, SpanForM, SpanForJ, MembLcomD.aUlmf[0]);

        ForceNC = aPreForceLcom[i];
        CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];

        for (int k=0; k<2; ++k) // I,J
        {
            if(bChkIJ[k]==FALSE) continue;
            BOOL bPosI = (k==0);
            MembLcomD.Memb.ChIf.nChkPos = k;
            MembLcomD.Memb.ChIf.bCantilever = FALSE; // bCantilever?
            MembLcomD.bChkUlmf = aUblmElemK.GetSize()>0;

            ConvertCsgCalcChkForce4Stage(k, ForceNC, MembLcomD.aChkFor[0]);

            double dMuy = ForceNC.dMuy[k];

            InitDgnResult(ChkConsR);
            CSG_CHK_RES_DTR_PARAM  DtrR;

            Check_Constructibility(ElemK, k, MembLcomD, rMembR, ChkConsR, DtrR);

            double dRatio_F = DtrR.dDTR_F;
            double dRatio_S = DtrR.dDTR_S;

            BOOL bPositive = (dMuy>=0.0) ? TRUE : FALSE;
            int nIx = (dMuy>=0.0) ? k*2 : k*2+1;
            ScbrR.ScbrB[nIx].nSectType = nSType;

            T_CSCH_REF RsltRef;
            RsltRef.pScbrB = &ScbrR.ScbrB[nIx];
            RsltRef.pScvrB = &ScvrR.ScvrB[k];

            RsltRef.pCcbrB = &CcbrR.CcbrB[nIx];
            RsltRef.pCcvrB = &CcvrR.CcvrB[k];

            if (nMcount[nIx]==0)
            { 
                dMmaxRat[nIx] = dRatio_F;
                SetScbrBase(bChkIJ[k], nLcomK4CS, 0/*CsifKD.StagK*/, 0/*CsifKD.nStepK*/, RsltRef);
                ConvertCsgScbrRes(bPositive, rMembR, ChkConsR, MembLcomD, dRatio_F, RsltRef);
            }
            else
            {
                if(dMmaxRat[nIx]<dRatio_F)
                { 
                    dMmaxRat[nIx] = dRatio_F;
                    SetScbrBase(bChkIJ[k], nLcomK4CS, 0/*CsifKD.StagK*/, 0/*CsifKD.nStepK*/, RsltRef);
                    ConvertCsgScbrRes(bPositive, rMembR, ChkConsR, MembLcomD, dRatio_F, RsltRef);          
                }
            }
            nMcount[nIx]++;

            if (nVcount[k]==0)
            { 
                dVmaxRat[k] = dRatio_S;
                SetScvrBase(bChkIJ[k], nLcomK4CS, 0/*CsifKD.StagK*/, 0/*CsifKD.nStepK*/, RsltRef);				
                ConvertCsgScvrRes(rMembR, ChkConsR, MembLcomD, dRatio_S, RsltRef);
            }
            else
            {
                if(dVmaxRat[k]<dRatio_S)
                { 
                    dVmaxRat[k] = dRatio_S;
                    SetScvrBase(bChkIJ[k], nLcomK4CS, 0/*CsifKD.StagK*/, 0/*CsifKD.nStepK*/, RsltRef);								
                    ConvertCsgScvrRes(rMembR, ChkConsR, MembLcomD, dRatio_S, RsltRef);
                }
            }
            nVcount[k]++;
        } // I, J

    } // nLcomSise



    if (pCheckGirder != NULL)
    {    
        if (IsCSACode(m_nDgnCode))
        {
            pCheckGirder->WriteCSBendingResistanceCSA_S6(ElemK, CcbrR);
            pCheckGirder->WriteCSShearResistanceCSA_S6  (ElemK, CcvrR);
        }
        else
        {
            pCheckGirder->WriteCSBendingResistanceKSCE_LSD(ElemK, ScbrR);
            pCheckGirder->WriteCSShearResistanceKSCE_LSD  (ElemK, ScvrR);
        }
    }

    ClassDelDgnResult(ChkConsR);

    return TRUE;
}


BOOL CDgnCsgDataCtrl::Check_ConstructionStage(ElemPairK ElemK, int nLcomK4CS,
                                              ArrElemPairKey& aUblmElemK,
                                              T_CSLF_CPT &Cslf,
                                              CSG_MEMB_POSD_KSCE_LSD &MembLcomD, 
                                              CSG_MEMB_RESULT_D &rMembR, CPG_CSIF_D& NCStageD)
{
	bool bCalcEngine = CDgnCodeCtrl::IsIDesignModule(m_iCivilCode);

    CStageInfo *pStageInfo = m_pDoc->m_pPostCtrl->GetStageInfo();

    CPlateGirderDesign *pCheckGirder = m_pDoc->m_pPostCtrl->GetPlateGirder();

    CArray<T_STAG_K,T_STAG_K> aStagK;
    pStageInfo->GetResultSavedStagKeyList(aStagK);
    int nStageSize = aStagK.GetSize();
    if(nStageSize==0)	return FALSE;

    BOOL bChkIJ[2]={FALSE};
    bChkIJ[0] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 1);
    bChkIJ[1] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 2);

    ElemPairK EPairI = ElemK, EPairM = ElemK, EPairJ = ElemK;
    int nPosI=0;
    int nPosM=0; // 임시, 찾아야함. 
    int nPosJ=1;

    BOOL bChkUblm=FALSE;
    BOOL bMakeMembFor=FALSE;
    DgnBeamPairK nUblmK(EN_EL_BEAM, 0);
    BOOL bCantilever = FALSE;

    m_pDataCtrl->GetUbLengthElemInfo(ElemK, nUblmK, EPairI, EPairM, EPairJ, nPosI, nPosM, nPosJ, bCantilever);

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);  

    T_ELEM_D ElemD;
    T_SECT_D SectD;
    m_pDoc->m_pAttrCtrl->GetElem(ElemK.first,ElemD);	  
    m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro,SectD);

    int nSType = GetSectType(SectD);

    CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
    T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);

    if (IsBeforeSgldDL())
    {
        strSgldName = _LS(IDS_DB_SGLD_DL);	// D.
        SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
    }

    ArCSIFK aCsif;

    for(int i=0; i<nStageSize; ++i)
    {
        T_STAG_K StagK = aStagK.GetAt(i);    
        if (ElemK.second == EN_EL_BEAM)
        {
            if (!pStageInfo->IsActiveElemInTgtStag(StagK, ElemK.first)) continue;
        }        

        // 0:오류 또는 시공 합성 단면 아님
        // 1:시공 합성 단면 : 합성 전 (Active Stage 이전)
        // 2:시공 합성 단면 : 합성 중 (Active되고 모든 Part가 합성되기 전)
        // 3:시공 합성 단면 : 합성 후 (모든 Part가 합성된 후)
        T_CSCS_D CscsD; CscsD.Initialize();
        int nCompositeType = 0;
        if (ElemK.second == EN_EL_BEAM)
        {
            nCompositeType = m_pDoc->m_pAttrCtrl->GetElemSectCompositeInfo(ElemK.first, StagK, CscsD);
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            nCompositeType = m_pDoc->m_pAttrCtrl->GetSecvCompositeInfo(ElemK.first, StagK, CscsD);
        }
        else ASSERT(0);
        

        if(nCompositeType==0 || nCompositeType == 3) continue;

        // 합성전까지만 돌린다.
        //     if(nCompositeType>1)
        //     {
        //       if(nCompositeType==3) break;
        //     }

        CPG_CSIF_D CsifD;
        CsifD.StagK = StagK;

        T_STAG_STEP_INFO StageInfo; StageInfo.Initialize();
        m_pDoc->m_pAttrCtrl->GetStagStepInfo(StagK,StageInfo);
        int nStepSize = StageInfo.aInfo.GetSize();
        if(nStepSize==0)
        {
            int nFinalStepNo=0;
            m_pDataCtrl->Get_LastStepInCurStage(StagK, nFinalStepNo);
            CsifD.StagK        = StagK;
            CsifD.bIsStep      = FALSE;
            CsifD.nSerialStepK = nFinalStepNo;
            aCsif.Add(CsifD);
        }
        else
        {
            for(int j=0; j<nStepSize; ++j)
            {
                int nStep=j+1; 
                int nSerialStep=0;
                if (!pStageInfo->GetStepSerialKey(StagK,nStep,nSerialStep)) continue;
                CsifD.StagK        = StagK;
                CsifD.bIsStep      = TRUE;
                CsifD.nStepK       = nStep;
                CsifD.nSerialStepK = nSerialStep;
                aCsif.Add(CsifD);
            }	
        }
    }
    if(aCsif.GetSize() > 0) NCStageD = aCsif[aCsif.GetSize()-1]; //예외처리 추가

	if ( bCalcEngine ) return TRUE; //엔진에서 가능할까?


    CSG_CHECK_RESULT_CODES ChkConsR;

    ClassNewDgnResult(ChkConsR);

    int nMcount[4]={0,};
    double dMmaxRat[4]={0.0,};

    int nVcount[2]={0,};
    double dVmaxRat[2]={0.0,};

    int nScount[4]={0,};
    double dSmaxRat[4]={0.0,};

    T_SCBR_D ScbrR;
    T_SCVR_D ScvrR;
    T_CSAC_D CsacR;

    T_CCBR_D CcbrR;
    T_CCVR_D CcvrR;

    const CSG_CHCK_INFO &Chif = MembLcomD.Memb.ChIf;

    int nStageStepSize = aCsif.GetSize();

    int nPreStagK=-1;
    for(int i=0; i<nStageStepSize; ++i)
    {
        const CPG_CSIF_D &CsifKD = aCsif[i];

        Get_CsgUlmSection(EPairI, EPairM, EPairJ, nPosI, nPosM, nPosJ, MembLcomD.Memb.Ulms);

        _DGN_FORC_CRC ForcBe;
        m_pForcCtrl->Get_GirderForce4CStage(StctD, ElemK, CsifKD.StagK, CsifKD.nSerialStepK, Cslf, ForcBe);        

        T_STRB_D MembForNC; MembForNC.Initialize();
        m_pForcCtrl->Get_GirderMembForce4CStage(StctD, CsifKD.StagK, CsifKD.nSerialStepK, Cslf, aUblmElemK, &MembForNC);
        ConvertCsgCalcUblMForce(0, MembForNC, MembLcomD.aUlmFor[0].ForD[1].ForNC);
        ConvertCsgCalcUblMForce(1, MembForNC, MembLcomD.aUlmFor[0].ForD[2].ForNC);
        ConvertCsgCalcUblMForce(2, MembForNC, MembLcomD.aUlmFor[0].ForD[3].ForNC);
        MembLcomD.aUlmFor[0].dMmax = m_pForcCtrl->GetMaxMyByMembForce(&MembForNC);
        int nMid = 0;
        int nCom = 0;
        GetUlmMidIndex(MembForNC, nMid, nCom);
        ConvertCsgCalcUlmForce(MembForNC, MembLcomD.aUlmf[0]); // LC
        ConvertCsgCalcUlmForce(MembForNC, MembLcomD.aUlmf[1]); // NC

        ConvertCsgCalcUlmForceCb(MembForNC, nMid, nCom, MembLcomD.aUlmf[0]); // LC
        ConvertCsgCalcUlmForceCb(MembForNC, nMid, nCom, MembLcomD.aUlmf[1]); // NC

        CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];

        for (int k=0; k<2; ++k) // I,J
        {
            if(bChkIJ[k]==FALSE) continue;
            BOOL bPosI = (k==0);
            MembLcomD.Memb.ChIf.nChkPos = k;
            MembLcomD.Memb.ChIf.bCantilever = bCantilever;
            MembLcomD.bChkUlmf = aUblmElemK.GetSize()>0;

            ConvertCsgCalcChkForce4Stage(k, ForcBe, MembLcomD.aChkFor[0]);

            double dMuy = ForcBe.dMuy[k];

            InitDgnResult(ChkConsR);
            CSG_CHK_RES_DTR_PARAM  DtrR;

            Check_Constructibility(ElemK, k, MembLcomD, rMembR, ChkConsR, DtrR);

            double dRatio_F = DtrR.dDTR_F;
            double dRatio_S = DtrR.dDTR_S;

            int nIx = (dMuy>=0.0) ? k*2 : k*2+1;
            BOOL bPositive = (dMuy>=0.0) ? TRUE :  FALSE;
            ScbrR.ScbrB[nIx].nSectType = nSType;

            T_CSCH_REF RsltRef;
            RsltRef.pScbrB = &ScbrR.ScbrB[nIx];
            RsltRef.pScvrB = &ScvrR.ScvrB[k];

            RsltRef.pCcbrB = &CcbrR.CcbrB[nIx];
            RsltRef.pCcvrB = &CcvrR.CcvrB[k];

            if (nMcount[nIx]==0)
            { 
                dMmaxRat[nIx] = dRatio_F;
                SetScbrBase(bChkIJ[k], nLcomK4CS, CsifKD.StagK, CsifKD.nStepK, RsltRef);
                ConvertCsgScbrRes(bPositive, rMembR, ChkConsR, MembLcomD, dRatio_F, RsltRef);
            }
            else
            {
                if(dMmaxRat[nIx]<dRatio_F)
                { 
                    dMmaxRat[nIx] = dRatio_F;
                    SetScbrBase(bChkIJ[k], nLcomK4CS, CsifKD.StagK, CsifKD.nStepK, RsltRef);
                    ConvertCsgScbrRes(bPositive, rMembR, ChkConsR, MembLcomD, dRatio_F, RsltRef);                    
                }
            }
            nMcount[nIx]++;

            if (nVcount[k]==0)
            { 
                dVmaxRat[k] = dRatio_S;
                SetScvrBase(bChkIJ[k], nLcomK4CS, CsifKD.StagK, CsifKD.nStepK, RsltRef);	
                ConvertCsgScvrRes(rMembR, ChkConsR, MembLcomD, dRatio_S, RsltRef);
            }
            else
            {
                if(dVmaxRat[k]<dRatio_S)
                { 
                    dVmaxRat[k] = dRatio_S;
                    SetScvrBase(bChkIJ[k], nLcomK4CS, CsifKD.StagK, CsifKD.nStepK, RsltRef);	
                    ConvertCsgScvrRes(rMembR, ChkConsR, MembLcomD, dRatio_S, RsltRef);
                }
            }
            nVcount[k]++;
        }
    }

    if (pCheckGirder != NULL)
    {    
        if (IsCSACode(m_nDgnCode))
        {
            pCheckGirder->WriteCSBendingResistanceCSA_S6(ElemK, CcbrR);
            pCheckGirder->WriteCSShearResistanceCSA_S6  (ElemK, CcvrR);
        }
        else
        {
            pCheckGirder->WriteCSBendingResistanceKSCE_LSD(ElemK, ScbrR);
            pCheckGirder->WriteCSShearResistanceKSCE_LSD  (ElemK, ScvrR);
        }
    }

    ClassDelDgnResult(ChkConsR);

    return TRUE;
}


BOOL CDgnCsgDataCtrl::Check_Constructibility(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_Strength(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_Service(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_Fatigue(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR)
{
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_ShearConnector(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_Stiffener(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Check_BearingStiffener(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
    return TRUE;
}

void CDgnCsgDataCtrl::GetMy4Strength(CSG_CHECK_RESULT_CODES &ChkRes, double &dMuy)
{
}

void CDgnCsgDataCtrl::ClassNewMembResult(CSG_MEMB_RESULT_D &MembR)
{
    ASSERT(0);
}

void CDgnCsgDataCtrl::ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
{
    ASSERT(0);

}

void CDgnCsgDataCtrl::InitMembResult(CSG_MEMB_RESULT_D &MembR)
{

}

void CDgnCsgDataCtrl::InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
{

}

void CDgnCsgDataCtrl::ClassDelDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
{
    if (ChkResD.pAASHTO != NULL) delete ChkResD.pAASHTO;
    if (ChkResD.pLSD    != NULL) delete ChkResD.pLSD;
    if (ChkResD.pCSA    != NULL) delete ChkResD.pCSA;
}



// BOOL CDgnCsgDataCtrl::CalcStiffnessOfStiffener(T_SECT_STIFF_D &Stfn, ST_SECT_STIFF_D &rData)
// {
//   int nStfnShapeSize = Stfn.aStiffShape.GetSize();
//   int nConvStfnSize  = rData.aStiffShape.GetSize();
//   if (nStfnShapeSize<1) return FALSE;
//   if (nStfnShapeSize != nConvStfnSize) return FALSE;
// 
//   //arData.SetSize(nStfnShapeSize);
//   T_SECT_D CalcS; CalcS.Initialize();
//   for (int i=0; i<nStfnShapeSize; ++i)
//   {
//     T_STIFF_SHAPE_D &StfnD = Stfn.aStiffShape[i];
// 
//     T_SECT_STIFFNESS &StfnStiff = CalcS.SectBefore.SectI.Stiffness;
//     CalcS.nStype = D_SECT_TYPE_REGULAR;
//     if      (StfnD.nType==0) CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_SB;
//     else if (StfnD.nType==1) CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_T;
//     else if (StfnD.nType==2) CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_URIB;
//     else ASSERT(0);
//     for (int j=0; j<8; ++j) CalcS.SectBefore.SectI.Size[j] = StfnD.dSize[j];
//     
//     CSectUtil::CalcSectData(CalcS);  
//     
//     ConvertSectStiffnessData(StfnStiff, rData.aStiffShape[i].Stiffness);
//   }
// 
//   return TRUE;
// }

BOOL CDgnCsgDataCtrl::GetEndElemOfSpan(T_SPAN_K SpanK, T_ELEM_K &nElemI, T_ELEM_K &nElemJ)
{
    T_SPAN_D SpanD; SpanD.Initialize();
    if (m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD))
    {
        int nSpanElemSize = SpanD.aElemBase.GetSize();
        if (nSpanElemSize>0)
        {
            nElemI = nElemJ = SpanD.aElemBase[0].ElemK;
            if (nSpanElemSize>1) nElemJ = SpanD.aElemBase[nSpanElemSize-1].ElemK;
        }    
    }
    return TRUE;
}

BOOL CDgnCsgDataCtrl::GetEndElemOfSpan4Stage(T_STAG_K StagK, T_SPAN_K SpanK, T_ELEM_K &nElemI, T_ELEM_K &nElemM, T_ELEM_K &nElemJ)
{
    // Stage 별, Span의 요소가 다를 수 있기 때문에 찾아야 한다.  

    return TRUE;
}

BOOL CDgnCsgDataCtrl::GetReactionNodeOfElem(T_LCOM_D LcomD, ElemPairK ElemPairK, int nPosIJ, int nMaxMinType, T_REAC_D& rReacD)
{
	if (nPosIJ < 0 || nPosIJ>1) return FALSE;

    // 반력 && Link Force 
    T_ELEM_D ElemD; ElemD.Initialize();
    m_pDoc->m_pPostCtrl->GetElemPost(ElemPairK.first, ElemD);

    m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);                       // Load Combi를 셋팅시킴.

    T_REAC_D ReacMaxData;  ReacMaxData.Initialize();
    T_REAC_D ReacMinData;  ReacMinData.Initialize();
    if (ElemPairK.second == EN_EL_BEAM)
    {
        m_pDoc->m_pPostCtrl->GetReacNew(ElemD.elnod[nPosIJ], &ReacMaxData, &ReacMinData, NULL); // Load Combi에 대한 반력 
    }
    else if (ElemPairK.second == EN_EL_VBEAM)
    {
        T_VBEM_D VbemD; VbemD.Initialize();
        if (m_pDoc->m_pAttrCtrl2->GetVbem(ElemPairK.first, VbemD))
        {
            T_VSEC_K CurVsecK = nPosIJ == 0 ? VbemD.Vsec1 : VbemD.Vsec2;
            m_pDoc->m_pPostCtrl->GetReacNewVeam(CurVsecK, &ReacMaxData, &ReacMinData, NULL);
        }
        
    }
    else ASSERT(0);


    double dFz = 0.0;
    if (nMaxMinType == 0)
    {
        rReacD = ReacMaxData;;
        dFz = ReacMaxData.dblReac[2]; //
    }
    else
    {
		rReacD = (nMaxMinType % 2 == 1) ? ReacMaxData : ReacMinData;
        dFz = rReacD.dblReac[2];
    }
    //

    // Link Force 정보
    if (dFz == 0.0)
    {
        dFz = GetLinkForce(0, nMaxMinType, nPosIJ, ElemD.elnod[nPosIJ]);
        rReacD.Initialize();
        rReacD.dblReac[2] = fabs(dFz);
    }   

    return TRUE;
}

BOOL CDgnCsgDataCtrl::GetDeadLoadCSMaxFactor(int &nLcomK, double &dFactor)
{
    int nLcomSize = m_pForcCtrl->Get_LcomDataCount();
    int nChkLcomCount=0;

    double dMaxFactor=0.0;
    int    nChkLcomK=-1;
    for (int i=0; i<nLcomSize; ++i)
    {
        int nDgnLcomNo = i+1;
        _DGN_LCOM DgnLcom;
        if (m_pForcCtrl->Get_DgnLcomData(nDgnLcomNo, DgnLcom))
        {
            BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(nDgnLcomNo);
            if (bULSLcom==TRUE)
            {
                nChkLcomCount++;
                if (nChkLcomCount==1)
                {
                    nChkLcomK  = DgnLcom.OriginalLcomKey;
                    dMaxFactor = DgnLcom.dFactorDeadCS;
                }
                else
                {
                    if (dMaxFactor<DgnLcom.dFactorDeadCS)
                    {
                        nChkLcomK  = DgnLcom.OriginalLcomKey;
                        dMaxFactor = DgnLcom.dFactorDeadCS;
                    }
                }
            }      
        }
    }

    nLcomK  = nChkLcomK;
    dFactor = dMaxFactor;   
    return (nLcomK==-1) ? FALSE : TRUE;
}


double CDgnCsgDataCtrl::GetLinkForce(int nOrgLcomNo, int nMaxMinType, int nIJ, int elnod)
{

    double dFx = 0.0; // Link의 축력 사용
    T_REAC_D ReacMaxData;  ReacMaxData.Initialize();
    T_STEK_D Stek;         Stek.Initialize();
    T_STEK_D StekMin;      StekMin.Initialize();

    auto _Rec = [&](int nType, const T_STEK_D& MaxR, const T_STEK_D& MinR) -> double
    {
        double dFz = 0.0;
        if (nType==0)
        {
            dFz = MaxR.dblForce[nIJ][0]; //
        }
        else
        {
            dFz = (nType % 2 == 1) ? MaxR.dblForce[nIJ][0] : MinR.dblForce[nIJ][0];
        }
        return dFz;
    };

    // Elastic Link
    int nLinkKey = GetElasticLinkKey4Node(elnod);
    if(nLinkKey!=0)
    {
        m_pDoc->m_pPostCtrl->GetStekNew(nLinkKey, &Stek, &StekMin, 0); // Load Combi에 대한 Link Force 

        dFx = _Rec(nMaxMinType, Stek, StekMin);
        return dFx;
    }

    // General link
    nLinkKey = GetGeneralLinkKey4Node(elnod);
    if(nLinkKey!=0)
    {
        m_pDoc->m_pPostCtrl->GetStekNew(nLinkKey, &Stek, &StekMin, 0); // Load Combi에 대한 Link Force 

        dFx = _Rec(nMaxMinType, Stek, StekMin);
        return dFx;
    }


    // Rigid link
    nLinkKey = GetRigidLinkKey4Node(elnod);
    if(nLinkKey!=0)
    {
        m_pDoc->m_pPostCtrl->GetStekNew(nLinkKey, &Stek, &StekMin, 0); // Load Combi에 대한 Link Force 

        dFx = _Rec(nMaxMinType, Stek, StekMin);
        return dFx;
    }


    return dFx;
}

int CDgnCsgDataCtrl::GetElasticLinkKey4Node(int elnod)
{
    int nLinkKey =0;

    CArray<T_ELNK_K, T_ELNK_K> rElKeyList; rElKeyList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetElnkKeyList(rElKeyList); 
    int nSizeofElLink = rElKeyList.GetSize();
    for(int i=0; i<nSizeofElLink; ++i)
    {
        T_ELNK_K Key = rElKeyList.GetAt(i);
        T_ELNK_D rData;
        m_pDoc->m_pAttrCtrl->GetElnk(Key,rData);

        if(elnod==rData.Node1 || elnod==rData.Node2)
        {
            nLinkKey = Key;
            break;
        }
    }

    return nLinkKey;
}

int CDgnCsgDataCtrl::GetGeneralLinkKey4Node(int elnod)
{
    int nLinkKey =0;

    CArray<T_NLNK_K, T_NLNK_K> rKeyList; rKeyList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetElnkKeyList(rKeyList); 
    int nSizeofElLink = rKeyList.GetSize();
    for(int i=0; i<nSizeofElLink; ++i)
    {
        T_NLNK_K Key = rKeyList.GetAt(i);
        T_NLNK_D rData;
        m_pDoc->m_pAttrCtrl->GetNlnk(Key,rData);

        if(elnod==rData.Node1 || elnod==rData.Node2)
        {
            nLinkKey = Key;
            break;
        }
    }

    return nLinkKey;
}


int CDgnCsgDataCtrl::GetRigidLinkKey4Node(int elnod)
{
    int nLinkKey =0;

    CArray<T_RIGD_K, T_RIGD_K>  rarKey; rarKey.RemoveAll(); 
    CArray<T_RIGD_D, T_RIGD_D&> rarData; rarData.RemoveAll();
    m_pDoc->m_pPostCtrl->GetRigdPost(elnod, rarKey, rarData, FALSE);

    int nSize= rarData.GetSize();
    for(int i=0; i<nSize; ++i)
    {
        T_RIGD_D RIGD = rarData.GetAt(i);

        CArray<T_NODE_K, T_NODE_K> arNode; arNode.RemoveAll();
        arNode.Copy(RIGD.arSlave);

        // Slave
        int nNodeSize = arNode.GetSize();
        for(int j=0; j<nNodeSize; ++j)
        {
            T_NODE_K Nodek = arNode.GetAt(j);

            int nElLinKey = GetElasticLinkKey4Node(Nodek);
            if(nElLinKey !=0)
            {
                nLinkKey = nElLinKey;
                return nLinkKey;
            }

            nElLinKey = GetGeneralLinkKey4Node(Nodek);
            if(nElLinKey !=0)
            {
                nLinkKey = nElLinKey;
                return nLinkKey;
            }
        }
    }

    return nLinkKey;
}



BOOL CDgnCsgDataCtrl::Get_CsgUlmSection(ElemPairK ElemI, ElemPairK ElemM, ElemPairK ElemJ, int nPosI, int nPosM, int nPosJ, CSG_ULMS_D &rData)
{
    if (nPosI<0 || nPosI>1) return FALSE;
    if (nPosM<0 || nPosM>1) return FALSE; // M은 0,1,2,3,4 가능 
    if (nPosJ<0 || nPosJ>1) return FALSE;


	const auto& itSectI = m_mCsgElemSect.find(ElemI);
    // I
    if (itSectI != m_mCsgElemSect.end())
    {
        const auto& itSect = m_mCsgSect.find(itSectI->second);
        if (itSect != m_mCsgSect.end())
        {
            rData.SectD[0] = itSect->second.SectPos[nPosI];
        }
        const auto& itTrst = m_mCsgTrst.find(itSectI->second);
		if (itTrst != m_mCsgTrst.end())
		{
			rData.TrstD[0] = itTrst->second.TrstPos[nPosI];
		}
        
    }
	const auto& itUfraI = m_mCsgUfra.find(ElemI);
    if (itUfraI != m_mCsgUfra.end())
    {
        rData.UfraD[0] = itUfraI->second.UfraPos[nPosI];
    }

    int nNode[2] = { 0, 0 };
    BOOL bSupprotNode[2] = { FALSE, FALSE };
    CheckSupprotNodeElem(ElemI, nNode);
    rData.abSupport[0] = nNode[nPosI];
    bool bInteriorSupport[2] = { false, false };
    CheckInteriorSupportNodeElem(ElemI, bInteriorSupport);
    rData.abInteriorSupport[0] = bInteriorSupport[nPosI];

    // J
    const auto& itSectJ = m_mCsgElemSect.find(ElemJ);
    if (itSectJ != m_mCsgElemSect.end())
    {
        const auto& itSect = m_mCsgSect.find(itSectJ->second);
        if (itSect != m_mCsgSect.end())
        {
            rData.SectD[2] = itSect->second.SectPos[nPosJ];
        }
        const auto& itTrst = m_mCsgTrst.find(itSectJ->second);
        if (itTrst != m_mCsgTrst.end())
        {
            rData.TrstD[1] = itTrst->second.TrstPos[nPosJ];
        }

    }
    const auto& itUfraJ = m_mCsgUfra.find(ElemJ);
    if (itUfraJ != m_mCsgUfra.end())
    {
        rData.UfraD[1] = itUfraJ->second.UfraPos[nPosJ];
    }
    CheckSupprotNodeElem(ElemJ, nNode, TRUE);
    rData.abSupport[1] = nNode[nPosJ];
    CheckInteriorSupportNodeElem(ElemJ, bInteriorSupport);
    rData.abInteriorSupport[1] = bInteriorSupport[nPosJ];

    // M,  
    const auto& itSectM = m_mCsgElemSect.find(ElemM);    
    if (itSectM != m_mCsgElemSect.end())
    {
        const auto& itSect = m_mCsgSect.find(itSectM->second);
        if (itSect != m_mCsgSect.end())
        {
            rData.SectD[1] = itSect->second.SectPos[nPosM];
        }
    }

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Get_CsgUlmSection(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD& rData)
{
    ElemPairK EPairI = ElemK, EPairM = ElemK, EPairJ = ElemK;
    int nPosI = 0;
    int nPosM = 0;
    int nPosJ = 1;

    BOOL bChkUblm = FALSE;
    BOOL bCantilever = FALSE;
    BOOL bMakeMembFor = FALSE;
    BOOL b2SideBgg = FALSE;
    DgnBeamPairK nUblmK(EN_EL_BEAM, 0);
    DgnBeamPairK nCurUblmK(EN_EL_BEAM, 0);
    ArrElemPairKey aUblmElemK;

    m_pDataCtrl->GetUbLengthElemInfo(ElemK, nUblmK, nCurUblmK, EPairI, EPairM, EPairJ, nPosI, nPosM, nPosJ, bChkUblm, bCantilever, bMakeMembFor, b2SideBgg, aUblmElemK);
    rData.bChkUlmf = bChkUblm;
	rData.Memb.ChIf.bCantilever = bCantilever;
	rData.Memb.ChIf.b2SideLateralRestraint = b2SideBgg;

    Get_CsgUlmSection(EPairI, EPairM, EPairJ, nPosI, nPosM, nPosJ, rData.Memb.Ulms);
    return TRUE;
}

BOOL CDgnCsgDataCtrl::Get_CsgUlmsData4MyMax(ElemPairK ElemK, int nPos, CSG_ULMS_D& rData)
{
    const auto& itSect4E = m_mCsgElemSect.find(ElemK);

    if (itSect4E != m_mCsgElemSect.end())
    {
        rData.nSectK4Mymax = static_cast<unsigned int>(itSect4E->second.first);
		const auto& itSect = m_mCsgSect.find(itSect4E->second);
		if (itSect != m_mCsgSect.end())
		{
			rData.SectMmax = itSect->second.SectPos[nPos];
		}
        else
        {
            CSG_SECT_ELEM SectElem;
            BOOL bLongTermSectProp = FALSE;
            if (Get_CsgSectData4Elem(ElemK, bLongTermSectProp, SectElem))
            {
                SectPairK SPairK = m_pDataCtrl->GetSectPairKey(ElemK);
                m_mCsgSect.insert({ SPairK, SectElem });
            }
            rData.SectMmax = SectElem.SectPos[nPos];
        }

        const auto& itTrst = m_mCsgTrst.find(itSect4E->second);
		if (itTrst != m_mCsgTrst.end())
		{
			rData.TrstMmax = itTrst->second.TrstPos[nPos];
		}

		const auto& itUfra = m_mCsgUfra.find(ElemK);
		if (itUfra != m_mCsgUfra.end())
		{
			rData.UfraMmax = itUfra->second.UfraPos[nPos];
		}
    }
    rData.nElemK4Mymax = ElemK.first;
    rData.nBeamType = static_cast<unsigned int>(ElemK.second);

    return TRUE;
}

BOOL CDgnCsgDataCtrl::GetCsgSectionSupport(ElemPairK EPairK, CSG_MEMB_POSD_KSCE_LSD& rData)
{
    int nNode[2] = { 0, 0 };
    BOOL bSupportNode[2] = { FALSE, FALSE };
    CheckSupprotNodeElem(EPairK, nNode);
    bSupportNode[0] = nNode[0];
    CheckSupprotNodeElem(EPairK, nNode, TRUE);
    bSupportNode[1] = nNode[1];
    bool bInteriorSupport[2] = { false, false };
    CheckInteriorSupportNodeElem(EPairK, bInteriorSupport);

    const auto& itSect4E = m_mCsgElemSect.find(EPairK);
    if (itSect4E != m_mCsgElemSect.end())
    {
        const auto& itSect = m_mCsgSect.find(itSect4E->second);
        if (itSect != m_mCsgSect.end())
        {
            rData.Memb.PosD[0].Sect = itSect->second.SectPos[0];
            rData.Memb.PosD[1].Sect = itSect->second.SectPos[1];
            rData.Memb.PosD[0].bSupprotNode = bSupportNode[0];
            rData.Memb.PosD[1].bSupprotNode = bSupportNode[1];
            rData.Memb.PosD[0].bInteriorSupport = bInteriorSupport[0];
            rData.Memb.PosD[1].bInteriorSupport = bInteriorSupport[1];
        }
    }

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Get_UnbracedLengthData(ElemPairK ElemK, BOOL bKfac, T_KFAC_D &KFacD, CSG_MEMB_ULEN &rData)
{
    T_LENG_D LengD; LengD.Initialize();
    BOOL bAutoCalc=FALSE;
    if (ElemK.second == EN_EL_BEAM)
    {
        if (m_pDoc->m_pAttrCtrl->GetLeng(ElemK.first, LengD))
        {
            rData.dLy = LengD.BLy;
            rData.dLz = LengD.BLz;
            rData.dLb = LengD.BLu;
            bAutoCalc = LengD.bAutoCalcLe;
        }
        else
        {
            auto itUblmK = m_pDataCtrl->m_mElemUblmKey.find(ElemK);
            if (itUblmK != m_pDataCtrl->m_mElemUblmKey.end())
            {
                auto itUblMemb = m_pDataCtrl->m_mUblenMemb.find(itUblmK->second);
                if (itUblMemb != m_pDataCtrl->m_mUblenMemb.end())
                {
                    const T_ULEN_MEMB_D& UblmD = itUblMemb->second;
                    rData.dLy = UblmD.dLength;
                    rData.dLz = UblmD.dLength;
                    rData.dLb = UblmD.dLength;
                }
            }
            else
            {
                double dElemLength = m_pDoc->calcLAVElem(ElemK.first);
                rData.dLy = dElemLength;
                rData.dLz = dElemLength;
                rData.dLb = dElemLength;
            }
        }
    }
    else if (ElemK.second == EN_EL_VBEAM)
    {
		T_LENV_D LenvD;
        if (m_pDoc->m_pAttrCtrl->GetLenv(ElemK.first, LenvD))
        {
            rData.dLy = LenvD.BLy;
            rData.dLz = LenvD.BLz;
            rData.dLb = LenvD.BLu;
            // bAutoCalc = LengD.bAutoCalcLe; auto 할 수 없는데...
        }
        else
        {
            double dElemLength = m_pDoc->calcLenthVbem(ElemK.first);
            rData.dLy = dElemLength;
            rData.dLz = dElemLength;
            rData.dLb = dElemLength;
        }
    }
    else ASSERT(0);

    if (bKfac)
    {
        rData.dKy = KFacD.BKy;
        rData.dKz = KFacD.BKz;
    }
    else
    {
        rData.dKy = rData.dKz = 1.0;
    }
    return TRUE;
}

void CDgnCsgDataCtrl::SetScbrBase(BOOL bChk, int nLcomK4CS, int nStageK, int nStepK, T_CSCH_REF &rData)
{
    if (rData.pScbrB==NULL) return;
    rData.pScbrB->bChk      = bChk;
    rData.pScbrB->nOrgLcomK = nLcomK4CS;
    rData.pScbrB->nStageK   = nStageK;
    rData.pScbrB->nStepK    = nStepK;

}

void CDgnCsgDataCtrl::SetScvrBase(BOOL bChk, int nLcomK4CS, int nStageK, int nStepK, T_CSCH_REF &rData)
{
    if (rData.pScvrB==NULL) return;
    rData.pScvrB->bChk      = bChk;
    rData.pScvrB->nOrgLcomK = nLcomK4CS;
    rData.pScvrB->nStageK   = nStageK;
    rData.pScvrB->nStepK    = nStepK;

}

void CDgnCsgDataCtrl::SetCsbrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
    if (rData.pCsbrB==NULL) return;
    rData.pCsbrB->bChk      = bChk;
    rData.pCsbrB->nLcomK	  = nDgnLcomNo;
    rData.pCsbrB->nOrgLcomK = nOrgLcomNo;		    
    rData.pCsbrB->nMax	    = nMaxMinType;

}
void CDgnCsgDataCtrl::SetCsvrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
    if (rData.pCsvrB==NULL) return;
    rData.pCsvrB->bChk      = bChk;
    rData.pCsvrB->nLcomK	  = nDgnLcomNo;
    rData.pCsvrB->nOrgLcomK = nOrgLcomNo;		    
    rData.pCsvrB->nMax	    = nMaxMinType;

}

void CDgnCsgDataCtrl::SetCsslBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
    if (rData.pCsslB==NULL) return;
    rData.pCsslB->bChk      = bChk;
    rData.pCsslB->nLcomK	  = nDgnLcomNo;
    rData.pCsslB->nOrgLcomK = nOrgLcomNo;		    
    rData.pCsslB->nMax	    = nMaxMinType;

}

void CDgnCsgDataCtrl::SetCsacBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
    if (rData.pCsacB==NULL) return;
    rData.pCsacB->bChk      = bChk;
    rData.pCsacB->nLcomK	  = nDgnLcomNo;
    rData.pCsacB->nOrgLcomK = nOrgLcomNo;		    
    rData.pCsacB->nMax	    = nMaxMinType;

}
void CDgnCsgDataCtrl::SetCsfcBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
    //rData.bChk      = bChk;
    //rData.LoadR.nLcomK	  = nDgnLcomNo;
    //rData.nOrgLcomK = nOrgLcomNo;		    
    //rData.nMax	    = nMaxMinType;

}

BOOL CDgnCsgDataCtrl::Check_LcomMaxMinType(UINT nLcomK)
{
    T_LCOM_D LcomData; 		LcomData.Initialize();
    m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, nLcomK, LcomData);
    BOOL bMaxMin = m_pDoc->m_pAttrCtrl->IsMaxMinComb(LcomData);

    return bMaxMin;
}

void CDgnCsgDataCtrl::SetCsfcLoadBase(int nType, BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nTopBot, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::SetCsfcFlexBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::SetCsfcShearBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::SetCscrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
    if (rData.pCscrB==NULL) return;
    rData.pCscrB->bChk      = bChk;
    rData.pCscrB->nLcomK	  = nDgnLcomNo;
    rData.pCscrB->nOrgLcomK = nOrgLcomNo;		    

    BOOL bMaxMin = Check_LcomMaxMinType(rData.pCscrB->nOrgLcomK);
    rData.pCscrB->nMax      = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nMaxMinType; //nMaxMinType+1 : 0;

}

void CDgnCsgDataCtrl::SetBstfBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
    if (rData.pBstfB==NULL) return;
    rData.pBstfB->bChk      = bChk;
    rData.pBstfB->nLcomK	  = nDgnLcomNo;
    rData.pBstfB->nOrgLcomK = nOrgLcomNo;		    

    BOOL bMaxMin = Check_LcomMaxMinType(rData.pBstfB->nOrgLcomK);
    rData.pBstfB->nMax      = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nMaxMinType; //nMaxMinType+1 : 0; 
}

void CDgnCsgDataCtrl::SetCpbsBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
	if (rData.pCpbsB == NULL) return;
	rData.pCpbsB->bChk = bChk;
	rData.pCpbsB->nLcomK = nDgnLcomNo;
	rData.pCpbsB->nOrgLcomK = nOrgLcomNo;

	BOOL bMaxMin = Check_LcomMaxMinType(rData.pCpbsB->nOrgLcomK);
	rData.pCpbsB->nMax = (bMaxMin) ? GetLcomTypeIndex4MaxMin() : nMaxMinType; //nMaxMinType+1 : 0; 
}

void CDgnCsgDataCtrl::SetCpfcStudBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
    //   rData.bChk      = bChk;
    //   rData.nLcomK	  = nDgnLcomNo;
    //   rData.nOrgLcomK = nOrgLcomNo;		    
    //   rData.nMax	    = nMaxMinType;
}

void CDgnCsgDataCtrl::SetCpfcSconBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
    //   rData.bChk      = bChk;
    //   rData.nLcomK	  = nDgnLcomNo;
    //   rData.nOrgLcomK = nOrgLcomNo;		    
    //   rData.nMax	    = nMaxMinType;
}

BOOL CDgnCsgDataCtrl::Get_CsgMembInData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &rData)
{
    rData.Memb.ChIf.nElem = ElemK.first;

    auto iter = m_mCsgElemSect.find(ElemK);

    Get_CsgPosData(ElemK, iter->second, 0, rData.Memb.PosD[0]);
    Get_CsgPosData(ElemK, iter->second, 1, rData.Memb.PosD[1]);

	auto itSpanL = m_mElemSpanLength.find(ElemK);
	if (itSpanL != m_mElemSpanLength.end())
	{
        rData.Memb.ChIf.dSpanLen = itSpanL->second;
	}	

    return TRUE;
}

BOOL CDgnCsgDataCtrl::Get_CsgPosData(ElemPairK ElemK, SectPairK SectK, int nPos, CSG_DATA_POSD &rData)
{
    if(nPos<0 || nPos>1) return FALSE;

	const auto& itCpsc = m_mCsgCpsc.find(ElemK);
    if (itCpsc != m_mCsgCpsc.end())
    {
        rData.Scon = itCpsc->second.TfType[nPos];
    }

	const auto& itCpfp = m_mCsgCpfp.find(ElemK);
    if (itCpfp != m_mCsgCpfp.end())
    {
        rData.Fati = itCpfp->second.TfType[nPos];
    }

    const auto& itCpcb = m_mCsgCpcb.find(ElemK);
    if (itCpcb != m_mCsgCpcb.end())
    {
        rData.Cpcb = itCpcb->second.TfType[nPos];
    }

    const auto& itDohl = m_mCsgDohl.find(ElemK);
    if (itDohl != m_mCsgDohl.end())
    {
        rData.Dohl = itDohl->second.DohlPos[nPos];
    }

    const auto& itTrst = m_mCsgTrst.find(SectK);
    if (itTrst != m_mCsgTrst.end())
    {
        rData.Trst = itTrst->second.TrstPos[nPos];
    }

    const auto& itEbsl = m_mCsgEbsl.find(ElemK);
    if (itEbsl != m_mCsgEbsl.end())
    {
        rData.Ebsl = itEbsl->second.EbslPos[nPos];
    }

    const auto& itLtbd = m_mCsgLtbd.find(ElemK);
    if (itLtbd != m_mCsgLtbd.end())
    {
        rData.Ltbd = itLtbd->second.LtbPos[nPos];
    }

    const auto& itDwsp = m_mCsgDwsp.find(ElemK);
    if (itDwsp != m_mCsgDwsp.end())
    {
        rData.Dwsp = itDwsp->second.DwspPos[nPos];
    }

    const auto& itUfra = m_mCsgUfra.find(ElemK);
    if (itUfra != m_mCsgUfra.end())
    {
        rData.Ufra = itUfra->second.UfraPos[nPos];
    }

    const auto& itMdfs = m_mCsgMdfs.find(ElemK);
    if (itMdfs != m_mCsgMdfs.end())
    {
        rData.Mdfs = itMdfs->second.MdfsPos[nPos];
    }

    const auto& itLshr = m_mCsgLshr.find(ElemK);
    if (itLshr != m_mCsgLshr.end())
    {
        rData.Lshr = itLshr->second.LshrPos[nPos];
    }

    CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
    Get_CpgRbarElem(ElemK, RbarElem);

    ConvertCsgCalcData(SectK.first, nPos, NULL, &RbarElem, rData);

    return TRUE;
}

int  CDgnCsgDataCtrl::GetSectType(T_SECT_D &rSect)
{
    int nStype = rSect.nStype;
    if(rSect.nStype == D_SECT_TYPE_TAPERED) nStype = rSect.SectBefore.nStype;
    return nStype;
}

void CDgnCsgDataCtrl::ConvertCGSGlobalData(T_CPGD_D &CpgdD, S_CSGD_D &rData)
{
    rData.dPhi_y = CpgdD.dPhi_y;
    rData.dPhi_u = CpgdD.dPhi_u;
    rData.dPhi_c = CpgdD.dPhi_c;
    rData.dPhi_f = CpgdD.dPhi_f;
    rData.dPhi_v = CpgdD.dPhi_v;
    rData.dPhi_se= CpgdD.dPhi_se;
    rData.dPhi_b = CpgdD.dPhi_b;
    memcpy(rData.dCSLcbFactor,CpgdD.dCSLcbFactor,sizeof(rData.dCSLcbFactor));
    rData.nGirderType = CpgdD.nGirderType;
    rData.nForceType  = CpgdD.nForceType;
    for ( int i = 0; i < 7; ++i )
    {
        rData.bDesignParameter[i] = CpgdD.bDesignParameter[i] ? true : false;
    }
    rData.nConsiderPv = CpgdD.nConsiderPv;
    rData.bAppendix6  = CpgdD.bAppendix6;

    rData.bDistortion =CpgdD.bDistortion;       
    rData.b1p3RhMy    =CpgdD.b1p3RhMy;          
    rData.bPBTFA4SH   =CpgdD.bPBTFA4SH; 
    rData.bWarping    =CpgdD.bDesignParameter[6];
	rData.bLongTermSectProp4NM = CpgdD.bLongTermSectProp;

    rData.bPrtOptSum	= CpgdD.bPrtOptSum;          
    rData.bPrtOptDetail	= CpgdD.bPrtOptDetail;      

    rData.dPhi_s_Flex  = CpgdD.dPhi_f ;
    rData.dPhi_s_Shear = CpgdD.dPhi_v ;
    rData.dPhi_s_Comp  = CpgdD.dPhi_c ;
    rData.dPhi_S_tens  = CpgdD.dPhi_s_Tens;
    rData.dPhi_tos     = CpgdD.dPhi_s_Tos ;
    rData.dPhi_r       = CpgdD.dPhi_y ;
    rData.dPhi_SC      = CpgdD.dPhi_se ;
    rData.dPhi_Con     = CpgdD.dPhi_b;
    rData.dPhi_ui      = CpgdD.dPhi_u;
	rData.bRebarConsideration4PNA = CpgdD.bCompReinforcement ? true : false;


    int nDgnCode = 0;
    nDgnCode = CpgdD.iDgnCode;
    if(CpgdD.iDgnCode == KSCE_LSD15_CSG && CpgdD.nConsiderPv == 1) // 곡선교
    {
        nDgnCode = KSSC_2014_CSG;
    }

    rData.nDgnCode = GetConvertCSGCodeCvlToEng(nDgnCode);
    rData.nState = CpgdD.nSubCode;

}

void CDgnCsgDataCtrl::ConvertCsgCalcData(T_SECT_K SectK, int nPos, CPG_SECT_ELEM *pSect, CPG_RBAR_ELEM *pRbar, CSG_DATA_POSD &rData)
{
    if (nPos<0 || nPos>1) return;

    ConvertPscCalcRbarData(&pRbar->RbarPosi[nPos], rData.Rbar);
}

BOOL CDgnCsgDataCtrl::ConvertCsgCalcMatlData(CPG_MATL_ELEM *pMatlD, CSG_MATL_ELEM &rData)
{
    if (pMatlD==NULL) return FALSE;

    // 요소별 저장하지 말고 Material key로 하자

    rData.bHybrid = pMatlD->MatdD.Data1.bHybridFactor;
    T_CPGD_D CpgdD;
    if(!m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD))
    {
        CpgdD.Initialize();
    }

    if (rData.bHybrid==TRUE && CDBLib::IsHybridMatlByDgnCode(CpgdD.iDgnCode))
    {
        rData.MatStf.csCodeName = pMatlD->MatdD.Data1.CodeName;
        rData.MatStf.csMatlName = pMatlD->MatdD.Data1.DesignTopFlange.strMatlName;
        ConvertCsgCalcMatlStlPart(pMatlD->MatdD.Data1.AnalysisTopFlange, pMatlD->MatdD.Data1.DesignTopFlange, rData.MatStf.Steel);

        rData.MatSbf.csCodeName = pMatlD->MatdD.Data1.CodeName;
        rData.MatSbf.csMatlName = pMatlD->MatdD.Data1.DesignBotFlange.strMatlName;
        ConvertCsgCalcMatlStlPart(pMatlD->MatdD.Data1.AnalysisBotFlange, pMatlD->MatdD.Data1.DesignBotFlange, rData.MatSbf.Steel);

        rData.MatSw.csCodeName = pMatlD->MatdD.Data1.CodeName;
        rData.MatSw.csMatlName = pMatlD->MatdD.Data1.DesignWeb.strMatlName;
        ConvertCsgCalcMatlStlPart(pMatlD->MatdD.Data1.AnalysisWeb, pMatlD->MatdD.Data1.DesignWeb, rData.MatSw.Steel);

    }
    else
    {
        rData.MatStf.csCodeName = pMatlD->MatdD.Data1.CodeName;
        rData.MatStf.csMatlName = pMatlD->MatdD.Data1.CodeMatlName;
        ConvertCsgCalcMatlStlDgn(pMatlD->MatdD.Data1, rData.MatStf.Steel);

        rData.MatSbf = rData.MatStf;
        rData.MatSw  = rData.MatStf; 
    }

    rData.MatSlab.ConcD.strCodeName     = pMatlD->MatdD.Data2.CodeName;
    rData.MatSlab.ConcD.strMatlName     = pMatlD->MatdD.Data2.CodeMatlName;
    rData.MatSlab.RbarD.strCodeName     = pMatlD->MatdD.Rebar_CodeName;
    rData.MatSlab.RbarD.strMatlName     = pMatlD->MatdD.MainRebar_RebarName;
    rData.MatSlab.RbarD.strSbarMatlName = pMatlD->MatdD.SubRebar_RebarName;

    rData.MatSlab.RC.dFck  = pMatlD->MatdD.Data2.Design.C_fc;
    rData.MatSlab.RC.dFy   = pMatlD->MatdD.MainRebarData.B_fy;
    rData.MatSlab.RC.dFys  = pMatlD->MatdD.SubRebarData.B_fy;	
    rData.MatSlab.RC.dEc   = pMatlD->MatdD.Data2.Analysis.Elast;
    rData.MatSlab.RC.dEs   = pMatlD->MatdD.MainRebarData.B_Elast;
    rData.MatSlab.RC.dEss  = pMatlD->MatdD.SubRebarData.B_Elast;

    switch (m_iCivilCode)
    {
    case STL_NR_GN_CIV_025_06:
    case NR_GN_CIV_025_06_CSG:
        return TRUE;
    }

    if(pMatlD->MatdD.Type!=_T("SRC"))
    { // src가 아니면 강합성 설계 대화창에서 모든 정보 입력해야 설계가능하도록 함
        if(rData.MatSlab.ConcD.strCodeName==_T("")) return FALSE;
        if(rData.MatSlab.ConcD.strMatlName==_T("")) return FALSE;
        if(rData.MatSlab.RbarD.strCodeName==_T("")) return FALSE;
        if(rData.MatSlab.RbarD.strMatlName==_T("")) return FALSE;
        //if(rData.MatSlab.RbarD.strSbarMatlName==_T("")) return FALSE;

        if(rData.MatSlab.RC.dFck <=0.0) return FALSE;
        if(rData.MatSlab.RC.dFy  <=0.0) return FALSE;
        if(rData.MatSlab.RC.dEc  <=0.0) return FALSE;
        if (rData.MatSlab.RbarD.strCodeName != _T("None"))
        {
            if (rData.MatSlab.RC.dFys <= 0.0) return FALSE;
            if (rData.MatSlab.RC.dEs  <= 0.0) return FALSE;
            if (rData.MatSlab.RC.dEss <= 0.0) return FALSE;
        }
    }
    return TRUE;
}

void CDgnCsgDataCtrl::ConvertCsgCalcMatlStlDgn(T_MATD_ALL &MatlDgn, T_DGN_MATL_STEEL &rData)
{  
    ConvertCsgCalcMatlStlPart(MatlDgn.Analysis, MatlDgn.Design, rData);
}
void CDgnCsgDataCtrl::ConvertCsgCalcMatlStlPart(T_MATL_ANALYSIS &AnalMatl, T_MATL_DESIGN &DgnMatl, T_DGN_MATL_STEEL &rData)
{
    rData.Elast       = AnalMatl.Elast;
	rData.Shear       = AnalMatl.ShearModulus;
    rData.Poisson     = AnalMatl.Poisson;
    rData.Thermal     = AnalMatl.Thermal;
    rData.Density     = AnalMatl.Density;
    rData.MassDensity = AnalMatl.MassDensity;
    rData.S_Fu        = DgnMatl.S_Fu;
    rData.S_Fy        = DgnMatl.S_Fy;
    rData.S_Fy1       = DgnMatl.S_Fy1;
    rData.S_Fy2       = DgnMatl.S_Fy2;
    rData.S_Fy3       = DgnMatl.S_Fy3;
    rData.S_Fy4       = DgnMatl.S_Fy4;
    rData.S_Fy5       = DgnMatl.S_Fy5;
    rData.S_Fy6       = DgnMatl.S_Fy6;
}

void CDgnCsgDataCtrl::ConvertCsgOrgSectData(int nPos, T_SECT_D *pSectD, CSG_SECT_POSD &rData)
{
    if (pSectD==NULL) return;
    if (nPos<0 || nPos>1) return;

    int nStype = pSectD->nStype;
    if (pSectD->nStype==D_SECT_TYPE_TAPERED) nStype = pSectD->SectBefore.nStype;
    rData.SectInfo.nStype = GetConvertSectTypeCvlToEng(nStype);
    rData.SectInfo.SName  = pSectD->SName;
    rData.SectInfo.Shape  =  GetConvertSectShapeCvlToEng(pSectD->SectBefore.Shape);
    rData.SectInfo.nOffsetPoint = pSectD->SectBefore.nOffsetPoint;
    rData.SectInfo.nLytype      = pSectD->SectBefore.nLytype;
    rData.SectInfo.nLztype      = pSectD->SectBefore.nLztype;

    if (nPos==0)
    {
        ConvertCsgSectBaseData(&pSectD->SectBefore.SectI, rData.SectInfo.Before);
        if (pSectD->nStype==D_SECT_TYPE_TAPERED)
        {
            ConvertSectStiffnessData(pSectD->SectBefore.SectI.Stiffness, rData.SectInfo.Before.Stiffness);
            rData.SectInfo.Before.CorrStiffness = rData.SectInfo.Before.Stiffness;
        }
        else
        {
            switch (nStype)
            {
            case D_SECT_TYPE_REGULAR:
            case D_SECT_TYPE_STLG_I:
            case D_SECT_TYPE_STLG_B:
                ConvertSectStiffnessData(pSectD->SectBefore.SectI.Stiffness, rData.SectInfo.Before.Stiffness);
                rData.SectInfo.Before.CorrStiffness = rData.SectInfo.Before.Stiffness; // 각각 경우 확인...
                break;
            default:
                ConvertSectStiffnessData(pSectD->SectBefore.Stiffness, rData.SectInfo.Before.Stiffness);
                rData.SectInfo.Before.CorrStiffness = rData.SectInfo.Before.Stiffness;
                break;
            }            
        }


        ConvertCsgSectBaseData(&pSectD->SectAfter.SectJ, rData.SectInfo.Slab);
    }
    else if (nPos==1)
    {
        if (pSectD->nStype==D_SECT_TYPE_TAPERED) 
        {
            ConvertCsgSectBaseData(&pSectD->CmpTapJ, rData.SectInfo.Before);
            ConvertSectStiffnessData(pSectD->SectBefore.SectJ.Stiffness, rData.SectInfo.Before.Stiffness);
            rData.SectInfo.Before.CorrStiffness = rData.SectInfo.Before.Stiffness;
        }
        else 
        {
            ConvertCsgSectBaseData(&pSectD->SectBefore.SectI, rData.SectInfo.Before);
            switch (nStype)
            {
            case D_SECT_TYPE_REGULAR:
            case D_SECT_TYPE_STLG_I:
            case D_SECT_TYPE_STLG_B:
                ConvertSectStiffnessData(pSectD->SectBefore.SectI.Stiffness, rData.SectInfo.Before.Stiffness);
                rData.SectInfo.Before.CorrStiffness = rData.SectInfo.Before.Stiffness;
                break;
            default:
                ConvertSectStiffnessData(pSectD->SectBefore.Stiffness, rData.SectInfo.Before.Stiffness);
                rData.SectInfo.Before.CorrStiffness = rData.SectInfo.Before.Stiffness;
                break;
            }
        }

        ConvertCsgSectBaseData(&pSectD->SectAfter.SectJ, rData.SectInfo.Slab);
    }	

    //PMS:LRFD12 BSC-20140205
    if (nPos==0)
    {
        ConvertCsgSectBaseData(&pSectD->SectAfter.SectI, rData.SectInfo.After);
    }
    else if (nPos==1)
    {
        if (pSectD->nStype==D_SECT_TYPE_TAPERED) 
            ConvertCsgSectBaseData(&pSectD->SectAfter.SectJ, rData.SectInfo.After);
        else 
            ConvertCsgSectBaseData(&pSectD->SectAfter.SectI, rData.SectInfo.After);		
    }	

    // **********************
    // 삽도 데이터
    // **********************
    ConvertCsgSectPolygonData(nPos, pSectD, rData.SectInfo.After);

}

BOOL CDgnCsgDataCtrl::ConvertCsgSectPolygonData(int nPos, T_SECT_D *pSectD, ST_SECT_SECTBASE_D &rData)
{
    CDb_DllManager_Base DllMgr;
    DGN_GSEC_SHAPE_D SectD; SectD.Initialize();
    int nPosConv = (nPos==0)? 0 : 4;
    if(!DllMgr.ConvertToDLL_SectData_Pos(*pSectD, nPosConv, SectD, TRUE, TRUE)) return FALSE;

    DGN_GSEC_POLYGON_LIST    arConcOutPoly;       arConcOutPoly.RemoveAll(); // 콘크리트 슬래브
    DGN_GSEC_POLYGON_LIST    arConcInPoly;        arConcInPoly.RemoveAll();
    DGN_GSEC_POLYGON_LIST    arSteelOutPoly;      arSteelOutPoly.RemoveAll(); // Steel 거더
    DGN_GSEC_POLYGON_LIST    arSteelInPoly;       arSteelInPoly.RemoveAll();
    DGN_GSEC_LINE_LIST       arSteelLine;         arSteelLine.RemoveAll();
    DGN_GSEC_VERTEXLINE_LIST arSteelVertexLine;   arSteelVertexLine.RemoveAll();
    DGN_GSEC_POLYGON_LIST    arSteelLinePoly;     arSteelLinePoly.RemoveAll();
    double dOutPos[6];
    for(int i=0 ; i<6 ; i++) dOutPos[i] = 0.0;
    BOOL bLineDetail = TRUE;

    if(!dgn::link::ITmpDgnDrawLib::GetInstance()->Get_SectionPolygonData(SectD, arSteelOutPoly, arSteelInPoly, arConcOutPoly, arConcInPoly, arSteelLine, arSteelVertexLine, arSteelLinePoly, dOutPos, -SectD.dYBar, -SectD.dZBar, bLineDetail)) return FALSE;

    // Outpoly  1:슬래브, 나머지 :steel
    int	nOutSize1 = arConcOutPoly.GetSize();
    int nOutSize2 = arSteelOutPoly.GetSize();
    rData.aOuterPolygon.SetSize(nOutSize1+nOutSize2);

    // 슬래브
    for(int i=0 ; i<nOutSize1 ; i++)
    {
        DGN_GSEC_POLYGON  POLYGON;
        int nSize = arConcOutPoly[i].aVertex.GetSize();
        rData.aOuterPolygon[i].aVertex.SetSize(nSize);
		rData.aOuterPolygon[i].nMatkK = 1; // Concrete for Drawing
        for(int j=0 ; j<nSize ; j++)
        {
            rData.aOuterPolygon[i].aVertex[j].dX = arConcOutPoly[i].aVertex[j].dX;
            rData.aOuterPolygon[i].aVertex[j].dY = arConcOutPoly[i].aVertex[j].dY;
        }
    }

    int nTotOut=0; 
    // 거더
    for(int i=0 ; i<nOutSize2 ; i++)
    {
        nTotOut =nOutSize1+i;
        DGN_GSEC_POLYGON  POLYGON;
        int nSize = arSteelOutPoly[i].aVertex.GetSize();
        rData.aOuterPolygon[nTotOut].aVertex.SetSize(nSize);
        rData.aOuterPolygon[nTotOut].nMatkK = 2; // Steel for Drawing
        for(int j=0 ; j<nSize ; j++)
        {
            rData.aOuterPolygon[nTotOut].aVertex[j].dX = arSteelOutPoly[i].aVertex[j].dX;
            rData.aOuterPolygon[nTotOut].aVertex[j].dY = arSteelOutPoly[i].aVertex[j].dY;
        }
    }


    return TRUE;
}

void CDgnCsgDataCtrl::ConvertCsgSectBaseData(T_SECT_SECTBASE_D *pSectD, ST_SECT_SECTBASE_D &rData)
{
    if (pSectD==NULL) return;

    rData.Shape  = pSectD->Shape;
    rData.DBName = pSectD->DBName;
    rData.SName  = pSectD->SName;
    rData.SName2 = pSectD->SName2;
    rData.nSymmType = pSectD->bSymAutoCalc ? 0 : 1;

    for(int i=0; i<D_SECT_SIZE_NUM_MAX; ++i)
    {
        rData.Size[i] = pSectD->Size[i];
    }  
	double dHw = pSectD->Size[0];
    ConvertCsgStiffenerData(dHw, &pSectD->Stiffener, rData.Stiffener);

}

void CDgnCsgDataCtrl::ConvertCsgStiffenerData(double dHw, T_SECT_STIFF_D *pStfn, ST_SECT_STIFF_D &rData)
{
    if (pStfn==NULL) return;

    rData.nPosition = pStfn->nPosition;
    memcpy(rData.nStiffNum, pStfn->nStiffNum, sizeof(rData.nStiffNum));
    int nShapeSize = pStfn->aStiffShape.GetSize();
    if (nShapeSize>0) rData.aStiffShape.SetSize(nShapeSize);
    for (int i=0; i<nShapeSize; ++i)
    {
        ConvertCsgStfnShapeData(&pStfn->aStiffShape[i], rData.aStiffShape[i]);
    }

    ConvertCsgStfnProfileData(pStfn->aStiffLeft, rData.aStiffLeft);
    ConvertCsgStfnProfileData(pStfn->aStiffRight, rData.aStiffRight);
    ConvertCsgStfnProfileData(pStfn->aStiffBottom, rData.aStiffBottom);
    ConvertCsgStfnProfileData(pStfn->aStiffTop, rData.aStiffTop);
	if (pStfn->nRefPos2 == 1)
	{
		ConvertCsgStfnProfileDataBottom2Top(dHw, rData.aStiffLeft);
		ConvertCsgStfnProfileDataBottom2Top(dHw, rData.aStiffRight);
	}
}

void CDgnCsgDataCtrl::ConvertCsgStfnShapeData(T_STIFF_SHAPE_D *pShape, ST_STIFF_SHAPE_D &rData)
{
    if (pShape==NULL) return;

    rData.strName = pShape->strName;
    rData.nType   = pShape->nType;
    memcpy(rData.dSize,pShape->dSize,sizeof(rData.dSize));  
}

void CDgnCsgDataCtrl::ConvertCsgStfnProfileData(CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStfnPro, CArray<ST_STIFF_PROFILE_D,ST_STIFF_PROFILE_D&> &arData)
{
    int nProfileSize = aStfnPro.GetSize();
    if (nProfileSize<=0) return;

    arData.SetSize(nProfileSize);
    for (int i=0; i<nProfileSize; ++i)
    {
        ConvertCsgStfnProfileData(&aStfnPro[i], arData[i]);
    }
}

void CDgnCsgDataCtrl::ConvertCsgStfnProfileData(T_STIFF_PROFILE_D *pStfnProf, ST_STIFF_PROFILE_D &rData)
{
    if (pStfnProf==NULL) return;
    rData.dSpacing = pStfnProf->dSpacing;
    rData.nShape   = pStfnProf->nShape;
    rData.bCalc    = pStfnProf->bCalc;	
}

void CDgnCsgDataCtrl::ConvertCsgStfnProfileDataBottom2Top(double dHw, CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&> &arData)
{
	CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&> arDataTemp;
	arDataTemp.Copy(arData);
	arData.RemoveAll();

	int nSize = arDataTemp.GetSize();
	double dSpaseSum = 0.0;
	for (int i =0; i < nSize; i++)
	{
		dSpaseSum += arDataTemp[i].dSpacing;
	}

	double dSpacePre = 0.0;
	for (int i = nSize-1; i >=0 ; --i)
	{
		dSpacePre = arDataTemp[i].dSpacing;
		if(i == nSize - 1)
		{
			arDataTemp[i].dSpacing = dHw - dSpaseSum;
		}
		else
		{
			arDataTemp[i].dSpacing = dSpacePre;
		}

		arData.Add(arDataTemp[i]);
	}

}

void CDgnCsgDataCtrl::ConvertCsgCalcSectData(CPG_SECT_POSI *pSectPos, CSG_SECT_POSD &rData)
{
    if (pSectPos==NULL) return;

    ConvertSectStiffnessData(pSectPos->PureSect, rData.PureSect);
    ConvertSectStiffnessData(pSectPos->RebarSect, rData.RbarStiffn);
    ConvertSectStiffnessData(pSectPos->NonConSect, rData.NonSlabStiff);
    ConvertSectStiffnessData(pSectPos->RbarStiff3n, rData.RbarStiff3n);
    ConvertSectStiffnessData(pSectPos->NonSlabStiff2, rData.NonSlabStiff2);

}

void CDgnCsgDataCtrl::ConvertCsgCalcCgtsData(T_CGTS_SSM_D *pCgtsSSM, CSG_TRST_SECT &rData)
{
    if (pCgtsSSM==NULL) return;

    ConvertCsgCalcCgtsPosData(pCgtsSSM->aStiffener[0], rData.TrstPos[0]);
    if (pCgtsSSM->bSameJ==TRUE)
    {
        rData.TrstPos[1] = rData.TrstPos[0];    
    }
    else
    {
        ConvertCsgCalcCgtsPosData(pCgtsSSM->aStiffener[1], rData.TrstPos[1]);
    }
}

void CDgnCsgDataCtrl::ConvertCsgCalcCgtsPosData(T_CGTS_SUB_SSM &CgtsSub, CSG_TRST_POSD &rData)
{
    rData.bUseStifTop       = CgtsSub.bUseStifTop      ;       
    rData.nStiffenerTypeTop = CgtsSub.nStiffenerTypeTop;    
    rData.nStifNumTop       = CgtsSub.nStifNumTop      ;          
    rData.dPitchTop         = CgtsSub.dPitchTop        ;    
    memcpy(rData.dSizeTop, CgtsSub.dSizeTop, sizeof(rData.dSizeTop));
    rData.dFyTop            = CgtsSub.dFyTop;   

    rData.bUseStifBot       = CgtsSub.bUseStifBot      ;       
    rData.nStiffenerTypeBot = CgtsSub.nStiffenerTypeBot;    
    rData.nStifNumBot       = CgtsSub.nStifNumBot      ;          
    rData.dPitchBot         = CgtsSub.dPitchBot        ;    
    memcpy(rData.dSizeBot, CgtsSub.dSizeBot, sizeof(rData.dSizeBot));
    rData.dFyBot            = CgtsSub.dFyBot;   

    rData.bUseStifWeb       = CgtsSub.bUseStifWeb      ;       
    rData.nStiffenerTypeWeb = CgtsSub.nStiffenerTypeWeb;    
    rData.nStifNumWeb       = CgtsSub.nStifNumWeb      ;          
    rData.dPitchWeb         = CgtsSub.dPitchWeb        ;    
    memcpy(rData.dSizeWeb, CgtsSub.dSizeWeb, sizeof(rData.dSizeWeb));
    rData.dFyWeb            = CgtsSub.dFyWeb;   

    rData.bUseStifBearing     = CgtsSub.bUseStifBearing;  
    rData.nBearingType        = CgtsSub.nBearingType;        
    rData.nBearingPairNum     = CgtsSub.nBearingPairNum;     
    rData.dPitchBearing       = CgtsSub.dPitchBearing;       
    memcpy(rData.dSizeBearing, CgtsSub.dSizeBearing, sizeof(rData.dSizeBearing));
    rData.dFyBearing          = CgtsSub.dFyBearing;
    rData.dClip4Fillet        = CgtsSub.dClip4Fillet;

	auto _L_EnPos = [&](int nPos) -> enTSReinSectPosition
		{
			switch (nPos)
			{
			case 0: return enTSReinSectPosition::EN_TS_REIN_SECT_POS_1;
			case 1: return enTSReinSectPosition::EN_TS_REIN_SECT_POS_2;
			case 2: return enTSReinSectPosition::EN_TS_REIN_SECT_POS_3;
			case 3: return enTSReinSectPosition::EN_TS_REIN_SECT_POS_4;
            case 4: return enTSReinSectPosition::EN_TS_REIN_SECT_POS_5;
            case 5: return enTSReinSectPosition::EN_TS_REIN_SECT_POS_6;
            case 6: return enTSReinSectPosition::EN_TS_REIN_SECT_POS_7;
            case 7: return enTSReinSectPosition::EN_TS_REIN_SECT_POS_8;
			default: ASSERT(0); return enTSReinSectPosition::EN_TS_REIN_SECT_POS_1;
			}
		};

    if (m_nDgnCode==STL_NR_GN_CIV_025_06 || m_nDgnCode==NR_GN_CIV_025_06_CSG || m_nDgnCode==BS5400_3_00_CSG)
    {
        // Web
		const T_CGTS_SUB_STIFFENER& TSWeb = CgtsSub.StiffNR_Web;
        rData.bSymmWeb          = TSWeb.bSymmWeb; // 
//  	rData.nStiffenerTypeWeb = TSWeb.nStiffenerTypeUp;
//      rData.nStiffenerTypeWeb2= TSWeb.nStiffenerTypeLow;
        rData.nStifNumWeb       = TSWeb.nPairNum;
        rData.dPitchWeb         = TSWeb.dPitch;
        rData.dFyWeb            = TSWeb.dFy;
		rData.dFilletWeldsWeb   = TSWeb.dFilletWeld;
		rData.bLocatedUFrameWeb = TSWeb.bUframeUse;
        rData.dThetaWeb         = TSWeb.dTheta;
        rData.nNumUFrameWeb     = TSWeb.nNumUftame;
//         for (int i=0; i<TSWeb.aStiffShape.GetSize(); ++i)
//         {
// 			const T_STIFF_SHAPE_D& TSShape = TSWeb.aStiffShape[i];
//             // T. Stiffener
//             if (TSShape.nType == 0 || TSShape.nType == 1) // TSWeb.aStiffShape에 0 또는 1만 있다고 합니다.
//             { 
// 				rData.nStiffenerTypeWeb = TSShape.nType;
//                 memcpy(rData.dSizeWeb, TSShape.dSize, sizeof(rData.dSizeWeb));
//                 break;
//             }
//         }
        for (int i = 0; i < TSWeb.aStiffShape.GetSize(); ++i)
        {
            const T_STIFF_SHAPE_D& TSShape = TSWeb.aStiffShape[i];
			if (TSWeb.strShapeNameUp == TSShape.strName)
			{
				rData.nStiffenerTypeWeb = TSShape.nType;
				memcpy(rData.dSizeWeb, TSShape.dSize, sizeof(rData.dSizeWeb));
			}
			if (TSWeb.strShapeNameLow == TSShape.strName)
			{
				rData.nStiffenerTypeWeb2 = TSShape.nType;
				memcpy(rData.dSizeWeb2, TSShape.dSize, sizeof(rData.dSizeWeb2));
			}
        }

		for (int i=0; i<TSWeb.aStiffPosD.GetSize(); ++i)
		{
			const T_SECT_STIFF_POS_D& RivetB = TSWeb.aStiffPosD[i];
			
            for (int j=0; j<TSWeb.aStiffShape.GetSize(); ++j)
            {
                const T_STIFF_SHAPE_D& TSShape = TSWeb.aStiffShape[j];
				// TL=1, TR=2, BR=3, BR=4
                if (RivetB.strShapeName.CompareNoCase(TSShape.strName)==0)
                {
                    ST_REINFORCED_SECT_B ReinS;
					memcpy(ReinS.Size, TSShape.dSize, sizeof(TSShape.dSize));
                    if (TSShape.nType != 3)
                    {
                        ASSERT(0);
                    }
                    ReinS.Shape = DGN_SECT_SHAPE_REG_L;
                    ReinS.SName = RivetB.strShapeName;                    
                    ReinS.enTSPos = _L_EnPos(i);
					ReinS.bUse = RivetB.bUseStiff;

                    T_SECT_D CalcS; CalcS.Initialize();
                    T_SECT_STIFFNESS& StfnStiff = CalcS.SectBefore.SectI.Stiffness;
                    CalcS.nStype = D_SECT_TYPE_REGULAR;
                    memcpy(CalcS.SectBefore.SectI.Size, TSShape.dSize, sizeof(TSShape.dSize));
                    if (TSShape.nType == 3)
                    {
                        CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_L;
                    }
                    else
                        ASSERT(0);
                    CSectUtil::CalcSectData(CalcS);
                    ConvertSectStiffnessData(StfnStiff, ReinS.Stiffness);
                    ConvertSectStiffnessData(StfnStiff, ReinS.CorrStiffness);

                    rData.aAngleSectWeb.Add(ReinS);
                }
            }
		}

        // Bearing
        const T_CGTS_SUB_STIFFENER& TBearing = CgtsSub.StiffNR_Bearing;
//      rData.nBearingType = TBearing.nStiffenerTypeUp;
        rData.bSymmBearing = TBearing.bSymmWeb;
        rData.nBearingPairNum = TBearing.nPairNum;
        rData.dPitchBearing = TBearing.dPitch;
        rData.dFyBearing = TBearing.dFy;
        rData.dClip4Fillet = TBearing.dFilletWeld;
        rData.bLocatedUFrameBearing = TBearing.bUframeUse;
        rData.daBearing = TBearing.da;
        rData.deBearing = TBearing.de;
		rData.dThetaBearing = TBearing.dTheta_d;
//         for (int i = 0; i < TBearing.aStiffShape.GetSize(); ++i)
//         {
//             const T_STIFF_SHAPE_D& TSShape = TBearing.aStiffShape[i];
//             // T. Stiffener
//             if (TSShape.nType == 0 || TSShape.nType == 1) // TBearing.aStiffShape에 0 또는 1만 있다고 합니다.
//             {
//                 rData.bSymmBearing = true;
//                 rData.nBearingType = TSShape.nType;
//                 rData.nBearingType2 = TSShape.nType;
//                 memcpy(rData.dSizeBearing, TSShape.dSize, sizeof(rData.dSizeBearing));
//                 memcpy(rData.dSizeBearing2, TSShape.dSize, sizeof(rData.dSizeBearing2));
//                 break;
//             }
//         }

		for (int i = 0; i < TBearing.aStiffShape.GetSize(); ++i)
		{
			const T_STIFF_SHAPE_D& TSShape = TBearing.aStiffShape[i];
            if (TBearing.strShapeNameUp == TSShape.strName)
            {
                rData.nBearingType = TSShape.nType;
                memcpy(rData.dSizeBearing, TSShape.dSize, sizeof(rData.dSizeBearing));
            }
            if (TBearing.strShapeNameLow == TSShape.strName)
            {                
                rData.nBearingType2 = TSShape.nType;
				memcpy(rData.dSizeBearing2, TSShape.dSize, sizeof(rData.dSizeBearing2));				
			}
		}

        for (int i = 0; i < TBearing.aStiffPosD.GetSize(); ++i)
        {
            const T_SECT_STIFF_POS_D& RivetB = TBearing.aStiffPosD[i];

            for (int j = 0; j < TBearing.aStiffShape.GetSize(); ++j)
            {
                const T_STIFF_SHAPE_D& TSShape = TBearing.aStiffShape[j];
                // TL=1, TR=2, BR=3, BR=4
                if (RivetB.strShapeName.CompareNoCase(TSShape.strName) == 0)
                {
                    ST_REINFORCED_SECT_B ReinS;
                    memcpy(ReinS.Size, TSShape.dSize, sizeof(TSShape.dSize));
                    if (TSShape.nType != 3)
                    {
                        ASSERT(0);
                    }
                    ReinS.Shape = DGN_SECT_SHAPE_REG_L;
                    ReinS.SName = RivetB.strShapeName;
                    ReinS.enTSPos = _L_EnPos(i);
                    ReinS.bUse = RivetB.bUseStiff;

                    T_SECT_D CalcS; CalcS.Initialize();
                    T_SECT_STIFFNESS& StfnStiff = CalcS.SectBefore.SectI.Stiffness;
                    CalcS.nStype = D_SECT_TYPE_REGULAR;
                    memcpy(CalcS.SectBefore.SectI.Size, TSShape.dSize, sizeof(TSShape.dSize));
                    if (TSShape.nType == 3)
                    {
                        CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_L;
                    }
                    else
                        ASSERT(0);
                    CSectUtil::CalcSectData(CalcS);
                    ConvertSectStiffnessData(StfnStiff, ReinS.Stiffness);
                    ConvertSectStiffnessData(StfnStiff, ReinS.CorrStiffness);

                    rData.aAngleSectBearing.Add(ReinS);
                }
            }
        }

    }

}

void CDgnCsgDataCtrl::ConvertCsgCalcTrstData(CPG_TRST_POSI *pTrstD, CSG_TRST_POSD &rData)
{
    if (pTrstD==NULL) return;

}

void CDgnCsgDataCtrl::ConvertCsgCalcTsesData(CPG_TSES_POSI *pTsesD, CSG_TSES_POSD& rData)
{
    if (pTsesD==NULL) return;

}

void CDgnCsgDataCtrl::ConvertCsgCalcCordData(int nPos, T_CRDP_SSM_D* pCrdp, ST_SECT_SECTBASE_D& rData)
{
	if (pCrdp == NULL) return;
	if (nPos<0 || nPos>1) return;
    
    int nID = pCrdp->bSameIJ ? 0 : nPos;

	int nSize = pCrdp->CrdpSub[nID].GetSize();
	rData.aCorrDepth.SetSize(nSize);
    
    for (int i=0; i<pCrdp->CrdpSub[nID].GetSize(); ++i)
    {
		ConvertCsgCalcCordPosData(pCrdp->CrdpSub[nID][i], rData.aCorrDepth[i]);
    }
}

void CDgnCsgDataCtrl::ConvertCsgCalcCordPosData(T_CRDP_SUB_SSM& CrdpSub, ST_CORR_DEPTH_B& rData)
{
    rData.enPart = [](int nComp)
    {
		switch (nComp)
		{
		case 0:	return enCorrosionPart::EN_Corr_Part_Top_Flange;			
		case 1: return enCorrosionPart::EN_Corr_Part_Web;
		case 2: return enCorrosionPart::EN_Corr_Part_Bottom_Flange;
		case 3: return enCorrosionPart::EN_Corr_Part_Angle_Pos1;
		case 4: return enCorrosionPart::EN_Corr_Part_Angle_Pos2;
		case 5: return enCorrosionPart::EN_Corr_Part_Angle_Pos3;
		case 6: return enCorrosionPart::EN_Corr_Part_Angle_Pos4;
		case 7: return enCorrosionPart::EN_Corr_Part_Angle_Pos5;
		case 8: return enCorrosionPart::EN_Corr_Part_Angle_Pos6;
		case 9: return enCorrosionPart::EN_Corr_Part_Angle_Pos7;
		case 10: return enCorrosionPart::EN_Corr_Part_Angle_Pos8;
        default: ASSERT(0);	return enCorrosionPart::EN_Corr_Part_Top_Flange;
		}
    }(CrdpSub.nComp);
    
    rData.enType = [](int nComp, int nPos)
    {
		auto L_GetPosTypeFlg = [](int nPos)
		{
			switch (nPos)
			{
			case 0: return enCorrDepthType::EN_Corr_Depth_Type_Flange_Top;
			case 1: return enCorrDepthType::EN_Corr_Depth_Type_Flange_Bottom;
			case 2: return enCorrDepthType::EN_Corr_Depth_Type_Flange_Left_Edge;
			case 3: return enCorrDepthType::EN_Corr_Depth_Type_Flange_Right_Edge;
			default: ASSERT(0); return enCorrDepthType::EN_Corr_Depth_Type_Flange_Top;
			}
		};

		auto L_GetPosTypeWeb = [](int nPos)
		{
			switch (nPos)
			{
			case 0: return enCorrDepthType::EN_Corr_Depth_Type_Web_Left;
			case 1: return enCorrDepthType::EN_Corr_Depth_Type_Web_Right;
			case 2: return enCorrDepthType::EN_Corr_Depth_Type_Web_Left_And_Right;
			default: ASSERT(0); return enCorrDepthType::EN_Corr_Depth_Type_Web_Left;
			}
		};

		auto L_GetPosTypeAng = [](int nPos)
		{
			switch (nPos)
			{
			case 0: return enCorrDepthType::EN_Corr_Depth_Type_Angle_Ver;
			case 1: return enCorrDepthType::EN_Corr_Depth_Type_Angle_Hor;
			default: ASSERT(0); return enCorrDepthType::EN_Corr_Depth_Type_Angle_Ver;
			}
		};

		switch (nComp)
		{
		case 0:
		case 2:
			return L_GetPosTypeFlg(nPos);
		case 1:
			return L_GetPosTypeWeb(nPos);
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
            return L_GetPosTypeAng(nPos);
		default:
			ASSERT(0);
			return L_GetPosTypeFlg(nPos);
		}
    }(CrdpSub.nComp, CrdpSub.nPos);
    
    rData.dDepth = CrdpSub.dCorrDepth;
}

void CDgnCsgDataCtrl::ConvertCsgCalcUlmForcePos3(int nPosI, int nPosM, int nPosJ, _DGN_FORC_CRC &ForI, _DGN_FORC_CRC &ForM, _DGN_FORC_CRC &ForJ,
                                             CSG_ULMF_D &rData)
{
    if (nPosI<0 || nPosI>1) return;
    if (nPosM<0 || nPosM>1) return;
    if (nPosJ<0 || nPosJ>1) return;

    ConvertCsgCalcForce(nPosI, ForI, rData.ForD[0]);
    ConvertCsgCalcForce(nPosM, ForM, rData.ForD[1]);
    ConvertCsgCalcForce(nPosJ, ForJ, rData.ForD[2]);
}

void CDgnCsgDataCtrl::ConvertCsgCalcUblMForce(int nPos, T_STRB_D &ForD, CSG_FORCE_D &rData) const
{
    if (nPos<0 || nPos>5) return;
    int nElemSub = 0;
    if (nPos==0) nElemSub = 0;
    else if (nPos==1) nElemSub = 1;
    else if (nPos==2) nElemSub = 2;
    else if (nPos==3) nElemSub = 3;
    else if (nPos==4) nElemSub = 3;

    int nKind = (nPos==4) ? 0 : 6;
    int nW    = (nPos==4) ? 12 : 15;
    rData.dForce[0] = ForD.dblForce[nElemSub][nKind + 0];
    rData.dForce[1] = ForD.dblForce[nElemSub][nKind + 1];
    rData.dForce[2] = ForD.dblForce[nElemSub][nKind + 2];
    rData.dForce[3] = ForD.dblForce[nElemSub][nKind + 3];
    rData.dForce[4] = ForD.dblForce[nElemSub][nKind + 4];
    rData.dForce[5] = ForD.dblForce[nElemSub][nKind + 5];
    //
    rData.dForce[6] = ForD.dblForce[nElemSub][nW + 0];
    rData.dForce[7] = ForD.dblForce[nElemSub][nW + 1];
    rData.dForce[8] = ForD.dblForce[nElemSub][nW + 2];
}

void CDgnCsgDataCtrl::ConvertCsgCalcUblMForce4Pos(int nPos, T_STRB_D& ForD, CSG_FORCE_D& rData) const
{
    if (nPos < 0 || nPos>5) return;
    int nElemSub = 0;
    if (nPos == 0) nElemSub = 0;
    else if (nPos == 1) nElemSub = 1;
    else if (nPos == 2) nElemSub = 2;
    else if (nPos == 3) nElemSub = 3;
    else if (nPos == 4) nElemSub = 3;

    int nKind = 0;
    int nW = 12;
    if (nPos == 0)
    {
        nKind = 0;
        nW = 12;
    }
    else if (nPos == 4)
    {
        nKind = 6;
        nW = 15;
    }
    else
    {
        nKind = (nPos == 1 || nPos == 2) ? 0 : 6;
        nW = (nPos == 1 || nPos == 2) ? 12 : 15;
    }

    rData.dForce[0] = ForD.dblForce[nElemSub][nKind + 0];
    rData.dForce[1] = ForD.dblForce[nElemSub][nKind + 1];
    rData.dForce[2] = ForD.dblForce[nElemSub][nKind + 2];
    rData.dForce[3] = ForD.dblForce[nElemSub][nKind + 3];
    rData.dForce[4] = ForD.dblForce[nElemSub][nKind + 4];
    rData.dForce[5] = ForD.dblForce[nElemSub][nKind + 5];
    //
    rData.dForce[6] = ForD.dblForce[nElemSub][nW + 0];
    rData.dForce[7] = ForD.dblForce[nElemSub][nW + 1];
    rData.dForce[8] = ForD.dblForce[nElemSub][nW + 2];
}

void CDgnCsgDataCtrl::ConvertCsgCalcChkForce(int nPos, _DGN_FORC_CRC &ForNC, _DGN_FORC_CRC &ForLT, _DGN_FORC_CRC &ForST, _DGN_FORC_CRC &ForLC, CSG_FORCE_COMP &rData)
{
    ConvertCsgCalcForce(nPos, ForNC, rData.ForNC);
    ConvertCsgCalcForce(nPos, ForLT, rData.ForLT);
    ConvertCsgCalcForce(nPos, ForST, rData.ForST);
    ConvertCsgCalcForce(nPos, ForLC, rData.ForLC);
}

void CDgnCsgDataCtrl::ConvertCsgCalcChkForce4Stage(int nPos, _DGN_FORC_CRC &ForNC, CSG_FORCE_COMP &rData)
{
    ConvertCsgCalcForce(nPos, ForNC, rData.ForNC);
}

void CDgnCsgDataCtrl::ConvertCsgCalcForce(int nPos, _DGN_FORC_CRC &For, CSG_FORCE_D &rData)
{
    if (nPos<0 || nPos>1) return;

    rData.dForce[0] = For.dFxx[nPos];
    rData.dForce[1] = For.dFyy[nPos];
    rData.dForce[2] = For.dFzz[nPos];
    rData.dForce[3] = For.dMux[nPos];
    rData.dForce[4] = For.dMuy[nPos];
    rData.dForce[5] = For.dMuz[nPos];
    rData.dForce[6] = For.dMbi[nPos];
    rData.dForce[7] = For.dTst[nPos];
    rData.dForce[8] = For.dTwp[nPos];
}

void CDgnCsgDataCtrl::ConvertCsgCalcForce(int nPos, _DGN_FORC_CRC_EX &For, CSG_FORCE_D &rData)
{
    if ( nPos<0 || nPos>4 ) return;

    rData.dForce[0] = For.dFxx[nPos];
    rData.dForce[1] = For.dFyy[nPos];
    rData.dForce[2] = For.dFzz[nPos];
    rData.dForce[3] = For.dMux[nPos];
    rData.dForce[4] = For.dMuy[nPos];
    rData.dForce[5] = For.dMuz[nPos];
    rData.dForce[6] = For.dMbi[nPos];
    rData.dForce[7] = For.dTst[nPos];
    rData.dForce[8] = For.dTwp[nPos];
}

void CDgnCsgDataCtrl::ConvertCsgCalcForce2(int nPos, CSG_FORCE_D &For, _DGN_FORC_CRC &rData)
{
    if ( nPos<0 || nPos>1 ) return;

    rData.dFxx[nPos] = For.dForce[0];
    rData.dFyy[nPos] = For.dForce[1];
    rData.dFzz[nPos] = For.dForce[2];
    rData.dMux[nPos] = For.dForce[3];
    rData.dMuy[nPos] = For.dForce[4];
    rData.dMuz[nPos] = For.dForce[5];
    rData.dMbi[nPos] = For.dForce[6];
    rData.dTst[nPos] = For.dForce[7];
    rData.dTwp[nPos] = For.dForce[8];
}

void CDgnCsgDataCtrl::ConvertCsgCalcMVLcaseData(double dW, int nLane, CSG_LCOM_MV_IN &rData)
{
    rData.dW    = dW;
    rData.nLane = nLane;
}

void CDgnCsgDataCtrl::ConvertCsgScbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomD, double dRatio_F, T_CSCH_REF &rData)
{
    if (LcomD.aChkFor.GetSize() == 0)  return;
    if (rData.pScbrB==NULL) return;
    if (ResD.pLSD==NULL) return;

    T_SCBR_BASE *pScbr = rData.pScbrB;
    CSG_CONS_RES_KSCE_LSD12 &ConsR = ResD.pLSD->ConsD;

    for (int i = 0; i<6; ++i)
    {
        pScbr->CsFor.dForce[i] = LcomD.aChkFor[0].ForNC.dForce[i];
    }

    for (int i = 0; i<3/*LcomD.aUlmf.GetSize()*/; ++i)
    {
        memcpy(pScbr->UlFor.For[i].dForce, LcomD.aUlmf[0].ForD[i].dForce, sizeof(pScbr->UlFor.For[i].dForce));
        memcpy(pScbr->UlFor.CbFor[i].dForce, LcomD.aUlmf[0].CbForce[i].dForce, sizeof(pScbr->UlFor.CbFor[i].dForce));
    }

	for (int i = 0; i < 2; ++i)
	{
		memcpy(pScbr->UlFor.For13[i].dForce, LcomD.aUlmf[0].For13D[i].dForce, sizeof(pScbr->UlFor.For13[i].dForce));
	}
    pScbr->UlFor.MmaxFor.dForce[4] = LcomD.aUlmFor[0].dMmax;

    int nSetType = LcomD.Memb.PosD[0].Sect.SectInfo.nStype;

    BOOL bChcek = FALSE;
    if     (IsSecTypeI(nSetType))  bChcek = TRUE;
    else if(nSetType==DGN_SECT_TYPE_COMPO_TUB && bPositive) bChcek = TRUE;


    // 복부
    pScbr->dfcw     = ConsR.Flexure.dfcw;       
    pScbr->dfcw_lim = ConsR.Flexure.dfcw_lim;   

    // 압축 플랜지 : I, Tub(정모멘트만)
    pScbr->dfcf     = (bChcek)? ConsR.Flexure.dfc        : 0.0 ;       
    pScbr->dfcf_lim = (bChcek)? ConsR.Flexure.FlexD.dFcr : 0.0 ;       

    // 인장 플랜지 : I
    pScbr->dftf     = (IsSecTypeI(nSetType))? ConsR.Flexure.dft         : 0.0 ;
    pScbr->dftf_lim = (IsSecTypeI(nSetType))?  ConsR.Flexure.FlexD.dFtr : 0.0 ;    

    BOOL Chk1 =  (fabs(pScbr->dfcw) <= pScbr->dfcw_lim);
    BOOL Chk2 =  (fabs(pScbr->dfcf) <= pScbr->dfcf_lim);
    BOOL Chk3 =  (fabs(pScbr->dftf) <= pScbr->dftf_lim);
    pScbr->nOK = (Chk1 && Chk2 && Chk3)? 1 : 0;
    pScbr->dRatio = dRatio_F;
}

void CDgnCsgDataCtrl::ConvertCsgScvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgCsbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgCsvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgCsacRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_Ser, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgFatiLoadRes(int nType, int nPart, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgFatiFlexRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgFatiShearRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgFatiStudRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgFatiSconRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgCscrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ResD, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData) 
{
}

void CDgnCsgDataCtrl::ConvertCsgCscrMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgCsslMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgCsslMembNoLcomRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSBR_D &CsbrResD, T_CSCH_REF &rData)
{

}

void CDgnCsgDataCtrl::ConvertCsgCptsMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgCptsResLcom(CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
}

void CDgnCsgDataCtrl::ConvertCsgBstfMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData)
{
}


// 여기는 지우자..
void CDgnCsgDataCtrl::ConvertCsgCscrMembRes(int nPos, CSG_MEMB_RES_KSCE_LSD12_D &MembRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCR_BASE &rData)
{
    if (nPos<0 || nPos>1) return;

    CSG_CPSC_POSD &SconD = LcomResD.Memb.PosD[nPos].Scon;

    const CSG_MEMB_RES_KSCE_LSD12_D_CSCR &SCmembR = MembRes.Detail_CSCR;
    rData.dH_D     = SCmembR.dH_d;
    rData.dH_D_lim = SCmembR.dH_d_lim;
    rData.dp       = SconD.dPitch;

    rData.dp_lim2  = SCmembR.dpuse_lim1;
    rData.dp_lim3  = SCmembR.dpuse_lim2; 
    rData.dN       = (double)SconD.nNum;
    rData.dNreq    = max(SCmembR.dN_req_po, SCmembR.dN_req_ne);
    BOOL bOK01  = rData.nOK;
    rData.nOK   = bOK01;//(bOK01 && rData.dN>=rData.dNreq)? 1 : 0;

    BOOL bCheck = (SconD.nNum==0 && SconD.dPitch==0.0 && SconD.dSpace==0.0)? FALSE : TRUE;
    rData.bChk  = bCheck;

}

void CDgnCsgDataCtrl::ConvertCsgCsslMembRes(int nPos, T_CSBR_D &CsbrResD, CSG_MEMB_RES_KSCE_LSD12_D &MembRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSSL_BASE &rData)
{
    BOOL bTop=FALSE;
    BOOL bBot=FALSE;
    if(nPos==0) //i
    {
        bTop = CsbrResD.CsbrB[0].bChk;
        bBot = CsbrResD.CsbrB[1].bChk;
    }
    else // j
    {
        bTop = CsbrResD.CsbrB[2].bChk;
        bBot = CsbrResD.CsbrB[3].bChk;
    }

    int nWeb=0;
    CSG_MEMB_RES_KSCE_LSD12_D_CSSL  &LStfnR_Top = (nWeb==0) ? MembRes.Detail_CSSL[0] : MembRes.Detail_CSSL2[0];
    CSG_MEMB_RES_KSCE_LSD12_D_CSSL  &LStfnR_Bot = (nWeb==0) ? MembRes.Detail_CSSL[1] : MembRes.Detail_CSSL2[1];

    CSG_MEMB_RES_KSCE_LSD12_D_CSSL  LStfnR; LStfnR.Initialize();
    int nCSSLType = 0;
    if(bTop && bBot)
    {
        LStfnR    = (LStfnR_Top.ddo >= LStfnR_Bot.ddo)? LStfnR_Top : LStfnR_Bot;
        nCSSLType = (LStfnR_Top.ddo >= LStfnR_Bot.ddo)? 0 : 1;
    }
    else if(bTop)
    {
        LStfnR = LStfnR_Top;
        nCSSLType = 0;
    }
    else if(bBot)
    {
        LStfnR = LStfnR_Bot;
        nCSSLType = 1;
    }
    else ASSERT(0);


    rData.nMax      = nCSSLType; // 0: Top, 1: Bot
    rData.dbl       = LStfnR.dbl;
    rData.dbl_lim   = LStfnR.dbl_lim;    
    rData.dI        = LStfnR.dIl;
    rData.dI_lim    = LStfnR.dIl_lim;   
    rData.dr        = LStfnR.dr_to;
    rData.dr_lim    = LStfnR.dr_lim;

    BOOL bCKk1 = (rData.dbl <=rData.dbl_lim)? TRUE : FALSE;
    BOOL bCKk2 = (rData.dI  >=rData.dI_lim)?  TRUE : FALSE;
    BOOL bCKk3 = (rData.dr  >=rData.dr_lim)?  TRUE : FALSE;

    rData.nOK  = (bCKk1 && bCKk2 && bCKk3) ? 1 : 0;

    rData.bChk = (LStfnR.dbl==0.0 && LStfnR.dtp==0.0)? FALSE : TRUE;
}

void CDgnCsgDataCtrl::WriteResultData()
{
	CPlateGirderDesign *pCheckGirder = m_pDoc->m_pPostCtrl->GetPlateGirder();

	if ( !pCheckGirder ) return;

	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	T_ELEM_K nElemK = GetDgnElemNo();
    ElemPairK EPariK = GetCheckElemPairK();
	
	T_COMPOSITE_STEEL_GIRDER_RES_D* pRes = m_vRes[EPariK];
	if ( CpgdD.bDesignParameter[0] ) pCheckGirder->WriteFlexuralStrengthAS5100_6(EPariK, pRes->AsfcD);
	if ( CpgdD.bDesignParameter[1] ) pCheckGirder->WriteShearStrengthAS5100_6(EPariK, pRes->AsscD);
	if ( CpgdD.bDesignParameter[0] ) pCheckGirder->WriteCombinedMVStrengthAS5100_6(EPariK, pRes->AscbD);
	if ( CpgdD.bDesignParameter[1] ) pCheckGirder->WriteLongShearAS5100_6(EPariK, pRes->AslsD);
	if ( CpgdD.bDesignParameter[0] ) pCheckGirder->WriteFlangeWithLStiffenerBoxShapeAS5100_6(EPariK, pRes->AsfgD);
	if ( CpgdD.bDesignParameter[0] ) pCheckGirder->WriteFlangeStiffenerWithLStiffenerBoxShapeAS5100_6(EPariK, pRes->AsfsD);
	if ( CpgdD.bDesignParameter[0] ) pCheckGirder->WriteWebYieldingWithLStiffenerBoxShapeAS5100_6(EPariK, pRes->AswyD);
	if ( CpgdD.bDesignParameter[0] ) pCheckGirder->WriteWebBucklingWithLStiffenerBoxShapeAS5100_6(EPariK, pRes->AswbD);
	if ( CpgdD.bDesignParameter[0] ) pCheckGirder->WriteWebStiffenerWithLStiffenerBoxShapeAS5100_6(EPariK, pRes->AswsD);
	// SLS
	if ( CpgdD.bDesignParameter[2] ) pCheckGirder->WriteSLSAS5100_6(EPariK, pRes->AsslD);
	if ( CpgdD.bDesignParameter[2] ) pCheckGirder->WriteLongShearSLSAS5100_6(EPariK, pRes->AslsSLS);
	// CS
	if ( CpgdD.bDesignParameter[0] ) pCheckGirder->WriteCSFlexuralStrengthAS5100_6(EPariK, pRes->CSfcD);
	if ( CpgdD.bDesignParameter[1] ) pCheckGirder->WriteCSShearStrengthAS5100_6(EPariK, pRes->CSscD);
	if ( CpgdD.bDesignParameter[0] ) pCheckGirder->WriteCSCombinedMVStrengthAS5100_6(EPariK, pRes->CscbD);
}

// void CDgnCsgDataCtrl::ConvertCsgFlangeClassCSA(CSG_MEMB_RES_CSA_S6_D &MembRes, CSG_CHECK_RES_CSA_S6 &ResD, int &nTopClass, int &nBotClass)
// {
//   nTopClass = MembRes.Detail_F[0].nClassTF;
//   nBotClass = MembRes.Detail_F[0].nClassBF;
// }

void CDgnCsgDataCtrl::ConvertCSGDCOMF(CSG_FORCE_COMP &InD, T_CSGD_COMF &rData)
{
    //if (LcomResD.aChkFor.GetSize() == 0)  return;
    ConvertCSGDForce(InD.ForNC, rData.ForNC);
    ConvertCSGDForce(InD.ForST, rData.ForST);
    ConvertCSGDForce(InD.ForLT, rData.ForLT);
    ConvertCSGDForce(InD.ForLC, rData.ForLC);
}

void CDgnCsgDataCtrl::ConvertCSGDForce(CSG_FORCE_D &InD, T_CSGD_FORCE &rData)
{
    memcpy(rData.dForce, InD.dForce, sizeof(rData.dForce));
}

void CDgnCsgDataCtrl::ConvertCSGEngToUlmForce(CSG_ULMF_D &InD, T_CSGD_ULMF &rData)
{
    for (int i = 0; i<3; ++i)
    {
        ConvertCSGDForce(InD.ForD[i], rData.For[i]);
    }
	for (int i=0; i<2; ++i)
	{
		ConvertCSGDForce(InD.For13D[i], rData.For13[i]);
	}
    ConvertCSGDForce(InD.MyForce, rData.MmaxFor);
}

void CDgnCsgDataCtrl::ConvertCSGEngToUblmForce(BOOL bConsStage,  CSG_MEMB_FOR_D &InD, T_CSGD_ULMF &rData)
{
    if(bConsStage)
    {
        ConvertCSGDForce(InD.ForD[1].ForNC, rData.For[0]);
        ConvertCSGDForce(InD.ForD[2].ForNC, rData.For[1]);
        ConvertCSGDForce(InD.ForD[3].ForNC, rData.For[2]);
    }
    else
    {
        ConvertCSGDForce(InD.ForD[1].ForLC, rData.For[0]);
        ConvertCSGDForce(InD.ForD[2].ForLC, rData.For[1]);
        ConvertCSGDForce(InD.ForD[3].ForLC, rData.For[2]);
    }

}

void CDgnCsgDataCtrl::ConvertCsgEngToUblMembForce(CSGULMFOR_ARRAY& aUlmf, T_CSGF_MEMB& rData)
{
    if ( aUlmf.GetSize()>6 ) return;

    T_CSGD_ULMF Ulmf;
    ConvertCSGEngToUlmForce(aUlmf[0], Ulmf);
    for ( int i=0; i<3; ++i )
    {
        rData.MembFor[i].ForLC = Ulmf.For[i];
    }
    for (int i=0; i<2; ++i)
    {
        rData.MembFor13[i].ForLC = Ulmf.For13[i];
    }
    rData.MyMaxFor.ForLC = Ulmf.MmaxFor;
    
    Ulmf.Initialize();
    ConvertCSGEngToUlmForce(aUlmf[1], Ulmf);
    for ( int i=0; i<3; ++i )
    {
        rData.MembFor[i].ForNC = Ulmf.For[i];
    }
    for (int i=0; i<2; ++i)
    {
        rData.MembFor13[i].ForNC = Ulmf.For13[i];
    }
    rData.MyMaxFor.ForNC = Ulmf.MmaxFor;

    Ulmf.Initialize();
    ConvertCSGEngToUlmForce(aUlmf[2], Ulmf);
    for ( int i=0; i<3; ++i )
    {
        rData.MembFor[i].ForLT = Ulmf.For[i];
    }
    for (int i=0; i<2; ++i)
    {
        rData.MembFor13[i].ForLT = Ulmf.For13[i];
    }
    rData.MyMaxFor.ForLT = Ulmf.MmaxFor;

    Ulmf.Initialize();
    ConvertCSGEngToUlmForce(aUlmf[3], Ulmf);
    for ( int i=0; i<3; ++i )
    {
        rData.MembFor[i].ForST = Ulmf.For[i];
    }
    for (int i=0; i<2; ++i)
    {
        rData.MembFor13[i].ForST = Ulmf.For13[i];
    }
    rData.MyMaxFor.ForST = Ulmf.MmaxFor;

    Ulmf.Initialize();
    ConvertCSGEngToUlmForce(aUlmf[4], Ulmf);
    for (int i = 0; i < 3; ++i)
    {
        rData.MembFor[i].ForMV = Ulmf.For[i];
    }
    for (int i=0; i<2; ++i)
    {
        rData.MembFor13[i].ForMV = Ulmf.For13[i];
    }
    rData.MyMaxFor.ForMV = Ulmf.MmaxFor;

    Ulmf.Initialize();
    ConvertCSGEngToUlmForce(aUlmf[5], Ulmf);
    for (int i = 0; i < 3; ++i)
    {
        rData.MembFor[i].ForSV = Ulmf.For[i];
    }
    for (int i=0; i<2; ++i)
    {
        rData.MembFor13[i].ForSV = Ulmf.For13[i];
    }
    rData.MyMaxFor.ForSV = Ulmf.MmaxFor;
}

void CDgnCsgDataCtrl::ConvertCsgEngToDgnForce(CSG_MEMB_POSD_KSCE_LSD& LcomFor, T_CSGF_MEMB& rData)
{
    if ( LcomFor.aChkFor.GetSize()==0 ) return;
    ConvertCSGDCOMF(LcomFor.aChkFor[0], rData.ChkFor);
    ConvertCSGDForce(LcomFor.aChkFor[0].ForMV, rData.MVFor);
    ConvertCSGDForce(LcomFor.aChkFor[0].ForSV, rData.MVForStd);
    ConvertCsgEngToUblMembForce(LcomFor.aUlmf, rData);
    rData.ElemK4Mymax = LcomFor.Memb.Ulms.nElemK4Mymax;
    rData.SectK4Mymax = LcomFor.Memb.Ulms.nSectK4Mymax;
    rData.nPos4Mymax  = LcomFor.Memb.Ulms.nPos4Mymax;
}

void CDgnCsgDataCtrl::ConvertCsgUlmfCvlToEng(T_CSGD_ULMF &For, CSG_ULMF_D &rData)
{
    ConvertCsgForceCvlToEng( For.For[0], rData.ForD[0]);
    ConvertCsgForceCvlToEng( For.For[1], rData.ForD[1]);
    ConvertCsgForceCvlToEng( For.For[2], rData.ForD[2]);
}
void CDgnCsgDataCtrl::ConvertCsgComfCvlToEng(T_CSGD_COMF &For, CSG_FORCE_COMP &rData)
{
    rData.nLcomSub = For.nFatigueI;
    ConvertCsgForceCvlToEng( For.ForNC, rData.ForNC);
    ConvertCsgForceCvlToEng( For.ForLT, rData.ForLT);
    ConvertCsgForceCvlToEng( For.ForST, rData.ForST);
    ConvertCsgForceCvlToEng( For.ForLC, rData.ForLC);
    ConvertCsgForceCvlToEng( For.ForMV, rData.ForMV);
    ConvertCsgForceCvlToEng( For.ForSV, rData.ForSV);
}

void CDgnCsgDataCtrl::ConvertCsgForceCvlToEng( T_CSGD_FORCE &For, CSG_FORCE_D &rData)
{
    for (int i=0; i<D_CSG_FORCE_COMPONENT; ++i)
    {
        rData.dForce[i] = For.dForce[i];
    }    
}

void CDgnCsgDataCtrl::ConvertCsgUlmForce(int nPos, CSG_FORCE_COMP& For, CSGULMFOR_ARRAY& raData)
{
    if ( nPos<0 || nPos>2 ) return;
    if ( raData.GetSize()>6 ) return;
    
    for ( int i=0; i<D_CSG_FORCE_COMPONENT; ++i )
    {
        raData[0].ForD[nPos].dForce[i] = For.ForLC.dForce[i];
        raData[1].ForD[nPos].dForce[i] = For.ForNC.dForce[i];
        raData[2].ForD[nPos].dForce[i] = For.ForLT.dForce[i];
        raData[3].ForD[nPos].dForce[i] = For.ForST.dForce[i];
        raData[4].ForD[nPos].dForce[i] = For.ForMV.dForce[i];
        raData[5].ForD[nPos].dForce[i] = For.ForSV.dForce[i];
    }    
}

void CDgnCsgDataCtrl::ConvertCsgUlm13Force(int nPos13, CSG_FORCE_COMP& For, CSGULMFOR_ARRAY& raData)
{
    if (nPos13 <0 || nPos13>1 ) return;
    if ( raData.GetSize()>6 ) return;
    
    for ( int i=0; i<D_CSG_FORCE_COMPONENT; ++i )
    {
        raData[0].For13D[nPos13].dForce[i] = For.ForLC.dForce[i];
        raData[1].For13D[nPos13].dForce[i] = For.ForNC.dForce[i];
        raData[2].For13D[nPos13].dForce[i] = For.ForLT.dForce[i];
        raData[3].For13D[nPos13].dForce[i] = For.ForST.dForce[i];
        raData[4].For13D[nPos13].dForce[i] = For.ForMV.dForce[i];
        raData[5].For13D[nPos13].dForce[i] = For.ForSV.dForce[i];
    }    
}

void CDgnCsgDataCtrl::ConvertCsgUlmMyMaxForce(CSG_FORCE_COMP& For, CSGULMFOR_ARRAY& raData)
{
    if ( raData.GetSize()>6 ) return;
    
    for ( int i=0; i<D_CSG_FORCE_COMPONENT; ++i )
    {
        raData[0].MyForce.dForce[i] = For.ForLC.dForce[i];
        raData[1].MyForce.dForce[i] = For.ForNC.dForce[i];
        raData[2].MyForce.dForce[i] = For.ForLT.dForce[i];
        raData[3].MyForce.dForce[i] = For.ForST.dForce[i];
        raData[4].MyForce.dForce[i] = For.ForMV.dForce[i];
        raData[5].MyForce.dForce[i] = For.ForSV.dForce[i];
    }    
}

BOOL CDgnCsgDataCtrl::CheckSupprotNode(int nNode)
{
    BOOL bSupportNode = FALSE;
    CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
    m_pDoc->m_pAttrCtrl->GetConsNodeKeyList(arNodeKeyList);	// 반력있는 절점

    int nSize = arNodeKeyList.GetSize();
    for(int i=0; i<nSize; ++i )
    {
        if(nNode == arNodeKeyList[i])
        {
            bSupportNode = TRUE;
            break;
        }   
    }

    // link는 제외한다 
    BOOL bAppLink = FALSE;
    if ( bAppLink )
    {
        if ( bSupportNode==FALSE )
        {
            BOOL bFix[6]= { FALSE };
            m_pDoc->m_pAttrCtrl->IsDofConstraintForDesign(nNode, bFix, FALSE);
            BOOL bLatFix = (bFix[0] || bFix[1] || bFix[2]);
            BOOL bTorFix = (bFix[3] || bFix[4] || bFix[5]);

            bSupportNode = (bLatFix || bTorFix);
        }
    }

    return bSupportNode;
}


BOOL CDgnCsgDataCtrl::CheckSupprotNodeElem(ElemPairK ElemK,  int* nNode, BOOL bCheckJ)
{
    nNode[0] = 0;  // I
    nNode[1] = 0;  // J

    auto _L_CheckJNode = [&]() -> void
        {
            auto itUblmK = m_pDataCtrl->m_mElemUblmKey.find(ElemK);
            if (itUblmK != m_pDataCtrl->m_mElemUblmKey.end())
            {
                auto itUblMemb = m_pDataCtrl->m_mUblenMemb.find(itUblmK->second);
                if (itUblMemb != m_pDataCtrl->m_mUblenMemb.end())
                {
                    const T_ULEN_MEMB_D& UblmD = itUblMemb->second;
                    int nUbElemSize = UblmD.aElemK.GetSize();
                    if (nUbElemSize > 1)
                    {
                        for (int i = 0; i < nUbElemSize; ++i)
                        {
                            if (UblmD.aElemK[i] == ElemK)
                            {
                                if (i == nUbElemSize - 1)
                                {
                                    if (UblmD.bJsupport) nNode[1] = 1;
                                }
                            }
                        }

                    }
                    else
                    {
                        if (UblmD.bJsupport) nNode[1] = 1;
                    }

                }
            }
        };

    T_SPAN_BASE SpanBase; SpanBase.Initialize();
	const auto& itElemSpan = m_mElemSpanD.find(ElemK);
    if ( itElemSpan != m_mElemSpanD.end() )
    {
		SpanBase = itElemSpan->second;
        if ( SpanBase.nSupport==0 )
        {
            if (bCheckJ)
            {
                _L_CheckJNode();
            }
            else
            {
                return FALSE;
            }                    
        }
        else if ( SpanBase.nSupport==1 )
        {
            if (bCheckJ)
            {
                _L_CheckJNode();
            }
            else
            {
                nNode[0] = 1;
            }            
        }
        else if ( SpanBase.nSupport==2 )  nNode[1]=1;
        else ASSERT(0);
    }
    else
    {
        if (ElemK.second == EN_EL_BEAM)
        {
            T_ELEM_D ElemD; ElemD.Initialize();
            if (m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD))
            {
                int nNodeT[2] = { 0, 0 };
                nNodeT[0] = ElemD.elnod[0];
                nNodeT[1] = ElemD.elnod[1];
                nNode[0] = CheckSupprotNode(nNodeT[0]) ? 1 : 0;
                nNode[1] = CheckSupprotNode(nNodeT[1]) ? 1 : 0;
            }
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            //ASSERT(0); // ???
        }
        else ASSERT(0);
    }

    return TRUE;
}

BOOL CDgnCsgDataCtrl::CheckInteriorSupportNodeElem(ElemPairK ElemK, bool* bIJ)
{
    bIJ[0] = false;
    bIJ[1] = false;

    CSG_INTERIOR_SUPPORT SupportD;
	const auto& iter = m_mInteriorSupport.find(ElemK);
    if ( iter != m_mInteriorSupport.end() )        
    {
        bIJ[0] = SupportD.bSupport[0];
        bIJ[1] = SupportD.bSupport[1];
    }
    return TRUE;
}



BOOL CDgnCsgDataCtrl::Check_Strength_ASD(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RES_KSCE_LSD12_D &MembRes, STL_KR_RES_D &rData)
{
    // MembRes 데이터는 ??

    if (!m_pCsgManager->Check_StrengthResKSCE_ASD(m_nDgnCode, InD, rData)) return FALSE;
    return TRUE;
}

BOOL CDgnCsgDataCtrl::IsCSACode(int nDgnCode)
{
    switch (nDgnCode)
    {
    case CSA_S6_19_CSG:
    case CSA_S6_14_CSG:
    case CSA_S6_10_CSG:
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

BOOL CDgnCsgDataCtrl::IsAASHTOCode( int nDgnCode )
{
    switch (nDgnCode)
    {
    case AASHTO_LRFD20_CSG:
    case AASHTO_LRFD17_CSG:
    case AASHTO_LRFD16_CSG:
    case AASHTO_LRFD12_CSG:
    case AASHTO_LRFD07_CSG:
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

BOOL CDgnCsgDataCtrl::IsLSDCode( int nDgnCode )
{
    switch (nDgnCode)
    {
    case KDS_24_14_31_2018_CSG:
    case KSCE_LSD15_CSG:
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

BOOL CDgnCsgDataCtrl::GetForceByERLoad( _DGN_FORC_CRC &Er_I, _DGN_FORC_CRC &Er_M, _DGN_FORC_CRC &Er_J, _DGN_FORC_CRC &Force_I, _DGN_FORC_CRC &Force_M, _DGN_FORC_CRC &Force_J, double dFacER )
{
    Er_I *= dFacER;
    Er_M *= dFacER;
    Er_J *= dFacER;
    Force_I += Er_I;
    Force_M += Er_M;
    Force_J += Er_J;
    return TRUE;
}

int CDgnCsgDataCtrl::GetLcomTypeIndex4MaxMin()
{
    return DGN_FORCE_CONCURRENT_7DOF+1;
}

BOOL CDgnCsgDataCtrl::IsChkFatigue2(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes)
{
    return FALSE;
}

BOOL CDgnCsgDataCtrl::IsChkFatigueCr(int nFatiLcomType, int nFatigueType2)
{
    return TRUE;
}

BOOL CDgnCsgDataCtrl::IsSkipChkScon(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes)
{
    return FALSE;
}

BOOL CDgnCsgDataCtrl::IsChkFatiLoadStud()
{
    return FALSE;
}

BOOL CDgnCsgDataCtrl::IsChkFatiTranFlex()
{
    return FALSE;
}

BOOL CDgnCsgDataCtrl::IsBeforeSgldDL()
{
    return TRUE;
}

BOOL CDgnCsgDataCtrl::IsAppErLoad()
{
    return TRUE;
}

BOOL CDgnCsgDataCtrl::IsSCMembChk()
{
    return FALSE;
}

// BOOL CDgnCsgDataCtrl::GetERMembForce( T_STCT_D StctD, T_STAG_K StageK, int nSerialStepK, CArray<T_ELEM_K,T_ELEM_K&> &aUblmElemK, T_STRB_D& MembForNC_ER1, T_STRB_D& MembForNC_ER2, T_STRB_D& MembForNC_ER3 )
// {
//     CString strSgldName = _T("");
//     T_SGLD_K SgldK = 0;
//     //Er1
//     if (StctD.nNumStctCase_NotUse>0)
//     {
//         strSgldName = StctD.strErection1_NotUse;	// EL.
//         SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
//         if(!m_pForcCtrl->GetCSGirderMembForce(SgldK, StageK, nSerialStepK, aUblmElemK, &MembForNC_ER1)) return FALSE;  //Erection Load중 Dead Before로 가는 부재력(Part2 이전 단계의 부재력)
//     }
//     //Er2
//     if (StctD.nNumStctCase2_NotUse>0)
//     {
//         strSgldName = StctD.strErection2_NotUse;
//         SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
//         if(m_pForcCtrl->GetCSGirderMembForce(SgldK, StageK, nSerialStepK, aUblmElemK, &MembForNC_ER2))  return FALSE;
//     }
//     //Er3
//     if (StctD.nNumStctCase3_NotUse>0)
//     {
//         strSgldName = StctD.strErection3_NotUse;
//         SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
//         if(m_pForcCtrl->GetCSGirderMembForce(SgldK, StageK, nSerialStepK, aUblmElemK, &MembForNC_ER3)) return FALSE;
//     }
// 
//     return TRUE;
// }

UINT CDgnCsgDataCtrl::GetDgnElemNo() const
{
	return GetCheckElem();
}

UINT CDgnCsgDataCtrl::GetDgnPosiNo() const
{
	return GetCheckPosition();
}

T_UNIT_INDEX CDgnCsgDataCtrl::GetCodeUnit() const
{
	T_UNIT_INDEX CodeUnit;
	switch ( m_nDgnCode )
	{
	case KDS_24_14_31_2018_CSG:
	case KSCE_LSD15_CSG:
	case KSSC_2014_CSG:
	case KSCE_ASD10_CSG:
	case KSCE_RAIL_ASD11_CSG:
	case CSA_S6_10_CSG:
	case CSA_S6_14_CSG:
    case CSA_S6_19_CSG:
	case CS457_R1_CSG:
	case AS5100_6_17_CSG:
    case NR_GN_CIV_025_06_CSG:
		{
			CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
			CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
		}
		break;
	case AASHTO_LRFD20_CSG:
	case AASHTO_LRFD17_CSG:
	case AASHTO_LRFD16_CSG:
	case AASHTO_LRFD12_CSG:
	case AASHTO_LRFD07_CSG:
		{
			CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
			CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_KIP;
		}
		break;
	default: ASSERT(0); break;
	}

	return CodeUnit;
}

int CDgnCsgDataCtrl::GetDgnLcomNo() const
{
	return m_nDgnLcomNo;
}

void CDgnCsgDataCtrl::SetDgnLcomNo(int nLcomNo)
{
	m_nDgnLcomNo = nLcomNo;
}

int CDgnCsgDataCtrl::GetDgnCurrentLcomIndex(int nLcom)
{
	ASSERT(nLcom != 0);
	return nLcom - 1;
}

void CDgnCsgDataCtrl::SetLcomInfoResultStruct(const CSG_MEMB_POSD_KSCE_LSD& InD, T_COMPOSITE_STEEL_GIRDER_LCOM_RES_D& LcomR)
{
	double dFactor = 0.0;
	int nPos = InD.Memb.ChIf.nChkPos;
	const CSG_FORCE_COMP& ForceL = InD.aChkFor[0];
	const CSG_FORCE_D& ForNC = ForceL.ForNC;
	const CSG_FORCE_D& ForLT = ForceL.ForLT;
	const CSG_FORCE_D& ForST = ForceL.ForST;
	const CSG_FORCE_D& ForLC = ForceL.ForLC;

	T_ASCD_CSG_BS_BASE ComD;
	ComD.Set_Param(TRUE, InD.Memb.ChIf.nElem, InD.Memb.ChIf.nBeamType, InD.Memb.ChIf.nChkPos, LcomR.nDgnLcomNo, ForceL.nMax, dFactor);
	ComD.ForD.ChkFor.ForNC.SetForce(ForNC.Fx(), ForNC.Fy(), ForNC.Fz(), ForNC.Mx(), ForNC.My(), ForNC.Mz());
	ComD.ForD.ChkFor.ForLT.SetForce(ForLT.Fx(), ForLT.Fy(), ForLT.Fz(), ForLT.Mx(), ForLT.My(), ForLT.Mz());
	ComD.ForD.ChkFor.ForST.SetForce(ForST.Fx(), ForST.Fy(), ForST.Fz(), ForST.Mx(), ForST.My(), ForST.Mz());
	ComD.ForD.ChkFor.ForLC.SetForce(ForLC.Fx(), ForLC.Fy(), ForLC.Fz(), ForLC.Mx(), ForLC.My(), ForLC.Mz());

	for ( int i = 0; i < 3; ++i )
	{
		const CSG_FORCE_D& MForLC = InD.aUlmf[0].ForD[i];
		const CSG_FORCE_D& MForNC = InD.aUlmf[1].ForD[i];
		const CSG_FORCE_D& MForLT = InD.aUlmf[2].ForD[i];
		const CSG_FORCE_D& MForST = InD.aUlmf[3].ForD[i];
		T_CSGD_COMF& CMFor = ComD.ForD.MembFor[i];
		CMFor.ForNC.SetForce(MForNC.Fx(), MForNC.Fy(), MForNC.Fz(), MForNC.Mx(), MForNC.My(), MForNC.Mz());
		CMFor.ForLT.SetForce(MForLT.Fx(), MForLT.Fy(), MForLT.Fz(), MForLT.Mx(), MForLT.My(), MForLT.Mz());
		CMFor.ForST.SetForce(MForST.Fx(), MForST.Fy(), MForST.Fz(), MForST.Mx(), MForST.My(), MForST.Mz());
		CMFor.ForLC.SetForce(MForLC.Fx(), MForLC.Fy(), MForLC.Fz(), MForLC.Mx(), MForLC.My(), MForLC.Mz());
	}
	for ( int i = 0; i < 2; ++i )
	{
		const CSG_FORCE_D& MForLC = InD.aUlmf[0].For13D[i];
		const CSG_FORCE_D& MForNC = InD.aUlmf[1].For13D[i];
		const CSG_FORCE_D& MForLT = InD.aUlmf[2].For13D[i];
		const CSG_FORCE_D& MForST = InD.aUlmf[3].For13D[i];
		T_CSGD_COMF& CMFor = ComD.ForD.MembFor13[i];
		CMFor.ForNC.SetForce(MForNC.Fx(), MForNC.Fy(), MForNC.Fz(), MForNC.Mx(), MForNC.My(), MForNC.Mz());
		CMFor.ForLT.SetForce(MForLT.Fx(), MForLT.Fy(), MForLT.Fz(), MForLT.Mx(), MForLT.My(), MForLT.Mz());
		CMFor.ForST.SetForce(MForST.Fx(), MForST.Fy(), MForST.Fz(), MForST.Mx(), MForST.My(), MForST.Mz());
		CMFor.ForLC.SetForce(MForLC.Fx(), MForLC.Fy(), MForLC.Fz(), MForLC.Mx(), MForLC.My(), MForLC.Mz());
	}

	if ( LcomR.bULSLcom )
	{
		_DGN_FORC_CRC ForceNC;
		_DGN_FORC_CRC ForceLT;
		_DGN_FORC_CRC ForceST;
		GetPanelInfoForce(ForceNC, ForceLT, ForceST);
		for ( int i = 0; i < 2; ++i )
		{
			LcomR.AswyD.AswyB[nPos].Panelij[i].ForNC.SetForce(ForceNC.dFxx[i], ForceNC.dFyy[i], ForceNC.dFzz[i], ForceNC.dMux[i], ForceNC.dMuy[i], ForceNC.dMuz[i]);
			LcomR.AswyD.AswyB[nPos].Panelij[i].ForLT.SetForce(ForceLT.dFxx[i], ForceLT.dFyy[i], ForceLT.dFzz[i], ForceLT.dMux[i], ForceLT.dMuy[i], ForceLT.dMuz[i]);
			LcomR.AswyD.AswyB[nPos].Panelij[i].ForST.SetForce(ForceST.dFxx[i], ForceST.dFyy[i], ForceST.dFzz[i], ForceST.dMux[i], ForceST.dMuy[i], ForceST.dMuz[i]);
		}
	}	

	if ( LcomR.bULSLcom )
	{
		LcomR.AsfcD.AsfcB[nPos].ComD = ComD;
		LcomR.AsscD.AsscB[nPos].ComD = ComD;
		LcomR.AscbD.AscbB[nPos].ComD = ComD;
		LcomR.AslsD.AslsB[nPos].ComD = ComD;
		LcomR.AsfgD.AsfgB[nPos*2+0].ComD = ComD;
		LcomR.AsfgD.AsfgB[nPos*2+1].ComD = ComD;
		LcomR.AsfsD.AsfsB[nPos*2+0].ComD = ComD;
		LcomR.AsfsD.AsfsB[nPos*2+1].ComD = ComD;
		LcomR.AswyD.AswyB[nPos].ComD = ComD;
		LcomR.AswbD.AswbB[nPos].ComD = ComD;
		LcomR.AswsD.AswsB[nPos].ComD = ComD;
		LcomR.AssbD.AssbB[nPos].RFData.ComD = ComD;
		LcomR.CSfcD.AsfcB[nPos*2+0].ComD = ComD;
		LcomR.CSfcD.AsfcB[nPos*2+1].ComD = ComD;
		LcomR.CSscD.AsscB[nPos].ComD = ComD;
		LcomR.CscbD.AscbB[nPos].ComD = ComD;
	}

	if ( LcomR.bSLSLcom )
	{
		LcomR.AsslD.AsslB[nPos].ComD = ComD;
		LcomR.AslsSLS.AslsB[nPos].ComD = ComD;
		//LcomR.AssbD.AssbB[nPos].ComD = ComD;
	}
	//LcomR.CSfcD.AsfcB[nPos].ComD = ComD;
	//LcomR.CSscD.AsscB[nPos].ComD = ComD;
}

bool CDgnCsgDataCtrl::SetResult(const dgn::def::IResult* pResult)
{
	//BS TODO
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE )
	{
		return false; // 일단 double만
	}

	BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(GetDgnLcomNo());
	BOOL bSLSLcom = m_pForcCtrl->Is_LcomForStrs(GetDgnLcomNo());
	BOOL bFLSLcom = m_pForcCtrl->Is_LcomForFati(GetDgnLcomNo());

	UINT nElemK = GetDgnElemNo();
    ElemPairK EPairK = GetCheckElemPairK();
	int nDgnLcomNo = GetDgnLcomNo();
	int nDgnLcomIndex = GetDgnCurrentLcomIndex(nDgnLcomNo);

	T_COMPOSITE_STEEL_GIRDER_RES_D* pData = m_vRes[EPairK];
	if ( !pData ) return false;

	int iMaxMin = 0;

	CPlateGirderDesign* pPGD = m_pDoc->m_pPostCtrl->GetPlateGirder();
	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetCheckPosition();
	switch ( enResult )
	{
	case enDgnResultItem::SECTION_CLASS_SHEAR____P:
	case enDgnResultItem::SHEAR_ZF_P:
	case enDgnResultItem::SHEAR_VALUE_ZF_P:
	case enDgnResultItem::BEND__M_P:
		{
			if ( !bULSLcom ) return true;

			T_ASSC_CSG_BS_CASE& AsscCase = pData->vLcomRes[nDgnLcomIndex].AsscD;
			SetShearResistance(pResult, AsscCase);
		}
		break;
	case enDgnResultItem::SECTION_CLASS_BEND_P____P:
	case enDgnResultItem::SECTION_CLASS_BEND_N____P:
	case enDgnResultItem::CSG_BEND__MNP:
	case enDgnResultItem::CSG_BEND__MPP:
	case enDgnResultItem::CSG_BEND_YMNP:
	case enDgnResultItem::CSG_BEND_YMPP:
	case enDgnResultItem::CSG_BEND__SNP:
	case enDgnResultItem::CSG_BEND__SPP:
	case enDgnResultItem::BEND_PR__MNT:
	case enDgnResultItem::BEND_PR__MPT:
		{
			if ( !bULSLcom ) return true;						
			T_ASFC_CSG_BS_CASE& AsfcCase = pData->vLcomRes[nDgnLcomIndex].AsfcD;
			SetBendingResistance(pResult, AsfcCase);
		}
		break;
	case enDgnResultItem::EFFECTIVE_STIFF__A_P:
	case enDgnResultItem::EFFECTIVE_STIFF__I_P:
	case enDgnResultItem::EFFECTIVE_STIFF_BEND__I_P:
		{
			if ( !bULSLcom ) return true;

			//T_RHOC_LCOM* pRhoc = &m_svRes[nElemK]->svLcomRes[nDgnLcomIndex].RhoC;
			//SetRhocAndEffectiveStiffnessParam(pResult, *pRhoc);
		}
		break;
	case enDgnResultItem::SECTION_CLASS_LTB____P:
	case enDgnResultItem::LTB_AXIAL__F_P:
	case enDgnResultItem::LTB_BENDING__M_P:
	case enDgnResultItem::LTB_BENDING_MCR__M_P:
	case enDgnResultItem::LTB_INTERACTION____P:
		{
			if ( !bULSLcom ) return true;

			//T_PLTB_D* pPltbD = &m_svRes[nElemK]->svLcomRes[nDgnLcomIndex].PltbD;
			//SetLateralTorsionalBucklingLcomResult(pResult, *pPltbD);
		}
		break;
	case enDgnResultItem::COMB_BS_BEND__FLANGE__MNP:
	case enDgnResultItem::COMB_BS_SHEAR_FLANGE__FNP:
		{
			if ( !bULSLcom ) return true;

			T_ASCB_CSG_BS_CASE& AscbCase = pData->vLcomRes[nDgnLcomIndex].AscbD;
			SetCombinedMVResistance(pResult, AscbCase);
		}
		break;
	case enDgnResultItem::FLANGE_TOP_COMP_CHK_W_LS_S_P:
	case enDgnResultItem::FLANGE_TOP_COMP_CHK_WO_LS_S_P:
	case enDgnResultItem::FLANGE_BOT_COMP_CHK_W_LS_S_P:
	case enDgnResultItem::FLANGE_BOT_COMP_CHK_WO_LS_S_P:
	case enDgnResultItem::FLANGE_TOP_TENS_CHK_S_P:
	case enDgnResultItem::FLANGE_TOP_TENS_CHK_WO_LS_S_P:
	case enDgnResultItem::FLANGE_BOT_TENS_CHK_S_P:
		{
			if ( !bULSLcom ) return true;

			T_ASFG_CSG_BS_D& AsfgD = pData->vLcomRes[nDgnLcomIndex].AsfgD;
			SetFlangeWithStiffener(pResult, AsfgD);
		}
		break;
	case enDgnResultItem::FLANGE_FLG_XSNP:
	case enDgnResultItem::FLANGE_STIFFENER_XSNP:
	case enDgnResultItem::FLANGE_FLG_XSPP:
	case enDgnResultItem::FLANGE_STIFFENER_XSPP:
		{
			if ( !bULSLcom ) return true;

			T_ASFS_CSG_BS_D& AsfsD = pData->vLcomRes[nDgnLcomIndex].AsfsD;
			SetFlangeStiffenerWithStiffener(pResult, AsfsD);
		}
		break;
	case enDgnResultItem::WEB_YIELD_S_P:
		{
			if ( !bULSLcom ) return true;

			T_ASWY_CSG_BS_D& AswyCase = pData->vLcomRes[nDgnLcomIndex].AswyD;
			SetWebYieldingWithStiffener(pResult, AswyCase);
		}
		break;
	case enDgnResultItem::WEB_BUCK_COEFF___P:
	case enDgnResultItem::WEB_BUCK_CHK__S_P:
		{
			if ( !bULSLcom ) return true;

			T_ASWB_CSG_BS_D& AswbCase = pData->vLcomRes[nDgnLcomIndex].AswbD;
			SetWebBucklingWithStiffener(pResult, AswbCase);
		}
		break;
	case enDgnResultItem::WEB_STIFFENER__S_P:
	case enDgnResultItem::WEB_STIFFENER_MIN_STIFF__S_P:
		{
			if ( !bULSLcom ) return true;

			T_ASWS_CSG_BS_D& AswsCase = pData->vLcomRes[nDgnLcomIndex].AswsD;
			SetWebStiffenerWithStiffener(pResult, AswsCase);
		}
		break;
	case enDgnResultItem::FATIGUE_SC_STR__S_P:
	case enDgnResultItem::FATIGUE_SC_VR___P:
		{
			if ( !bFLSLcom ) return true;

			//T_PGRF_D* pPgrfD = &m_svRes[nElemK]->svLcomRes[nDgnLcomIndex].PgrfD;
			//SetFatigueResistanceLcomResult(pResult, *pPgrfD);
		}
		break;
	case enDgnResultItem::LONG_SHEAR_FOR__F_P:
	case enDgnResultItem::LONG_SHEAR_DC__F_P:
		{
			if ( !bULSLcom ) return true;

			T_ASLS_CSG_BS_CASE& AslsCase = pData->vLcomRes[nDgnLcomIndex].AslsD;
			SetLongitudinalShearResistance(pResult, AslsCase);
		}
		break;
	case enDgnResultItem::TRANSVERSE_FORCE_FR__F_P:
	case enDgnResultItem::TRANSVERSE_FORCE_INTER___P:
		{
			if ( !bULSLcom ) return true;

			//T_PRTF_D* pPrtfD = &m_svRes[nElemK]->svLcomRes[nDgnLcomIndex].PrtfD;
			//SetTransverseForceLcomResult(pResult, *pPrtfD);
		}
		break;
	case enDgnResultItem::STRS_CC_Q__SPP:
	case enDgnResultItem::STRS_CC_Q__SNP:
	case enDgnResultItem::STRS_ST__SNP:
	case enDgnResultItem::STRS_ST__SPP:
	case enDgnResultItem::STRS_STN_S_P:
	case enDgnResultItem::STRS_STS_S_P:
	case enDgnResultItem::STRS_STC_S_P:
		//case enDgnResultItem::STRS_CC_C__SNP:
		//case enDgnResultItem::STRS_CC_C__SPP:
		{
			if ( !bSLSLcom ) return true;

			T_ASSL_CSG_BS_CASE& AsslCase = pData->vLcomRes[nDgnLcomIndex].AsslD;
			SetSLSStress(pResult, AsslCase);
		}
		break;
	case enDgnResultItem::SLS_LONG_SHEAR_FOR__F_P:
	case enDgnResultItem::SLS_LONG_SHEAR_DC__F_P:
		{
			if ( !bSLSLcom ) return true;

			T_ASLS_CSG_BS_CASE& AslsCase = pData->vLcomRes[nDgnLcomIndex].AslsSLS;
			SetLongitudinalShearResistance(pResult, AslsCase);
		}
		break;
	case enDgnResultItem::FATIGUE_LONG_SHEAR_DC__F_P:
	case enDgnResultItem::FATIGUE_LONG_SHEAR_FOR__F_P:
		break; // 이게 들어오면 안되는거 아닌가? 확인 하자.
	case enDgnResultItem::BEND_YM_P:
	case enDgnResultItem::BEND__MPP:
	case enDgnResultItem::BEND_YMPP:
	case enDgnResultItem::BEND__SPP:
	case enDgnResultItem::BEND__MNP:
	case enDgnResultItem::BEND_YMNP:
	case enDgnResultItem::BEND__SNP:
		{
			if ( !bULSLcom ) return true;
			T_ASFC_CSG_BS_D& AsfcD = pData->vLcomRes[nDgnLcomIndex].CSfcD;
			SetCSBendingResistance(pResult, AsfcD);
		}
		break;
	case enDgnResultItem::SHEAR__F_P:
		{
			if ( !bULSLcom ) return true;

			T_ASSC_CSG_BS_CASE& AsscCase = pData->vLcomRes[nDgnLcomIndex].CSscD;
			SetCSShearResistance(pResult, AsscCase);
		}
		break;
	case enDgnResultItem::COMB_BS_BEND__FLANGE__MPP:
	case enDgnResultItem::COMB_BS_SHEAR_FLANGE__FPP:
		{
			if ( !bULSLcom ) return true;

			T_ASCB_CSG_BS_CASE& AscbCase = pData->vLcomRes[nDgnLcomIndex].CscbD;
			SetCSCombinedMVResistance(pResult, AscbCase);
		}
		break;
	case enDgnResultItem::KFAC____M:
	case enDgnResultItem::BGGL__L_P:
	case enDgnResultItem::AXIAL__F_P:
	case enDgnResultItem::TORSION__M_P:
	case enDgnResultItem::SHEAR_YF_P:
	case enDgnResultItem::BEND_BUCK_YM_P:
	case enDgnResultItem::BEND_ZM_P:
	case enDgnResultItem::SECTION_CLASS_BEND_Y__P:
	case enDgnResultItem::SECTION_CLASS_BEND_Z__P:
	case enDgnResultItem::COMBINE____P:
	case enDgnResultItem::COMB_AXIAL__F_P:
	case enDgnResultItem::COMB_BEND_YM_P:
	case enDgnResultItem::COMB_BEND_BUCK_YM_P:
	case enDgnResultItem::COMB_BEND_ZM_P:
	case enDgnResultItem::COMB_SHEAR_ZF_P:
	case enDgnResultItem::COMB_SHEAR_YF_P:
	case enDgnResultItem::COMB_TORSION__M_P:
	case enDgnResultItem::SECTION_CLASS_INTER_P:
	case enDgnResultItem::STFN__I_P:
	case enDgnResultItem::STFN__A_P:
	case enDgnResultItem::STFN_END__F_P:
	case enDgnResultItem::CMFT____P:
	case enDgnResultItem::LTB_CBFT____P:
	case enDgnResultItem::SLENDER____P:
	case enDgnResultItem::DEFLECTION__L_P:
	case enDgnResultItem::FATIGUE__S_P:
		break; // 이게 들어오면 안되는거 아닌가? 확인 하자.
	default:
		ASSERT(0);
		break;
	}

	return true;
}

bool CDgnCsgDataCtrl::GetReportInput(dgn::link::MItemInputList& vMembItemInput, dgn::link::MembInfoList& vMembInfo)
{
	//초기에 불러줘야 하는 함수들.
	//m_pDataCtrl->MakePlateGirderDesign4Report();

	SetReportMode(true);

	auto _L_MomentIndex = [&] (bool bPosiM, int nPos) -> int
	{
		if ( nPos == 0 )
			return bPosiM ? 0 : 1;
		else
			return bPosiM ? 2 : 3;
	};

	CPlateGirderDesign *pCheckGirder = m_pDoc->m_pPostCtrl->GetPlateGirder();

	m_pDataCtrl->Get_UnbracedLength();

	if ( m_mCsgSect.size() == 0 )
	{
		//if ( !m_pDataCtrl->Cal_LcomDataForDesignAll() )	return false;
		m_pForcCtrl->Set_LcomDataForDesign();
		Make_CSGDgnData();
	}		

	T_CPGD_D CpgdD;
	if ( !m_pDoc->m_pAttrCtrl->GetCpgd(CpgdD) )
	{
		CpgdD.Initialize();
	}

	CArray<T_CGRE_K, T_CGRE_K> aKeyElem;
	m_pDoc->m_pAttrCtrl2->GetCgreKeyList(aKeyElem);
	int nRptElemSize = aKeyElem.GetSize();

    CArray<T_CGRV_K, T_CGRV_K> aKeyVB;
    m_pDoc->m_pAttrCtrl2->GetCgrvKeyList(aKeyVB);
    int nRptVBeamSize = aKeyVB.GetSize();

    if ( nRptElemSize == 0 && nRptVBeamSize == 0)
		return true;

	std::vector<std::pair<ElemPairK, int>> vRptElem;
	T_CGRE_D CgreD;
	for ( int i = 0; i < nRptElemSize; ++i )
	{
		if ( m_pDoc->m_pAttrCtrl2->GetCgre(aKeyElem[i], CgreD) )
		{
			T_CGDP_D CgdpD; CgdpD.Initialize();
			if ( m_pDoc->m_pAttrCtrl->GetCgdp(aKeyElem[i], CgdpD) )
			{
				if ( CgreD.iDgnPosi == 0 && CgdpD.iDgnPosi == 1 ) continue;
				if ( CgreD.iDgnPosi == 1 && CgdpD.iDgnPosi == 0 ) continue;

                ElemPairK EPairK(aKeyElem[i], EN_EL_BEAM);
				switch ( CgreD.iDgnPosi )
				{
				case 0:
				case 1:
					vRptElem.push_back(std::make_pair(EPairK, CgreD.iDgnPosi));
					break;
				case 2:
					vRptElem.push_back(std::make_pair(EPairK, 0));
					vRptElem.push_back(std::make_pair(EPairK, 1));
					break;
				default:
					ASSERT(0);
					break;
				}
			}
		}
	}

    for (int i = 0; i < nRptElemSize; ++i)
    {
        T_CGRV_D CgrvD; CgrvD.Initialize();
        if (m_pDoc->m_pAttrCtrl2->GetCgrv(aKeyVB[i], CgrvD))
        {
            T_CGDV_D CgdvD; CgdvD.Initialize();
            if (m_pDoc->m_pAttrCtrl->GetCgdv(aKeyVB[i], CgdvD))
            {
                if (CgrvD.iDgnPosi == 0 && CgdvD.iDgnPosi == 1) continue;
                if (CgrvD.iDgnPosi == 1 && CgdvD.iDgnPosi == 0) continue;

                ElemPairK EPairK(aKeyVB[i], EN_EL_VBEAM);
                switch (CgrvD.iDgnPosi)
                {
                case 0:
                case 1:
                    vRptElem.push_back(std::make_pair(EPairK, CgrvD.iDgnPosi));
                    break;
                case 2:
                    vRptElem.push_back(std::make_pair(EPairK, 0));
                    vRptElem.push_back(std::make_pair(EPairK, 1));
                    break;
                default:
                    ASSERT(0);
                    break;
                }
            }
        }
    }

    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(m_pDoc);

	CCalcType CType(GetDgnCode(), GetDgnType(), GetDgnMembType());

	CItemList ItemList;
	if ( pDgnDoc->GetDgnCalc()->GetPerformSectionItem(CType, ItemList) == 0 )
	{
		ASSERT(0); return false;
	}
	std::vector<item::enDgnResultItem> ResultItem = ItemList.GetList();
	int nSize = ResultItem.size();

	for ( auto itr : vRptElem )
	{
		auto CurEPairK = itr.first;
		std::vector<std::pair<item::enDgnResultItem, std::pair<def::CDgnInput*, def::ReportCrit>>> ItemInputList;
		auto* pRptInfo = new dgn::rpt::CDgnReportInfo;
		pRptInfo->DgnType = enDgn::CSG;
		pRptInfo->MembType = enMemb::Beam;
		auto* pRptName = new dgn::rpt::CRptMemberNameInfo;
		pRptName->MembK = CurEPairK.first;
		CString strPos = itr.second == 0 ? _T("i") : _T("j");
		CString strElemPos; strElemPos.Format(_T("%d - %s"), itr.first, strPos);
		CString strMemberName;
		strMemberName.Format(_T("Steel Composite  :  %s"), strElemPos);
		pRptName->strMemberName = CT2W(strMemberName);
		pRptInfo->PushReportInfo(pRptName);
		auto* pRptDgnCode = new dgn::rpt::CRptDesignCodeInfo;
		CString strCodeName = CDBLib::GetCsgCodeName(CpgdD.iDgnCode);
		CString strNA = CNationalAnnexTool::ConvertNationalAnnex(CpgdD.nSubCode);
		CString strCsgCodeName;
		//strCsgCodeName.Format(_T("%s (NA : %s)"), strCodeName, strNA);
		strCsgCodeName.Format(_T("%s"), strCodeName);
		pRptDgnCode->strDesignCode = CT2W(strCsgCodeName);
		pRptInfo->PushReportInfo(pRptDgnCode);

		auto* pRptSection = new dgn::rpt::CRptSectionInfo;
		T_SECT_D SectD;   SectD.Initialize();
		m_pDoc->m_pPostCtrl->GetSectPostTsgrDgn(CurEPairK, SectD);

        int nMatlK = CDBLib::GetMaterialKey4ElemPairK(CurEPairK);
		pRptSection->strSectionName = CT2W(SectD.SName);
		T_SECT_SECTBASE_D& SectB = (itr.second == 1 && SectD.nStype == D_SECT_TYPE_TAPERED) ? SectD.CmpTapJ : SectD.SectBefore.SectI;
		for ( int j = 0; j < 14; ++j ) // 14 -> CRptSectionInfo의 dSize
		{
			pRptSection->dSize[j] = SectB.Size[j];
		}
		pRptInfo->PushReportInfo(pRptSection);

		auto* pRptMembLength = new dgn::rpt::CRptMemberLengthInfo;
		pRptMembLength->dL = CDBLib::GetLengthOfBeam(CurEPairK);
		pRptInfo->PushReportInfo(pRptMembLength);

		CSG_MATL_ELEM MatlElem; MatlElem.Initialize();
		m_mCsgMatl.Lookup(nMatlK, MatlElem);
		//const T_MATD_D& MatdD = MatlElem.MatdD;
		auto* pRptMaterialSTL = new dgn::rpt::CRptMaterialNameSTLInfo;
		pRptMaterialSTL->dFy = MatlElem.MatSw.Steel.S_Fy1;
		pRptMaterialSTL->dEs = MatlElem.MatSw.Steel.Elast;
		pRptInfo->PushReportInfo(pRptMaterialSTL);
		auto* pRptMaterialRC = new dgn::rpt::CRptMaterialNameRCInfo;
		pRptMaterialRC->dfck = MatlElem.MatSlab.RC.dFck;
		pRptMaterialRC->dEc  = MatlElem.MatSlab.RC.dEc;
		pRptMaterialRC->dfy  = MatlElem.MatSlab.RC.dFy;
		pRptMaterialRC->dEs  = MatlElem.MatSlab.RC.dEs;
		
		pRptInfo->PushReportInfo(pRptMaterialRC);

        auto* pRptMembLegacy = new dgn::def::CRptMembInfoLegacy;
        pRptMembLegacy->RptPref = CRptMembInfoLegacy::MemberReport;
        pRptMembLegacy->MembType = enMemb::Beam;
        // 데이터를 더 넣어야 하나?
        pRptInfo->PushReportInfo(pRptMembLegacy);

		//이걸 어쩔쓰까나
		SetCheckElem(CurEPairK.first);
		SetCheckPosition(itr.second);
        SetCheckElemPairK(CurEPairK);
		for ( int i = 0; i < nSize; ++i )
		{
			std::pair<item::enDgnResultItem, std::pair<def::CDgnInput*, def::ReportCrit>> pairCritic;
			auto Item = ResultItem[i];

			TTargetMembItem MembItem;
			MembItem.Sector = enSector::None;
			MembItem.Item = Item;

			TCriticalInfo Info;
			Info.iDcasK = INTEGRATIONDESIGNCASE;

			ReportCrit Critic;

			//불리한 하중조합과 그때 position으로 input data를 만들어줘야 한다.
			int iDgnLcomNo = 0, iMaxMin = 0;
			UINT OrgLcomK = 0;
			bool bCS = false;
			bool bCheck = true;
			CString strStageName = _T("");
			switch ( Item )
			{
			case enDgnResultItem::CSG_BEND_YMNP:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ASFC_CSG_BS_D AsfcD;
					if ( !pCheckGirder->ReadFlexuralStrengthAS5100_6(CurEPairK, AsfcD) )
					{
						ASSERT(0); continue;
					}

					int nMIdx = _L_MomentIndex(false, itr.second);
					const T_ASFC_CSG_BS_BASE& AsfcB = AsfcD.AsfcB[nMIdx];
					if ( !AsfcB.ComD.bChk ) continue;
// 					if ( Item == enDgnResultItem::BEND_YMNP )
// 					{
// 						if ( MT0(AsfcB.dPhify) ) 
// 							Item = enDgnResultItem::BEND__SNP;
// 					}

					OrgLcomK = AsfcB.ComD.RCaseK;
					iDgnLcomNo = AsfcB.ComD.nDgnLcomK;
					iMaxMin = AsfcB.ComD.unConcurrent;
					SetDgnForce(AsfcB.ComD.ForD);
				}
				break;
			case enDgnResultItem::CSG_BEND_YMPP:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ASFC_CSG_BS_D AsfcD;
					if ( !pCheckGirder->ReadFlexuralStrengthAS5100_6(CurEPairK, AsfcD) )
					{
						ASSERT(0); continue;
					}

					int nMIdx = _L_MomentIndex(true, itr.second);
					const T_ASFC_CSG_BS_BASE& AsfcB = AsfcD.AsfcB[nMIdx];
					if ( !AsfcB.ComD.bChk ) continue;

// 					if ( Item == enDgnResultItem::BEND_YMPP )
// 					{
// 						if ( MT0(AsfcB.dPhify) )
// 							Item = enDgnResultItem::BEND__SPP;
// 					}

					OrgLcomK = AsfcB.ComD.RCaseK;
					iDgnLcomNo = AsfcB.ComD.nDgnLcomK;
					iMaxMin = AsfcB.ComD.unConcurrent;
					SetDgnForce(AsfcB.ComD.ForD);
				}
				break;
			case enDgnResultItem::SHEAR_ZF_P:
			case enDgnResultItem::SHEAR_VALUE_ZF_P:
				{
					if ( !CpgdD.bStrength[1] )
						continue;

					T_ASSC_CSG_BS_CASE AsscD;
					if ( !pCheckGirder->ReadShearStrengthAS5100_6(CurEPairK, AsscD) )
					{
						ASSERT(0); continue;
					}

					const T_ASSC_CSG_BS_BASE& AsscB = AsscD.AsscB[itr.second];
					if ( !AsscB.ComD.bChk ) continue;

					OrgLcomK = AsscB.ComD.RCaseK;
					iDgnLcomNo = AsscB.ComD.nDgnLcomK;
					iMaxMin = AsscB.ComD.unConcurrent;
					SetDgnForce(AsscB.ComD.ForD);
				}
				break;
			case enDgnResultItem::COMB_BS_SHEAR_FLANGE__FNP:
			//case enDgnResultItem::COMB_BS_SHEAR_FLANGE__FPP:
				{
					if ( !CpgdD.bStrength[1] )
						continue;

					T_ASCB_CSG_BS_CASE AscbD;
					if ( !pCheckGirder->ReadCombinedMVStrengthAS5100_6(CurEPairK, AscbD) )
					{
						ASSERT(0); continue;
					}

					const T_ASCB_CSG_BS_BASE& AscbB = AscbD.AscbB[itr.second];
					if ( !AscbB.ComD.bChk ) continue;

					OrgLcomK = AscbB.ComD.RCaseK;
					iDgnLcomNo = AscbB.ComD.nDgnLcomK;
					iMaxMin = AscbB.ComD.unConcurrent;
					SetDgnForce(AscbB.ComD.ForD);
				}
				break;
			case enDgnResultItem::LONG_SHEAR_FOR__F_P:
			//case enDgnResultItem::LONG_SHEAR_DC__F_P:
				{
					if ( !CpgdD.bStrength[4] )
						continue;

					T_ASLS_CSG_BS_CASE AslsD;
					if ( !pCheckGirder->ReadLongShearAS5100_6(CurEPairK, AslsD) )
					{
						ASSERT(0); continue;
					}

					const T_ASLS_CSG_BS_BASE& AslsB = AslsD.AslsB[itr.second];
					if ( !AslsB.ComD.bChk ) continue;

					OrgLcomK = AslsB.ComD.RCaseK;
					iDgnLcomNo = AslsB.ComD.nDgnLcomK;
					iMaxMin = AslsB.ComD.unConcurrent;
					SetDgnForce(AslsB.ComD.ForD);
				}
				break;
			case enDgnResultItem::FLANGE_TOP_COMP_CHK_W_LS_S_P:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ASFG_CSG_BS_D AsfgD;
					if ( !pCheckGirder->ReadFlangeWithLStiffenerBoxShapeAS5100_6(CurEPairK, AsfgD) )
					{
						ASSERT(0); continue;
					}

					int nMIdx = _L_MomentIndex(true, itr.second);
					const T_ASFG_CSG_BS_BASE& AsfgB = AsfgD.AsfgB[nMIdx];
					if ( !AsfgB.ComD.bChk ) continue;

					OrgLcomK = AsfgB.ComD.RCaseK;
					iDgnLcomNo = AsfgB.ComD.nDgnLcomK;
					iMaxMin = AsfgB.ComD.unConcurrent;
					SetDgnForce(AsfgB.ComD.ForD);
				}
				break;
			case enDgnResultItem::FLANGE_TOP_TENS_CHK_S_P:
			case enDgnResultItem::FLANGE_BOT_TENS_CHK_S_P:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ASFG_CSG_BS_D AsfgD;
					if ( !pCheckGirder->ReadFlangeWithLStiffenerBoxShapeAS5100_6(CurEPairK, AsfgD) )
					{
						ASSERT(0); continue;
					}

					int nMIdx = _L_MomentIndex(false, itr.second);
					const T_ASFG_CSG_BS_BASE& AsfgB = AsfgD.AsfgB[nMIdx];
					if ( !AsfgB.ComD.bChk ) continue;

					OrgLcomK = AsfgB.ComD.RCaseK;
					iDgnLcomNo = AsfgB.ComD.nDgnLcomK;
					iMaxMin = AsfgB.ComD.unConcurrent;
					SetDgnForce(AsfgB.ComD.ForD);
				}
				break;
			case enDgnResultItem::FLANGE_STIFFENER_XSPP:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ASFS_CSG_BS_D AsfsD;
					if ( !pCheckGirder->ReadFlangeStiffenerWithLStiffenerBoxShapeAS5100_6(CurEPairK, AsfsD) )
					{
						ASSERT(0); continue;
					}

					int nMIdx = _L_MomentIndex(true, itr.second);
					const T_ASFS_CSG_BS_BASE& AsfsB = AsfsD.AsfsB[nMIdx];
					if ( !AsfsB.ComD.bChk ) continue;

					OrgLcomK = AsfsB.ComD.RCaseK;
					iDgnLcomNo = AsfsB.ComD.nDgnLcomK;
					iMaxMin = AsfsB.ComD.unConcurrent;
					SetDgnForce(AsfsB.ComD.ForD);
				}
				break;
			case enDgnResultItem::FLANGE_STIFFENER_XSNP:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ASFS_CSG_BS_D AsfsD;
					if ( !pCheckGirder->ReadFlangeStiffenerWithLStiffenerBoxShapeAS5100_6(CurEPairK, AsfsD) )
					{
						ASSERT(0); continue;
					}

					int nMIdx = _L_MomentIndex(false, itr.second);
					const T_ASFS_CSG_BS_BASE& AsfsB = AsfsD.AsfsB[nMIdx];
					if ( !AsfsB.ComD.bChk ) continue;

					OrgLcomK = AsfsB.ComD.RCaseK;
					iDgnLcomNo = AsfsB.ComD.nDgnLcomK;
					iMaxMin = AsfsB.ComD.unConcurrent;
					SetDgnForce(AsfsB.ComD.ForD);
				}
				break;
			case enDgnResultItem::WEB_YIELD_S_P:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ASWY_CSG_BS_D AswyD;
					if ( !pCheckGirder->ReadWebYieldingWithLStiffenerBoxShapeAS5100_6(CurEPairK, AswyD) )
					{
						ASSERT(0); continue;
					}

					const T_ASWY_CSG_BS_BASE& AswyB = AswyD.AswyB[itr.second];
					if ( !AswyB.ComD.bChk ) continue;

					OrgLcomK = AswyB.ComD.RCaseK;
					iDgnLcomNo = AswyB.ComD.nDgnLcomK;
					iMaxMin = AswyB.ComD.unConcurrent;
					SetDgnForce(AswyB.ComD.ForD);
					SetPanelForce(AswyB.Panelij);
				}
				break;
			case enDgnResultItem::WEB_BUCK_CHK__S_P:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ASWB_CSG_BS_D AswbD;
					if ( !pCheckGirder->ReadWebBucklingWithLStiffenerBoxShapeAS5100_6(CurEPairK, AswbD) )
					{
						ASSERT(0); continue;
					}

					const T_ASWB_CSG_BS_BASE& AswbB = AswbD.AswbB[itr.second];
					if ( !AswbB.ComD.bChk ) continue;

					OrgLcomK = AswbB.ComD.RCaseK;
					iDgnLcomNo = AswbB.ComD.nDgnLcomK;
					iMaxMin = AswbB.ComD.unConcurrent;
					SetDgnForce(AswbB.ComD.ForD);
				}
				break;
			case enDgnResultItem::WEB_STIFFENER__S_P:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ASWS_CSG_BS_D AswsD;
					if ( !pCheckGirder->ReadWebStiffenerWithLStiffenerBoxShapeAS5100_6(CurEPairK, AswsD) )
					{
						ASSERT(0); continue;
					}

					const T_ASWS_CSG_BS_BASE& AswsB = AswsD.AswsB[itr.second];
					if ( !AswsB.ComD.bChk ) continue;

					OrgLcomK = AswsB.ComD.RCaseK;
					iDgnLcomNo = AswsB.ComD.nDgnLcomK;
					iMaxMin = AswsB.ComD.unConcurrent;
					SetDgnForce(AswsB.ComD.ForD);
				}
				break;
			case enDgnResultItem::CS_0:
			case enDgnResultItem::CS_1:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_ACSF_CSG_BS_D AcsfD;
					if ( !pCheckGirder->ReadCSFlexuralStrengthAS5100_6(CurEPairK, AcsfD) )
					{
						ASSERT(0); continue;
					}

					int nMIdx = 0;
					if ( itr.second == 0 )
					{
						nMIdx = (Item == enDgnResultItem::CS_0) ? 0 : 1;
					}
					else
					{
						nMIdx = (Item == enDgnResultItem::CS_0) ? 2 : 3;
					}					

					const T_ASFC_CSG_BS_BASE& AsfcB = AcsfD.AcsfB[nMIdx];
					if ( !AsfcB.ComD.bChk ) continue;

					OrgLcomK = AsfcB.ComD.RCaseK;
					iDgnLcomNo = AsfcB.ComD.nDgnLcomK;
					iMaxMin = AsfcB.ComD.unConcurrent;
					SetDgnForce(AsfcB.ComD.ForD);
					bCS = true;
					T_STAG_D StageD;
					m_pDoc->m_pAttrCtrl->GetStag(AsfcB.ComD.StageK, StageD);
					strStageName = StageD.StageName;
				}
				break;
			//case enDgnResultItem::CS_1:
			//case enDgnResultItem::CS_2:
			case enDgnResultItem::CS_3:
				{
					if ( !CpgdD.bStrength[1] )
						continue;

					T_ACSV_CSG_BS_D AcsvD;
					if ( !pCheckGirder->ReadCSShearStrengthAS5100_6(CurEPairK, AcsvD) )
					{
						ASSERT(0); continue;
					}

					const T_ASSC_CSG_BS_BASE& AsscB = AcsvD.AcsvB[itr.second];
					if ( !AsscB.ComD.bChk ) continue;

					OrgLcomK = AsscB.ComD.RCaseK;
					iDgnLcomNo = AsscB.ComD.nDgnLcomK;
					iMaxMin = AsscB.ComD.unConcurrent;
					SetDgnForce(AsscB.ComD.ForD);
					bCS = true;
					T_STAG_D StageD;
					m_pDoc->m_pAttrCtrl->GetStag(AsscB.ComD.StageK, StageD);
					strStageName = StageD.StageName;
				}
				break;
			case enDgnResultItem::CS_4:
				{
					if ( !CpgdD.bStrength[1] )
						continue;

					T_ASCB_CSG_BS_CASE CscbD;
					if ( !pCheckGirder->ReadCSCombinedMVStrengthAS5100_6(CurEPairK, CscbD) )
					{
						ASSERT(0); continue;
					}

					const T_ASCB_CSG_BS_BASE& AscbB = CscbD.AscbB[itr.second];
					if ( !AscbB.ComD.bChk ) continue;

					OrgLcomK = AscbB.ComD.RCaseK;
					iDgnLcomNo = AscbB.ComD.nDgnLcomK;
					iMaxMin = AscbB.ComD.unConcurrent;
					SetDgnForce(AscbB.ComD.ForD);
					bCS = true;
					T_STAG_D StageD;
					m_pDoc->m_pAttrCtrl->GetStag(AscbB.ComD.StageK, StageD);
					strStageName = StageD.StageName;
				}
				break;
			case enDgnResultItem::LTB_INTERACTION____P:
				//case enDgnResultItem::LTB_BENDING__M_P:
				{
					if ( !CpgdD.bStrength[2] )
						continue;

					T_PLTB_D PltbD;
					//if ( !m_pAnalysisResult->GetPltbResultData(nElemK, PltbD) )
					{
						ASSERT(0); continue;
					}

					OrgLcomK = PltbD.PltbBase[itr.second].OrgLcomK;
					iDgnLcomNo = PltbD.PltbBase[itr.second].LcomK;
					iMaxMin = PltbD.PltbBase[itr.second].iMax;
				}
				break;
			case enDgnResultItem::TRANSVERSE_FORCE_INTER___P:
				{
					if ( !CpgdD.bStrength[3] )
						continue;

					T_PRTF_D PrtfD;
					//if ( !m_pAnalysisResult->GetPrtfResultData(nElemK, PrtfD) )
					{
						ASSERT(0); continue;
					}
					OrgLcomK = PrtfD.PrtfBase[itr.second].OrgLcomK;
					iDgnLcomNo = PrtfD.PrtfBase[itr.second].LcomK;
					iMaxMin = PrtfD.PrtfBase[itr.second].iMax;
				}
				break;
			case enDgnResultItem::FATIGUE_SC_STR__S_P:
			case enDgnResultItem::FATIGUE_SC_VR___P:
				{
					if ( !CpgdD.bStrength[5] )
						continue;

					T_PGRF_D PgrfD;
					//if ( !m_pAnalysisResult->GetPgrfResultData(nElemK, PgrfD) )
					{
						ASSERT(0); continue;
					}

					OrgLcomK = PgrfD.PgrfBase[itr.second].OrgLcomK;
					iDgnLcomNo = PgrfD.PgrfBase[itr.second].LcomK;
					iMaxMin = PgrfD.PgrfBase[itr.second].iMax;
				}
				break;
			//case enDgnResultItem::STRS_STN_S_P:
			case enDgnResultItem::STRS_ST__SNP:
			//case enDgnResultItem::STRS_ST__SPP:
				{
					if ( !CpgdD.bService[0] )
						continue;

					T_ASSL_CSG_BS_D AsslD;
					if ( !pCheckGirder->ReadSLSAS5100_6(CurEPairK, AsslD) )
					{
						ASSERT(0); continue;
					}

					int nMIdx = _L_MomentIndex(false, itr.second);  // 철근 검토만 하기 때문에 부모멘트만 유효
					const T_ASSL_CSG_BS_BASE& AsslB = AsslD.AsslB[nMIdx];
					if ( !AsslB.ComD.bChk ) continue;


					OrgLcomK = AsslB.ComD.RCaseK;
					iDgnLcomNo = AsslB.ComD.nDgnLcomK;
					iMaxMin = AsslB.ComD.unConcurrent;
					SetDgnForce(AsslB.ComD.ForD);
					_DGN_STRS_CRC GirderStr;
					GirderStr.dStl[itr.second] = AsslB.dsig_top[0];
					GirderStr.dStr[itr.second] = AsslB.dsig_top[1];
					GirderStr.dSbl[itr.second] = AsslB.dsig_bot[0];
					GirderStr.dSbr[itr.second] = AsslB.dsig_bot[1];

					_DGN_STRS_CRC SlabStr;
					SlabStr.dStl[itr.second] = AsslB.dft_s;
					SlabStr.dStr[itr.second] = AsslB.dft_s;
					SlabStr.dSbl[itr.second] = AsslB.dfb_s;
					SlabStr.dSbr[itr.second] = AsslB.dfb_s;

					SetDgnGirderStress(GirderStr);
					SetDgnSlabStress(SlabStr);

// 					if ( Item == enDgnResultItem::STRS_STC_S_P )
// 					{
// 						OrgLcomK = PgbsD.PgbsBase[itr.second].ChkGirder.OrgLcomK;
// 						iDgnLcomNo = PgbsD.PgbsBase[itr.second].ChkGirder.LcomK;
// 						iMaxMin = PgbsD.PgbsBase[itr.second].ChkGirder.iMax;
// 					}
// 					else if ( Item == enDgnResultItem::STRS_ST__SNP )
// 					{
// 						OrgLcomK = PgbsD.PgbsBase[itr.second].ChkBar.OrgLcomK;
// 						iDgnLcomNo = PgbsD.PgbsBase[itr.second].ChkBar.LcomK;
// 						iMaxMin = PgbsD.PgbsBase[itr.second].ChkBar.iMax;
// 					}
// 					else if ( Item == enDgnResultItem::STRS_ST__SPP )
// 					{
// 						OrgLcomK = PgbsD.PgbsBase[itr.second].ChkSlab.OrgLcomK;
// 						iDgnLcomNo = PgbsD.PgbsBase[itr.second].ChkSlab.LcomK;
// 						iMaxMin = PgbsD.PgbsBase[itr.second].ChkSlab.iMax;
// 					}
// 					else ASSERT(0);

				}
				break;
			case enDgnResultItem::SLS_LONG_SHEAR_FOR__F_P:
				{
					if ( !CpgdD.bService[1] )
						continue;

					T_ASLS_CSG_BS_CASE AslsD;
					if ( !pCheckGirder->ReadLongShearSLSAS5100_6(CurEPairK, AslsD) )
					{
						ASSERT(0); continue;
					}

					const T_ASLS_CSG_BS_BASE& AslsB = AslsD.AslsB[itr.second];
					if ( !AslsB.ComD.bChk ) continue;

					OrgLcomK = AslsB.ComD.RCaseK;
					iDgnLcomNo = AslsB.ComD.nDgnLcomK;
					iMaxMin = AslsB.ComD.unConcurrent;
					SetDgnForce(AslsB.ComD.ForD);
				}
				break;
			default:
				//ASSERT(0);//추가해줘야지.
				break;
			}

			//if ( iDgnLcomNo == 0 )
			//	continue;

			T_LCOM_D LcomD; LcomD.Initialize();
			CString strLcomName = _T("");
			if ( bCS )
			{
				Critic.aInfo[ReportCrit::LcomName] = CT2W(strStageName);
			}
			else if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, OrgLcomK, LcomD) )
			{
				Critic.aInfo[ReportCrit::LcomName] = CT2W(LcomD.LoadCombName);
			}
			else
			{
				Critic.aInfo[ReportCrit::LcomName] = CT2W(_T("-"));
			}

			SetDgnLcomNo(iDgnLcomNo);
			auto* pDgnInput = new CDgnInput;
			IDgnCodeCheck::GetDgnInput(*pDgnInput, enDgnMode::Checking);
			pairCritic.first = Item;
			pairCritic.second = std::make_pair(pDgnInput, Critic);

			ItemInputList.push_back(pairCritic);
		}

		if ( ItemInputList.size() == 0 )
		{
			//_SAFE_DELETE(pRptName);
			_SAFE_DELETE(pRptInfo);
			continue;
		}

		vMembInfo.push_back(pRptInfo);
		vMembItemInput.push_back(ItemInputList);
	}

	SetReportMode(false);

	return true;
}

bool CDgnCsgDataCtrl::GetUnbracedLength(dgn::def::CUnbracedLength* pData) const
{
	T_ELEM_K ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();
	
    ElemPairK EPairK(ElemK, EN_EL_BEAM);

	double dLy = 0.0;
	double dLz = 0.0;
	double dLb = 0.0;

	T_LENG_D LengD; LengD.Initialize();
	BOOL bAutoCalc = FALSE;
	if ( m_pDoc->m_pAttrCtrl->GetLeng(ElemK, LengD) )
	{
		dLy = LengD.BLy;
		dLz = LengD.BLz;
		dLb = LengD.BLu;
		bAutoCalc = LengD.bAutoCalcLe;
	}
	else
	{
        auto itUblmK = m_pDataCtrl->m_mElemUblmKey.find(EPairK);
		if (itUblmK != m_pDataCtrl->m_mElemUblmKey.end())
		{
            auto itUblMemb = m_pDataCtrl->m_mUblenMemb.find(itUblmK->second);
			if (itUblMemb != m_pDataCtrl->m_mUblenMemb.end())
			{
                const T_ULEN_MEMB_D& UblmD = itUblMemb->second;
				dLy = UblmD.dLength;
				dLz = UblmD.dLength;
				dLb = UblmD.dLength;
			}
		}
		else
		{
			double dElemLength = m_pDoc->calcLAVElem(ElemK);
			dLy = dElemLength;
			dLz = dElemLength;
			dLb = dElemLength;
		}
	}

	pData->dLy = dgn::lib::TFactor(false, dLy);
	pData->dLz = dgn::lib::TFactor(false, dLz);
	pData->dLb = dgn::lib::TFactor(false, dLb);

	return true;
}

void CDgnCsgDataCtrl::CalcCriticalLcomResult()
{
	UINT nElemK = GetDgnElemNo();
    ElemPairK EPairK = GetCheckElemPairK();
	T_COMPOSITE_STEEL_GIRDER_RES_D* pRes = m_vRes[EPairK];

	for ( int i = 0; i < 2; ++i )
	{
		double dPosMCr[2] = { -DBL_MAX, -DBL_MAX };
		double dNegMCr[2] = { -DBL_MAX, -DBL_MAX };
		double dShearCr[2] = { -DBL_MAX, -DBL_MAX };
		double dCombMVCr[2] = { -DBL_MAX, -DBL_MAX };
		double dSConCr[2] = { -DBL_MAX, -DBL_MAX };
		double dCompFlangeCr[2] = { -DBL_MAX, -DBL_MAX };
		double dTensFlangeCr[2] = { -DBL_MAX, -DBL_MAX };
		double dTopFlgStiffenerCr[2] = { -DBL_MAX, -DBL_MAX };
		double dBotFlgStiffenerCr[2] = { -DBL_MAX, -DBL_MAX };
		double dWebYieldCr[2] = { -DBL_MAX, -DBL_MAX };
		double dWebBucklingCr[2] = { -DBL_MAX, -DBL_MAX };
		double dWebStiffenerCr[2] = { -DBL_MAX, -DBL_MAX };
		double dPosMStressCr[2] = { -DBL_MAX, -DBL_MAX };
		double dNegMStressCr[2] = { -DBL_MAX, -DBL_MAX };
		double dSConSLSCr[2] = { -DBL_MAX, -DBL_MAX };
		for ( auto& rLcom : pRes->vLcomRes )
		{
			if ( rLcom.bULSLcom )
			{
				const T_ASFC_CSG_BS_BASE& AsfcB = rLcom.AsfcD.AsfcB[i];
				//const T_ASFC_CSG_BS_BASE& AcsfB = rLcom.CSfcD.AsfcB[i];

				double dMuy = AsfcB.ComD.ForD.ChkFor.ForLC.dForce[4];
				double dMratio = AsfcB.dA;

				if ( AsfcB.ComD.bChk )
				{
					if ( ME0(dMuy) )
					{
						if ( MT(dMratio, dPosMCr[i]) )
						{
							dPosMCr[i] = dMratio;
							int nPIndex = i == 0 ? 0 : 2;
							pRes->AsfcD.AsfcB[nPIndex] = AsfcB;
							pRes->CSfcD.AcsfB[nPIndex] = rLcom.CSfcD.AsfcB[nPIndex]; // 완성계 불리한 것과 일치 시키기 위해.
						}
					}
					else
					{
						if ( MT(dMratio, dNegMCr[i]) )
						{
							dNegMCr[i] = dMratio;
							int nNIndex = i == 0 ? 1 : 3;
							pRes->AsfcD.AsfcB[nNIndex] = AsfcB;
							pRes->CSfcD.AcsfB[nNIndex] = rLcom.CSfcD.AsfcB[nNIndex];
						}
					}
				}

				const T_ASSC_CSG_BS_BASE& AsscB = rLcom.AsscD.AsscB[i];
				const T_ASSC_CSG_BS_BASE& CSscB = rLcom.CSscD.AsscB[i];
				if ( AsscB.ComD.bChk )
				{
					if ( MT(AsscB.dA, dShearCr[i]) )
					{
						dShearCr[i] = AsscB.dA;
						pRes->AsscD.AsscB[i] = AsscB;
						pRes->CSscD.AcsvB[i] = CSscB;

					}
				}

				const T_ASCB_CSG_BS_BASE& AscbB = rLcom.AscbD.AscbB[i];
				const T_ASCB_CSG_BS_BASE& CscbB = rLcom.CscbD.AscbB[i];
				if ( AscbB.ComD.bChk )
				{
					if ( MT(AscbB.dA, dCombMVCr[i]) )
					{
						dCombMVCr[i] = AscbB.dA;
						pRes->AscbD.AscbB[i] = AscbB;
						pRes->CscbD.AscbB[i] = CscbB;
					}
				}

				const T_ASLS_CSG_BS_BASE& AslsB = rLcom.AslsD.AslsB[i];
				if ( AslsB.ComD.bChk )
				{
					if ( MT(AslsB.dA, dSConCr[i]) )
					{
						dSConCr[i] = AslsB.dA;
						pRes->AslsD.AslsB[i] = AslsB;
					}
				}

				int nCompFlgIndex = i * 2 + 0;
				int nTensFlgIndex = i * 2 + 1;
				const T_ASFG_CSG_BS_BASE& AsfgCompB = rLcom.AsfgD.AsfgB[nCompFlgIndex];
				const T_ASFG_CSG_BS_BASE& AsfgTensB = rLcom.AsfgD.AsfgB[nTensFlgIndex];
				if ( AsfgCompB.ComD.bChk )
				{
					if ( MT(AsfgCompB.dA, dCompFlangeCr[i]) )
					{
						dCompFlangeCr[i] = AsfgCompB.dA;
						pRes->AsfgD.AsfgB[nCompFlgIndex] = AsfgCompB;
					}
				}
				if ( AsfgTensB.ComD.bChk )
				{
					if ( MT(AsfgTensB.dA, dTensFlangeCr[i]) )
					{
						dTensFlangeCr[i] = AsfgTensB.dA;
						pRes->AsfgD.AsfgB[nTensFlgIndex] = AsfgTensB;
					}
				}

				int nTopFlgIndex = i * 2 + 0;
				int nBotFlgIndex = i * 2 + 1;
				const T_ASFS_CSG_BS_BASE& AsfsTop = rLcom.AsfsD.AsfsB[nTopFlgIndex];
				const T_ASFS_CSG_BS_BASE& AsfsBot = rLcom.AsfsD.AsfsB[nBotFlgIndex];
				const T_ASFS_CSG_BS_BASE& AsfsFlgTop = rLcom.AsfsD.AsfsFlgB[nTopFlgIndex];
				const T_ASFS_CSG_BS_BASE& AsfsFlgBot = rLcom.AsfsD.AsfsFlgB[nBotFlgIndex];
				if ( AsfsTop.ComD.bChk )
				{
					if ( MT(AsfsTop.dA, dTopFlgStiffenerCr[i]) ) // Stiffener로 불리한 조건.., Flange도 판단해야 하면 분리..
					{
						dTopFlgStiffenerCr[i] = AsfsTop.dA;
						pRes->AsfsD.AsfsB[nTopFlgIndex] = AsfsTop;
						pRes->AsfsD.AsfsFlgB[nTopFlgIndex] = AsfsFlgTop;
					}
				}
				if ( AsfsBot.ComD.bChk )
				{
					if ( MT(AsfsBot.dA, dBotFlgStiffenerCr[i]) ) // Stiffener로 불리한 조건.., Flange도 판단해야 하면 분리..
					{
						dBotFlgStiffenerCr[i] = AsfsBot.dA;
						pRes->AsfsD.AsfsB[nBotFlgIndex] = AsfsBot;
						pRes->AsfsD.AsfsFlgB[nBotFlgIndex] = AsfsFlgBot;
					}
				}

				const T_ASWY_CSG_BS_BASE& AswyB = rLcom.AswyD.AswyB[i];
				if ( AswyB.ComD.bChk )
				{
					if ( MT(AswyB.dA, dWebYieldCr[i]) )
					{
						dWebYieldCr[i] = AswyB.dA;
						pRes->AswyD.AswyB[i] = AswyB;
					}
				}

				const T_ASWB_CSG_BS_BASE& AswbB = rLcom.AswbD.AswbB[i];
				if ( AswbB.ComD.bChk )
				{
					if ( MT(AswbB.dA, dWebBucklingCr[i]) )
					{
						dWebBucklingCr[i] = AswbB.dA;
						pRes->AswbD.AswbB[i] = AswbB;
					}
				}

				const T_ASWS_CSG_BS_BASE& AswsB = rLcom.AswsD.AswsB[i];
				if ( AswsB.ComD.bChk )
				{
					if ( MT(AswsB.dA, dWebStiffenerCr[i]) )
					{
						dWebStiffenerCr[i] = AswsB.dA;
						pRes->AswsD.AswsB[i] = AswsB;
					}
				}

			}

			if ( rLcom.bSLSLcom )
			{
				const T_ASSL_CSG_BS_BASE& AsslB = rLcom.AsslD.AsslB[i];
				double dMuy = AsslB.ComD.ForD.ChkFor.ForLT.dForce[4];

				if ( AsslB.ComD.bChk )
				{
					if ( ME0(dMuy) )
					{
						if ( MT(AsslB.dA, dPosMStressCr[i]) )
						{
							dPosMStressCr[i] = AsslB.dA;
							int nPIndex = i == 0 ? 0 : 2;
							pRes->AsslD.AsslB[nPIndex] = AsslB;
						}
					}
					else
					{
						if ( MT(AsslB.dA, dNegMStressCr[i]) )
						{
							dNegMStressCr[i] = AsslB.dA;
							int nNIndex = i == 0 ? 1 : 3;
							pRes->AsslD.AsslB[nNIndex] = AsslB;
						}
					}
				}

				
				const T_ASLS_CSG_BS_BASE& AslsSLSB = rLcom.AslsSLS.AslsB[i];
				if ( AslsSLSB.ComD.bChk )
				{
					if ( MT(AslsSLSB.dA, dSConSLSCr[i]) )
					{
						dSConSLSCr[i] = AslsSLSB.dA;
						pRes->AslsSLS.AslsB[i] = AslsSLSB;
					}
				}


			}
		}

	}
}

void CDgnCsgDataCtrl::SetBendingResistance(const dgn::def::IResult* pResult, T_ASFC_CSG_BS_CASE& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASFC_CSG_BS_BASE& BendD = rData.AsfcB[nCheckPosition];

	BendD.ComD.bChk   = !m_CpgdD.bDesignParameter[0] ? FALSE : TRUE; // 조건이 필요할까?
	BendD.ComD.ElemK  = GetDgnElemNo();
	BendD.ComD.unPosi = nCheckPosition;
	BendD.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	BendD.ComD.unConcurrent = nMaxMinType;
	BendD.ComD.nDgnLcomK = nDgnLcomNo;


	switch ( enResult )
	{
	case enDgnResultItem::SECTION_CLASS_BEND_P____P:
	case enDgnResultItem::SECTION_CLASS_BEND_N____P:
		{
// 			rData.PgbrBase[nCheckPosition].iSectClass = static_cast<int>(pCasted->GetResult(EN_DGN_COMPO_V4_0));
// 			rData.PgbrBase[nCheckPosition].iTopClass = static_cast<int>(pCasted->GetResult(EN_DGN_COMPO_V4_1));
// 			rData.PgbrBase[nCheckPosition].iWebClass = static_cast<int>(pCasted->GetResult(EN_DGN_COMPO_V4_2));
// 			rData.PgbrBase[nCheckPosition].iBotClass = static_cast<int>(pCasted->GetResult(EN_DGN_COMPO_V4_3));
// 			if ( rData.PgbrBase[nCheckPosition].iSectClass == 5 )
// 			{
// 				rData.PgbrBase[nCheckPosition].iSectClass = 2;
// 			}
		}
		break;
	case enDgnResultItem::BEND__MPP:
	case enDgnResultItem::BEND__MNP:
		{
			//rData.PgbrBase[nCheckPosition].dMpl_Rd = pCasted->GetResult(EN_DGN_COMPO_DC_OWN);
		}
		break;
	case enDgnResultItem::BEND_PR__MNT:
	case enDgnResultItem::BEND_PR__MPT:
		{
			//BendD.dR = pCasted->GetResult(EN_DGN_COMPO_VALUE);
		}
		break;
	case enDgnResultItem::CSG_BEND_YMNP:
	case enDgnResultItem::CSG_BEND_YMPP:
		{	
			if ( BendD.ComD.bChk )
			{
				bool bPositiveM = (enResult == enDgnResultItem::CSG_BEND_YMPP);
				BendD.ComD.bChk = IsBendingChecking(bPositiveM) ? TRUE : FALSE;
			}
			
			BendD.dSA = pCasted->GetResult(EN_DGN_COMPO_DC_REQ);
			BendD.dR = pCasted->GetResult(EN_DGN_COMPO_DC_OWN);
            // Box, Tub형 Not compact 경우 dR == 0 인데, 이 경우 최대 부재력을 찾기 위해 수정(값은 의미 없음)
            double dCapa = EQ0(BendD.dR) ? 1.0/(D_ZERO_LIMIT* D_ZERO_LIMIT) : BendD.dR;
			BendD.dA = SafeDivAbs(BendD.dSA, dCapa);
		}
		break;
	case enDgnResultItem::CSG_BEND__SNP:
	case enDgnResultItem::CSG_BEND__SPP:
		{	
			BendD.dfs      = pCasted->GetResult(EN_DGN_COMPO_V6_1); // fs
			BendD.dfc      = pCasted->GetResult(EN_DGN_COMPO_V6_2); // fc 
			BendD.dPhify   = pCasted->GetResult(EN_DGN_COMPO_V6_3); // Phify 
			BendD.dPhifb   = pCasted->GetResult(EN_DGN_COMPO_V6_4); // Phifb 
			BendD.dfcLimit = pCasted->GetResult(EN_DGN_COMPO_V6_5); // 0.62fc' 
			//BendD.dA = SafeDivAbs(BendD.dfs, BendD.dPhify);
			if ( MT0(BendD.dPhify) )
			{
				BendD.dA = pCasted->GetResult(EN_DGN_COMPO_V6_0); // fs
				// Box, Tub형 Not compact 경우 dA == 0 인데, 이 경우 최대 부재력을 찾기 위해 수정(값은 의미 없음)
                if (EQ0(BendD.dA))
                {
					double dCapa = 1.0 / (D_ZERO_LIMIT * D_ZERO_LIMIT);
					BendD.dA = SafeDivAbs(BendD.dSA, dCapa);
                }
			}
	}
		break;
	default:
		break;
	}
	BendD.nOK = LE(BendD.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetCSBendingResistance(const dgn::def::IResult* pResult, T_ASFC_CSG_BS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	int nCheckIndex = 0;
	switch ( enResult )
	{
	case enDgnResultItem::BEND_YMPP:
	case enDgnResultItem::BEND__SPP:
	case enDgnResultItem::BEND__MPP:
		{
			nCheckIndex = nCheckPosition*2 + 0;
		}
		break;
	case enDgnResultItem::BEND_YMNP:
	case enDgnResultItem::BEND__SNP:
	case enDgnResultItem::BEND__MNP:
		{
			nCheckIndex = nCheckPosition*2 + 1;
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	T_ASFC_CSG_BS_BASE& BendD = rData.AsfcB[nCheckIndex];

    const auto& EPairK = GetCheckElemPairK();
	BendD.ComD.ElemK  = GetDgnElemNo();
	BendD.ComD.unPosi = nCheckPosition;
	BendD.ComD.nBeamType = EPairK.second;

	CPG_CSIF_D NCStageD;
	GetGirderStageInfo(EPairK, NCStageD);

	BendD.ComD.bChk = !m_CpgdD.bDesignParameter[3] || !IsCSChecking() ? FALSE : TRUE;
	BendD.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	BendD.ComD.unConcurrent = 0.0;
	BendD.ComD.nDgnLcomK = nDgnLcomNo;
	BendD.ComD.StageK = NCStageD.StagK;
	BendD.ComD.nStepK = NCStageD.nStepK;


	switch ( enResult )
	{
	case enDgnResultItem::BEND_YM_P:
	case enDgnResultItem::BEND__MNP:
	case enDgnResultItem::BEND__MPP:
	//case enDgnResultItem::BEND_YM_P:
		{	
		}
		break;
	case enDgnResultItem::BEND_YMNP:
		{	
			if ( BendD.ComD.bChk )
			{
				BendD.ComD.bChk = IsCSChecking() && IsBendingChecking(false) ? TRUE : FALSE;
			}
			BendD.dSA = pCasted->GetResult(EN_DGN_COMPO_DC_REQ);
			BendD.dR  = pCasted->GetResult(EN_DGN_COMPO_DC_OWN);
			BendD.dA  = SafeDivAbs(BendD.dSA, BendD.dR);
			if ( BendD.ComD.bChk )
			{
				BendD.ComD.bChk = LT0(BendD.dSA) ? TRUE : FALSE;
			}			
		}
		break;
	case enDgnResultItem::BEND_YMPP:
		{	
			if ( BendD.ComD.bChk )
			{
				BendD.ComD.bChk = IsCSChecking() && IsBendingChecking(true) ? TRUE : FALSE;
			}
			BendD.dSA = pCasted->GetResult(EN_DGN_COMPO_DC_REQ);
			BendD.dR  = pCasted->GetResult(EN_DGN_COMPO_DC_OWN);
			BendD.dA  = SafeDivAbs(BendD.dSA, BendD.dR);
			if ( BendD.ComD.bChk )
			{
				BendD.ComD.bChk = ME0(BendD.dSA) ? TRUE : FALSE;
			}			
		}
		break;
	case enDgnResultItem::BEND__SNP:
	case enDgnResultItem::BEND__SPP:
		{	
			BendD.dfs      = pCasted->GetResult(EN_DGN_COMPO_V6_1); // fs
			BendD.dfc      = pCasted->GetResult(EN_DGN_COMPO_V6_2); // fc 
			BendD.dPhify   = pCasted->GetResult(EN_DGN_COMPO_V6_3); // Phify 
			BendD.dPhifb   = pCasted->GetResult(EN_DGN_COMPO_V6_4); // Phifb 
			BendD.dfcLimit = pCasted->GetResult(EN_DGN_COMPO_V6_5); // 0.62fc' 
			BendD.dA       = pCasted->GetResult(EN_DGN_COMPO_V6_0); // fs
		}
		break;
	default:
		break;
	}
	BendD.nOK = LE(BendD.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetShearResistance(const dgn::def::IResult* pResult, T_ASSC_CSG_BS_CASE& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASSC_CSG_BS_BASE& ShearB = rData.AsscB[nCheckPosition];

	// Web 보강재 3개 이상인 경우는 7.4절 검토를 따르기 때문에 5.8절은 검토 안하는 것으로	

	ShearB.ComD.bChk   = !m_CpgdD.bDesignParameter[1] ? FALSE : (AreThere3orMoreLStiffenerInWeb() ? FALSE : TRUE);
	ShearB.ComD.ElemK  = GetDgnElemNo();
	ShearB.ComD.unPosi = nCheckPosition;
	ShearB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	ShearB.ComD.unConcurrent = nMaxMinType;
	ShearB.ComD.nDgnLcomK = nDgnLcomNo;

	switch ( enResult )
	{
	case enDgnResultItem::SECTION_CLASS_BEND_P____P:
	case enDgnResultItem::SECTION_CLASS_BEND_N____P:
		{
		}
		break;
	case enDgnResultItem::SHEAR_ZF_P:
		{	
			ShearB.dSA = pCasted->GetResult(EN_DGN_COMPO_DC_REQ);
			ShearB.dR  = pCasted->GetResult(EN_DGN_COMPO_DC_OWN);
			ShearB.dA  = SafeDgnRatioC0(ShearB.dSA, ShearB.dR);
		}
		break;
	default:
		break;
	}
	ShearB.nOK = LE(ShearB.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetCSShearResistance(const dgn::def::IResult* pResult, T_ASSC_CSG_BS_CASE& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASSC_CSG_BS_BASE& ShearB = rData.AsscB[nCheckPosition];

    const auto& EPairK = GetCheckElemPairK();
	ShearB.ComD.bChk   = !m_CpgdD.bDesignParameter[3] ? FALSE : (!IsCSChecking() || AreThere3orMoreLStiffenerInWeb() ? FALSE : TRUE);
	ShearB.ComD.ElemK  = GetDgnElemNo();
	ShearB.ComD.unPosi = nCheckPosition;

	CPG_CSIF_D NCStageD;
	GetGirderStageInfo(EPairK, NCStageD);

	ShearB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	ShearB.ComD.unConcurrent = 0.0;
	ShearB.ComD.nDgnLcomK = nDgnLcomNo;
	ShearB.ComD.StageK = NCStageD.StagK;
	ShearB.ComD.nStepK = NCStageD.nStepK;

	switch ( enResult )
	{
	case enDgnResultItem::SHEAR__F_P:
		{	
			ShearB.dSA = pCasted->GetResult(EN_DGN_COMPO_DC_REQ);
			ShearB.dR  = pCasted->GetResult(EN_DGN_COMPO_DC_OWN);
			ShearB.dA  = SafeDivAbs(ShearB.dSA, ShearB.dR);
		}
		break;
	default:
		break;
	}
	ShearB.nOK = LE(ShearB.dA, 1.0) ? 1 : 0;
}

void CDgnCsgDataCtrl::SetCombinedMVResistance(const dgn::def::IResult* pResult, T_ASCB_CSG_BS_CASE& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASCB_CSG_BS_BASE& CombB = rData.AscbB[nCheckPosition];

	CombB.ComD.bChk   = !m_CpgdD.bDesignParameter[2] ? FALSE : IsCombinedMVChecking();
	CombB.ComD.ElemK  = GetDgnElemNo();
	CombB.ComD.unPosi = nCheckPosition;
	CombB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	CombB.ComD.unConcurrent = nMaxMinType;
	CombB.ComD.nDgnLcomK = nDgnLcomNo;

	switch ( enResult )
	{
	case enDgnResultItem::SECTION_CLASS_BEND_P____P:
	case enDgnResultItem::SECTION_CLASS_BEND_N____P:
		{
		}
		break;
	case enDgnResultItem::COMB_BS_BEND__FLANGE__MNP:
	//case enDgnResultItem::COMB_BS_BEND__FLANGE__MPP:
		{
			CombB.dM_D = pCasted->GetResult(EN_DGN_COMPO_VL_VAL);
			CombB.dMf  = pCasted->GetResult(EN_DGN_COMPO_VL_LIM);
			//CombB.dRF = SafeDivAbs(ShearB.dSA, ShearB.dR);
		}
		break;
	case enDgnResultItem::COMB_BS_SHEAR_FLANGE__FNP:
	//case enDgnResultItem::COMB_BS_SHEAR_FLANGE__FPP:
		{	
			CombB.dV_D = pCasted->GetResult(EN_DGN_COMPO_VL_VAL);
			CombB.dV_R = pCasted->GetResult(EN_DGN_COMPO_VL_LIM);
			double dMrat = SafeDivAbs(CombB.dM_D, CombB.dMf);
			double dVrat = SafeDivAbs(CombB.dV_D, CombB.dV_R);
			CombB.dA = max(dMrat, dVrat);
		}
		break;
	default:
		break;
	}
	CombB.nOK = LE(CombB.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetCSCombinedMVResistance(const dgn::def::IResult* pResult, T_ASCB_CSG_BS_CASE& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASCB_CSG_BS_BASE& CombB = rData.AscbB[nCheckPosition];

    const auto& EPairK = GetCheckElemPairK();
	CombB.ComD.bChk   = !m_CpgdD.bDesignParameter[3] ? FALSE : (IsCSChecking() && IsCombinedMVChecking() ? TRUE : FALSE);
	CombB.ComD.ElemK  = GetDgnElemNo();
	CombB.ComD.unPosi = nCheckPosition;

	CPG_CSIF_D NCStageD;
	GetGirderStageInfo(EPairK, NCStageD);

	CombB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	CombB.ComD.unConcurrent = 0.0;
	CombB.ComD.nDgnLcomK = nDgnLcomNo;
	CombB.ComD.StageK = NCStageD.StagK;
	CombB.ComD.nStepK = NCStageD.nStepK;	

	switch ( enResult )
	{
	case enDgnResultItem::SECTION_CLASS_BEND_P____P:
	case enDgnResultItem::SECTION_CLASS_BEND_N____P:
		{
		}
		break;
	case enDgnResultItem::COMB_BS_BEND__FLANGE__MPP:
		{
			CombB.dM_D = pCasted->GetResult(EN_DGN_COMPO_VL_VAL);
			CombB.dMf  = pCasted->GetResult(EN_DGN_COMPO_VL_LIM);
			//CombB.dRF = SafeDivAbs(ShearB.dSA, ShearB.dR);
		}
		break;
	case enDgnResultItem::COMB_BS_SHEAR_FLANGE__FPP:
		{	
			CombB.dV_D = pCasted->GetResult(EN_DGN_COMPO_VL_VAL);
			CombB.dV_R = pCasted->GetResult(EN_DGN_COMPO_VL_LIM);
			double dMrat = SafeDivAbs(CombB.dM_D, CombB.dMf);
			double dVrat = SafeDivAbs(CombB.dV_D, CombB.dV_R);
			CombB.dA = max(dMrat, dVrat);
		}
		break;
	default:
		break;
	}
	CombB.nOK = LE(CombB.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetLongitudinalShearResistance(const dgn::def::IResult* pResult, T_ASLS_CSG_BS_CASE& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASLS_CSG_BS_BASE& LShearB = rData.AslsB[nCheckPosition];

	LShearB.ComD.bChk   = m_CpgdD.bService[1];
	LShearB.ComD.ElemK  = GetDgnElemNo();
	LShearB.ComD.unPosi = nCheckPosition;
	LShearB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	LShearB.ComD.unConcurrent = nMaxMinType;
	LShearB.ComD.nDgnLcomK = nDgnLcomNo;

	switch ( enResult )
	{
	case enDgnResultItem::LONG_SHEAR_FOR__F_P:
	case enDgnResultItem::SLS_LONG_SHEAR_FOR__F_P:
		{	
			LShearB.dq   = pCasted->GetResult(EN_DGN_COMPO_V2_0);
			LShearB.dqr  = pCasted->GetResult(EN_DGN_COMPO_V2_1);
			if ( EQ0(LShearB.dqr) )
			{
				LShearB.dA = SafeDivAbs(LShearB.dq, D_ZERO_LIMIT);
			}
			else
			{
				LShearB.dA = SafeDivAbs(LShearB.dq, LShearB.dqr);
			}			
		}
		break;
	default:
		break;
	}
	LShearB.nOK = LE(LShearB.dA, 1.0) ? 1 : 0;
}

void CDgnCsgDataCtrl::SetFlangeWithStiffener(const dgn::def::IResult* pResult, T_ASFG_CSG_BS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	int nFlgIndex = 0;
	switch ( enResult )
	{
	case enDgnResultItem::FLANGE_TOP_COMP_CHK_W_LS_S_P:  // 압축
	case enDgnResultItem::FLANGE_BOT_COMP_CHK_W_LS_S_P:
		nFlgIndex = nCheckPosition * 2 + 0;
		break;
	case enDgnResultItem::FLANGE_BOT_TENS_CHK_S_P:
	case enDgnResultItem::FLANGE_TOP_TENS_CHK_S_P:
	case enDgnResultItem::FLANGE_TOP_TENS_CHK_WO_LS_S_P:
		nFlgIndex = nCheckPosition * 2 + 1;
		break;	
	}

	T_ASFG_CSG_BS_BASE& AsfgB = rData.AsfgB[nFlgIndex];

	AsfgB.ComD.bChk   = IsFlangeChecking();
	AsfgB.ComD.ElemK  = GetDgnElemNo();
	AsfgB.ComD.unPosi = nCheckPosition;
	AsfgB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	AsfgB.ComD.unConcurrent = nMaxMinType;
	AsfgB.ComD.nDgnLcomK = nDgnLcomNo;
	AsfgB.ComD.bBottomFlangeWithLS = IsFlangeWithLS();
	AsfgB.ComD.bWebWithLS = IsWebWithLS();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(AsfgB.ComD.ElemK, ElemD) ) return;

	SectPairK SPairK(ElemD.elpro, EN_EL_BEAM);

	const auto& itSect = m_mCsgSect.find(SPairK);
	if ( itSect == m_mCsgSect.end()) return;

	const CSG_SECT_POSD& SectPos = itSect->second.SectPos[nCheckPosition];

	double dMuy = GetForceMuy();
	BOOL   bPositiveM = MT0(dMuy);

	switch ( enResult )
	{
	case enDgnResultItem::FLANGE_TOP_COMP_CHK_W_LS_S_P:
	case enDgnResultItem::FLANGE_BOT_COMP_CHK_W_LS_S_P:
		{
			AsfgB.dSig_f_DL = pCasted->GetResult(EN_DGN_COMPO_V5_0);
			AsfgB.dTau_DL   = pCasted->GetResult(EN_DGN_COMPO_V5_1);
			AsfgB.dSig_f_ST = pCasted->GetResult(EN_DGN_COMPO_V5_2);
			AsfgB.dTau_ST   = pCasted->GetResult(EN_DGN_COMPO_V5_3);
			if ( IsLongitudinalStiffener() )
			{
				if ( !bPositiveM && SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_TUB )
				{
					AsfgB.dA = SafeDivAbs(AsfgB.dSig_f_ST, AsfgB.dTau_ST); // Internal만 유효
				}
				else if ( SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_B )
				{
					AsfgB.dA = SafeDivAbs(AsfgB.dSig_f_ST, AsfgB.dTau_ST);
				}
			}
		}
		break;
	case enDgnResultItem::FLANGE_TOP_TENS_CHK_S_P:
	case enDgnResultItem::FLANGE_BOT_TENS_CHK_S_P:
		{
			AsfgB.dSig_f_DL = pCasted->GetResult(EN_DGN_COMPO_V5_0);
			AsfgB.dTau_DL   = pCasted->GetResult(EN_DGN_COMPO_V5_1);
			AsfgB.dSig_f_ST = pCasted->GetResult(EN_DGN_COMPO_V5_2);
			AsfgB.dTau_ST   = pCasted->GetResult(EN_DGN_COMPO_V5_3);
			if ( IsLongitudinalStiffener() )
			{
				if ( bPositiveM && SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_TUB )
				{
					AsfgB.dA = SafeDivAbs(AsfgB.dSig_f_ST, AsfgB.dTau_ST); // Internal만 유효
				}
				else if ( SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_B )
				{
					AsfgB.dA = SafeDivAbs(AsfgB.dSig_f_ST, AsfgB.dTau_ST);
				}
			}
		}
		break;
	case enDgnResultItem::FLANGE_TOP_COMP_CHK_WO_LS_S_P:
	case enDgnResultItem::FLANGE_BOT_COMP_CHK_WO_LS_S_P:
		{
			AsfgB.dSig_yf   = pCasted->GetResult(EN_DGN_COMPO_V5_0);   // fs
			AsfgB.dSig_f_SV = pCasted->GetResult(EN_DGN_COMPO_V5_1); // Phify
			AsfgB.dTau_SV   = pCasted->GetResult(EN_DGN_COMPO_V5_2); // Phifb
			AsfgB.dPsi      = pCasted->GetResult(EN_DGN_COMPO_V5_3); // Class 
			//AsfgB.dRA       = pCasted->GetResult(EN_DGN_COMPO_V5_3);
			//AsfgB.dA        = SafeDivAbs(AsfgB.dSA, AsfgB.dRA);
			if ( IsLongitudinalStiffener() )
			{
				if ( bPositiveM && SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_TUB )
				{
					AsfgB.dA = SafeDivAbs(AsfgB.dSig_yf, AsfgB.dSig_f_SV); // Outstand만 유효
				}
				else if ( SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_B )
				{
					// Ratio 판단 Skip
				}
			}
			else
			{
				if ( EQ(AsfgB.dPsi, 4.0) ) // enSectClassUSSeries::NotCompact
				{
					AsfgB.dA = SafeDivAbs(AsfgB.dSig_yf, AsfgB.dSig_f_SV);
				}				
			}

		}
		break;
	case enDgnResultItem::FLANGE_TOP_TENS_CHK_WO_LS_S_P:
		{
			AsfgB.dSig_yf   = pCasted->GetResult(EN_DGN_COMPO_V5_0);   // fs
			AsfgB.dSig_f_SV = pCasted->GetResult(EN_DGN_COMPO_V5_1); // Phify
			AsfgB.dTau_SV   = pCasted->GetResult(EN_DGN_COMPO_V5_2); // Phifb
			AsfgB.dPsi      = pCasted->GetResult(EN_DGN_COMPO_V5_3); // Class
			//AsfgB.dRA       = pCasted->GetResult(EN_DGN_COMPO_V5_3);
			//AsfgB.dA        = SafeDivAbs(AsfgB.dSA, AsfgB.dRA);
			if ( IsLongitudinalStiffener() )
			{
				if ( !bPositiveM && SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_TUB )
				{
					AsfgB.dA = SafeDivAbs(AsfgB.dSig_yf, AsfgB.dSig_f_SV); // Outstand만 유효
				}
				else if ( SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_B )
				{
					// Ratio 판단 Skip
				}
			}
			else
			{
				if ( EQ(AsfgB.dPsi, 4.0) ) // enSectClassUSSeries::NotCompact
				{
					AsfgB.dA = SafeDivAbs(AsfgB.dSig_yf, AsfgB.dSig_f_SV);
				}
			}
		}
		break;
	default:
		break;
	}
	AsfgB.nOK = LE(AsfgB.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetFlangeStiffenerWithStiffener(const dgn::def::IResult* pResult, T_ASFS_CSG_BS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	int nFlgIndex = 0;
	switch ( enResult )
	{
	case enDgnResultItem::FLANGE_FLG_XSPP:  // 0=Top
	case enDgnResultItem::FLANGE_STIFFENER_XSPP:
		nFlgIndex = nCheckPosition * 2 + 0;
		break;
	case enDgnResultItem::FLANGE_FLG_XSNP:
	case enDgnResultItem::FLANGE_STIFFENER_XSNP:
		nFlgIndex = nCheckPosition * 2 + 1;
		break;
	}

	T_ASFS_CSG_BS_BASE& AsfsStiffB = rData.AsfsB[nFlgIndex];
	T_ASFS_CSG_BS_BASE& AsfsFlgB   = rData.AsfsFlgB[nFlgIndex];

	AsfsStiffB.ComD.bChk   = IsFlangeWithLS();
	AsfsStiffB.ComD.ElemK  = GetDgnElemNo();
	AsfsStiffB.ComD.unPosi = nCheckPosition;
	AsfsStiffB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	AsfsStiffB.ComD.unConcurrent = nMaxMinType;
	AsfsStiffB.ComD.nDgnLcomK = nDgnLcomNo;

	switch ( enResult )
	{
	case enDgnResultItem::FLANGE_FLG_XSNP:
	case enDgnResultItem::FLANGE_FLG_XSPP:
		{
			AsfsFlgB.dSig_a_SV = pCasted->GetResult(EN_DGN_COMPO_V2_0);
			AsfsFlgB.dTau_1_SV = pCasted->GetResult(EN_DGN_COMPO_V2_1);
			AsfsFlgB.dA = SafeDivAbs(AsfsFlgB.dSig_a_SV, AsfsFlgB.dTau_1_SV);
		}
		break;
	case enDgnResultItem::FLANGE_STIFFENER_XSNP:
	case enDgnResultItem::FLANGE_STIFFENER_XSPP:
		{
			AsfsStiffB.dSig_a_SV = pCasted->GetResult(EN_DGN_COMPO_V2_0);
			AsfsStiffB.dTau_1_SV = pCasted->GetResult(EN_DGN_COMPO_V2_1);
			AsfsStiffB.dA  = SafeDivAbs(AsfsStiffB.dSig_a_SV, AsfsStiffB.dTau_1_SV);
			AsfsStiffB.ComD.bChk = EQ0(AsfsStiffB.dTau_1_SV) ? FALSE : TRUE;
		}
		break;
	default:
		break;
	}
	AsfsStiffB.nOK = LE(AsfsStiffB.dA, 1.0) ? 1 : 0;
	AsfsFlgB.nOK   = LE(AsfsFlgB.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetWebYieldingWithStiffener(const dgn::def::IResult* pResult, T_ASWY_CSG_BS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASWY_CSG_BS_BASE& AswyB = rData.AswyB[nCheckPosition];

	// Web 보강재 3개 이상인 경우는 7.4절 검토를 따르기 때문에 5.8절은 검토 안하는 것으로	
	AswyB.ComD.bChk   = AreThere3orMoreLStiffenerInWeb() ? TRUE :FALSE; //IsWebWithLS();
	AswyB.ComD.ElemK  = GetDgnElemNo();
	AswyB.ComD.unPosi = nCheckPosition;
	AswyB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	AswyB.ComD.unConcurrent = nMaxMinType;
	AswyB.ComD.nDgnLcomK = nDgnLcomNo;

	switch ( enResult )
	{
	case enDgnResultItem::WEB_YIELD_S_P:
		{
			AswyB.dSig_1e_DL = pCasted->GetResult(EN_DGN_COMPO_V5_1);
			AswyB.dTau_DL    = pCasted->GetResult(EN_DGN_COMPO_V5_2);
			AswyB.dSig_1e_ST = pCasted->GetResult(EN_DGN_COMPO_V5_3);
			AswyB.dTau_ST    = pCasted->GetResult(EN_DGN_COMPO_V5_4);
			AswyB.dA         = pCasted->GetResult(EN_DGN_COMPO_V5_0); //SafeDivAbs(AswyB.dSA, AswyB.dRA);
		}
		break;
	default:
		break;
	}
	AswyB.nOK   = LE(AswyB.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetWebBucklingWithStiffener(const dgn::def::IResult* pResult, T_ASWB_CSG_BS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASWB_CSG_BS_BASE& AswbB = rData.AswbB[nCheckPosition];

	AswbB.ComD.bChk   = AreThere3orMoreLStiffenerInWeb() ? TRUE : FALSE; //IsWebWithLS();
	AswbB.ComD.ElemK  = GetDgnElemNo();
	AswbB.ComD.unPosi = nCheckPosition;
	AswbB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	AswbB.ComD.unConcurrent = nMaxMinType;
	AswbB.ComD.nDgnLcomK = nDgnLcomNo;

	switch ( enResult )
	{
	case enDgnResultItem::WEB_BUCK_COEFF___P:
		{
			AswbB.dK1 = pCasted->GetResult(EN_DGN_COMPO_V3_0);
			AswbB.dKq = pCasted->GetResult(EN_DGN_COMPO_V3_1);
			AswbB.dKb = pCasted->GetResult(EN_DGN_COMPO_V3_2);
		}
		break;
	case enDgnResultItem::WEB_BUCK_CHK__S_P:
		{
			AswbB.dSig_1_DL = pCasted->GetResult(EN_DGN_COMPO_V5_0);
			AswbB.dSig_b_DL = pCasted->GetResult(EN_DGN_COMPO_V5_1);
			AswbB.dTau_DL   = pCasted->GetResult(EN_DGN_COMPO_V5_2);
			AswbB.dSA       = pCasted->GetResult(EN_DGN_COMPO_V5_3);
			AswbB.dRA       = pCasted->GetResult(EN_DGN_COMPO_V5_4);
			AswbB.dA        = SafeDivAbs(AswbB.dSA, AswbB.dRA);
		}
		break;
	default:
		break;
	}
	AswbB.nOK   = LE(AswbB.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetWebStiffenerWithStiffener(const dgn::def::IResult* pResult, T_ASWS_CSG_BS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASWS_CSG_BS_BASE& AswsB = rData.AswsB[nCheckPosition];

	AswsB.ComD.bChk   = IsWebWithLS() ? TRUE : FALSE;
	AswsB.ComD.ElemK  = GetDgnElemNo();
	AswsB.ComD.unPosi = nCheckPosition;
	AswsB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	AswsB.ComD.unConcurrent = nMaxMinType;
	AswsB.ComD.nDgnLcomK = nDgnLcomNo;

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(AswsB.ComD.ElemK, ElemD) ) return;

    SectPairK SPairK(ElemD.elpro, EN_EL_BEAM);

    const auto& itSect = m_mCsgSect.find(SPairK);
    if (itSect == m_mCsgSect.end()) return;

    const CSG_SECT_POSD& SectPos = itSect->second.SectPos[nCheckPosition];

	switch ( enResult )
	{
	case enDgnResultItem::WEB_STIFFENER__S_P:
		{
			AswsB.dSig_se_SV = pCasted->GetResult(EN_DGN_COMPO_DC_REQ);
			AswsB.dSig_se_DL = pCasted->GetResult(EN_DGN_COMPO_DC_OWN);
			AswsB.dA  = SafeDivAbs(AswsB.dSig_se_SV, AswsB.dSig_se_DL);
		}
		break;
	case enDgnResultItem::WEB_STIFFENER_MIN_STIFF__S_P:
		{
			if (IsSecTypeI(SectPos.SectInfo.nStype) && !AreThere3orMoreLStiffenerInWeb() )
			{
				AswsB.dIs1     = pCasted->GetResult(EN_DGN_COMPO_V5_0);
				AswsB.dIs1_min = pCasted->GetResult(EN_DGN_COMPO_V5_1);
				AswsB.dIs2     = pCasted->GetResult(EN_DGN_COMPO_V5_2);
				AswsB.dIs2_min = pCasted->GetResult(EN_DGN_COMPO_V5_3);

				BOOL bIs1OK = TRUE;
				BOOL bIs2OK = TRUE;

				if ( MT0(AswsB.dIs1) )
				{
					bIs1OK = ME(AswsB.dIs1, AswsB.dIs1_min);
					AswsB.dA = SafeDivAbs(AswsB.dIs1_min, AswsB.dIs1);
				}
				if ( MT0(AswsB.dIs2) )
				{
					bIs2OK = ME(AswsB.dIs2, AswsB.dIs2_min);
					double dRat2 = SafeDivAbs(AswsB.dIs2_min, AswsB.dIs2);
					AswsB.dA = max(AswsB.dA, dRat2);
				}
			}
		}
		break;
	default:
		break;
	}
	AswsB.nOK   = LE(AswsB.dA, 1.0) ? 1 : 0;

}

void CDgnCsgDataCtrl::SetSLSStress(const dgn::def::IResult* pResult, T_ASSL_CSG_BS_CASE& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetDgnPosiNo();
	int nDgnLcomNo = GetDgnLcomNo();
	UINT nLcomIdx  = nDgnLcomNo - 1;
	UINT ElemK     = GetDgnElemNo();
	UINT unPos     = GetCheckPosition();

	CString strOrgLcomNa = _T("");
	int nMaxMinType = 0.0;
	m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nMaxMinType);

	T_ASSL_CSG_BS_BASE& AsslB = rData.AsslB[nCheckPosition];

	AsslB.ComD.bChk   = m_CpgdD.bService[0];
	AsslB.ComD.ElemK  = ElemK;
	AsslB.ComD.unPosi = nCheckPosition;
	AsslB.ComD.RCaseK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	AsslB.ComD.unConcurrent = nMaxMinType;
	AsslB.ComD.nDgnLcomK = nDgnLcomNo;

	auto itrP1 = m_mStress.find(ElemK);
	if ( itrP1 == m_mStress.end() )
	{
		ASSERT(0); return;
	}

	auto itrP2 = m_mStressP2.find(ElemK);
	if ( itrP2 == m_mStressP2.end() )
	{
		ASSERT(0); return;
	}

	switch ( enResult )
	{
	case enDgnResultItem::STRS_CC_Q__SPP:
		{
			AsslB.dsig_c_lim = pCasted->GetResult(EN_DGN_COMPO_DC_OWN); // Top
		}
		break;
	case enDgnResultItem::STRS_CC_Q__SNP:
		{
			AsslB.dsig_t_lim = pCasted->GetResult(EN_DGN_COMPO_DC_OWN); // Bottom
		}
		break;
	case enDgnResultItem::STRS_STN_S_P:
		{
			AsslB.dsig_top[0] = itrP1->second.aStressLcom[nLcomIdx].dStl[unPos];
			AsslB.dsig_top[1] = itrP1->second.aStressLcom[nLcomIdx].dStr[unPos];
			AsslB.dsig_bot[0] = itrP1->second.aStressLcom[nLcomIdx].dSbl[unPos];
			AsslB.dsig_bot[1] = itrP1->second.aStressLcom[nLcomIdx].dSbr[unPos];
			double dftmax = AbsME(AsslB.dsig_top[0], AsslB.dsig_top[1]) ? AsslB.dsig_top[0] : AsslB.dsig_top[1];
			double dfbmax = AbsME(AsslB.dsig_bot[0], AsslB.dsig_bot[1]) ? AsslB.dsig_bot[0] : AsslB.dsig_bot[1];
			double dDemand = pCasted->GetResult(EN_DGN_COMPO_DC_REQ); 
			double dCapa   = pCasted->GetResult(EN_DGN_COMPO_DC_OWN); 
			//AsslB.dA = SafeDivAbs(dDemand, dCapa);
		}
		break;
	case enDgnResultItem::STRS_ST__SPP:
		{
			double dfTL = itrP2->second.aStressLcom[nLcomIdx].dStl[unPos];
			double dfTR = itrP2->second.aStressLcom[nLcomIdx].dStr[unPos];
			double dfBR = itrP2->second.aStressLcom[nLcomIdx].dSbr[unPos];
			double dfBL = itrP2->second.aStressLcom[nLcomIdx].dSbl[unPos];
			AsslB.dft_s = LE(dfTL, dfTR) ? dfTL : dfTR;
			AsslB.dfb_s = LE(dfBL, dfBR) ? dfBL : dfBR;
			AsslB.dft_s_lim = pCasted->GetResult(EN_DGN_COMPO_DC_OWN);
			double dConcRatio = SafeDivAbs(AsslB.dft_s, AsslB.dft_s_lim);
			//AsslB.dA = max(AsslB.dA, dConcRatio);
		}
		break;
	case enDgnResultItem::STRS_ST__SNP:
		{
			AsslB.dfr     = pCasted->GetResult(EN_DGN_COMPO_DC_REQ);
			AsslB.dfr_lim = pCasted->GetResult(EN_DGN_COMPO_DC_OWN);
			double dRbarRatio = SafeDivAbs(AsslB.dfr, AsslB.dfr_lim);
			AsslB.dA = max(AsslB.dA, dRbarRatio);
		}
		break;
	default:
		break;
	}
	AsslB.nOK   = LE(AsslB.dA, 1.0) ? 1 : 0;

}

bool CDgnCsgDataCtrl::IsCSChecking()
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

    SectPairK SPairK(ElemD.elpro, EN_EL_BEAM);

    const auto& itSect = m_mCsgSect.find(SPairK);
    if (itSect == m_mCsgSect.end()) return false;

	const CSG_SECT_POSD& SectPos = itSect->second.SectPos[unPos];
	const ST_SECT_SECTBASE_D& SectB = SectPos.SectInfo.Before;

	switch ( SectPos.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		{
			return true;
		}
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			return false;
		}
	default: ASSERT(0); break;
	}

	return false;

}
