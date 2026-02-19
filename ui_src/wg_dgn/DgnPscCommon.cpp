// DgnPscCommon.cpp: implementation of the CDgnPscCommon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\DgnEngine\Src\DGNe_RUS\DgnCalc_Rus_PSC_Struct.h"
#include "DgnPscCommon.h"
#include "DgnSectUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectInfo.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\MatlDB.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_Product.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\MembCtrl.h"

#if _MSC_VER >= 1700  // vc++11.0 //sjhuh-2014-02-21
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include <fstream.h>	// for rptwofstream
#endif
#include "..\dgnengine\idesign\DGN_link\ITmpDgnCalcBaseLib.h"
#include "..\dgnengine\idesign\DGN_def\DgnCodeDef.h"
#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
#include "..\dgnengine\idesign\DGN_lib\Macro.h"
#include "..\dgnengine\idesign\DGN_lib\NationalAnnexInterface.h"
#include "..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnUnitedStatesDef.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace dgn::lib;
using namespace dgn::def;

CDgnPscCommon::CDgnPscCommon()
{
    m_pDoc = CDBDoc::GetDocPoint();
    ASSERT(m_pDoc);
    m_pMembCtrl = CMembCtrl::GetMembCtrl();

    m_dZero=1.0E-07;
    m_dRatMax=99.99;
    m_dPI = CMathFunc::m_pi;
    m_Iteration = 20;
    m_dzDimComp=0.0;

    m_bSameConsiderRbar=FALSE;
    m_bChangeLcAxOfTapSect = FALSE;
    m_amXibJTG[0].InitHashTable(HASHSIZEELEM); m_amXibJTG[1].InitHashTable(HASHSIZEELEM);
    m_amMatlElem.clear();
    m_amSectElem[0].clear();      m_amSectElem[1].clear();
    m_amSectElem_Compo[0].clear(); m_amSectElem_Compo[1].clear();
    m_amSecgElem.clear();
    m_amSecgElem_Compo.clear();

    m_amRbarElem[0].clear(); m_amRbarElem[1].clear();
    m_amRshrElem[0].clear(); m_amRshrElem[1].clear();
    m_amTndnElem[0].clear(); m_amTndnElem[1].clear();
    m_amTndnTdna.InitHashTable(HASHSIZETDNA);
    m_amExpcElem.clear();
    m_amPscnElem.clear();
    m_amPscsElem.clear();
    m_amFackElem.clear();
	m_amPscnElem_IRS.clear();
    m_amCinfElem.clear();
    m_amSpbfElem.clear();
    m_amElemLp.clear();
    m_amElemPosLp.clear();

    // Composite Plate Girder
    m_iCivilCode = m_pDoc->GetCivilCodeDgn();
	m_nNatinoAnnex = 0;
    m_nBridgeType = 0;
    m_mapCpgMatlElem.clear();
    m_mapCpgSectElem.clear();
    m_mapCpgLstiElem.clear();
    m_mapCpgRbarElem.clear();
    m_mapCpgSconElem.clear();
    m_mapCpgTrstElem.clear();
    m_mapCpgTsesElem.clear();
    m_mapCpgBcdtElem.clear();
    m_mapCpgLtbdElem.clear();
    m_mapCpgFrdtElem.clear();
    m_mapCpgMembElem.clear();
    m_TdnaInfo.Initialize();

    m_unChkElem = 0;
    m_unChkPos  = 0;
    m_EPairK.first = 0;
    m_EPairK.second = EN_EL_BEAM;

    // Coded by Seungjun for test. ('20060719)
    CString csDesignOutput = CProduct::GetTestEnvValue(_ULS(Design Output));
    m_bTestMode = csDesignOutput==_ULS(yes) ? TRUE : FALSE;
}

CDgnPscCommon::~CDgnPscCommon()
{
    m_pDoc->SetDesignStatus(0);	// Initialize.
}

void CDgnPscCommon::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
    m_pDataCtrl = pDataCtrl;
    if(m_pDataCtrl==NULL) {ASSERT(0); return;}
    // Set ForceCtrl Pointer.
    m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
    if(m_pForcCtrl==NULL) {ASSERT(0); return;}

    if(pDataCtrl==NULL)  {ASSERT(0); return;}
    m_pForcCtrl->Set_DataCtrl(pDataCtrl);

    SetDataForTest(2);
}

void CDgnPscCommon::SetDataForTest(int iTestKind)
{
    // iTestKind
    // 1 = Sect for Anl,Dgn (DatazzzDataForSect_HETBAN.txt)
    // 2 = JTG D62-04 Detail Result.
    //++++++++++++++++++++++++++
    // Text File for Testing.
    m_strTestSectFile = _T("");
    m_strTestDgnFile = _T("");
    if(m_bTestMode)
    {
        m_aSelElemK.clear();
        CArray<T_ELEM_K,T_ELEM_K> aSelElemK;
        m_pDoc->m_pViewCtrl->GetAllSelectedElem(aSelElemK);
        for(int i=0; i<aSelElemK.GetSize(); ++i)
        {
            ElemPairK ElemK(aSelElemK.GetAt(i), aSelElemK.GetAt(i));
            m_aSelElemK.insert({ ElemK,ElemK });
        }
        if(aSelElemK.GetSize() > 0)
        {
            if(iTestKind==1)	// For zzzDataForSect_HETBAN.txt
            {
                m_strTestSectFile = _T("zzzDataForSect_HETBAN.txt");
                rptwofstream fout1(m_strTestSectFile, ios::trunc);
            }
            else if(iTestKind==2)	// For zzzDataForDgn_JTG_D62_04.txt
            {
                m_strTestDgnFile = _T("zzzDataForDgn_JTG_D62_04.txt");
                rptwofstream fout2(m_strTestDgnFile, ios::trunc);
            }
        }
    }
    //++++++++++++++++++++++++++
}

void CDgnPscCommon::PrtTestForSect()
{
    SetDataForTest(1);
    //++++++++++++++++++++++++++
    // For zzzDataForSect_HETBAN.txt
    if(m_bTestMode && m_aSelElemK.size() > 0)
    {
        rptwofstream fout(m_strTestSectFile, ios::app);
        // Beam Section Change Option.
        T_STCT_D StctD;
        if(!m_pDoc->m_pAttrCtrl->GetStct(StctD))	StctD.Initialize();
        // Reflection Rebar Option.
        T_ACTL_D ActlD;
        if(!m_pDoc->m_pAttrCtrl->GetActl(ActlD))	ActlD.Initialize();
        // Option Calculating for Section.
        CString strSectAnl = (ActlD.bConsiderRbar ? _T("CONC + RBAR") : _T("CONC"));
        CString strSectDgn = (CDBLib::IsConsiderRbarForDgn() ? _T("CONC + RBAR") : _T("CONC"));
        if(StctD.nBeamSectionChange==1)
        {
            strSectAnl = strSectAnl + _T(" + TNDN");
            strSectDgn = strSectDgn + _T(" + TNDN");
        }
        T_STRSPOST_D StrsPost;

        for (const auto& itElem : m_aSelElemK)
        {
            const auto& EPairK = itElem.first;
            T_ELEM_K SelElemK = EPairK.first;
            for(int i=0; i<2; ++i)	// 0=Analysis, 1=Design.
            {
                if(i==0)	{if(!m_pDoc->m_pPostCtrl->GetSectInfo()->Get_StrsResu(     SelElemK,StrsPost))	{ASSERT(0); continue;}}
                else			{if(!m_pDoc->m_pPostCtrl->GetSectInfo()->Get_StrsPscd(TRUE,SelElemK,StrsPost))	{ASSERT(0); continue;}}

                for(int j=0; j<2; ++j)	// 0=I, 1=J.
                {
                    T_STRSPOST_BASE StrsBase = (j==0 ? StrsPost.StrsBaseI : StrsPost.StrsBaseJ);
                    CString strShow=_T("");	strShow.Format(_T("%16s"), (i==0 ? _T("ANALYSIS") : _T("DESIGN")));
                    CString strElem=_T("");	strElem.Format(_T("%6d (%s)"), SelElemK, (j==0 ? _T("I") : _T("J")));
                    CString strType=_T("");	strType.Format(_T("%-20s"), (i==0 ? strSectAnl : strSectDgn));
                    CString strArea=_T("");	strArea.Format(_T("%16g"), StrsBase.dArea);
                    CString strIyy =_T("");	strIyy.Format( _T("%16g"), StrsBase.dIyy);
                    CString strIzz =_T("");	strIzz.Format( _T("%16g"), StrsBase.dIzz);
                    CString strQyb =_T("");	strQyb.Format( _T("%16g"), StrsBase.dQyb);
                    CString strQzb =_T("");	strQzb.Format( _T("%16g"), StrsBase.dQzb);
                    CString strKt  =_T("");	strKt.Format(  _T("%16g"), StrsBase.dKt);
                    CString strKtT =_T("");	strKtT.Format( _T("%16g"), StrsBase.dKtThik);
                    CString strPoM1=_T("");	strPoM1.Format(_T("%16g, %16g"), StrsBase.dyzM[0][0], StrsBase.dyzM[1][0]);
                    CString strPoM2=_T("");	strPoM2.Format(_T("%16g, %16g"), StrsBase.dyzM[0][1], StrsBase.dyzM[1][1]);
                    CString strPoM3=_T("");	strPoM3.Format(_T("%16g, %16g"), StrsBase.dyzM[0][2], StrsBase.dyzM[1][2]);
                    CString strPoM4=_T("");	strPoM4.Format(_T("%16g, %16g"), StrsBase.dyzM[0][3], StrsBase.dyzM[1][3]);
                    CString strPoV1=_T("");	strPoV1.Format(_T("%16g"), StrsBase.dyzV[1][0]);
                    CString strPoV2=_T("");	strPoV2.Format(_T("%16g"), StrsBase.dyzV[1][2]);
                    CString strPoV3=_T("");	strPoV3.Format(_T("%16g"), StrsBase.dyzV[1][4]);
                    CString strbQ1=_T("");	strbQ1.Format( _T("%16g, %16g"), StrsBase.db[0], StrsBase.dQ[0]);
                    CString strbQ2=_T("");	strbQ2.Format( _T("%16g, %16g"), StrsBase.db[1], StrsBase.dQ[1]);
                    CString strbQ3=_T("");	strbQ3.Format( _T("%16g, %16g"), StrsBase.db[2], StrsBase.dQ[2]);
                    fout<<endl
                        <<_T(" **************************************")<<endl
                        <<_T(" *. Show Type : ")<<strShow<<endl
                        <<_T(" *. ELEM (POSI) : ")<<strElem<<endl
                        <<_T(" *. Type      : ")<<strType<<endl
                        <<_T(" *. Area      : ")<<strArea<<endl
                        <<_T(" *. Iyy       : ")<<strIyy <<endl
                        <<_T(" *. Izz       : ")<<strIzz <<endl
                        <<_T(" *. Qyb       : ")<<strQyb <<endl
                        <<_T(" *. Qzb       : ")<<strQzb <<endl
                        <<_T(" *. Kt        : ")<<strKt  <<endl
                        <<_T(" *. Thik (Kt) : ")<<strKtT <<endl
                        <<_T(" *. y1,z1 (M) : ")<<strPoM1<<endl
                        <<_T(" *. y2,z2 (M) : ")<<strPoM2<<endl
                        <<_T(" *. y3,z3 (M) : ")<<strPoM3<<endl
                        <<_T(" *. y4,z4 (M) : ")<<strPoM4<<endl
                        <<_T(" *. Z1 (V)    : ")<<strPoV1<<endl
                        <<_T(" *. Z2 (V)    : ")<<strPoV2<<endl
                        <<_T(" *. Z3 (V)    : ")<<strPoV3<<endl
                        <<_T(" *. b1,Q1 (V) : ")<<strbQ1 <<endl
                        <<_T(" *. b2,Q2 (V) : ")<<strbQ2 <<endl
                        <<_T(" *. b3,Q3 (V) : ")<<strbQ3 <<endl
                        <<_T(" **************************************")<<endl
                        <<endl;
                    // Add by ZINU.('06.01.14). For Test Acor.
                    if(i==1)	// Only Design.
                    {
                        T_ELEM_D ElemD;
                        if(!m_pDoc->m_pPostCtrl->GetElemPost(SelElemK,ElemD)) ElemD.Initialize();
                        double dPh=0.0, dAoh=0.0, dLbarThk=0.0;
                        if(!CSectUtil::GetPscPeriAcor(TRUE, EPairK, (j==0?TRUE:FALSE), dPh, dAoh, dLbarThk, m_strTestSectFile))	ASSERT(0);
                    }
                }
            }

        }
    }
    //++++++++++++++++++++++++++
}

BOOL CDgnPscCommon::Make_PscDgnData()
{
    int iReturn=0;
    // 0 = OK.
    // 1 = NG Sect.
    // 2 = NG Secg.
    // 3 = NG Rbar.
    // 4 = NG Rshr.
    // 5 = NG Tndn (Elem)
    // 6 = NG Tndn (Tdna).
    // 7 = NG Matl.
    // 8 = NG Xib (JTG D62-04).
    /////////////////////////////////////////

    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    T_ACTL_D ActlD;
    if(!m_pDoc->m_pAttrCtrl->GetActl(ActlD))	ActlD.Initialize();
    m_pDoc->m_pAttrCtrl->GetActl(ActlD);
    BOOL bConsiderRbarAnl = ActlD.bConsiderRbar;
    BOOL bConsiderRbarDgn = CDBLib::IsConsiderRbarForDgn();
    m_bSameConsiderRbar = (bConsiderRbarAnl==bConsiderRbarDgn ? TRUE : FALSE);
    m_bChangeLcAxOfTapSect = ActlD.bChangeLcAxOfTapSect;
    m_pDataCtrl->SetChangeLocalAxisOfTapSect(m_bChangeLcAxOfTapSect);

    // Set Code.
    T_PSCD_D PscdD;
    PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
    m_iCode = PscdD.iDgnCode;
    m_nBridgeType = PscdD.iBrgType;
	//ConvertPscParamData(PscdD, m_SPscdD);

    BOOL bIsPoscChk = TRUE;
    switch (PscdD.iDgnCode)
    {
    case JSCE02:
        bIsPoscChk = FALSE;
        break;
    default: break;
    }

    // 설계 요소를 정하기 전에 Lpt에 해당하는 요소를 만들기 위해 여기서 호출
    Make_PscTdnaDgnInfo();
    m_pDataCtrl->MakeNumberofSupportByTendonBetweenAnchorage();

    // Set arElemK, arTdnaK.
    // Change by ZINU.('04.12.22). Get Only Selected Elems.
    m_arElemK.RemoveAll();
    ArrElemPairKey arElemK;
    if(m_pDataCtrl->Get_CrcElemListForDgn(TRUE, arElemK))
    {
        int nElemSize = arElemK.GetSize(); 
        for(int i=0; i<nElemSize; ++i)
        {
            ElemPairK EPairK = arElemK.GetAt(i);
            BOOL bPosc = FALSE;
            BOOL bPosx = FALSE;
            if (EPairK.second == EN_EL_BEAM)
            {
                bPosc = m_pDoc->m_pAttrCtrl->ExistPosc(EPairK.first);
                bPosx = m_pDoc->m_pAttrCtrl->ExistPosx(EPairK.first);
            }
            else if (EPairK.second == EN_EL_VBEAM)
            {
                bPosc = m_pDoc->m_pAttrCtrl->ExistPovc(EPairK.first);
                bPosx = m_pDoc->m_pAttrCtrl->ExistPosv(EPairK.first);
            }
            else ASSERT(0);

            BOOL bSaveElemK = bIsPoscChk ? bPosc : TRUE;
            // Save ElemK.
            if(bSaveElemK)	m_arElemK.Add(EPairK);
        }
        if(m_arElemK.GetSize()==0)  
        {
			m_arElemK.Copy(arElemK);			
        }
    }

    
    if (m_arElemK.GetSize() > 0)
    {        
        ArrElemPairKey aAddElemLp;
		if (GetCheckElemListAppTendonLp(m_arElemK, aAddElemLp))
		{
			m_arElemK.Append(aAddElemLp);
		}
    }

    m_arTdnaK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(m_arTdnaK);

    m_arTdntK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdntKeyList(m_arTdntK);

    Make_PscSpbfElem();
    m_pDataCtrl->MakeElemLength4TendonProfile();

    BOOL bComposite = m_pDoc->m_pPostCtrl->IsPSCCompositeSect();
    if(bComposite)
    {
        // PSC Composite 위함
        if(!Make_PscSectElem(TRUE)) return FALSE; //PSC Composite 환산단면
        Make_PscSecgElem(TRUE); //PSC Composite 순단면
    }

    if(iReturn==0)  iReturn = (Make_PscSectElem() ? 0 : 1);
    if(iReturn==0)  iReturn = (Make_PscSecgElem() ? 0 : 2);
    if(iReturn==0)  iReturn = (Make_PscRbarElem() ? 0 : 3);
    if(iReturn==0)  iReturn = (Make_PscRshrElem() ? 0 : 4);
    if(iReturn==0)  iReturn = (Make_PscTndnTdna() ? 0 : 6);
    if(iReturn==0)  iReturn = (Make_PscTndnElem() ? 0 : 5);
    if(iReturn==0)  iReturn = (Make_PscMatlElem() ? 0 : 7);	// Keep After Make_PscTndnElem().
    if(iReturn==0)  iReturn = (Make_PscTndnTdnt() ? 0 : 8);
    if(iReturn==0 && (m_iCode==JTG_D62_04 || m_iCode==CJJ_11_2011))  iReturn = (Make_PscXib_JTG() ? 0 : 8);
    if(iReturn==0 && (CDBLib::IsPscCodeLSD(m_iCode) ||
        m_iCode==SNiP_20503_84_PSC  || m_iCode==SP_35_13330_11_PSC ||
        m_iCode==SNiP_20503_84_PSC_MKS || m_iCode==SP_35_13330_11_PSC_MKS ||
        m_iCode==IRC_112_2011_PSC   || m_iCode==AS_5100_5_17_PSC || 
		m_iCode==IRS_PSC || m_iCode==BS5400_90_PSC || m_iCode == TMH07_3_1989 ||
        m_iCode==BD_44_15_PSC || m_iCode==CS_455_PSC ||
        m_iCode==IRC_112_2020_PSC))						                       iReturn = (Make_PscExpcElem() ? 0 : 10);
    if(iReturn==0 && (CDBLib::IsPscCodeLSD(m_iCode) ||
        m_iCode==SNiP_20503_84_PSC_MKS || m_iCode==SP_35_13330_11_PSC_MKS || 
        m_iCode==IRC_112_2011_PSC   || m_iCode==AS_5100_5_17_PSC ||
		m_iCode==IRS_PSC || m_iCode == BS5400_90_PSC || m_iCode == TMH07_3_1989 ||
        m_iCode == IRC_112_2020_PSC))						                   iReturn = (Make_PscPscnElem() ? 0 : 11);
    if(iReturn==0 && (m_iCode==EUROCODE2_2_05_PSC || m_iCode==IRC_112_2011_PSC ||
        m_iCode==AS_5100_5_17_PSC || m_iCode==IRS_PSC ||
        m_iCode == IRC_112_2020_PSC || m_iCode==ABNT_NBR_6118_2023_PSC))
        iReturn = (Make_PscFackElem() ? 0 : 12);
    if(iReturn==0 && (CDBLib::IsPscCodeAASHTOSince12(m_iCode) ||
        CDBLib::IsPscCodeCSA(m_iCode)))                                        iReturn = (Make_PscPscsElem() ? 0 : 13);
    if( iReturn==0) iReturn = (Make_PscCinfElem() ? 0 : 14);
	
    if(iReturn==0)  return TRUE;
    else            return FALSE;
}

BOOL CDgnPscCommon::Make_PscDgnDataForJTG_TndnStress()
{
    int iReturn=0;
    // 0 = OK.
    // 1 = NG Sect.
    // 2 = NG Secg.
    // 3 = NG Rbar.
    // 4 = NG Rshr.
    // 5 = NG Tndn (Elem)
    // 6 = NG Tndn (Tdna).
    // 7 = NG Matl.
    // 8 = NG Xib (JTG D62-04).
    /////////////////////////////////////////

    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    T_ACTL_D ActlD;
    if(!m_pDoc->m_pAttrCtrl->GetActl(ActlD))	ActlD.Initialize();
    m_pDoc->m_pAttrCtrl->GetActl(ActlD);
    BOOL bConsiderRbarAnl = ActlD.bConsiderRbar;
    BOOL bConsiderRbarDgn = CDBLib::IsConsiderRbarForDgn();
    m_bSameConsiderRbar = (bConsiderRbarAnl==bConsiderRbarDgn ? TRUE : FALSE);

    // Set Code.
    T_PSCD_D PscdD;
    PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
    m_iCode = PscdD.iDgnCode;
    // Set arElemK, arTdnaK.
    // Change by ZINU.('04.12.22). Get Only Selected Elems.
    m_arElemK.RemoveAll();
    ArrElemPairKey arElemK;
    if(m_pDataCtrl->Get_ElemListForTendon(arElemK))
    {
        for(int i=0; i<arElemK.GetSize(); ++i)
        {
            //if(PscdD.iDgnCode!=JTG_D62_04) ASSERT(0);
            if(!(PscdD.iDgnCode==JTG_D62_04 || PscdD.iDgnCode==CJJ_11_2011)) ASSERT(0);
            // Save ElemK.
            m_arElemK.Copy(arElemK);
        }
    }
    m_arTdnaK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(m_arTdnaK);

    m_arTdntK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdntKeyList(m_arTdntK);

    if(iReturn==0)  iReturn = (Make_PscSectElem() ? 0 : 1);
    if(iReturn==0)  iReturn = (Make_PscSecgElem() ? 0 : 2);
    if(iReturn==0)  iReturn = (Make_PscRbarElem() ? 0 : 3);
    if(iReturn==0)  iReturn = (Make_PscRshrElem() ? 0 : 4);
    if(iReturn==0)  iReturn = (Make_PscTndnTdna() ? 0 : 6);
    if(iReturn==0)  iReturn = (Make_PscTndnElem() ? 0 : 5);
    if(iReturn==0)  iReturn = (Make_PscMatlElem() ? 0 : 7);	// Keep After Make_PscTndnElem().
    if(iReturn==0)  iReturn = (Make_PscTndnTdnt() ? 0 : 8);
    if(iReturn==0 && (m_iCode==JTG_D62_04 || m_iCode==CJJ_11_2011))     iReturn = (Make_PscXib_JTG() ? 0 : 8);
    
    if(iReturn==0)  return TRUE;
    else            return FALSE;
}

BOOL CDgnPscCommon::Make_BridgeLoadRatingDgnData()
{
    int iReturn=0;
    // 0 = OK.
    // 1 = NG Sect.
    // 2 = NG Secg.
    // 3 = NG Rbar.
    // 4 = NG Rshr.
    // 5 = NG Tndn (Elem)
    // 6 = NG Tndn (Tdna).
    // 7 = NG Matl.
    // 8 = NG Xib (JTG D62-04).
    /////////////////////////////////////////

    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    T_ACTL_D ActlD;
    if(!m_pDoc->m_pAttrCtrl->GetActl(ActlD))	ActlD.Initialize();
    m_pDoc->m_pAttrCtrl->GetActl(ActlD);
    BOOL bConsiderRbarAnl = ActlD.bConsiderRbar;
    BOOL bConsiderRbarDgn = CDBLib::IsConsiderRbarForDgn();
    m_bSameConsiderRbar = (bConsiderRbarAnl==bConsiderRbarDgn ? TRUE : FALSE);

    // Set Code.
    // 	T_PSCD_D PscdD;
    // 	PscdD.Initialize();
    // 	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
    T_RACD_D RacdD;  RacdD.Initialize();	
    m_pDoc->m_pAttrCtrl2->GetDgnRacd(RacdD);	

    switch ( RacdD.nRatingCode )
    {
    case KSCE_USD05_RATING:
        m_iCode = KSCE_USD05;
        break;
    case AASHTO_LRFD05_PSC_RATING:
        m_iCode = AASHTO_LRFD08;
        break;
    case AASHTO_LRFD12_PSC_RATING:
        m_iCode = AASHTO_LRFD12;
        break;
    case AASHTO_LRFD19_PSC_RATING:
        m_iCode = AASHTO_LRFD17;
        break;
    case KSCE_USD10_RATING:
        m_iCode = KSCE_USD10;
        break;
    case KSCE_RAIL_USD11_RATING:
        m_iCode = KSCE_RAIL_USD11;
        break;
    case KSCE_LSD15_RATING:
        m_iCode = KSCE_LSD15_PSC;
        break;
    case CS_454_20_PSC_RATING:
        m_iCode = CS_455_PSC;
        break;
    default:
        ASSERT(0);
        break;
    }

    // Set arElemK, arTdnaK.
    // Change by ZINU.('04.12.22). Get Only Selected Elems.
    m_arElemK.RemoveAll();
    if(m_pDataCtrl->Get_ElemListForBlrDgn(TRUE, m_arElemK))

        m_arTdnaK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(m_arTdnaK);

    m_arTdntK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdntKeyList(m_arTdntK);

    Make_PscSpbfElem();
    m_pDataCtrl->MakeElemLength4TendonProfile();
    m_pDataCtrl->MakeNumberofSupportByTendonBetweenAnchorage();

    BOOL bComposite = m_pDoc->m_pPostCtrl->IsPSCCompositeSect();
    if(bComposite)
    {
        // PSC Composite 위함
        Make_PscSectElem(TRUE); //PSC Composite 환산단면
        Make_PscSecgElem(TRUE); //PSC Composite 순단면
    }

    if(iReturn==0)  iReturn = (Make_PscSectElem() ? 0 : 1);
    if(iReturn==0)  iReturn = (Make_PscSecgElem() ? 0 : 2);
    if(iReturn==0)  iReturn = (Make_PscRbarElem() ? 0 : 3);
    if(iReturn==0)  iReturn = (Make_PscRshrElem() ? 0 : 4);
    if(iReturn==0)  iReturn = (Make_PscTndnTdna() ? 0 : 6);
    if(iReturn==0)  iReturn = (Make_PscTndnElem(DGN_TYPE_RATING) ? 0 : 5);
    if(iReturn==0)  iReturn = (Make_PscMatlElem(DGN_TYPE_RATING) ? 0 : 7);	// Keep After Make_PscTndnElem().
    if(iReturn==0)  iReturn = (Make_PscTndnTdnt() ? 0 : 8);
    if(iReturn==0)  iReturn = (Make_PscCinfElem() ? 0 : 9);
    if(iReturn==0 && (m_iCode==BS5400_90_PSC || m_iCode==BD_44_15_PSC || m_iCode==CS_455_PSC || m_iCode == TMH07_3_1989))
    {
        iReturn = (Make_PscExpcElem() ? 0 : 10);
        iReturn = (Make_PscPscnElem() ? 0 : 11);
    }

    if(iReturn==0)  return TRUE;
    else            return FALSE;
}

BOOL CDgnPscCommon::Make_PlateGirderDgnData()
{
    CCurUnitSaver Save(TRUE);

    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit); 

    int iReturn=0;

    //!!/ Plate Girder 에도 적용해야 도는 것인가?.
    /*
    T_ACTL_D ActlD;
    if(!m_pDoc->m_pAttrCtrl->GetActl(ActlD))	ActlD.Initialize();
    m_pDoc->m_pAttrCtrl->GetActl(ActlD);
    BOOL bConsiderRbarAnl = ActlD.bConsiderRbar;
    BOOL bConsiderRbarDgn = CDBLib::IsConsiderRbarForDgn();
    m_bSameConsiderRbar = (bConsiderRbarAnl==bConsiderRbarDgn ? TRUE : FALSE);
    */

    // Set arElemK
    // Change by ZINU.('04.12.22). Get Only Selected Elems.
    m_arElemK.RemoveAll();
	ArrElemPairKey arElemK;
    if(m_pDataCtrl->Get_ElemListForPlateGirder(TRUE, arElemK))
    {
        for(int i=0; i<arElemK.GetSize(); ++i)
        {
            ElemPairK ElemK = arElemK.GetAt(i);			
            //T_CGDP_D
            BOOL bCgdp = FALSE;
            if (ElemK.second == EN_EL_BEAM)
                bCgdp = m_pDoc->m_pAttrCtrl->ExistCgdp(ElemK.first);
            else if (ElemK.second == EN_EL_VBEAM)
                bCgdp = m_pDoc->m_pAttrCtrl->ExistCgdv(ElemK.first);
			else ASSERT(0);

            BOOL bSaveElemK=FALSE;
            if(CDBLib::IsCSGCode(m_iCivilCode)==TRUE)
            {
                bSaveElemK = bCgdp;
            }
            else	ASSERT(0);
            // Save ElemK.
            if(bSaveElemK)	m_arElemK.Add(ElemK);
        }
        if(m_arElemK.GetSize()==0)  
        {
            m_arElemK.Copy(arElemK);
        }
    }

    if(iReturn==0)  iReturn = (Make_CpgMatlElem() ? 0 : 1);  
    if(iReturn==0)  iReturn = (Make_CpgSectElem() ? 0 : 2);
    if(iReturn==0)  iReturn = (Make_CpgRbarElem() ? 0 : 3);	
    if(iReturn==0)  iReturn = (Make_CpgSconElem() ? 0 : 4);
    if(iReturn==0)  iReturn = (Make_CpgLstiElem() ? 0 : 5);
    if(iReturn==0)  iReturn = (Make_CpgTrstElem() ? 0 : 6);
    if(iReturn==0)  iReturn = (Make_CpgTsesElem() ? 0 : 7);
    if(iReturn==0)  iReturn = (Make_CpgBcdtElem() ? 0 : 8); // Buckling coefficients for different types of load application.
    if(iReturn==0)  iReturn = (Make_CpgLtbdElem() ? 0 : 9); // Lateral Torsional Buckling Data
    if(iReturn==0)  iReturn = (Make_CpgFrdtElem() ? 0 : 10); // Fatigue Data
    if(iReturn==0)  iReturn = (Make_CpgMembElem() ? 0 : 11); // Member Data

    CString strErrMsg;   
    switch (iReturn)
    {
    case 1:strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_MATERIAL));  GSaveHistoryFormatNF(strErrMsg);   break;
    case 2:strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_SECTION));   GSaveHistoryFormatNF(strErrMsg);   break;
    default:  break;
    }

    if(iReturn==0)  return TRUE;
    else            return FALSE;

}

BOOL CDgnPscCommon::Make_PscMatlElem(int nDgnType)
{
    m_amMatlElem.clear();
    _PSC_MATL_ELEM MatlElem;

    T_PSCD_D PscdD;
    PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    T_MATD_D MatdD;

    if ( nDgnType == DGN_TYPE_RATING )
    {
        T_RAKR_D RakrD; RakrD.Initialize();
        m_pDoc->m_pAttrCtrl2->GetRakr(RakrD);
        for ( int j=0; j<4; j++ )
        {
            PscdD.dPhi_c[j] = RakrD.dPhi_c[j];
            PscdD.dPhi_ps[j] = RakrD.dPhi_ps[j];
            PscdD.dPhi_s[j] = RakrD.dPhi_s[j];
        }
    }

    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {
        MatlElem.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        
        MatdD.Initialize();
        if ( !m_pDataCtrl->Get_ConMatd(ElemK, MatdD) )	continue;

        switch ( m_iCode )
        {
        case JTG_D62_04:
        case CJJ_11_2011:
            if( !Make_PscMatlElem_JTG(ElemK, PscdD, MatdD, MatlElem) ) { ASSERT(0); continue; }
            break;
        case EUROCODE2_2_05_PSC:
            if ( !Make_PscMatlElem_EURO(ElemK, PscdD, MatdD, MatlElem) ) { ASSERT(0); continue; }
            break;
        case ABNT_NBR_6118_2023_PSC:
            if (!Make_PscMatlElem_BRA(ElemK, PscdD, MatdD, MatlElem)) { ASSERT(0); continue; }
            break;
        case IRC_112_2011_PSC:
        case IRC_112_2020_PSC:
            if ( !Make_PscMatlElem_IRC(ElemK, PscdD, MatdD, MatlElem) ) { ASSERT(0); continue; }
            break;
        case AS_5100_5_17_PSC:
            if ( !Make_PscMatlElem_AS(ElemK, PscdD, MatdD, MatlElem) ) { ASSERT(0); continue; }
            break;
        case KDS_24_14_21_2022_PSC:
        case KSCE_LSD15_PSC:
            if ( !Make_PscMatlElem_KSCE(ElemK, PscdD, MatdD, MatlElem) ) { ASSERT(0); continue; }
            break;
        case SNiP_20503_84_PSC:
        case SP_35_13330_11_PSC:
        case SNiP_20503_84_PSC_MKS:
        case SP_35_13330_11_PSC_MKS:
            if ( !Make_PscMatlElem_RUS(ElemK, PscdD, MatdD, MatlElem) ) { ASSERT(0); continue; }
            break;
        case BS5400_90_PSC:
		case TMH07_3_1989:
        case BD_44_15_PSC:
        case CS_455_PSC:
            if ( !Make_PscMatlElem_BS(ElemK, PscdD, MatdD, MatlElem) ) { ASSERT(0); continue; }
            break;
        case IRS_PSC:
            if ( !Make_PscMatlElem_IRS(ElemK, PscdD, MatdD, MatlElem) ) { ASSERT(0); continue; }
            break;
        default:
            break;
        }
        // Set Matd.
        MatlElem.MatdD = MatdD;
        m_amMatlElem[ElemK] = MatlElem;
        // Add by ZINU.('05.04.11). Set ftk.
        m_pForcCtrl->m_arftkData.insert({ ElemK, MatlElem.dftk});
    }
    return TRUE;
}

BOOL CDgnPscCommon::Make_PscMatlElem_JTG(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem)
{
    /////////////////////////////////////
    // Concrete.
    double dfck=0.0, dftk=0.0, dfckp=0.0, dftkp=0.0, dfcd=0.0, dftd=0.0, dGc=0.0;
    if ( !Get_MatlConc(ElemK, dfck, dftk, dfckp, dftkp, dfcd, dftd, dGc) ) { ASSERT(0); return FALSE; }
    MatlElem.dfck	 = dfck;
    MatlElem.dftk  = dftk;
    MatlElem.dfckp = dfckp;
    MatlElem.dftkp = dftkp;
    MatlElem.dfcd  = dfcd;
    MatlElem.dftd  = dftd;
    /////////////////////////////////////
    // Rbar.
    double dfskM=0.0, dfsdtM=0.0, dfsdcM=0.0;
    double dfskS=0.0, dfsdtS=0.0, dfsdcS=0.0;
    if ( !Get_MatlRbar(ElemK, dfskM, dfsdtM, dfsdcM, dfskS, dfsdtS, dfsdcS) ) { ASSERT(0); return FALSE; }
    // Main Rbar.
    MatlElem.dfskM	= dfskM;
    MatlElem.dfsdtM	= dfsdtM;
    MatlElem.dfsdcM	= dfsdcM;
    // Sub Rbar.
    MatlElem.dfskS	= dfskS;
    MatlElem.dfsdtS	= dfsdtS;
    MatlElem.dfsdcS	= dfsdcS;
    // Diagonal Rbar.
    MatlElem.dfskD	= dfskS;	// Assume by ZINU.
    MatlElem.dfsdtD	= dfsdtS;	// Assume by ZINU.
    MatlElem.dfsdcD	= dfsdcS;	// Assume by ZINU.

    // 전단강봉.
    _PSC_TNDN_ELEM TndnElem;
    TndnElem.Initialize();
    if ( !Get_PscTndnElem(TRUE, ElemK, TndnElem) )	ASSERT(0);

    for ( int j=0; j<2; ++j )
    {
        if ( TndnElem.TndnPosi[j].arTndnPosi.GetSize()==0 )	continue;
        // Assumed by ZINU.
        MatlElem.dfpkV[j]  = TndnElem.TndnPosi[j].arTndnPosi[0].dfpk;		// by Code Unit.
        MatlElem.dfpdtV[j] = TndnElem.TndnPosi[j].arTndnPosi[0].dfpdt;	// by Code Unit.
        MatlElem.dfpdcV[j] = TndnElem.TndnPosi[j].arTndnPosi[0].dfpdc;	// by Code Unit.
    }

    if ( dGc > 0.0 ) MatlElem.MatdD.Data1.Analysis.ShearModulus = dGc;	// See JTG-D62-04 Table 3.1.7.

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscMatlElem_EURO(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem)
{
    T_ELEM_D ElemD;
    ElemD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemD))	return FALSE;
    }

    int nMatlK = m_pDataCtrl->GetMaterialKey4ElemPairK(ElemK);

    //우선 Eurocode에서의 fctm 값에 사용하기 위해 작업 : CIVIL-2644

    T_TMAT_D TmatD; TmatD.Initialize();
    T_TDME_D TdmeGrD; TdmeGrD.Initialize();
    T_TDME_D TdmeSlD; TdmeSlD.Initialize();

    BOOL bGetTdme = FALSE;
    if ( m_pDoc->m_pPostCtrl->GetTmatPost(nMatlK, TmatD) )
    {
        bGetTdme = m_pDoc->m_pAttrCtrl->GetTdme(TmatD.TdMatlElastKey, TdmeGrD);
    }

    BOOL bPscComposite = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

    T_CSCS_BASE CscsB; CscsB.Initialize();

    if ( bPscComposite )
    {
        if (ElemK.second == EN_EL_BEAM)
        {
            CArray<T_CSCS_K, T_CSCS_K> aCscsKey;
            CArray<T_CSCS_D, T_CSCS_D&> aCscsData;
            if (m_pDoc->m_pAttrCtrl->GetCscsList(ElemD.elpro, aCscsKey, aCscsData))
            {
                if (aCscsData.GetSize() > 0)
                {
                    // 따라서 DataList의 첫번째, aPart의 두번째 정보가 Slab 정보!
                    if (aCscsData.GetAt(0).aPart.GetSize() > 1) CscsB = aCscsData.GetAt(0).aPart.GetAt(1);
                }
            }
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            CArray<T_CSCV_K, T_CSCV_K> aCscvKey;
            CArray<T_CSCV_D, T_CSCV_D&> aCscvData;
            if (m_pDoc->m_pAttrCtrl->GetCscvList(ElemK.first, aCscvKey, aCscvData)) // Composite Section for C.S. 의 경우 단면당 하나만 정의할 수 있음!
            {
                if (aCscvData.GetSize() > 0)
                {
                    if (aCscvData.GetAt(0).aPart.GetSize() > 1) CscsB = aCscvData.GetAt(0).aPart.GetAt(1);
                }
            }
        }
        else ASSERT(0);
    }

    T_TMAT_D TmatSL; TmatSL.Initialize();
    BOOL bGetTdmeSL = FALSE;
    if ( m_pDoc->m_pPostCtrl->GetTmatPost(CscsB.MatlKey, TmatSL) )
    {
        bGetTdmeSL = m_pDoc->m_pAttrCtrl->GetTdme(TmatSL.TdMatlElastKey, TdmeSlD);
    }

    int iNA = PscdD.nNationalAnnex;
    m_pNA = CNationalAnnexInterface::Instance();
    const auto Code = m_pNA->GetBridgeCode(dgn::def::enDgn::RCS, iNA);
    m_pNA->SetCode(Code);

    BOOL bUser = PscdD.bUserInputData;
    double dAlpha_cc = PscdD.dAlpha_cc;
    double dGamma_c_PT = PscdD.dGamma_c_PT;
    double dGamma_c_A  = PscdD.dGamma_c_A;
    double dGamma_s_PT = PscdD.dGamma_s_PT;
    double dGamma_s_A  = PscdD.dGamma_s_A;

    // girder
    double dfck = MatdD.Data1.Design.C_fc;
    double dfyk = MatdD.MainRebarData.B_fy;
    double dfywk = MatdD.SubRebarData.B_fy;

    if ( bGetTdme )
    {
        MatlElem.dfcm = TdmeGrD.dCompStrength;
    }
    else
    {
        MatlElem.dfcm  = dfck + 8.0;
    }

    MatlElem.dfctm = Get_fctm_EC2(dfck, MatlElem.dfcm);    
    MatlElem.dfctk = 0.7*MatlElem.dfctm;  // fctk;0.05

    int nType=0; // nType : 0=Persistent & Transient, 1=Accidental
    double dfcd_PT=0.0, dfcd_A=0.0;
    double dfyd_PT=0.0, dfyd_A=0.0;
    double dfywd_PT=0.0, dfywd_A=0.0;
    Get_fcd_4EuroCode(iNA, bUser, dAlpha_cc, dGamma_c_PT, dGamma_c_A, dfck, dfcd_PT, dfcd_A);
    Get_fyd_4EuroCode(iNA, bUser, dGamma_s_PT, dGamma_s_A, dfyk, dfywk, dfyd_PT, dfyd_A, dfywd_PT, dfywd_A);

    MatlElem.dfcd_PT = dfcd_PT;
    MatlElem.dfcd_A  = dfcd_A;
    MatlElem.dfyd_PT = dfyd_PT;
    MatlElem.dfyd_A  = dfyd_A;
    MatlElem.dfywd_PT = dfywd_PT;
    MatlElem.dfywd_A  = dfywd_A;
    //
    MatlElem.dfci = 0.8*dfck; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 

    // slab
    if ( bPscComposite )
    {
        // 슬래브
        double dfck_s  = MatdD.Data2.Design.C_fc;
        double dfyk_s  = MatdD.MainRebarData2.B_fy;
        double dfywk_s = MatdD.SubRebarData2.B_fy;

        if ( bGetTdmeSL )
        {
            MatlElem.dfcm_s  = TdmeSlD.dCompStrength;
        }
        else
        {
            MatlElem.dfcm_s  = dfck_s + 8.0;
        }

        MatlElem.dfctm_s = Get_fctm_EC2(dfck_s, MatlElem.dfcm_s);
        MatlElem.dfctk_s = 0.7*MatlElem.dfctm_s;  // fctk;0.05

        double dfcd_PT_s =0.0, dfcd_A_s =0.0;
        double dfyd_PT_s =0.0, dfyd_A_s =0.0;
        double dfywd_PT_s=0.0, dfywd_A_s=0.0;
        Get_fcd_4EuroCode(iNA, bUser, dAlpha_cc, dGamma_c_PT, dGamma_c_A, dfck_s, dfcd_PT_s, dfcd_A_s);
        Get_fyd_4EuroCode(iNA, bUser, dGamma_s_PT, dGamma_s_A, dfyk_s, dfywk_s, dfyd_PT_s, dfyd_A_s, dfywd_PT_s, dfywd_A_s);

        MatlElem.dfcd_PT_s  = dfcd_PT_s;
        MatlElem.dfcd_A_s   = dfcd_A_s;
        MatlElem.dfyd_PT_s  = dfyd_PT_s;
        MatlElem.dfyd_A_s   = dfyd_A_s;
        MatlElem.dfywd_PT_s = dfywd_PT_s;
        MatlElem.dfywd_A_s  = dfywd_A_s;
        //
        MatlElem.dfci_s = 0.8*dfck_s; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscMatlElem_BRA(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem)
{
    T_ELEM_D ElemD;
    ElemD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemD))	return FALSE;    
    }

    int nMatlK = m_pDataCtrl->GetMaterialKey4ElemPairK(ElemK);    

    //우선 Eurocode에서의 fctm 값에 사용하기 위해 작업 : CIVIL-2644

    T_TMAT_D TmatD; TmatD.Initialize();
    T_TDME_D TdmeGrD; TdmeGrD.Initialize();
    T_TDME_D TdmeSlD; TdmeSlD.Initialize();

    BOOL bGetTdme = FALSE;
    if ( m_pDoc->m_pPostCtrl->GetTmatPost(nMatlK, TmatD) )
    {
        bGetTdme = m_pDoc->m_pAttrCtrl->GetTdme(TmatD.TdMatlElastKey, TdmeGrD);
    }

    BOOL bPscComposite = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

    T_CSCS_BASE CscsB; CscsB.Initialize();

    if ( bPscComposite )
    {
        if (ElemK.second == EN_EL_BEAM)
        {
            CArray<T_CSCS_K, T_CSCS_K> aCscsKey;
            CArray<T_CSCS_D, T_CSCS_D&> aCscsData;
            if (m_pDoc->m_pAttrCtrl->GetCscsList(ElemD.elpro, aCscsKey, aCscsData))
            {
                if (aCscsData.GetSize() > 0)
                {
                    // 따라서 DataList의 첫번째, aPart의 두번째 정보가 Slab 정보!
                    if (aCscsData.GetAt(0).aPart.GetSize() > 1) CscsB = aCscsData.GetAt(0).aPart.GetAt(1);
                }
            }
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            CArray<T_CSCV_K, T_CSCV_K> aCscvKey;
            CArray<T_CSCV_D, T_CSCV_D&> aCscvData;
            if (m_pDoc->m_pAttrCtrl->GetCscvList(ElemK.first, aCscvKey, aCscvData)) // Composite Section for C.S. 의 경우 단면당 하나만 정의할 수 있음!
            {
                if (aCscvData.GetSize() > 0)
                {
                    if (aCscvData.GetAt(0).aPart.GetSize() > 1) CscsB = aCscvData.GetAt(0).aPart.GetAt(1);
                }
            }
        }
        else ASSERT(0);
    }

    T_TMAT_D TmatSL; TmatSL.Initialize();
    BOOL bGetTdmeSL = FALSE;
    if ( m_pDoc->m_pPostCtrl->GetTmatPost(CscsB.MatlKey, TmatSL) )
    {
        bGetTdmeSL = m_pDoc->m_pAttrCtrl->GetTdme(TmatSL.TdMatlElastKey, TdmeSlD);
    }

    BOOL bUser = PscdD.bUserInputData;
    double dGamma_c_N  = PscdD.dGamma_c_N_NBR;
    double dGamma_c_SC = PscdD.dGamma_c_SC_NBR;
    double dGamma_s_N  = PscdD.dGamma_s_N_NBR;
    double dGamma_s_SC = PscdD.dGamma_s_SC_NBR;

    // girder
    double dfck = MatdD.Data1.Design.C_fc;
    double dfyk = MatdD.MainRebarData.B_fy;
    double dfywk = MatdD.SubRebarData.B_fy;

    if ( bGetTdme )
    {
        MatlElem.dfcm = TdmeGrD.dCompStrength;
    }
    else
    {
        MatlElem.dfcm  = dfck + 8.0;
    }

    MatlElem.dfctm = Get_fctm_EC2(dfck, MatlElem.dfcm);    
    MatlElem.dfctk = 0.7*MatlElem.dfctm;  // fctk;0.05

    int nType=0; // nType : 0=Persistent & Transient, 1=Accidental
    double dfcd_N=0.0, dfcd_SC=0.0;
    double dfyd_N=0.0, dfyd_SC=0.0;
    double dfywd_N=0.0, dfywd_SC=0.0;
    Get_fcd_4BRA(bUser, dGamma_c_N, dGamma_c_SC, dfck, dfcd_N, dfcd_SC);
    Get_fyd_4BRA(bUser, dGamma_s_N, dGamma_s_SC, dfyk, dfywk, dfyd_N, dfyd_SC, dfywd_N, dfywd_SC);

    MatlElem.dfcd_PT = dfcd_N;
    MatlElem.dfcd_A  = dfcd_SC;
    MatlElem.dfyd_PT = dfyd_N;
    MatlElem.dfyd_A  = dfyd_SC;
    MatlElem.dfywd_PT = dfywd_N;
    MatlElem.dfywd_A  = dfywd_SC;
    //
    MatlElem.dfci = 0.8*dfck; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 

    // slab
    if ( bPscComposite )
    {
        // 슬래브
        double dfck_s  = MatdD.Data2.Design.C_fc;
        double dfyk_s  = MatdD.MainRebarData2.B_fy;
        double dfywk_s = MatdD.SubRebarData2.B_fy;

        if ( bGetTdmeSL )
        {
            MatlElem.dfcm_s  = TdmeSlD.dCompStrength;
        }
        else
        {
            MatlElem.dfcm_s  = dfck_s + 8.0;
        }

        MatlElem.dfctm_s = Get_fctm_EC2(dfck_s, MatlElem.dfcm_s);
        MatlElem.dfctk_s = 0.7*MatlElem.dfctm_s;  // fctk;0.05

        double dfcd_N_s =0.0, dfcd_SC_s =0.0;
        double dfyd_N_s =0.0, dfyd_SC_s =0.0;
        double dfywd_N_s=0.0, dfywd_SC_s=0.0;
        Get_fcd_4BRA(bUser, dGamma_c_N, dGamma_c_SC, dfck_s, dfcd_N_s, dfcd_SC_s);
        Get_fyd_4BRA(bUser, dGamma_s_N, dGamma_s_SC, dfyk_s, dfywk_s, dfyd_N_s, dfyd_SC_s, dfywd_N_s, dfywd_SC_s);

        MatlElem.dfcd_PT_s  = dfcd_N_s;
        MatlElem.dfcd_A_s   = dfcd_SC_s;
        MatlElem.dfyd_PT_s  = dfyd_N_s;
        MatlElem.dfyd_A_s   = dfyd_SC_s;
        MatlElem.dfywd_PT_s = dfywd_N_s;
        MatlElem.dfywd_A_s  = dfywd_SC_s;
        //
        MatlElem.dfci_s = 0.8*dfck_s; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscMatlElem_IRC(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem)
{
    BOOL bPscComposite = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

    int iNA = 0;//Not Required
    BOOL bUser = PscdD.bUserInputData;
    double dAlpha_cc = PscdD.dAlpha_cc;
    double dGamma_c_PT = PscdD.dGamma_c_PT;
    double dGamma_c_A  = PscdD.dGamma_c_A;
    double dGamma_s_PT = PscdD.dGamma_s_PT;
    double dGamma_s_A  = PscdD.dGamma_s_A;

    // girder
    double dfck = MatdD.Data1.Design.C_fc;
    double dfyk = MatdD.MainRebarData.B_fy;
    double dfywk = MatdD.SubRebarData.B_fy;

    MatlElem.dfcm  = dfck + 10.0;
    if ( dfck<60.0 ) MatlElem.dfctm = 0.259*pow(dfck, 2.0/3.0);
    else          MatlElem.dfctm = 2.27*log(1.+(MatlElem.dfcm/12.5));
    MatlElem.dfctk = 0.7*MatlElem.dfctm;  // fctk;0.05

    int nType=0; // nType : 0=Persistent & Transient, 1=Accidental
    double dfcd_PT=0.0, dfcd_A=0.0;
    double dfyd_PT=0.0, dfyd_A=0.0;
    double dfywd_PT=0.0, dfywd_A=0.0;
    Get_fcd_4IRC112(iNA, bUser, dAlpha_cc, dGamma_c_PT, dGamma_c_A, dfck, dfcd_PT, dfcd_A);
    Get_fyd_4IRC112(iNA, bUser, dGamma_s_PT, dGamma_s_A, dfyk, dfywk, dfyd_PT, dfyd_A, dfywd_PT, dfywd_A);

    MatlElem.dfcd_PT = dfcd_PT;
    MatlElem.dfcd_A  = dfcd_A;
    MatlElem.dfyd_PT = dfyd_PT;
    MatlElem.dfyd_A  = dfyd_A;
    MatlElem.dfywd_PT = dfywd_PT;
    MatlElem.dfywd_A  = dfywd_A;
    //
    MatlElem.dfci = 0.8*dfck; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 

    // slab
    if ( bPscComposite )
    {

        // 슬래브
        double dfck_s  = MatdD.Data2.Design.C_fc;
        double dfyk_s  = MatdD.MainRebarData2.B_fy;
        double dfywk_s = MatdD.SubRebarData2.B_fy;

        MatlElem.dfcm_s  = dfck_s + 10.0;
        if ( dfck_s<60.0 ) MatlElem.dfctm_s = 0.259*pow(dfck_s, 2.0/3.0);//CIVIL-6770
        else            MatlElem.dfctm_s = 2.27*log(1.+(MatlElem.dfcm_s /12.5));//CIVIL-6770
        MatlElem.dfctk_s = 0.7*MatlElem.dfctm_s;  // fctk;0.05

        double dfcd_PT_s =0.0, dfcd_A_s =0.0;
        double dfyd_PT_s =0.0, dfyd_A_s =0.0;
        double dfywd_PT_s=0.0, dfywd_A_s=0.0;
        Get_fcd_4IRC112(iNA, bUser, dAlpha_cc, dGamma_c_PT, dGamma_c_A, dfck_s, dfcd_PT_s, dfcd_A_s);
        Get_fyd_4IRC112(iNA, bUser, dGamma_s_PT, dGamma_s_A, dfyk_s, dfywk_s, dfyd_PT_s, dfyd_A_s, dfywd_PT_s, dfywd_A_s);

        MatlElem.dfcd_PT_s  = dfcd_PT_s;
        MatlElem.dfcd_A_s   = dfcd_A_s;
        MatlElem.dfyd_PT_s  = dfyd_PT_s;
        MatlElem.dfyd_A_s   = dfyd_A_s;
        MatlElem.dfywd_PT_s = dfywd_PT_s;
        MatlElem.dfywd_A_s  = dfywd_A_s;
        //
        MatlElem.dfci_s = 0.8*dfck_s; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 
    }
    return TRUE;
}

BOOL CDgnPscCommon::Make_PscMatlElem_AS(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D& MatdD, _PSC_MATL_ELEM& MatlElem)
{
    CArray<T_STAG_K, T_STAG_K> aStagK;
    m_pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagK);
    BOOL bPscComposite = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

    // 검토해서 수정해야함.
    int iNA = PscdD.nNationalAnnex;

	m_pNA = CNationalAnnexInterface::Instance();
	const auto Code = m_pNA->GetBridgeCode(dgn::def::enDgn::RCS, iNA);
	m_pNA->SetCode(Code);

    BOOL bUser = PscdD.bUserInputData;
    double dAlpha_cc = PscdD.dAlpha_cc;
    double dGamma_c_PT = PscdD.dGamma_c_PT;
    double dGamma_c_A  = PscdD.dGamma_c_A;
    double dGamma_s_PT = PscdD.dGamma_s_PT;
    double dGamma_s_A  = PscdD.dGamma_s_A;

    // girder
    double dfck = MatdD.Data1.Design.C_fc;
    double dfyk = MatdD.MainRebarData.B_fy;
    double dfywk = MatdD.SubRebarData.B_fy;
    double dfcki = MatdD.Data1.Design.dTransferFci;
    BOOL   bTran = MatdD.Data1.Design.bTransfer;

    MatlElem.dfcm  = dfck + 8.0;    
    MatlElem.dfctm = Get_fctm_EC2(dfck, MatlElem.dfcm);
    MatlElem.dfctk = 0.7*MatlElem.dfctm;  // fctk;0.05

    int nType=0; // nType : 0=Persistent & Transient, 1=Accidental
    double dfcd_PT=0.0, dfcd_A=0.0;
    double dfyd_PT=0.0, dfyd_A=0.0;
    double dfywd_PT=0.0, dfywd_A=0.0;
    Get_fcd_4EuroCode(iNA, bUser, dAlpha_cc, dGamma_c_PT, dGamma_c_A, dfck, dfcd_PT, dfcd_A);
    Get_fyd_4EuroCode(iNA, bUser, dGamma_s_PT, dGamma_s_A, dfyk, dfywk, dfyd_PT, dfyd_A, dfywd_PT, dfywd_A);

    MatlElem.dfcd_PT = dfcd_PT;
    MatlElem.dfcd_A  = dfcd_A;
    MatlElem.dfyd_PT = dfyd_PT;
    MatlElem.dfyd_A  = dfyd_A;
    MatlElem.dfywd_PT = dfywd_PT;
    MatlElem.dfywd_A  = dfywd_A;
    //
    if ( bTran )
    {
        MatlElem.dfci = dfcki;
    }
    else
    {
        MatlElem.dfci = 0.8*dfck; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 
    }

    // slab
    if ( bPscComposite )
    {
        if ( MatdD.Data2.CodeName==_T("") || MatdD.Data2.CodeName==_T("None") )
        {
            T_CSCS_D CscsD; CscsD.Initialize();
            m_pDataCtrl->Get_Cscs4CPT(aStagK, ElemK, CscsD);
            m_pDataCtrl->Get_SlabElastCscs(ElemK, CscsD, MatdD.Data2.Analysis.Elast);

            if ( fabs(MatdD.Data2.Analysis.Elast)<cDGN_Zero )
            {
                CMatlDB MatlDB(m_pDoc);
                CString strSlabCodeName = MATLCODE_CON_AS17;
                MatdD.Data2.Analysis.Elast    = MatlDB.Get_Ec(strSlabCodeName, MatdD.Data2.CodeMatlName, MatdD.Data2.Design.C_fc);
            }
        }
        // 슬래브
        double dfck_s  = MatdD.Data2.Design.C_fc;
        double dfyk_s  = MatdD.MainRebarData2.B_fy;
        double dfywk_s = MatdD.SubRebarData2.B_fy;

        MatlElem.dfcm_s  = dfck_s + 8.0;
        MatlElem.dfctm_s = Get_fctm_EC2(dfck_s, MatlElem.dfcm_s);        
        MatlElem.dfctk_s = 0.7*MatlElem.dfctm_s;  // fctk;0.05

        double dfcd_PT_s =0.0, dfcd_A_s =0.0;
        double dfyd_PT_s =0.0, dfyd_A_s =0.0;
        double dfywd_PT_s=0.0, dfywd_A_s=0.0;
        Get_fcd_4EuroCode(iNA, bUser, dAlpha_cc, dGamma_c_PT, dGamma_c_A, dfck_s, dfcd_PT_s, dfcd_A_s);
        Get_fyd_4EuroCode(iNA, bUser, dGamma_s_PT, dGamma_s_A, dfyk_s, dfywk_s, dfyd_PT_s, dfyd_A_s, dfywd_PT_s, dfywd_A_s);

        MatlElem.dfcd_PT_s  = dfcd_PT_s;
        MatlElem.dfcd_A_s   = dfcd_A_s;
        MatlElem.dfyd_PT_s  = dfyd_PT_s;
        MatlElem.dfyd_A_s   = dfyd_A_s;
        MatlElem.dfywd_PT_s = dfywd_PT_s;
        MatlElem.dfywd_A_s  = dfywd_A_s;
        //
        MatlElem.dfci_s = 0.8*dfck_s; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscMatlElem_KSCE(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem)
{
    BOOL bPscComposite = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

    int iNA = 0; //PscdD.nNationalAnnex;
    BOOL bUser = PscdD.bUserInputData;
    double dAlpha_cc = 0.85; //PscdD.dAlpha_cc;
    double dGamma_c_PT = PscdD.dGamma_c_PT;
    double dGamma_c_A  = PscdD.dGamma_c_A;
    double dGamma_s_PT = PscdD.dGamma_s_PT;
    double dGamma_s_A  = PscdD.dGamma_s_A;

    double dPhi_c_U = PscdD.dPhi_c[0];  //극한한계
    double dPhi_s_U = PscdD.dPhi_s[0];
    double dPhi_ps_U = PscdD.dPhi_ps[0];
    double dPhi_c_S = PscdD.dPhi_c[2];  //사용한계
    double dPhi_s_S = PscdD.dPhi_s[2];
    double dPhi_ps_S = PscdD.dPhi_ps[2];

    double dfck  = MatdD.Data1.Design.C_fc;
    double dfyk  = MatdD.MainRebarData.B_fy;
    double dfywk = MatdD.SubRebarData.B_fy;

    double dDeltaF = 0.0;
    if ( dfck <40.0 )  dDeltaF = 4.0;
    else if ( dfck >=60.0 ) dDeltaF = 6.0;
    else   dDeltaF = 4.0 + (dfck-40.0)/10.0;

    MatlElem.dfcm  = dfck + dDeltaF;
    MatlElem.dfctm = 0.3*pow(MatlElem.dfcm, 2.0/3.0);
    MatlElem.dfctk = 0.7*MatlElem.dfctm;

    int nType=0; // nType : 0=Persistent & Transient, 1=Accidental
    double dfcd_PT=0.0, dfcd_A=0.0;
    double dfyd_PT=0.0, dfyd_A=0.0;
    double dfywd_PT=0.0, dfywd_A=0.0;
    //       Get_fcd_4EuroCode(iNA, bUser, dAlpha_cc, dGamma_c_PT, dGamma_c_A, dfck, dfcd_PT, dfcd_A);
    //       Get_fyd_4EuroCode(iNA, bUser, dGamma_s_PT, dGamma_s_A, dfyk, dfywk, dfyd_PT, dfyd_A, dfywd_PT, dfywd_A);

    // 
    MatlElem.dfcd_PT = dPhi_c_U*0.85*dfck;
    MatlElem.dfcd_A  = dPhi_c_S*0.85*dfck;
    MatlElem.dfyd_PT = dPhi_s_U*dfyk;
    MatlElem.dfyd_A  = dPhi_s_S*dfyk;
    MatlElem.dfywd_PT = dPhi_s_U*dfyk;
    MatlElem.dfywd_A  = dPhi_s_S*dfyk;
    //
    MatlElem.dfci = 0.8*dfck; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 

    if ( bPscComposite )
    {
        // 슬래브
        double dfck_s  = MatdD.Data2.Design.C_fc;
        double dfyk_s  = MatdD.MainRebarData2.B_fy;
        double dfywk_s = MatdD.SubRebarData2.B_fy;

        double dDeltaF = 0.0;
        if ( dfck_s <40.0 )  dDeltaF = 4.0;
        else if ( dfck_s >=60.0 ) dDeltaF = 6.0;
        else   dDeltaF = 4.0 + (dfck_s-40.0)/10.0;

        MatlElem.dfcm_s  = dfck_s + dDeltaF;
        MatlElem.dfctm_s = 0.3*pow(MatlElem.dfcm_s, 2.0/3.0);
        MatlElem.dfctk_s = 0.7*MatlElem.dfctm_s;

        MatlElem.dfcd_PT_s = dPhi_c_U*0.85*dfck_s;
        MatlElem.dfcd_A_s  = dPhi_c_S*0.85*dfck_s;
        MatlElem.dfyd_PT_s = dPhi_s_U*dfyk_s;
        MatlElem.dfyd_A_s  = dPhi_s_S*dfyk_s;
        MatlElem.dfywd_PT_s = dPhi_s_U*dfyk_s;
        MatlElem.dfywd_A_s  = dPhi_s_S*dfyk_s;
        //
        MatlElem.dfci_s = 0.8*dfck_s; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscMatlElem_RUS(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem)
{
    double dfck = MatdD.Data1.Design.C_fc;
    MatlElem.dfci = 0.8*dfck;

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscMatlElem_BS(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem)
{
    BOOL bUser = FALSE;

    double dGamma_mc      = 1.25; //PscdD.dGamma_c_PT;
    double dGamma_mt_pre  = 1.25;
    double dGamma_mt_post = 1.55;
    double dGamma_ms = 1.0; //PscdD.dGamma_s_PT;

    double dGamma_mc_u = 1.0;
    double dGamma_ms_u = 1.0;
    double dGamma_mv   = 1.0;

    double dfcu = MatdD.Data1.Design.C_fc;
    //             double dfy  = MatdD.MainRebarData.B_fy;
    //             double dfys = MatdD.SubRebarData.B_fy;
    // 
    //             MatlElem.dfcd_PT = dGamma_mc_u==0.0 ? 0.0 : dfcu / dGamma_mc_u;
    //             MatlElem.dfcd_A  = MatlElem.dfcd_PT;
    //             MatlElem.dfyd_PT = dGamma_ms_u==0.0 ? 0.0 : dfy / dGamma_ms_u;
    //             MatlElem.dfyd_A  = MatlElem.dfyd_PT;      
    //             MatlElem.dfywd_PT = dGamma_ms_u==0.0 ? 0.0 : dfy / dGamma_ms_u;
    //             MatlElem.dfywd_A  = MatlElem.dfywd_PT;
    // 
    //             MatlElem.dfcd_c_sls      = dGamma_mc==0.0 ? 0.0 : dfcu / dGamma_mc;
    //             MatlElem.dfcd_t_pre_sls  = dGamma_mt_pre==0.0 ? 0.0 : dfcu / dGamma_mt_pre;
    //             MatlElem.dfcd_t_post_sls = dGamma_mt_post==0.0 ? 0.0 : dfcu / dGamma_mt_post;
    MatlElem.dGamma_mc = dGamma_mc_u;
    MatlElem.dGamma_ms = dGamma_ms_u;
    MatlElem.dGamma_mv = dGamma_mv;

    MatlElem.dfci = 0.8*dfcu; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다.

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscMatlElem_IRS(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem)
{
    BOOL bPscComposite = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

    int iNA = 0;//Not Required
    BOOL bUser = PscdD.bUserInputData;
    double dAlpha_cc = PscdD.dAlpha_cc;
    double dGamma_c_PT = PscdD.dGamma_c_PT;
    double dGamma_c_A  = PscdD.dGamma_c_A;
    double dGamma_s_PT = PscdD.dGamma_s_PT;
    double dGamma_s_A  = PscdD.dGamma_s_A;

    // girder
    double dfck = MatdD.Data1.Design.C_fc;
    double dfyk = MatdD.MainRebarData.B_fy;
    double dfywk = MatdD.SubRebarData.B_fy;

    MatlElem.dfcm  = dfck + 10.0;
    if ( dfck<60.0 ) MatlElem.dfctm = 0.259*pow(dfck, 2.0/3.0);
    else          MatlElem.dfctm = 2.27*log(1.+(MatlElem.dfcm/12.5));
    MatlElem.dfctk = 0.7*MatlElem.dfctm;  // fctk;0.05

    int nType=0; // nType : 0=Persistent & Transient, 1=Accidental
    double dfcd_PT=0.0, dfcd_A=0.0;
    double dfyd_PT=0.0, dfyd_A=0.0;
    double dfywd_PT=0.0, dfywd_A=0.0;
    Get_fcd_4IRC112(iNA, bUser, dAlpha_cc, dGamma_c_PT, dGamma_c_A, dfck, dfcd_PT, dfcd_A);
    Get_fyd_4IRC112(iNA, bUser, dGamma_s_PT, dGamma_s_A, dfyk, dfywk, dfyd_PT, dfyd_A, dfywd_PT, dfywd_A);

    MatlElem.dfcd_PT = 0.67*dfck/1.5;
    MatlElem.dfcd_A  = 0.67*dfck/1.5;
    MatlElem.dfyd_PT = dfyd_PT;
    MatlElem.dfyd_A  = dfyd_A;
    MatlElem.dfywd_PT = dfywd_PT;
    MatlElem.dfywd_A  = dfywd_A;
    //
    MatlElem.dfci = 0.8*dfck; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 

    // slab
    if ( bPscComposite )
    {

        // 슬래브
        double dfck_s  = MatdD.Data2.Design.C_fc;
        double dfyk_s  = MatdD.MainRebarData2.B_fy;
        double dfywk_s = MatdD.SubRebarData2.B_fy;

        MatlElem.dfcm_s  = dfck_s + 10.0;
        if ( dfck_s<60.0 ) MatlElem.dfctm_s = 0.259*pow(dfck, 2.0/3.0);
        else            MatlElem.dfctm_s = 2.27*log(1.+(MatlElem.dfctm_s/12.5));
        MatlElem.dfctk_s = 0.7*MatlElem.dfctm_s;  // fctk;0.05

        double dfcd_PT_s =0.0, dfcd_A_s =0.0;
        double dfyd_PT_s =0.0, dfyd_A_s =0.0;
        double dfywd_PT_s=0.0, dfywd_A_s=0.0;
        Get_fcd_4IRC112(iNA, bUser, dAlpha_cc, dGamma_c_PT, dGamma_c_A, dfck_s, dfcd_PT_s, dfcd_A_s);
        Get_fyd_4IRC112(iNA, bUser, dGamma_s_PT, dGamma_s_A, dfyk_s, dfywk_s, dfyd_PT_s, dfyd_A_s, dfywd_PT_s, dfywd_A_s);

        MatlElem.dfcd_PT_s  = dfcd_PT_s;
        MatlElem.dfcd_A_s   = dfcd_A_s;
        MatlElem.dfyd_PT_s  = dfyd_PT_s;
        MatlElem.dfyd_A_s   = dfyd_A_s;
        MatlElem.dfywd_PT_s = dfywd_PT_s;
        MatlElem.dfywd_A_s  = dfywd_A_s;
        //
        MatlElem.dfci_s = 0.8*dfck_s; // 0.8은 default.. 입력을 받지 않으므로 우선 0.8로 한다. 
    }

    return TRUE;
}


BOOL CDgnPscCommon::Make_PscSectElem(BOOL bSaveComposite)
{
    // Add by ZINU.('06.08.31). Initialize Test File.
    CSectUtil::Init_PrintTestFile();

    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    int iSta = 0;
    int iEnd = (m_bSameConsiderRbar ? 1 : 2);
    int iDgnStatus = m_pDoc->GetDesignStatus();

    for(int n=iSta; n<iEnd; n++)
    {
        // Add by ZINU.('05.09.05).	PSC-Design/Strs(5), PSC-Design/Strn(6).
        m_pDoc->SetDesignStatus(n+5);

        m_amSectElem[n].clear();
        _PSC_SECT_ELEM SectElem;
        for(int i=0; i<m_arElemK.GetSize(); ++i)
        {
            SectElem.Initialize();
            auto ElemK = m_arElemK.GetAt(i);

            BOOL bComposite = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);
            if(bSaveComposite && !bComposite) continue;

            T_RPSC_D RpscD; RpscD.Initialize();
            m_pDoc->m_pAttrCtrl2->GetRpscByConsiderRbarEPairK(ElemK, TRUE, RpscD);

            // 슬래브
            double dBc = 0.0;
            double dtc = 0.0;
            double dHh = 0.0;
            if(bSaveComposite && bComposite) Get_CompositeSlabSection(ElemK, dBc, dtc, dHh);

            for(int j=0; j<2; ++j)
            {
                BOOL bPosiI = (j==0 ? TRUE : FALSE);
                T_SPSC_SECT SpscSect;
                SpscSect.Initialize();
                CString strShape=_T("");
                BOOL bClosedSect=FALSE;
                int  nDgnSectType = -1;
                double dAg=0.0, dAoh=0.0, dPh=0.0, dKt=0.0, dWt=0.0, dTorThk=0.0;
                double dpMomPos[4][2]={0.,0.,0.,0.,0.,0.,0.,0.};
                double dpShrPos[3]={0.,0.,0.}, dpShrThk[3]={0.,0.,0.}, dpQ[3]={0.,0.,0.}, dpFlgThk[2]={0.,0.};
                double dPcp=0.0, dAcp=0.0;
                CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
                CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arInner;
                double dLbarThk=0.0;
                double dbtf=0.0;
                BOOL bDivSect4Tor=FALSE;
                int nDivPart=0;
                double dCutPos[3]={0.,0.,0.};
                double dCutPosFromBottom[3]={0.,0.,0.};
				int nWebSize = 0;
                BOOL bCalcOK = FALSE;                      
                if(bSaveComposite && bComposite) 
                {					
                    bCalcOK = m_pDataCtrl->Get_ConSect_Compo(ElemK, bPosiI, SpscSect, strShape, bClosedSect, nDgnSectType, dAg, dAoh, dPh, dKt, dWt, dpMomPos, dpShrPos, dpShrThk, dpQ, dTorThk, dpFlgThk,
                        dLbarThk, bDivSect4Tor, nDivPart, dCutPos, arOuter, arInner, nWebSize);
                    if(!bCalcOK) continue;
                    m_dzDimComp=SpscSect.dzDim;

                    CSectUtil::Get_Torsion_Pcp_Acp_Compo(bPosiI, RpscD, arOuter, dPcp, dAcp);
                    // Composite PC 또는 General Section은 nDgnSectType를 정하기 어렵기 때문에 여기서 다시 판단
                    BOOL bGenSect = strShape==D_SECT_SHAPE_COMPO_PC || strShape==D_SECT_SHAPE_COMPO_G;
                    if (bGenSect && !bClosedSect)
                    {
                        double dBt = fabs(dpMomPos[0][0]-dpMomPos[1][0]);
                        double dBb = fabs(dpMomPos[3][0]-dpMomPos[2][0]);
                        BOOL bSameBtb    = fabs(dBt-dBb)<cDGN_Zero;
                        BOOL bSameB_ShrT = fabs(dBt-dpShrThk[1])<cDGN_Zero;

                        if (bSameBtb && bSameB_ShrT) nDgnSectType = DGN_CHK_SHAPE_TYPE_SB;
                        else
                        {
                            if (dBt>dpShrThk[1]) nDgnSectType = DGN_CHK_SHAPE_TYPE_T;
                        }
                    }
                }
                else
                {
                    double dzDimComp=m_dzDimComp;
                    bCalcOK = m_pDataCtrl->Get_ConSect(ElemK, bPosiI, dzDimComp, SpscSect, strShape, bClosedSect, nDgnSectType, dAg, dAoh, dPh, dKt, dWt, dpMomPos, dpShrPos, dpShrThk, dpQ, dTorThk, dpFlgThk,
                        dLbarThk, dbtf, bDivSect4Tor, nDivPart, dCutPos, arOuter, arInner, nWebSize);
                    if(!bCalcOK) continue;

                    CSectUtil::Get_Torsion_Pcp_Acp(bPosiI, RpscD, arOuter, dPcp, dAcp);
                }

                // 거더
                double dIxx_part[3]={0.,0.,0.};
                double dAoh_part[3]={0.,0.,0.};
                double dPh_part[3]={0.,0.,0.};
                double dPcp_part[3]={0.,0.,0.};
                double dAcp_part[3]={0.,0.,0.};
                double dyDim_part[3]={0.,0.,0.};
                double dzDim_part[3]={0.,0.,0.};
                // 슬래브
                double dIxx_part_sl=0.0;
                double dAoh_part_sl=0.0;
                double dPh_part_sl=0.0;
                double dPcp_part_sl=0.0;
                double dAcp_part_sl=0.0;
                double dyDim_part_sl=0.0;
                double dzDim_part_sl=0.0;
                double dCutPosFromBottom_sl=0.0;

                if( CDBLib::IsPscCodeLSD(m_iCode) || m_iCode==BS5400_90_PSC || m_iCode==IRC_112_2011_PSC || m_iCode==IRS_PSC  ||
                    m_iCode == BD_44_15_PSC || m_iCode==CS_455_PSC || m_iCode == IRC_112_2020_PSC || m_iCode == TMH07_3_1989)
                {       
                    double dLength = SpscSect.dyDim*1.05; // 전체 길이보다 조금 길면 된다.          
                    if (m_iCode == TMH07_3_1989) bDivSect4Tor = true;//Pinakin Added for Trial
                    if(bSaveComposite && bComposite) 
                    {
                        double dzDimComp=SpscSect.dzDim;
                        T_SPSC_SECT SpscSectTemp; SpscSectTemp.Initialize();
                        if(!CSectUtil::GetPscSectCPR_PreComposite(TRUE, ElemK, bPosiI, FALSE, TRUE, dzDimComp, SpscSectTemp))	return FALSE;

                        double dDelHeight = SpscSect.dzDim - SpscSectTemp.dzDim;

                        // dCutPos는 거더 상부부터 거리임.
                        for(int kk=0; kk<nDivPart; ++kk) 
                        {
                            dCutPosFromBottom[kk] = (SpscSect.dzDim-dDelHeight)-dCutPos[kk];
                        }
                        if(bDivSect4Tor) 
                        {
                            // 거더
                            CArray<T_SPSC_COOR,T_SPSC_COOR> arOuterGr; arOuterGr.Add(arOuter[0]);
                            Get_TorsionSectProperty4OpenShape(nDivPart, dCutPosFromBottom, dLength, dLbarThk, arOuterGr, dIxx_part, dAoh_part, dPh_part, dPcp_part, dAcp_part, dyDim_part, dzDim_part);

                            // 슬래브
                            dCutPosFromBottom_sl=SpscSect.dzDim;
                            CArray<T_SPSC_COOR,T_SPSC_COOR> arOuterSl; arOuterSl.Add(arOuter[1]);
                            Get_TorsionSectProperty4OpenShape_Part(nDivPart, dCutPosFromBottom_sl, dLength, dLbarThk, arOuterSl, dIxx_part_sl, dAoh_part_sl, dPh_part_sl, dPcp_part_sl, dAcp_part_sl, dyDim_part_sl, dzDim_part_sl);
                        }
                    }
                    else
                    {
                        for(int kk=0; kk<nDivPart; ++kk) dCutPosFromBottom[kk] = SpscSect.dzDim-dCutPos[kk];
                        if(bDivSect4Tor) Get_TorsionSectProperty4OpenShape(nDivPart, dCutPosFromBottom, dLength, dLbarThk, arOuter, dIxx_part, dAoh_part, dPh_part, dPcp_part, dAcp_part, dyDim_part, dzDim_part);
                    }

                }
                // Inner Polygon Number.
                if(arInner.GetSize() > cSECT_INN)	{bCalcOK=FALSE; ASSERT(0);}
                // Set Sect (0=I, 1=J).
                if(bCalcOK)
                {
                    SectElem.SectPosi[j].strShape     = strShape;
                    SectElem.SectPosi[j].SpscSect	  = SpscSect;
                    SectElem.SectPosi[j].bClosedSect  = bClosedSect;
                    SectElem.SectPosi[j].nDgnSectType = nDgnSectType;
                    SectElem.SectPosi[j].bCompoSect   = (bSaveComposite && bComposite);
					SectElem.SectPosi[j].bUseAoh      = RpscD.Sbar[j].nAutoUser;
                    SectElem.SectPosi[j].dAg		  = dAg;
                    SectElem.SectPosi[j].dAoh		  = dAoh;
                    SectElem.SectPosi[j].dPh		  = dPh;
                    SectElem.SectPosi[j].dKt		  = dKt;
                    SectElem.SectPosi[j].dWt		  = dWt;
                    SectElem.SectPosi[j].dPcp		  = dPcp;
                    SectElem.SectPosi[j].dAcp		  = dAcp;

                    SectElem.SectPosi[j].dBc		  = dBc;
                    SectElem.SectPosi[j].dtc		  = dtc;
                    SectElem.SectPosi[j].dHh		  = dHh;
                    SectElem.SectPosi[j].dbtf         = dbtf;

                    if( CDBLib::IsPscCodeLSD(m_iCode)|| m_iCode==BS5400_90_PSC || m_iCode==IRC_112_2011_PSC || m_iCode==IRS_PSC ||
                        m_iCode == BD_44_15_PSC || m_iCode==CS_455_PSC || m_iCode == IRC_112_2020_PSC || m_iCode == TMH07_3_1989)
                    {
                        SectElem.SectPosi[j].bDivSect4Tor = bDivSect4Tor;
                        SectElem.SectPosi[j].dLbarThk = dLbarThk;            
                        SectElem.SectPosi[j].nDivPart = nDivPart;
                        for(int kk=0; kk<3; ++kk)
                        {
                            SectElem.SectPosi[j].dCutPos[kk] = dCutPosFromBottom[kk]; // Bottom에서 떨어진 거리.
                            SectElem.SectPosi[j].dIxx_part[kk] = dIxx_part[kk];
                            SectElem.SectPosi[j].dAoh_part[kk] = dAoh_part[kk];
                            SectElem.SectPosi[j].dPh_part[kk]  = dPh_part[kk];
                            SectElem.SectPosi[j].dPcp_part[kk] = dPcp_part[kk];
                            SectElem.SectPosi[j].dAcp_part[kk] = dAcp_part[kk];
                            SectElem.SectPosi[j].dyDim_part[kk] = dyDim_part[kk];
                            SectElem.SectPosi[j].dzDim_part[kk] = dzDim_part[kk];
                        }            
                        if(bSaveComposite && bComposite) 
                        {
                            SectElem.SectPosi[j].dCutPos_Sl   = dCutPosFromBottom_sl;   // Bottom에서 떨어진 거리.
                            SectElem.SectPosi[j].dIxx_part_Sl = dIxx_part_sl;
                            SectElem.SectPosi[j].dAoh_part_Sl = dAoh_part_sl;
                            SectElem.SectPosi[j].dPh_part_Sl  = dPh_part_sl;
                            SectElem.SectPosi[j].dPcp_part_Sl = dPcp_part_sl;
                            SectElem.SectPosi[j].dAcp_part_Sl = dAcp_part_sl;
                        }

                    }

                    for(int m=0; m<4; m++)
                    {
                        SectElem.SectPosi[j].dpMomPos[m][0] = dpMomPos[m][0];
                        SectElem.SectPosi[j].dpMomPos[m][1] = dpMomPos[m][1];
                    }
                    for(int m=0; m<3; m++)
                    {
                        SectElem.SectPosi[j].dpShrPos[m] = dpShrPos[m];
                        SectElem.SectPosi[j].dpShrThk[m] = dpShrThk[m];
                        SectElem.SectPosi[j].dQ[m]       = dpQ[m];
                    }
                    SectElem.SectPosi[j].dpFlgThk[0]	= dpFlgThk[0];	// Top.
                    SectElem.SectPosi[j].dpFlgThk[1]	= dpFlgThk[1];	// Bot.
                    SectElem.SectPosi[j].dTorThk			= dTorThk;
                    SectElem.SectPosi[j].arOuter.Copy(arOuter);				
                    int iInnCount=0;
                    // Add Jaeoh ('05.05.18)
                    SectElem.SectPosi[j].iInnerCellNum = nWebSize-1;
                    for(int m=0; m<arInner.GetSize(); m++)
                    {
                        T_SPSC_COOR_AR* arpInner = arInner[m];
                        SectElem.SectPosi[j].arInner[m].Copy(*arpInner);
                    }
					SectElem.SectPosi[j].nWebSize = nWebSize;
                }
                // Delete because of New Operator.
                for(int k=0; k<arInner.GetSize(); ++k)
                {
                    if(arInner[k])
                    {
                        arInner[k]->RemoveAll();
                        delete arInner[k];
                        arInner[k]=NULL;
                    }
                }
                arInner.RemoveAll();
            }
            // Set Sect.
            if(bSaveComposite && bComposite)  m_amSectElem_Compo[n][ElemK] = SectElem;
            else                              m_amSectElem[n][ElemK] = SectElem;
        }
    }
    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    if(m_bSameConsiderRbar)
    {
        if(bSaveComposite)
        {
            m_amSectElem_Compo[1] = m_amSectElem_Compo[0];
        }
        else
        {
            m_amSectElem[1] = m_amSectElem[0];
        }
    }
    return TRUE;
}

BOOL CDgnPscCommon::Make_PscSecgElem(BOOL bSaveComposite)
{
    m_amSecgElem.clear();
    _PSC_SECG_ELEM SecgElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {
        SecgElem.Initialize();
        auto ElemK = m_arElemK.GetAt(i);

        BOOL bComposite = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);
        if(bSaveComposite && !bComposite) continue;

        // 슬래브
        double dBc = 0.0;
        double dtc = 0.0;
        double dHh = 0.0;
        if(bSaveComposite && bComposite) Get_CompositeSlabSection(ElemK, dBc, dtc, dHh);

        for(int j=0; j<2; ++j)
        {
            BOOL bPosiI = (j==0 ? TRUE : FALSE);
            T_SPSC_SECT SpsgSect;
            SpsgSect.Initialize();

            if(bSaveComposite && bComposite)
            {
                if(!m_pDataCtrl->Get_ConSecg_Compo(ElemK, bPosiI, SpsgSect))	continue;
            }
            else
            {
                if(!m_pDataCtrl->Get_ConSecg(ElemK, bPosiI, SpsgSect))	continue;
            }

            // Set Secg (0=I, 1=J).
            SecgElem.SectPosi[j] = SpsgSect;
        }
        // Set Secg.
        if(bSaveComposite && bComposite) m_amSecgElem_Compo[ElemK] = SecgElem;
        else                             m_amSecgElem[ElemK] = SecgElem;
    }
    return TRUE;
}


BOOL CDgnPscCommon::Make_PscRbarElem()
{
    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    int iSta = 0;
    int iEnd = (m_bSameConsiderRbar ? 1 : 2);
    int iDgnStatus = m_pDoc->GetDesignStatus();

    for(int n=iSta; n<iEnd; n++)
    {
        // Add by ZINU.('05.09.05).	PSC-Design/Strs(5), PSC-Design/Strn(6).
        m_pDoc->SetDesignStatus(n+5);

        m_amRbarElem[n].clear();
        _PSC_RBAR_ELEM RbarElem;
        for(int i=0; i<m_arElemK.GetSize(); ++i)
        {
            RbarElem.Initialize();
            auto ElemK = m_arElemK.GetAt(i);
            for(int j=0; j<2; ++j)
            {
                BOOL bPosiI = (j==0 ? TRUE : FALSE);
                CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
                if(!m_pDataCtrl->Get_ConRbar(ElemK, bPosiI, arRbarPosi))	continue;
                // Set Rbar (0=I, 1=J).
                RbarElem.RbarPosi[j].arRbarPosi.Copy(arRbarPosi);
            }
            // Set Rbar.
            m_amRbarElem[n][ElemK] = RbarElem;
        }
    }
    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    if(m_bSameConsiderRbar)
    {
		m_amRbarElem[1] = m_amRbarElem[0];
    }
    return TRUE;
}

BOOL CDgnPscCommon::Make_PscRshrElem()
{
    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    int iSta = 0;
    int iEnd = (m_bSameConsiderRbar ? 1 : 2);
    int iDgnStatus = m_pDoc->GetDesignStatus();

    for(int n=iSta; n<iEnd; n++)
    {
        // Add by ZINU.('05.09.05).	PSC-Design/Strs(5), PSC-Design/Strn(6).
        m_pDoc->SetDesignStatus(n+5);

        m_amRshrElem[n].clear();
        _PSC_RSHR_ELEM RshrElem;
        for(int i=0; i<m_arElemK.GetSize(); ++i)
        {
            RshrElem.Initialize();
            auto ElemK = m_arElemK.GetAt(i);
            for(int j=0; j<2; ++j)
            {
                BOOL bPosiI = (j==0 ? TRUE : FALSE);
                T_RPSC_SBAR RpscSbar;
                RpscSbar.Initialize();
                double dfdt=0.0, dfdb=0.0, dVd=0.0, dMd=0.0;
                if(!m_pDataCtrl->Get_ConRshr(ElemK, bPosiI, dfdt, dfdb, dVd, dMd, RpscSbar))	continue;
                // Set Rshr (0=I, 1=J).
                RshrElem.RshrPosi[j].RpscSbar	= RpscSbar;
                RshrElem.RshrPosi[j].dfdt			= dfdt;
                RshrElem.RshrPosi[j].dfdb			= dfdb;
                RshrElem.RshrPosi[j].dVd			= dVd;
                RshrElem.RshrPosi[j].dMd			= dMd;
            }
            // Set Rshr.
            m_amRshrElem[n][ElemK] = RshrElem;
        }
    }
    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    if(m_bSameConsiderRbar)
    {
        m_amRshrElem[1] = m_amRshrElem[0];
    }
    return TRUE;
}

BOOL CDgnPscCommon::Make_PscTndnElem(int nDgnType)
{
    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    int iSta = 0;
    int iEnd = (m_bSameConsiderRbar ? 1 : 2);
    int iDgnStatus = m_pDoc->GetDesignStatus();

    T_PSCD_D PscdD;
    PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    int iNA = PscdD.nNationalAnnex;
    BOOL bUser = PscdD.bUserInputData;
    double dGamma_ps_PT = PscdD.dGamma_ps_PT;
    double dGamma_ps_A  = PscdD.dGamma_ps_A;
    int nMatType = 0;  // 0 이면 Character 1이면 Worst
    BOOL bChar = TRUE;
    if ( nDgnType == DGN_TYPE_RATING )
    {
        T_RACD_D RacdD;  RacdD.Initialize();
        m_pDoc->m_pAttrCtrl2->GetDgnRacd(RacdD);
        // 연결 필요..
        bUser = TRUE;
        bChar = TRUE;
        dGamma_ps_PT = 1.0;
        dGamma_ps_A  = 1.0;
    }

    T_ASPM_D AspmD; AspmD.Initialize();
    if ( m_iCode==BD_44_15_PSC || m_iCode==CS_455_PSC )
    {
        m_pDoc->m_pAttrCtrl->GetAspm(AspmD);
    }

    int i=0, j=0, k=0, n=0;
    for ( n=iSta; n<iEnd; n++ )
    {
        // Add by ZINU.('05.09.05).	PSC-Design/Strs(5), PSC-Design/Strn(6).
        m_pDoc->SetDesignStatus(n+5);

        m_amTndnElem[n].clear();
        _PSC_TNDN_ELEM TndnElem;
        for ( i=0; i<m_arElemK.GetSize(); ++i )
        {
            TndnElem.Initialize();
            auto ElemK = m_arElemK.GetAt(i);
            for ( j=0; j<2; ++j )
            {
                BOOL bPosiI = (j==0 ? TRUE : FALSE);
                BOOL bIsGrup = IsTndnGroupCheck(m_iCode);
                double dMuyMax=0.0, dMuyMin=0.0;
                CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
                if ( !m_pDataCtrl->Get_ConTndn(ElemK, bPosiI, bIsGrup, dMuyMax, dMuyMin, arTndnPosi) )	continue;

                int nTndnPosSize = arTndnPosi.GetSize();
                // Only JTG-D62-04.
                if ( m_iCode==JTG_D62_04 || m_iCode==CJJ_11_2011 )
                {
                    int iMaxLoadType=0;
                    int iMaxRelxType=0;
                    for ( k=0; k<nTndnPosSize; ++k )
                    {
                        _DGN_TNDN_CRC &TndnPosi = arTndnPosi.GetAt(k);
                        double dfpk=0.0, dfpdt=0.0, dfpdc=0.0;
                        if ( !Get_MatlTndn(TndnPosi.MatlK, dfpk, dfpdt, dfpdc) )	continue;
                        TndnPosi.dfpk  = dfpk;	// by Code Unit.
                        TndnPosi.dfpdt = dfpdt;	// by Code Unit.
                        TndnPosi.dfpdc = dfpdc;	// by Code Unit.

                        arTndnPosi.SetAt(k, TndnPosi);
                        TndnElem.TndnPosi[j].iMaxLoadType = max(iMaxLoadType, TndnPosi.iLoadType);
                        TndnElem.TndnPosi[j].iMaxRelxType = max(iMaxRelxType, TndnPosi.iRelxType);
                    }
                }
                else if ( m_iCode==EUROCODE2_2_05_PSC )
                {
                    for ( k=0; k<nTndnPosSize; ++k )
                    {
                        _DGN_TNDN_CRC &TndnPosi = arTndnPosi.GetAt(k);
                        double dFp01=0.0, dfp01k=0.0, dfpd_PT=0.0, dfpd_A=0.0;

                        // Modify, Jaeoh. [5/7/2010], Get_MatlTndnData4EC2을 사용하지 않고 fpy를 사용하기로함, 이대근부장님,최성기 과장님 협의 
                        //if(!Get_MatlTndnData4EC2(TndnPosi.MatlK, dFp01,dfp01k))	continue;
                        dfp01k = TndnPosi.dfpy;
                        TndnPosi.dFp01   = dFp01;	// by Code Unit.
                        TndnPosi.dfp01k  = dfp01k;	// by Code Unit.            
                        //
                        Get_fpd_4EuroCode(iNA, bUser, dGamma_ps_PT, dGamma_ps_A, dfp01k, dfpd_PT, dfpd_A);
                        TndnPosi.dfpd_PT = dfpd_PT;	// by Code Unit.
                        TndnPosi.dfpd_A  = dfpd_A;	// by Code Unit.
                        //
                        arTndnPosi.SetAt(k, TndnPosi);

                        //(2010.01.18) Modify by Unsang :: 주석제거함.
                        int iMaxLoadType=0;
                        int iMaxRelxType=0;

                        TndnElem.TndnPosi[j].iMaxLoadType = max(iMaxLoadType, TndnPosi.iLoadType);
                        TndnElem.TndnPosi[j].iMaxRelxType = max(iMaxRelxType, TndnPosi.iRelxType);
                    }
                }
                else if ( m_iCode==ABNT_NBR_6118_2023_PSC)
                {
                    for ( k=0; k<nTndnPosSize; ++k )
                    {
                        _DGN_TNDN_CRC &TndnPosi = arTndnPosi.GetAt(k);
                        double dFp01=0.0, dfp01k=0.0, dfpd_PT=0.0, dfpd_A=0.0;

                        dfp01k = TndnPosi.dfpy;
                        TndnPosi.dFp01   = dFp01;	// by Code Unit.
                        TndnPosi.dfp01k  = dfp01k;	// by Code Unit.            
                        //
                        Get_fpd_4BRA(bUser, PscdD.dGamma_c_N_NBR, PscdD.dGamma_c_SC_NBR, dfp01k, dfpd_PT, dfpd_A);
                        TndnPosi.dfpd_PT = dfpd_PT;	// by Code Unit.
                        TndnPosi.dfpd_A  = dfpd_A;	// by Code Unit.
                        //
                        arTndnPosi.SetAt(k, TndnPosi);
                        
                        int iMaxLoadType=0;
                        int iMaxRelxType=0;

                        TndnElem.TndnPosi[j].iMaxLoadType = max(iMaxLoadType, TndnPosi.iLoadType);
                        TndnElem.TndnPosi[j].iMaxRelxType = max(iMaxRelxType, TndnPosi.iRelxType);
                    }
                }
                else if ( m_iCode==IRC_112_2011_PSC || m_iCode==IRS_PSC || m_iCode == IRC_112_2020_PSC)
                {
                    for ( k=0; k<nTndnPosSize; ++k )
                    {
                        _DGN_TNDN_CRC &TndnPosi = arTndnPosi.GetAt(k);
                        double dFp01=0.0, dfp01k=0.0, dfpd_PT=0.0, dfpd_A=0.0;

                        // Modify, Jaeoh. [5/7/2010], Get_MatlTndnData4EC2을 사용하지 않고 fpy를 사용하기로함, 이대근부장님,최성기 과장님 협의 
                        //if(!Get_MatlTndnData4EC2(TndnPosi.MatlK, dFp01,dfp01k))	continue;
                        dfp01k = TndnPosi.dfpy;
                        TndnPosi.dFp01   = dFp01;	// by Code Unit.
                        TndnPosi.dfp01k  = dfp01k;	// by Code Unit.            
                        //
                        Get_fpd_4IRC112(iNA, bUser, dGamma_ps_PT, dGamma_ps_A, dfp01k, dfpd_PT, dfpd_A);
                        TndnPosi.dfpd_PT = dfpd_PT;	// by Code Unit.
                        TndnPosi.dfpd_A  = dfpd_A;	// by Code Unit.
                        //
                        arTndnPosi.SetAt(k, TndnPosi);

                        //(2010.01.18) Modify by Unsang :: 주석제거함.
                        int iMaxLoadType=0;
                        int iMaxRelxType=0;

                        TndnElem.TndnPosi[j].iMaxLoadType = max(iMaxLoadType, TndnPosi.iLoadType);
                        TndnElem.TndnPosi[j].iMaxRelxType = max(iMaxRelxType, TndnPosi.iRelxType);
                    }
                }
                else if ( m_iCode==KSCE_LSD15_PSC || m_iCode == KDS_24_14_21_2022_PSC )
                {
                    for ( k=0; k<nTndnPosSize; ++k )
                    {
                        _DGN_TNDN_CRC &TndnPosi = arTndnPosi.GetAt(k);
                        double dFp01=0.0, dfp01k=0.0, dfpd_PT=0.0, dfpd_A=0.0;

                        dfp01k = TndnPosi.dfpy;
                        TndnPosi.dFp01   = dFp01;	// by Code Unit.
                        TndnPosi.dfp01k  = dfp01k;	// by Code Unit.            
                        //                        
                        //Get_fpd_4EuroCode(iNA, bUser, dGamma_ps_PT, dGamma_ps_A, dfp01k, dfpd_PT, dfpd_A);
                        TndnPosi.dfpd_PT =  PscdD.dPhi_ps[0]* dfp01k;  //dfpd_PT;	// by Code Unit.
                        TndnPosi.dfpd_A  =  PscdD.dPhi_ps[2]* dfp01k;  //dfpd_A;	// by Code Unit.
                        //
                        arTndnPosi.SetAt(k, TndnPosi);

                        //(2010.01.18) Modify by Unsang :: 주석제거함.
                        int iMaxLoadType=0;
                        int iMaxRelxType=0;

                        TndnElem.TndnPosi[j].iMaxLoadType = max(iMaxLoadType, TndnPosi.iLoadType);
                        TndnElem.TndnPosi[j].iMaxRelxType = max(iMaxRelxType, TndnPosi.iRelxType);
                    }
                }
                else if ( CDBLib::IsPscCodeAASHTO(m_iCode) ||
                    CDBLib::IsPscCodeCSA(m_iCode)    ||
                    m_iCode==AS_5100_5_17_PSC )
                {                       
                    for ( int k=0; k<nTndnPosSize; ++k )
                    {
                        _DGN_TNDN_CRC &TndnD = arTndnPosi[k];
                        UINT TdnaK = TndnD.KeyTdna;
                        _PSC_TNDN_TDNA TndnTdnaD; TndnTdnaD.Initialize();
                        if ( m_amTndnTdna.Lookup(TdnaK, TndnTdnaD) )
                        {
                            TndnD.dLength = TndnTdnaD.dTendonLength;
                            TndnD.iBoundaryNum = TndnTdnaD.iBoundaryNum;

                            arTndnPosi.SetAt(k, TndnD);
                        }
                    }
                }
                else if ( m_iCode==SNiP_20503_84_PSC || m_iCode==SP_35_13330_11_PSC || m_iCode==SNiP_20503_84_PSC_MKS || m_iCode==SP_35_13330_11_PSC_MKS )
                {
                    for ( int k=0; k<nTndnPosSize; ++k )
                    {
                        _DGN_TNDN_CRC &TndnD = arTndnPosi[k];
                        UINT TdnaK = TndnD.KeyTdna;
                        _PSC_TNDN_TDNA TndnTdnaD; TndnTdnaD.Initialize();
                        if ( m_amTndnTdna.Lookup(TdnaK, TndnTdnaD) )
                        {
                            TndnD.dLength = TndnTdnaD.dTendonLength;
                            TndnD.iBoundaryNum = TndnTdnaD.iBoundaryNum;
                            TndnD.dfpd_PT =  TndnTdnaD.dfpd;

                            //arTndnPosi.SetAt(k, TndnD);
                        }
                    }
                }
                else if ( m_iCode==BS5400_90_PSC || m_iCode==BD_44_15_PSC || m_iCode==CS_455_PSC || m_iCode == TMH07_3_1989)
                {
                    double dGamma4Tnd = 0.0;

                    if ( m_iCode==BS5400_90_PSC || m_iCode == TMH07_3_1989)
                    {
                        if (bUser)
                        {
                            dGamma4Tnd = PscdD.dCsTndn;
                        }
                        else
                        {
                            dGamma4Tnd = 1.15;
                        }
                    }
                    else
                    {
                        if ( AspmD.bUserInput )
                        {
                            if ( AspmD.nGamma_m == 0 )
                            {
                                dGamma4Tnd = AspmD.Factors.dCsTndn;
                            }
                            else
                            {
                                dGamma4Tnd = AspmD.Factors.dWsTndn;
                            }
                        }
                        else
                        {
                            if ( AspmD.nGamma_m == 0 )
                            {
                                dGamma4Tnd = 1.15;
                            }
                            else
                            {
                                dGamma4Tnd = 1.1;
                            }
                        }
                    }

                    for ( k=0; k<nTndnPosSize; ++k )
                    {
                        _DGN_TNDN_CRC &TndnPosi = arTndnPosi.GetAt(k);
                        double dfpd = 0.0;
                        Get_fpd_4BS(bChar, bUser, dGamma4Tnd, TndnPosi.dfpu, dfpd);
                        TndnPosi.dfpd_PT = dfpd;
                        TndnPosi.dfpd_A  = dfpd;
                    }
                }
                double dPre_Ap=0.0, dPost_Ap=0.0;
                double dBond_Ap=0.0, dUnbond_Ap=0.0;
                for ( k=0; k<nTndnPosSize; ++k )
                {
                    _DGN_TNDN_CRC &TndnPosi = arTndnPosi.GetAt(k);
                    double dAp = TndnPosi.dArea;
                    if ( TndnPosi.iLoadType==0 )
                    {
                        dPre_Ap += dAp;
                    }
                    else if ( TndnPosi.iLoadType==1 )
                    {
                        dPost_Ap += dAp;
                        if ( TndnPosi.iBondType==0 ) dBond_Ap += dAp; // 0:Bonded, 1:Unbonded
                        else                      dUnbond_Ap += dAp;
                    }
                    else ASSERT(0);
                }
                BOOL bPostTension = dPost_Ap<dPre_Ap ? FALSE : TRUE;
                BOOL bBonded = TRUE;
                if ( !bPostTension ) bBonded = TRUE;
                else              bBonded = dBond_Ap<dUnbond_Ap ? FALSE : TRUE;
                //
                TndnElem.TndnPosi[j].bPost   = bPostTension;
                TndnElem.TndnPosi[j].bBonded = bBonded;

                // Set Tndn (0=I, 1=J).
                TndnElem.TndnPosi[j].arTndnPosi.Copy(arTndnPosi);
            }
            // Set Tndn.
            m_amTndnElem[n][ElemK] = TndnElem;
        }
    }
    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    if ( m_bSameConsiderRbar )
    {
        m_amTndnElem[1] = m_amTndnElem[0];
    }
    return TRUE;
}

BOOL CDgnPscCommon::Make_PscTndnTdna()
{
    m_amTndnTdna.RemoveAll();
    _PSC_TNDN_TDNA TndnTdna;
    int nTdnaKeySize = m_arTdnaK.GetSize();
    for(int i=0; i<nTdnaKeySize; ++i)
    {
        TndnTdna.Initialize();
        T_TDNA_K TdnaK = m_arTdnaK.GetAt(i);
        int iLoadType=0, iRelxType=0;
        double dfpu=0.0, dfpy=0.0;    
        double dTendonLength=0.0;
        int iBoundaryNum=0;
        if(!m_pDataCtrl->Get_ConTndn(TdnaK,iLoadType,iRelxType,dfpu,dfpy, dTendonLength, iBoundaryNum))	{ASSERT(0); continue;}
        // Tendon Profile.
        T_TDNA_D TdnaD;
        TdnaD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK,TdnaD))	{ASSERT(0); continue;}
        // Tendon Property.
        T_TDNT_D TdntD;
        TdntD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdnt(TdnaD.TendonTypeKey,TdntD))	{ASSERT(0); continue;}
        double dfpk=0.0, dfpdt=0.0, dfpdc=0.0;
        // Only JTG-D62-04.
        if(m_iCode==JTG_D62_04 || m_iCode==CJJ_11_2011)
        {
            if(!Get_MatlTndn(TdntD.MatlKey, dfpk,dfpdt,dfpdc))	{ASSERT(0); continue;}
        }
        else if ( CDBLib::IsPscCodeLSD(m_iCode) || m_iCode==IRC_112_2011_PSC || m_iCode==IRS_PSC ||
                  m_iCode == IRC_112_2020_PSC)
        {
            double dFp01=0.0, dfp01k=0.0;
            dfp01k = dfpy;
            // Modify, Jaeoh. [5/7/2010], Get_MatlTndnData4EC2을 사용하지 않고 fpy를 사용하기로함, 이대근부장님,최성기 과장님 협의 
            //if(!Get_MatlTndnData4EC2(TdntD.MatlKey, dFp01,dfp01k))	continue;
            TndnTdna.dFp01   = dFp01;	// by Code Unit.
            TndnTdna.dfp01k  = dfp01k;	// by Code Unit.    
        }
        else if(CDBLib::IsPscCodeAASHTO(m_iCode) || 
            CDBLib::IsPscCodeCSA(m_iCode)    ||
            m_iCode==AS_5100_5_17_PSC)
        {
            // 다음 데이터를 만들자. 
            // tendon이 물고있는 요소를 찾아서 
            // 그 요소사이의 지점을 찾는다. 
            // tendon의 길이는?
            // li = length of tendon between anchorages
            // Ns = number of support hinges crossed by the tendon between anchorages or discretely bonded points
            iBoundaryNum = 0;
 	        auto iterNs = m_pDataCtrl->m_mNs4Td.find(TdnaK);
 	        if (iterNs != m_pDataCtrl->m_mNs4Td.end())
 	        {
                iBoundaryNum = iterNs->second;
 	        }
        }
        else if (m_iCode==SNiP_20503_84_PSC || m_iCode==SP_35_13330_11_PSC ||m_iCode==SNiP_20503_84_PSC_MKS || m_iCode==SP_35_13330_11_PSC_MKS)
        {
            double dfpd = 0.0;
            Get_MatlTndnData4SNiP(m_nBridgeType, TdntD.MatlKey, TdntD.dYieldStrength, dfpd);
            TndnTdna.dfpd  = dfpd;
        }
        else if(m_iCode==BS5400_90_PSC || m_iCode==BD_44_15_PSC || m_iCode==CS_455_PSC || m_iCode == TMH07_3_1989)
        {
            //
        }
        // Set Tdna.
        TndnTdna.iLoadType = iLoadType;
        TndnTdna.iRelxType = iRelxType;
        TndnTdna.dfpu			= dfpu;
        TndnTdna.dfpy			= dfpy;
        TndnTdna.dfpk			= dfpk;
        TndnTdna.dfpdt		    = dfpdt;
        TndnTdna.dfpdc		    = dfpdc;
        TndnTdna.dTendonLength  = dTendonLength;
        TndnTdna.iBoundaryNum   = iBoundaryNum;
        TndnTdna.dTransBeginLen = TdnaD.dNoTensionBeginLen;
        TndnTdna.dTransEndLen   = TdnaD.dNoTensionEndLen; 
        TndnTdna.dDebondedBeginLen = TdnaD.dDebondBeginLen;
        TndnTdna.dDebondedEndLen   = TdnaD.dDebondEndLen;

        m_amTndnTdna.SetAt(TdnaK, TndnTdna);
    }		
    Get_PscTdnaInfo(m_TdnaInfo);
    return TRUE;
}

BOOL CDgnPscCommon::Make_PscTndnTdnt()
{	
    _PSC_TNDN_TDNT TndnTdnt;
    int nTdntSize = m_arTdntK.GetSize();
    if (nTdntSize>0) 
    {
        m_arTndnTdnt.SetSize(nTdntSize);
        m_TndnProp.aTndnProp.SetSize(nTdntSize);
    }
    for(int i=0; i<nTdntSize; ++i)
    {
        TndnTdnt.Initialize();
        T_TDNT_K TdntK = m_arTdntK.GetAt(i);
        int iLoadType=0;
        double dfpu=0.0, dfpy=0.0;
        // Tendon Property.
        T_TDNT_D TdntD;
        TdntD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdnt(TdntK,TdntD))	{ASSERT(0); continue;}
        double dfpk=0.0, dfpdt=0.0, dfpdc=0.0;
        // Only JTG-D62-04.
        if(m_iCode==JTG_D62_04 || m_iCode==CJJ_11_2011)
        {
            if(!Get_MatlTndn(TdntD.MatlKey, dfpk,dfpdt,dfpdc))	{ASSERT(0); continue;}
            iLoadType = TdntD.nLoadType;	// 0=Pre-tension, 1=Post-tension.	    
            dfpu = TdntD.dUltimateStrength;
            dfpy = TdntD.dYieldStrength;

        }
        // Set Tdnt.
        TndnTdnt.TendonTypeName = TdntD.TendonTypeName;
        TndnTdnt.TndnTdna.iLoadType = iLoadType;
        //TndnTdnt.TndnTdna.iRelxType = iRelxType;  // 사용안함.
        TndnTdnt.TndnTdna.dfpu			= dfpu;
        TndnTdnt.TndnTdna.dfpy			= dfpy;
        TndnTdnt.TndnTdna.dfpk			= dfpk;
        TndnTdnt.TndnTdna.dfpdt		  = dfpdt;
        TndnTdnt.TndnTdna.dfpdc		  = dfpdc;
        m_arTndnTdnt.SetAt(i, TndnTdnt);

        _PSC_TNDN_PROP TndnProp;
        TndnProp.strTendonTypeName = TdntD.TendonTypeName;
        TndnProp.nTendonType       = TdntD.nTendonType;
        TndnProp.dTendonArea       = TdntD.dTendonArea;
        TndnProp.dDuctArea         = TdntD.dDuctArea;
        TndnProp.dUltimateStrength = TdntD.dUltimateStrength;
        TndnProp.dYieldStrength    = TdntD.dYieldStrength;
        if(m_iCode==SNiP_20503_84_PSC || m_iCode==SP_35_13330_11_PSC || m_iCode==SNiP_20503_84_PSC_MKS || m_iCode==SP_35_13330_11_PSC_MKS)
        {
            double dfpd = 0.0;
            Get_MatlTndnData4SNiP(m_nBridgeType, TdntD.MatlKey, TdntD.dYieldStrength, dfpd);
            TndnProp.dYieldStrength  = dfpd;
        }

        TndnProp.nLoadType         = TdntD.nLoadType;
        TndnProp.nTypeId           = TdntD.nTypeId;
        TndnProp.nBondType         = TdntD.nBondType;
        TndnProp.nRelaxMethod      = TdntD.nRelaxMethod;
        TndnProp.dRelaxRatio       = TdntD.dRelaxRatio;

        m_TndnProp.aTndnProp.SetAt(i, TndnProp);
    }		
    return TRUE;
}

BOOL CDgnPscCommon::Make_PscXib_JTG()
{
    if(!(m_iCode==JTG_D62_04 || m_iCode==CJJ_11_2011))	{ASSERT(0); return FALSE;}

    m_amXibJTG[0].RemoveAll();
    m_amXibJTG[1].RemoveAll();
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {
        auto ElemK = m_arElemK.GetAt(i);

        // Get Matl Data.
        T_MATD_D MatdD;
        MatdD.Initialize();
        if(!m_pDataCtrl->Get_ConMatd(ElemK, MatdD))	continue;
        if(MatdD.Data1.CodeName!=MATLCODE_CON_JTG04)	continue;

        int iXiBarType=0;	  // 1=R235, 2=HRB335, 3=HRB400/KL400,     
        int iXiTndnType=0;  // 1=Strand/Wire, 2=Steel Bar.
        _PSC_RBAR_ELEM RbarElem;
        RbarElem.Initialize();
        BOOL bExistRbar = Get_PscRbarElem(TRUE, ElemK, RbarElem);
        for(int j=0; j<2; ++j)	// 0=I, 1=J.
        {
            if(bExistRbar)	bExistRbar = (RbarElem.RbarPosi[j].arRbarPosi.GetSize() > 0 ? TRUE : FALSE);
            if(bExistRbar)	// Exist Rbar.
            {
                if(MatdD.MainRebar_RebarName==_T("R235"))		iXiBarType = 1;
                else if(MatdD.MainRebar_RebarName==_T("HRB335"))	iXiBarType = 2;
                else if(MatdD.MainRebar_RebarName==_T("HRB400"))	iXiBarType = 3;
                else if(MatdD.MainRebar_RebarName==_T("KL400"))		iXiBarType = 3;
                else	{ASSERT(0);	continue;}
            }     

            _PSC_TNDN_ELEM TndnElem;
            TndnElem.Initialize();
            BOOL bExistTndn = Get_PscTndnElem(TRUE, ElemK, TndnElem);
            if(bExistTndn)
            {
                if(TndnElem.TndnPosi[j].iMaxLoadType==0)	iXiTndnType = 1;	// Strand.
                else if(TndnElem.TndnPosi[j].iMaxLoadType==1)	iXiTndnType = 1;	// Wire.
                else if(TndnElem.TndnPosi[j].iMaxLoadType==2)	iXiTndnType = 2;	// Steelbar.
                else	ASSERT(0);
            }

            // See JTG-D62-04 Table 5.2.1 (0=I,1=J).
            double dXib=0.0;
            double dXibB=0.0; // Bar
            double dXibT=0.0; // Tendon
            if(iXiBarType==1)	// R235.
            {
                if(MatdD.Data1.Design.C_fc < 50.0+m_dZero)	dXibB = 0.62;
                else if(MatdD.Data1.Design.C_fc < 60.0+m_dZero)	dXibB = 0.60;
                else if(MatdD.Data1.Design.C_fc < 70.0+m_dZero)	dXibB = 0.58;
                else																						dXibB = 0.58;
            }
            else if(iXiBarType==2)	// HRB335.
            {
                if(MatdD.Data1.Design.C_fc < 50.0+m_dZero)	dXibB = 0.56;
                else if(MatdD.Data1.Design.C_fc < 60.0+m_dZero)	dXibB = 0.54;
                else if(MatdD.Data1.Design.C_fc < 70.0+m_dZero)	dXibB = 0.52;
                else																						dXibB = 0.52;
            }
            else if(iXiBarType==3)	// HRB400/KL440.
            {
                if(MatdD.Data1.Design.C_fc < 50.0+m_dZero)	dXibB = 0.53;
                else if(MatdD.Data1.Design.C_fc < 60.0+m_dZero)	dXibB = 0.51;
                else if(MatdD.Data1.Design.C_fc < 70.0+m_dZero)	dXibB = 0.49;
                else																						dXibB = 0.49;
            }
            //else ASSERT(0);

            if(iXiTndnType==1)	// Strand/Wire.
            {
                if(MatdD.Data1.Design.C_fc < 50.0+m_dZero)	dXibT = 0.40;
                else if(MatdD.Data1.Design.C_fc < 60.0+m_dZero)	dXibT = 0.38;
                else if(MatdD.Data1.Design.C_fc < 70.0+m_dZero)	dXibT = 0.36;
                else if(MatdD.Data1.Design.C_fc < 80.0+m_dZero)	dXibT = 0.35;
                else																						dXibT = 0.35;
            }
            else if(iXiTndnType==2)	// Steelbar.
            {
                if(MatdD.Data1.Design.C_fc < 50.0+m_dZero)	dXibT = 0.40;
                else if(MatdD.Data1.Design.C_fc < 60.0+m_dZero)	dXibT = 0.38;
                else if(MatdD.Data1.Design.C_fc < 70.0+m_dZero)	dXibT = 0.36;
                else																						dXibT = 0.36;
            }
            //else	ASSERT(0);			

            if(bExistRbar && bExistTndn)       dXib = min(dXibB, dXibT);
            else if(bExistRbar && !bExistTndn) dXib = dXibB;
            else if(!bExistRbar && bExistTndn) dXib = dXibT;
            else ASSERT(0);
            // Set Xib.
            m_amXibJTG[j].SetAt(ElemK.first, dXib);
        }
    }
    return TRUE;
}

BOOL CDgnPscCommon::Make_PscExpcElem()
{
    m_amExpcElem.clear();
    _PSC_EXPC_ELEM ExpcElem;

    T_PESC_D PescD;

    int i=0,j=0;
    for(i=0; i<m_arElemK.GetSize(); ++i)
    {
        ExpcElem.Initialize();

        PescD.Initialize();
        auto PescK = m_arElemK[i];
        BOOL bPesc = FALSE;
        if (PescK.second == EN_EL_BEAM)
        {
            bPesc = m_pDoc->m_pAttrCtrl->GetPesc(PescK.first, PescD);
        }
        else if (PescK.second == EN_EL_VBEAM)
        {
            bPesc = m_pDoc->m_pAttrCtrl->GetPesv(PescK.first, PescD);
        }
        else ASSERT(0);
        if(bPesc)
        {
            for(j=0; j<2; ++j) // Top,Bottom
            {
                // I
                ExpcElem.ExpcPosi[0].iExposureClass[j] = PescD.iExposureClassI[j];
                // J
                ExpcElem.ExpcPosi[1].iExposureClass[j] = PescD.iExposureClassJ[j];
            }
        }
        // Operation condition factor
        T_OCDF_D OcdfD;
        if (m_pDoc->m_pAttrCtrl2->GetOcdf(m_arElemK[i].first, OcdfD))
        {
            for (j=0; j<2; ++j)
            {
                _PSC_EXPC_POSI &ExpcPosD = ExpcElem.ExpcPosi[j];
                ExpcPosD.bmb6      = OcdfD.bmb6;     
                ExpcPosD.nmb6Type  = OcdfD.nmb6Type; 
                ExpcPosD.dSigma_by = OcdfD.dSigby;
                ExpcPosD.dmb6      = OcdfD.dmb6;     
                ExpcPosD.dmb6_s    = OcdfD.dmb6_s;
                ExpcPosD.bmb10     = OcdfD.bmb10; 

                if (OcdfD.bmb10)
                {
                    if (j==0)
                    {
                        if (OcdfD.nPosmb10==0 || OcdfD.nPosmb10==2)
                        {
                            ExpcPosD.bmb10     = TRUE;
                            ExpcPosD.nmb10Type = OcdfD.nmb10Type;
                            ExpcPosD.dSeamThick= OcdfD.dThick;
                            ExpcPosD.dRbj      = OcdfD.dRbj;
                            ExpcPosD.dmb10     = OcdfD.dmb10;
                        }
                    }
                    else if (j==1)
                    {
                        if (OcdfD.nPosmb10==1 || OcdfD.nPosmb10==2)
                        {
                            ExpcPosD.bmb10     = TRUE;
                            ExpcPosD.nmb10Type = OcdfD.nmb10Type;
                            ExpcPosD.dSeamThick= OcdfD.dThick;
                            ExpcPosD.dRbj      = OcdfD.dRbj;
                            ExpcPosD.dmb10     = OcdfD.dmb10;
                        }
                    }
                }
            }
        }
        m_amExpcElem[PescK] = ExpcElem;
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscPscnElem()
{

    m_amPscnElem.clear();
    _PSC_PSCN_ELEM PscnElem;

    T_PSCN_D PscnD;

    BOOL bCheckLS = TRUE;
    if(m_iCode == BD_44_15_PSC || m_iCode==CS_455_PSC )
    {
        // Assessment는 longitudinal shear 검토 막음 by 곽용기, 이대근
        CString strCS454_LS = CProduct::GetTestEnvValue(_ULS(CS454_LS)); // Yes면 부재력 우드아머 먼저 계산하고 스무딩
        bCheckLS = strCS454_LS.CompareNoCase(_ULS(yes))==0 ? TRUE : FALSE;
    }
    
    int i=0,j=0;
    for(i=0; i<m_arElemK.GetSize(); ++i)
    {
        PscnElem.Initialize();

        PscnD.Initialize();
        auto PscnK = m_arElemK[i];
        BOOL bPscn = FALSE;
        if (PscnK.second == EN_EL_BEAM)
        {
            bPscn = m_pDoc->m_pAttrCtrl2->GetPscn(PscnK.first, PscnD);
        }
        else if (PscnK.second == EN_EL_VBEAM)
        {
            bPscn = m_pDoc->m_pAttrCtrl2->GetPscv(PscnK.first, PscnD);
        }
        else ASSERT(0);
        if(bPscn)
        {
            for(j=0; j<2; ++j) 
            {
                PscnElem.nFaceState[j] = (m_iCode == BS5400_90_PSC || m_iCode == BD_44_15_PSC || m_iCode==CS_455_PSC || m_iCode == TMH07_3_1989) ? PscnD.nFaceState[j]+1 : PscnD.nFaceState[j];
                PscnElem.dAngle[j]     = PscnD.dAngle[j]    ; 
                PscnElem.dAw[j]        = PscnD.dAw[j]       ; 
                PscnElem.dfy[j]        = PscnD.dfy[j]       ; 
				
				PscnElem.dLs[j] = bCheckLS ? PscnD.dLs[j] : 0.0;
				PscnElem.dAe[j] = PscnD.dAe[j];
				PscnElem.ds[j]  = 0.0; // v895에서 고려 안함.
    
				//IRS_SConn Data
				PscnElem.IRS_SConn.nFaceState[j] = PscnD.IRS_SConn.nFaceState[j];

				PscnElem.IRS_SConn.dSL[j]  = PscnD.IRS_SConn.dSL[j] ;
				PscnElem.IRS_SConn.dASL[j] = PscnD.IRS_SConn.dASL[j];
				PscnElem.IRS_SConn.dSt[j]  = PscnD.IRS_SConn.dSt[j] ;
				PscnElem.IRS_SConn.dAe[j]  = PscnD.IRS_SConn.dAe[j] ;
				PscnElem.IRS_SConn.dFck[j] = PscnD.IRS_SConn.dFck[j];
				PscnElem.IRS_SConn.dFy[j]  = PscnD.IRS_SConn.dFy[j] ;
				
            }
        }
        m_amPscnElem[PscnK] = PscnElem;
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscPscnElem_IRS()
{
	m_amPscnElem_IRS.clear();
	_PSC_PSCN_IRS_ELEM PscnElem;

	T_PSCN_IRS_D PscnD;

	int i=0,j=0;
	for(i=0; i<m_arElemK.GetSize(); ++i)
	{
		PscnElem.Initialize();

		PscnD.Initialize();
		auto PscnK = m_arElemK[i];
        BOOL bPscn = FALSE;
        if (PscnK.second == EN_EL_BEAM)
        {
            bPscn = m_pDoc->m_pAttrCtrl2->GetPscn_IRS(PscnK.first, PscnD);
        }
        else if (PscnK.second == EN_EL_VBEAM)
        {
            ASSERT(0);
            bPscn = m_pDoc->m_pAttrCtrl2->GetPscn_IRS(PscnK.first, PscnD);
        }
        else ASSERT(0);
        if(bPscn)
		{
			for(j=0; j<2; ++j) 
			{
				PscnElem.nFaceState[j] = PscnD.nFaceState[j];
				PscnElem.dSL[j]  = PscnD.dSL[j]    ; 
				PscnElem.dASL[j] = PscnD.dASL[j]   ; 
				PscnElem.dSt[j]  = PscnD.dSt[j]    ; 
				PscnElem.dAe[j]  = PscnD.dAe[j]    ; 
				PscnElem.dFck[j] = PscnD.dFck[j]   ; 
				PscnElem.dFy[j]  = PscnD.dFy[j]    ;
			}
		}
		m_amPscnElem_IRS[PscnK] = PscnElem;
	}

	return TRUE;
}

BOOL CDgnPscCommon::Make_PscCinfElem()
{
    switch (m_iCode)
    {
    case BS5400_90_PSC:
	case TMH07_3_1989:
    case BD_44_15_PSC:
    case CS_455_PSC:
    	break;
    default:
        return TRUE;
    }

    m_amCinfElem.clear();

    INT_PTR nElemSize = m_arElemK.GetSize();
    for (INT_PTR i=0; i<nElemSize; ++i)
    {
        T_ASOP_D Asop; Asop.Initialize();
        _PSC_CINF_ELEM Cinf;
        auto ElemK = m_arElemK[i];
        BOOL bAsop = FALSE;
        if (ElemK.second == EN_EL_BEAM)
        {
            bAsop = m_pDoc->m_pAttrCtrl->GetAsop(ElemK.first, Asop);
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            bAsop = m_pDoc->m_pAttrCtrl->GetAsop(ElemK.first, Asop);
        }
        else ASSERT(0);
        if (bAsop)
        {
            Cinf.nClass = Asop.nClass + 1;
        }
        m_amCinfElem[ElemK] = Cinf;
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscSpbfElem()
{
    m_amSpbfElem.clear();

    CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
    m_pDoc->m_pAttrCtrl->GetSpanKeyList(aSpanKeyList);    
        
    for ( INT_PTR i=0; i<aSpanKeyList.GetSize(); ++i )
    {
        T_SPAN_D SpanD; SpanD.Initialize();
        T_SPAN_K SpanK = aSpanKeyList[i];
        m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);

        CArray<double, double> aElemSpan;
        m_pDoc->m_pAttrCtrl->GetSpanLengthByElementLength(SpanD, aElemSpan);
        double dSpanLength = 0.0;
        for ( INT_PTR kk=0; kk<aElemSpan.GetSize(); ++kk )
        {
            dSpanLength += aElemSpan[kk];
        }

        double dLengthEndToElemI = 0.0;
        for ( INT_PTR j=0; j<SpanD.aElemBase.GetSize(); ++j )
        {
            _PSC_SPBF_ELEM SpbfElem;
            const T_SPAN_BASE &SpanB = SpanD.aElemBase[j];
            double dElemLen = m_pDoc->calcLAVElem(SpanB.ElemK);            

            SpbfElem.PosD[0].dLengthFromEnd = dLengthEndToElemI;
            CalcLengthFromEnd(dSpanLength, SpbfElem.PosD[0]);
            dLengthEndToElemI += dElemLen;
            SpbfElem.PosD[1].dLengthFromEnd = dLengthEndToElemI;
            CalcLengthFromEnd(dSpanLength, SpbfElem.PosD[1]);

			ElemPairK EPairK(SpanB.ElemK, EN_EL_BEAM);
            m_amSpbfElem[EPairK] = SpbfElem;
        }
    }

    CArray<T_SPAV_K, T_SPAV_K> aSpavKeyList;
    m_pDoc->m_pAttrCtrl->GetSpavKeyList(aSpavKeyList);    
        
    for ( INT_PTR i=0; i<aSpanKeyList.GetSize(); ++i )
    {
        T_SPAV_D SpavD; SpavD.Initialize();
        T_SPAV_K SpavK = aSpavKeyList[i];
        m_pDoc->m_pAttrCtrl->GetSpav(SpavK, SpavD);

        CArray<double, double> aElemSpan;
        m_pDoc->m_pAttrCtrl->GetSpanLengthByElementLength(SpavD, aElemSpan);
        double dSpanLength = 0.0;
        for ( INT_PTR kk=0; kk<aElemSpan.GetSize(); ++kk )
        {
            dSpanLength += aElemSpan[kk];
        }

        double dLengthEndToElemI = 0.0;
        for ( INT_PTR j=0; j<SpavD.aElemBase.GetSize(); ++j )
        {
            _PSC_SPBF_ELEM SpbfElem;
            const T_SPAN_BASE &SpanB = SpavD.aElemBase[j];
            double dElemLen = m_pDoc->calcLenthVbem(SpanB.ElemK); // T_SPAV_D의 SpanB.ElemK는 virtual key ?

            SpbfElem.PosD[0].dLengthFromEnd = dLengthEndToElemI;
            CalcLengthFromEnd(dSpanLength, SpbfElem.PosD[0]);
            dLengthEndToElemI += dElemLen;
            SpbfElem.PosD[1].dLengthFromEnd = dLengthEndToElemI;
            CalcLengthFromEnd(dSpanLength, SpbfElem.PosD[1]);

			ElemPairK EPairK(SpanB.ElemK, EN_EL_VBEAM);
            m_amSpbfElem[EPairK] = SpbfElem;
        }
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscTdnaDgnInfo()
{
    m_amTndnTran.RemoveAll();
    CArray<T_TDNA_K, T_TDNA_K> arTdnaK;
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);

    for ( INT_PTR i=0; i<arTdnaK.GetSize(); ++i )
    {
        _DGN_TNDN_TRAN TranD;
        if ( m_pDataCtrl->Get_TransferTendonInfo(arTdnaK[i], TranD) )
        {
            m_amTndnTran.SetAt(arTdnaK[i], TranD);
        }
    }

    ;
    UINT nId = 0;
    POSITION pos = m_amTndnTran.GetStartPosition();
    while ( pos )
    {
        T_TDNA_K TKey=0;
        _DGN_TNDN_TRAN TData;
        m_amTndnTran.GetNextAssoc(pos, TKey, TData);
        for ( int i=0; i<2; ++i )
        {
            const _DGN_TNDN_TRAN_INF& TranB = TData.TranB[i];
			for ( INT_PTR jj = 0; jj < TranB.aElemLptToLp.GetSize(); ++jj )
			{
				auto TElemK = TranB.aElemLptToLp[jj];
                m_amElemLp[TElemK] = TElemK;
			}
            ElemPairK ElemK = TranB.ElemLpK;
            m_amElemLp[ElemK] = ElemK;

            UINT nIJ = TranB.bLpI_End ? 0 : 1;
            std::pair<ElemPairK, UINT> prElemPos(ElemK, nIJ);
            
            if ( m_amElemPosLp.find(prElemPos) == m_amElemPosLp.end() )
            {                
                m_amElemPosLp.insert(make_pair(prElemPos, ++nId));
            }
        }
    }    

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscPscsElem()
{
    m_amPscsElem.clear();
    _PSC_PSCS_ELEM PscsElem;

    T_PSCS_D PscsD;

    int i=0,j=0;
    for(i=0; i<m_arElemK.GetSize(); ++i)
    {
        PscsElem.Initialize();

        PscsD.Initialize();
        auto PscsK = m_arElemK[i];
        BOOL bPscs = FALSE;
        if (PscsK.second == EN_EL_BEAM)
        {
            bPscs = m_pDoc->m_pAttrCtrl2->GetPscs(PscsK.first, PscsD);
        }
        else if (PscsK.second == EN_EL_VBEAM)
        {
            bPscs = m_pDoc->m_pAttrCtrl2->GetPscs(PscsK.first, PscsD);
        }
        else ASSERT(0);
        if(bPscs)
        {
            for(j=0; j<2; ++j) 
            {
                PscsElem.nFaceState[j]  = PscsD.nFaceState[j];
                PscsElem.dBvi[j]        = PscsD.dBvi[j]      ; 
                PscsElem.dAvf[j]        = PscsD.dAvf[j]      ; 
                PscsElem.dfy[j]         = PscsD.dfy[j]       ; 
                PscsElem.dAlphaf[j]     = PscsD.dAf[j]       ; 
            }
        }
        m_amPscsElem[PscsK] = PscsElem;
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_PscFackElem()
{
    m_amFackElem.clear(); //
    _PSC_FACK_ELEM FackElem;

    T_FACK_D FackD;

    int i=0,j=0;
    for(i=0; i<m_arElemK.GetSize(); ++i)
    {
        FackElem.Initialize();

        FackD.Initialize();
        auto FackK = m_arElemK[i];
		BOOL bFack = FALSE;
        if (FackK.second == EN_EL_BEAM)
        {
            bFack = m_pDoc->m_pAttrCtrl2->GetFack(FackK.first, FackD);
        }
        else if (FackK.second == EN_EL_VBEAM)
        {
            // virtual ???
            bFack = m_pDoc->m_pAttrCtrl2->GetFack(FackK.first, FackD);
        }
        else ASSERT(0);
        if(bFack)
        {
            for(j=0; j<2; ++j) 
            {
                FackElem.dt0[j] = FackD.dt0[j];
            }
        }
        m_amFackElem[FackK] = FackElem;
    }

    return TRUE;
}


// Composite Plate Girder
BOOL CDgnPscCommon::Make_CpgMatlElem()
{
    m_mapCpgMatlElem.clear();
    CPG_MATL_ELEM MatlElem;
    T_MATD_D MatdD;
    CPG_SECT_ELEM SectElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {
        double dfck=0.0, dfcm=0.0, dfctm=0.0, dEcm=0.0;
        MatlElem.Initialize();		
        auto ElemK = m_arElemK.GetAt(i);
        MatdD.Initialize();
        //!/  
        BOOL bSRCMat = m_pDataCtrl->Get_SrcMatd(ElemK, MatdD);
        BOOL bSTLMat = m_pDataCtrl->Get_StlMatd(ElemK, MatdD);
        if(!bSRCMat && !bSTLMat)	continue;	// SRC, Steel 재료일때도 반영한다. Steel일땐 설계재료대화창에서 수정되어야함.
        if(m_iCivilCode == EUROCODE4_2_04_CSG || m_iCivilCode==IRC22_08_CSG || m_iCivilCode==IRC22_15_CSG) // 여기는 EC4만 적용한다.
        {
            //!/
            // T_MATD_D에서 가져오지 못하는 Data는 CPG_MATL_ELEM에 변수를 추가하고
            //여기서 처리한다. 
            if(!Get_MatlCpgConc(ElemK, dfck, dfcm, dfctm, dEcm))
            {
                CString strErrMsg;   
                strErrMsg.Format(_T("%s element  %d : "), _LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_MATERIAL), ElemK.first);
                GSaveHistoryFormatNF(strErrMsg);
                continue;
            }
            //For IRC22 08 Ecm is changed
            if (m_iCivilCode==IRC22_08_CSG || m_iCivilCode==IRC22_15_CSG)
            {

                dfcm = dfck + 10.0;
                if(dfck<=60.0)
                {
                    dfctm = 0.259*pow(dfck,2./3.);
                }
                else
                {
                    dfctm = 2.12*log(1.0+(dfcm/12.5));
                }
                dEcm = 22.0E+03*pow(dfcm/12.5,0.3);
                //For setting Fy according to material,thickness 
                T_MATL_STEEL SteelMaterial;
                if(m_pDoc->m_pMatlDB->GetSteelData(MatdD.Data1.CodeName, MatdD.Data1.CodeMatlName, SteelMaterial))
                {
                    SectElem.Initialize();
                    auto ElemK = m_arElemK.GetAt(i);

                    T_SECT_D &SectD = SectElem.SectD;    
                    if (!m_pDoc->m_pPostCtrl->GetSectPostTsgrDgn(ElemK,SectD)) continue; 

                    double dtw   = 0.0;          
                    double dtf1  = 0.0;          
                    double dtf2  = 0.0;

                    dtw  = SectD.SectBefore.SectI.Size[1];//bPosI ? SectD.SectBefore.SectI.Size[1]: SectD.SectBefore.SectJ.Size[1];        
                    dtf1 = SectD.SectBefore.SectI.Size[3];//bPosI ? SectD.SectBefore.SectI.Size[3]: SectD.SectBefore.SectJ.Size[3];
                    dtf2 = SectD.SectBefore.SectI.Size[5];//bPosI ? SectD.SectBefore.SectI.Size[5]: SectD.SectBefore.SectJ.Size[5];

                    //Top Flange

                    MatdD.Data1.DesignTopFlange.S_Fu = SteelMaterial.S_Fu;
                    MatdD.Data1.DesignTopFlange.S_Fy1 = GetFySteelSectIRC(SteelMaterial,dtf1);

                    //Bot Flange

                    MatdD.Data1.DesignBotFlange.S_Fu = SteelMaterial.S_Fu;
                    MatdD.Data1.DesignBotFlange.S_Fy1 =GetFySteelSectIRC(SteelMaterial,dtf2);


                    //WEb

                    MatdD.Data1.DesignWeb.S_Fu  = SteelMaterial.S_Fu;
                    MatdD.Data1.DesignWeb.S_Fy1 = GetFySteelSectIRC(SteelMaterial,dtw);

                    MatlElem.MatdD.Data1.Analysis.ShearModulus = 77000;	//MPA

                }
            }
            MatlElem.dfck  = dfck;
            MatlElem.dfcm  = dfcm;
            MatlElem.dfctm = dfctm;
            MatlElem.dEcm  = dEcm;
            //
            CString strMatName = MatdD.Data1.CodeMatlName;
            int iMatClass=0;
            Get_ClassifiedMatl(strMatName,iMatClass);
            MatlElem.iMatClass = iMatClass;

            // Set Matd.
            MatlElem.MatdD = MatdD;		    
            m_mapCpgMatlElem[ElemK] = MatlElem;
        }
        else ASSERT(0);
    }
    return TRUE;
}

BOOL CDgnPscCommon::Get_ClassifiedMatl(CString& strMatName, int& iMatClass)
{
    iMatClass=0;

    if(strMatName==_T("S235")     || strMatName==_T("S275")     || strMatName==_T("S355") || 
        strMatName==_T("S275N/NL") || strMatName==_T("S355N/NL") ||       
        strMatName==_T("S275M/ML") || strMatName==_T("S355M/ML") ||
        strMatName==_T("S235W")    || strMatName==_T("S355W") )
    {
        iMatClass = 1;
    }
    else if(strMatName==_T("S420")     || strMatName==_T("S460")     ||
        strMatName==_T("S420N/NL") || strMatName==_T("S460N/NL") ||
        strMatName==_T("S420M/ML") || strMatName==_T("S460M/ML") )
    {
        iMatClass = 2;
    }
    else
    {
        iMatClass = 3;
    }

    return true;
}

BOOL CDgnPscCommon::Make_CpgSectElem()
{	
    BOOL bPostMode = m_pDoc->IsPostMode();
    if(!bPostMode) return FALSE;

    int i,j,k;
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    m_mapCpgSectElem.clear();
    CPG_SECT_ELEM SectElem;
    for(i=0; i<m_arElemK.GetSize(); ++i)
    {
        SectElem.Initialize();
        auto ElemK = m_arElemK.GetAt(i);

        T_ELEM_D ElemD, ElemPreD;
        ElemD.Initialize();
        ElemPreD.Initialize();
        if (ElemK.second == EN_EL_BEAM)
        {
            BOOL bGetElem = m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD);
            BOOL bGetElemPre = m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemPreD);
            if (!bGetElem || !bGetElemPre)	continue;
        }

        T_SECT_K SectK = ElemD.elpro;
        T_SECT_K SectPreK = ElemPreD.elpro;
        if (!m_pDoc->m_pPostCtrl->GetSectPostTsgrDgn(ElemK, SectElem.SectD)) continue;

        // 해석에 연관되기 때문에 RPSD 가 아닌 RPSC 사용. 
        T_RPSC_D RpsdD; RpsdD.Initialize();
        m_pDoc->m_pAttrCtrl2->GetRpscEPairK(ElemK, RpsdD);
        SectElem.bCracked = RpsdD.bCracked; 

        // Girder 개수가 2개 이상인 경우는 설계대상에서 제외.
        // 2015.01.07_ 이정우GJ 요청으로 막음.
        //     if(SectD.SectAfter.SectI.BuiltUpFlag>=2)
        //     {
        //       ASSERT(0);
        //       continue;
        //     }    

        int nSectType = 0;
        if(SectElem.SectD.nStype == D_SECT_TYPE_TAPERED && SectElem.SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_G) nSectType = D_SECT_TYPE_COMPO_G;
        else if(SectElem.SectD.nStype == D_SECT_TYPE_COMPO_G) nSectType = D_SECT_TYPE_COMPO_G;
        else nSectType = SectElem.SectD.nStype;
        BOOL bTapered = SectElem.SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;

        for(j=0; j<2; ++j)
        {
            BOOL bPosiI = (j==0 ? TRUE : FALSE);             
            T_SECT_SECTBASE_D &SectBaseD = (!bPosiI && bTapered && j==1) ? SectElem.SectD.SectBefore.SectJ : SectElem.SectD.SectBefore.SectI;

            // Compo-G J단 정보는 SectD.SectBefore.SectJ에 , 합성보 J단 slab 정보는 SectD.CmpTapJ 의 size에 있다. 

            // Composite General Section
            if(nSectType==D_SECT_TYPE_COMPO_G)
            {
                SectElem.bGenSect = TRUE;

                T_GLINE_INFO GLineInfoD;
                if(!CSectUtil::Get_GLineInfo4Dgn(SectK, bPosiI, SectBaseD, GLineInfoD)) continue;
                T_GLINE_DGN_D GLineDgnD;
                if(!CSectUtil::Make_LineInfo4Dgn(GLineInfoD, GLineDgnD)) continue;
                SectElem.GLineDgn[j] = GLineDgnD;

                BOOL bExistStfn=FALSE;
                T_STFN_SSM_D StfnD; // Stiffeners
                if(m_pDoc->m_pAttrCtrl2->GetStfnSsm(SectK, StfnD)) 
                {
                    if(StfnD.aStiffener[j].GetSize()>0) bExistStfn=TRUE;
                }
                TCHAR cIJ = bPosiI ? 'I' : 'J';
                T_SECT_STIFFNESS* pAfterStiff = CSectUtil::GetProperStiffPosition(SectElem.SectD, TRUE, 0, cIJ);

                T_SECT_STIFFNESS aSpscSect[3];
                for(k=0; k<3; ++k) aSpscSect[k].Initialize();
                T_GENL_DATA GenLineData;
                if(bExistStfn) Get_GenLineDataFromGLineDgn(GLineDgnD, GenLineData);
                if(!Get_CpgSectProperty(bExistStfn, ElemK, bPosiI, SectBaseD, *pAfterStiff, GenLineData, aSpscSect)) continue;
                SectElem.SectPosi[j].RebarSect  = aSpscSect[0]; 
                SectElem.SectPosi[j].NonConSect = aSpscSect[1]; 
                SectElem.SectPosi[j].PureSect   = aSpscSect[2]; 
                double dYbar_p = aSpscSect[2].Cym;
                double dZbar_p = aSpscSect[2].Czm;
                Get_ConvertPointFromCentroid(dYbar_p, dZbar_p, SectBaseD, SectElem.GLineDgn[j]);
            }
            else
            {
				if (nSectType == D_SECT_TYPE_COMPO_STLG_I ||
					nSectType == D_SECT_TYPE_COMPO_STLG_B ||
					nSectType == D_SECT_TYPE_COMPO_STLG_TUB)
				{
					CSectUtil::GetLongStiffener4Dgn(SectElem.SectD.SectBefore.SectI.SODStiffener);
					CSectUtil::GetLongStiffener4Dgn(SectElem.SectD.SectBefore.SectJ.SODStiffener);
				}
                else
                {
					CSectUtil::GetLongStiffener4Dgn(SectElem.SectD.SectBefore.SectI.Stiffener);
					CSectUtil::GetLongStiffener4Dgn(SectElem.SectD.SectBefore.SectJ.Stiffener);
                }                

				CSectUtil::CalcSectData(SectElem.SectD);

                // 0=Rebar 반영한 환산단면
                // 1=부모멘트경우(콘크리트 무시) 환산단면
                T_SECT_STIFFNESS SpscSect[2]; 
                for(k=0; k<2; ++k) SpscSect[k].Initialize();      

                BOOL bCalcOK = m_pDataCtrl->Get_CpgSect(ElemK, nSectType, bPosiI, SpscSect);

                // Set Sect (0=I, 1=J).
                if(bCalcOK)
                {          
                    SectElem.SectPosi[j].NonConSect = SpscSect[1]; // bCrack !!!
                    double dYbar_temp=0.0, dZbar_temp=0.0;
                    CSectUtil::GetCompoSectStiffnessOfSECT(SectElem.SectD, bPosiI, SectElem.SectPosi[j].PureSect, dYbar_temp, dZbar_temp);
                    // Modification, MQC, 4596 관련 철근 정보는 단면특성에 반영 안하는 것으로. 
                    SectElem.SectPosi[j].RebarSect     = SectElem.SectPosi[j].PureSect;
                    SectElem.SectPosi[j].RbarStiff3n   = SectElem.SectPosi[j].PureSect;  // 수정필요
                    SectElem.SectPosi[j].NonSlabStiff2 = SpscSect[1]; // 수정 필요 
                }        
            }
        }
        // Set Sect.
        m_mapCpgSectElem[ElemK] = SectElem;
    }

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    if(m_mapCpgSectElem.size()==0) return FALSE;

    return TRUE;
}

BOOL CDgnPscCommon::Get_CpgSectProperty(BOOL bStiffener, ElemPairK ElemK, BOOL bPosiI, const T_SECT_SECTBASE_D& SectBase, T_SECT_STIFFNESS& AfterStiff, T_GENL_DATA& GLineData, T_SECT_STIFFNESS* aSpscSect)
{
    int i=0;
    T_SECT_STIFFNESS CompoStiff; CompoStiff.Initialize();
    if(bStiffener) // Stiffener가 있을 경우만 재계산한다.
    {
        // Composite 경우. 
        // Girder + Slab
        // Tapered가 아니여도 J단 값을 Set 해야함.
        T_SECT_D CalcSectD; CalcSectD.Initialize();
        CalcSectD.nStype = D_SECT_TYPE_TAPERED;
        CalcSectD.SectBefore.nStype = D_SECT_TYPE_COMPO_G;
        CalcSectD.SectBefore.SectI = SectBase;
        CalcSectD.SectBefore.SectI.aLine.Copy(GLineData.aLine); // I이지만. aEffLine은 bPosiI에 데이터를 따른다. 

        int iStaIx=0, iEndIx=0;
        int iGenPartSize = CalcSectD.SectBefore.SectI.aGeneralPart.GetSize(); 
        for(i=0; i<iGenPartSize; ++i)
        {
            T_GSEC_PART GsecPartD = SectBase.aGeneralPart[i];
            if(GsecPartD.bPlane)
            {
                //
            }
            else // Line
            {      
                iStaIx = GsecPartD.IdxStart[0]; // 0=Polygon
                iEndIx = GsecPartD.IdxEnd[0];
                int iSize = iEndIx - iStaIx;
                if(iSize!=1) { ASSERT(0); return FALSE; }
                // Effective polygon으로 교체      
                CalcSectD.SectBefore.SectI.aOuterPolygon.SetAt(iStaIx, GLineData.PolyD);      
                GsecPartD.IdxStart[2] = 0;
                GsecPartD.IdxEnd[2] = GLineData.aLine.GetSize();
                CalcSectD.SectBefore.SectI.aGeneralPart.SetAt(i, GsecPartD);
            }

        }
        CalcSectD.SectBefore.SectJ = CalcSectD.SectBefore.SectI;

        m_pDoc->m_pSectDB->CalcSectCompo_G(CalcSectD, FALSE);
        CompoStiff = CalcSectD.SectBefore.SectI.Stiffness;
        aSpscSect[2] = CompoStiff;
    }
    else
    {
        aSpscSect[2] = AfterStiff;
    }

    // 0=Rebar 반영한 환산단면
    // 1=부모멘트경우(콘크리트 무시) 환산단면
    // 2=Pure.    
    T_SECT_INFO_D SectInfoD;
    T_SECT_PROP_D SectPropD;
    double dPosition = bPosiI ? 0.0 : 1.0;
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!CSectUtil::Get_SectInfoData(ElemK, dPosition, SectInfoD, TRUE)) return FALSE;
    }
    else if (ElemK.second == EN_EL_VBEAM)
    {
        ASSERT(0);
        //if (!CSectUtil::Get_VSectInfoData(ElemK.first, dPosition, SectInfoD, TRUE)) return FALSE;
    }
    else ASSERT(0);
    
    double dScaleFactor[7] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; 
    BOOL bScaleFactor=FALSE;

    if(bStiffener)
    {
        SectInfoD.StiffMainD.Stiffness = CompoStiff;
        SectInfoD.StiffMainD.dYbar = CompoStiff.Cym;
        SectInfoD.StiffMainD.dZbar = CompoStiff.Czm;
    }

    CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
	const auto& itMatl = m_mapCpgMatlElem.find(ElemK);    
    if(itMatl == m_mapCpgMatlElem.end())
    {
        CString strErrMsg; 
        strErrMsg.Format(_LS(IDS_DGN_CPG_NO_DGN_MATERIAL), ElemK.first);
        GSaveHistoryFormatNF(strErrMsg);
        return FALSE;
    }

    double dE_bar = MatlElem.MatdD.Data1.Analysis.Elast;
    // Modify, Jaeoh. 이대근 BJ님 요청, 정모멘트 경우 철근 반영 안한다. 
    if(!CSectUtil::Get_GeneralSectProperty(bScaleFactor, SectInfoD, dScaleFactor, SectPropD)) return FALSE;
    aSpscSect[0] = SectPropD.aSectProp[0].StiffPure.Stiffness; // StiffRbar -> StiffPure

    SectInfoD.bCracked = TRUE;
    T_SECT_STIFF_BASE CrackedStiff;
    if(!CSectUtil::Get_GeneralCrackedSectProperty(TRUE, dE_bar, bScaleFactor, SectInfoD, dScaleFactor, CrackedStiff)) return FALSE;
    aSpscSect[1] = CrackedStiff.Stiffness;

    return TRUE;
}

BOOL CDgnPscCommon::Get_EdgeData(T_GSEC_POLYGON& GsecPolyD, CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine)
{
    // Left, Right처럼.
    // Stiffener 는 aLine에 없어야함. 

    int i=0;
    CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&> aVertex;
    aVertex.Copy(GsecPolyD.aVertex);

    int iLineSize = aLine.GetSize();
    int ve1=0, ve2=0; //vertex end
    int iLineIx1=0, iLineIx2=0; // 여러개 Line중 양쪽 끝의 Line index
    double db=0.0; // 단순 더하기로 구하자.

    CMap<int,int,T_GLINE_BASE,T_GLINE_BASE> aVixLine;
    T_GLINE_BASE GLineDgn;
    for(i=0; i<iLineSize; ++i)
    {    
        GLineDgn.Initialize();
        T_GSEC_LINE LineD = aLine[i];
        int v1 = LineD.v1;
        CSectUtil::Set_ConnectedLineToVertex(v1, i, aVixLine);
        int v2 = LineD.v2;
        CSectUtil::Set_ConnectedLineToVertex(v2, i, aVixLine);
    }

    // 양 끝단을 찾기 위해.
    //CSectUtil::Get_VertexIdOfLineEnd(GsecPolyD, aVixLine, ve1, ve2);

    // Stiffener가 있을 경우 끝단에서 부터 그 다음 Stiffener의 vertex정보까지의   

    CArray<T_GSEC_LINE, T_GSEC_LINE&> aPanLine;
    if(aVixLine.Lookup(ve1, GLineDgn))
    {
        int iConnLineSize = GLineDgn.aLineIx.GetSize();

        if(iConnLineSize==1)
        {
        }
        else if(iConnLineSize>1)
        {
            for(i=0; i<iConnLineSize; ++i)
            {
            }
        }
        else
        {
            ASSERT(0);
        }

        int iEndLineIx = GLineDgn.aLineIx[0];
        T_GSEC_LINE LineD = aLine[iEndLineIx];
        aPanLine.Add(LineD);
        int iNextVix=0;
        if     (ve1==LineD.v1) iNextVix = LineD.v2;
        else if(ve1==LineD.v2) iNextVix = LineD.v1;
        else ASSERT(0);    

    }



    for(i=0; i<iLineSize; ++i)
    {
        T_GSEC_LINE LineD = aLine[i];
        int v1 = LineD.v1;
        int v2 = LineD.v2;
        double dbi = CSectUtil::Get_LineLength(GsecPolyD, LineD);
        db += dbi;
        if(i==0)
        {
            ve1 = LineD.v1;
            ve2 = LineD.v2;
            iLineIx1 = 0;
            iLineIx2 = 0;      
        }
        else
        {
            if(ve1==v1)
            {
                ve1 = v2;
                iLineIx1 = i;
            }
            else if(ve1==v2)
            {
                ve1 = v1;
                iLineIx1 = i;
            }
            //
            if(ve2==v1)
            {
                ve2 = v2;
                iLineIx2 = i;
            }
            else if(ve2==v2)
            {
                ve2 = v1;
                iLineIx2 = i;
            }
        }
    }

    // 도심을 입력받고 도심에서 양끝점의 떨어진 거리도 필요함.

    // 유효단면을 만들기 위해서....
    // 1. stiffener의 Line index를 알아야함.
    // 2. stiffener의 위치에서 다음 stiffener또는 양끝단의 Line정보를 알아야함.
    // 3. stiffener의 위치에서 양쪽으로 Rho*b/2만큼의 위치를 찾아서 Line정보를 만들어줌.

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgSecgElem()
{
    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgLstiElem()
{
    // Add by ZINU.('05.09.05). Compare Result with Design (Option).
    int iDgnStatus = m_pDoc->GetDesignStatus();

    // Add by ZINU.('05.09.05).	PSC-Design/Strs(5), PSC-Design/Strn(6).
    //m_pDoc->SetDesignStatus(n+5);

    m_mapCpgLstiElem.clear();
    CPG_LSTI_ELEM LstiElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {
        LstiElem.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        if (ElemK.second != EN_EL_BEAM) continue;

        for(int j=0; j<2; ++j)
        {
            BOOL bPosiI = (j==0 ? TRUE : FALSE);
            CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
            if(!m_pDataCtrl->Get_CpgLsti(ElemK, bPosiI, arLstiPosi))	continue;
            // Set longitudinal stiffener (0=I, 1=J).
            LstiElem.LstiPosi[j].arLstiPosi.Copy(arLstiPosi);
        }
        // Set Rbar.
        m_mapCpgLstiElem[ElemK] = LstiElem;
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgRbarElem()
{
    m_pDoc->SetDesignStatus(7);

    m_mapCpgRbarElem.clear();
    CPG_RBAR_ELEM RbarElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {
        RbarElem.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        for(int j=0; j<2; ++j)
        {
            BOOL bPosiI = (j==0 ? TRUE : FALSE);
            for(int k=0; k<2; ++k) // 0=비균열, 1=균열
            {
                BOOL bCrack = k==0 ? TRUE: FALSE;
                CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
                if(!m_pDataCtrl->Get_CpgRbar(ElemK, bPosiI, bCrack, arRbarPosi))	continue;
                // Set Rbar (0=I, 1=J).
                if(k==0) RbarElem.RbarPosi[j].arRbarPosi.Copy(arRbarPosi);
                else     RbarElem.RbarPosiCR[j].arRbarPosi.Copy(arRbarPosi);
            }
        }
        // Set Rbar.
        m_mapCpgRbarElem[ElemK] = RbarElem;
    }

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgSconElem()
{
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    m_mapCpgSconElem.clear();
    CPG_SCON_ELEM SconElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {
        SconElem.Initialize();
        T_CGSC_D CgscD;
        CgscD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        if (ElemK.second == EN_EL_BEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetCgsc(ElemK.first, CgscD))	continue;
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetCgsc(ElemK.first, CgscD))	continue;
        }
        else ASSERT(0);
        
        // Set Rbar (0=I, 1=J).
        for(int j=0; j<2; ++j)
        {
            SconElem.SconPosi[j].bUseShear = CgscD.Sconn[j].bUseShear; 
            SconElem.SconPosi[j].iNum      = CgscD.Sconn[j].iNum;      
            SconElem.SconPosi[j].dfu       = CgscD.Sconn[j].dfu;       
            SconElem.SconPosi[j].dDia      = CgscD.Sconn[j].dDiameter;      
            SconElem.SconPosi[j].dhsc      = CgscD.Sconn[j].dhsc;      
            SconElem.SconPosi[j].dSt       = CgscD.Sconn[j].dSt;       
            SconElem.SconPosi[j].dSpace    = CgscD.Sconn[j].dSpace;

            SconElem.SconPosi[j].dQr       = CgscD.Sconn[j].dQr      ;
            SconElem.SconPosi[j].dAst      = CgscD.Sconn[j].dAst     ;
            SconElem.SconPosi[j].dPlaneLen = CgscD.Sconn[j].dPlaneLen;

        }

        // Set Rbar.
        m_mapCpgSconElem[ElemK] = SconElem;
    }

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgTrstElem()
{
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    m_mapCpgTrstElem.clear();
    CPG_TRST_ELEM TrstElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {
        TrstElem.Initialize();
        T_CGTS_D CgtsD;
        CgtsD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        if (ElemK.second == EN_EL_BEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetCgts(ElemK.first, CgtsD))	continue;
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetCgts(ElemK.first, CgtsD))	continue;
        }
        else ASSERT(0);
        
        // Set Rbar (0=I, 1=J).
        for(int j=0; j<2; ++j)
        {
            TrstElem.TrstPosi[j].bUseStif           = CgtsD.Tstif[j].bUseStif;          
            TrstElem.TrstPosi[j].bInterRigidStif    = CgtsD.Tstif[j].bInterRigidStif;   
            TrstElem.TrstPosi[j].bInterNonRigidStif = CgtsD.Tstif[j].bInterNonRigidStif; 
            TrstElem.TrstPosi[j].iStifNum           = CgtsD.Tstif[j].iStifNum;           
            TrstElem.TrstPosi[j].dht                = CgtsD.Tstif[j].dht;                
            TrstElem.TrstPosi[j].dt                 = CgtsD.Tstif[j].dt;                 
            TrstElem.TrstPosi[j].dPitch             = CgtsD.Tstif[j].dPitch;            
            TrstElem.TrstPosi[j].dDistRstif         = CgtsD.Tstif[j].dDistRstif;        
            TrstElem.TrstPosi[j].dDistNRstif        = CgtsD.Tstif[j].dDistNRstif;       
        }
        //
        m_mapCpgTrstElem[ElemK] = TrstElem;
    }

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgCgtsSect()
{
    // 	m_mapCpgCgtsSect.RemoveAll();
    // 	T_CGTS_SSM_D CgtsD;
    //   int nElemSize = m_arElemK.GetSize(); 
    // 	for(int i=0; i<nElemSize; ++i)
    // 	{    
    //     CgtsD.Initialize();
    // 		ElemPairK ElemK = m_arElemK.GetAt(i);
    // 		if(!m_pDoc->m_pAttrCtrl->GetCgts(ElemK,CgtsD))	continue;
    // 		// Set Rbar (0=I, 1=J).
    // 		for(int j=0; j<2; ++j)
    //     {
    //       TrstElem.TrstPosi[j].bUseStif           = CgtsD.Tstif[j].bUseStif;          
    //       TrstElem.TrstPosi[j].bInterRigidStif    = CgtsD.Tstif[j].bInterRigidStif;   
    //       TrstElem.TrstPosi[j].bInterNonRigidStif = CgtsD.Tstif[j].bInterNonRigidStif; 
    //       TrstElem.TrstPosi[j].iStifNum           = CgtsD.Tstif[j].iStifNum;           
    //       TrstElem.TrstPosi[j].dht                = CgtsD.Tstif[j].dht;                
    //       TrstElem.TrstPosi[j].dt                 = CgtsD.Tstif[j].dt;                 
    //       TrstElem.TrstPosi[j].dPitch             = CgtsD.Tstif[j].dPitch;            
    //       TrstElem.TrstPosi[j].dDistRstif         = CgtsD.Tstif[j].dDistRstif;        
    //       TrstElem.TrstPosi[j].dDistNRstif        = CgtsD.Tstif[j].dDistNRstif;       
    //     }
    //     //
    // 		m_mapCpgTrstElem.SetAt(ElemK, TrstElem);
    // 	}
    // 
    //   m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgTsesElem()
{
    if (m_iCivilCode == IRC22_15_CSG) return Make_CpgTsesElem_IRC22(); 

    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    m_mapCpgTsesElem.clear();	
    CArray<T_CGTE_K, T_CGTE_K> CgteKeyList;
    m_pDoc->m_pAttrCtrl->GetCgteKeyList(CgteKeyList);
    for(int i=0; i<CgteKeyList.GetSize(); ++i)
    {
        CArray<T_ELEM_K, T_ELEM_K> arElemKey;
        arElemKey.RemoveAll();
        T_NODE_K NodeK = CgteKeyList[i];
        T_CGTE_D CgteD;
        CgteD.Initialize();
        m_pDoc->m_pAttrCtrl->GetCgte(CgteKeyList[i],CgteD);
        m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(NodeK, arElemKey);
        for(int j=0; j<arElemKey.GetSize(); ++j)
        {
            CPG_TSES_ELEM TsesElem;
            TsesElem.Initialize();
            ElemPairK ElemK(0, EN_EL_BEAM);
            if (ElemK.second == EN_EL_BEAM)
            {
				ElemK.first = arElemKey[j];
				ElemK.second = EN_EL_BEAM;
                T_ELEM_D ElemD;
                ElemD.Initialize();
                if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemD)) continue;
                //
                const auto& itTses = m_mapCpgTsesElem.find(ElemK);
                if (itTses != m_mapCpgTsesElem.end())
                {
					TsesElem = itTses->second;
                }
                if (NodeK == ElemD.elnod[0])
                {
                    TsesElem.TsesPosi[0].bUseStif = TRUE;
                    TsesElem.TsesPosi[0].iEndpost = CgteD.iEndpost;
                    TsesElem.TsesPosi[0].dht = CgteD.dht;
                    TsesElem.TsesPosi[0].dt = CgteD.dt;
                    TsesElem.TsesPosi[0].de = CgteD.de;
                }
                else
                {
                    TsesElem.TsesPosi[1].bUseStif = TRUE;
                    TsesElem.TsesPosi[1].iEndpost = CgteD.iEndpost;
                    TsesElem.TsesPosi[1].dht = CgteD.dht;
                    TsesElem.TsesPosi[1].dt = CgteD.dt;
                    TsesElem.TsesPosi[1].de = CgteD.de;
                }
            }
            else if (ElemK.second == EN_EL_VBEAM)
            {
                ASSERT(0);
                //ElemK = arElemKey[j];
            }
			else ASSERT(0);

            m_mapCpgTsesElem[ElemK] = TsesElem;
        }
    }

	// VBEAM 처리 필요...
    //CArray<T_CGTE_K, T_CGTE_K> CgteKeyList;
    //m_pDoc->m_pAttrCtrl->GetCgteKeyList(CgteKeyList);
    //for (int i = 0; i < CgteKeyList.GetSize(); ++i)
    //{
    //}

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgTsesElem_IRC22()
{
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    m_mapCpgTsesElem.clear();	
    CArray<T_CGTE_IRC_K, T_CGTE_IRC_K> CgteKeyList;
    m_pDoc->m_pAttrCtrl->GetCgteKeyList_IRC(CgteKeyList);
    for(int i=0; i<CgteKeyList.GetSize(); ++i)
    {
        CArray<T_ELEM_K, T_ELEM_K> arElemKey;
        arElemKey.RemoveAll();
        T_NODE_K NodeK = CgteKeyList[i];
        T_CGTE_IRC_D CgteD;
        CgteD.Initialize();
        m_pDoc->m_pAttrCtrl->GetCgte_IRC(CgteKeyList[i],CgteD);
        m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(NodeK, arElemKey);
        for(int j=0; j<arElemKey.GetSize(); ++j)
        {
            CPG_TSES_ELEM TsesElem;
            TsesElem.Initialize();
            ElemPairK ElemK(0, EN_EL_BEAM);
            if (ElemK.second == EN_EL_BEAM)
            {
                ElemK.first = arElemKey[j];
				ElemK.second = EN_EL_BEAM;
                T_ELEM_D ElemD;
                ElemD.Initialize();
                if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemD)) continue;
                //
                const auto& itTses = m_mapCpgTsesElem.find(ElemK);
                if (itTses != m_mapCpgTsesElem.end())
                {
                    TsesElem = itTses->second;
                }
                if (NodeK == ElemD.elnod[0])
                {
                    TsesElem.TsesPosi[0].bUseStif = TRUE;
                    TsesElem.TsesPosi[0].iEndpost = CgteD.iEndpost;
                    //         TsesElem.TsesPosi[0].dht      = CgteD.dht;
                    //         TsesElem.TsesPosi[0].dt       = CgteD.dt;
                    //         TsesElem.TsesPosi[0].de       = CgteD.de;
                }
                else
                {
                    TsesElem.TsesPosi[1].bUseStif = TRUE;
                    TsesElem.TsesPosi[1].iEndpost = CgteD.iEndpost;
                    //         TsesElem.TsesPosi[1].dht      = CgteD.dht;
                    //         TsesElem.TsesPosi[1].dt       = CgteD.dt;
                    //         TsesElem.TsesPosi[1].de       = CgteD.de;
                }

            }
            else if (ElemK.second == EN_EL_VBEAM)
            {
                ASSERT(0);
                //ElemK = arElemKey[j];
            }
            else ASSERT(0);

            m_mapCpgTsesElem[ElemK] = TsesElem;
        }
    }

    // VBEAM 처리 필요...
    //CArray<T_CGTE_K, T_CGTE_K> CgteKeyList;
    //m_pDoc->m_pAttrCtrl->GetCgteKeyList(CgteKeyList);
    //for (int i = 0; i < CgteKeyList.GetSize(); ++i)
    //{
    //}

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgBcdtElem()
{
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    m_mapCpgBcdtElem.clear();	
    CPG_BCDT_ELEM BcdtElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {		
        T_CPTF_D CptfD;
        CptfD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        if (ElemK.second == EN_EL_BEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetCptf(ElemK.first, CptfD))	continue;
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            
            if (!m_pDoc->m_pAttrCtrl->GetCptf(ElemK.first, CptfD))	continue;
        }
        else ASSERT(0);
        
        //
        for(int j=0; j<2; ++j)
        {
            BcdtElem.BcdtPosi[j].bUseTfType   = CptfD.TfType[j].bUseTfType;
            BcdtElem.BcdtPosi[j].iLoadAppType = CptfD.TfType[j].iLoadAppType;
            BcdtElem.BcdtPosi[j].da           = CptfD.TfType[j].da;           
            BcdtElem.BcdtPosi[j].dSs          = CptfD.TfType[j].dSs;
            BcdtElem.BcdtPosi[j].dC           = CptfD.TfType[j].dC;
        }
        //
        m_mapCpgBcdtElem[ElemK] = BcdtElem;
    }

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgLtbdElem()
{
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    m_mapCpgLtbdElem.clear();	
    CPG_LTBD_ELEM LtbdElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {		
        T_CGLT_D CgltD;
        CgltD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        if (ElemK.second == EN_EL_BEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetCglt(ElemK.first, CgltD))	continue;
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetCglt(ElemK.first, CgltD))	continue;
        }
        else ASSERT(0);        
        //
        for(int j=0; j<2; ++j)
        {
            LtbdElem.LtbdPosi[j].bUseData = CgltD.LTdata[j].bUseData;
            LtbdElem.LtbdPosi[j].dl       = CgltD.LTdata[j].dl;
            LtbdElem.LtbdPosi[j].dCd      = CgltD.LTdata[j].dCd;           
            LtbdElem.LtbdPosi[j].iAlpha   = CgltD.LTdata[j].iAlpha;
            LtbdElem.LtbdPosi[j].da       = CgltD.LTdata[j].da;
        }
        //
        m_mapCpgLtbdElem[ElemK] = LtbdElem;
    }

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgFrdtElem()
{
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    m_mapCpgFrdtElem.clear();	
    CPG_FRDT_ELEM FrdtElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {		
        T_CGFR_D CgfrD;
        CgfrD.Initialize();
        auto ElemK = m_arElemK.GetAt(i);
        if (ElemK.second == EN_EL_BEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetCgfr(ElemK.first, CgfrD))	continue;
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetCgfr(ElemK.first,CgfrD))	continue;
        }
		else ASSERT(0);
        
        //
        for(int j=0; j<2; ++j)
        {
            FrdtElem.FrdtPosi[j].bUseData  = CgfrD.FRdata[j].bUseData;
            FrdtElem.FrdtPosi[j].dLamda_v2 = CgfrD.FRdata[j].dLamda_v2;
            FrdtElem.FrdtPosi[j].dLamda_v4 = CgfrD.FRdata[j].dLamda_v4;
        }
        //
        m_mapCpgFrdtElem[ElemK] = FrdtElem;
    }

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPscCommon::Make_CpgMembElem()
{
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    m_mapCpgMembElem.clear();	
    CPG_MEMB_ELEM MembElem;
    for(int i=0; i<m_arElemK.GetSize(); ++i)
    {		
        auto ElemK = m_arElemK.GetAt(i);

        for(int j=0; j<2; ++j)
        {
            int iPosi = j==0 ? 1 : 2;
			double dLy=0.0;
			double dLz=0.0;
            double dLu=0.0;
            m_pDataCtrl->Get_RigidLength(ElemK,iPosi, dLy, dLz, dLu);
            //
			MembElem.MembPosi[j].dLy = dLy;
			MembElem.MembPosi[j].dLz = dLz;
            MembElem.MembPosi[j].dLu = dLu;
        }
        //
        m_mapCpgMembElem[ElemK] = MembElem;
    } 

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPscCommon::Get_PscMatlElem(ElemPairK ElemK, _PSC_MATL_ELEM& MatlElem)
{
    MatlElem.Initialize();
	const auto& itMatl = m_amMatlElem.find(ElemK);
    if (itMatl != m_amMatlElem.end())
    {
        MatlElem = itMatl->second;
        return TRUE;
    }    
    
    return TRUE;
}

BOOL CDgnPscCommon::Get_PscSectElem(BOOL bStrn, ElemPairK ElemK, _PSC_SECT_ELEM& SectElem)
{
    SectElem.Initialize();
    int iKind = (bStrn ? 1 : 0);
	const auto& itSect = m_amSectElem[iKind].find(ElemK);
    if (itSect != m_amSectElem[iKind].end())
    {
        SectElem = itSect->second;
        return TRUE;
    }

	return FALSE;
}
BOOL CDgnPscCommon::Get_CompoPscSectElem(BOOL bStrn, ElemPairK ElemK, _PSC_SECT_ELEM& SectElem)
{
    SectElem.Initialize();
    int iKind = (bStrn ? 1 : 0);
    const auto& itSect = m_amSectElem_Compo[iKind].find(ElemK);
    if (itSect != m_amSectElem_Compo[iKind].end())
    {
        SectElem = itSect->second;
        return TRUE;
	}

	return FALSE;
}

BOOL CDgnPscCommon::Get_PscSecgElem(ElemPairK ElemK, _PSC_SECG_ELEM& SecgElem)
{
    SecgElem.Initialize();
    const auto& itSect = m_amSecgElem.find(ElemK);
    if ( itSect != m_amSecgElem.end())
    {
        SecgElem = itSect->second;
        return TRUE;
	}
    
    return FALSE;
}
BOOL CDgnPscCommon::Get_CompoPscSecgElem(ElemPairK ElemK, _PSC_SECG_ELEM& SecgElem)
{
    SecgElem.Initialize();
    const auto& itSect = m_amSecgElem_Compo.find(ElemK);
    if (itSect != m_amSecgElem_Compo.end())
    {
        SecgElem = itSect->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscRbarElem(BOOL bStrn, ElemPairK ElemK, _PSC_RBAR_ELEM& RbarElem)
{
    RbarElem.Initialize();
    int iKind = (bStrn ? 1 : 0);
    const auto& itRbar = m_amRbarElem[iKind].find(ElemK);
    if (itRbar != m_amRbarElem[iKind].end())
    {
        RbarElem = itRbar->second;
		return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscRshrElem(BOOL bStrn, ElemPairK ElemK, _PSC_RSHR_ELEM& RshrElem)
{
    RshrElem.Initialize();
    int iKind = (bStrn ? 1 : 0);
    const auto& itRshr = m_amRshrElem[iKind].find(ElemK);
    if (itRshr != m_amRshrElem[iKind].end())
    {
        RshrElem = itRshr->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscTndnElem(BOOL bStrn, ElemPairK ElemK, _PSC_TNDN_ELEM& TndnElem)
{
    TndnElem.Initialize();
    int iKind = (bStrn ? 1 : 0);
    const auto& itTndn = m_amTndnElem[iKind].find(ElemK);
    if (itTndn != m_amTndnElem[iKind].end())
    {
        TndnElem = itTndn->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscTndnTdna(T_TDNA_K TdnaK, _PSC_TNDN_TDNA& TndnTdna)
{
    TndnTdna.Initialize();
    if(m_amTndnTdna.Lookup(TdnaK,TndnTdna))	return TRUE;
    else	return FALSE;
}

BOOL CDgnPscCommon::Get_Xib_JTG(ElemPairK ElemK, BOOL bPosiI, double& dXib)
{
    dXib=0.0;
    int iPosiNo = (bPosiI==TRUE ? 0 : 1);
    if(m_amXibJTG[iPosiNo].Lookup(ElemK.first,dXib))	return TRUE;
    else	return FALSE;
}

BOOL CDgnPscCommon::Get_PscExpcElem(ElemPairK ElemK, _PSC_EXPC_ELEM& ExpcElem)
{
    ExpcElem.Initialize();
    const auto& itExpc = m_amExpcElem.find(ElemK);
    if (itExpc != m_amExpcElem.end())
    {
        ExpcElem = itExpc->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscPscnElem(ElemPairK ElemK, _PSC_PSCN_ELEM& PscnElem)
{
    PscnElem.Initialize();
    const auto& itPscn = m_amPscnElem.find(ElemK);
    if (itPscn != m_amPscnElem.end())
    {
        PscnElem = itPscn->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscPscnElem_IRS(ElemPairK ElemK, _PSC_PSCN_IRS_ELEM& PscnElem)
{
	PscnElem.Initialize();
    const auto& itPscn = m_amPscnElem_IRS.find(ElemK);
    if (itPscn != m_amPscnElem_IRS.end())
    {
        PscnElem = itPscn->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscPscsElem(ElemPairK ElemK, _PSC_PSCS_ELEM& PscsElem)
{
    PscsElem.Initialize();
    const auto& itPscs = m_amPscsElem.find(ElemK);
    if (itPscs != m_amPscsElem.end())
    {
        PscsElem = itPscs->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscFackElem(ElemPairK ElemK, _PSC_FACK_ELEM& FackElem)
{
    FackElem.Initialize();
    const auto& itFack = m_amFackElem.find(ElemK);
    if (itFack != m_amFackElem.end())
    {
        FackElem = itFack->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscCinfElem(ElemPairK ElemK, _PSC_CINF_ELEM& CinfElem)
{
    CinfElem.Initialize();
    const auto& itCinf = m_amCinfElem.find(ElemK);
    if (itCinf != m_amCinfElem.end())
    {
        CinfElem = itCinf->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscSpbfElem(ElemPairK ElemK, _PSC_SPBF_ELEM& SpbfElem)
{
    SpbfElem.Initialize();
    const auto& itSpbf = m_amSpbfElem.find(ElemK);
    if (itSpbf != m_amSpbfElem.end())
    {
        SpbfElem = itSpbf->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_PscTndnTran(T_TDNA_K TdnaK, _DGN_TNDN_TRAN& TranD)
{
    TranD.Initialize();
    if ( m_amTndnTran.Lookup(TdnaK, TranD) ) return TRUE;
    return FALSE;
}


// BOOL CDgnPscCommon::Get_CheckElemUblmKeyList(CArray<T_ELEM_K,T_ELEM_K> &rData)
// {
//   rData.Copy(m_aChkElemUblmKey);
//   return TRUE;
// }

BOOL CDgnPscCommon::Get_CpgMatlElem(ElemPairK ElemK, CPG_MATL_ELEM& MatlElem) const
{
    MatlElem.Initialize();
    const auto& itMatl = m_mapCpgMatlElem.find(ElemK);
    if (itMatl != m_mapCpgMatlElem.end())
    {
        MatlElem = itMatl->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_CpgSectElem(ElemPairK ElemK, CPG_SECT_ELEM& SectElem) const
{
    SectElem.Initialize();
    const auto& itSect = m_mapCpgSectElem.find(ElemK);
    if (itSect != m_mapCpgSectElem.end())
    {
        SectElem = itSect->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_CpgSecgElem(ElemPairK ElemK, CPG_SECT_ELEM& SecgElem)
{
    return TRUE;
}

BOOL CDgnPscCommon::Get_CpgLstiElem(ElemPairK ElemK, CPG_LSTI_ELEM& LstiElem) const  
{  
    LstiElem.Initialize();
    const auto& itLsti = m_mapCpgLstiElem.find(ElemK);
    if (itLsti != m_mapCpgLstiElem.end())
    {
        LstiElem = itLsti->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_CpgRbarElem(ElemPairK ElemK, CPG_RBAR_ELEM& RbarElem) const  
{  
    RbarElem.Initialize();
    const auto& itRbar = m_mapCpgRbarElem.find(ElemK);
    if (itRbar != m_mapCpgRbarElem.end())
    {
        RbarElem = itRbar->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_CpgSconElem(ElemPairK ElemK, CPG_SCON_ELEM& SconElem) const
{  
    SconElem.Initialize();
    const auto& itScon = m_mapCpgSconElem.find(ElemK);
    if (itScon != m_mapCpgSconElem.end())
    {
        SconElem = itScon->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_CpgTrstElem(ElemPairK ElemK, CPG_TRST_ELEM& TrstElem) const
{
    TrstElem.Initialize();
    const auto& itTrst = m_mapCpgTrstElem.find(ElemK);
    if (itTrst != m_mapCpgTrstElem.end())
    {
        TrstElem = itTrst->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_CpgTsesElem(ElemPairK ElemK, CPG_TSES_ELEM& TsesElem) const
{
    TsesElem.Initialize();
    const auto& itTses = m_mapCpgTsesElem.find(ElemK);
    if (itTses != m_mapCpgTsesElem.end())
    {
        TsesElem = itTses->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_CpgBcdtElem(ElemPairK ElemK, CPG_BCDT_ELEM& BcdtElem) const
{
    BcdtElem.Initialize();
    const auto& itBcdt = m_mapCpgBcdtElem.find(ElemK);
    if (itBcdt != m_mapCpgBcdtElem.end())
    {
        BcdtElem = itBcdt->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_CpgLtbdElem(ElemPairK ElemK, CPG_LTBD_ELEM& LtbdElem) const
{
    LtbdElem.Initialize();
	auto iter = m_mapCpgLtbdElem.find(ElemK);
    if (iter != m_mapCpgLtbdElem.end()) 
    {
        LtbdElem = iter->second;
        return TRUE;
    }
    else
		return FALSE;
}

BOOL CDgnPscCommon::Get_CpgFrdtElem(ElemPairK ElemK, CPG_FRDT_ELEM& FrdtElem) const
{
    FrdtElem.Initialize();
    const auto& itFrdt = m_mapCpgFrdtElem.find(ElemK);
    if (itFrdt != m_mapCpgFrdtElem.end())
    {
        FrdtElem = itFrdt->second;
        return TRUE;
    }

    return FALSE;
}

BOOL CDgnPscCommon::Get_CpgMembElem(ElemPairK ElemK, CPG_MEMB_ELEM& MembElem) const
{
    MembElem.Initialize();
    const auto& itMemb = m_mapCpgMembElem.find(ElemK);
    if (itMemb != m_mapCpgMembElem.end())
    {
        MembElem = itMemb->second;
        return TRUE;
    }

    return FALSE;
}

void CDgnPscCommon::GetPureSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData)
{
	const UINT nPos = GetCheckPositionIndex(bPosI);
	if ( !IsCheckPosition(nPos) ) return;
	
	rData = SectElemD.SectPosi[nPos].PureSect;
}

void CDgnPscCommon::GetNonComSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData)
{
	const UINT nPos = GetCheckPositionIndex(bPosI);
	if ( !IsCheckPosition(nPos) ) return;

	rData = SectElemD.SectPosi[nPos].NonConSect;
}

void CDgnPscCommon::GetRebarSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData) const
{
	const UINT nPos = GetCheckPositionIndex(bPosI);
	if ( !IsCheckPosition(nPos) ) return;

	rData = SectElemD.SectPosi[nPos].RebarSect;
}

BOOL CDgnPscCommon::GetGirderStageInfo(ElemPairK ElemK, CPG_CSIF_D& NCStageD) const
{
    CStageInfo *pStageInfo = m_pDoc->m_pPostCtrl->GetStageInfo();

    CArray<T_STAG_K,T_STAG_K> aStagK;
    pStageInfo->GetResultSavedStagKeyList(aStagK);
    int nStageSize = aStagK.GetSize();
    if(nStageSize==0)	return FALSE;

    ArCSIFK aCsif;

    for(int i=0; i<nStageSize; ++i)
    {
        T_STAG_K StagK = aStagK.GetAt(i);    
        if(!pStageInfo->IsActiveElemInTgtStag(StagK, ElemK)) continue;

        // 0:오류 또는 시공 합성 단면 아님
        // 1:시공 합성 단면 : 합성 전 (Active Stage 이전)
        // 2:시공 합성 단면 : 합성 중 (Active되고 모든 Part가 합성되기 전)
        // 3:시공 합성 단면 : 합성 후 (모든 Part가 합성된 후)
        T_CSCS_D CscsD; CscsD.Initialize();
        int nCompositeType = m_pDoc->m_pAttrCtrl->GetElemSectCompositeInfoDgn(ElemK, StagK, CscsD);
        if(nCompositeType==0 || nCompositeType == 3) continue;

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

	return TRUE;
}

BOOL CDgnPscCommon::MakeConvertCsgMemberForce(BOOL bIsPLCB,
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

    // 이 함수는 지워야 할 것 같다
    DGN_MY_MAX_B MyMaxLcom;
    DGN_MY_MAX_B MyMaxLTerm;
    DGN_MY_MAX_B MyMaxPre;
    m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem, &MembForD, MyMaxLcom);  //Unbraced 내 요소들에 대해서 I, 1/4, 1/2, 3/4, J 구간의 부재력을 가져옴(PartI, Part2, Part3, Part4)
    m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_LT, &MembForD_LTerm, MyMaxLTerm);
    if(bIsPLCB) m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_Pre, &MembForD_Pre, MyMaxPre);

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

    //ConvertCsgCalcUblMForce(0, MembForD, MembLcomD.aUlmFor[0].ForD[1].ForLC);   //MembForD의 Part1의 J단을 Ma로 저장
    //ConvertCsgCalcUblMForce(1, MembForD, MembLcomD.aUlmFor[0].ForD[2].ForLC);   //MembForD의 Part2의 J단을 Mb로 저장
    //ConvertCsgCalcUblMForce(2, MembForD, MembLcomD.aUlmFor[0].ForD[3].ForLC);   //MembForD의 Part3의 J단을 Mc로 저장
    //MembLcomD.aUlmFor[0].dMmax = m_pForcCtrl->GetMaxMyByMembForce(&MembForD);
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

    return TRUE;
}


BOOL CDgnPscCommon::Get_MatlTndn(T_MATL_K MatlK, double& dfpk, double& dfpdt, double& dfpdc)
{
    // Always Code Units (N,mm).
    dfpk =0.0;
    dfpdt=0.0;
    dfpdc=0.0;
    ////////////////////////////
    // Get Matl Data.
    T_MATD_D MatdD;
    MatdD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK,MatdD))	{ASSERT(0); return FALSE;}
    if(!(MatdD.Data1.CodeName == MATLCODE_STL_JTG04 || MatdD.Data1.CodeName== MATLCODE_STL_TB05))	{ASSERT(0); return FALSE;}
    // See JTG D62-04 Table 3.2.4-2 fpd,f'pd(프리스트레스철근 인장,압축설계값).
    if(MatdD.Data1.CodeName == MATLCODE_STL_JTG04)
    {
        if(MatdD.Data1.CodeMatlName==_T("Strand1470"))		{dfpk=1470.;  dfpdt=1000.;  dfpdc=390.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1570"))		{dfpk=1570.;  dfpdt=1070.;  dfpdc=390.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1720"))		{dfpk=1720.;  dfpdt=1170.;  dfpdc=390.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1860"))		{dfpk=1860.;  dfpdt=1260.;  dfpdc=390.;}
        else if(MatdD.Data1.CodeMatlName==_T("Wire1470"))			{dfpk=1470.;  dfpdt=1000.;  dfpdc=410.;}
        else if(MatdD.Data1.CodeMatlName==_T("Wire1570"))			{dfpk=1570.;  dfpdt=1070.;  dfpdc=410.;}
        else if(MatdD.Data1.CodeMatlName==_T("Wire1670"))			{dfpk=1670.;  dfpdt=1140.;  dfpdc=410.;}
        else if(MatdD.Data1.CodeMatlName==_T("Wire1770"))			{dfpk=1770.;  dfpdt=1200.;  dfpdc=410.;}
        else if(MatdD.Data1.CodeMatlName==_T("Steelbar540"))	{dfpk= 540.;  dfpdt= 450.;  dfpdc=400.;}
        else if(MatdD.Data1.CodeMatlName==_T("Steelbar785"))	{dfpk= 785.;  dfpdt= 650.;  dfpdc=400.;}
        else if(MatdD.Data1.CodeMatlName==_T("Steelbar930"))	{dfpk= 930.;  dfpdt= 770.;  dfpdc=400.;}
        else	{ASSERT(0);	return FALSE;}
    }
    else if(MatdD.Data1.CodeName== MATLCODE_STL_TB05)
    {
        if(MatdD.Data1.CodeMatlName==_T("PSB830"))		  {dfpk= 830.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Wire1470"))		{dfpk=1470.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Wire1570"))		{dfpk=1570.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Wire1670"))		{dfpk=1670.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Wire1770"))		{dfpk=1770.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Wire1860"))		{dfpk=1860.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1470"))	{dfpk=1470.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1570"))	{dfpk=1570.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1670"))	{dfpk=1670.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1720"))	{dfpk=1720.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1770"))	{dfpk=1770.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1820"))	{dfpk=1820.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else if(MatdD.Data1.CodeMatlName==_T("Strand1860"))	{dfpk=1860.;  dfpdt=0.9*dfpk;  dfpdc=380.;}
        else	{ASSERT(0);	return FALSE;}
    }

    return TRUE;
}

BOOL CDgnPscCommon::Get_MatlTndnData4EC2(T_MATL_K MatlK, double& dFp01, double& dfp01k)
{
    // Get Matl Data.
    T_MATD_D MatdD;
    MatdD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK,MatdD))	{return FALSE;}
    if(!(MatdD.Data1.CodeName== MATLCODE_STL_EN05 || MatdD.Data1.CodeName== MATLCODE_STL_EN05_PS || MatdD.Data1.CodeName== MATLCODE_STL_EN05_SW))	{ASSERT(0); return FALSE;}

    CString strMatlName = MatdD.Data1.CodeMatlName;
    // dFp01 : Characteristic value of 0.1% proof force
    double dSn=0.0; // Cross sectional area
    if(strMatlName==_T("Y1670S7(15.2mm)"))		{dSn=139.0;  dFp01=204.0;}
    else if(strMatlName.Find(_T("Y1770S7"))>=0)
    {
        if(strMatlName==_T("Y1770S7(9.3mm)"))		{dSn= 52.0;  dFp01= 81.0;}
        else if(strMatlName==_T("Y1770S7(9.6mm)"))		{dSn= 55.0;  dFp01= 85.7;}
        else if(strMatlName==_T("Y1770S7(11.0mm)"))		{dSn= 70.0;  dFp01=109.0;}
        else if(strMatlName==_T("Y1770S7(12.5mm)"))		{dSn= 93.0;  dFp01=145.0;}
        else if(strMatlName==_T("Y1770S7(12.9mm)"))		{dSn=100.0;  dFp01=156.0;}
        else if(strMatlName==_T("Y1770S7(15.2mm)"))		{dSn=139.0;  dFp01=216.0;}
        else if(strMatlName==_T("Y1770S7(15.3mm)"))		{dSn=140.0;  dFp01=218.0;}
        else if(strMatlName==_T("Y1770S7(15.7mm)"))		{dSn=150.0;  dFp01=234.0;}
    }
    else if(strMatlName.Find(_T("Y1860S7"))>=0)
    {
        if(strMatlName==_T("Y1860S7(9.3mm)"))		{dSn= 52.0;  dFp01= 85.1;}
        else if(strMatlName==_T("Y1860S7(9.6mm)"))		{dSn= 55.0;  dFp01= 89.8;}
        else if(strMatlName==_T("Y1860S7(11.0mm)"))		{dSn= 70.0;  dFp01=114.0;}
        else if(strMatlName==_T("Y1860S7(11.3mm)"))		{dSn= 75.0;  dFp01=123.0;}
        else if(strMatlName==_T("Y1860S7(12.5mm)"))		{dSn= 93.0;  dFp01=152.0;}
        else if(strMatlName==_T("Y1860S7(12.9mm)"))		{dSn=100.0;  dFp01=164.0;}
        else if(strMatlName==_T("Y1860S7(13.0mm)"))		{dSn=102.0;  dFp01=167.0;}
        else if(strMatlName==_T("Y1860S7(15.2mm)"))		{dSn=139.0;  dFp01=228.0;}
        else if(strMatlName==_T("Y1860S7(15.3mm)"))		{dSn=140.0;  dFp01=229.0;}
        else if(strMatlName==_T("Y1860S7(15.7mm)"))		{dSn=150.0;  dFp01=246.0;}
    }
    else	{ASSERT(0);	return FALSE;}

    dFp01 *= 1000.0; // dFp01는 kN단위 이므로.
    dfp01k = dSn==0.0 ? 0.0 : dFp01/dSn;

    return TRUE;
}

BOOL CDgnPscCommon::Get_MatlTndnData4SNiP(int nBridgeType, T_MATL_K MatlK, double dfpy, double& dfpd)
{
    T_MATD_D MatdD;
    MatdD.Initialize();
    if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK,MatdD))	{return FALSE;}
    if (MatdD.Data1.CodeName== MATLCODE_STL_GOST_SP)
    {
        CString strMatlName = MatdD.Data1.CodeMatlName;
        double dfpy   = MatdD.Data1.Design.S_Fy1;
        double dfpd_R = dfpy;
        double dfpd_H = dfpy;

        BOOL   bOK = FALSE;
        if      (strMatlName==_T("A600")   )  { bOK = TRUE, dfpd_R=435.0,  dfpd_H=465.0; }
        else if (strMatlName==_T("A800")   )  { bOK = TRUE, dfpd_R=565.0,  dfpd_H=600.0; }
        else if (strMatlName==_T("AT600")  )  { bOK = TRUE, dfpd_R=465.0,  dfpd_H=465.0; }
        else if (strMatlName==_T("AT800")  )  { bOK = TRUE, dfpd_R=645.0,  dfpd_H=645.0; }
        else if (strMatlName==_T("AT1000") )  { bOK = TRUE, dfpd_R=775.0,  dfpd_H=775.0; }
        else if (strMatlName==_T("V1500")  )  { bOK = TRUE, dfpd_R=1120.0, dfpd_H=1180.0;}
        else if (strMatlName==_T("V1400")  )  { bOK = TRUE, dfpd_R=1000.0, dfpd_H=1055.0;}
        else if (strMatlName==_T("V1400 (Group S)")  )   { bOK = TRUE, dfpd_R=1000.0*0.94, dfpd_H=1055.0*0.94;} // SNiP 3.45
        else if (strMatlName==_T("V1400 (Group Zh)")  )  { bOK = TRUE, dfpd_R=1000.0*0.88, dfpd_H=1055.0*0.88;}
        else if (strMatlName==_T("V1300")  )  { bOK = TRUE, dfpd_R=940.0,  dfpd_H=995.0; }
        else if (strMatlName==_T("V1200")  )  { bOK = TRUE, dfpd_R=885.0,  dfpd_H=930.0; }
        else if (strMatlName==_T("Vp1500") )  { bOK = TRUE, dfpd_R=1100.0, dfpd_H=1155.0;}
        else if (strMatlName==_T("Vp1400") )  { bOK = TRUE, dfpd_R=940.0,  dfpd_H=995.0; }
        else if (strMatlName==_T("Vp1200") )  { bOK = TRUE, dfpd_R=885.0,  dfpd_H=930.0; }
        else if (strMatlName==_T("K7-1500"))  { bOK = TRUE, dfpd_R=1000.0, dfpd_H=1055.0;}
        else if (strMatlName==_T("K7-1400"))  { bOK = TRUE, dfpd_R=970.0,  dfpd_H=1025.0;}

        if (bOK == FALSE)
        {
            if      (fabs(dfpy-590.0 )<m_dZero )  { dfpd_R=435.0,  dfpd_H=465.0; }
            else if (fabs(dfpy-785.0 )<m_dZero )  { dfpd_R=565.0,  dfpd_H=600.0; }
            //else if (fabs(dfpy-590.0 )<m_dZero )  { dfpd_R=465.0,  dfpd_H=465.0; }
            //else if (fabs(dfpy-785.0 )<m_dZero )  { dfpd_R=645.0,  dfpd_H=645.0; }
            else if (fabs(dfpy-980.0 )<m_dZero )  { dfpd_R=775.0,  dfpd_H=775.0; }
            else if (fabs(dfpy-1490.0)<m_dZero )  { dfpd_R=1120.0, dfpd_H=1180.0;}
            else if (fabs(dfpy-1335.0)<m_dZero )  { dfpd_R=1000.0, dfpd_H=1055.0;}
            else if (fabs(dfpy-1255.0)<m_dZero )  { dfpd_R=940.0,  dfpd_H=995.0; }
            else if (fabs(dfpy-1175.0)<m_dZero )  { dfpd_R=885.0,  dfpd_H=930.0; }
            else if (fabs(dfpy-1460.0)<m_dZero )  { dfpd_R=1100.0, dfpd_H=1155.0;}
            //else if (fabs(dfpy-1255.0)<m_dZero )  { dfpd_R=940.0,  dfpd_H=995.0; }
            //else if (fabs(dfpy-1175.0)<m_dZero )  { dfpd_R=885.0,  dfpd_H=930.0; }
            //else if (fabs(dfpy-1335.0)<m_dZero )  { dfpd_R=1000.0, dfpd_H=1055.0;}
            else if (fabs(dfpy-1295.0)<m_dZero )  { dfpd_R=970.0,  dfpd_H=1025.0;}
        }
        dfpd = (nBridgeType==0) ? dfpd_H : dfpd_R;	
    }  
    else if (MatdD.Data1.CodeName== MATLCODE_STL_GOST_SNIP)
    {
        CString strMatlName = MatdD.Data1.CodeMatlName;
        double dfpy   = MatdD.Data1.Design.S_Fy1;
        double dfpd_R = dfpy;
        double dfpd_H = dfpy;

        BOOL   bOK = FALSE;
        if      (strMatlName==_T("A-IV")       )  { bOK = TRUE, dfpd_R=435.0,  dfpd_H=465.0; }
        else if (strMatlName==_T("A-V")        )  { bOK = TRUE, dfpd_R=565.0,  dfpd_H=600.0; }
        else if (strMatlName==_T("AT-IV")      )  { bOK = TRUE, dfpd_R=465.0,  dfpd_H=465.0; }
        else if (strMatlName==_T("AT-V")       )  { bOK = TRUE, dfpd_R=645.0,  dfpd_H=645.0; }
        else if (strMatlName==_T("AT-VI")      )  { bOK = TRUE, dfpd_R=775.0,  dfpd_H=775.0; }
        else if (strMatlName==_T("V-II 1500")  )  { bOK = TRUE, dfpd_R=1120.0, dfpd_H=1180.0;}
        else if (strMatlName==_T("V-II 1400")  )  { bOK = TRUE, dfpd_R=1000.0, dfpd_H=1055.0;}
        else if (strMatlName==_T("V-II 1400 (Group S)")  )   { bOK = TRUE, dfpd_R=1000.0*0.94, dfpd_H=1055.0*0.94;}
        else if (strMatlName==_T("V-II 1400 (Group Zh)")  )  { bOK = TRUE, dfpd_R=1000.0*0.88, dfpd_H=1055.0*0.88;}
        else if (strMatlName==_T("V-II 1300")  )  { bOK = TRUE, dfpd_R=940.0,  dfpd_H=995.0; }
        else if (strMatlName==_T("V-II 1200")  )  { bOK = TRUE, dfpd_R=885.0,  dfpd_H=930.0; }
        else if (strMatlName==_T("Vr-II 1500") )  { bOK = TRUE, dfpd_R=1100.0, dfpd_H=1155.0;}
        else if (strMatlName==_T("Vr-II 1400") )  { bOK = TRUE, dfpd_R=940.0,  dfpd_H=995.0; }
        else if (strMatlName==_T("Vr-II 1200") )  { bOK = TRUE, dfpd_R=885.0,  dfpd_H=930.0; }
        else if (strMatlName==_T("K7-1500")    )  { bOK = TRUE, dfpd_R=1000.0, dfpd_H=1055.0;}
        else if (strMatlName==_T("K7-1400")    )  { bOK = TRUE, dfpd_R=970.0,  dfpd_H=1025.0;}

        if (bOK == FALSE)
        {
            if      (fabs(dfpy-590.0 )<m_dZero )  { dfpd_R=435.0,  dfpd_H=465.0; }
            else if (fabs(dfpy-785.0 )<m_dZero )  { dfpd_R=565.0,  dfpd_H=600.0; }
            //else if (fabs(dfpy-590.0 )<m_dZero )  { dfpd_R=465.0,  dfpd_H=465.0; }
            //else if (fabs(dfpy-785.0 )<m_dZero )  { dfpd_R=645.0,  dfpd_H=645.0; }
            else if (fabs(dfpy-980.0 )<m_dZero )  { dfpd_R=775.0,  dfpd_H=775.0; }
            else if (fabs(dfpy-1490.0)<m_dZero )  { dfpd_R=1120.0, dfpd_H=1180.0;}
            else if (fabs(dfpy-1335.0)<m_dZero )  { dfpd_R=1000.0, dfpd_H=1055.0;}
            else if (fabs(dfpy-1255.0)<m_dZero )  { dfpd_R=940.0,  dfpd_H=995.0; }
            else if (fabs(dfpy-1175.0)<m_dZero )  { dfpd_R=885.0,  dfpd_H=930.0; }
            else if (fabs(dfpy-1460.0)<m_dZero )  { dfpd_R=1100.0, dfpd_H=1155.0;}
            //else if (fabs(dfpy-1255.0)<m_dZero )  { dfpd_R=940.0,  dfpd_H=995.0; }
            //else if (fabs(dfpy-1175.0)<m_dZero )  { dfpd_R=885.0,  dfpd_H=930.0; }
            //else if (fabs(dfpy-1335.0)<m_dZero )  { dfpd_R=1000.0, dfpd_H=1055.0;}
            else if (fabs(dfpy-1295.0)<m_dZero )  { dfpd_R=970.0,  dfpd_H=1025.0;}
        }
        dfpd = (nBridgeType==0) ? dfpd_H : dfpd_R;	
    } 
    else
    {
        dfpd = dfpy; //MatdD.Data1.Design.S_Fy1;
    }

    return TRUE;
}

BOOL CDgnPscCommon::Get_MatlRbar(ElemPairK ElemK, double& dfskM, double& dfsdtM, double& dfsdcM, double& dfskS, double& dfsdtS, double& dfsdcS)
{
    // Always Code Units (N,mm).
    dfskM =0.0;
    dfsdtM=0.0;
    dfsdcM=0.0;
    dfskS =0.0;
    dfsdtS=0.0;
    dfsdcS=0.0;
    ////////////////////////////
    // Get Matl Data.
    int nMatlK = m_pDataCtrl->GetMaterialKey4ElemPairK(ElemK);
    T_MATD_D MatdD;
    MatdD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(nMatlK,MatdD))	{ASSERT(0); return FALSE;}
    if ( !(MatdD.Data1.CodeName==MATLCODE_CON_JTG04 || MatdD.Data1.CodeName==MATLCODE_CON_TB05)) { ASSERT(0); return FALSE; }
    for(int i=0; i<2; ++i)	// 0=M-bar, 1=S-bar.
    {
        CString strRbarNa = (i==0 ? MatdD.MainRebar_RebarName : MatdD.SubRebar_RebarName);
        double dfsk=0.0, dfsdt=0.0, dfsdc=0.0;
        // See JTG D62-04 Table 3.2.4-1 fsd,f'sd(보통철근인장,압축설계값).
        if(MatdD.Data1.CodeName==MATLCODE_CON_JTG04)
        {
            if(strRbarNa==_T("R235"))		{dfsk=235.0; dfsdt=195.0; dfsdc=195.0;}
            else if(strRbarNa==_T("HRB335"))	{dfsk=335.0; dfsdt=280.0; dfsdc=280.0;}
            else if(strRbarNa==_T("HRB400"))	{dfsk=400.0; dfsdt=330.0; dfsdc=330.0;}
            else if(strRbarNa==_T("KL400"))		{dfsk=400.0; dfsdt=330.0; dfsdc=330.0;}
            else	{ASSERT(0);	return FALSE;}
        }
        else if(MatdD.Data1.CodeName==MATLCODE_CON_TB05)
        {
            if(strRbarNa==_T("Q235"))		{dfsk=235.0; dfsdt=235.0; dfsdc=235.0;}
            else if(strRbarNa==_T("HRB335"))	{dfsk=335.0; dfsdt=335.0; dfsdc=335.0;}
        }
        (i==0 ? dfskM  : dfskS ) = dfsk;
        (i==0 ? dfsdtM : dfsdtS) = dfsdt;
        (i==0 ? dfsdcM : dfsdcS) = dfsdc;
    }
    return TRUE;
}

BOOL CDgnPscCommon::Get_MatlConc(ElemPairK ElemK, double& dfck, double& dftk, double& dfckp, double& dftkp, double& dfcd, double& dftd, double& dGc)
{
    // Always Code Units (N,mm).
    dfck=0.0;
    dftk=0.0;
    dfckp=0.0;
    dftkp=0.0;
    dfcd=0.0;
    dftd=0.0;
    dGc=0.0;
    ////////////////////////////
    // Get Matl Data.
    int nMatlK = m_pDataCtrl->GetMaterialKey4ElemPairK(ElemK);
    T_MATD_D MatdD;
    MatdD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(nMatlK,MatdD))	{ASSERT(0); return FALSE;}
    if(!(MatdD.Data1.CodeName==MATLCODE_CON_JTG04 || MatdD.Data1.CodeName==MATLCODE_CON_TB05))	{ASSERT(0); return FALSE;}
    // See JTG D62-04 Table 3.1.5 fck,ftk(콘크리트압축,인장표준값), 3.1.6 fcd,ftd(콘크리트압축,인장설계값).
    if(MatdD.Data1.CodeName==MATLCODE_CON_JTG04 )
    {
        if(MatdD.Data1.CodeMatlName==_T("C15"))	{dfck=10.0; dftk=1.27; dfcd= 6.9; dftd=0.88;}
        else if(MatdD.Data1.CodeMatlName==_T("C20"))	{dfck=13.4; dftk=1.54; dfcd= 9.2; dftd=1.06;}
        else if(MatdD.Data1.CodeMatlName==_T("C25"))	{dfck=16.7; dftk=1.78; dfcd=11.5; dftd=1.23;}
        else if(MatdD.Data1.CodeMatlName==_T("C30"))	{dfck=20.1; dftk=2.01; dfcd=13.8; dftd=1.39;}
        else if(MatdD.Data1.CodeMatlName==_T("C35"))	{dfck=23.4; dftk=2.20; dfcd=16.1; dftd=1.52;}
        else if(MatdD.Data1.CodeMatlName==_T("C40"))	{dfck=26.8; dftk=2.40; dfcd=18.4; dftd=1.65;}
        else if(MatdD.Data1.CodeMatlName==_T("C45"))	{dfck=29.6; dftk=2.51; dfcd=20.5; dftd=1.74;}
        else if(MatdD.Data1.CodeMatlName==_T("C50"))	{dfck=32.4; dftk=2.65; dfcd=22.4; dftd=1.83;}
        else if(MatdD.Data1.CodeMatlName==_T("C55"))	{dfck=35.5; dftk=2.74; dfcd=24.4; dftd=1.89;}
        else if(MatdD.Data1.CodeMatlName==_T("C60"))	{dfck=38.5; dftk=2.85; dfcd=26.5; dftd=1.96;}
        else if(MatdD.Data1.CodeMatlName==_T("C65"))	{dfck=41.5; dftk=2.93; dfcd=28.5; dftd=2.02;}
        else if(MatdD.Data1.CodeMatlName==_T("C70"))	{dfck=44.5; dftk=3.00; dfcd=30.5; dftd=2.07;}
        else if(MatdD.Data1.CodeMatlName==_T("C75"))	{dfck=47.4; dftk=3.05; dfcd=32.4; dftd=2.10;}
        else if(MatdD.Data1.CodeMatlName==_T("C80"))	{dfck=50.2; dftk=3.10; dfcd=34.6; dftd=2.14;}
        else	{ASSERT(0);	return FALSE;}
        // Add by ZINU.('05.03.09). Agree with 계만수BJ님('05.03.05).
        dfckp = 0.8 * dfck;
        dftkp = 0.8 * dftk;
        // See JTG D62-04 3.1.7 Gc(콘크리트전단변형계수).
        dGc = 0.4 * MatdD.Data1.Analysis.Elast;

        ////////////////////////////
        // Get Secg Data.
        _PSC_SECG_ELEM SecgElem; SecgElem.Initialize();
        if(!Get_PscSecgElem(ElemK, SecgElem)) {ASSERT(0); return FALSE;}
        double dyDim = max(SecgElem.SectPosi[0].dyDim, SecgElem.SectPosi[1].dyDim);
        double dzDim = max(SecgElem.SectPosi[0].dzDim, SecgElem.SectPosi[1].dzDim);
        // See JTG D62-04 Table 3.1.6 주.
        if(max(dyDim,dzDim) < 300.+m_dZero)	{dfcd*=0.8; dftd*=0.8;}
    }
    else if(MatdD.Data1.CodeName==MATLCODE_CON_TB05 )
    {
        if(MatdD.Data1.CodeMatlName==_T("C20"))	{dfck=13.5; dftk=1.70; dfcd=13.5; dftd=1.70;}
        else if(MatdD.Data1.CodeMatlName==_T("C25"))	{dfck=17.0; dftk=2.00; dfcd=17.0; dftd=2.00;}
        else if(MatdD.Data1.CodeMatlName==_T("C30"))	{dfck=20.0; dftk=2.20; dfcd=20.0; dftd=2.20;}
        else if(MatdD.Data1.CodeMatlName==_T("C35"))	{dfck=23.5; dftk=2.25; dfcd=23.5; dftd=2.25;}
        else if(MatdD.Data1.CodeMatlName==_T("C40"))	{dfck=27.0; dftk=2.70; dfcd=27.0; dftd=2.70;}
        else if(MatdD.Data1.CodeMatlName==_T("C45"))	{dfck=30.0; dftk=2.90; dfcd=30.0; dftd=2.90;}
        else if(MatdD.Data1.CodeMatlName==_T("C50"))	{dfck=33.5; dftk=3.10; dfcd=33.5; dftd=3.10;}
        else if(MatdD.Data1.CodeMatlName==_T("C55"))	{dfck=37.0; dftk=3.30; dfcd=37.0; dftd=3.30;}
        else if(MatdD.Data1.CodeMatlName==_T("C60"))	{dfck=40.0; dftk=3.50; dfcd=40.0; dftd=3.50;}
        else	{ASSERT(0);	return FALSE;}
        // Add by ZINU.('05.03.09). Agree with 계만수BJ님('05.03.05).
        dfckp = 0.8 * dfck;
        dftkp = 0.8 * dftk;
        // See JTG D62-04 3.1.7 Gc(콘크리트전단변형계수).
        dGc = 0.4 * MatdD.Data1.Analysis.Elast;
    }

    return TRUE;
}


BOOL CDgnPscCommon::Get_fcd_4EuroCode(int iNA, BOOL bUser, double dAlpha_cc, double dGamma_c_PT, double dGamma_c_A, double dfck, double& dfcd_PT, double& dfcd_A)
{

    if(bUser)
    {
        dfcd_PT = dGamma_c_PT==0.0 ? 0.0 : dAlpha_cc*dfck/dGamma_c_PT;
        dfcd_A  = dGamma_c_A ==0.0 ? 0.0 : dAlpha_cc*dfck/dGamma_c_A;
    }
    else
    {
        int nType=0;
        double dAlpha_cc_annex = Get_Alpha_cc_4EuroCode(iNA);
        double dGamma_c = Get_Gamma_c_4EuroCode(iNA, nType);
        dfcd_PT = dGamma_c==0.0 ? 0.0 : dAlpha_cc_annex*dfck/dGamma_c;
        //
        nType=1;
        dGamma_c = Get_Gamma_c_4EuroCode(iNA, nType);
        dfcd_A = dGamma_c==0.0 ? 0.0 :dfck/dGamma_c;
    }
    return TRUE;
}

BOOL CDgnPscCommon::Get_fcd_4IRC112(int iNA, BOOL bUser, double dAlpha_cc, double dGamma_c_PT, double dGamma_c_A, double dfck, double& dfcd_PT, double& dfcd_A)
{

    if(bUser)
    {
        dfcd_PT = dGamma_c_PT==0.0 ? 0.0 : dAlpha_cc*dfck/dGamma_c_PT;
        dfcd_A  = dGamma_c_A ==0.0 ? 0.0 : dAlpha_cc*dfck/dGamma_c_A;
    }
    else
    {
        int nType=0;
        double dAlpha_cc_annex = Get_Alpha_cc_4IRC112();
        double dGamma_c = Get_Gamma_c_4IRC112(nType);
        dfcd_PT = dGamma_c==0.0 ? 0.0 : dAlpha_cc_annex*dfck/dGamma_c;
        //
        nType=1;
        dGamma_c = Get_Gamma_c_4IRC112( nType);
        dfcd_A = dGamma_c==0.0 ? 0.0 :dfck/dGamma_c;
    }
    return TRUE;
}

BOOL CDgnPscCommon::Get_fcd_4BRA(BOOL bUser, double dGamma_c_N, double dGamma_c_SC, double dfck, double& dfcd_N, double& dfcd_SC)
{
    if(bUser)
    {
        dfcd_N  = SafeDiv(dfck, dGamma_c_N);
        dfcd_SC = SafeDiv(dfck, dGamma_c_SC);
    }
    else
    {
        int nType=0;
        double dAlpha_cc_annex = Get_Alpha_cc_4BRA();
        double dGamma_c = Get_Gamma_c_4BRA(nType);
        dfcd_N = SafeDiv(dAlpha_cc_annex*dfck, dGamma_c);
        //
        nType=1;
        dGamma_c = Get_Gamma_c_4BRA(nType);
        dfcd_SC = SafeDiv(dfck, dGamma_c);
    }
    return TRUE;
}


BOOL CDgnPscCommon::Get_fyd_4EuroCode(int iNA, BOOL bUser, double dGamma_s_PT, double dGamma_s_A, double dfyk, double dfywk, double& dfyd_PT, double& dfyd_A, double& dfywd_PT, double& dfywd_A)
{
    if(bUser)
    {
        dfyd_PT = dGamma_s_PT==0.0 ? 0.0 :dfyk/dGamma_s_PT;
        dfyd_A  = dGamma_s_A==0.0 ? 0.0 :dfyk/dGamma_s_A;
        dfywd_PT = dGamma_s_PT==0.0 ? 0.0 :dfywk/dGamma_s_PT;
        dfywd_A  = dGamma_s_A==0.0 ? 0.0 :dfywk/dGamma_s_A;
    }
    else
    {
        int nType=0;
        double dGamma_s = Get_Gamma_s_4EuroCode(iNA, nType);
        dfyd_PT = dGamma_s==0.0 ? 0.0 :dfyk/dGamma_s;
        dfywd_PT = dGamma_s==0.0 ? 0.0 :dfywk/dGamma_s;
        //
        nType=1;
        dGamma_s = Get_Gamma_s_4EuroCode(iNA, nType);
        dfyd_A = dGamma_s==0.0 ? 0.0 :dfyk/dGamma_s;
        dfywd_A = dGamma_s==0.0 ? 0.0 :dfywk/dGamma_s;
    } 

    return TRUE;
}

BOOL CDgnPscCommon::Get_fyd_4IRC112(int iNA, BOOL bUser, double dGamma_s_PT, double dGamma_s_A, double dfyk, double dfywk, double& dfyd_PT, double& dfyd_A, double& dfywd_PT, double& dfywd_A)
{
    if(bUser)
    {
        dfyd_PT = dGamma_s_PT==0.0 ? 0.0 :dfyk/dGamma_s_PT;
        dfyd_A  = dGamma_s_A==0.0 ? 0.0 :dfyk/dGamma_s_A;
        dfywd_PT = dGamma_s_PT==0.0 ? 0.0 :dfywk/dGamma_s_PT;
        dfywd_A  = dGamma_s_A==0.0 ? 0.0 :dfywk/dGamma_s_A;
    }
    else
    {
        int nType=0;
        double dGamma_s = Get_Gamma_s_4IRC112(nType);
        dfyd_PT = dGamma_s==0.0 ? 0.0 :dfyk/dGamma_s;
        dfywd_PT = dGamma_s==0.0 ? 0.0 :dfywk/dGamma_s;
        //
        nType=1;
        dGamma_s = Get_Gamma_s_4IRC112(nType);
        dfyd_A = dGamma_s==0.0 ? 0.0 :dfyk/dGamma_s;
        dfywd_A = dGamma_s==0.0 ? 0.0 :dfywk/dGamma_s;
    } 

    return TRUE;
}

BOOL CDgnPscCommon::Get_fyd_4BRA(BOOL bUser, double dGamma_s_N, double dGamma_s_SC, double dfyk, double dfywk, double& dfyd_N, double& dfyd_SC, double& dfywd_N, double& dfywd_SC)
{
    if(bUser)
    {
        dfyd_N   = SafeDiv(dfyk, dGamma_s_N);
        dfyd_SC  = SafeDiv(dfyk, dGamma_s_SC);
        dfywd_N  = SafeDiv(dfywk, dGamma_s_N);
        dfywd_SC = SafeDiv(dfywk, dGamma_s_SC);
    }
    else
    {
        int nType=0;
        double dGamma_s = Get_Gamma_s_4BRA(nType);
        dfyd_N   = SafeDiv(dfyk, dGamma_s);
        dfywd_SC = SafeDiv(dfywk, dGamma_s);
        //
        nType=1;
        dGamma_s = Get_Gamma_s_4BRA(nType);
        dfyd_N   = SafeDiv(dfyk, dGamma_s);
        dfywd_SC = SafeDiv(dfywk, dGamma_s);
    } 

    return TRUE;
}

BOOL CDgnPscCommon::Get_fpd_4EuroCode(int iNA, BOOL bUser, double dGamma_ps_PT, double dGamma_ps_A, double dfp01k, double& dfpd_PT, double& dfpd_A)
{
    if(bUser)
    {
        dfpd_PT = dGamma_ps_PT==0.0 ? 0.0 :dfp01k/dGamma_ps_PT;
        dfpd_A  = dGamma_ps_A==0.0 ? 0.0 :dfp01k/dGamma_ps_A;
    }
    else
    {
        int nType=0;
        double dGamma_s = Get_Gamma_ps_4EuroCode(iNA, nType);
        dfpd_PT = dGamma_s==0.0 ? 0.0 :dfp01k/dGamma_s;
        //
        nType=1;
        dGamma_s = Get_Gamma_ps_4EuroCode(iNA, nType);
        dfpd_A = dGamma_s==0.0 ? 0.0 :dfp01k/dGamma_s;
    } 

    return TRUE;
}

BOOL CDgnPscCommon::Get_fpd_4IRC112(int iNA, BOOL bUser, double dGamma_ps_PT, double dGamma_ps_A, double dfp01k, double& dfpd_PT, double& dfpd_A)
{
    if(bUser)
    {
        dfpd_PT = dGamma_ps_PT==0.0 ? 0.0 :dfp01k/dGamma_ps_PT;
        dfpd_A  = dGamma_ps_A==0.0 ? 0.0 :dfp01k/dGamma_ps_A;
    }
    else
    {
        int nType=0;
        double dGamma_s = Get_Gamma_ps_4IRC112( nType);
        dfpd_PT = dGamma_s==0.0 ? 0.0 :dfp01k/dGamma_s;
        //
        nType=1;
        dGamma_s = Get_Gamma_ps_4IRC112( nType);
        dfpd_A = dGamma_s==0.0 ? 0.0 :dfp01k/dGamma_s;
    } 

    return TRUE;
}

BOOL CDgnPscCommon::Get_fpd_4BRA(BOOL bUser, double dGamma_ps_N, double dGamma_ps_SC, double dfp01k, double& dfpd_N, double& dfpd_SC)
{
    if(bUser)
    {
        dfpd_N  = SafeDiv(dfp01k, dGamma_ps_N);
        dfpd_SC = SafeDiv(dfp01k, dGamma_ps_SC);
    }
    else
    {
        int nType=0;
        double dGamma_s = Get_Gamma_ps_4BRA(nType);
        dfpd_N = SafeDiv(dfp01k, dGamma_s);
        //
        nType=1;
        dGamma_s = Get_Gamma_ps_4BRA(nType);
        dfpd_SC = SafeDiv(dfp01k, dGamma_s);
    } 

    return TRUE;
}

double CDgnPscCommon::Get_Gamma_c_4EuroCode(int iNA, int nType)
{
    // nType : 0=Persistent & Transient, 1=Accidental
    double dGamma_c=0.0;

    if(nType==0)
    {
        dGamma_c = 1.5;
    }
    else
    {
        dGamma_c = 1.2;
    }

    return dGamma_c;
}

double CDgnPscCommon::Get_Gamma_c_4IRC112( int nType)
{
    // nType : 0=Persistent & Transient, 1=Accidental
    // for IRC nType : 0=Basic & Seismic, 1=Accidental
    double dGamma_c=0.0;

    if(nType==0)
    {
        dGamma_c = 1.5;

    }
    else
    {
        dGamma_c = 1.2;

    }

    return dGamma_c;
}

double CDgnPscCommon::Get_Gamma_c_4BRA(int nType)
{
    // nType : 0=Persistent & Transient, 1=Accidental
    double dGamma_c=0.0;

    if(nType==0)
    {
        dGamma_c = 1.4;
    }
    else
    {
        dGamma_c = 1.2;
    }

    return dGamma_c;
}

double CDgnPscCommon::Get_Gamma_s_4EuroCode(int iNA, int nType)
{
    // nType : 0=Persistent & Transient, 1=Accidental
    double dGamma_s=0.0;

    if(nType==0)
    {
        dGamma_s = 1.15;
    }
    else
    {
        dGamma_s = 1.0;
    }

    return dGamma_s;
}

double CDgnPscCommon::Get_Gamma_s_4IRC112( int nType)
{
    // nType : 0=Persistent & Transient, 1=Accidental
    // for IRC nType : 0=Basic & Seismic, 1=Accidental
    double dGamma_s=0.0;

    if(nType==0)
    {
        dGamma_s = 1.15;		
    }
    else
    {
        dGamma_s = 1.0;	
    }

    return dGamma_s;
}

double CDgnPscCommon::Get_Gamma_s_4BRA(int nType)
{
    // nType : 0=Persistent & Transient, 1=Accidental
    double dGamma_s=0.0;

    if(nType==0)
    {
        dGamma_s = 1.15;
    }
    else
    {
        dGamma_s = 1.0;
    }

    return dGamma_s;
}

double CDgnPscCommon::Get_Gamma_ps_4EuroCode(int iNA, int nType)
{
    // nType : 0=Persistent & Transient, 1=Accidental
    double dGamma_ps=0;

    if(nType==0)
    {
        dGamma_ps = 1.15;
    }
    else
    {
        dGamma_ps = 1.0;
    }

    return dGamma_ps;
}

double CDgnPscCommon::Get_Gamma_ps_4IRC112(int nType)
{
    // nType : 0=Persistent & Transient, 1=Accidental
    // for IRC nType : 0=Basic & Seismic, 1=Accidental
    double dGamma_ps=0;

    if(nType==0)
    {
        dGamma_ps = 1.15;		
    }
    else
    {
        dGamma_ps = 1.0;		
    }

    return dGamma_ps;
}

double CDgnPscCommon::Get_Gamma_ps_4BRA(int nType)
{
    // nType : 0=Persistent & Transient, 1=Accidental
    double dGamma_ps=0;

    if(nType==0)
    {
        dGamma_ps = 1.15;
    }
    else
    {
        dGamma_ps = 1.0;
    }

    return dGamma_ps;
}

double CDgnPscCommon::Get_Alpha_cc_4EuroCode(int iNA)
{
    double dAlpha_cc = 0.0;

    if ( !m_pNA )
    {
        ASSERT(0); return dAlpha_cc;
    }

    enAlphacc Type;
    m_pNA->GetParamType(Type);

    TNAParam Param;
    switch ( Type )
    {
    case enAlphacc::None:
        break;
    case enAlphacc::bCompression:
        {
            bool bCompression = true;
            Param.vb.push_back(bCompression);
        }
        break;
    case enAlphacc::bSimpleBending:
        {
            bool bSimpleBending = true;
            Param.vb.push_back(bSimpleBending);
        }
        break;
    case enAlphacc::bLightWeight:
        {
            bool bLightWeight = false;
            Param.vb.push_back(bLightWeight);
        }
        break;
    case enAlphacc::RatioOfLongterm:
        {
            double dRatioOfLongterm = 0.9;
            Param.vd.push_back(dRatioOfLongterm);
        }
        break;
    default: ASSERT(0); break;
    }

    return m_pNA->GetAlphacc(Type, Param);
}

double CDgnPscCommon::Get_Alpha_ct_4EuroCode(int iNA)
{
    double dAlpha_ct = 0.0;

    if ( !m_pNA )
    {
        ASSERT(0); return dAlpha_ct;
    }

    return m_pNA->GetAlphact();
}

double CDgnPscCommon::Get_Alpha_cc_4IRC112()
{
    double dAlpha_cc = 0.67;
    return dAlpha_cc;
}

double CDgnPscCommon::Get_Alpha_ct_4IRC112()
{
    double dAlpha_ct = 0.0;

    dAlpha_ct = 1.0;	

    return dAlpha_ct;
}

double CDgnPscCommon::Get_Alpha_cc_4BRA()
{
    double dAlpha_cc = 1.0;
    return dAlpha_cc;
}

double CDgnPscCommon::Get_Alpha_ct_4BRA()
{
    double dAlpha_ct = 1.0;
    return dAlpha_ct;
}


BOOL CDgnPscCommon::Get_fcd_4BS(BOOL bChar, BOOL bUser, double dGamma_mc, double dfcu, double& dfcd)
{
    if(bUser)
    {
        dfcd = dGamma_mc==0.0 ? 0.0 :dfcu/dGamma_mc;
    }
    else
    {
        double dGam_mc = bChar ? 1.50 : 1.20;
        dfcd = dGam_mc==0.0 ? 0.0 :dfcu/dGam_mc;
    } 
    return TRUE;
}

BOOL CDgnPscCommon::Get_fyd_4BS(BOOL bChar, BOOL bUser, double dGamma_ms, double dfy,  double& dfyd)
{
    if(bUser)
    {
        dfyd = dGamma_ms==0.0 ? 0.0 :dfy/dGamma_ms;
    }
    else
    {
        double dGam_ms = bChar ? 1.15 : 1.10;
        dfyd = dGam_ms==0.0 ? 0.0 :dfy/dGam_ms;
    } 
    return TRUE;
}

// BOOL CDgnPscCommon::Get_fysd_4BS(BOOL bChar, BOOL bUser, double dGamma_ms, double dfys,  double& dfysd)
// {
//     return TRUE;
// }

BOOL CDgnPscCommon::Get_fpd_4BS(BOOL bChar, BOOL bUser, double dGamma_ms, double dfpu, double& dfpd)
{
    dfpd = dGamma_ms==0.0 ? 0.0 :dfpu/dGamma_ms;

    return TRUE;
}

BOOL CDgnPscCommon::Get_fcd_SLS_4BS(BOOL bChar, BOOL bUser, double dGamma_mc, double dGamma_mt_pre, double dGamma_mt_post, double dfcu, double& dfcd_c, double& dfcd_t_pre, double& dfcd_t_post)
{
    double dGam_mc      = 1.25;
    double dGam_mt_pre  = 1.25;
    double dGam_mt_post = 1.55;
    if (bUser)
    {
        dGam_mc      = dGamma_mc;
        dGam_mt_pre  = dGamma_mt_pre;
        dGam_mt_post = dGamma_mt_post;
    }

    dfcd_c      = dGam_mc     ==0.0 ? 0.0 :dfcu/dGam_mc;
    dfcd_t_pre  = dGam_mt_pre ==0.0 ? 0.0 :dfcu/dGam_mt_pre;
    dfcd_t_post = dGam_mt_post==0.0 ? 0.0 :dfcu/dGam_mt_post;


    return TRUE;
}

// Composite Plate Girder
BOOL CDgnPscCommon::Get_MatlCpgConc(ElemPairK ElemK, double& dfck, double& dfcm, double& dfctm, double& dEcm)
{
    // Always Code Units (N,mm).
    dfck=0.0;
    dfcm=0.0;
    dfctm=0.0;
    dEcm=0.0;
    ////////////////////////////
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    // Get Matl Data.
    int nMatlK = m_pDataCtrl->GetMaterialKey4ElemPairK(ElemK);
    T_MATD_D MatdD;
    MatdD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(nMatlK,MatdD))	{ASSERT(0); return FALSE;}


    // MQC 10967. User Defined Matl 지원을 위해 Code Name Check   조건 막음(PCS, 15.07.21) 
    //if(MatdD.Data2.CodeName==_T("")) return FALSE;
    if(MatdD.Data2.Design.C_fc < cDGN_Zero) return FALSE;
    // Ref. EN 1992-1-1:2004, 29p
    const CString& strMatlName = MatdD.Data2.CodeMatlName;
    if(MatdD.Data2.CodeName==MATLCODE_CON_EN04 || MatdD.Data2.CodeName==MATLCODE_CON_SS)	
    {
        if(strMatlName ==_T("C12/15"))	 {dfck=12.0; dfcm=20.0; dfctm=1.6; dEcm=27.0E+03;}
        else if(strMatlName==_T("C16/20"))	{dfck=16.0; dfcm=24.0; dfctm=1.9; dEcm=29.0E+03;}
        else if(strMatlName==_T("C20/25"))	{dfck=20.0; dfcm=28.0; dfctm=2.2; dEcm=30.0E+03;}
        else if(strMatlName==_T("C25/30"))	{dfck=25.0; dfcm=33.0; dfctm=2.6; dEcm=31.0E+03;}
        else if(strMatlName==_T("C30/37"))	{dfck=30.0; dfcm=38.0; dfctm=2.9; dEcm=33.0E+03;}
        else if(strMatlName==_T("C32/40"))	{dfck=32.0; dfcm=40.0; dfctm=3.0; dEcm=33.4E+03;}
        else if(strMatlName==_T("C35/45"))	{dfck=35.0; dfcm=43.0; dfctm=3.2; dEcm=34.0E+03;}
        else if(strMatlName==_T("C40/50"))	{dfck=40.0; dfcm=48.0; dfctm=3.5; dEcm=35.0E+03;}
        else if(strMatlName==_T("C45/55"))	{dfck=45.0; dfcm=53.0; dfctm=3.8; dEcm=36.0E+03;}
        else if(strMatlName==_T("C50/60"))	{dfck=50.0; dfcm=58.0; dfctm=4.1; dEcm=37.0E+03;}
        else if(strMatlName==_T("C55/67"))	{dfck=55.0; dfcm=63.0; dfctm=4.2; dEcm=38.0E+03;}
        else if(strMatlName==_T("C60/75"))	{dfck=60.0; dfcm=68.0; dfctm=4.4; dEcm=39.0E+03;}
        else if(strMatlName==_T("C70/85"))	{dfck=70.0; dfcm=78.0; dfctm=4.6; dEcm=41.0E+03;}
        else if(strMatlName==_T("C80/95"))	{dfck=80.0; dfcm=88.0; dfctm=4.8; dEcm=42.0E+03;}
        else if(strMatlName==_T("C90/105")) {dfck=90.0; dfcm=98.0; dfctm=5.0; dEcm=44.0E+03;}
        else	{ASSERT(0);	return FALSE;}
    }
    else if(MatdD.Data2.CodeName==MATLCODE_CON_NTC08 || MatdD.Data2.CodeName==MATLCODE_CON_NTC12 || MatdD.Data2.CodeName==MATLCODE_CON_NTC18) // Modify by GAY. PMS:4355. ('11.12.22).
    {
        if(strMatlName ==_T("C8/10"))	 {dfck= 8.0; dfcm=16.0; dfctm=1.2; dEcm=25.0E+03;}
        else if(strMatlName==_T("C12/15"))	{dfck=12.0; dfcm=20.0; dfctm=1.6; dEcm=27.0E+03;}
        else if(strMatlName==_T("C16/20"))	{dfck=16.0; dfcm=24.0; dfctm=1.9; dEcm=29.0E+03;}
        else if(strMatlName==_T("C20/25"))	{dfck=20.0; dfcm=28.0; dfctm=2.2; dEcm=30.0E+03;}
        else if(strMatlName==_T("C25/30"))	{dfck=25.0; dfcm=33.0; dfctm=2.6; dEcm=31.0E+03;}
        else if(strMatlName==_T("C28/35"))	{dfck=28.0; dfcm=36.0; dfctm=2.8; dEcm=32.0E+03;}
        else if(strMatlName==_T("C30/37"))	{dfck=30.0; dfcm=38.0; dfctm=2.9; dEcm=33.0E+03;}
        else if(strMatlName==_T("C32/40"))	{dfck=32.0; dfcm=40.0; dfctm=3.0; dEcm=33.4E+03;}
        else if(strMatlName==_T("C35/45"))	{dfck=35.0; dfcm=43.0; dfctm=3.2; dEcm=34.0E+03;}
        else if(strMatlName==_T("C40/50"))	{dfck=40.0; dfcm=48.0; dfctm=3.5; dEcm=35.0E+03;}
        else if(strMatlName==_T("C45/55"))	{dfck=45.0; dfcm=53.0; dfctm=3.8; dEcm=36.0E+03;}
        else if(strMatlName==_T("C50/60"))	{dfck=50.0; dfcm=58.0; dfctm=4.1; dEcm=37.0E+03;}
        else if(strMatlName==_T("C55/67"))	{dfck=55.0; dfcm=63.0; dfctm=4.2; dEcm=38.0E+03;}
        else if(strMatlName==_T("C60/75"))	{dfck=60.0; dfcm=68.0; dfctm=4.4; dEcm=39.0E+03;}
        else if(strMatlName==_T("C70/85"))	{dfck=70.0; dfcm=78.0; dfctm=4.6; dEcm=41.0E+03;}
        else if(strMatlName==_T("C80/95"))	{dfck=80.0; dfcm=88.0; dfctm=4.8; dEcm=42.0E+03;}
        else if(strMatlName==_T("C90/105")) {dfck=90.0; dfcm=98.0; dfctm=5.0; dEcm=44.0E+03;}
        else	{ASSERT(0);	return FALSE;}
    }
    else
    {
        dfck = MatdD.Data2.Design.C_fc;
        dfcm = dfck + 8.0;
        if(dfck<=50.0)
        {
            dfctm = 0.3*pow(dfck,2./3.);
        }
        else
        {
            dfctm = 2.12*log(1.0+(dfcm/10.));
        }
        dEcm = 22.0E+03*pow(dfcm/10.0,0.3);

    }

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
    return TRUE;
}



BOOL CDgnPscCommon::CalcSectPSC_QA(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, double& dCutArea, double& dQ)
{
    // Outer=CCW, Inn=CW.
    dQ = 0.0;
    dCutArea = 0.0;
    //*************************************
    // Get Polygon by Cutting Line.
    I_PolyMaker* pPolyMaker;
    pPolyMaker = I_PolyMakerFactory::GetInstance()->New_PMaker();
    int i=0, j=0, k=0;    
    // Outer.
    CArray<XGL_3dp,XGL_3dp&> arOutCut;
    for(i=0; i<arOuter.GetSize(); ++i)
    {
        T_SPSC_COOR SpscCoor = arOuter[i];
        for(j=0; j<SpscCoor.arSpscPont[0].GetSize(); ++j)
        {
            // x,y Coordinate (from Left, Bottom).
            double dx = SpscCoor.arSpscPont[0].GetAt(j);
            double dy = SpscCoor.arSpscPont[1].GetAt(j);
            XGL_3dp OutXGL;
            OutXGL.Set(dx,dy,0.0);
            arOutCut.Add(OutXGL);
        }
    }
    pPolyMaker->SetOutterPoly(arOutCut);

    // Inner.
    for(i=0; i<arInner.GetSize(); ++i)
    {
        CArray<XGL_3dp,XGL_3dp&> arInnCut;
        T_SPSC_COOR_AR* apSpscCoor = arInner[i];
        for(j=0; j<apSpscCoor->GetSize(); ++j)
        {
            arInnCut.RemoveAll();
            T_SPSC_COOR SpscCoor = apSpscCoor->GetAt(j);
            for(k=0; k<SpscCoor.arSpscPont[0].GetSize(); ++k)
            {
                // x,y Coordinate (from Left, Bottom).
                double dx = SpscCoor.arSpscPont[0].GetAt(k);
                double dy = SpscCoor.arSpscPont[1].GetAt(k);
                XGL_3dp InnXGL;
                InnXGL.Set(dx,dy,0.0);
                arInnCut.Add(InnXGL);
            }
            pPolyMaker->AddInnerPoly(arInnCut);
        }
    }

    double dZero=1.0E-07;
    // Make Polygon by Cutting Line.
    // Get Left Part of Cutting Line (Sta->End).
    XGL_3dp StaXGL, EndXGL;
    double dxSta=0.0, dxEnd=0.0, dySta=0.0, dyEnd=0.0;
    if(bDir==1)  // 1: Left->Right , Top->Bot  0: Right->Left, Bot->Top
    {
        dxSta = (baxis==1 ? (-1)*dZero : dpos);  //1: y-axis 0: z-axis
        dxEnd = (baxis==1 ? dlen+dZero : dpos);
        dySta = (baxis==1 ? dpos : dlen+dZero);
        dyEnd = (baxis==1 ? dpos : (-1)*dZero);
    }
    else
    {
        dxSta = (baxis==1 ? dlen+dZero : dpos);  //1: y-axis 0: z-axis
        dxEnd = (baxis==1 ? (-1)*dZero : dpos);
        dySta = (baxis==1 ? dpos : (-1)*dZero);
        dyEnd = (baxis==1 ? dpos : dlen+dZero);
    }

    StaXGL.Set(dxSta,dySta,0.0);
    EndXGL.Set(dxEnd,dyEnd,0.0);
    pPolyMaker->SetCuttingLine(StaXGL,EndXGL);
    if(!pPolyMaker->MakeResult())	ASSERT(0);
    // Result Polygon.
    for(j=0; j<pPolyMaker->GetResultPolyCount(); ++j)
    {
        CArray<XGL_3dp,XGL_3dp&> arResPoly;
        pPolyMaker->GetResultPoly(j,arResPoly);
        int iResNum = arResPoly.GetSize();
        //---------------------------------
        // Calculate Center(x,y), Area.
        double* dpx = new double[iResNum];
        double* dpy = new double[iResNum];
        for(k=0; k<iResNum; ++k)
        {
            dpx[k] = arResPoly[k].x();
            dpy[k] = arResPoly[k].y();
        }
        double dxCen=0.0, dyCen=0.0, dArea=0.0;
        if(!CMathFunc::mathPolyCentroid(iResNum,dpx,dpy,dxCen,dyCen,dArea))	ASSERT(0);
        delete []dpx;
        delete []dpy;
        //---------------------------------
        dCutArea += dArea;
        if(baxis==1)			dQ += dArea*fabs(dyCen-dpos);
        else if(baxis==0)	dQ += dArea*fabs(dxCen-dpos);
        else	ASSERT(0);
    }
    //------------------------------------------------------------------------------
    // Hole 을 얻어 오는것이 별도 함수로 분리 되면서 추가된 부분 
    for(j=0; j<pPolyMaker->GetResultHolePolyCount(); ++j)
    {
        CArray<XGL_3dp,XGL_3dp&> arResPoly;
        pPolyMaker->GetResultHolePoly(j,arResPoly);
        int iResNum = arResPoly.GetSize();
        //---------------------------------
        // Calculate Center(x,y), Area.
        double* dpx = new double[iResNum];
        double* dpy = new double[iResNum];
        for(k=0; k<iResNum; ++k)
        {
            dpx[k] = arResPoly[k].x();
            dpy[k] = arResPoly[k].y();
        }
        double dxCen=0.0, dyCen=0.0, dArea=0.0;
        if(!CMathFunc::mathPolyCentroid(iResNum,dpx,dpy,dxCen,dyCen,dArea))	ASSERT(0);
        delete []dpx;
        delete []dpy;
        //---------------------------------
        dCutArea -= dArea;
        if(baxis==1)			dQ -= dArea*fabs(dyCen-dpos);
        else if(baxis==0)	dQ -= dArea*fabs(dxCen-dpos);
        else	ASSERT(0);
    }

    I_PolyMakerFactory::GetInstance()->Del_PMaker(pPolyMaker);


    return TRUE;
}

// Cutting line polygon
/*
BOOL CDgnPscCommon::Get_PolygonCuttingLine(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, CArray<T_SPSC_COOR,T_SPSC_COOR>& arGetOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arGetInner)
{
arGetOuter.RemoveAll();
arGetInner.RemoveAll();

// Outer=CCW, Inn=CW.
//*************************************
// Get Polygon by Cutting Line.
I_PolyMaker* pPolyMaker;
pPolyMaker = I_PolyMakerFactory::GetInstance()->New_PMaker();
int i=0, j=0, k=0;    
// Outer.
CArray<XGL_3dp,XGL_3dp&> arOutCut;
for(i=0; i<arOuter.GetSize(); ++i)
{
T_SPSC_COOR SpscCoor = arOuter[i];
for(j=0; j<SpscCoor.arSpscPont[0].GetSize(); ++j)
{
// x,y Coordinate (from Left, Bottom).
double dx = SpscCoor.arSpscPont[0].GetAt(j);
double dy = SpscCoor.arSpscPont[1].GetAt(j);
XGL_3dp OutXGL;
OutXGL.Set(dx,dy,0.0);
arOutCut.Add(OutXGL);
}
}
pPolyMaker->SetOutterPoly(arOutCut);

// Inner.
for(i=0; i<arInner.GetSize(); ++i)
{
CArray<XGL_3dp,XGL_3dp&> arInnCut;
T_SPSC_COOR_AR* apSpscCoor = arInner[i];
for(j=0; j<apSpscCoor->GetSize(); ++j)
{
arInnCut.RemoveAll();
T_SPSC_COOR SpscCoor = apSpscCoor->GetAt(j);
for(k=0; k<SpscCoor.arSpscPont[0].GetSize(); ++k)
{
// x,y Coordinate (from Left, Bottom).
double dx = SpscCoor.arSpscPont[0].GetAt(k);
double dy = SpscCoor.arSpscPont[1].GetAt(k);
XGL_3dp InnXGL;
InnXGL.Set(dx,dy,0.0);
arInnCut.Add(InnXGL);
}
pPolyMaker->AddInnerPoly(arInnCut);
}
}

double dZero=1.0E-07;
// Make Polygon by Cutting Line.
// Get Left Part of Cutting Line (Sta->End).
XGL_3dp StaXGL, EndXGL;
double dxSta=0.0, dxEnd=0.0, dySta=0.0, dyEnd=0.0;
if(bDir==1)  // 1: Left->Right , Top->Bot  0: Right->Left, Bot->Top
{
dxSta = (baxis==1 ? (-1)*dZero : dpos);  //1: y-axis 0: z-axis
dxEnd = (baxis==1 ? dlen+dZero : dpos);
dySta = (baxis==1 ? dpos : dlen+dZero);
dyEnd = (baxis==1 ? dpos : (-1)*dZero);
}
else
{
dxSta = (baxis==1 ? dlen+dZero : dpos);  //1: y-axis 0: z-axis
dxEnd = (baxis==1 ? (-1)*dZero : dpos);
dySta = (baxis==1 ? dpos : (-1)*dZero);
dyEnd = (baxis==1 ? dpos : dlen+dZero);
}

StaXGL.Set(dxSta,dySta,0.0);
EndXGL.Set(dxEnd,dyEnd,0.0);
pPolyMaker->SetCuttingLine(StaXGL,EndXGL);
if(!pPolyMaker->MakeResult())	ASSERT(0);
// Get Polygon Data.
// Result Polygon.
for(j=0; j<pPolyMaker->GetResultPolyCount(); ++j)
{
CArray<XGL_3dp,XGL_3dp&> arResPoly;
pPolyMaker->GetResultPoly(j,arResPoly);
int iResNum = arResPoly.GetSize();
T_SPSC_COOR CoorD; 
CoorD.Initialize();
CoorD.arSpscPont[0].SetSize(iResNum); // x
CoorD.arSpscPont[1].SetSize(iResNum); // y
//---------------------------------
// Calculate Center(x,y), Area.
double dX=0.0, dY=0.0;		
for(k=0; k<iResNum; ++k)
{
dX = arResPoly[k].x();
dY = arResPoly[k].y();
CoorD.arSpscPont[0].SetAt(k,dX);
CoorD.arSpscPont[1].SetAt(k,dY);
}
arGetOuter.Add(CoorD);
}
//------------------------------------------------------------------------------
// Hole 을 얻어 오는것이 별도 함수로 분리 되면서 추가된 부분 
CArray<T_SPSC_COOR,T_SPSC_COOR>* parSpscInn;
parSpscInn = new CArray<T_SPSC_COOR,T_SPSC_COOR>;

//CArray<T_SPSC_COOR,T_SPSC_COOR> CoorArD;
//CoorArD.RemoveAll();
for(j=0; j<pPolyMaker->GetResultHolePolyCount(); ++j)
{
CArray<XGL_3dp,XGL_3dp&> arResPoly;
pPolyMaker->GetResultHolePoly(j,arResPoly);
int iResNum = arResPoly.GetSize();
T_SPSC_COOR CoorD; 
CoorD.Initialize();
CoorD.arSpscPont[0].SetSize(iResNum); // x
CoorD.arSpscPont[1].SetSize(iResNum); // y
double dX=0.0, dY=0.0;		
for(k=0; k<iResNum; ++k)
{
dX = arResPoly[k].x();
dY = arResPoly[k].y();
CoorD.arSpscPont[0].SetAt(k,dX);
CoorD.arSpscPont[1].SetAt(k,dY);
}
//CoorArD.Add(CoorD);
parSpscInn->Add(CoorD);
}
if(pPolyMaker->GetResultHolePolyCount()>0) 
{
arGetInner.Add(parSpscInn);
}
else
{
if(parSpscInn)	delete parSpscInn;
}

I_PolyMakerFactory::GetInstance()->Del_PMaker(pPolyMaker);  

return TRUE;
}
*/

BOOL CDgnPscCommon::Get_TorsionSectProperty4OpenShape(int nPart, double* dpDivPos, double dLength, double dCover, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter,
                                                      double* dpIxx, double* dpAoh, double* dpPh, double* dpPcp, double* dpAcp, double* dpyDim, double* dpzDim)
{
    if(nPart<1 || nPart>3) { ASSERT(0); return FALSE;}

    BOOL bDirection=TRUE;
    double dCutPos=0.;
    double dIxx=0., dAoh=0., dPh=0., dPcp=0., dAcp=0., dyDim=0., dzDim=0.;
    if(nPart==2)
    {
        // Part 1
        dCutPos = dpDivPos[0];
        Get_TorsionSectProperty4OpenShape_Part(bDirection, dCutPos, dLength, dCover, arOuter, dIxx, dAoh, dPh, dPcp, dAcp, dyDim, dzDim);
        dpIxx[0]=dIxx, dpAoh[0]=dAoh, dpPh[0]=dPh, dpPcp[0]=dPcp, dpAcp[0]=dAcp, dpyDim[0]=dyDim, dpzDim[0]=dzDim;
        // Part 2
        bDirection = FALSE;
        dCutPos -= m_dZero;
        Get_TorsionSectProperty4OpenShape_Part(bDirection, dCutPos, dLength, dCover, arOuter, dIxx, dAoh, dPh, dPcp, dAcp, dyDim, dzDim);
        dpIxx[1]=dIxx, dpAoh[1]=dAoh, dpPh[1]=dPh, dpPcp[1]=dPcp, dpAcp[1]=dAcp, dpyDim[1]=dyDim, dpzDim[1]=dzDim;
    }
    else if(nPart==3)
    {
        // I 형으로 보고.
        // Part 1
        bDirection = TRUE;
        dCutPos = dpDivPos[0];
        Get_TorsionSectProperty4OpenShape_Part(bDirection, dCutPos, dLength, dCover, arOuter, dIxx, dAoh, dPh, dPcp, dAcp, dyDim, dzDim);
        dpIxx[0]=dIxx, dpAoh[0]=dAoh, dpPh[0]=dPh, dpPcp[0]=dPcp, dpAcp[0]=dAcp, dpyDim[0]=dyDim, dpzDim[0]=dzDim;

        // Part 2
        CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arTempInner;
        CArray<T_SPSC_COOR,T_SPSC_COOR> arGetOuter;  
        CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arGetInner;
        T_SPSC_COOR CutPoint;
        bDirection = FALSE; // Top flange를 제외한 polygon을 얻기위해.
        BOOL bYaxis=TRUE;
        if(!CSectUtil::Get_PolygonCuttingLine(arOuter, arTempInner, bDirection, bYaxis, dCutPos, dLength, arGetOuter, arGetInner, CutPoint)) return FALSE;

        bDirection = TRUE;
        dCutPos = dpDivPos[1];
        Get_TorsionSectProperty4OpenShape_Part(bDirection, dCutPos, dLength, dCover, arGetOuter, dIxx, dAoh, dPh, dPcp, dAcp, dyDim, dzDim);
        dpIxx[1]=dIxx, dpAoh[1]=dAoh, dpPh[1]=dPh, dpPcp[1]=dPcp, dpAcp[1]=dAcp, dpyDim[1]=dyDim, dpzDim[1]=dzDim;

        // Part 3
        bDirection = FALSE;
        dCutPos = dpDivPos[1];
        Get_TorsionSectProperty4OpenShape_Part(bDirection, dCutPos, dLength, dCover, arOuter, dIxx, dAoh, dPh, dPcp, dAcp, dyDim, dzDim);
        dpIxx[2]=dIxx, dpAoh[2]=dAoh, dpPh[2]=dPh, dpPcp[2]=dPcp, dpAcp[2]=dAcp, dpyDim[2]=dyDim, dpzDim[2]=dzDim;

    }  

    return TRUE;
}

BOOL CDgnPscCommon::Get_TorsionSectProperty4OpenShape_Part(BOOL bDirection, double dCutPos, double dLength, double dCover, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter,  
                                                           double& dIxx, double& dAoh, double& dPh, double& dPcp, double& dAcp, double& dyDim, double& dzDim)
{  
    BOOL bYaxis=TRUE; // Y축만 하자. 

    CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arTempInner;
    CArray<T_SPSC_COOR,T_SPSC_COOR> arGetOuter;  
    CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arGetInner;
    T_SPSC_COOR CutPoint;

    if(!CSectUtil::Get_PolygonCuttingLine(arOuter, arTempInner, bDirection, bYaxis, dCutPos, dLength, arGetOuter, arGetInner, CutPoint)) return FALSE;

    BOOL bSkip_Ixx_mode = FALSE; 
    double dcutYcent=0.0, dcutZcent=0.0, dcutArea=0.0, dcutIyy=0.0, dcutIzz=0.0, dcutIxx=0.0;
    if(!CSectUtil::GetPscCutSectProperty(arGetOuter, arGetInner, dcutYcent, dcutZcent, dcutArea, dcutIyy, dcutIzz, dcutIxx, bSkip_Ixx_mode)) return FALSE;
    dIxx = dcutIxx;

    //
    dPcp = dAcp = 0.0;
    CSectUtil::Get_Torsion_Pcp_Acp(arGetOuter, dPcp, dAcp);

    //
    if(arGetOuter.GetSize()!=1) return FALSE;
    T_SPSC_COOR &SpscCoor = arGetOuter[0];
    int nSize = SpscCoor.arSpscPont[0].GetSize();

    double (*polyLine)[2];		
    polyLine = new double[nSize][2];  

    int i;
    double xmin=0.0, xmax=0.0, ymax=0.0, ymin=0.0;
    for(i=0 ; i<nSize ; ++i)
    {
        // x,y Coordinate (from Left, Bottom).
        double dx = SpscCoor.arSpscPont[0].GetAt(i);
        double dy = SpscCoor.arSpscPont[1].GetAt(i);
        polyLine[i][0] = dx;
        polyLine[i][1] = dy;
        //
        if(i == 0)
        {
            xmin = xmax = dx;
            ymin = ymax = dy;
        }
        else
        {
            if(dx < xmin) xmin = dx;
            if(dx > xmax) xmax = dx;
            if(dy < ymin) ymin = dy;
            if(dy > ymax) ymax = dy;
        }
    }
    dyDim = xmax-xmin; 
    dzDim = ymax-ymin;

    // =================
    T_PSCD_D PscdD; PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
    if( CDBLib::IsPscCodeLSD(m_iCode) || PscdD.iDgnCode==IRC_112_2011_PSC || m_iCode==IRS_PSC ||
        m_iCode == IRC_112_2020_PSC)
    {
        double dcover_temp = dCover;
        double dt_df  = (dPcp==0.0)? 0.0 : dAcp / dPcp;
        double dThick = (dcover_temp==0.0)? 0.0 : max(dt_df/2.0, dcover_temp);   

        dCover = dThick;
    }
    // =======================

    double dOffset = -1.0*dCover;
    if(!CMathFunc::mathOffsetOfPolyline(dOffset, nSize, polyLine)) 
    { delete[] polyLine; return FALSE; }

    CArray<T_SPSC_COOR,T_SPSC_COOR> arOffestOuter; 
    arOffestOuter.RemoveAll();  
    T_SPSC_COOR CovertCood;
    CovertCood.Initialize();
    CovertCood.arSpscPont[0].SetSize(nSize); // x
    CovertCood.arSpscPont[1].SetSize(nSize); // y
    for(i=0 ; i<nSize ; ++i)
    {    
        CovertCood.arSpscPont[0].SetAt(i,polyLine[i][0]);
        CovertCood.arSpscPont[1].SetAt(i,polyLine[i][1]);
    }
    arOffestOuter.Add(CovertCood);

    dPh = dAoh = 0.0;
    CSectUtil::Get_Torsion_Pcp_Acp(arOffestOuter, dPh, dAoh);

    delete[] polyLine;

    return TRUE;
}


BOOL CDgnPscCommon::Get_TndnReBarInfor(BOOL bYaxis, double dNeuC, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
                                       _DGN_TNDN_RBAR& rTnRb, double dfsd)
{
    double dyDim = SpscSect.dyDim;
    double dzDim = SpscSect.dzDim;
    double dyCen = SpscSect.dyCen;
    double dzCen = SpscSect.dzCen;
    double dCpy  = dyDim - dyCen;
    double dCpz  = dzDim - dzCen;
    double dAstol=0.0, dAsY=0.0, dTtol=0.0;  
    double dCp = (bYaxis==TRUE ? dCpz : dCpy);
    double dgap=0.0;

    double dBotSumfpdtApYp=0.0, dBotSumfpdtAp=0.0;
    double dBotSumfpdcApYp=0.0, dBotSumfpdcAp=0.0;
    //Tension Tendon.
    double ddpt_max =0.0;
    for(int i=0 ; i<arTndnPosi.GetSize() ; ++i)
    {
        _DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
        //if(rTndn.iBondType==1) continue;              
        double dfpdt = rTndn.dfpdt;
        double dfpdc = rTndn.dfpdc;
        double dyposi = (bYaxis==TRUE ? rTndn.dz : rTndn.dy);
        if(dNeuC>=dCp && dyposi<0.0)
        {    
            dgap=dNeuC-dCp;
            if(fabs(dyposi)>dgap)
            {
                dAstol += rTndn.dArea;
                dAsY   += rTndn.dArea*(fabs(dyposi)-dgap);
                dTtol  += rTndn.dStress[0];
                ddpt_max = max(ddpt_max, fabs(dyposi)-dgap);
                // Add
                double dfpdtiApi = dfpdt*rTndn.dArea;
                dBotSumfpdtAp   += dfpdtiApi;
                dBotSumfpdtApYp += dfpdtiApi*(fabs(dyposi)-dgap);
                double dfpdciApi = dfpdc*rTndn.dArea;
                dBotSumfpdcAp   += dfpdciApi;
                dBotSumfpdcApYp += dfpdciApi*(fabs(dyposi)-dgap);
            }
        }
        else if(dNeuC<dCp)
        {
            dgap=dCp-dNeuC;
            if(dyposi<0.0)
            {
                dAstol += rTndn.dArea;
                dAsY += rTndn.dArea*(dgap+fabs(dyposi));
                dTtol  += rTndn.dStress[0];
                ddpt_max = max(ddpt_max, dgap+fabs(dyposi));
                // Add
                double dfpdtiApi = dfpdt*rTndn.dArea;
                dBotSumfpdtAp   += dfpdtiApi;
                dBotSumfpdtApYp += dfpdtiApi*(dgap+fabs(dyposi));
                double dfpdciApi = dfpdc*rTndn.dArea;
                dBotSumfpdcAp   += dfpdciApi;
                dBotSumfpdcApYp += dfpdciApi*(dgap+fabs(dyposi));
            }
            else if(dyposi<dgap)
            {
                dAstol += rTndn.dArea;
                dAsY += rTndn.dArea*(dgap-fabs(dyposi));
                dTtol  += rTndn.dStress[0];
                ddpt_max = max(ddpt_max, dgap+fabs(dyposi));
                // Add
                double dfpdtiApi = dfpdt*rTndn.dArea;
                dBotSumfpdtAp   += dfpdtiApi;
                dBotSumfpdtApYp += dfpdtiApi*(dgap-fabs(dyposi));
                double dfpdciApi = dfpdc*rTndn.dArea;
                dBotSumfpdcAp   += dfpdciApi;
                dBotSumfpdcApYp += dfpdciApi*(dgap-fabs(dyposi));
            }

        }
    }
    rTnRb.dApttol = dAstol;
    rTnRb.dNtodpt = (dAstol==0.0 ? 0.0 : dAsY/dAstol);
    rTnRb.ddpt    = (dAstol==0.0 ? 0.0 : dNeuC+rTnRb.dNtodpt);
    rTnRb.dTpttol = dTtol;
    rTnRb.ddpt_max = ddpt_max;

    double ddpc_max =0.0;
    dAstol=0.0; dAsY=0.0; dTtol=0.0;
    double dTopSumfpdtApYp=0.0, dTopSumfpdtAp=0.0;
    double dTopSumfpdcApYp=0.0, dTopSumfpdcAp=0.0;
    //Compression Tendon.
    for(int i=0 ; i<arTndnPosi.GetSize() ; ++i)
    {
        _DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
        //if(rTndn.iBondType==1) continue;
        double dfpdt = rTndn.dfpdt;
        double dfpdc = rTndn.dfpdc;
        double dyposi = (bYaxis==TRUE ? rTndn.dz : rTndn.dy);
        if(dNeuC>=dCp)
        {    
            dgap=dNeuC-dCp;
            if(dyposi>0.0)
            {
                dAstol += rTndn.dArea;
                dAsY   += rTndn.dArea*(dyposi+dgap);
                dTtol  += rTndn.dStress[0];
                ddpc_max = max(ddpc_max, (dyposi+dgap));
                // Add
                double dfpdtiApi = dfpdt*rTndn.dArea;
                dTopSumfpdtAp   += dfpdtiApi;
                dTopSumfpdtApYp += dfpdtiApi*(dyposi+dgap);
                double dfpdciApi = dfpdc*rTndn.dArea;
                dTopSumfpdcAp   += dfpdciApi;
                dTopSumfpdcApYp += dfpdciApi*(dyposi+dgap);
            }
            else if(dyposi<0.0 && fabs(dyposi)<dgap)
            {
                dAstol += rTndn.dArea;
                dAsY   += rTndn.dArea*(dgap-fabs(dyposi));   
                dTtol  += rTndn.dStress[0];
                ddpc_max = max(ddpc_max, (dgap-fabs(dyposi)));
                // Add
                double dfpdtiApi = dfpdt*rTndn.dArea;
                dTopSumfpdtAp   += dfpdtiApi;
                dTopSumfpdtApYp += dfpdtiApi*(dgap-fabs(dyposi));
                double dfpdciApi = dfpdc*rTndn.dArea;
                dTopSumfpdcAp   += dfpdciApi;
                dTopSumfpdcApYp += dfpdciApi*(dgap-fabs(dyposi));
            }
        }
        else if(dNeuC<dCp)
        {
            dgap=dCp-dNeuC;
            if(dyposi>dgap)
            {
                dAstol += rTndn.dArea;
                dAsY += rTndn.dArea*(dyposi-dgap);   
                dTtol  += rTndn.dStress[0];
                ddpc_max = max(ddpc_max, (dyposi-dgap));
                // Add
                double dfpdtiApi = dfpdt*rTndn.dArea;
                dTopSumfpdtAp   += dfpdtiApi;
                dTopSumfpdtApYp += dfpdtiApi*(dyposi-dgap);
                double dfpdciApi = dfpdc*rTndn.dArea;
                dTopSumfpdcAp   += dfpdciApi;
                dTopSumfpdcApYp += dfpdciApi*(dyposi-dgap);
            }
        }
    }
    rTnRb.dApctol = dAstol;
    rTnRb.dNtodpc = (dAstol==0.0 ? 0.0 : dAsY/dAstol);
    rTnRb.ddpc    = (dAstol==0.0 ? 0.0 : dNeuC-rTnRb.dNtodpc);
    rTnRb.dTpctol = dTtol;
    rTnRb.ddpc_max = ddpc_max;

    //Tension Rebar.
    dAstol=0.0; dAsY=0.0;
    double dBotSumfsdAsYs=0.0, dBotSumfsdAs=0.0;
    for(int i=0 ; i<arRbarPosi.GetSize() ; ++i)
    {
        _DGN_RBAR_CRC rBar = arRbarPosi.GetAt(i);
        double dyposi = (bYaxis==TRUE ? rBar.dz : rBar.dy);
        if(dNeuC>=dCp && dyposi<0.0)
        {    
            dgap=dNeuC-dCp;
            if(fabs(dyposi)>dgap)
            {
                dAstol += rBar.dArea;
                dAsY   += rBar.dArea*(fabs(dyposi)-dgap);
                // Add
                double dfsdiAsi = dfsd*rBar.dArea;
                dBotSumfsdAs   += dfsdiAsi;
                dBotSumfsdAsYs += dfsdiAsi*(fabs(dyposi)-dgap);
            }
        }
        else if(dNeuC<dCp)
        {
            dgap=dCp-dNeuC;
            if(dyposi<0.0)
            {
                dAstol += rBar.dArea;
                dAsY += rBar.dArea*(dgap+fabs(dyposi));   
                // Add
                double dfsdiAsi = dfsd*rBar.dArea;
                dBotSumfsdAs   += dfsdiAsi;
                dBotSumfsdAsYs += dfsdiAsi*(dgap+fabs(dyposi));
            }
            else if(dyposi<dgap)
            {
                dAstol += rBar.dArea;
                dAsY += rBar.dArea*(dgap-fabs(dyposi));
                // Add
                double dfsdiAsi = dfsd*rBar.dArea;
                dBotSumfsdAs   += dfsdiAsi;
                dBotSumfsdAsYs += dfsdiAsi*(dgap+fabs(dyposi));
            }
        }
    }
    rTnRb.dAsttol = dAstol;
    rTnRb.dNtodst = (dAstol==0.0 ? 0.0 : dAsY/dAstol);
    rTnRb.ddst    = (dAstol==0.0 ? 0.0 : dNeuC+rTnRb.dNtodst);

    dAstol=0.0; dAsY=0.0;
    //Compression Rebar.
    double dTopSumfsdAsYs=0.0, dTopSumfsdAs=0.0;
    for(int i=0 ; i<arRbarPosi.GetSize() ; ++i)
    {
        _DGN_RBAR_CRC rBar = arRbarPosi.GetAt(i);
        double dyposi = (bYaxis==TRUE ? rBar.dz : rBar.dy);
        if(dNeuC>=dCp)
        {    
            dgap=dNeuC-dCp;
            if(dyposi>0.0)
            {
                dAstol += rBar.dArea;
                dAsY   += rBar.dArea*(dyposi+dgap);           
                // Add
                double dfsdiAsi = dfsd*rBar.dArea;
                dTopSumfsdAs   += dfsdiAsi;
                dTopSumfsdAsYs += dfsdiAsi*(dyposi+dgap);
            }
            else if(dyposi<0.0 && fabs(dyposi)<dgap)
            {
                dAstol += rBar.dArea;
                dAsY   += rBar.dArea*(dgap-fabs(dyposi));           
                // Add
                double dfsdiAsi = dfsd*rBar.dArea;
                dTopSumfsdAs   += dfsdiAsi;
                dTopSumfsdAsYs += dfsdiAsi*(dgap-fabs(dyposi));
            }
        }
        else if(dNeuC<dCp)
        {
            dgap=dCp-dNeuC;
            if(dyposi>dgap)
            {
                dAstol += rBar.dArea;
                dAsY += rBar.dArea*(fabs(dyposi)-dgap);           
                // Add
                double dfsdiAsi = dfsd*rBar.dArea;
                dTopSumfsdAs   += dfsdiAsi;
                dTopSumfsdAsYs += dfsdiAsi*(fabs(dyposi)-dgap);
            }
        }
    }
    rTnRb.dAsctol = dAstol;
    rTnRb.dNtodsc = (dAstol==0.0 ? 0.0 : dAsY/dAstol);
    rTnRb.ddsc    = (dAstol==0.0 ? 0.0 : dNeuC-rTnRb.dNtodsc);

    // Add
    double dAreaBotTen  = dBotSumfpdtAp + dBotSumfsdAs; // fsd는 압축,인장값이 동일.
    double dAreaYBotTen = dBotSumfpdtApYp + dBotSumfsdAsYs;
    //
    double dAreaBotCon  = dBotSumfpdcAp + dBotSumfsdAs;
    double dAreaYBotCon = dBotSumfpdcApYp + dBotSumfsdAsYs;
    //
    double dAreaTopTen  = dTopSumfpdtAp + dTopSumfsdAs;
    double dAreaYTopTen = dTopSumfpdtApYp + dTopSumfsdAsYs;
    //
    double dAreaTopCon  = dTopSumfpdcAp + dTopSumfsdAs;
    double dAreaYTopCon = dTopSumfpdcApYp + dTopSumfsdAsYs;

    double dNtoBotYt = (dAreaBotTen==0.0 ?  0.0 : dAreaYBotTen/dAreaBotTen);
    double dNtoBotYc = (dAreaBotCon==0.0 ?  0.0 : dAreaYBotCon/dAreaBotCon);
    double dNtoTopYt = (dAreaTopTen==0.0 ?  0.0 : dAreaYTopTen/dAreaTopTen);
    double dNtoTopYc = (dAreaTopCon==0.0 ?  0.0 : dAreaYTopCon/dAreaTopCon);

    rTnRb.daBott = (dAreaBotTen==0.0 ? 0.0 : dzDim-dNeuC-dNtoBotYt);
    rTnRb.daBotc = (dAreaBotCon==0.0 ? 0.0 : dzDim-dNeuC-dNtoBotYc);
    rTnRb.daTopt = (dAreaTopTen==0.0 ? 0.0 : dNeuC - dNtoTopYt);
    rTnRb.daTopc = (dAreaTopCon==0.0 ? 0.0 : dNeuC - dNtoTopYc);

    return TRUE;

}

BOOL CDgnPscCommon::Get_TndnReBarTransform(BOOL bYaxis, double dMu, double dNueC, double dCp, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
                                           CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& rarRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& rarTndnPosi)                                            
{
    rarRbarPosi.RemoveAll();
    rarTndnPosi.RemoveAll();
    int i=0;
    // Reinforcement.
    for(i=0 ; i<arRbarPosi.GetSize(); ++i)
    {
        _DGN_RBAR_CRC rRbar = arRbarPosi.GetAt(i);
        _DGN_RBAR_CRC rTnfrC; rTnfrC.Initialize();
        rTnfrC = rRbar;
        double dgap =  (dMu>=0.0 ? dNueC-dCp : dCp-dNueC); 
        if(bYaxis) rTnfrC.dz = rRbar.dz+dgap;
        else       rTnfrC.dy = rRbar.dy+dgap;
        rarRbarPosi.Add(rTnfrC);
    }

    // Tendon.
    for(i=0 ; i<arTndnPosi.GetSize(); ++i)
    {
        _DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
        _DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
        rTnfrC = rTndn;
        double dgap =  (dMu>=0.0 ? dNueC-dCp : dCp-dNueC); 
        if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
        else       rTnfrC.dy = rTndn.dy+dgap;
        rarTndnPosi.Add(rTnfrC);
    }  

    return TRUE;
}

BOOL CDgnPscCommon::Get_TndnReBarTransform(BOOL bYaxis, BOOL bPositive, double dNueC, double dCp, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
                                           CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& rarRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& rarTndnPosi)
{
    rarRbarPosi.RemoveAll();
    rarTndnPosi.RemoveAll();
    int i=0;
    // Reinforcement.
    for(i=0 ; i<arRbarPosi.GetSize(); ++i)
    {
        _DGN_RBAR_CRC rRbar = arRbarPosi.GetAt(i);
        _DGN_RBAR_CRC rTnfrC; rTnfrC.Initialize();
        rTnfrC = rRbar;
        double dgap =  (bPositive ? dNueC-dCp : dCp-dNueC); 
        if(bYaxis) rTnfrC.dz = rRbar.dz+dgap;
        else       rTnfrC.dy = rRbar.dy+dgap;
        rarRbarPosi.Add(rTnfrC);
    }

    // Tendon.
    for(i=0 ; i<arTndnPosi.GetSize(); ++i)
    {
        _DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
        _DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
        rTnfrC = rTndn;
        double dgap =  (bPositive ? dNueC-dCp : dCp-dNueC); 
        if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
        else       rTnfrC.dy = rTndn.dy+dgap;
        rarTndnPosi.Add(rTnfrC);
    }  

    return TRUE;
}

BOOL CDgnPscCommon::Get_TndnTheta(double dStreX, double dStreY, double& dSinT, double& dThetaR, double& dThetaD)
{
    double dPi = 4*atan(1.0);
    double dStreXY = sqrt(dStreX*dStreX + dStreY*dStreY);
    dSinT   = (dStreXY==0.0 ? 0.0 : dStreY/dStreXY);
    dThetaR  = asin(dSinT);
    dThetaD  = dThetaR*(180.0/dPi);

    return TRUE;
}

BOOL CDgnPscCommon::Get_TndnKindrp(int iKind, double& drp)
{
    // PS강재의 종류(0=저릴랙세이션(0.28), 1=응력제거강재(0.4), 2=강봉(0.55))
    if(iKind==0) { drp = 0.28; }
    else if(iKind==1) { drp = 0.40; }
    else if(iKind==2) { drp = 0.55; }
    else { ASSERT(0); drp = 0.28; }

    return TRUE;
}

BOOL CDgnPscCommon::Get_AASHTOTndnValueOfk(int iKind, double& dk)
{
    // AASHTO. 5-33, Table C5.7.3.1.1-1 Values of k
    // Low relaxtion strand. (0.28)
    // Stress-relieved strand and type 1 high-strength bar (0.38)
    // Type 2 high-strength bar (0.48)
    if(iKind==0) { dk = 0.28; }
    else if(iKind==1) { dk = 0.38; }
    else if(iKind==2) { dk = 0.48; }
    else { ASSERT(0); dk = 0.28; }

    return TRUE;
}

// BOOL CDgnPscCommon::Get_Torsion_Pcp_Acp(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, double& dPcp, double& dAcp)
// {
//   T_SPSC_COOR aCoor = arOuter[0];
//   UINT iNumPoint = arOuter[0].arSpscPont[0].GetSize();
//   double dxC=0.0, dyC=0.0;
//   double* dpx = new double[iNumPoint];
// 	double* dpy = new double[iNumPoint];
//   //Pcp
//   for(int i=0; i<iNumPoint; ++i)
//   {
//     int iPrev = i;
// 		int iNext = (i<iNumPoint-1 ? i+1 : 0);
//     double dy1 = aCoor.arSpscPont[0].GetAt(iPrev);
//     double dz1 = aCoor.arSpscPont[1].GetAt(iPrev);
//     double dy2 = aCoor.arSpscPont[0].GetAt(iNext);
//     double dz2 = aCoor.arSpscPont[1].GetAt(iNext);			
//     dPcp += sqrt(pow((dy1-dy2),2)+pow((dz1-dz2),2));
// 
//     dpx[i] = aCoor.arSpscPont[0].GetAt(i);
// 	  dpy[i] = aCoor.arSpscPont[1].GetAt(i);
//   }
//   //Acp  
// 	if(!CMathFunc::mathPolyCentroid(iNumPoint,dpx,dpy,dxC,dyC,dAcp))	ASSERT(0);
//   delete [] dpx; delete [] dpy;
//   
//   return TRUE;
//   
// }

BOOL CDgnPscCommon::Get_PrincipalExtraRatio(ElemPairK ElemK, double* dAlwStress)
{
    T_PSCD_D PscdD;
    PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);	
    if(PscdD.iDgnCode!=JSCE02)	return FALSE;

    // 0=전사하중시(전단), 1=전사하중시(전단+비틀림)
    // 2=기타하중시(전단), 3=기타하주시(전단+비틀림) 
    if(!m_pDoc->IsPostMode())	ASSERT(0);
    T_MATD_D MatdD;
    MatdD.Initialize();
    int nMatlK = m_pDataCtrl->GetMaterialKey4ElemPairK(ElemK);
    // Get Matl.
    if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(nMatlK,MatdD))	{ASSERT(0); return FALSE;}
    double dfc = MatdD.Data1.Design.C_fc;

    // Get fc (Current -> Code).
    T_UNIT_INDEX CurrUnit, CodeUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    CodeUnit.nBase_Force	= D_UNITSYS_FORCE_INDEX_N;
    CodeUnit.nBase_Length	= D_UNITSYS_LENGTH_INDEX_MM;
    dfc = Cng_StressUnit(CurrUnit, CodeUnit,dfc);	

    if(dfc<30.0+m_dZero) 
    {
        if(30.0-m_dZero<dfc && dfc<30.+m_dZero) { dAlwStress[0]=0.8, dAlwStress[1]=1.1, dAlwStress[2]=1.7, dAlwStress[3]=2.2;}
        else
        {
            dAlwStress[0] = (1.0-0.8)/10.0 * dfc + (0.8 - (1.0-0.8)/10.0*30.0);
            dAlwStress[1] = (1.3-1.1)/10.0 * dfc + (1.1 - (1.3-1.1)/10.0*30.0);
            dAlwStress[2] = (2.0-1.7)/10.0 * dfc + (1.7 - (2.0-1.7)/10.0*30.0);
            dAlwStress[3] = (2.5-2.2)/10.0 * dfc + (2.2 - (2.5-2.2)/10.0*30.0);
        }
    }
    else if(30.0<dfc && dfc<40.+m_dZero) 
    {
        if(40.0-m_dZero<dfc && dfc<40.+m_dZero) { dAlwStress[0]=1.0, dAlwStress[1]=1.3, dAlwStress[2]=2.0, dAlwStress[3]=2.5; }
        else
        {
            dAlwStress[0] = 0.8 + (dfc-30.0)*(1.0-0.8)/10.0;
            dAlwStress[1] = 1.1 + (dfc-30.0)*(1.3-1.1)/10.0;
            dAlwStress[2] = 1.7 + (dfc-30.0)*(2.0-1.7)/10.0;
            dAlwStress[3] = 2.2 + (dfc-30.0)*(2.5-2.2)/10.0;
        }
    }
    else if(40.0<dfc && dfc<50.+m_dZero) 
    {
        if(50.0-m_dZero<dfc && dfc<50.+m_dZero) { dAlwStress[0]=1.2, dAlwStress[1]=1.5, dAlwStress[2]=2.3, dAlwStress[3]=2.8; }
        else
        {
            dAlwStress[0] = 1.0 + (dfc-40.0)*(1.2-1.0)/10.0;
            dAlwStress[1] = 1.3 + (dfc-40.0)*(1.5-1.3)/10.0;
            dAlwStress[2] = 2.0 + (dfc-40.0)*(2.3-2.0)/10.0;
            dAlwStress[3] = 2.5 + (dfc-40.0)*(2.8-2.5)/10.0;
        }
    }
    else if(50.0<dfc && dfc<60.0+m_dZero) 
    {
        if(60.0-m_dZero<dfc && dfc<60.+m_dZero) { dAlwStress[0]=1.3, dAlwStress[1]=1.6, dAlwStress[2]=2.5, dAlwStress[3]=3.0; }
        else
        {
            dAlwStress[0] = 1.2 + (dfc-40.0)*(1.3-1.2)/10.0;
            dAlwStress[1] = 1.5 + (dfc-40.0)*(1.6-1.5)/10.0;
            dAlwStress[2] = 2.3 + (dfc-40.0)*(2.5-2.3)/10.0;
            dAlwStress[3] = 2.8 + (dfc-40.0)*(3.0-2.8)/10.0;
        }
    }
    else if(dfc>60.0)
    {
        dAlwStress[0] = (1.3-1.2)/10.0 * dfc + (1.2 - (1.3-1.2)/10.0*50.0);
        dAlwStress[1] = (1.6-1.5)/10.0 * dfc + (1.5 - (1.6-1.5)/10.0*50.0);
        dAlwStress[2] = (2.5-2.3)/10.0 * dfc + (2.3 - (2.5-2.3)/10.0*50.0);
        dAlwStress[3] = (3.0-2.8)/10.0 * dfc + (2.8 - (3.0-2.8)/10.0*50.0);
    }
    else ASSERT(0);

    return TRUE;
}

BOOL CDgnPscCommon::CalcStiffnessOfStiffener(T_SECT_STIFF_D &Stfn, ST_SECT_STIFF_D &rData)
{
    int nStfnShapeSize = Stfn.aStiffShape.GetSize();
    int nConvStfnSize  = rData.aStiffShape.GetSize();
    if (nStfnShapeSize<1) return FALSE;
    if (nStfnShapeSize != nConvStfnSize) return FALSE;

    //arData.SetSize(nStfnShapeSize);
    T_SECT_D CalcS; CalcS.Initialize();
    for (int i=0; i<nStfnShapeSize; ++i)
    {
        T_STIFF_SHAPE_D &StfnD = Stfn.aStiffShape[i];

        T_SECT_STIFFNESS &StfnStiff = CalcS.SectBefore.SectI.Stiffness;
        CalcS.nStype = D_SECT_TYPE_REGULAR;
        if      (StfnD.nType==0) CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_SB;
        else if (StfnD.nType==1) CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_T;
        else if (StfnD.nType==2) CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_URIB;
        else ASSERT(0);
        for (int j=0; j<8; ++j) CalcS.SectBefore.SectI.Size[j] = StfnD.dSize[j];

        CSectUtil::CalcSectData(CalcS);  
        ConvertSectStiffnessData(StfnStiff, rData.aStiffShape[i].Stiffness);

    }

    return TRUE;
}

BOOL CDgnPscCommon::GetElementList4TendonLp(ArrElemPairKey& aElemKey)
{
    std::map<ElemPairK,ElemPairK> mElemLp;
    POSITION pos = m_amTndnTran.GetStartPosition();
    while ( pos )
    {
        T_TDNA_K TKey=0;
        _DGN_TNDN_TRAN TData;
        m_amTndnTran.GetNextAssoc(pos, TKey, TData);
        for ( int i=0; i<2; ++i )
        {
            ElemPairK ElemK = TData.TranB[i].ElemLpK;
            T_ELEM_K TKey = 0;
            if (CDBLib::IsPscDgnFor1st(ElemK, TRUE))
            {
                mElemLp[ElemK] = ElemK;
            }
        }
    }
	ConvertMapToSortedArray(mElemLp, aElemKey);
    return TRUE;
}

BOOL CDgnPscCommon::GetCheckElemListAppTendonLp(const ArrElemPairKey& aChkElemKey, ArrElemPairKey& aElemKey)
{
	switch (m_iCode)
	{
	case AS_5100_5_17_PSC:
	case AASHTO_LRFD08:
	case AASHTO_LRFD12:
	case AASHTO_LRFD14:
	case AASHTO_LRFD16:
	case AASHTO_LRFD17:
	case AASHTO_LRFD20:
    case AASHTO_LRFD24:
    case AREMA_23:
		break;
	default:
		return FALSE;
	}
    aElemKey.RemoveAll();

    std::map<ElemPairK, ElemPairK> mChkElemList;
    for ( INT_PTR i=0; i<aChkElemKey.GetSize(); ++i )
    {
        ElemPairK ElemK = aChkElemKey[i];
        mChkElemList.insert({ ElemK, ElemK });
    }

    for ( const auto& CurElem : m_amElemLp)
    {
        ElemPairK ElemK = CurElem.first;

        if (CDBLib::IsPscDgnFor1st(ElemK, TRUE))
        {
            const auto& itChk = mChkElemList.find(ElemK);
            if (itChk == mChkElemList.end())
            {
                aElemKey.Add(ElemK);
			}
        }
	}

    return TRUE;
}

void CDgnPscCommon::ConvertMapToSortedArray(const std::map<ElemPairK, ElemPairK>& mElemPairK, ArrElemPairKey& outArr)
{
    outArr.RemoveAll();

    for (const auto& CurElem : mElemPairK)
    {
        outArr.Add(CurElem.first);
    }

    if (outArr.GetSize() <= 1) return;

    std::sort(
        outArr.GetData(),
        outArr.GetData() + outArr.GetSize(),
        [](const ElemPairK& a, const ElemPairK& b)
        {
            if (a.first != b.first)
                return a.first < b.first;
            return a.second < b.second;
        }
    );
}

double CDgnPscCommon::Cng_LengthUnit(T_UNIT_INDEX FromUnit, T_UNIT_INDEX ToUnit, double dLength, int iUp)
{
    double dToLen=0.0;
    switch(ToUnit.nBase_Length)
    {
    case D_UNITSYS_LENGTH_INDEX_M:	{dToLen=1.0;		break;}
    case D_UNITSYS_LENGTH_INDEX_CM:	{dToLen=0.01;		break;}
    case D_UNITSYS_LENGTH_INDEX_MM:	{dToLen=0.001;	break;}
    case D_UNITSYS_LENGTH_INDEX_IN:	{dToLen=0.0254;	break;}
    case D_UNITSYS_LENGTH_INDEX_FT:	{dToLen=0.3048;	break;}
    default:	ASSERT(0);
    }
    double dFromLen=0.0;
    switch(FromUnit.nBase_Length)
    {
    case D_UNITSYS_LENGTH_INDEX_M:	{dFromLen=1.0;		break;}
    case D_UNITSYS_LENGTH_INDEX_CM:	{dFromLen=0.01;		break;}
    case D_UNITSYS_LENGTH_INDEX_MM:	{dFromLen=0.001;	break;}
    case D_UNITSYS_LENGTH_INDEX_IN:	{dFromLen=0.0254;	break;}
    case D_UNITSYS_LENGTH_INDEX_FT:	{dFromLen=0.3048;	break;}
    default:	ASSERT(0);
    }
    return dLength * pow(dFromLen/dToLen, iUp);
}

double CDgnPscCommon::Cng_ForceUnit(T_UNIT_INDEX FromUnit, T_UNIT_INDEX ToUnit, double dForce)
{
    double dToFor=0.0;
    switch(ToUnit.nBase_Force)
    {
    case D_UNITSYS_FORCE_INDEX_KG:	{dToFor=9.80665;	break;}
    case D_UNITSYS_FORCE_INDEX_TON:	{dToFor=9806.65;	break;}
    case D_UNITSYS_FORCE_INDEX_N:		{dToFor=1.0;			break;}
    case D_UNITSYS_FORCE_INDEX_KN:	{dToFor=1000.0;		break;}
    case D_UNITSYS_FORCE_INDEX_LBF:	{dToFor=4.448223;	break;}
    case D_UNITSYS_FORCE_INDEX_KIP:	{dToFor=4448.223;	break;}
    default:	ASSERT(0);
    }
    double dFromFor=0.0;
    switch(FromUnit.nBase_Force)
    {
    case D_UNITSYS_FORCE_INDEX_KG:	{dFromFor=9.80665;	break;}
    case D_UNITSYS_FORCE_INDEX_TON:	{dFromFor=9806.65;	break;}
    case D_UNITSYS_FORCE_INDEX_N:		{dFromFor=1.0;			break;}
    case D_UNITSYS_FORCE_INDEX_KN:	{dFromFor=1000.0;		break;}
    case D_UNITSYS_FORCE_INDEX_LBF:	{dFromFor=4.448223;	break;}
    case D_UNITSYS_FORCE_INDEX_KIP:	{dFromFor=4448.223;	break;}
    default:	ASSERT(0);
    }
    return dForce * (dFromFor/dToFor);
}

double CDgnPscCommon::Cng_StressUnit(T_UNIT_INDEX FromUnit, T_UNIT_INDEX ToUnit, double dStress)
{
    double dLength=1.0;
    dLength = Cng_LengthUnit(FromUnit,ToUnit,dLength,1);
    double dForce=1.0;
    dForce = Cng_ForceUnit(FromUnit,ToUnit,dForce);
    return dStress * (dForce/pow(dLength,2));
}

BOOL CDgnPscCommon::IsTndnGroupCheck(int nDgnCode)
{
    switch (nDgnCode)
    {
    case AASHTO_LRFD08:
    case AASHTO_LRFD12:
    case AASHTO_LRFD14:
    case AASHTO_LRFD16:
    case AASHTO_LRFD17:
    case AASHTO_LRFD20:
    case AASHTO_LRFD24:
    case AREMA_23:
    case CSA_S6_10:
    case CSA_S6_14:
	case CSA_S6_19:
    case SNiP_20503_84_PSC:
    case SP_35_13330_11_PSC:
    case SNiP_20503_84_PSC_MKS:
    case SP_35_13330_11_PSC_MKS:
    case AS_5100_5_17_PSC:
    case BS5400_90_PSC:
	case TMH07_3_1989:
    case BD_44_15_PSC:
    case CS_455_PSC:
	case EUROCODE2_2_05_PSC:
    case IRC_112_2020_PSC:
    case ABNT_NBR_6118_2023_PSC:
		return FALSE;
    default: 
        return TRUE;
    }
    return TRUE;
}

CString CDgnPscCommon::GetRCMatlCodeName(int nDgnCode)
{
    CString strMatlCodeName = _T("");
    switch (nDgnCode)
    {
    case KDS_24_14_31_2018_CSG:      
    case KSCE_LSD15_CSG:      
    case KSSC_2014_CSG:       
        {
            strMatlCodeName = MATLCODE_CON_KSCE_LSD15;
        }
        break;
    case AASHTO_LRFD20_CSG:
        {
            strMatlCodeName = MATLCODE_CON_ASTM19_CIVIL;
        }
        break;
    case AASHTO_LRFD17_CSG:
    case AASHTO_LRFD16_CSG:
    case AASHTO_LRFD12_CSG:
    case AASHTO_LRFD07_CSG:
        {
            strMatlCodeName = MATLCODE_CON_ASTM;
        }
        break;
    case KSCE_ASD10_CSG:
    case KSCE_RAIL_ASD11_CSG:
        {
            strMatlCodeName = MATLCODE_CON_KS01_CIVIL;
        }
        break;
    case SNiP_20503_84_CSG:
        {
            strMatlCodeName = MATLCODE_CON_GOST_SNIP;
        }
        break;
    case SP_35_13330_11_CSG:
        {
            strMatlCodeName = MATLCODE_CON_GOST_SP;
        }
        break;
    case CSA_S6_10_CSG:
    case CSA_S6_14_CSG:
    case CSA_S6_19_CSG:
        {
            strMatlCodeName = MATLCODE_CON_CSA;
        }
        break;
    case CS457_R1_CSG:
        {
            strMatlCodeName = MATLCODE_CON_BS;
        }
        break;
    case NR_GN_CIV_025_06_CSG:
    {
        strMatlCodeName = MATLCODE_CON_BS;
    }
    break;
    case AS5100_6_17_CSG:
		{
			strMatlCodeName = MATLCODE_CON_AS17;
		}
		break;
	default: ASSERT(0); break;
    }

    return strMatlCodeName;
}

BOOL CDgnPscCommon::Get_Unit4CalcEc(int nDgnCode, T_UNIT_INDEX& UnitIndex)
{    
    CString strCode = GetRCMatlCodeName(nDgnCode);
    if ( strCode == MATLCODE_CON_KSCE_LSD15 ) 
    {
        UnitIndex.nBase_Length=D_UNITSYS_LENGTH_INDEX_MM;
        UnitIndex.nBase_Force =D_UNITSYS_FORCE_INDEX_N;        
    }
    else if ( strCode == MATLCODE_CON_ASTM || strCode == MATLCODE_CON_ASTM19 )
    {
        UnitIndex.nBase_Length=D_UNITSYS_LENGTH_INDEX_IN;
        UnitIndex.nBase_Force =D_UNITSYS_FORCE_INDEX_LBF;
    }
    else if ( strCode == MATLCODE_CON_ASTM19_CIVIL )
    {
        UnitIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
        UnitIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_KIP;
    }
    else if ( strCode == MATLCODE_CON_KS01_CIVIL )
    {
        UnitIndex.nBase_Length=D_UNITSYS_LENGTH_INDEX_MM;
        UnitIndex.nBase_Force =D_UNITSYS_FORCE_INDEX_N;
    }
    else if ( strCode == MATLCODE_CON_GOST_SNIP )
    {
        UnitIndex.nBase_Length=D_UNITSYS_LENGTH_INDEX_MM;
        UnitIndex.nBase_Force =D_UNITSYS_FORCE_INDEX_N;

    }
    else if ( strCode == MATLCODE_CON_GOST_SP )
    {
        UnitIndex.nBase_Length=D_UNITSYS_LENGTH_INDEX_MM;
        UnitIndex.nBase_Force =D_UNITSYS_FORCE_INDEX_N;

    }
    else if ( strCode == MATLCODE_CON_CSA )
    {
        UnitIndex.nBase_Length=D_UNITSYS_LENGTH_INDEX_MM;
        UnitIndex.nBase_Force =D_UNITSYS_FORCE_INDEX_N;
    }
    else if ( strCode == MATLCODE_CON_BS )
    {
        UnitIndex.nBase_Length=D_UNITSYS_LENGTH_INDEX_MM;
        UnitIndex.nBase_Force =D_UNITSYS_FORCE_INDEX_N;
    }
	else if ( strCode == MATLCODE_CON_AS17 )
	{
		UnitIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		UnitIndex.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
	}
	else
	{
		UnitIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		UnitIndex.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
		ASSERT(0);
	}

    return TRUE;
}

double CDgnPscCommon::Calc_Ec(int nDgnCode, double dfc)
{
    CCurUnitSaver Save(TRUE);

    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();

    T_UNIT_INDEX UnitD = CodeUnit; // Heat, Temper는 그대로 사용하기 위해.
    Get_Unit4CalcEc(nDgnCode, UnitD);

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitD);

    double dfc_c = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(CodeUnit.nBase_Force, CodeUnit.nBase_Length, D_UNITSYS_BASE_STRESS, dfc);

    CMatlDB MatlDB(m_pDoc);
    
    CString strSlabCodeName = GetRCMatlCodeName(nDgnCode);
    double dEc = MatlDB.Get_Ec(strSlabCodeName, _T(""), dfc_c);

    dEc = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(CodeUnit.nBase_Force, CodeUnit.nBase_Length, D_UNITSYS_BASE_STRESS, dEc);

    return dEc;
}

void CDgnPscCommon::ConvertPscParamData(const T_PSCD_D& PscdD, S_PSCD_D& rData)
{
    rData.Initialize();
    for(int i=0; i<4; ++i) rData.bBckPrint[i]  = PscdD.bBckPrint[i];
    rData.iDimType   = PscdD.iDimType;
    rData.iBrgType   = PscdD.iBrgType;
    rData.iCrackCode = PscdD.iCrackCode;
    for (int i = 0; i < 2; ++i) rData.iCrackWidth[i] = PscdD.iCrackWidth[i];
    rData.iPsReduFactor  = PscdD.iPsReduFactor;
    for (int i = 0; i < 5; ++i) rData.bDgnPrint[i]    = PscdD.bDgnPrint[i]   ;
    for (int i = 0; i < 5; ++i) rData.bStructPrint[i] = PscdD.bStructPrint[i];
    for (int i = 0; i < 8; ++i) rData.bMomPrint[i]    = PscdD.bMomPrint[i]   ;  
    for (int i = 0; i < 5; ++i) rData.bShrPrint[i]    = PscdD.bShrPrint[i];  
    for (int i = 0; i < 4; ++i) rData.bTorPrint[i]    = PscdD.bTorPrint[i];
    for (int i = 0; i < 2; ++i) rData.bEtcPrint[i]    = PscdD.bEtcPrint[i];  
    for (int i = 0; i < 4; ++i) rData.bBckPrint[i]    = PscdD.bBckPrint[i];

    rData.iDgnCode   = GetConvertCodeCvlToEng(PscdD.iDgnCode); 
    rData.iSafeType  = PscdD.iSafeType ;
    rData.iCastType  = PscdD.iCastType ;
    if( CDBLib::IsPscCodeLSD(m_iCode) || PscdD.iDgnCode==BS5400_90_PSC|| PscdD.iDgnCode == TMH07_3_1989|| PscdD.iDgnCode==IRC_112_2011_PSC ||
       PscdD.iDgnCode==AS_5100_5_17_PSC   || PscdD.iDgnCode==IRS_PSC       || PscdD.iDgnCode==BD_44_15_PSC || PscdD.iDgnCode==CS_455_PSC ||
       m_iCode == IRC_112_2020_PSC)
    {
        rData.ifpsMethod = 1;
    }
    else   
    {
        rData.ifpsMethod = PscdD.ifpsMethod;
    }


    if(PscdD.iDgnCode==EUROCODE2_2_05_PSC || PscdD.iDgnCode==IRC_112_2011_PSC || m_iCode==IRS_PSC || m_iCode == IRC_112_2020_PSC ||
        PscdD.iDgnCode == ABNT_NBR_6118_2023_PSC)
        rData.iTndnKind  = PscdD.nPreStressStl ;
    else                                   rData.iTndnKind  = PscdD.iTndnKind ;

    rData.bRefRebar  = PscdD.bRefRebar ;
    rData.bCmpWeb    = PscdD.bCmpWeb   ;
    rData.iBrgDir    = PscdD.iBrgDir   ;
    for (int i=0 ; i<14 ; ++i) rData.bBeamPrintCH[i] = PscdD.bBeamPrintCH[i];
    for (int i=0 ; i<10 ; ++i) rData.bColumnPrintCH[i] = PscdD.bColumnPrintCH[i];
    rData.iConstructionType   = PscdD.iConstructionType;
    rData.iCorrosiveCondition = PscdD.iCorrosiveCondition;
    rData.iExposureType       = PscdD.iExposureType;
    rData.dExposureFactor     = PscdD.dExposureFactor;

    rData.nNationalAnnex  = PscdD.nNationalAnnex ;
    rData.iTndnCalcMethod = PscdD.iTndnCalcMethod;
    rData.iExposureClass  = PscdD.iExposureClass ;

    rData.iShearMethod    = PscdD.iShearMethod;
    rData.enPL = PscdD.enPL;
    rData.enSL = PscdD.enSL;
    rData.enSType = PscdD.enSType;
    rData.enBType = PscdD.enBType;
    rData.dkc = PscdD.dkc;
    rData.bUserInputData  = PscdD.bUserInputData ;   
    rData.dGamma_c_PT     = PscdD.dGamma_c_PT    ;
    rData.dGamma_c_A      = PscdD.dGamma_c_A     ; 
    rData.dGamma_s_PT     = PscdD.dGamma_s_PT    ;
    rData.dGamma_s_A      = PscdD.dGamma_s_A     ; 
    rData.dGamma_ps_PT    = PscdD.dGamma_ps_PT   ;
    rData.dGamma_ps_A     = PscdD.dGamma_ps_A    ;
    rData.dGamma_c_serv   = PscdD.dGamma_c_serv  ;
    rData.dGamma_s_serv   = PscdD.dGamma_s_serv  ;    
    rData.dAlpha_cc       = PscdD.dAlpha_cc;
    rData.dAlphacc4Shear  = PscdD.dAlphacc4Shear;
    rData.dAlpha_ct       = PscdD.dAlpha_ct      ;
    rData.dtheta          = PscdD.dtheta         ;  
    rData.dk1_ps          = PscdD.dk1_ps         ;
    rData.dk2_ps          = PscdD.dk2_ps         ;
    rData.dk5_ps          = PscdD.dk5_ps         ;
    rData.dk7_ps          = PscdD.dk7_ps         ;
    rData.dk8_ps          = PscdD.dk8_ps         ;
    rData.dk1_sl          = PscdD.dk1_sl         ;
    rData.dk3_sl          = PscdD.dk3_sl         ;
    rData.dk4_sl          = PscdD.dk4_sl         ;
    rData.dk6_sl          = PscdD.dk6_sl         ;
    rData.dk_c_ps_stage   = PscdD.dk_c_ps_stage  ;
    rData.dk_t_ps_stage   = PscdD.dk_t_ps_stage  ;
    rData.dk_c_ps         = PscdD.dk_c_ps        ;
    rData.dk_t_ps         = PscdD.dk_t_ps        ;
    rData.dk3_cr          = PscdD.dk3_cr         ;
    rData.dk4_cr          = PscdD.dk4_cr         ;
    for (int i = 0; i < 3; ++i) rData.bUltimateEuro[i] = PscdD.bUltimateEuro[i];
    for (int i = 0; i < 6; ++i) rData.bServiceabilityEuro[i] = PscdD.bServiceabilityEuro[i];

    rData.bExistSpecialLoad  = PscdD.bExistSpecialLoad;
    for (int i = 0; i < 4; ++i) rData.dPhi_c[i]  = PscdD.dPhi_c[i] ;
    for (int i = 0; i < 4; ++i) rData.dPhi_s[i]  = PscdD.dPhi_s[i] ;
    for (int i = 0; i < 4; ++i) rData.dPhi_ps[i] = PscdD.dPhi_ps[i];

    rData.bmb7mb8     = PscdD.bmb7mb8    ;	  
    rData.nmb7mb8Type = PscdD.nmb7mb8Type;
    rData.dmb7        = PscdD.dmb7       ;
    rData.dmb8        = PscdD.dmb8       ;             
    rData.bmb9        = PscdD.bmb9       ;             
    rData.nmb9Type    = PscdD.nmb9Type   ;         
    rData.dmb9        = PscdD.dmb9       ;

    switch ( PscdD.iDgnCode )
    {
    case KSCE_LSD15_PSC:
    case KDS_24_14_21_2022_PSC:
        {
            rData.nBetaCalType = PscdD.nBetaCalType;
            rData.nCalcCcType = PscdD.nCalcCcType;
        }
        break;
    case EUROCODE2_2_05_PSC:
        {
            rData.nBetaCalType= PscdD.nBetaCalType;
            rData.nCalcCcType =0;
            if ( PscdD.bUserInputData == false )
            {
                rData.dAlpha_cc =  Get_Alpha_cc_4EuroCode(PscdD.nNationalAnnex);
                rData.dAlpha_ct =  Get_Alpha_ct_4EuroCode(PscdD.nNationalAnnex);
            }
        }
        break;
    case ABNT_NBR_6118_2023_PSC:
        {
            rData.nBetaCalType= PscdD.nBetaCalType;
            rData.nCalcCcType =0;
            if ( PscdD.bUserInputData == false )
            {
                rData.dAlpha_cc =  Get_Alpha_cc_4BRA();
                rData.dAlpha_ct =  Get_Alpha_ct_4BRA();
            }
            else
            {
                rData.dAlpha_cc = 1.0;
				rData.dAlpha_ct = 1.0;
            }
            rData.dGamma_c_PT  = PscdD.dGamma_c_N_NBR;
            rData.dGamma_c_A   = PscdD.dGamma_c_SC_NBR;
            rData.dGamma_s_PT  = PscdD.dGamma_s_N_NBR;
            rData.dGamma_s_A   = PscdD.dGamma_s_SC_NBR;
            rData.dGamma_ps_PT = PscdD.dGamma_s_N_NBR;
            rData.dGamma_ps_A  = PscdD.dGamma_s_SC_NBR;
            rData.dGamma_c_serv = 1.0;
			rData.dGamma_s_serv = 1.0;

        }
        break;
    case AS_5100_5_17_PSC:
        {
            rData.nBetaCalType= PscdD.nBetaCalType;
            rData.nCalcCcType =0;
            rData.dAggregateSize = PscdD.dAggregateSize;
            rData.dfs_alw        = PscdD.dMaxStlStress;
        }
        break;
	case TMH07_3_1989:
    case BS5400_90_PSC:
        {
            rData.dPS_Comp    = PscdD.dPS_Comp;
            rData.dPS_Tens    = PscdD.dPS_Tens;
            rData.dPS_CSComp  = PscdD.dPS_CSComp;
            rData.dPS_CSTens  = PscdD.dPS_CSTens;
            rData.bIncrevtu   = PscdD.bIncrevtu;
            rData.dTsPreTens  = PscdD.dTsPreTens;
            rData.dTsPostTens = PscdD.dTsPostTens;

            rData.dGamma_c_PT       = PscdD.dCsConcrete;
            rData.dGamma_s_PT       = PscdD.dCsTndn;
            rData.dGamma_mc_c_s     = PscdD.dSfTriCmp;
            rData.dGamma_mc_ca_s    = PscdD.dSfUniformCmp;
            rData.dGamma_mc_t_pre   = PscdD.dSfPreTens;
            rData.dGamma_mc_t_post  = PscdD.dSfPostTens;
        }
        break;
    case AASHTO_LRFD17:
        {
            switch (PscdD.nNationalAnnex)
            {
			case enAAshtoAmendments::Amend_Recommended:
			    {
                    rData.nNationalAnnex = enUnitedStates::Spec;
                }
			    break;
			case enAAshtoAmendments::Amend_California:
			    {
                    rData.nNationalAnnex = enUnitedStates::California;
                }
			    break;
			default: ASSERT(0);
                break;            	
            }            
        }
        break;
    default:
        {
            rData.nBetaCalType= 0;
            rData.nCalcCcType =0;
        }
        break;
    }

    rData.dAggregateSize = PscdD.dAggregateSize;
}

void CDgnPscCommon::ConvertPscParamASPM(const T_ASPM_D& AspmD, S_PSCD_D& rData)
{
    rData.bUserInputData = AspmD.bUserInput;
    rData.nMatStrnType   = AspmD.nGamma_m;
    rData.dmb9           = AspmD.dConditionFactor; // dmb9에 할당하자.
    rData.bAppVp         = AspmD.bOptionShr;
    const T_ASPM_FT_D &FacD = AspmD.Factors;
    
    rData.dGamma_s_PT = FacD.dCsTndn;
    rData.dGamma_s_A  = FacD.dWsTndn;
    rData.dGamma_c_PT = FacD.dCsConcrete;
    rData.dGamma_c_A  = FacD.dWsConcrete;
    rData.dGamma_ps_PT= FacD.dCsShearConc;
    rData.dGamma_ps_A = FacD.dWsShearConc;

    rData.dGamma_mc_c_s    = FacD.dSfTriCmp;      // SLS, Table 4,
    rData.dGamma_mc_ca_s   = FacD.dSfUniformCmp;     //
    rData.dGamma_mc_t_pre  = FacD.dSfPreTens;    // pre-tensioned
    rData.dGamma_mc_t_post = FacD.dSfPostTens;   // post-tensioned
    //rData.dGamma_fL        = FacD.;
    //rData.dGamma_ms        = FacD.;

}


BOOL CDgnPscCommon::Get_PscDataPosi(ElemPairK ElemK, int nIJ, PSC_DATA_POSI& InData, BOOL bIsCompo)
{
    if(nIJ<0 || nIJ>1) return FALSE;

    BOOL bCalcOK=TRUE;
    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
    //
    _PSC_SECT_ELEM SectElem;
    _PSC_SECG_ELEM SecgElem;
    if (bIsCompo==TRUE)
    {
        if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE; // 합성후 환산단면
        //
        if(!Get_CompoPscSecgElem(ElemK, SecgElem)) return FALSE; // 합성후 순단면
    }
    else
    {    
        if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
        //    
        if(!Get_PscSecgElem(ElemK, SecgElem)) return FALSE;
    }
    //
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) return FALSE;
    //
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) return FALSE;  
    // Section g
    T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
    //
    _PSC_RSHR_ELEM RshrElem;  
    if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) return FALSE;
    //  
    // MQC-a/b. 5302-10, 균열 관련, 기준에 따라 _PSC_EXPC_ELEM 데이터가 없을 수 도 있기 때문에 return FALSE 하지 않는다.  
    _PSC_EXPC_ELEM ExpcElem;
    Get_PscExpcElem(ElemK, ExpcElem); 
	//For IRS Torsion Design
	T_PSCD_D PscdD;
	PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode ==IRS_PSC)
	{
		_PSC_PSCN_ELEM PscnElem;
		Get_PscPscnElem(ElemK, PscnElem);
		ConvertPscnData(nIJ, PscnElem, InData);
	}
    if ( m_iCivilCode == BS5400_90_PSC )  //BS 5400 에서 Class를 지정하는 부분 다른 기준은 검토하면 안됨
    {
        _PSC_PSCN_ELEM PscnElem;
        Get_PscPscnElem(ElemK, PscnElem);
        ConvertPscnData(nIJ, PscnElem, InData);

        T_POSC_D PoscD;
        PoscD.Initialize();
        if (ElemK.second == EN_EL_BEAM)
        {
            m_pDoc->m_pAttrCtrl->GetPosc(ElemK.first, PoscD);
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            m_pDoc->m_pAttrCtrl->GetPovc(ElemK.first, PoscD);
        }
        else ASSERT(0);
        
        InData.CalcD.nClass = PoscD.nClass + 1;
        InData.CalcD.bTypeC = PoscD.bTypeC;
    }

	if (m_iCivilCode == TMH07_3_1989)  //TMH07
	{
		_PSC_PSCN_ELEM PscnElem;
		Get_PscPscnElem(ElemK, PscnElem);
		ConvertPscnData(nIJ, PscnElem, InData);

		T_POSC_D PoscD;
		PoscD.Initialize();
        if (ElemK.second == EN_EL_BEAM)
        {
            m_pDoc->m_pAttrCtrl->GetPosc(ElemK.first, PoscD);
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            m_pDoc->m_pAttrCtrl->GetPovc(ElemK.first, PoscD);
        }
        else ASSERT(0);

		InData.CalcD.nClass = PoscD.nClass + 1;
		InData.CalcD.bTypeC = PoscD.bTypeC;
        if (nIJ==0)//I End Theta C & ThetaV 
        {
			InData.CalcD.dIThetaC = PoscD.dIThetaC;
			InData.CalcD.dIThetaV = PoscD.dIThetaV;
			InData.CalcD.dJThetaC = PoscD.dIThetaC;
			InData.CalcD.dJThetaV = PoscD.dIThetaV;
        }
        else//J End Theta C & ThetaV 
        {
			InData.CalcD.dIThetaC = PoscD.dJThetaC;
			InData.CalcD.dIThetaV = PoscD.dJThetaV;
			InData.CalcD.dJThetaC = PoscD.dJThetaC;
			InData.CalcD.dJThetaV = PoscD.dJThetaV;
        }
       
	}

    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	//End IRS Torsion Design
    BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

    ConvertPscCalcData(ElemK, nIJ, bCompositeGen,  &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], &SpbfElem.PosD[nIJ], InData);
    BOOL bchkCivilCode = (m_iCivilCode == BS5400_90_PSC || m_iCivilCode == TMH07_3_1989) ? TRUE : FALSE;
    if (!bchkCivilCode)//( m_iCivilCode != BS5400_90_PSC ) //다른 PSC 기준들이 Class 를 지정하는 부분 BS 5400 은 T_POSC_D 에서 클래스를 받음
    {
        _PSC_CINF_ELEM CinfElem;
        Get_PscCinfElem(ElemK, CinfElem);
        ConvertPscCalcInfo(&CinfElem, InData);
    }

    if (bIsCompo==TRUE)
    {
        if(bCompositeGen) 
        {
            if(!ConvertPscCalcShearSectData(ElemK, nIJ, InData.SectD))
            {
                // Get Elem.
                T_ELEM_D ElemD; ElemD.Initialize();
                if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first,ElemD)) {return FALSE;}	

                T_SECT_K SectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(ElemD.elpro);

                CString strText; strText.Format(_LS(IDS_DGN_PSC_COMPOSITE_SECTION_CHECK), SectK);
                AfxMessageBox(strText); // _T("Please Check Composite Section for PSC Design!")
                return FALSE;
            }
        }

        InData.bComposite = bIsCompo;
        InData.bGenComposite = bCompositeGen;
        // 합성전 단면
        _PSC_SECT_ELEM SectElemPre;
        Get_PscSectElem(TRUE, ElemK, SectElemPre); // 합성전 환산단면
        _PSC_SECG_ELEM SecgElemPre;
        Get_PscSecgElem(ElemK, SecgElemPre);        // 합성전 순단면
        _PSC_PSCN_ELEM PscnElem;
        Get_PscPscnElem(ElemK, PscnElem);    // 전단연결재
        _PSC_FACK_ELEM FackElem;
        Get_PscFackElem(ElemK, FackElem);    // EC PSC 피로
		  

        ConvertPscCalcSectData(ElemK, nIJ, bCompositeGen, &SectElemPre.SectPosi[nIJ], InData.SectPreD);
        ConvertPscCalcSectgData(&SecgElemPre.SectPosi[nIJ], InData.SectgPreD);
        ConvertPscnData(nIJ, PscnElem, InData);
        ConvertFackData(nIJ, FackElem, InData);
		
    }

    T_STCT_D StctD;
    if (!m_pDoc->m_pAttrCtrl->GetStct(StctD))	StctD.Initialize();
    InData.nBeamSectionChange = StctD.nBeamSectionChange; // 0=Constant, 1=Change

    return TRUE;
}


BOOL CDgnPscCommon::ConvertPscCalcShearSectData(ElemPairK ElemK, int nIJ, PSC_SECT_POSI& InData)
{
    T_SECP_D SecpD; SecpD.Initialize();
    // Get Elem.
    if (ElemK.second == EN_EL_BEAM)
    {
        T_ELEM_D ElemD; ElemD.Initialize();
        if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD)) { return FALSE; }

        T_SECT_K SectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(ElemD.elpro);        
        if (!m_pDoc->m_pAttrCtrl2->GetSecp(SectK, SecpD)) { return FALSE; }
    }
    else if (ElemK.second == EN_EL_VBEAM)
    {
        ASSERT(0);
    }
    else ASSERT(0);

    // Get Section Bw (General Section)
    if (SecpD.bCompPSC==TRUE)
    {
        InData.dbw = SecpD.dt1[0]; // Input value
    }
    else
    {
        InData.dbw = InData.dpShrThk[1]; // Centroid Auto 
    }

    return TRUE;
}


void CDgnPscCommon::ConvertPscCalcData(ElemPairK ElemK, int nIJ,  BOOL bCompositeGen, _PSC_MATL_ELEM* pMatlD, const _PSC_SECT_POSI* pSectD, const T_SPSC_SECT* pSectgD, 
                                       _PSC_RBAR_POSI* pRbarD, const _PSC_RSHR_POSI* pRshrD,  _PSC_TNDN_POSI* pTndnD, const _PSC_EXPC_POSI* pExpcD, _PSC_SPBF_POSI*pSpbf, PSC_DATA_POSI& rData)
{
    //   T_PSCD_D PscdD;
    //   m_pDoc->m_pAttrCtrl->GetPscd(PscdD);
    //   int iDgnCode = PscdD.iDgnCode;
    //   rData.PscdD = m_SPscdD;
    //   int iDgnCode = GetConvertCodeCvlToEng(m_SPscdD.iDgnCode);
    rData.iElemK = ElemK.first;
    rData.iPosition = nIJ;
    rData.CalcD.iPosition = nIJ;
    rData.PscdD = m_SPscdD;
    //
    ConvertPscCalcMatlData(m_iCivilCode, pMatlD, rData.MatlD);
    //
    ConvertPscCalcSectData(ElemK, nIJ, bCompositeGen, pSectD, rData.SectD);
    //
    ConvertPscCalcSectgData(pSectgD, rData.SectgD);
    //
    ConvertPscCalcRbarData(pRbarD, rData.RbarD);
    //
    ConvertPscCalcRshrData(pRshrD, rData.RshrD);
    //
    ConvertPscCalcTndnData(pTndnD, rData.TndnD);
    //
    ConvertPscCalcPescData(pExpcD, rData);
    //
    ConvertPscLengthFromEnd(pSpbf, rData.EinfD);

    //   if(fabs(rData.SectD.dBc * rData.SectD.dtc) < m_dZero) rData.bComposite = FALSE;  
    //   else                                                  rData.bComposite = TRUE;
    int nSize = (pSectD==NULL)? 0 :  pSectD->arOuter.GetSize();
    rData.bComposite =(nSize >= 2) ? TRUE : FALSE;

    if ( rData.pTdnaInfo == nullptr )
    {
        rData.pTdnaInfo = &m_TdnaInfo;
    }
}
void CDgnPscCommon::ConvertPscCalcDataPreD(ElemPairK ElemK, int nIJ, BOOL bCompositeGen, const _PSC_SECT_POSI* pSectD, const T_SPSC_SECT* pSectgD, PSC_DATA_POSI& rData)
{
    rData.PscdD = m_SPscdD;
    //
    ConvertPscCalcSectData(ElemK, nIJ, bCompositeGen, pSectD, rData.SectPreD);
    //
    ConvertPscCalcSectgData(pSectgD, rData.SectgPreD);
    //

}

void CDgnPscCommon::ConvertPscCalcMatlData(int iDgnCode, _PSC_MATL_ELEM* pMatlD, PSC_MATL_ELEM& rData)
{  
    if(!pMatlD) return;
    rData.bfci_u  = pMatlD->MatdD.Data1.Design.bTransfer;
    rData.C_fc    = pMatlD->MatdD.Data1.Design.C_fc;
    rData.C_fc_s  = pMatlD->MatdD.Data2.Design.C_fc;
    switch (iDgnCode)
    {
    case EUROCODE2_2_05_PSC:
    case IRC_112_2011_PSC:
    case IRC_112_2020_PSC:
    case AS_5100_5_17_PSC:
	case TMH07_3_1989:
    case BS5400_90_PSC:
    case BD_44_15_PSC:
    case CS_455_PSC:
    case KDS_24_14_21_2022_PSC:
    case KSCE_LSD15_PSC:
    case SNiP_20503_84_PSC:
    case SP_35_13330_11_PSC:
    case SNiP_20503_84_PSC_MKS:
    case SP_35_13330_11_PSC_MKS:
	case IRS_PSC:
    case ABNT_NBR_6118_2023_PSC:
        rData.C_fci   = pMatlD->dfci;
        break;
    default: break;
    }
    CMatlDB MatlDB(m_pDoc);

    rData.Elast   = pMatlD->MatdD.Data1.Analysis.Elast;
    rData.B_fy    = pMatlD->MatdD.MainRebarData.B_fy;
    rData.B_Elast = pMatlD->MatdD.MainRebarData.B_Elast;
    rData.fy_sub  = pMatlD->MatdD.SubRebarData.B_fy;
    rData.Elast_sub = pMatlD->MatdD.SubRebarData.B_Elast;
    rData.C_fc_s    = pMatlD->MatdD.Data2.Design.C_fc;
    rData.C_fci_s    = pMatlD->dfci_s;  
    rData.strConMatlName = CT2W(pMatlD->MatdD.Data1.CodeMatlName);

    rData.Elast_s   = pMatlD->MatdD.Data2.Analysis.Elast;
    rData.B_fy_s    = pMatlD->MatdD.MainRebarData2.B_fy;
    rData.B_Elast_s = pMatlD->MatdD.MainRebarData2.B_Elast;
    rData.fy_sub_s  = pMatlD->MatdD.SubRebarData2.B_fy;
    rData.Elast_sub_s = pMatlD->MatdD.SubRebarData2.B_Elast;
    rData.Density_s   = pMatlD->MatdD.Data2.Analysis.Density;
    rData.strConMatlName_s = CT2W(pMatlD->MatdD.Data2.CodeMatlName);

    rData.dfck   = pMatlD->dfck  ; 
    rData.dftk   = pMatlD->dftk  ; 
    rData.dfckp  = pMatlD->dfckp ; 
    rData.dftkp  = pMatlD->dftkp ; 
    rData.dfcd   = pMatlD->dfcd  ; 
    rData.dftd   = pMatlD->dftd  ;
    rData.dfskM  = pMatlD->dfskM ; 
    rData.dfsdtM = pMatlD->dfsdtM; 
    rData.dfsdcM = pMatlD->dfsdcM;
    rData.dfskS  = pMatlD->dfskS ; 
    rData.dfsdtS = pMatlD->dfsdtS;
    rData.dfsdcS = pMatlD->dfsdcS;
    rData.dfskD  = pMatlD->dfskD ; 
    rData.dfsdtD = pMatlD->dfsdtD; 
    rData.dfsdcD = pMatlD->dfsdcD;

    // slab value
    if(UQ0(pMatlD->MatdD.Data2.Design.C_fc))
    {
		if(LE(rData.C_fc, rData.C_fc_s))
		{
			rData.bChkLambda = pMatlD->MatdD.Data1.Design.bLambda;
			rData.dLambda    = pMatlD->MatdD.Data1.Design.dLambda;
		}
		else
		{
			rData.bChkLambda = pMatlD->MatdD.Data2.Design.bLambda;
			rData.dLambda    = pMatlD->MatdD.Data2.Design.dLambda;
		}
    }
    else
    {
		rData.bChkLambda = pMatlD->MatdD.Data1.Design.bLambda;
		rData.dLambda    = pMatlD->MatdD.Data1.Design.dLambda;
    }

    for(int i=0; i<2; ++i)
    {
        rData.dfpkV[i] = pMatlD->dfpkV[i] ; 
        rData.dfpdtV[i]= pMatlD->dfpdtV[i]; 
        rData.dfpdcV[i]= pMatlD->dfpdcV[i];
    }
    rData.dfcd_PT = pMatlD->dfcd_PT;
    rData.dfcd_A  = pMatlD->dfcd_A;
    rData.dfyd_PT = pMatlD->dfyd_PT;
    rData.dfyd_A  = pMatlD->dfyd_A;
    rData.dfywd_PT= pMatlD->dfywd_PT;
    rData.dfywd_A = pMatlD->dfywd_A;

    rData.dfcm = pMatlD->dfcm;
    rData.dfctm = pMatlD->dfctm;
    rData.dfctk = pMatlD->dfctk;

    rData.dfcd_PT_s = pMatlD->dfcd_PT_s;
    rData.dfcd_A_s  = pMatlD->dfcd_A_s;
    rData.dfyd_PT_s = pMatlD->dfyd_PT_s;
    rData.dfyd_A_s  = pMatlD->dfyd_A_s;
    rData.dfywd_PT_s= pMatlD->dfywd_PT_s;
    rData.dfywd_A_s = pMatlD->dfywd_A_s;

    rData.dfcm_s = pMatlD->dfcm_s;
    rData.dfctm_s = pMatlD->dfctm_s;
    rData.dfctk_s = pMatlD->dfctk_s;

}

BOOL CDgnPscCommon::ConvertPscCalcSectData(ElemPairK ElemK, int nIJ, BOOL bCompositeGen, const _PSC_SECT_POSI* pSectD, PSC_SECT_POSI& rData)
{	
    if(!pSectD) return FALSE;
    int i,j;
    ConvertPscCalcSectgData(&(pSectD->SpscSect), rData.SpscSect);
    //
    rData.strShape = pSectD->strShape;
    rData.bClosedSect = pSectD->bClosedSect ? true : false;
    rData.nDgnSectType = pSectD->nDgnSectType;
	rData.bUseAoh = pSectD->bUseAoh ? true : false;
    rData.dAg  = pSectD->dAg;
    rData.dAoh = pSectD->dAoh;
    rData.dPh  = pSectD->dPh;
    rData.dKt  = pSectD->dKt;
    rData.dWt  = pSectD->dWt;
    rData.dPcp = pSectD->dPcp;
    rData.dAcp = pSectD->dAcp;	
    for(i=0; i<4; ++i)
    {
        rData.dpMomPos[i][0]=pSectD->dpMomPos[i][0];
        rData.dpMomPos[i][1]=pSectD->dpMomPos[i][1];
    }
    for(i=0; i<3; ++i)
    {
        rData.dpShrPos[i]=pSectD->dpShrPos[i];
        rData.dpShrThk[i]=pSectD->dpShrThk[i];
        rData.dQ[i]      =pSectD->dQ[i];
    }
    for(i=0; i<2; ++i) rData.dpFlgThk[i] = pSectD->dpFlgThk[i];

    if(bCompositeGen)
    { // Composte PSC General 단면
        if(!ConvertPscCalcShearSectData(ElemK, nIJ, rData)) 
        {
            return FALSE;
        }
        //rData.dTorThk = pSectD->dTorThk;
        //rData.dbw = rData.dpShrThk[1]; //min(min(rData.dpShrThk[0],rData.dpShrThk[1]),rData.dpShrThk[2]);

    }
    else
    {	
        rData.dTorThk = pSectD->dTorThk;
        rData.dbw = min(min(rData.dpShrThk[0],rData.dpShrThk[1]),rData.dpShrThk[2]);
    }
    rData.dbtf = pSectD->dbtf;

    int iOuterNum = pSectD->arOuter.GetSize();
    if(iOuterNum>0) rData.arOuter.resize(iOuterNum);
    for(i=0; i<iOuterNum; ++i)
    {
        T_SPSC_COOR CoorD;
        CoorD = pSectD->arOuter.GetAt(i);
        DGN_SPSC_COOR DgnCoorD;    
        for ( j=0; j<2; ++j )
        {
            for ( int k = 0; k<CoorD.arSpscPont[j].GetSize(); ++k )
            {
                DgnCoorD.arSpscPont[j].push_back(CoorD.arSpscPont[j][k]);
            }
        }
        rData.arOuter[i] = DgnCoorD;
    }

    // Modify, Jaeoh. [1/26/2010] Box형의 경우 빈공간의 cell의 개수를 넘겨주어야함. 
    int iInnerCellNum = 0;  
    for(i=0; i<cSECT_INN; ++i)
    {
        int iInnerNum = pSectD->arInner[i].GetSize();
        if(iInnerNum>0) 
        {
            rData.arInner[i].resize(iInnerNum);
            iInnerCellNum += iInnerNum;
        }
        for(j=0; j<iInnerNum; ++j)
        {
            T_SPSC_COOR CoorD;
            CoorD = pSectD->arInner[i].GetAt(j);
            DGN_SPSC_COOR DgnCoorD;    
            for(int k=0; k<2; ++k)
            {
                for ( int l = 0; l<CoorD.arSpscPont[k].GetSize(); ++l )
                {
                    DgnCoorD.arSpscPont[k].push_back(CoorD.arSpscPont[k][l]);
                }
            }
            rData.arInner[i][j] =  DgnCoorD;
        }    
    }

    rData.iInnerCellNum = iInnerCellNum;

    double dbi = 0.0;
    dgn::link::ITmpDgnCalcBaseLib::GetInstance()->CalcCompositeGirderWidth(rData.arOuter, dbi);

    rData.dLbarThk = pSectD->dLbarThk;
    rData.bDivSect4Tor = pSectD->bDivSect4Tor;
    rData.nDivPart = pSectD->nDivPart;
    for(i=0; i<3; ++i)
    {
        rData.dCutPos[i]   = pSectD->dCutPos[i];
        rData.dIxx_part[i] = pSectD->dIxx_part[i];
        rData.dAoh_part[i] = pSectD->dAoh_part[i];
        rData.dPh_part[i]  = pSectD->dPh_part[i];
        rData.dPcp_part[i] = pSectD->dPcp_part[i];
        rData.dAcp_part[i] = pSectD->dAcp_part[i];
        rData.dyDim_part[i] = pSectD->dyDim_part[i];
        rData.dzDim_part[i] = pSectD->dzDim_part[i];
    }	

    rData.dCutPos_Sl    = pSectD->dCutPos_Sl  ;
    rData.dIxx_part_Sl  = pSectD->dIxx_part_Sl;
    rData.dAoh_part_Sl  = pSectD->dAoh_part_Sl;
    rData.dPh_part_Sl   = pSectD->dPh_part_Sl ;
    rData.dPcp_part_Sl  = pSectD->dPcp_part_Sl;
    rData.dAcp_part_Sl  = pSectD->dAcp_part_Sl;
	rData.dyDim_part_Sl = pSectD->dyDim_part_Sl;
    rData.dzDim_part_Sl = pSectD->dzDim_part_Sl;
    rData.dbi           = dbi;
    if (pSectD->bCompoSect)
    {
		if ( pSectD->bUseAoh )
		{
			rData.dPcp_c = pSectD->dPcp;
		}
		else
		{
			if (pSectD->strShape == D_SECT_SHAPE_COMPO_PC ||
                pSectD->strShape == D_SECT_SHAPE_COMPO_G  )
            {
				rData.dPcp_c = rData.dPcp - 2.0*dbi;
            }
            else if (pSectD->strShape == D_SECT_SHAPE_COMPO_CI ||
                     pSectD->strShape == D_SECT_SHAPE_COMPO_CT)
            {
                double dbf_t = fabs(pSectD->dpMomPos[1][0]-pSectD->dpMomPos[0][0]);
                rData.dPcp_c = rData.dPcp - 2.0*dbf_t;
            }
            else ASSERT(0);    
		}
    }

    rData.dBc = pSectD->dBc;
    rData.dtc = pSectD->dtc;
    rData.dHh = pSectD->dHh;

	rData.nWebSize = pSectD->nWebSize;

    if (m_iCivilCode==TMH07_3_1989)
    {
	
        //Calculation for TMH7 Section CLass
        BOOL bBox1 = (pSectD->strShape == DGN_SECT_SHAPE_PSC_1CELL || pSectD->strShape == DGN_SECT_SHAPE_PSC_2CELL ||
            pSectD->strShape == DGN_SECT_SHAPE_PSC_3CELL || pSectD->strShape == DGN_SECT_SHAPE_PSC_NCEL2 || pSectD->strShape == DGN_SECT_SHAPE_PSC_4CELL ||
            pSectD->strShape == DGN_SECT_SHAPE_PSC_VALUE) ? TRUE : FALSE;

        BOOL bRect1 = (pSectD->strShape == DGN_SECT_SHAPE_PSC_VALUE || pSectD->strShape == DGN_SECT_SHAPE_PSC_MID) ? TRUE : FALSE;
        BOOL bRectSectDiv = (pSectD->nDivPart == 0) ? TRUE : FALSE;
        BOOL bRect2 = ((pSectD->dpFlgThk[0] == pSectD->SpscSect.dzDim) || (pSectD->dpFlgThk[1] == pSectD->SpscSect.dzDim)) ? TRUE : FALSE;
        BOOL bISect = (pSectD->strShape == DGN_SECT_SHAPE_PSC_VALUE || pSectD->strShape == DGN_SECT_SHAPE_PSC_I || pSectD->strShape == DGN_SECT_SHAPE_PSC_T || 
                       pSectD->strShape == DGN_SECT_SHAPE_PSC_MID || pSectD->strShape == DGN_SECT_SHAPE_COMPO_CI || pSectD->strShape == DGN_SECT_SHAPE_COMPO_I ||
                       pSectD->strShape == DGN_SECT_SHAPE_COMPO_CT || pSectD->strShape == DGN_SECT_SHAPE_COMPO_CT || pSectD->strShape == DGN_SECT_SHAPE_COMPO_PC ||
                       pSectD->strShape == DGN_SECT_SHAPE_COMPO_TUB) ? TRUE : FALSE;
        BOOL bISectFlange = ((pSectD->dpFlgThk[0] + pSectD->dpFlgThk[1] > 0)) ? TRUE : FALSE;
        BOOL bISectDivision = (pSectD->nDivPart >= 2) ? TRUE : FALSE;
		if (pSectD->bCompoSect && (pSectD->strShape == DGN_SECT_SHAPE_COMPO_TUB || pSectD->strShape == DGN_SECT_SHAPE_COMPO_PC)) {
			bBox1 = TRUE; iInnerCellNum = 1;}//Design U Type Section as Box Type
      
        
        BOOL bIRectDivision = (pSectD->nDivPart == 2) ? TRUE : FALSE;
        BOOL bPSCValueWebExist = pSectD->nWebSize > 0 ? TRUE : FALSE;
        BOOL bWebnFLangeExistValue = bISectFlange && bPSCValueWebExist ? TRUE : FALSE;
        if (bBox1 && iInnerCellNum >= 1)
        {
            //Box Section
            rData.TMH7SectClass = 10;
        }
        else if (bRect1 && bRectSectDiv)
        {
            if (bWebnFLangeExistValue && !bRect2)  rData.TMH7SectClass = 30;
            else
            {
                //Rectangular Section
                rData.TMH7SectClass = 20;
            }
        }
        else if (bISect && bISectFlange && bISectDivision)
        {
            //T,L,I Section
            rData.TMH7SectClass = 30;
        }

		// Get Elem.
        BOOL bComposite = m_pDoc->m_pPostCtrl->IsPSCCompositeSect();

        T_SECT_D SectD; SectD.Initialize();
        if (!m_pDoc->m_pPostCtrl->GetSectPostTsgrDgn(ElemK, SectD)) return FALSE;


		//T_SECT_SECTBASE_D& SBase = (i == 0) ? SectD.SectBefore.SectI : SectD.SectBefore.SectJ;
		T_SECT_SECTION_D& SectBef = SectD.SectBefore;
		T_SECT_SECTBASE_D& SBase = SectBef.SectI;
        BOOL bBox = ((SBase.Size[1] * SBase.Size[2] * SBase.Size[3] * SBase.Size[4] * SBase.Size[5] * SBase.Size[6] * SBase.Size[7]) > 0.0) ? TRUE : FALSE;
        BOOL bRect = ((SBase.Size[1] * SBase.Size[3] * SBase.Size[4] * SBase.Size[6] * SBase.Size[7]) > 0.0) ? TRUE : FALSE;
        BOOL bTtype = ((SBase.Size[1] * SBase.Size[5] * SBase.Size[9] * SBase.Size[10] * SBase.Size[11]) > 0.0) ? TRUE : FALSE;
        if (rData.TMH7SectClass == 0)
        {
            if (SectBef.bSymmetric == TRUE && SectBef.Shape == DGN_SECT_SHAPE_PSC_T && bBox)
            {
                //Box Section
                if (rData.TMH7SectClass < 10)
                {
                    rData.TMH7SectClass = 10;
                }

            }
            else if (bRect1 || bRect)
            {
                //Rectangular Section
                if (rData.TMH7SectClass < 20) rData.TMH7SectClass = 20;
            }
            else
            {
                //I Section
                if (rData.TMH7SectClass == 0) rData.TMH7SectClass = 30;
            }


        }
		if (rData.TMH7SectClass == 30)
		{
			//TLI SEction
			if (SectBef.Shape == DGN_SECT_SHAPE_PSC_I || SectBef.Shape == DGN_SECT_SHAPE_COMPO_CI || SectBef.Shape == DGN_SECT_SHAPE_COMPO_I)
			{
               //Width
                rData.dyDim_part[0] = pSectD->SpscSect.dyDim;
                rData.dyDim_part[1] = SBase.Size[10];
                rData.dyDim_part[2] = pSectD->SpscSect.dyDim;

                //Height
                rData.dzDim_part[0] = pSectD->dpFlgThk[0];
                rData.dzDim_part[1] = pSectD->SpscSect.dzDim - pSectD->dpFlgThk[0] - pSectD->dpFlgThk[1];
                rData.dzDim_part[2] = pSectD->dpFlgThk[1];

                rData.nDivPart = 3;
                if (bComposite)
                {
					double dH = 0., dTw = 0., dB1 = 0., dTF1 = 0., dB2 = 0., dTF2 = 0., dHw = 0.;
					Get_SectionSizeOfSect_I(SBase.Size, dH, dTw, dB1, dTF1, dB2, dTF2, dHw);
					//Width
					rData.dyDim_part[0] = SBase.Size[11]+ SBase.Size[27];
					rData.dyDim_part[1] = SBase.Size[10] + SBase.Size[26];
					rData.dyDim_part[2] = SBase.Size[14] + SBase.Size[30];

					//Height
					rData.dzDim_part[0] = SBase.Size[1] + SBase.Size[2];
					rData.dzDim_part[1] = SBase.Size[5];
					rData.dzDim_part[2] = SBase.Size[6] + SBase.Size[9];
                }
			}
            else if (SectBef.Shape == DGN_SECT_SHAPE_PSC_T || SectBef.Shape == DGN_SECT_SHAPE_COMPO_CT || SectBef.Shape == DGN_SECT_SHAPE_PSC_VALUE)
            {
                BOOL bIsPartZero = ((rData.dyDim_part[0] * rData.dzDim_part[0] * rData.dyDim_part[1] * rData.dzDim_part[1]) > 0.) ? TRUE : FALSE;
				if (!bIsPartZero)
				{
                    if (SectBef.Shape == DGN_SECT_SHAPE_PSC_VALUE)
                    {
						//Width
						rData.dyDim_part[0] = SBase.Size[1] ;
						rData.dyDim_part[1] = SBase.Size[2];
						//rData.dyDim_part[2] = pSectD->SpscSect.dyDim;

						//Height
						rData.dzDim_part[0] = SBase.Size[2];
						rData.dzDim_part[1] = SBase.Size[1] - SBase.Size[2];
						//rData.dzDim_part[2] = pSectD->dpFlgThk[1];
						rData.nDivPart = 2;
                    }
                    else
                    {
						//Width
						rData.dyDim_part[0] = SBase.Size[7] + SBase.Size[22];
						rData.dyDim_part[1] = SBase.Size[4] + SBase.Size[19];
						//rData.dyDim_part[2] = pSectD->SpscSect.dyDim;

						//Height
						rData.dzDim_part[0] = SBase.Size[1];
						rData.dzDim_part[1] = SBase.Size[3];
						//rData.dzDim_part[2] = pSectD->dpFlgThk[1];
						rData.nDivPart = 2;

                    }
				

					if (bComposite)
					{
						double dH = 0., dTw = 0., dB1 = 0., dTF1 = 0., dB2 = 0., dTF2 = 0., dHw = 0.;
						Get_SectionSizeOfSect_I(SBase.Size, dH, dTw, dB1, dTF1, dB2, dTF2, dHw);
						//Width
						rData.dyDim_part[0] = max(dB1,max(rData.dyDim_part[0],SBase.Size[11] + SBase.Size[27]));
						rData.dyDim_part[1] = max(dTw,max(rData.dyDim_part[1],SBase.Size[10] + SBase.Size[26]));
						//rData.dyDim_part[2] = SBase.Size[14] + SBase.Size[30];

						//Height
						rData.dzDim_part[0] = max(dTF1,max(rData.dzDim_part[0],SBase.Size[1] + SBase.Size[2]));
						rData.dzDim_part[1] = max(dHw,max(rData.dzDim_part[1],SBase.Size[5]));
						//rData.dzDim_part[2] = SBase.Size[6] + SBase.Size[9];
					}
				}				
               
            }
			else 
			{
				double dH = 0., dTw = 0., dB1 = 0., dTF1 = 0., dB2 = 0., dTF2 = 0., dHw = 0.;
				Get_SectionSizeOfSect_I(SBase.Size, dH, dTw, dB1, dTF1, dB2, dTF2, dHw);
				//Width
				rData.dyDim_part[0] = dB1;
				rData.dyDim_part[1] = dTw;
				rData.dyDim_part[2] = dB2;



				//Height
				rData.dzDim_part[0] = dTF1;
				rData.dzDim_part[1] = dHw;
				rData.dzDim_part[2] = dTF2;
                if ((dB1* dTF1==0) || (dB2* dTF2==0)) rData.nDivPart = 2;
                else  rData.nDivPart = 3;
			}
		}
    }
  
    return TRUE;
}

void CDgnPscCommon::ConvertPscCalcSectgData(const T_SPSC_SECT* pSectgD, DGN_SPSC_SECT& rData)
{
    if(!pSectgD) return;
    rData.dyDim = pSectgD->dyDim;
    rData.dzDim = pSectgD->dzDim;
    rData.dyCen = pSectgD->dyCen; 
    rData.dzCen = pSectgD->dzCen;
    rData.dArea = pSectgD->dArea;
    rData.dAsy  = pSectgD->dAsy ; 
    rData.dAsz  = pSectgD->dAsz ;
    rData.dIxx  = pSectgD->dIxx ;
    rData.dIyy  = pSectgD->dIyy ;
    rData.dIzz  = pSectgD->dIzz ;
    rData.dSlt  = pSectgD->dSlt ;
    rData.dSlb  = pSectgD->dSlb ;
    rData.dSrt  = pSectgD->dSrt ;
    rData.dSrb  = pSectgD->dSrb ;  
    rData.dSlt_s  = pSectgD->dSlt_s ;
    rData.dSlb_s  = pSectgD->dSlb_s ;
    rData.dSrt_s  = pSectgD->dSrt_s ;
    rData.dSrb_s  = pSectgD->dSrb_s ;  

    rData.dYlt    = pSectgD->dYlt ;
    rData.dYlb    = pSectgD->dYlb ;
    rData.dYrt    = pSectgD->dYrt ;
    rData.dYrb    = pSectgD->dYrb ;  
    rData.dYlt_s  = pSectgD->dYlt_s ;
    rData.dYlb_s  = pSectgD->dYlb_s ;
    rData.dYrt_s  = pSectgD->dYrt_s ;
    rData.dYrb_s  = pSectgD->dYrb_s ;  
    rData.dGirderWidth  = pSectgD->dGirderWidth ; 

}

void CDgnPscCommon::ConvertSectStiffnessData(T_SECT_STIFFNESS &rStiff, DGN_SECT_STIFFNESS &rData)
{  
    rData.dArea  = rStiff.Area;  
    rData.dAsy   = rStiff.Asy;
    rData.dAsz   = rStiff.Asz; 
    rData.dIxx   = rStiff.Rxx;
    rData.dIyy   = rStiff.Ryy; 
    rData.dIzz   = rStiff.Rzz; 
    rData.dCyp   = rStiff.Cyp; 
    rData.dCym   = rStiff.Cym; 
    rData.dCzp   = rStiff.Czp; 
    rData.dCzm   = rStiff.Czm; 
    rData.dQyb   = rStiff.Qyb; 
    rData.dQzb   = rStiff.Qzb; 
    rData.dWArea = rStiff.WArea;
    rData.dx1    = rStiff.dx1;  
    rData.dx2    = rStiff.dx2;  
    rData.dx3    = rStiff.dx3;  
    rData.dx4    = rStiff.dx4;  
    rData.dy1    = rStiff.dy1;  
    rData.dy2    = rStiff.dy2;  
    rData.dy3    = rStiff.dy3;  
    rData.dy4    = rStiff.dy4;  
    rData.dCwp   = rStiff.dIw;
    rData.dZBar  = rStiff.Czm;
}

void CDgnPscCommon::ConvertCompSectStiffness4Warping(int nPos, const T_SECT_D& rSectD, CSG_SECT_POSD &rData)
{
    if (rData.SectInfo.bIsTapered)
    {
        if (nPos==0)
        {
            ConvertSectStiff4Warping(rSectD.SectBefore.SectI.Stiffness, rData.WSgr);
            ConvertSectStiff4Warping(rSectD.SectAfter.SectI.Stiffness, rData.WSst);
        }
        else
        {
            ConvertSectStiff4Warping(rSectD.SectBefore.SectJ.Stiffness, rData.WSgr);
            ConvertSectStiff4Warping(rSectD.SectAfter.SectJ.Stiffness, rData.WSst);
        }
    }
    else
    {
        ConvertSectStiff4Warping(rSectD.SectBefore.SectI.Stiffness, rData.WSgr);
        ConvertSectStiff4Warping(rSectD.SectAfter.SectI.Stiffness, rData.WSst);
    }
}

void CDgnPscCommon::ConvertSectStiff4Warping(const T_SECT_STIFFNESS &rStiff, DGN_SECT_WARPING_STIFF &rData)
{
    rData.dIw = rStiff.dIw;
    rData.dw1 = rStiff.dw1;
    rData.dw2 = rStiff.dw2;
    rData.dw3 = rStiff.dw3;
    rData.dw4 = rStiff.dw4;
}

void CDgnPscCommon::ConvertPscCalcRbarData( _PSC_RBAR_POSI* pRbarD, DGN_RBAR_POSI& rData)
{
    if(!pRbarD) return;
    //CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
    int iNum = pRbarD->arRbarPosi.GetSize();
    if(iNum>0) rData.arRbarPosi.SetSize(iNum);
    for(int i=0; i<iNum; ++i)
    {
        _DGN_RBAR_CRC &RbarCrcD = pRbarD->arRbarPosi[i];
        DGN_RBAR_CRC RbarDgnD;
        ConvertPscCalcRbarCrcData(&RbarCrcD, RbarDgnD);
        rData.arRbarPosi.SetAt(i, RbarDgnD);    
    }
}

void CDgnPscCommon::ConvertPscCalcRshrData(const _PSC_RSHR_POSI* pRshrD, PSC_RSHR_POSI& rData)
{
    if(!pRshrD) return;
    rData.dfdt = pRshrD->dfdt;
    rData.dfdb = pRshrD->dfdb;
    rData.dVd  = pRshrD->dVd ;
    rData.dMd  = pRshrD->dMd ;
    //
    ConvertPscCalcRpscSbarData(&(pRshrD->RpscSbar), rData.RpscSbar);
}

void CDgnPscCommon::ConvertPscCalcTndnData( _PSC_TNDN_POSI* pTndnD, PSC_TNDN_POSI& rData)
{
    if(!pTndnD) return;
    rData.iMaxLoadType = pTndnD->iMaxLoadType;
    rData.iMaxRelxType = pTndnD->iMaxRelxType;
    //	
    int iTndnNum = pTndnD->arTndnPosi.GetSize();
    if(iTndnNum>0) rData.arTndnPosi.SetSize(iTndnNum);
    for(int i=0; i<iTndnNum; ++i)
    {
        _DGN_TNDN_CRC &TndnCrcD = pTndnD->arTndnPosi[i];
        DGN_TNDN_CRC TndnDgnD;
        ConvertPscCalcTndnCrcData(&TndnCrcD,TndnDgnD);
        rData.arTndnPosi.SetAt(i, TndnDgnD);
    }  
    rData.bPost   = pTndnD->bPost;
    rData.bBonded = pTndnD->bBonded;
}

void CDgnPscCommon::ConvertPscCalcRbarCrcData(const _DGN_RBAR_CRC* pRbarD, DGN_RBAR_CRC& rData)
{  
    if(!pRbarD) return;
    rData.strRbarNa = pRbarD->strRbarNa;
    rData.dArea     = pRbarD->dArea; 
    rData.dDia      = pRbarD->dDia ;
    rData.dy        = pRbarD->dy   ; 
    rData.dz        = pRbarD->dz   ;
    rData.nPart     = pRbarD->nPart;

}

void CDgnPscCommon::ConvertPscCalcRpscSbarData(const T_RPSC_SBAR* pSbarD, DGN_RPSC_SBAR& rData)
{
    if(!pSbarD) return;
    // Diagonal Reinforcement
    rData.bSbarExist  = pSbarD->bSbarExist ;
    if(rData.bSbarExist)
    {
        rData.dSbarPitch  = pSbarD->dSbarPitch ;
        rData.dSbarTheta  = pSbarD->dSbarTheta ;
        rData.dSbarAw     = pSbarD->dSbarAw    ;
    }

    // Steel bar for web
    rData.bTndnExist  = pSbarD->bTndnExist ;
    if(rData.bTndnExist)
    {
        rData.dTndnDist   = pSbarD->dTndnDist  ;
        rData.dTndnTheta  = pSbarD->dTndnTheta ;
        rData.dTndnAp     = pSbarD->dTndnAp    ;
        rData.dTndnPs     = pSbarD->dTndnPs    ;
    }

    // Torsional Reinforcement
    rData.bLbarExist  = pSbarD->bLbarExist ;
    if(rData.bLbarExist)
    {
        rData.dLbarPitch  = pSbarD->dLbarPitch ;
        rData.dLbarAwt    = pSbarD->dLbarAwt   ;
        rData.dLbarAlt    = pSbarD->dLbarAlt   ;
    }

    // 전단철근
    rData.bStirExist  = pSbarD->bStirExist ;
    if(rData.bStirExist)
    {
        rData.dStirPitch  = pSbarD->dStirPitch ;
        rData.dStirAw     = pSbarD->dStirAw    ;
        rData.dTndnFactor = pSbarD->dTndnFactor;
    }

    // Enclosing Stirrup
    rData.bLbarFlg    = pSbarD->bLbarFlg   ;
    if(rData.bLbarFlg)
    {
        rData.dLbarThick  = pSbarD->dLbarThick ;
        rData.bIncFC      = pSbarD->bIncFC     ;
    }   
}

void CDgnPscCommon::ConvertPscCalcTndnCrcData(const _DGN_TNDN_CRC* pTndnD, DGN_TNDN_CRC& rData)
{
    if(!pTndnD) return;
    rData.MatlK         = pTndnD->MatlK      ;
    rData.strMatlName   = pTndnD->strMatlName;
    rData.strTendonName = pTndnD->strTendonName;
    rData.strTndnGrup   = pTndnD->strTndnGrup;
    rData.KeyTdna       = pTndnD->KeyTdna    ;
    rData.dTndnNum      = pTndnD->dTndnNum   ;
    rData.iTndnType     = pTndnD->iTndnType  ;
    rData.iBondType     = pTndnD->iBondType  ;
    rData.iLoadType     = pTndnD->iLoadType  ;
    rData.iRelxType     = pTndnD->iRelxType  ;
    rData.dEps          = pTndnD->dEps       ; 
    rData.dfy           = pTndnD->dfy        ;
    rData.dfpu          = pTndnD->dfpu       ;
    rData.dfpy          = pTndnD->dfpy       ;
    rData.dArea         = pTndnD->dArea      ;
    rData.dy            = pTndnD->dy         ; 
    rData.dz            = pTndnD->dz         ;
    rData.dzAng         = pTndnD->dzAng      ;
    rData.dfpk          = pTndnD->dfpk       ; 
    rData.dfpdt         = pTndnD->dfpdt      ; 
    rData.dfpdc         = pTndnD->dfpdc      ;
    rData.dEffDia       = pTndnD->dEffDia    ;
    rData.dStressLL     = pTndnD->dStressLL  ;
    for(int i=0; i<3; ++i)
    {
        rData.dStress[i] = pTndnD->dStress[i];
        for(int j=0; j<3; ++j)	rData.dStrLoss[i][j] = pTndnD->dStrLoss[i][j];
    }
    rData.dFp01    = pTndnD->dFp01   ;
    rData.dfp01k   = pTndnD->dfp01k  ;
    rData.dfpd_PT  = pTndnD->dfpd_PT ;
    rData.dfpd_A   = pTndnD->dfpd_A  ;
    rData.dEpsi_pd = pTndnD->dEpsi_pd;
    rData.dLength      = pTndnD->dLength;
    rData.iBoundaryNum = pTndnD->iBoundaryNum;
    rData.dDuctDia = pTndnD->dDuctDia;
    rData.bReverse    = pTndnD->bReverse ? true : false;
    rData.bDebond     = pTndnD->bDebond ? true : false;
    rData.dTranRatio  = pTndnD->dTranRatio;
    rData.dLengthElem = pTndnD->dLengthElem;
    rData.bFrom_I     = pTndnD->bFrom_I ? true : false;
}

void CDgnPscCommon::ConvertPscCalcTdnaData(const _PSC_TNDN_TDNA* pTdnaD, PSC_TNDN_TDNA& rData)
{
    if(!pTdnaD) return;

    rData.iLoadType = pTdnaD->iLoadType;
    rData.iRelxType = pTdnaD->iRelxType;
    rData.dfpu      = pTdnaD->dfpu; 
    rData.dfpy      = pTdnaD->dfpy;
    rData.dfpk      = pTdnaD->dfpk; 
    rData.dfpdt     = pTdnaD->dfpdt; 
    rData.dfpdc     = pTdnaD->dfpdc;
    rData.dFp01     = pTdnaD->dFp01;
    rData.dfp01k    = pTdnaD->dfp01k;
    rData.dfpd      = pTdnaD->dfpd;
    rData.dTransBeginLen = pTdnaD->dTransBeginLen;
    rData.dTransEndLen   = pTdnaD->dTransEndLen;
    rData.dDebondedBeginLen = pTdnaD->dDebondedBeginLen;
    rData.dDebondedEndLen   = pTdnaD->dDebondedEndLen;

}

void CDgnPscCommon::ConvertPscCalcTndnTranToTdna(const _DGN_TNDN_TRAN* pTran, PSC_TNDN_TDNA& rData)
{
    if ( !pTran ) return;

    rData.dfpe_Lpt_Begin    = pTran->TranB[0].dfpe_Lpt;
    rData.dfps_Lp_Begin_Pos = pTran->TranB[0].Pos.dfps_Lp;
    rData.dfps_Lp_Begin_Neg = pTran->TranB[0].Neg.dfps_Lp;
    rData.dfpe_Lpt_End      = pTran->TranB[1].dfpe_Lpt;
    rData.dfps_Lp_End_Pos   = pTran->TranB[1].Pos.dfps_Lp;
    rData.dfps_Lp_End_Neg   = pTran->TranB[1].Neg.dfps_Lp;
}

void CDgnPscCommon::ConvertPscCalcPescData(const _PSC_EXPC_POSI* pExpcD, PSC_DATA_POSI& rData)
{
    if(!pExpcD) return;

    int i=0; 
    for(i=0; i<2; ++i) rData.iExposureClass[i] = pExpcD->iExposureClass[i];  

    rData.bmb6      = pExpcD->bmb6;     
    rData.nmb6Type  = pExpcD->nmb6Type; 
    rData.dSigma_by = pExpcD->dSigma_by;
    rData.dmb6      = pExpcD->dmb6;     
    rData.dmb6_s    = pExpcD->dmb6_s;     
    rData.bmb10     = pExpcD->bmb10;    
    rData.nmb10Type = pExpcD->nmb10Type;
    rData.dThick    = pExpcD->dSeamThick;
    rData.dRbj      = pExpcD->dRbj;
    rData.dmb10     = pExpcD->dmb10;      
}

void CDgnPscCommon::ConvertPscnData(int nIJ, const _PSC_PSCN_ELEM& PscnElem, PSC_DATA_POSI& rData)
{
    rData.PscnD.iPosition  = nIJ; // 0=I, 1=J  
    rData.PscnD.nFaceState = PscnElem.nFaceState[nIJ];
    rData.PscnD.dAngle     = PscnElem.dAngle[nIJ];
    rData.PscnD.dAw        = PscnElem.dAw[nIJ];   
    rData.PscnD.dfy        = PscnElem.dfy[nIJ];   
    rData.PscnD.dAe        = PscnElem.dAe[nIJ];
    rData.PscnD.ds         = PscnElem.ds[nIJ];
    rData.PscnD.dLs        = PscnElem.dLs[nIJ];

	//IRS Data
	rData.PscnD_IRS.iPosition  = nIJ; // 0=I, 1=J  
	rData.PscnD_IRS.nFaceState = PscnElem.nFaceState[nIJ];
	rData.PscnD_IRS.dSL    = PscnElem.IRS_SConn.dSL[nIJ]   ;
	rData.PscnD_IRS.dASL    = PscnElem.IRS_SConn.dASL[nIJ]  ;
	rData.PscnD_IRS.dSt     = PscnElem.IRS_SConn.dSt[nIJ]   ;
	rData.PscnD_IRS.dAe     = PscnElem.IRS_SConn.dAe[nIJ]   ;
	rData.PscnD_IRS.dFck    = PscnElem.IRS_SConn.dFck[nIJ]  ;
	rData.PscnD_IRS.dFy     = PscnElem.IRS_SConn.dFy[nIJ]   ;
}

void CDgnPscCommon::ConvertPscnData_IRS(int nIJ, const _PSC_PSCN_IRS_ELEM& PscnElem, PSC_DATA_POSI& rData)
{
    
	//rData.PscnD_IRS.iPosition  = nIJ; // 0=I, 1=J  
	//rData.PscnD_IRS.nFaceState = PscnElem.nFaceState[nIJ];
	//rData.PscnD_IRS.dSL    = PscnElem.dSL[nIJ]   ;
	//rData.PscnD_IRS.dASL    = PscnElem.dASL[nIJ]  ;
	//rData.PscnD_IRS.dSt     = PscnElem.dSt[nIJ]   ;
	//rData.PscnD_IRS.dAe     = PscnElem.dAe[nIJ]   ;
	//rData.PscnD_IRS.dFck    = PscnElem.dFck[nIJ]  ;
	//rData.PscnD_IRS.dFy     = PscnElem.dFy[nIJ]   ;
    
}

void CDgnPscCommon::ConvertPscCalcInfo(const _PSC_CINF_ELEM* pCinfD, PSC_DATA_POSI& rData)
{
    rData.CalcD.nClass = pCinfD->nClass;
}

void CDgnPscCommon::ConvertPscsData(int nIJ, const _PSC_PSCS_ELEM& PscsElem, PSC_DATA_POSI& rData)
{
    rData.PscsD.iPosition   = nIJ; // 0=I, 1=J  
    rData.PscsD.nFaceState  = PscsElem.nFaceState[nIJ];
    rData.PscsD.dBvi        = PscsElem.dBvi[nIJ];
    rData.PscsD.dAvf        = PscsElem.dAvf[nIJ];   
    rData.PscsD.dfy         = PscsElem.dfy[nIJ]; 
    rData.PscsD.dAlphaf     = PscsElem.dAlphaf[nIJ];
}

void CDgnPscCommon::ConvertFackData(int nIJ, const _PSC_FACK_ELEM& FackElem, PSC_DATA_POSI& rData)
{

    rData.FackD.iPosition  = nIJ; // 0=I, 1=J  
    rData.FackD.dt0     = FackElem.dt0[nIJ];

}


void CDgnPscCommon::ConvertPscCalcTndnPropData(const _PSC_TNDN_PROP *pTnptD, PSC_TNDN_PROP &rData)
{
    if (pTnptD==NULL) return;

    rData.strTendonTypeName = pTnptD->strTendonTypeName;
    rData.nTendonType       = pTnptD->nTendonType;
    rData.dTendonArea       = pTnptD->dTendonArea;
    rData.dDuctArea         = pTnptD->dDuctArea;
    rData.dUltimateStrength = pTnptD->dUltimateStrength;
    rData.dYieldStrength    = pTnptD->dYieldStrength;
    rData.nLoadType         = pTnptD->nLoadType;
    rData.nTypeId           = pTnptD->nTypeId;
    rData.nBondType         = pTnptD->nBondType;
    rData.nRelaxMethod      = pTnptD->nRelaxMethod;
    rData.dRelaxRatio       = pTnptD->dRelaxRatio;
}

void CDgnPscCommon::ConvertPscCalcTndnPropInfo(const _PSC_TNDN_PTIF *pTnptInfo, PSC_TNDN_PTIF &rData)
{
    if (pTnptInfo==NULL) return;
    int nPropSize = pTnptInfo->aTndnProp.GetSize();
    if (nPropSize>0) rData.aTndnProp.SetSize(nPropSize);
    for (int i=0; i<nPropSize; ++i)
    {
        ConvertPscCalcTndnPropData(&pTnptInfo->aTndnProp[i], rData.aTndnProp[i]);
    }
}

void CDgnPscCommon::ConvertPscLengthFromEnd(_PSC_SPBF_POSI*pSpbf, PSC_EINF_POSI& rData)
{
    if ( pSpbf==NULL ) return;
    rData.bFromI = pSpbf->bFromI ? true : false;
    rData.dLengthFromEnd = pSpbf->dLengthFromEnd;
}

void CDgnPscCommon::ConvertReinforcedSectInfo(int nPos, const T_SECT_D& rSectD, CSG_SECT_POSD& rData)
{
    if (rData.SectInfo.bIsTapered)
    {
        if (nPos == 0)
        {
            ConvertReinforcedSectRivetProp(rSectD.SectBefore.SectI.RivetStiff, rData);
        }
        else
        {
            ConvertReinforcedSectRivetProp(rSectD.SectBefore.SectJ.RivetStiff, rData);
        }
    }
    else
    {
        ConvertReinforcedSectRivetProp(rSectD.SectBefore.SectI.RivetStiff, rData);
    }

}

void CDgnPscCommon::ConvertReinforcedSectRivetProp(const T_SECT_RIVET_STIFF_D& ReinSect, CSG_SECT_POSD& rData)
{
    ST_SECT_SECTBASE_D& SectB = rData.SectInfo.Before;


    auto _L_EnSectPos = [&](int nPos) -> enReinforcedPositionOfSect
        {
            switch (nPos)
            {
            case 0: return enReinforcedPositionOfSect::EN_REIN_SECT_POS_1_WTL;
            case 1: return enReinforcedPositionOfSect::EN_REIN_SECT_POS_2_WBL;
            case 2: return enReinforcedPositionOfSect::EN_REIN_SECT_POS_3_TL;
            case 3: return enReinforcedPositionOfSect::EN_REIN_SECT_POS_4_BL;
            case 4: return enReinforcedPositionOfSect::EN_REIN_SECT_POS_5_WTR;
            case 5: return enReinforcedPositionOfSect::EN_REIN_SECT_POS_6_WBR;
            case 6: return enReinforcedPositionOfSect::EN_REIN_SECT_POS_7_TR;
            case 7: return enReinforcedPositionOfSect::EN_REIN_SECT_POS_8_BR;
            default: ASSERT(0); return enReinforcedPositionOfSect::EN_REIN_SECT_POS_1_WTL;
            }
        };

	auto _L_RivetPos = [&](int nPos, bool bIsFlange, CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetChk) -> void
		{            
            for (int i = 0; i < ReinSect.aRivetPosD.GetSize(); ++i)
            {
                T_SECT_RIVET_POS_D RivetB = ReinSect.aRivetPosD[i];
                if (RivetB.nAngle == nPos)
                {
                    if (bIsFlange && RivetB.nComponent != 2)
                    {
                        RivetB.bUser = TRUE; // if를 만족하면 데이터가 유효한 걸로.. 나중에는 UI에서 고려해줘야함
                        aRivetChk.Add(RivetB);
                    }
					else if (!bIsFlange && RivetB.nComponent == 2)
					{
                        RivetB.bUser = TRUE;
                        aRivetChk.Add(RivetB);
					}
                }
            }
        };

    auto _L_ConvertToRivet = [&](const T_SECT_RIVET_POS_D& RivPos, ST_RIVET_PLATE_B& rData) -> void
        {
            rData.bRivet = RivPos.bUser;
            rData.nRefPos = RivPos.nRefPos;
            rData.dDia = RivPos.dDia;
            rData.dSpace = RivPos.dDist;
        };

    auto _L_RivetPart = [&](int nPos, enReinforcedPositionOfSect enPart, CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetChk) -> void
        {
            for (int i = 0; i < ReinSect.aRivetPosD.GetSize(); ++i)
            {
                T_SECT_RIVET_POS_D RivetB = ReinSect.aRivetPosD[i];
                if (RivetB.nAngle == nPos)
                {
                    switch (enPart)
                    {
                    case EN_REIN_SECT_PART_FLG_TL:
                    {
                        if (RivetB.nComponent == 0)
                        {
                            aRivetChk.Add(RivetB);
                        }
                    }
                    break;
                    case EN_REIN_SECT_PART_FLG_TR:
                    {
                        if (RivetB.nComponent == 1)
                        {
                            aRivetChk.Add(RivetB);
                        }
                    }
                    break;
                    case EN_REIN_SECT_PART_WEB:
                    {
                        if (RivetB.nComponent == 2)
                        {
                            aRivetChk.Add(RivetB);
                        }
                    }
                    break;
                    case EN_REIN_SECT_PART_FLG_BL:
                    {
                        if (RivetB.nComponent == 3)
                        {
                            aRivetChk.Add(RivetB);
                        }
                    }
                    break;
                    case EN_REIN_SECT_PART_FLG_BR:
                    {
                        if (RivetB.nComponent == 4)
                        {
                            aRivetChk.Add(RivetB);
                        }
                    }
                    break;
                    default:
						ASSERT(0);
                        break;
                    }
                }
            }
        };


    auto _L_LPos = [&](int nPos) -> int
        {
            int nLPos = nPos;
            if (ReinSect.bSymmetric)
            {
                if (nLPos > 3)
                {
                    nLPos = nPos - 4;
                }
            }
            return nLPos;
        };

    for (int i = 0; i < ReinSect.aStiffPosD.GetSize(); ++i)
    {
        const T_SECT_STIFF_POS_D& RSectB = ReinSect.aStiffPosD[i];

        for (int j = 0; j < ReinSect.aStiffShape.GetSize(); ++j)
        {
            const T_STIFF_SHAPE_D& RShape = ReinSect.aStiffShape[j];
            // TL=1, TR=2, BR=3, BR=4
            if (RSectB.strShapeName.CompareNoCase(RShape.strName) == 0)
            {
                ST_REINFORCED_SECT_B ReinS;
                memcpy(ReinS.Size, RShape.dSize, sizeof(RShape.dSize));
                memcpy(ReinS.CorrSize, RShape.dSize, sizeof(RShape.dSize));

                ReinS.Shape = DGN_SECT_SHAPE_REG_L;
                ReinS.SName = RSectB.strShapeName;
                ReinS.enPos = _L_EnSectPos(i);
                ReinS.bUse  = RSectB.bUseStiff;

                int nLPos = _L_LPos(i);

                CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aFlgRivetChk;
                _L_RivetPos(nLPos, true, aFlgRivetChk);
				for (int k = 0; k < aFlgRivetChk.GetSize(); ++k)
				{
					ST_RIVET_PLATE_B RivetB;
					_L_ConvertToRivet(aFlgRivetChk[k], RivetB);
					ReinS.Rivet.aFlange.Add(RivetB);
				}
                CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aWebRivetChk;
                _L_RivetPos(nLPos, false, aWebRivetChk);
                for (int k = 0; k < aWebRivetChk.GetSize(); ++k)
                {
                    ST_RIVET_PLATE_B RivetB;
                    _L_ConvertToRivet(aWebRivetChk[k], RivetB);
                    ReinS.Rivet.aWeb.Add(RivetB);
                }

                T_SECT_D CalcS; CalcS.Initialize();
                T_SECT_STIFFNESS& StfnStiff = CalcS.SectBefore.SectI.Stiffness;
                CalcS.nStype = D_SECT_TYPE_REGULAR;
                memcpy(CalcS.SectBefore.SectI.Size, RShape.dSize, sizeof(RShape.dSize));
                if (RShape.nType == 3)
                {
                    CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_L;
                }
                else
                    ASSERT(0);
                CSectUtil::CalcSectData(CalcS);
                ConvertSectStiffnessData(StfnStiff, ReinS.Stiffness);
                ConvertSectStiffnessData(StfnStiff, ReinS.CorrStiffness);

                SectB.aReinforcedSect.Add(ReinS);
            }
        }
    }

    // Angle과 겹치지 않는 Flange or Web의 Rivet
	enReinforcedPositionOfSect aenReinPart[5] = {
		EN_REIN_SECT_PART_FLG_TL,
		EN_REIN_SECT_PART_FLG_TR,
		EN_REIN_SECT_PART_WEB,
		EN_REIN_SECT_PART_FLG_BL,
		EN_REIN_SECT_PART_FLG_BR
	};

    for (int i = 0; i < 5; ++i) // Part 5개
    {        
        int nNone = 100;
        CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aRivetPart;
        _L_RivetPart(nNone, aenReinPart[i], aRivetPart);
		int nRivetPartSize = aRivetPart.GetSize();
        if (nRivetPartSize > 0)
        {
            ST_RIVET_PART_B RivetPartD;
			RivetPartD.enPos = aenReinPart[i];
            for (int j = 0; j < aRivetPart.GetSize(); ++j)
            {
                ST_RIVET_PLATE_B RivetB;
                _L_ConvertToRivet(aRivetPart[j], RivetB);
                RivetPartD.aRivet.Add(RivetB);
            }
			SectB.aRivetPart.Add(RivetPartD);   
        }
    }
}

void CDgnPscCommon::ConvertCorrosionSectInfo(int nPos, const T_SECT_D& rSectD, const T_CRDP_SSM_D& rCrdp, CSG_SECT_POSD& rData)
{
    T_SECT_D TSect = rSectD;
    T_CRDP_SSM_D TCrdp = rCrdp;

    BOOL bSectI = TRUE;
    if (rData.SectInfo.bIsTapered && nPos == 1) bSectI = FALSE;

    int nCrdpIdx = 0;
    if (!TCrdp.bSameIJ && nPos == 1) nCrdpIdx = 1;

    T_SECT_STIFFNESS Stiffness; Stiffness.Initialize();
    if (TCrdp.CrdpSub[nPos].GetSize() > 0)
    {
        CSectUtil::CalcSectDataWidhRivet(TSect, TCrdp);
        Stiffness = (nPos == 0) ? TSect.SectBefore.SectI.Stiffness : TSect.SectBefore.SectJ.Stiffness;
    }
    else
    {
        Stiffness = (bSectI) ? TSect.SectBefore.SectI.Stiffness : TSect.SectBefore.SectJ.Stiffness;
    }

    ConvertSectStiffnessData(Stiffness, rData.SectInfo.Before.CorrStiffness);

}

void CDgnPscCommon::ConvertCorrosionSectBase(int nPos, const T_SECT_D& rSectD, const T_CRDP_SSM_D& rCrdp, CSG_SECT_POSD& rData)
{

}


int CDgnPscCommon::GetConvertCodeCvlToEng(int nDgnCode)
{
    int nPscCode=0;
    switch(nDgnCode)
    {
    case JSCE02:                  nPscCode = DGN_JSCE02;               break;
    case KSCE_USD03:              nPscCode = DGN_KSCE_USD03;           break;
    case KSCE_USD05:              nPscCode = DGN_KSCE_USD05;           break;
    case KSCE_USD10:              nPscCode = DGN_KSCE_USD10;           break;
    case KSCE_RAIL_USD04:         nPscCode = DGN_KSCE_RAIL_USD04;      break;
    case KSCE_RAIL_USD11:         nPscCode = DGN_KSCE_RAIL_USD11;      break;
    case JTG_D62_04:              nPscCode = DGN_JTG_D62_04;           break;
    case TB_10002_3_05:           nPscCode = DGN_TB_10002_3_05;        break;
    case AASHTO_LRFD08:           nPscCode = DGN_AASHTO_LRFD08;        break;
    case EUROCODE2_2_05_PSC:      nPscCode = DGN_EUROCODE2_2_05_PSC;   break;
    case IRC_112_2011_PSC:	      nPscCode = DGN_IRC_112_2011_PSC;     break;
    case IRC_112_2020_PSC:	      nPscCode = DGN_IRC_112_2020_PSC;     break;
    case AS_5100_5_17_PSC:        nPscCode = DGN_AS_5100_5_17_PSC;     break;
    case KSCE_LSD15_PSC:          nPscCode = DGN_KSCE_LSD15_PSC;       break;
    case KDS_24_14_21_2022_PSC:   nPscCode = DGN_KDS_24_14_21_2021_PSC;break;
    case AASHTO_LRFD12:           nPscCode = DGN_AASHTO_LRFD12;        break;
    case AASHTO_LRFD14:           nPscCode = DGN_AASHTO_LRFD14;        break;  
    case AASHTO_LRFD16:           nPscCode = DGN_AASHTO_LRFD16;        break;  
    case AASHTO_LRFD17:           nPscCode = DGN_AASHTO_LRFD18;        break;  
    case AASHTO_LRFD20:           nPscCode = DGN_AASHTO_LRFD20;        break;  
    case AASHTO_LRFD24:           nPscCode = DGN_AASHTO_LRFD24;        break;
    case AREMA_23:                nPscCode = DGN_AREMA_23_PSC;         break;
    case CSA_S6_10:               nPscCode = DGN_CSA_S6_10;            break;
    case CSA_S6_14:               nPscCode = DGN_CSA_S6_14;            break;
	case CSA_S6_19:               nPscCode = DGN_CSA_S6_19;            break;
    case SNiP_20503_84_PSC:       nPscCode = DGN_SNiP_20503_84PSC;     break;
    case SP_35_13330_11_PSC:      nPscCode = DGN_SP_35_13330_11PSC;    break;
    case SNiP_20503_84_PSC_MKS:   nPscCode = DGN_SNiP_20503_84PSC_MKS;    break;
    case SP_35_13330_11_PSC_MKS:  nPscCode = DGN_SP_35_13330_11PSC_MKS;   break;
	case IRS_PSC:				  nPscCode = DGN_IRS_PSC;    break;
	case TMH07_3_1989:            nPscCode = DGN_TMH07_3_1989;    break; ///TODO : after Update DGN_TMH07_3_1989 in DgnEngine.
    case BS5400_90_PSC:           nPscCode = DGN_BS5400_90_PSC;       break;
    case BD_44_15_PSC:            nPscCode = DGN_BD_44_15_PSC;        break;
    case CS_455_PSC:              nPscCode = DGN_CS_455_PSC;          break;
    case ABNT_NBR_6118_2023_PSC:  nPscCode = DGN_ABNT_NBR_6118_2023_PSC; break;
    default:
        ASSERT(0);
    }
    return nPscCode;
}

int CDgnPscCommon::GetConvertCodeEngToCvl(int nDgnCode)
{
    int nPscCode=0;
    switch(nDgnCode)
    {
    case DGN_JSCE02:                  nPscCode = JSCE02;                  break;
    case DGN_KSCE_USD03:              nPscCode = KSCE_USD03;              break;
    case DGN_KSCE_USD05:              nPscCode = KSCE_USD05;              break;
    case DGN_KSCE_USD10:              nPscCode = KSCE_USD10;              break;
    case DGN_KSCE_RAIL_USD04:         nPscCode = KSCE_RAIL_USD04;         break;
    case DGN_KSCE_RAIL_USD11:         nPscCode = KSCE_RAIL_USD11;         break;
    case DGN_JTG_D62_04:              nPscCode = JTG_D62_04;              break;
    case DGN_TB_10002_3_05:           nPscCode = TB_10002_3_05;           break;
    case DGN_AASHTO_LRFD08:           nPscCode = AASHTO_LRFD08;           break;
    case DGN_EUROCODE2_2_05_PSC:      nPscCode = EUROCODE2_2_05_PSC;      break;
    case DGN_IRC_112_2011_PSC:	      nPscCode = IRC_112_2011_PSC;        break;
    case DGN_IRC_112_2020_PSC:	      nPscCode = IRC_112_2020_PSC;        break;
    case DGN_AS_5100_5_17_PSC:	      nPscCode = AS_5100_5_17_PSC;        break;
    case DGN_KSCE_LSD15_PSC:          nPscCode = KSCE_LSD15_PSC;          break;
    case DGN_KDS_24_14_21_2021_PSC:   nPscCode = KDS_24_14_21_2022_PSC;   break;
    case DGN_AASHTO_LRFD12:           nPscCode = AASHTO_LRFD12;           break;
    case DGN_AASHTO_LRFD14:           nPscCode = AASHTO_LRFD14;           break;  
    case DGN_AASHTO_LRFD16:           nPscCode = AASHTO_LRFD16;           break;  
    case DGN_AASHTO_LRFD18:           nPscCode = AASHTO_LRFD17;           break;  
    case DGN_AASHTO_LRFD20:           nPscCode = AASHTO_LRFD20;           break;  
	case DGN_AASHTO_LRFD24:           nPscCode = AASHTO_LRFD24;           break;
    case DGN_CSA_S6_10:               nPscCode = CSA_S6_10;               break;
    case DGN_CSA_S6_14:               nPscCode = CSA_S6_14;               break;
	case DGN_CSA_S6_19:               nPscCode = CSA_S6_19;               break;
    case DGN_SNiP_20503_84PSC:        nPscCode = SNiP_20503_84_PSC;       break;
    case DGN_SP_35_13330_11PSC:       nPscCode = SP_35_13330_11_PSC;      break;
    case DGN_SNiP_20503_84PSC_MKS:    nPscCode = SNiP_20503_84_PSC_MKS;   break;
    case DGN_SP_35_13330_11PSC_MKS:   nPscCode = SP_35_13330_11_PSC_MKS;  break;
	case DGN_IRS_PSC:				  nPscCode = IRS_PSC;                 break;
    case DGN_BS5400_90_PSC:           nPscCode = BS5400_90_PSC;           break;
	case DGN_TMH07_3_1989:             nPscCode = TMH07_3_1989;           break;
    case DGN_BD_44_15_PSC:            nPscCode = BD_44_15_PSC;            break;
    case DGN_CS_455_PSC:              nPscCode = CS_455_PSC;              break;
	case DGN_ABNT_NBR_6118_2023_PSC:  nPscCode = ABNT_NBR_6118_2023_PSC;  break;
    default:
        ASSERT(0);
    }
    return nPscCode;
}

int CDgnPscCommon::GetConvertCSGCodeCvlToEng(int nDgnCode)
{
    int nCsgCode=0;
    switch(nDgnCode)
    {
        //case EUROCODE4_2_04_CSG:     nCsgCode = ;              break;
    case KDS_24_14_31_2018_CSG: nCsgCode = CSG_KDS_24_14_31_2018;     break;
    case KSCE_LSD15_CSG:        nCsgCode = CSG_KSCE_LSD12;            break;
    case KSSC_2014_CSG:         nCsgCode = CSG_KSSC_LSD2014;          break;
    case AASHTO_LRFD20_CSG:     nCsgCode = CSG_AASHTO_LRFD20;         break;
    case AASHTO_LRFD17_CSG:     nCsgCode = CSG_AASHTO_LRFD18;         break;
    case AASHTO_LRFD16_CSG:     nCsgCode = CSG_AASHTO_LRFD16;         break;
    case AASHTO_LRFD12_CSG:     nCsgCode = CSG_AASHTO_LRFD12;         break;
    case AASHTO_LRFD07_CSG:     nCsgCode = CSG_AASHTO_LRFD07;         break;
    case KSCE_ASD10_CSG:        nCsgCode = CSG_KSCE_ASD10;            break;
    case KSCE_RAIL_ASD11_CSG:   nCsgCode = CSG_KSCE_RAIL_ASD11;       break;
    case SNiP_20503_84_CSG:     nCsgCode = CSG_RUS_SNiP20503_84;      break;
    case SP_35_13330_11_CSG:    nCsgCode = CSG_RUS_SP35_11;           break;
    case CSA_S6_10_CSG:         nCsgCode = CSG_CSA_S6_10;             break;
    case CSA_S6_14_CSG:         nCsgCode = CSG_CSA_S6_14;             break;
    case CSA_S6_19_CSG:         nCsgCode = CSG_CSA_S6_19;             break;
    case CS457_R1_CSG:          nCsgCode = CSG_CS_457_R1;             break;
    case NR_GN_CIV_025_06_CSG:  nCsgCode = CSG_NR_GN_CIV_025_06;      break;
    case BS5400_3_00_CSG:       nCsgCode = CSG_BS5400_3_00;           break;
	case AS5100_6_17_CSG:       nCsgCode = CSG_AS5100_6_17;           break;
	default: ASSERT(0); break;
    }
    return nCsgCode;

}

int CDgnPscCommon::GetConvertCSGCodeEngToCvl(int nDgnCode)
{
    int nCsgCode=0;
    switch(nDgnCode)
    {
        //case EUROCODE4_2_04_CSG:     nCsgCode = ;              break;
    case CSG_KDS_24_14_31_2018:           nCsgCode = KDS_24_14_31_2018_CSG; break;
    case CSG_KSCE_LSD12:                  nCsgCode = KSCE_LSD15_CSG;        break;
    case CSG_KSSC_LSD2014:                nCsgCode = KSSC_2014_CSG ;        break;
    case CSG_AASHTO_LRFD20:               nCsgCode = AASHTO_LRFD20_CSG;     break;
    case CSG_AASHTO_LRFD18:               nCsgCode = AASHTO_LRFD17_CSG;     break;
    case CSG_AASHTO_LRFD16:               nCsgCode = AASHTO_LRFD16_CSG;     break;
    case CSG_AASHTO_LRFD12:               nCsgCode = AASHTO_LRFD12_CSG;     break;
    case CSG_AASHTO_LRFD07:               nCsgCode = AASHTO_LRFD07_CSG;     break;
    case CSG_KSCE_ASD10:                  nCsgCode = KSCE_ASD10_CSG;        break;
    case CSG_KSCE_RAIL_ASD11:             nCsgCode = KSCE_RAIL_ASD11_CSG;   break;
    case CSG_RUS_SNiP20503_84:            nCsgCode = SNiP_20503_84_CSG;     break;
    case CSG_RUS_SP35_11:                 nCsgCode = SP_35_13330_11_CSG;    break;
    case CSG_CSA_S6_10:                   nCsgCode = CSA_S6_10_CSG;         break;
    case CSG_CSA_S6_14:                   nCsgCode = CSA_S6_14_CSG;         break;
	case CSG_CSA_S6_19:                   nCsgCode = CSA_S6_19_CSG;         break;
    case CSG_CS_457_R1:                   nCsgCode = CS457_R1_CSG;          break;
    case CSG_NR_GN_CIV_025_06:            nCsgCode = NR_GN_CIV_025_06_CSG;  break;
    case CSG_BS5400_3_00:                 nCsgCode = BS5400_3_00_CSG;       break;
	case CSG_AS5100_6_17:                 nCsgCode = AS5100_6_17_CSG;       break;
    default: ASSERT(0); break;
    }
    return nCsgCode;

}

int CDgnPscCommon::GetConvertSodCodeCvlToEng(int nDgnCode)
{
    int nSodCode=0;
    switch(nDgnCode)
    {
    case SNiP_20503_84_SOD:    nSodCode = SOD_SNiP_20503_84;    break;
    case SP_35_13330_11_SOD:   nSodCode = SOD_SP_35_13330_11;   break;
        //case SNiP_20503_84_SOD_MKS:   nSodCode = SOD_SNiP_20503_84_MKS;   break;
        //case SP_35_13330_11_SOD_MKS:      nSodCode = SOD_SP_35_13330_11_MKS;      break;
    default: ASSERT(0); break;
    }
    return nSodCode;
}

int CDgnPscCommon::GetConvertSodCodeEngToCvl(int nDgnCode)
{
    int nSodCode=0;
    switch(nDgnCode)
    {
    case SOD_SNiP_20503_84:    nSodCode = SNiP_20503_84_SOD;    break;
    case SOD_SP_35_13330_11:   nSodCode = SP_35_13330_11_SOD;   break;
        //case SOD_SNiP_20503_84_MKS:   nSodCode = SNiP_20503_84__SOD_MKS;   break;
        //case SOD_SP_35_13330_11_MKS:      nSodCode = SOD_SP_35_13330_11__SOD_MKS;      break;
    default: ASSERT(0); break;
    }
    return nSodCode;
}

double CDgnPscCommon::MinWithoutZero(const double &dVal1, const double &dVal2, const double &dVal3)
{
    double dMinVal = 0.0;
    BOOL bFlag = FALSE;

    if (dVal1 != 0.0)
    {
        dMinVal = dVal1;
        bFlag = TRUE;
    }

    if (dVal2 != 0.0)
    {
        if (bFlag)
        {
            dMinVal = min(dVal1, dVal2);
        }
        else
        {
            dMinVal = dVal2;
            bFlag = TRUE;
        }
    }

    if (dVal3 != 0.0)
    {
        if (bFlag == TRUE)
        {
            dMinVal = min(dMinVal, dVal3);
        }
        else
        {
            dMinVal = dVal3;
        }
    }

    return dMinVal;
}

int CDgnPscCommon::GetConvertSectTypeCvlToEng(int nSectType)
{
    int nSType=0;
    switch(nSectType)
    {
    case D_SECT_TYPE_TAPERED:        nSType = DGN_SECT_TYPE_TAPERED;   break;
    case D_SECT_TYPE_COMPO_I:        nSType = DGN_SECT_TYPE_COMPO_I;   break;
    case D_SECT_TYPE_COMPO_B:        nSType = DGN_SECT_TYPE_COMPO_B;   break;			
    case D_SECT_TYPE_COMPO_TUB:      nSType = DGN_SECT_TYPE_COMPO_TUB; break;
    case D_SECT_TYPE_STLG_I:         nSType = DGN_SECT_TYPE_STL_GIRDER_I; break;
    case D_SECT_TYPE_STLG_B:         nSType = DGN_SECT_TYPE_STL_GIRDER_B; break;
    case D_SECT_TYPE_COMPO_STLG_I:   nSType = DGN_SECT_TYPE_COMPO_STLG_I; break;
    case D_SECT_TYPE_COMPO_STLG_B:   nSType = DGN_SECT_TYPE_COMPO_STLG_B; break;
    case D_SECT_TYPE_COMPO_STLG_TUB: nSType = DGN_SECT_TYPE_COMPO_STLG_TUB; break;
    case D_SECT_TYPE_STLG_MCELL:     nSType = DGN_SECT_TYPE_STLG_MCELL; break;
    case D_SECT_TYPE_REGULAR:        nSType = DGN_SECT_TYPE_REGULAR; break;
    default: ASSERT(0); break;
    }
    return nSType;
}

CString CDgnPscCommon::GetConvertSectShapeCvlToEng(CString &strShape)
{
    CString strSectShape;
    if      (strShape==D_SECT_SHAPE_COMPO_B)        strSectShape = DGN_SECT_SHAPE_COMPO_B;
    else if (strShape==D_SECT_SHAPE_COMPO_I)        strSectShape = DGN_SECT_SHAPE_COMPO_I;
    else if (strShape==D_SECT_SHAPE_COMPO_TUB)      strSectShape = DGN_SECT_SHAPE_COMPO_TUB;
    else if (strShape==D_SECT_SHAPE_STLG_I)         strSectShape = DGN_SECT_SHAPE_STL_GIRDER_I;
    else if (strShape==D_SECT_SHAPE_STLG_B)         strSectShape = DGN_SECT_SHAPE_STL_GIRDER_BOX;
    else if (strShape==D_SECT_SHAPE_COMPO_STLG_I)   strSectShape = DGN_SECT_SHAPE_COMPO_STLG_I;
    else if (strShape==D_SECT_SHAPE_COMPO_STLG_B)   strSectShape = DGN_SECT_SHAPE_COMPO_STLG_B;
    else if (strShape==D_SECT_SHAPE_COMPO_STLG_TUB) strSectShape = DGN_SECT_SHAPE_COMPO_STLG_TUB;
    else if (strShape==D_SECT_SHAPE_STLG_MCELL)     strSectShape = DGN_SECT_SHAPE_STLG_MCELL;
    else if (strShape==D_SECT_SHAPE_REG_L)          strSectShape = DGN_SECT_SHAPE_REG_L;
    else if (strShape==D_SECT_SHAPE_REG_C)          strSectShape = DGN_SECT_SHAPE_REG_C;
    else if (strShape==D_SECT_SHAPE_REG_H)          strSectShape = DGN_SECT_SHAPE_REG_H;
    else if (strShape==D_SECT_SHAPE_REG_T)          strSectShape = DGN_SECT_SHAPE_REG_T;
    else if (strShape==D_SECT_SHAPE_REG_B)          strSectShape = DGN_SECT_SHAPE_REG_B;
    else if (strShape==D_SECT_SHAPE_REG_P)          strSectShape = DGN_SECT_SHAPE_REG_P;

    return strSectShape;
}

BOOL CDgnPscCommon::IsSecTypeI(int nSectType)
{
    switch (nSectType)
    {
    case DGN_SECT_TYPE_COMPO_I:
    case DGN_SECT_TYPE_STL_GIRDER_I:
        return TRUE;
    default: 
        break;
    }

    return FALSE;
}

BOOL CDgnPscCommon::IsSectTypeWith2Web(int nSectType)
{
    switch (nSectType)
    {
    case DGN_SECT_TYPE_COMPO_B:
    case DGN_SECT_TYPE_COMPO_TUB:
    case DGN_SECT_TYPE_STL_GIRDER_B:
    case DGN_SECT_TYPE_COMPO_STLG_B:
    case DGN_SECT_TYPE_COMPO_STLG_TUB:
        return TRUE;
    default:
        break;
    }

    return FALSE;
}

void CDgnPscCommon::ConvertCsgCalcUlmForce( T_STRB_D &MembD, CSG_ULMF_D &rData ) const
{
    rData.ForD[0].Initialize();
    rData.ForD[1].Initialize();
    rData.ForD[2].Initialize();
    for(int nIdx =0; nIdx < 6; nIdx++)
    {
        rData.ForD[0].dForce[nIdx] = MembD.dblForce[0][nIdx];   //Part1의 I단 - I
        
        for ( int j = 1; j<3; ++j )
        {
            rData.ForD[1].dForce[nIdx] = AbsMax(rData.ForD[1].dForce[nIdx], MembD.dblForce[j][nIdx]); //Part2의 J단 - M
        }
        rData.ForD[2].dForce[nIdx] = MembD.dblForce[3][nIdx+6]; //Part4의 J단 - J

		rData.For13D[0].dForce[nIdx] = AbsMax(MembD.dblForce[0][nIdx+6], MembD.dblForce[1][nIdx]);  // 1/4
		rData.For13D[1].dForce[nIdx] = AbsMax(MembD.dblForce[2][nIdx+6], MembD.dblForce[3][nIdx]); // 3/4
	}

    int nFor = 6;
    int nIJ  = 0;
    rData.ForD[0].dForce[nFor + 0] = MembD.dblForce[0][Mbi_Id + nIJ];   //Part1의 I단 - I
    rData.ForD[0].dForce[nFor + 1] = MembD.dblForce[0][Mst_Id + nIJ];   
    rData.ForD[0].dForce[nFor + 2] = MembD.dblForce[0][Mwp_Id + nIJ];   
    //
    // 1-j단, 2-i단 중 큰 값으로 할당하자.
    for ( int j = 1; j<3; ++j )
    {
        for ( int k =0; k<2; ++k )
        {
            rData.ForD[1].dForce[nFor + 0] = AbsMax(rData.ForD[1].dForce[nFor + 0], MembD.dblForce[j][Mbi_Id + k]); //Part2의 J단 - M
            rData.ForD[1].dForce[nFor + 1] = AbsMax(rData.ForD[1].dForce[nFor + 1], MembD.dblForce[j][Mst_Id + k]);
            rData.ForD[1].dForce[nFor + 2] = AbsMax(rData.ForD[1].dForce[nFor + 2], MembD.dblForce[j][Mwp_Id + k]);
        }
		rData.For13D[0].dForce[nFor + 0] = AbsMax(MembD.dblForce[0][Mbi_Id + 1], MembD.dblForce[1][Mbi_Id + 0]); 
		rData.For13D[0].dForce[nFor + 1] = AbsMax(MembD.dblForce[0][Mst_Id + 1], MembD.dblForce[1][Mst_Id + 0]);
		rData.For13D[0].dForce[nFor + 2] = AbsMax(MembD.dblForce[0][Mwp_Id + 1], MembD.dblForce[1][Mwp_Id + 0]);
		//
		rData.For13D[1].dForce[nFor + 0] = AbsMax(MembD.dblForce[2][Mbi_Id + 1], MembD.dblForce[3][Mbi_Id + 0]);
		rData.For13D[1].dForce[nFor + 1] = AbsMax(MembD.dblForce[2][Mst_Id + 1], MembD.dblForce[3][Mst_Id + 0]);
		rData.For13D[1].dForce[nFor + 2] = AbsMax(MembD.dblForce[2][Mwp_Id + 1], MembD.dblForce[3][Mwp_Id + 0]);
	}
    //
    nIJ = 1;
    rData.ForD[2].dForce[nFor + 0] = MembD.dblForce[3][Mbi_Id + nIJ]; //Part4의 J단 - J
    rData.ForD[2].dForce[nFor + 1] = MembD.dblForce[3][Mst_Id + nIJ];
    rData.ForD[2].dForce[nFor + 2] = MembD.dblForce[3][Mwp_Id + nIJ];
}

void CDgnPscCommon::ConvertCsgCalcUlmForceCb(T_STRB_D &MembD, int nMid, int nCom, CSG_ULMF_D &rData) const
{
    for ( int nIdx =0; nIdx < 6; nIdx++ )
    {
        rData.CbForce[0].dForce[nIdx] = MembD.dblForce[0][nIdx];   //Part1의 I단 - I
        rData.CbForce[1].dForce[nIdx] = MembD.dblForce[nMid][nIdx+nCom]; //Part2의 J단 - M
        rData.CbForce[2].dForce[nIdx] = MembD.dblForce[3][nIdx+6]; //Part4의 J단 - J
    }

    int nFor = 6;
    int nIJ  = 0;
    rData.CbForce[0].dForce[nFor + 0] = MembD.dblForce[0][Mbi_Id + nIJ];   //Part1의 I단 - I
    rData.CbForce[0].dForce[nFor + 1] = MembD.dblForce[0][Mst_Id + nIJ];
    rData.CbForce[0].dForce[nFor + 2] = MembD.dblForce[0][Mwp_Id + nIJ];
    //
    // 1-j단, 2-i단 중 큰 값으로 할당하자.
    nIJ = nMid == 1 ? 1 : 0;
    rData.CbForce[1].dForce[nFor + 0] = MembD.dblForce[nMid][Mbi_Id + nIJ]; //Part2의 J단 - M
    rData.CbForce[1].dForce[nFor + 1] = MembD.dblForce[nMid][Mst_Id + nIJ];
    rData.CbForce[1].dForce[nFor + 2] = MembD.dblForce[nMid][Mwp_Id + nIJ];
    //
    nIJ = 1;
    rData.CbForce[2].dForce[nFor + 0] = MembD.dblForce[3][Mbi_Id + nIJ]; //Part4의 J단 - J
    rData.CbForce[2].dForce[nFor + 1] = MembD.dblForce[3][Mst_Id + nIJ];
    rData.CbForce[2].dForce[nFor + 2] = MembD.dblForce[3][Mwp_Id + nIJ];
}

BOOL CDgnPscCommon::GetUlmMidIndex(T_STRB_D &MembD, int &nMid, int &nCom) const
{
	double dMy_Prej = MembD.dblForce[1][10];
	double dMy_Nxti = MembD.dblForce[2][4];
	// 1-j단, 2-i단 중 큰 값으로 할당하자.
	nMid = 1;
	nCom = 6;
	if ( fabs(dMy_Prej) < fabs(dMy_Nxti) )
	{
		nMid = 2;
		nCom = 0;
	}
	return TRUE;
}

enSupportType CDgnPscCommon::GetSupportType(BOOL bSupport, BOOL bInteriorSupport)
{
    if (!bSupport && !bInteriorSupport)
    {
        return enSupportType::EN_Support_None;
    }
    else if (bInteriorSupport)
    {
        return enSupportType::EN_Support_Interior;
    }
    else if (bSupport)
    {
        return enSupportType::EN_Support_End;
    }

	return enSupportType::EN_Support_None;
}

BOOL CDgnPscCommon::GetEndElemOfMember(ElemPairK EPairK, ElemPairK& ShowK, ElemPairK& IendK, ElemPairK& JendK) const
{
    BOOL bReverse = FALSE;
    T_ELEM_K showElemK = 0, IendElemK = 0, JendElemK = 0;
    if (EPairK.second == EN_EL_BEAM)
    {
        CArray<T_ELEM_K, T_ELEM_K> arMembElemList;
        if (m_pMembCtrl->GetElemListByIncludeElem(EPairK.first, bReverse, showElemK, arMembElemList))
        {
            int iCountElem = arMembElemList.GetSize();
            IendElemK = arMembElemList[0];
            JendElemK = arMembElemList[iCountElem - 1];
            IendK = { IendElemK, EN_EL_BEAM };
            JendK = { JendElemK, EN_EL_BEAM };
        }
    }
    else if (EPairK.second == EN_EL_VBEAM)
    {
        // V.Beam도 Member가 있어야 하는데...
        // m_pMembCtrl->GetElemListByIncludeElem(ElemK.first, bReverse, showElemK, arMembElemList)
        IendK = EPairK;
        JendK = EPairK;
    }
    else ASSERT(0);

    return TRUE;
}

BOOL CDgnPscCommon::Get_SectParam4CheckStrength(_PSC_SECT_POSI& SectPosiD, double& dbw, double* dShrThk, double* dFlgThk, double dpMomPos[][2])
{    
    dShrThk[0] = SectPosiD.dpShrThk[0];
    dShrThk[1] = SectPosiD.dpShrThk[1];
    dShrThk[2] = SectPosiD.dpShrThk[2];
    //
    dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
    //
    dFlgThk[0] = SectPosiD.dpFlgThk[0];
    dFlgThk[1] = SectPosiD.dpFlgThk[1]; 
    //
    dpMomPos[0][0] = SectPosiD.dpMomPos[2][0];
    dpMomPos[0][1] = SectPosiD.dpMomPos[2][1];
    dpMomPos[1][0] = SectPosiD.dpMomPos[3][0];
    dpMomPos[1][1] = SectPosiD.dpMomPos[3][1];   

    return TRUE;
}


BOOL CDgnPscCommon::Get_GenSectLinetypeData(T_SECT_SECTBASE_D& SectBaseD, T_GENL_DATA& rData)
{
    int iGenPartSize = SectBaseD.aGeneralPart.GetSize();
    if(iGenPartSize!=2) {ASSERT(0); return FALSE;} // General composite중 2개 구성되어 있는 것만.

    T_GSEC_PART SlabPart = SectBaseD.aGeneralPart[0]; // Slab

    int i=0,j=0;
    for(i=0; i<iGenPartSize; ++i)
    {
        T_GSEC_PART PartD = SectBaseD.aGeneralPart[i];
        if(PartD.bPlane) continue;
        int nStart = PartD.IdxStart[0];
        int nEnd = PartD.IdxEnd[0];
        rData.PolyD.aVertex.Copy(SectBaseD.aOuterPolygon[nStart].aVertex); 

        CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
        nStart = PartD.IdxStart[2]; // Line을 고려하는 범위 
        nEnd = PartD.IdxEnd[2];
        int iLineSize = nEnd - nStart;
        aLine.SetSize(iLineSize);
        // Index 0부터 들어가도록.
        for(j=0; j<iLineSize; ++j) 
        {
            int idx = nStart+j;
            T_GSEC_LINE GLine = SectBaseD.aLine[idx];
            aLine.SetAt(j, GLine);
        }

        rData.aLine.Copy(aLine);
        break; // 2개이상 Line Type이 없겠지만.. 하나만..
    }

    return TRUE;
}

BOOL CDgnPscCommon::Get_GenLineDataFromGLineDgn(const T_GLINE_DGN_D& GLineDgn, T_GENL_DATA& rData)
{
    rData.aLine.Copy(GLineDgn.aModifiedLine);
    rData.PolyD = GLineDgn.ModifiedGsecPolyD;

    return TRUE;
}

BOOL CDgnPscCommon::Get_ConvertSectBase4GenLineData(const T_SECT_SECTBASE_D& SectBaseD, const T_GLINE_DGN_D& GLineDgnD, T_SECT_SECTBASE_D& rData)
{
    int iGenPartSize = SectBaseD.aGeneralPart.GetSize();
    if(iGenPartSize!=2) {ASSERT(0); return FALSE;} // General composite중 2개 구성되어 있는 것만.

    rData = SectBaseD;

    int i=0,j=0;
    for(i=0; i<iGenPartSize; ++i)
    {
        T_GSEC_PART PartD = SectBaseD.aGeneralPart[i];
        int nStart = PartD.IdxStart[0];
        int nEnd = PartD.IdxEnd[0];
        if(nEnd-nStart>1) { ASSERT(0); return FALSE; }
        if(PartD.bPlane) 
        {
            T_GSEC_POLYGON PolyD = GLineDgnD.ModifiedSlabGsecPolyD;
            rData.aOuterPolygon.SetAt(nStart, PolyD); // slab도 수정한 데이터로.      
        }
        else
        {
            T_GSEC_POLYGON PolyD = GLineDgnD.ModifiedGsecPolyD;
            rData.aOuterPolygon.SetAt(nStart, PolyD); // Stiffener고려한 것으로 수정. 

            int iLineSize = GLineDgnD.aModifiedLine.GetSize();    
            PartD.IdxStart[2] = 0;
            PartD.IdxEnd[2] = iLineSize;
            rData.aGeneralPart.SetAt(i, PartD);
            rData.aLine.Copy(GLineDgnD.aModifiedLine);
        }    
    }


    return TRUE;
}

double CDgnPscCommon::Calc_StressOfPosition(double dFx, double dMy, double dMz, double dArea, double dIy, double dIz, double dy, double dz)
{
    if(dArea==0.0 || dIy==0.0 || dIz==0.0)
        return 0.0; 

    // Compression(-), Tension(+)
    double dStress = dFx/dArea + (-1.)*dMy*dz/dIy+ (-1.)*dMz*dy/dIz;

    return dStress;
}

BOOL CDgnPscCommon::Get_ShapeInfoOfDgnLineData(T_GLINE_DST& rData, double& db, double& dt, BOOL& bStfn)
{
    int i=0;
    int iSize = rData.aLineDgn.GetSize();
    db=0.0;
    double dt_min=0.0;
    for(i=0; i<iSize; ++i)
    {
        T_GLINE_BASE LineBase = rData.aLineDgn[i];
        double dbi = LineBase.db;
        double dti = LineBase.dt;
        db += LineBase.db;
        if(i==0)
        {
            dt_min = LineBase.dt;
        }
        else
        {
            if(dti<dt_min) dt_min = dti;
        }
    }
    dt = dt_min;

    int iStiffenerSize = rData.aStiffener.GetSize();
    bStfn = iStiffenerSize>0 ? TRUE : FALSE;

    return TRUE;  
}

BOOL CDgnPscCommon::Is_CompStress(double dStr1, double dStr2)
{
    // 압축(-), 인장(+)
    BOOL bCompStress=TRUE;
    if(dStr1*dStr2<0.0) // 한쪽이라도 압축이면.
    {
        bCompStress=TRUE;
    }
    else if(dStr1*dStr2>0.0)
    {
        if(dStr1<0.0) // 압축
        {
            bCompStress=TRUE;
        }
        else // 둘다 인장
        {
            bCompStress=FALSE;
        }
    }
    else if(dStr1*dStr2==0.0)
    {
        if(dStr1==0.0 && dStr2==0.0) 
        {
            bCompStress=FALSE; // 둘다 0이면 인장 
        }
        else if(dStr1==0.0)
        {
            if(dStr2<0.0)
                bCompStress=TRUE;
            else          
                bCompStress=FALSE;
        }
        else if(dStr2==0.0)
        {
            if(dStr1<0.0)
                bCompStress=TRUE;
            else          
                bCompStress=FALSE;
        }    
    }
    else 
        ASSERT(0);

    return bCompStress;
}

BOOL CDgnPscCommon::Is_DoubleCurvature(double dStr1, double dStr2)
{
    if(dStr1*dStr2<0.0) return TRUE;
    return FALSE;
}

int CDgnPscCommon::Get_StressDistributionType4Internal(double dStr1, double dStr2)
{
    int iType=-1;

    double dVal = dStr1*dStr2;
    if(dVal>0.0)
    {
        if(fabs(dStr1-dStr2)<m_dZero)
        {
            iType = Internal_Stress_Type1;
        }
        else
        {
            iType = Internal_Stress_Type2;
        }
    }
    else if(dVal<0.0)
    {
        iType = Internal_Stress_Type3;
    }
    else if(dStr1==0.0 && dStr2<0.0)
    {
        iType = Internal_Stress_Type2;
    }
    else if(dStr2==0.0 && dStr1<0.0)
    {
        iType = Internal_Stress_Type2;    
    }

    return iType;
}

int CDgnPscCommon::Get_StressDistributionType4Outstand(BOOL bSig1IsFree, double dStr1, double dStr2)
{
    int iType=-1;

    double dVal = dStr1*dStr2;

    iType = Get_StressDistributionType4Internal(dStr1, dStr2);
    if(bSig1IsFree) // Table 4.2의 첫번째.
    {    
        if(iType==Internal_Stress_Type1 || iType==Internal_Stress_Type2) iType = Outstand_Stress_Type1;
        else if(iType == Internal_Stress_Type3) iType = Outstand_Stress_Type2;
    }
    else
    {
        if(iType==Internal_Stress_Type1 || iType==Internal_Stress_Type2) iType = Outstand_Stress_Type3;
        else if(iType == Internal_Stress_Type3) iType = Outstand_Stress_Type4;
    }

    return iType;
}

BOOL CDgnPscCommon::Get_f1_f2(double dStr1, double dStr2, double& df1, double& df2, BOOL& bIs1_Sig1)
{
    // Stress는 원래 tension(+), compression(-)
    df1=df2=0.0;
    dStr1 *= -1.0;  
    dStr2 *= -1.0;

    if(dStr1<dStr2)
    {
        df1 = dStr2;
        df2 = dStr1;
        bIs1_Sig1 = FALSE;
    }
    else
    {
        df1 = dStr1;
        df2 = dStr2;
        bIs1_Sig1 = TRUE;
    }
    df1 *= -1.0;
    df2 *= -1.0;

    return TRUE;
}

double CDgnPscCommon::Get_traiangleRatioVal(double dval1, double dVAL1, double dVAL2)
{  
    if(dVAL1==0) return 0;
    double dval2 = (dval1/dVAL1)*dVAL2;
    return dval2;
}

double CDgnPscCommon::Get_yOfStrightLine(double da, double db, double dx)
{
    double dy = da*dx + db;
    return dy;
}

BOOL CDgnPscCommon::Get_b1b2DbCurvature(double dLength, double df1, double df2, double& db1, double& db2)
{  
    if(df1*df2>=0.0) return FALSE;
    // f1을 기준으로.
    double dVal1 = fabs(df1);
    double dVal2 = fabs(df2);

    db1 = dLength*dVal1/(dVal1+dVal2);
    db2 = dLength - db1;

    return TRUE;
}

BOOL CDgnPscCommon::Get_GirderStiffnessofCompo(BOOL bPosiI, T_SECT_D& SectD, T_SECT_STIFFNESS& rData) const
{
    BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;  
        
    if(!bTapered)
    {
        switch (SectD.nStype)
        {
        case D_SECT_TYPE_COMPO_I:
        case D_SECT_TYPE_COMPO_B:
        case D_SECT_TYPE_COMPO_TUB:
        case D_SECT_TYPE_COMPO_STLG_I:
        case D_SECT_TYPE_COMPO_STLG_B:
        case D_SECT_TYPE_COMPO_STLG_TUB:
			{
                rData = SectD.SectBefore.Stiffness;
            }
            break;
        case D_SECT_TYPE_COMPO_G:
            {
                rData = SectD.SectBefore.SectI.Stiffness;
            }
            break;
        default:
			ASSERT(0);
            break;
        }        
    }
    else
    {
        switch (SectD.SectBefore.nStype)
        {
        case D_SECT_TYPE_COMPO_I:
        case D_SECT_TYPE_COMPO_B:
        case D_SECT_TYPE_COMPO_TUB:
        case D_SECT_TYPE_COMPO_G:
        case D_SECT_TYPE_COMPO_STLG_I:
        case D_SECT_TYPE_COMPO_STLG_B:
		case D_SECT_TYPE_COMPO_STLG_TUB:
			{
                rData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;
            }
            break;        
        default:
			ASSERT(0);
            break;
        } 
    }

    return TRUE;
}

BOOL CDgnPscCommon::Get_GirderStiffnessofCompo(BOOL bPosiI, T_SECT_D& SectD, T_SECT_DESIGN& rData) const
{
	BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;

	int nSectType=0;
	if ( !bTapered )
	{
        switch (SectD.nStype)
        {
        case D_SECT_TYPE_COMPO_I:
        case D_SECT_TYPE_COMPO_B:
        case D_SECT_TYPE_COMPO_TUB:
        case D_SECT_TYPE_COMPO_STLG_I:
        case D_SECT_TYPE_COMPO_STLG_B:
		case D_SECT_TYPE_COMPO_STLG_TUB:
			{
				rData = SectD.SectBefore.Design;
			}
            break;
        case D_SECT_TYPE_COMPO_G:
            {
                rData = SectD.SectBefore.SectI.Design;
            }
            break;        
        default:
            ASSERT(0);
            break;
        }
	}
	else
	{
        switch (SectD.SectBefore.nStype)
        {
        case D_SECT_TYPE_COMPO_I:
        case D_SECT_TYPE_COMPO_B:
        case D_SECT_TYPE_COMPO_TUB:
        case D_SECT_TYPE_COMPO_G:
        case D_SECT_TYPE_COMPO_STLG_I:
        case D_SECT_TYPE_COMPO_STLG_B:
		case D_SECT_TYPE_COMPO_STLG_TUB:
			{
                rData = bPosiI ? SectD.SectBefore.SectI.Design : SectD.SectBefore.SectJ.Design;
            }
            break;
        default:
            ASSERT(0);
            break;
        }
	}

	return TRUE;
}

void CDgnPscCommon::Get_SectionSizeOfSect_I(double* aSize, double& dH, double& dtw, double& dB1, double& dtf1, double& dB2, double& dtf2, double& dh)
{
    dH   = aSize[0]+aSize[3]+aSize[5];
    dtw  = aSize[1];
    dB1  = aSize[2];
    dtf1 = aSize[3];
    dB2  = aSize[4];
    dtf2 = aSize[5];	
    if(dB2==0.)	dB2  = dB1;
    if(dtf2==0.)	dtf2 = dtf1;
    dh = aSize[0]; // C, web의 높이.
}

double CDgnPscCommon::Get_InertiaOfSB(double dh, double db)
{
    double dIner = db*pow(dh,3)/12.0;
    return dIner;
}

BOOL CDgnPscCommon::Get_StiffnessOfEffeSect(CArray<T_STIFF_CALC_D, T_STIFF_CALC_D&>& aStiffCalcD, double& dCzm, double& dIy)
{
    int iStiffCalcSize = aStiffCalcD.GetSize();
    double dArea_t = 0.0;
    double dAz_t = 0.0;
    double dIy_t = 0.0;
    for(int i=0; i<iStiffCalcSize; ++i)
    {
        T_STIFF_CALC_D CalcD = aStiffCalcD[i];
        dArea_t += CalcD.dArea;
        dAz_t += CalcD.dArea*CalcD.dz;    
    }
    dCzm = dArea_t==0.0 ? 0.0 : dAz_t/dArea_t;

    for(int i=0; i<iStiffCalcSize; ++i)
    {
        T_STIFF_CALC_D CalcD = aStiffCalcD[i];
        double dGap_z = dCzm - CalcD.dz;
        dIy_t += CalcD.dIy + CalcD.dArea*pow(dGap_z,2);
    }
    dIy = dIy_t;

    return TRUE;
}

BOOL CDgnPscCommon::Get_StiffnessOfSB(double Bs, double Bc, double tc, int iNg, T_SECT_STIFFNESS& rData)
{
    double Ng=(double)iNg;
    double A_s  = Bc * tc;
    double Iy_s = Bc*pow(tc,3.)/12.;
    double Iz_s = pow(Bs,3.)*tc/(Ng*12.);
    double Ix_s = (Bs*pow(tc,3.)/6.)/Ng;

    rData.Area  = A_s;
    rData.WArea = A_s;  
    rData.Asy = A_s*5.0/6.0;
    rData.Asz = A_s*5.0/6.0;

    rData.Rxx   = Ix_s;
    rData.Ryy   = Iy_s;
    rData.Rzz   = Iz_s;
    rData.Cym		= Bc/2.;
    rData.Cyp		= Bc/2.;
    rData.Czm		= tc/2.;
    rData.Czp		= tc/2.;
    //Design.YBar		  = Ybar;
    //Design.ZBar		  = tc/2. + Hh + tf1 + Hw + tf2;
    //PeriOut         = 2.0*(Bc+tc);
    //PeriIn          = 0.0;

    rData.dx1		= -(Bc/2.);
    rData.dy1		= tc/2.;
    rData.dx2		= (Bc/2.);
    rData.dy2		= tc/2.;
    rData.dx3		= (Bc/2.);
    rData.dy3		= -tc/2.;
    rData.dx4		= -(Bc/2.);
    rData.dy4		= -tc/2.;

    return TRUE;
}

BOOL CDgnPscCommon::Get_ConvertRbarPoint4Org(double dCym, double dCzm, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi)
{
    int iRbarSize = arRbarPosi.GetSize();
    for(int i=0; i<iRbarSize; ++i)
    {
        _DGN_RBAR_CRC RbarD = arRbarPosi[i];
        RbarD.dy += dCym;
        RbarD.dz += dCzm;
        arRbarPosi.SetAt(i, RbarD);
    }

    return TRUE;
}

BOOL CDgnPscCommon::Get_ConvertRbarCrcToGsecRbar(_DGN_RBAR_CRC& RbarCrc, T_GSEC_RBAR& rData)
{
    rData.dAs = RbarCrc.dArea;
    rData.dDia = RbarCrc.dDia;
    rData.dyz[0] = RbarCrc.dy;
    rData.dyz[1] = RbarCrc.dz;
    return TRUE;
}

BOOL CDgnPscCommon::Get_ConvertPointFromCentroid(double dYbar, double dZbar, T_SECT_SECTBASE_D& SectBaseD, T_GLINE_DGN_D& rData)
{
    double dMaxPt[2]={0.};
    double dMinPt[2]={0.};
    CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&> aVertex;  
    aVertex.Copy(rData.ModifiedGsecPolyD.aVertex);

    CSectUtil::Get_MaxMinPoint4LineType(rData.ModifiedGsecPolyD, rData.aModifiedLine, dMaxPt, dMinPt);
    // 도심의 좌표를 찾음.
    double dCentY = dMinPt[0] + dYbar;
    double dCentZ = dMinPt[1] + dZbar;

    int iVertexSize = aVertex.GetSize();  
    int i=0, j=0, k=0;
    for(i=0; i<iVertexSize; ++i)
    {
        T_GSEC_VERTEX VertexD = aVertex[i];
        VertexD.dX -= dCentY;
        VertexD.dY -= dCentZ;
        aVertex.SetAt(i,VertexD);    
    }
    rData.ModifiedGsecPolyD.aVertex.Copy(aVertex);


    int iPartSize = rData.aPartLine.GetSize();
    for(i=0; i<iPartSize; ++i)
    {
        T_GLINE_PART_DGN GLinePartDgnD = rData.aPartLine[i];
        int iOutstandSize = GLinePartDgnD.aOutstand.GetSize();
        for(j=0; j<iOutstandSize; ++j)
        {
            T_GLINE_DST GLineDstD = GLinePartDgnD.aOutstand[j];
            GLineDstD.dy1 -= dCentY;
            GLineDstD.dz1 -= dCentZ;
            GLineDstD.dy2 -= dCentY;
            GLineDstD.dz2 -= dCentZ;
            int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
            for(k=0; k<iLineDgnSize; ++k)
            {
                T_GLINE_BASE GLineBase = GLineDstD.aLineDgn[k];
                GLineBase.dy1 -= dCentY;
                GLineBase.dz1 -= dCentZ;
                GLineBase.dy2 -= dCentY;
                GLineBase.dz2 -= dCentZ;
                GLineDstD.aLineDgn[k] = GLineBase;
            }
            GLinePartDgnD.aOutstand[j] = GLineDstD;
        }
        //
        int iInternalSize = GLinePartDgnD.aInternal.GetSize();
        for(j=0; j<iInternalSize; ++j)
        {
            T_GLINE_DST GLineDstD = GLinePartDgnD.aInternal[j];
            GLineDstD.dy1 -= dCentY;
            GLineDstD.dz1 -= dCentZ;
            GLineDstD.dy2 -= dCentY;
            GLineDstD.dz2 -= dCentZ;
            int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
            for(k=0; k<iLineDgnSize; ++k)
            {
                T_GLINE_BASE GLineBase = GLineDstD.aLineDgn[k];
                GLineBase.dy1 -= dCentY;
                GLineBase.dz1 -= dCentZ;
                GLineBase.dy2 -= dCentY;
                GLineBase.dz2 -= dCentZ;
                GLineDstD.aLineDgn[k] = GLineBase;
            }
            GLinePartDgnD.aInternal[j] = GLineDstD;
        }  
        rData.aPartLine[i] = GLinePartDgnD;
    }

    // Slab
    int iGenPartSize = SectBaseD.aGeneralPart.GetSize();
    for(i=0; i<iGenPartSize; ++i)
    {
        T_GSEC_PART GsecPartD = SectBaseD.aGeneralPart[i];
        if(GsecPartD.bPlane)
        {
            int iStartId = GsecPartD.IdxStart[0];
            int iPolySize = SectBaseD.aOuterPolygon[iStartId].aVertex.GetSize();
            rData.ModifiedSlabGsecPolyD.aVertex.SetSize(iPolySize);
            for(j=0; j<iPolySize; ++j)
            {
                T_GSEC_VERTEX VertexD = SectBaseD.aOuterPolygon[iStartId].aVertex.GetAt(j);
                VertexD.dX -= dCentY;
                VertexD.dY -= dCentZ;
                rData.ModifiedSlabGsecPolyD.aVertex.SetAt(j, VertexD);        
            }
            break;
        }
    }

    return TRUE;
}

int CDgnPscCommon::Get_SectShapeType(T_SECT_D& rData)
{
    int iSectType=0;
    BOOL bTapered = rData.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;    
    if(!bTapered) iSectType = rData.nStype;
    else          iSectType = rData.SectBefore.nStype;

    return iSectType;
}


BOOL CDgnPscCommon::Get_CompositeSlabSection(ElemPairK ElemK, double& dBc, double& dtc, double& dHh)
{

    // Get Elem.
    T_SECT_D SectD; SectD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetSectPostTsgrDgn(ElemK, SectD)) return FALSE;

    if(SectD.nStype==D_SECT_TYPE_COMPO_CI || SectD.nStype==D_SECT_TYPE_COMPO_CT || SectD.nStype==D_SECT_TYPE_COMPO_PC)
    {        
        dBc = SectD.SectAfter.SectJ.Size[0];
        dtc = SectD.SectAfter.SectJ.Size[1];
        dHh = SectD.SectAfter.SectJ.Size[2];
    }
    else if (SectD.nStype == D_SECT_TYPE_TAPERED)
    {
        if (SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_CI || SectD.nStype==D_SECT_TYPE_COMPO_CT ||
            SectD.nStype==D_SECT_TYPE_COMPO_PC)
        {
            dBc = SectD.SectAfter.SectJ.Size[0];
            dtc = SectD.SectAfter.SectJ.Size[1];
            dHh = SectD.SectAfter.SectJ.Size[2];
        }
    }
    else
    {
        dBc = 0.0;
        dtc = 0.0;
        dHh = 0.0;
    }
    return TRUE;
}

BOOL CDgnPscCommon::Get_PscTndnPropData(PSC_TNDN_PTIF &TnPropD)
{
    ConvertPscCalcTndnPropInfo(&m_TndnProp, TnPropD);

    return TRUE;
}  

BOOL CDgnPscCommon::Get_PscTdnaInfo(PSC_TDNA_INFO& rData)
{
    POSITION pos = m_amTndnTdna.GetStartPosition();
    while ( pos )
    {
        T_TDNA_K TKey=0;
        _PSC_TNDN_TDNA TData;
        m_amTndnTdna.GetNextAssoc(pos, TKey, TData);
        PSC_TNDN_TDNA TdnaD;
        ConvertPscCalcTdnaData(&TData, TdnaD);
        rData.mTdnaInfo.SetAt(TKey, TdnaD);
    }

    return TRUE;
}

BOOL CDgnPscCommon::Get_PscTdnafpsInfo(PSC_TDNA_INFO& rData)
{
    POSITION Pos = m_amTndnTran.GetStartPosition();
    while ( Pos )
    {
        T_TDNA_K TKey=0;
        _DGN_TNDN_TRAN TData;
        m_amTndnTran.GetNextAssoc(Pos, TKey, TData);

        PSC_TNDN_TDNA TdnaD;
        if ( rData.mTdnaInfo.Lookup(TKey, TdnaD) )
        {
            ConvertPscCalcTndnTranToTdna(&TData, TdnaD);
            rData.mTdnaInfo.SetAt(TKey, TdnaD);
        }

    }
    return TRUE;
}

void CDgnPscCommon::Get_rElemK(CArray<T_ELEM_K,T_ELEM_K>& rElemK)
{
	m_pDataCtrl->GetElemKeyList4ElemPair(EN_EL_BEAM, m_arElemK, rElemK);
}

double CDgnPscCommon::GetFySteelSectIRC( T_MATL_STEEL& SteelMaterial,double dThk)
{
    if (dThk<20.0) return SteelMaterial.S_Fy1;
    else if (dThk>40.0) return SteelMaterial.S_Fy3;
    else return SteelMaterial.S_Fy2;

}

void CDgnPscCommon::CalcLengthFromEnd(double dSpanLength, _PSC_SPBF_POSI& rData)
{
    double dHalfLength = 0.5 * dSpanLength;
    if ( dHalfLength < rData.dLengthFromEnd )
    {
        rData.bFromI = FALSE;
        rData.dLengthFromEnd = dSpanLength - rData.dLengthFromEnd;
    }
}

double CDgnPscCommon::Get_fctm_EC2(double dfck, double dfcm)
{
	if (dgn::LE(dfck, 50.0))
	{
		return 0.3 * pow(dfck, 2.0 / 3.0);
	}
	else
	{
		return 2.12 * log(1.0 + (dfcm / 10.0));
	}
}

BOOL CDgnPscCommon::IsCheckPosition(int nPos) const
{
    if ( nPos < 0 || nPos >1 ) return FALSE;
    return TRUE;
}

void CDgnPscCommon::SetCheckElem(UINT unElem)
{
    m_unChkElem = unElem;
}

UINT CDgnPscCommon::GetCheckElem() const
{
    return m_unChkElem;
}

void CDgnPscCommon::SetCheckPosition(UINT unPos)
{
    m_unChkPos = unPos;
}

UINT CDgnPscCommon::GetCheckPosition() const
{
    return m_unChkPos;
}

UINT CDgnPscCommon::GetCheckPositionIndex(BOOL bPosi) const
{
	return bPosi ? 0 : 1;
}

void CDgnPscCommon::SetCheckElemPairK(ElemPairK EPairK)
{
	m_EPairK = EPairK;
}

ElemPairK CDgnPscCommon::GetCheckElemPairK() const
{
	return m_EPairK;
}


void CDgnPscCommon::ConvertSodSectLongStfn()
{
}

void CDgnPscCommon::ConvertSodStiffenerData(int nStype, T_SECT_SECTBASE_D* pSectD, T_STPO_D& StpoD, T_SECT_SOD_STIFF_D* pStfn, ST_SECT_STIFF_D& rData)
{
	if(pSectD==NULL) return;
	if(pStfn==NULL) return;

	int nShapeSize = pStfn->aStiffShape.GetSize();
	if(nShapeSize>0) rData.aStiffShape.SetSize(nShapeSize);

	for(int i=0; i<nShapeSize; ++i)
	{
		ConvertSodStfnShapeData(&pStfn->aStiffShape[i], rData.aStiffShape[i]);
		m_mStfnNameIndex.SetAt(pStfn->aStiffShape[i].strName, i);
	}

	int nStiffSubSize = pStfn->aSODStiffSub.GetSize();
	for(int i=0; i<nStiffSubSize; ++i)
	{
		T_SECT_SOD_STIFF_SUB_D& PartStfnD = pStfn->aSODStiffSub[i];

		ConvertSodStfnProfileData(nStype, pSectD, StpoD, PartStfnD, rData);
	}

}

void CDgnPscCommon::ConvertSodStfnShapeData(T_STIFF_SHAPE_D* pShape, ST_STIFF_SHAPE_D& rData)
{
	if(pShape==NULL) return;

	rData.strName = pShape->strName;
	rData.nType   = pShape->nType;
	memcpy(rData.dSize, pShape->dSize, sizeof(rData.dSize));

    T_SECT_D CalcS; CalcS.Initialize();
	T_SECT_STIFFNESS& StfnStiff = CalcS.SectBefore.SectI.Stiffness;
    CalcS.nStype = D_SECT_TYPE_REGULAR;
    switch(rData.nType)
    {
        case 0:     CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_SB;   break;
        case 1:     CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_T;    break;
        case 2:     CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_URIB; break;
        default: ASSERT(0);
                    CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_SB;   break;
    }
	for(int j=0; j<8; ++j) CalcS.SectBefore.SectI.Size[j] = rData.dSize[j];

	CSectUtil::CalcSectData(CalcS);
	ConvertSectStiffnessData(StfnStiff, rData.Stiffness);
}

void CDgnPscCommon::ConvertSodStfnProfileData(int nStype, T_SECT_SECTBASE_D* pSectD, T_STPO_D& StpoD, T_SECT_SOD_STIFF_SUB_D& PartStfn, ST_SECT_STIFF_D& rData)
{
	if(PartStfn.nDeckPart<0 || PartStfn.nDeckPart>2) return;

	double dWidth = 0.0;
	if(nStype==D_SECT_TYPE_STLG_B ||
        nStype==D_SECT_TYPE_COMPO_STLG_B || nStype==D_SECT_TYPE_COMPO_STLG_TUB)
	{
		if(PartStfn.nDeckPos==0) // Top
		{
			if(PartStfn.nDeckPart==0) // Left Outstand
			{
				dWidth = pSectD->Size[0];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTopLeft);
                rData.nStiffNum[3] += rData.aStiffTopLeft.GetSize();
			}
			else if(PartStfn.nDeckPart==1) // Internal
			{
				dWidth = pSectD->Size[1];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTop);
                rData.nStiffNum[3] += rData.aStiffTop.GetSize();

			}
			else if(PartStfn.nDeckPart==2) // Right Outstand
			{
				dWidth = pSectD->Size[2];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTopRight);
                rData.nStiffNum[3] += rData.aStiffTopRight.GetSize();
			}
			else ASSERT(0);
		}
		else if(PartStfn.nDeckPos==1) // web.L
		{
			dWidth = pSectD->Size[6];
			ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffLeft);
            rData.nStiffNum[0] += rData.aStiffLeft.GetSize();
		}
		else if(PartStfn.nDeckPos==2) // web.R
		{
			dWidth = pSectD->Size[6];
			ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffRight);
            rData.nStiffNum[1] += rData.aStiffRight.GetSize();
		}
		else if(PartStfn.nDeckPos==3) // Bottom
		{
			if(PartStfn.nDeckPart==0) // Left Outstand
			{
				dWidth = pSectD->Size[3];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottomLeft);
                rData.nStiffNum[2] += rData.aStiffBottomLeft.GetSize();
			}
			else if(PartStfn.nDeckPart==1) // Internal
			{
				dWidth = pSectD->Size[4];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottom);
                rData.nStiffNum[2] += rData.aStiffBottom.GetSize();

			}
			else if(PartStfn.nDeckPart==2) // Right Outstand
			{
				dWidth = pSectD->Size[5];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottomRigth);
                rData.nStiffNum[2] += rData.aStiffBottomRigth.GetSize();
			}
			else ASSERT(0);
		}
		else ASSERT(0);
	}
	else if(nStype==D_SECT_TYPE_STLG_I ||
        nStype==D_SECT_TYPE_COMPO_STLG_I)
	{
		if(PartStfn.nDeckPos==0) // Top
		{
			if(PartStfn.nDeckPart==0) // Left Outstand
			{
				dWidth = pSectD->Size[0];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTopLeft);
                rData.nStiffNum[3] += rData.aStiffTopLeft.GetSize();
			}
			else if(PartStfn.nDeckPart==1) // Right Outstand
			{
				dWidth = pSectD->Size[1];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTopRight);
                rData.nStiffNum[3] += rData.aStiffTopRight.GetSize();
			}
			else ASSERT(0);
		}
		else if(PartStfn.nDeckPos==1) // web
		{
			dWidth = pSectD->Size[4];
			ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffLeft);
            rData.nStiffNum[0] += rData.aStiffLeft.GetSize();
		}
		else if(PartStfn.nDeckPos==2) // Bottom
		{
			if(PartStfn.nDeckPart==0) // Left Outstand
			{
				dWidth = pSectD->Size[2];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottomLeft);
                rData.nStiffNum[2] += rData.aStiffBottomLeft.GetSize();
			}
			else if(PartStfn.nDeckPart==1) // Right Outstand
			{
				dWidth = pSectD->Size[3];
				ConvertSodStfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottomRigth);
                rData.nStiffNum[2] += rData.aStiffBottomRigth.GetSize();
			}
			else ASSERT(0);
		}
		else ASSERT(0);
	}
	else ASSERT(0);
}

void CDgnPscCommon::ConvertSodStfnProfileData(int nStype, T_STPO_D& StpoD, double dWidth, T_SECT_SOD_STIFF_SUB_D& PartStfn, CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&>& arData)
{
	int nProfileSize = PartStfn.aStiffData.GetSize();
	if(nProfileSize<=0) return;

	BOOL bReCalSpace = IsReplaceStfnIndex(nStype, PartStfn.nRefPos, PartStfn.nDeckPos, PartStfn.nDeckPart);

	double dDistLastStfn=0.0;
	if(bReCalSpace)
	{
		for(int i=0; i<nProfileSize; ++i)
		{
			dDistLastStfn += PartStfn.aStiffData[i].dSpacing;
		}
	}

	CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&> aStfnProD;

	aStfnProD.SetSize(nProfileSize);
	for(int i=0; i<nProfileSize; ++i)
	{
		int nId = (bReCalSpace==TRUE) ? nProfileSize-1-i : i;
		T_SECT_SOD_STIFF_DATA_D OneStfnD = PartStfn.aStiffData[nId];
		if(bReCalSpace)
		{
			if(i==0)
			{
				OneStfnD.dSpacing = dWidth - dDistLastStfn;
			}
			else
			{
				OneStfnD.dSpacing = PartStfn.aStiffData[nProfileSize-i].dSpacing;
			}
		}

		ConvertSodStfnProfileData(StpoD, PartStfn.nRefPos, PartStfn.nDeckPos, PartStfn.nDeckPart, dWidth, &OneStfnD, aStfnProD[i]); // OneStfnD.nStiffPos

	}
	arData.Append(aStfnProD);

}

void CDgnPscCommon::ConvertSodStfnProfileData(T_STPO_D& StpoD, int nRefPos, int nDeckPos, int nDeckPart, double dWidth, T_SECT_SOD_STIFF_DATA_D* pStfnProf, ST_STIFF_PROFILE_D& rData)
{
	if(pStfnProf==NULL) return;
	rData.dSpacing = pStfnProf->dSpacing;
	int nShape=0;
	if(m_mStfnNameIndex.Lookup(pStfnProf->strShapeName, nShape))
	{
		rData.nShape = nShape;
	}
	rData.bCalc     = pStfnProf->bCalc;
	rData.strName   = pStfnProf->strStiffName;
	rData.nPosition = pStfnProf->nStiffPos;

	CArray<T_POINT_SSM, T_POINT_SSM&> aStressPoint;
	CSectUtil::GetStressPoint4Stiiffener_SUB(StpoD, nDeckPos, nDeckPart, pStfnProf->strStiffName, aStressPoint);

	int nDSize = sizeof(rData.Point)/sizeof(DGN_GSEC_VERTEX);
	int nPtSize = aStressPoint.GetSize();

	if(nDSize<nPtSize)
	{
		ASSERT(0);
		return;
	}

	for(int i=0; i<nPtSize; ++i)
	{
		T_POINT_SSM& PtS = aStressPoint[i];
		rData.Point[i].dX = PtS.dPointY;
		rData.Point[i].dY = PtS.dPointZ;
	}
	//DGN_GSEC_VERTEX Point[20];   // flat : 4개, T    : 8개,      // U    : 12개
}

BOOL CDgnPscCommon::IsReplaceStfnIndex(int nStype, int nRefPos, int nDeckPos, int nDeckPart)
{
    BOOL bIsReplace=FALSE;
    if(nStype == D_SECT_TYPE_STLG_I || nStype == D_SECT_TYPE_COMPO_STLG_I)
    {
        // Deck Plate
        if((nDeckPos==0 || nDeckPos==2) && nDeckPart==0)
        {
            bIsReplace = (nRefPos==0);
        }
        else if((nDeckPos==0 || nDeckPos==2) && nDeckPart==1)
        {
            bIsReplace = (nRefPos==1);
        }

        // Web
        if(nDeckPos == 1)
        {
            bIsReplace = (nRefPos==1);
        }
    }
    else if(nStype == D_SECT_TYPE_STLG_B ||
        nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		if((nDeckPos==0 || nDeckPos==3) && nDeckPart==0)
		{
			bIsReplace = (nRefPos==0);
		}
		else if((nDeckPos==0 || nDeckPos==3) && (nDeckPart==1 || nDeckPart==2))
		{
			bIsReplace = (nRefPos==1);
		}
		// Web
		if((nDeckPos == 1 || nDeckPos == 2))
		{
			bIsReplace = (nRefPos==1);
		}
	}
	else{ ASSERT(0); }


	return bIsReplace;
}