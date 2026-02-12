// ReportTreeText.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ReportTreeText.h"

// #include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_LateralLoad.h"

#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_xl\ReportTableProcess.h"
#include "PostDrawAdapter.h"
#include "ReportAutoRegeneration.h"

#include "..\wg_dgn\DgnAmrSectionTb.h"
#include "..\wg_dgn\DgnDrawSection.h"

#include "..\wg_gr\GRAMRUDFData.h"
#include "..\wg_gps\GPSAMRUtil.h"  // for Dynamic Report Figure
#include "..\wg_gps\GPSAMRUDFDlg.h"
#include "..\wg_gps\GPSAMRUDCData.h"  // for Dynamic Report Chart(Graph)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportTreeText


CReportTreeText::CReportTreeText()
{
	m_pDoc = CDBDoc::GetDocPoint();
	if(!m_pDoc) 
	{
		ASSERT(0);
		return;
	}
	
	m_nStldNum=0;
	m_aStldDataNum.RemoveAll();
	
	m_bIsSetStld = FALSE;
	
	m_arElemKeyList.RemoveAll();
	m_arColumnElemKeyList.RemoveAll();
	m_arTdnaK.RemoveAll();
	m_vctDetailTable.clear();
	GetElementsForDesign();
}

CReportTreeText::~CReportTreeText()
{
	if(m_pDoc)
		m_pDoc = NULL;
	
	if(m_nStldNum != 0)
	{
		for(int i=0; i<D_STLD_DATA_NUM; i++)
		{
			if(m_aKeyList[i])
			{
				delete[] m_aKeyList[i];
				m_aKeyList[i] = NULL;
			}
		}
	}
}

BOOL CReportTreeText::WriteReportText(const CString strTextItem, CString& strWriteText)
{
	BOOL bRet=FALSE;
	
	//CString strTextItem = ReplaceTempCharBySpace(rsTextItem);
	
	if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Control_Data))==0)
	{
		bRet = WriteCtrl(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Static_Loadcase))==0)
	{
		bRet = WriteStld(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Story))==0)
	{
		bRet = WriteStor(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Node))==0)
	{
		bRet = WriteNode(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Boundary))==0)
	{
		bRet = WriteBndr(strWriteText);
		//AfxMessageBox(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Floor_Diaphragm_Rigid_Link))==0)
	{
		bRet = WriteRigd(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Beam))==0)
	{
		bRet = WriteBeam(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Truss))==0)
	{
		bRet = WriteTrus(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Wall))==0)
	{
		bRet = WriteWall(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Weight_Volume_Surface_area))==0)
	{
		bRet = WriteWVSA(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_Wind_Load                       ))==0 ||
		_tcsicmp(strTextItem, _LS(IDS_AMR_Seismic_Load                    ))==0 ||
		_tcsicmp(strTextItem, _LS(IDS_AMR_Load_Combination                ))==0 || 
		_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Self_Weight              ))==0 ||
		_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Nodal_Load               ))==0 ||
		_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Finishing_Material_Load  ))==0 ||
		_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_System_Temperature       ))==0 ||
		_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Nodal_Temperature        ))==0 ||
		_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Element_Temperature      ))==0 ||
		_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Beam_Section_Temperature ))==0 ||
		_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Response_Spectrum_Load))==0)
	{
		bRet = WriteStldData(strTextItem, strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Pushover_Global_Control))==0)
	{
		bRet = WritePOGD(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Pushover_Load_Case))==0)
	{
		bRet = WritePOLC(strWriteText);
	}
	//china report
	/** Add By JUHONGHUA(MITC) 2012-2-14 15:47:27**/
	/*
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_General))==0)
	{
	bRet = WriteBasicInfo_General(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Standard))==0)
	{
	bRet = WriteBasicInfo_Standard(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Code))==0)
	{
	bRet = WriteBasicInfo_Code(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Structure))==0)
	{
	bRet = WriteBasicInfo_Structure(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Material))==0)
	{
	bRet = WriteBasicInfo_Material(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Calculation))==0)
	{
	bRet = WriteBasicInfo_Calculation(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Model_Calculation))==0)
	{
	bRet = WriteModel_Calculation(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Model_Steel))==0)
	{
	bRet = WriteModel_Steel(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Model_Section))==0)
	{
	bRet = WriteModel_Section(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Model_Load))==0)
	{
	bRet = WriteModel_Load(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_ComDepth))==0)
	{
	bRet = WriteLoadCapacity_ComDepth(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_Bending))==0)
	{
	bRet = WriteLoadCapacity_Bending(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_Shearing))==0)
	{
	bRet = WriteLoadCapacity_Shearing(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_Torsion))==0)
	{
	bRet = WriteLoadCapacity_Torsion(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_Reforce))==0)
	{
	bRet = WriteLoadCapacity_Reforce(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_ServiceAbility_Normal))==0)
	{
	bRet = WriteServiceAbility_Normal(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_ServiceAbility_Oblique))==0)
	{
	bRet = WriteServiceAbility_Oblique(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Stress_NormalComp))==0)
	{
	bRet = WriteStress_NormalComp(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Stress_Tension))==0)
	{
	bRet = WriteStress_Tension(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Stress_PrincipalComp))==0)
	{
	bRet = WriteStress_PrincipalComp(strWriteText);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Stress_ShortTerm))==0)
	{
	bRet = WriteStress_ShortTerm(strWriteText);
	}*/
	else
	{
		ASSERT(0); bRet = FALSE;
	}
	
	
	
	
	
	
	return bRet;
}

// Control Data
BOOL CReportTreeText::WriteCtrl(CString& strText)
{
	// panel zone effect
	CString csPanelZone;  
	T_PZEF_D data;
	if (!m_pDoc->m_pAttrCtrl->GetPzef(data)) data.Initialize();
	if (!data.bOffset) csPanelZone = _LS(IDS_MAIN_MDL_CTRL_DO_NOT_CALC);
	else
	{
		CString aOffsType[2] = {_LS(IDS_MAIN_MDL_CTRL_PANEL_ZONE) , _LS(IDS_MAIN_MDL_CTRL_OFFSET_POS) };
		csPanelZone.Format(_LS(IDS_MAIN_MDL_CTRL_INFO), data.dOffsFact, aOffsType[data.nOffsType-1]);
	}
	
	// unit system
	T_UNIT_INDEX UnitIndex;
	CString aForce[] = { _T("KGF"), _T("TONF"), _T("N"), _T("KN"), _T("LBF"), _T("KIPS") };
	CString aLength[] = { _T("MM"), _T("CM"), _T("M"), _T("IN"), _T("FT") };
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	// definition of frame
	CString aFrame[] = {_T("Unbraced I Sway"), _T("Braced I Non-sway")};
	CString aDgnType[] = {_T("3-D"), _T("X-Z Plane"), _T("Y-Z Plane"), _T("X-Y Plane")};
	T_DCTL_D DctlD;
	if (!m_pDoc->m_pAttrCtrl->GetDctl(DctlD)) DctlD.Initialize();
	int nFrameX = DctlD.bBraceFrame_X ? 1 : 0; 
	int nFrameY = DctlD.bBraceFrame_Y ? 1 : 0; 
	
	// design code 
	CString csConCode = CDBLib::GetConCodeName();
	CString csStlCode = CDBLib::GetStlCodeName();
	CString csSRCCode = CDBLib::GetSrcCodeName();
	
	// write
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Control_Data));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_DATA_TITLE)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_PANEL_ZONE_INFO)), csPanelZone);
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_UNIT_SYS_INFO)), aForce[UnitIndex.nBase_Force], aLength[UnitIndex.nBase_Length]);
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_DEF_FRAME)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_DEF_FRAME_X)), aFrame[nFrameX]);
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_DEF_FRAME_Y)), aFrame[nFrameY]);
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_DEF_FRAME_TYPE)), aDgnType[DctlD.nDesignType]);
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_DESIGN_CODE)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_DESIGN_CODE_STL)), csStlCode);
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_DESIGN_CODE_CON)), csConCode);
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CTRL_DESIGN_CODE_SRC)), csSRCCode);
	strText += strTemp;
	
	return TRUE;
}

// Beam
BOOL CReportTreeText::WriteBeam(CString& strText)
{
	// beam
	CArray<T_ELEM_K, T_ELEM_K> aElemK, aBeamK;
	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	for (int i=0; i<aElemK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
		if (m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp))
			aBeamK.Add(aElemK[i]);
	}
	int nSize = aBeamK.GetSize();
	if (nSize <= 0) return FALSE;
	
	// 8  9  8  8  8  15  15  10
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Beam));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Beam));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BEAM_MEMBER_TITLE)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BEAM_MEMBER_HEAD1)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BEAM_MEMBER_HEAD2)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BEAM_MEMBER_HEAD3)));
	strText += strTemp;
	
	T_MATL_D MatlD;
	T_SECT_D SectD;
	CArray<T_FRLS_K, T_FRLS_K> aFrlsK;
	CArray<T_FRLS_D, T_FRLS_D&> aFrlsD;
	double dLength;
	CString csMatl, csSect, csFrlsI, csFrlsJ;
	
	for(int i=0; i<nSize; i++)
	{
		csFrlsI = _T("-");
		csFrlsJ = _T("-");
		csMatl  = _T("-");
		csSect  = _T("-");
		
		m_pDoc->m_pAttrCtrl->GetElem(aBeamK[i], ElemD);
		if (m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) csMatl = MatlD.Name;
		if (m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD)) csSect = SectD.SName;
		
		m_pDoc->m_pAttrCtrl->GetFrlsElem(aBeamK[i], aFrlsK, aFrlsD);
		if (aFrlsK.GetSize() > 0)
		{
			csFrlsI.Format(_T("  %c%c%c%c%c%c"), 
				aFrlsD[0].Release[0][0], aFrlsD[0].Release[0][1], aFrlsD[0].Release[0][2],
				aFrlsD[0].Release[0][3], aFrlsD[0].Release[0][4], aFrlsD[0].Release[0][5]);
			csFrlsJ.Format(_T("  %c%c%c%c%c%c"), 
				aFrlsD[0].Release[1][0], aFrlsD[0].Release[1][1], aFrlsD[0].Release[1][2],
				aFrlsD[0].Release[1][3], aFrlsD[0].Release[1][4], aFrlsD[0].Release[1][5]);
		}
		
		dLength = m_pDoc->calcLAVElem(ElemD);
		
		strTemp.Format(_T("   %8d %9d %8d %8s %8s %15s %15s %10.4g \n"),
			aBeamK[i], ElemD.elnod[0], ElemD.elnod[1], csFrlsI, csFrlsJ, _ns(csMatl,15), _ns(csSect,15), dLength);
		
		strText += strTemp;
	}
	
	return TRUE;
}

// Boundary
BOOL CReportTreeText::WriteBndr(CString& strText)
{
	// support
	CArray<T_CONS_K, T_CONS_K> aConsK;
	m_pDoc->m_pAttrCtrl->GetConsKeyList(aConsK);
	int nCons = aConsK.GetSize();
	
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Boundary));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Boundary));
	strText += strTemp;
	
	if (nCons > 0)
	{
		// specified displacement
		CArray<T_SDSP_K, T_SDSP_K> aSdspK;
		CArray<T_SDSP_D, T_SDSP_D&> aSdspD;
		
		// 8  8   10   10   10   10   10   10
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BNSP_DATA_TITLE)));
		strText += strTemp;
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BNSP_DATA_HEAD1)));
		strText += strTemp;
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BNSP_DATA_HEAD2)));
		strText += strTemp;
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BNSP_DATA_HEAD3)));
		strText += strTemp;
		
		int k;
		CString DDDRRR;
		double aDisp[6];
		T_CONS_D ConsD;
		
		for (int i=0; i<nCons; i++)
		{
			m_pDoc->m_pAttrCtrl->GetCons(aConsK[i], ConsD);
			DDDRRR.Format(_T("%c%c%c%c%c%c"), ConsD.Constraint[0], ConsD.Constraint[1], ConsD.Constraint[2],
				ConsD.Constraint[3], ConsD.Constraint[4], ConsD.Constraint[5]);
			
			m_pDoc->m_pAttrCtrl->GetSdsp(aConsK[i].key.entity, aSdspK, aSdspD);
			for (k=0; k<6; k++) aDisp[k] = 0.0;
			for (k=0; k<aSdspK.GetSize(); k++)
			{
				if (aSdspD[k].Flag[0]) aDisp[0] += aSdspD[k].Displacement[0];
				if (aSdspD[k].Flag[1]) aDisp[1] += aSdspD[k].Displacement[1];
				if (aSdspD[k].Flag[2]) aDisp[2] += aSdspD[k].Displacement[2];
				if (aSdspD[k].Flag[3]) aDisp[3] += aSdspD[k].Displacement[3];
				if (aSdspD[k].Flag[4]) aDisp[4] += aSdspD[k].Displacement[4];
				if (aSdspD[k].Flag[5]) aDisp[5] += aSdspD[k].Displacement[5];
			}
			
			strTemp.Format(_T("   %8d %8s %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f \n"), 
				aConsK[i].key.entity, DDDRRR, aDisp[0], aDisp[1], aDisp[2], aDisp[3], aDisp[4], aDisp[5]); 
			
			strText += strTemp;
		}
		strTemp = _T(" ");
		strText += strTemp;
	}
	
	// point spring support
	CArray<T_NSPR_K, T_NSPR_K> aNsprK;
	m_pDoc->m_pAttrCtrl->GetNsprKeyList(aNsprK);
	int nNspr = aNsprK.GetSize();
	
	if (nNspr > 0)
	{
		// 8  10   10   10   10   10   10
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NSPR_DATA_TITLE)));
		strText += strTemp;
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NSPR_DATA_HEAD1)));
		strText += strTemp;
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NSPR_DATA_HEAD2)));
		strText += strTemp;
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NSPR_DATA_HEAD3)));
		strText += strTemp;
		
		T_NSPR_D NsprD;
		for (int i=0; i<nNspr; i++)
		{
			m_pDoc->m_pAttrCtrl->GetNspr(aNsprK[i], NsprD);
			strTemp.Format(_T("   %8d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f \n"),
				aNsprK[i].key.entity, NsprD.Spring[0], NsprD.Spring[1], NsprD.Spring[2],
				NsprD.Spring[3], NsprD.Spring[4], NsprD.Spring[5]);
			
			strText += strTemp;
		}
		strTemp = _T(" ");
		strText += strTemp;
	}
	
	if(nCons + nNspr <= 0)
		return FALSE;
	
	//AfxMessageBox(strText);
	
	return TRUE;
}

// Node
BOOL CReportTreeText::WriteNode(CString& strText)
{
	// node
	CArray<T_NODE_K, T_NODE_K> aNodeK;
	T_NODE_D NodeD;
	m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeK);
	int nSize = aNodeK.GetSize();
	if (nSize <= 0) return FALSE;
	
	// nodal temperature
	CArray<T_NTMP_K, T_NTMP_K> aNtmpK;
	CArray<T_NTMP_D, T_NTMP_D&> aNtmpD;
	
	// 8  12  12   12   12
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Node));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Node));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NODE_DATA_TITLE)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NODE_DATA_HEAD1)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NODE_DATA_HEAD2)));
	strText += strTemp;
	
	double dTemperature;
	for (int i=0; i<nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetNode(aNodeK[i], NodeD);
		
		dTemperature = 0.0;
		m_pDoc->m_pAttrCtrl->GetNtmp(aNodeK[i], aNtmpK, aNtmpD);
		for (int k=0; k<aNtmpK.GetSize(); k++)
			dTemperature += aNtmpD[k].dblNodalTemp;
		
		strTemp.Format(_T("   %8d %12.4g %12.4g %12.4g  %12.4g \n"), 
			aNodeK[i], NodeD.x, NodeD.y, NodeD.z, dTemperature); 
		strText += strTemp;
	}
	
	return TRUE;
}

// Pushover Global Control
BOOL CReportTreeText::WritePOGD(CString& strText)
{
	// pushover Global Control
	T_POGD_D PogdD;
	PogdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetPogd(PogdD)) 
	{
		//ASSERT(0);
		return FALSE;
	}
	
	strText = _T("");
	CString strTemp=_T("");
	int i=0;
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Pushover_Global_Control));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Pushover_Global_Control));
	strText += strTemp;
	
	strTemp = _LS(IDS_AMR_Initial_Load__);
	strText += strTemp;
	
	strTemp = _T("");
	int nInitLoad = PogdD.aInitLoad.GetSize();
	for(int i=0;i<nInitLoad;++i)
	{
		T_STLD_K StldK=0;
		T_STLD_D StldD; StldD.Initialize();
		
		StldK = PogdD.aInitLoad[i].LoadCaseKey;
		if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD))
		{
			ASSERT(0);
		}
		
		CString str = _T("");
		if(i==0)
			str.Format(_LS(IDS_AMR_FORMAT_g_x_s_), PogdD.aInitLoad[i].dScaleFactor, StldD.LoadCaseName);	
		else
			str.Format(_LS(IDS_AMR_FORMAT_plus_g_x_s), PogdD.aInitLoad[i].dScaleFactor, StldD.LoadCaseName);	
		
		strTemp+=str;
	}
	if(nInitLoad>0)
	{
		strText += strTemp; strText += _T("\n");
	}
	
	strTemp = _LS(IDS_AMR_Default_Stiffness_Reduction_Ratio_of_Skeleton_Curve);
	strText += strTemp; strText += _T("\n");
	strTemp.Format(_LS(IDS_AMR_FORMAT_Trilinear_alpha1__g__g), PogdD.dTri_Tens_a1, PogdD.dTri_Comp_a1);
	strText += strTemp; strText += _T("\n");
	strTemp.Format(_LS(IDS_AMR_FORMAT_Trilinear_alpha2__g__g), PogdD.dTri_Tens_a2, PogdD.dTri_Comp_a2);
	strText += strTemp; strText += _T("\n");
	strTemp.Format(_LS(IDS_AMR_FORMAT_Bilinear_alpha1__g__g), PogdD.dBi_Tens_a1, PogdD.dBi_Comp_a1);
	strText += strTemp; strText += _T("\n");
	
	strTemp = _LS(IDS_AMR_Nonlinear_Analysis_Option);
	strText += strTemp; strText += _T("\n");
	strTemp.Format(_LS(IDS_AMR_FORMAT_Permit_Convergence_Failure__s), PogdD.bPermitFail ? _LS(IDS_AMR_Consider) : _LS(IDS_AMR_Not_Consider));
	strText += strTemp; strText += _T("\n");
	strTemp.Format(_LS(IDS_AMR_FORMAT_Maximum_Number_of_Substeps__d), PogdD.nSubstep);
	strText += strTemp; strText += _T("\n");
	strTemp.Format(_LS(IDS_AMR_FORMAT_Maximum_Iteration__d), PogdD.nMaxIterNum);
	strText += strTemp; strText += _T("\n");
	
	strTemp = _LS(IDS_AMR_Convergence_Criteria);
	strText += strTemp; strText += _T("\n");
	if(PogdD.bDisplNorm)
		strTemp.Format(_LS(IDS_AMR_Displacement_Norm__s__g), _LS(IDS_AMR_Consider) , PogdD.dDisplNorm);
	else
		strTemp.Format(_LS(IDS_AMR_Displacement_Norm__s), _LS(IDS_AMR_Not_Consider));
	strText += strTemp; strText += _T("\n");
	
	if(PogdD.bForceNorm)
		strTemp.Format(_LS(IDS_AMR_Force_Norm__s__g),  _LS(IDS_AMR_Consider), PogdD.dForceNorm);
	else
		strTemp.Format(_LS(IDS_AMR_Force_Norm__s), _LS(IDS_AMR_Not_Consider));	
	strText += strTemp; strText += _T("\n");
	
	if(PogdD.bEnergyNorm)
		strTemp.Format(_LS(IDS_AMR_Energy_Norm__s__g), _LS(IDS_AMR_Consider) , PogdD.dEnergyNorm);
	else
		strTemp.Format(_LS(IDS_AMR_Energy_Norm__s), _LS(IDS_AMR_Not_Consider));
	strText += strTemp; strText += _T("\n");
	
	CString strLoction=_T("");
	if(PogdD.nLocOfBeam==0)
		strLoction = _LS(IDS_AMR_I);
	else if(PogdD.nLocOfBeam==1)
		strLoction = _LS(IDS_AMR_J);
	else if(PogdD.nLocOfBeam==2)
		strLoction = _LS(IDS_AMR_Center);
	else
		ASSERT(0);
	
	strTemp.Format(_LS(IDS_AMR_FORMAT_Reference_Location_only_for_Distributed_Hinges__s), strLoction);
	strText += strTemp; strText += _T("\n");
	
	strTemp.Format(_LS(IDS_AMR_FORMAT_Calculate_Yield_Surface_of_Beam_considering_Buckling__s), PogdD.bBuckling ? _LS(IDS_AMR_Consider) : _LS(IDS_AMR_Not_Consider));
	strText += strTemp; strText += _T("\n");
	strTemp.Format(_LS(IDS_AMR_FORMAT_Calculate_Mc_Considering_Axial_Force__s), PogdD.bConsiderAxialForce ? _LS(IDS_AMR_Consider) : _LS(IDS_AMR_Not_Consider));
	strText += strTemp; strText += _T("\n");
	
	return TRUE;
}

// Pushover Load Case
BOOL CReportTreeText::WritePOLC(CString& strText)
{
	// pushover Global Control
	CArray<T_POLC_K, T_POLC_K> aPolcK;
	aPolcK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
	
	int nPolc = aPolcK.GetSize();
	if(nPolc<1)
	{
		//ASSERT(0);
		return FALSE;
	}
	
	T_POLC_K PolcK=0;
	T_POLC_D PolcD; 
	
	strText = _T("");
	CString strTemp=_T("");
	int i=0;
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Pushover_Load_Case));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Pushover_Load_Case));
	strText += strTemp;
	
	for(int i=0;i<nPolc;++i)
	{
		PolcD.Initialize();
		PolcK = aPolcK[i];
		if(!m_pDoc->m_pAttrCtrl->GetPolc(PolcK, PolcD))
		{
			ASSERT(0); return FALSE;
		}
		
		CString strLCName=_T("");
		strLCName.Format(_LS(IDS_AMR_FORMAT_Load_Case_Name__s), PolcD.strName);
		if (PolcD.strDescription!=_T(""))
		{
			strTemp.Format(_LS(IDS_AMR_FORMAT_s_s_), strLCName, PolcD.strDescription);
			strText += strTemp; strText += _T("\n");
		}
		else
		{
			strTemp = strLCName;
			strText += strTemp; strText += _T("\n");
		}
		
		strTemp.Format(_LS(IDS_AMR_FORMAT_Increment_Step__d), PolcD.nStepNum);
		strText += strTemp; strText += _T("\n");
		strTemp.Format(_LS(IDS_AMR_FORMAT_Initial_Load__s), PolcD.bUseInitial ? _LS(IDS_AMR_Consider) : _LS(IDS_AMR_Not_Consider));
		strText += strTemp; strText += _T("\n");
		strTemp.Format(_LS(IDS_AMR_FORMAT_P_Delta_Effect__s), PolcD.bConsiderPDelta ? _LS(IDS_AMR_Consider) : _LS(IDS_AMR_Not_Consider));
		strText += strTemp; strText += _T("\n");
		
		strTemp.Format(_LS(IDS_AMR_FORMAT_Increment_Method__s), (PolcD.nIncrementMethod==0) ? _LS(IDS_AMR_Load_Control) : _LS(IDS_AMR_Displacement_Control));
		strText += strTemp; strText += _T("\n");
		if(PolcD.nIncrementMethod==0)  // Load Control
		{
			CString strStepCtrlOption=_T("");
			if(PolcD.nStepCtrlOption==0)
				strStepCtrlOption = _LS(IDS_AMR_Auto_Stepping_Control);
			else if(PolcD.nStepCtrlOption==1)
				strStepCtrlOption = _LS(IDS_AMR_Equal_Step__);
			else if(PolcD.nStepCtrlOption==2)
				strStepCtrlOption = _LS(IDS_AMR_Incremental_Control_Function);
			else
			{
				ASSERT(0); return FALSE;
			}
			
			strTemp.Format(_LS(IDS_AMR_FORMAT_Set_Control_Option_s), strStepCtrlOption);
			strText += strTemp; strText += _T("\n");
			strTemp = _LS(IDS_AMR_Analysis_Stopping_Condition);
			strText += strTemp; strText += _T("\n");
			strTemp.Format(_LS(IDS_AMR_FORMAT_Current_Stiffness_Ratio_g_s), PolcD.dStiffPercent, _T("%"));
			strText += strTemp; strText += _T("\n");
			
			if(PolcD.bLimitDeformAngle)
				strTemp.Format(_LS(IDS_AMR_FORMAT_Limit_Inter_Story_Deformation_Angle__s__g_rad), _LS(IDS_AMR_Consider) , PolcD.dIntStorDeformValue);
			else
				strTemp.Format(_LS(IDS_AMR_FORMAT_Limit_Inter_Story_Deformation_Angle__s), _LS(IDS_AMR_Not_Consider));
			strText += strTemp; strText += _T("\n");
		}
		else if(PolcD.nIncrementMethod==1)  // Displacement Control
		{
			CString strDispCtrlOption=_T("");
			if(PolcD.nDispCtrlOption==0)
				strDispCtrlOption = _LS(IDS_AMR_Global_Control);
			else if(PolcD.nDispCtrlOption==1)
				strDispCtrlOption = _LS(IDS_AMR_Master_Node_Control);
			else
			{
				ASSERT(0); return FALSE;
			}
			
			strTemp.Format(_LS(IDS_AMR_FORMAT_Set_Control_Option_s), strDispCtrlOption);
			strText += strTemp; strText += _T("\n");
			
			if(PolcD.nDispCtrlOption==0) //Global Control
			{
				strTemp.Format(_LS(IDS_AMR_FORMAT_Maximum_Translational_Displacement__g), PolcD.dOverallMaxDisp);
				strText += strTemp; strText += _T("\n");
			}
			else if(PolcD.nDispCtrlOption==1) // Master Node Control
			{
				CString strDirection=_T("");
				if(PolcD.MasterDirection==0)
					strDirection = _LS(IDS_AMR_DX);
				else if(PolcD.MasterDirection==1)
					strDirection = _LS(IDS_AMR_DY);
				else if(PolcD.MasterDirection==2)
					strDirection = _LS(IDS_AMR_DZ);
				else
				{
					ASSERT(0); return FALSE;
				}
				strTemp.Format(_LS(IDS_AMR_FORMAT_Master_Node__d_Direction__s_Maximum_Displacemet_g), PolcD.MasterNode, strDirection, PolcD.dMasterMaxDisp);
				strText += strTemp; strText += _T("\n");
			}
			else
			{
				ASSERT(0); return FALSE;
			}
			
			strTemp = _LS(IDS_AMR_Analysis_Stopping_Condition);		
			strText += strTemp; strText += _T("\n");
			
			if(PolcD.bLimitDeformAngle)
				strTemp.Format(_LS(IDS_AMR_FORMAT_Limit_Inter_Story_Deformation_Angle__s__g_rad), _LS(IDS_AMR_Consider) , PolcD.dIntStorDeformValue);
			else
				strTemp.Format(_LS(IDS_AMR_FORMAT_Limit_Inter_Story_Deformation_Angle__s), _LS(IDS_AMR_Not_Consider));
			strText += strTemp; strText += _T("\n");
		}
		else
		{	
			ASSERT(0); 
			return FALSE; 
		}
		
		//Load Pattern
		CString strLoadPattern=_T("");
		if(PolcD.nLoadPattern==0)
			strLoadPattern = _LS(IDS_AMR_Static_Load_Cases);
		else if(PolcD.nLoadPattern==1)
			strLoadPattern = _LS(IDS_AMR_Uniform_Acceleration);
		else if(PolcD.nLoadPattern==2)
			strLoadPattern = _LS(IDS_AMR_Mode_Shape);
		else if(PolcD.nLoadPattern==3)
			strLoadPattern = _LS(IDS_AMR_Mode_Shape_Story);
		else
		{
			ASSERT(0); return FALSE;
		}
		strTemp.Format(_LS(IDS_AMR_FORMAT_Load_Pattern_s), strLoadPattern);
		strText += strTemp; strText += _T("\n");
		
		int nLoadPattern = PolcD.aLoadPattern.GetSize();
		if(PolcD.nLoadPattern==0)  // Static Load Cases
		{
			strTemp=_T("");
			for(int j=0;j<nLoadPattern;++j)
			{
				T_STLD_K StldK=0;
				T_STLD_D StldD; StldD.Initialize();
				
				StldK = PolcD.aLoadPattern[j].LoadCaseKey;
				if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD))
				{
					ASSERT(0);
				}
				
				CString str = _T("");
				if(j==0)
					str.Format(_LS(IDS_AMR_FORMAT_minus_g_x_s), PolcD.aLoadPattern[j].dScaleFactor, StldD.LoadCaseName);	
				else
					str.Format(_LS(IDS_AMR_FORMAT_plus_g_x_s), PolcD.aLoadPattern[j].dScaleFactor, StldD.LoadCaseName);	
				
				strTemp+=str;
			}
		}
		else if(PolcD.nLoadPattern==1)  // Uniform Acceleration
		{
			strTemp=_T("");
			for(int j=0;j<nLoadPattern;++j)
			{
				CString strUniform=_T("");
				if(PolcD.aLoadPattern[j].LoadCaseKey==0)
					strUniform = _LS(IDS_AMR_DX);
				else if(PolcD.aLoadPattern[j].LoadCaseKey==1)
					strUniform = _LS(IDS_AMR_DY);
				else if(PolcD.aLoadPattern[j].LoadCaseKey==2)
					strUniform = _LS(IDS_AMR_DZ);
				else
				{
					ASSERT(0); return FALSE;
				}
				
				CString str = _T("");
				if(j==0)
					str.Format(_LS(IDS_AMR_FORMAT_minus_g_x_s), PolcD.aLoadPattern[j].dScaleFactor, strUniform);	
				else
					str.Format(_LS(IDS_AMR_FORMAT_plus_g_x_s), PolcD.aLoadPattern[j].dScaleFactor, strUniform);	
				
				strTemp+=str;
			}
		}
		else if(PolcD.nLoadPattern==2 || PolcD.nLoadPattern==3)  // Mode Shape, Mode Shape * Story Mass
		{
			strTemp=_T("");
			for(int j=0;j<nLoadPattern;++j)
			{	
				CString str = _T("");
				if(j==0)
					str.Format(_LS(IDS_AMR_FORMAT_minus_g_x_mode_d_), PolcD.aLoadPattern[j].dScaleFactor, PolcD.aLoadPattern[j].LoadCaseKey);	
				else
					str.Format(_LS(IDS_AMR_FORMAT_plus_g_x_mode_d_), PolcD.aLoadPattern[j].dScaleFactor, PolcD.aLoadPattern[j].LoadCaseKey);	
				
				strTemp+=str;
			}
		}
		else
		{
			ASSERT(0); return FALSE;
		}
		
		strText += strTemp; strText += _T("\n");
		if(i<(nPolc-1))
		{
			strText += _T(" "); strText += _T("\n");
		}
	}
	
	return TRUE;
}

// Floor Diaphragm/Rigid Link
BOOL CReportTreeText::WriteRigd(CString& strText)
{
	// floor diaphragm
	CArray<T_STOR_K, T_STOR_K> aStorK, aDStorK;
	T_STOR_D StorD;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	for (int i=aStorK.GetSize()-1; i>=0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);
		if (StorD.bFloorDiaphragm) aDStorK.Add(aStorK[i]);
	}
	int nDiap = aDStorK.GetSize();
	
	// rigid link
	CArray<T_RIGD_K, T_RIGD_K> aRigdK;
	T_RIGD_D RigdD;
	m_pDoc->m_pAttrCtrl->GetRigdKeyList(aRigdK);
	int nRigd = aRigdK.GetSize();
	
	if (nDiap + nRigd <= 0) return FALSE;
	
	// 10  15  60 
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Floor_Diaphragm_Rigid_Link));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Floor_Diaphragm_Rigid_Link));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_RIGD_DATA_TITLE)));
	strText += strTemp;
	strText += _T(" ");
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_RIGD_DATA_HEAD1)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_RIGD_DATA_HEAD2)));
	strText += strTemp;
	
	CString csNodeList, csPre;
	CArray<T_NODE_K, T_NODE_K> aNodeK;
	CNumericOptimizer opt;
	
	CStringArray arWriteData;
	for(int i=0; i<nDiap; i++)
	{
		m_pDoc->m_pAttrCtrl->GetStor(aDStorK[i], StorD);
		m_pDoc->m_pAttrCtrl->GetNodeKeyListStorDiaphragm(aDStorK[i], aNodeK);
		
		csPre.Format(_T("   %10s %15s  "), _ns(StorD.StoryName,10), _LS(IDS_MAIN_MDL_RIGD_FLOOR_DIAP));
		if (aNodeK.GetSize() > 0)
			qsort((void*)aNodeK.GetData(), aNodeK.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
		csNodeList = opt.Optimize((long*)aNodeK.GetData(), aNodeK.GetSize());
		
		WriteList2(arWriteData, csPre, csNodeList, 91, csPre.GetLength());
	}
	
	for(int i=0; i<nRigd; i++)
	{
		m_pDoc->m_pAttrCtrl->GetRigd(aRigdK[i], RigdD);
		
		int nVal = 0;
		int nSlaveType = RigdD.nSlaveType;
		
		csPre.Format(_T("   %10d %15d  "), aRigdK[i].key.entity, nSlaveType);
		if (RigdD.arSlave.GetSize() > 0)
			qsort((void*)RigdD.arSlave.GetData(), RigdD.arSlave.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
		csNodeList = opt.Optimize((long*)RigdD.arSlave.GetData(), RigdD.arSlave.GetSize());
		
		WriteList2(arWriteData, csPre, csNodeList, 91, csPre.GetLength());
	}
	
	int nWriteDataCnt = arWriteData.GetSize();
	for(int i=0; i<nWriteDataCnt; i++)
	{
		strTemp = arWriteData[i];
		strText += strTemp;
	}
	
	return TRUE;
}

// Story
BOOL CReportTreeText::WriteStor(CString& strText)
{
	// story
	CArray<T_STOR_K, T_STOR_K> aStorK;
	T_STOR_D StorD;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nSize = aStorK.GetSize();
	if (nSize <= 0) return FALSE;
	
	// ground level
	double dGL = 0.0;
	T_BLDC_D BldcD;
	if (!m_pDoc->m_pAttrCtrl->GetBldc(BldcD))
		BldcD.Initialize();
	
	// 10  15  10  25
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Story));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Story));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STORY_DATA_TITLE)));
	strText += strTemp;
	strText += _T(" ");
	if (BldcD.bUseBaseLevel) 
	{
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STORY_GROUND_LEVEL)),  BldcD.dBaseLevel);
		strText += strTemp;
	}
	
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STORY_DATA_HEAD1)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STORY_DATA_HEAD2)));
	strText += strTemp;
	
	CString csDiaphragm;
	double dHeight, dOldLevel;
	
	for (int i=nSize-1; i>=0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);
		
		if (i==nSize-1) dHeight = 0.0;
		else dHeight = dOldLevel - StorD.dStoryLevel;
		dOldLevel = StorD.dStoryLevel;
		
		if (StorD.bFloorDiaphragm) csDiaphragm = _LS(IDS_MAIN_MDL_STORY_CONSIDER);
		else csDiaphragm = _LS(IDS_MAIN_MDL_STORY_NOT_CONSIDER);
		
		strTemp.Format(_T("   %15s %10.3f %10.3f  %s \n"), 
			_ns(StorD.StoryName,15), StorD.dStoryLevel, dHeight, csDiaphragm);
		strText += strTemp;
	}
	
	return TRUE;
}

// Truss
BOOL CReportTreeText::WriteTrus(CString& strText)
{
	// truss
	CArray<T_ELEM_K, T_ELEM_K> aElemK, aTrusK;
	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	for (int i=0; i<aElemK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
		if (m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp))
			aTrusK.Add(aElemK[i]);
	}
	int nSize = aTrusK.GetSize();
	if (nSize <= 0) return FALSE;
	
	// 8  9  8  10  10  11  10  10  10
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Truss));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Truss));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_TRUSS_MEMBER_TITLE)));
	strText += strTemp;
	strText += _T(" ");
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_TRUSS_MEMBER_HEAD1)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_TRUSS_MEMBER_HEAD2)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_TRUSS_MEMBER_HEAD3)));
	strText += strTemp;
	
	T_MATL_D MatlD;
	T_SECT_D SectD;
	T_SECF_D SecfD;
	double FA, FASY, FASZ, FIX, FIY, FIZ; 
	double dLength;
	CString csAreaI, csAreaJ;
	CString csMatl, csSect, csTCN, cSize1, cSize2;
	T_SECT_STIFFNESS* pStiff;
	
	for(int i=0; i<nSize; i++)
	{
		csMatl = _T("-");
		csSect = _T("-");
		
		m_pDoc->m_pAttrCtrl->GetElem(aTrusK[i], ElemD);
		if (m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) csMatl = MatlD.Name;
		if (m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD)) csSect = SectD.SName;
		if (ElemD.eltyp == TRUSS_EL) csTCN = _T("N");
		else if (ElemD.eltyp == TRTENS_EL) csTCN = _T("T");
		else if (ElemD.eltyp == TRCOMP_EL) csTCN = _T("C");
		else ASSERT(0);
		
		FA = FASY = FASZ = FIX = FIY = FIZ = 1;
		if (m_pDoc->m_pAttrCtrl->GetSecfSect(ElemD.elpro, SecfD))
		{
			FA    = SecfD.dScaleFactor[0];
			FASY  = SecfD.dScaleFactor[1];
			FASZ  = SecfD.dScaleFactor[2];
			FIX   = SecfD.dScaleFactor[3];
			FIY   = SecfD.dScaleFactor[4];
			FIZ   = SecfD.dScaleFactor[5];
		}
		
		if (SectD.nStype == D_SECT_TYPE_REGULAR ||
			SectD.nStype == D_SECT_TYPE_USER ||
			SectD.nStype == D_SECT_TYPE_SRC ||
			SectD.nStype == D_SECT_TYPE_COMBINED)
		{
			pStiff = CSectUtil::GetProperStiffPosition(SectD, FALSE); ASSERT(pStiff);
			csAreaI.Format(_T("%10.4g"), pStiff->Area);
			csAreaJ = _T("-");     
		}
		else if (SectD.nStype == D_SECT_TYPE_TAPERED)
		{
			pStiff = &(SectD.SectBefore.SectI.Stiffness);
			csAreaI.Format(_T("%10.4g"), pStiff->Area);
			pStiff = &(SectD.SectBefore.SectJ.Stiffness);
			csAreaJ.Format(_T("%10.4g"), pStiff->Area);
		}
		else 
		{
			csAreaI = _T("-"); csAreaJ = _T("-");
		}
		
		dLength = m_pDoc->calcLAVElem(ElemD);
		
		strTemp.Format(_T("   %8d %8d %8d %10s %10s %11s %10s %10s %10.4g \n"), 
			aTrusK[i], ElemD.elnod[0], ElemD.elnod[1], _ns(csMatl,10), _ns(csSect,10), csTCN, csAreaI, csAreaJ, dLength);
		
		strText += strTemp;
	}
	
	return TRUE;
}

// Weight/Volume/Surface area of all member
BOOL CReportTreeText::WriteWVSA(CString& strText)
{
	// section
	CArray<T_SECT_K, T_SECT_K> aSectK;
	T_SECT_D SectD;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);
	int nSize = aSectK.GetSize();
	if (nSize <= 0) return FALSE;      // Composite Section은 우짜지? 그냥 찍어주나? -> 찍어주지 말자!!
	
	// 8  15   15   15  15   8   8 
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Weight_Volumn_Surface_area_of_all_member));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Weight_Volumn_Surface_area_of_all_member));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_WVSA_DATA_TITLE)));
	strText += strTemp;
	strText += _T(" ");
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_WVSA_DATA_HEAD1)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_WVSA_DATA_HEAD2)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_WVSA_DATA_HEAD3)));
	strText += strTemp;
	
	CArray<T_SECT_K, T_SECT_K> aSectKey;
	CArray<T_NODE_K, T_NODE_K> aNodeKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemKey;
	double dVolumn, dWeight, dSArea; 
	int nFrameNum, nTrussNum;
	
	for (int i=0; i<nSize; i++)
	{
		aSectKey.RemoveAll();
		aNodeKey.RemoveAll();
		aElemKey.RemoveAll();
		
		m_pDoc->m_pAttrCtrl->GetSect(aSectK[i], SectD);
		aSectKey.Add(aSectK[i]);
		m_pDoc->m_pAttrCtrl->GetKeyListSectUser(&aSectKey, &aNodeKey, &aElemKey);
		
		// 넘겨준 Elem들의 체적, 중량, 표면적 구하기
		if (!CalcWeightVolumnSurfaceArea(&SectD, &aElemKey, dSArea, dVolumn, dWeight, nFrameNum, nTrussNum))
		{ 
			ASSERT(0); 
			continue;
		}
		
		strTemp.Format(_T("   %8d %15s %15.4g %15.4g %15.4g %8d %8d \n"),
			aSectK[i], _ns(SectD.SName,15), dSArea, dVolumn, dWeight, nFrameNum, nTrussNum);
		
		strText += strTemp;
	}
	
	return TRUE;
}

// 넘어온 Element들의 표면적, 부피, 총중량을 구하는 함수 
// Surface Area = (Peri_O + Peri_I) * Member Length
// Volumn       = WArea * Member Length
// Weight       = Volumn * Weight Density
BOOL CReportTreeText::CalcWeightVolumnSurfaceArea(T_SECT_D* pSectD, CArray<T_ELEM_K, T_ELEM_K>* paElemK, 
																									double &rSArea, double &rVolumn, double &rWeight, int &rFrame, int &rTruss)
{
	ASSERT(pSectD);
	ASSERT(paElemK);
	
	rSArea  = 0.0;
	rVolumn = 0.0;
	rWeight = 0.0;
	rFrame = 0;
	rTruss = 0;
	
	T_ELEM_K ElemK;
	T_ELEM_D ElemD;
	double dPeri_O = 0.0;
	double dPeri_I = 0.0;
	double dArea   = 0.0;
	double dTotalLeng = 0.0;
	
	T_SECT_STIFFNESS* pStiff;
	switch(pSectD->nStype)
	{
	case D_SECT_TYPE_REGULAR:
	case D_SECT_TYPE_USER:
	case D_SECT_TYPE_SRC:
	case D_SECT_TYPE_COMBINED:
		pStiff = CSectUtil::GetProperStiffPosition(*pSectD, FALSE);
		dArea = pStiff->WArea;
		dPeri_I = *CSectUtil::GetStandForPeriIn(*pSectD, FALSE);
		dPeri_O = *CSectUtil::GetStandForPeriOut(*pSectD, FALSE);
		break;
	case D_SECT_TYPE_TAPERED:
		dArea = (pSectD->SectBefore.SectI.Stiffness.WArea + pSectD->SectBefore.SectJ.Stiffness.WArea) / 2.0;
		dPeri_I = (pSectD->SectBefore.SectI.PeriIn  + pSectD->SectBefore.SectJ.PeriIn)  / 2.0;
		dPeri_O = (pSectD->SectBefore.SectI.PeriOut + pSectD->SectBefore.SectJ.PeriOut) / 2.0;
		break;
	default: //표면적, 부피, 총중량의 경우 다른 단면도 고려.
		pStiff = CSectUtil::GetProperStiffPosition(*pSectD, FALSE);
		dArea = pStiff->WArea;
		dPeri_I = *CSectUtil::GetStandForPeriIn(*pSectD, FALSE);
		dPeri_O = *CSectUtil::GetStandForPeriOut(*pSectD, FALSE);
		break;
	}
	
	// calc total length
	double dLength;
	T_MATL_D MatlD;
	for (int i=0; i<paElemK->GetSize(); i++)
	{
		ElemK = paElemK->GetAt(i);
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
		if (m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp)) rTruss++;
		else rFrame++;
		
		dLength = m_pDoc->calcLAVElem(ElemD);
		dTotalLeng += dLength;
		
		// SRC 역시 Steel 기준 환산 WArea이므로 Steel의 Weight Density로 계산.
		if (m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD))
			rWeight += dArea*dLength* MatlD.Data1.Analysis.Density; 
	}
	
	rSArea  = (dPeri_I + dPeri_O) * dTotalLeng;
	rVolumn = dArea * dTotalLeng;
	
	return TRUE;
}

// Wall
BOOL CReportTreeText::WriteWall(CString& strText)
{
	// wall
	CArray<T_ELEM_K, T_ELEM_K> aElemK, aWallK;
	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	for (int i=0; i<aElemK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
		if (m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))
			aWallK.Add(aElemK[i]);
	}
	int nSize = aWallK.GetSize();
	if (nSize <= 0) return FALSE;
	
	// 8  6  6  6  6  15  15  10  
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Wall));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Wall));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_WALL_MEMBER_TITLE)));
	strText += strTemp;
	strText += _T(" ");
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_WALL_MEMBER_HEAD1)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_WALL_MEMBER_HEAD2)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_WALL_MEMBER_HEAD3)));
	strText += strTemp;
	
	T_MATL_D MatlD;
	T_THIK_D ThikD;
	CString csMatl, csThik;
	double dThik, dArea;
	
	for(int i=0; i<nSize; i++)
	{
		csMatl = _T("-");
		csThik = _T("-");
		
		m_pDoc->m_pAttrCtrl->GetElem(aWallK[i], ElemD);
		if (m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) csMatl = MatlD.Name;
		if (m_pDoc->m_pAttrCtrl->GetThik(ElemD.elpro, ThikD)) 
		{
			m_pDoc->m_pAttrCtrl->GetThikForThikName(ThikD, dThik);
			csThik.Format(_T("%g"), dThik);
		}
		
		dArea = m_pDoc->calcLAVElem(ElemD);
		
		strTemp.Format(_T("   %8d %6d %6d %6d %6d %15s %15s %10.4g \n"),
			aWallK[i], ElemD.elnod[0], ElemD.elnod[1], ElemD.elnod[2], ElemD.elnod[3], _ns(csMatl,15), _ns(csThik,15), dArea);
		
		strText += strTemp;
	}
	
	return TRUE;
}

// Static Loadcase
BOOL CReportTreeText::WriteStld(CString& strText)
{
	// loadcase
	CArray<T_STLD_K, T_STLD_K> aStldK;
	T_STLD_D StldD;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	int nSize = aStldK.GetSize();
	if (nSize == 0) return TRUE;
	
	// self weight factor
	T_BODF_K BodfK;
	T_BODF_D BodfD;
	double SWF_X, SWF_Y, SWF_Z;
	
	// 8  20  10   6   6   6   40
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Static_Loadcase));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Static_Loadcase));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_TITLE)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_HEAD1)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_HEAD2)));
	strText += strTemp;
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_HEAD3)));
	strText += strTemp;
	
	CString str, str1, str2;
	for (int i = 0; i < nSize; i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD)) ASSERT(0);
		
		SWF_X = SWF_Y = SWF_Z = 0.0;
		if (m_pDoc->m_pAttrCtrl->GetBodf(aStldK[i], BodfK, BodfD))
		{ SWF_X = BodfD.SX; SWF_Y = BodfD.SY; SWF_Z = BodfD.SZ; }
		
		str1 = str2 = _T("");
		BOOL bNeed2ndLine = _ns_devide(StldD.Description, 40, str1, str2);
			
		strTemp.Format(_T("   %8d %20s %10s %6.3f %6.3f %6.3f  %s \n"), 
			aStldK[i], _ns(StldD.LoadCaseName, 20), StldD.LoadCaseType, SWF_X, SWF_Y, SWF_Z, str1);

		strText += strTemp;

		if(bNeed2ndLine)
		{
			strTemp.Format(_T("   %8s %20s %10s %6s %6s %6s  %s \n"), 
				_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), str2);
		 
			strText += strTemp;
		}    
	}
	
	return TRUE;
}

BOOL CReportTreeText::WriteStldData(const CString strTextItem, CString& strText)
{
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	int nSize = aStldK.GetSize();
	
	CStringArray arWriteData;
	
	if(nSize > 0)
	{
		ClassifyData();
		
		if (_tcsicmp(strTextItem, _LS(IDS_AMR_Load_Combination                ))==0)  // load combination
		{ 
			WriteLcom(arWriteData);
		}
		else if(_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Response_Spectrum_Load))==0)  // Response Spectrum Load case Data
		{ 
			WriteSplc(arWriteData);
		}
		else
		{
			for(int i=0; i<nSize; i++)
			{
				if (m_aStldDataNum[i] < 1) continue;
				
				if(_tcsicmp(strTextItem, _LS(IDS_AMR_Wind_Load                       ))==0) //wind
				{ 
					if (!WriteWind(i, arWriteData)) continue;//return FALSE;
				}
				else if(_tcsicmp(strTextItem, _LS(IDS_AMR_Seismic_Load                    ))==0)  //seis
				{ 
					if (!WriteSeis(i, arWriteData)) continue;//return FALSE;
				}
				else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Self_Weight              ))==0)  // Self Weight
				{ 
					if (!WriteBodf(i, arWriteData)) continue;//return FALSE;
				}
				else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Nodal_Load               ))==0)  // Nodal Node
				{ 
					if (!WriteCnld(i, arWriteData)) continue;//return FALSE;
				}
				// 			else if(_stricmp(_LS(IDS_AMR_COMMON_Specified_Displacement), strTextItem) == 0)  // Specified Displacement
				// 			{ 
				// 				if (!WriteSdsp(i, arWriteData)) return FALSE;
				// 			}
				// 			else if(_stricmp(_LS(IDS_AMR_COMMON_Beam_Load), strTextItem) == 0)  // Beam Load
				// 			{ 
				// 				if (!WriteBmld(i, arWriteData)) return FALSE;
				// 			}
				else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Finishing_Material_Load  ))==0)  // Finishing Material Load
				{ 
					if (!WriteFmld(i, arWriteData)) continue;//return FALSE;
				}
				else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_System_Temperature       ))==0) // System Temperature
				{ 
					if (!WriteStmp(i, arWriteData)) continue;//return FALSE;
				}
				else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Nodal_Temperature        ))==0)  // Nodal Temperature
				{ 
					if (!WriteNtmp(i, arWriteData)) continue;//return FALSE;
				}
				else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Element_Temperature      ))==0) // Element Temperature
				{ 
					if (!WriteEtmp(i, arWriteData)) continue;//return FALSE;
				}
				else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Beam_Section_Temperature ))==0) // Beam Section Temperature
				{ 
					if (!WriteBtmp(i, arWriteData)) continue;//return FALSE;
				}
				else
				{
					continue;
				}
			}
		}
	}
	
	// 	if (_stricmp(_LS("Load_Combination"                 ), strTextItem) == 0)  // load combination
	// 	{ 
	// 		WriteLcom(arWriteData);
	// 	}
	// 	else if(_stricmp(_LS("Response_Spectrum_Load_Case_Data" ), strTextItem) == 0)  // Response Spectrum Load case Data
	// 	{ 
	// 		WriteSplc(arWriteData);
	// 	}
	// 	else
	// 	{
	// 		return FALSE;
	// 	}
	
	strText = _T("");
	CString strTemp=_T("");
	
	int nWriteDataCnt = arWriteData.GetSize();
	if(nWriteDataCnt<=0)
		return FALSE;
	
	for(int i=0; i<nWriteDataCnt; i++)
	{
		strTemp = arWriteData[i];
		strText += strTemp;
	}
	
	DeleteStldData();
	
	return TRUE;
}

int CReportTreeText::ClassifyData()
{
	//m_aStldDataNum.RemoveAll(); 
	
	CArray<T_STLD_K, T_STLD_K> aStldKey;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldKey);
	int nSize = aStldKey.GetSize();
	if (nSize == 0) return 0;
	
	if(m_bIsSetStld)
	{
		ASSERT(m_nStldNum>0 && m_nStldNum==nSize);
		return m_nStldNum;
	}
	
	m_nStldNum = nSize;
	
	for (int i = 0; i < D_STLD_DATA_NUM; i++)
		m_aKeyList[i] = new CArray<UINT, UINT>[nSize];
	
	CMap<UINT, UINT, int, int> KeyPosMap;
	KeyPosMap.InitHashTable(31);
	for(int i=0; i < nSize; i++) KeyPosMap.SetAt(aStldKey[i], i);
	
	T_BODF_K kbodf; T_BODF_D bodf;
	T_CNLD_K kcnld; T_CNLD_D cnld;
	T_SDSP_K ksdsp; T_SDSP_D sdsp;
	T_BMLD_K kbmld; T_BMLD_D bmld;
	T_PRST_K kprst; T_PRST_D prst;
	T_PTNS_K kptns; T_PTNS_D ptns;
	T_FMLD_K kfmld; T_FMLD_D fmld;
	T_PRES_K kpres; T_PRES_D pres;
	T_STMP_K kstmp; T_STMP_D stmp;
	T_NTMP_K kntmp; T_NTMP_D ntmp;
	T_ETMP_K ketmp; T_ETMP_D etmp;
	T_BTMP_K kbtmp; T_BTMP_D btmp;
	T_GTMP_K kgtmp; T_GTMP_D gtmp;
	T_WIND_K kwind; T_WIND_D wind;
	T_SEIS_K kseis; T_SEIS_D seis;
	T_TDPL_K ktdpl; T_TDPL_D tdpl;
	
	POSITION pos;
	int nIX;
	BOOL bExist;
	// bodf
	pos = m_pDoc->m_pAttrCtrl->GetStartBodf();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextBodf(pos, kbodf, bodf);
		bExist = KeyPosMap.Lookup(bodf.LoadCaseKey, nIX);  // loadcase에 대응하는 배열 인덱스 찿기
		ASSERT(bExist);
		m_aKeyList[D_AMR_BODF_IX][nIX].Add(kbodf);          // 해당 loadcase에 데이타 키 넣기
	}
	// cnld
	pos = m_pDoc->m_pAttrCtrl->GetStartCnld();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextCnld(pos, kcnld, cnld);
		bExist = KeyPosMap.Lookup(cnld.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_CNLD_IX][nIX].Add(kcnld.keymap);
	}
	// sdsp
	pos = m_pDoc->m_pAttrCtrl->GetStartSdsp();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextSdsp(pos, ksdsp, sdsp);
		bExist = KeyPosMap.Lookup(sdsp.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_SDSP_IX][nIX].Add(ksdsp.keymap);
	}
	// bmld
	pos = m_pDoc->m_pAttrCtrl->GetStartBmld();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextBmld(pos, kbmld, bmld);
		bExist = KeyPosMap.Lookup(bmld.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_BMLD_IX][nIX].Add(kbmld.keymap);
	}
	// prst
	pos = m_pDoc->m_pAttrCtrl->GetStartPrst();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextPrst(pos, kprst, prst);
		bExist = KeyPosMap.Lookup(prst.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_PRST_IX][nIX].Add(kprst.keymap);
	}
	// ptns
	pos = m_pDoc->m_pAttrCtrl->GetStartPtns();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextPtns(pos, kptns, ptns);
		bExist = KeyPosMap.Lookup(ptns.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_PTNS_IX][nIX].Add(kptns.keymap);
	}
	// pres
	pos = m_pDoc->m_pAttrCtrl->GetStartPres();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextPres(pos, kpres, pres);
		bExist = KeyPosMap.Lookup(pres.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_PRES_IX][nIX].Add(kpres.keymap);
	}
	// fmld
	pos = m_pDoc->m_pAttrCtrl->GetStartFmld();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextFmld(pos, kfmld, fmld);
		bExist = KeyPosMap.Lookup(fmld.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_FMLD_IX][nIX].Add(kfmld.keymap);
	}
	// stmp
	pos = m_pDoc->m_pAttrCtrl->GetStartStmp();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextStmp(pos, kstmp, stmp);
		bExist = KeyPosMap.Lookup(stmp.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_STMP_IX][nIX].Add(kstmp);
	}
	// ntmp
	pos = m_pDoc->m_pAttrCtrl->GetStartNtmp();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextNtmp(pos, kntmp, ntmp);
		bExist = KeyPosMap.Lookup(ntmp.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_NTMP_IX][nIX].Add(kntmp.keymap);
	}
	// etmp
	pos = m_pDoc->m_pAttrCtrl->GetStartEtmp();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextEtmp(pos, ketmp, etmp);
		bExist = KeyPosMap.Lookup(etmp.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_ETMP_IX][nIX].Add(ketmp.keymap);
	}
	// btmp
	pos = m_pDoc->m_pAttrCtrl->GetStartBtmp();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextBtmp(pos, kbtmp, btmp);
		bExist = KeyPosMap.Lookup(btmp.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_BTMP_IX][nIX].Add(kbtmp.keymap);
	}
	// gtmp
	pos = m_pDoc->m_pAttrCtrl->GetStartGtmp();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextGtmp(pos, kgtmp, gtmp);
		bExist = KeyPosMap.Lookup(gtmp.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_GTMP_IX][nIX].Add(kgtmp.keymap);
	}
	// wind
	pos = m_pDoc->m_pAttrCtrl->GetStartWind();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextWind(pos, kwind, wind);
		bExist = KeyPosMap.Lookup(kwind, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_WIND_IX][nIX].Add(kwind);
	}
	// seis
	pos = m_pDoc->m_pAttrCtrl->GetStartSeis();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextSeis(pos, kseis, seis);
		bExist = KeyPosMap.Lookup(kseis, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_SEIS_IX][nIX].Add(kseis);
	}
	// tdpl
	pos = m_pDoc->m_pAttrCtrl->GetStartTdpl();
	while (pos) 
	{
		m_pDoc->m_pAttrCtrl->GetNextTdpl(pos, ktdpl, tdpl);
		bExist = KeyPosMap.Lookup(tdpl.LoadCaseKey, nIX);
		ASSERT(bExist);
		m_aKeyList[D_AMR_TDPL_IX][nIX].Add(ktdpl.keymap);
	}
	
	// 각 Loadcase에 대한 데이타 갯수를 계산한다.
	for(int i=0; i < nSize; i++)
	{
		int nCount = 0;
		for (int j = 0; j < D_STLD_DATA_NUM; j++)
			nCount += m_aKeyList[j][i].GetSize();
		m_aStldDataNum.Add(nCount);
	}
	
	if(nSize>0)
		m_bIsSetStld = TRUE;
	
	return nSize;
}

void CReportTreeText::DeleteStldData()
{
	if(m_nStldNum>0)
	{
		for(int i=0; i<D_STLD_DATA_NUM; i++)
		{
			if(m_aKeyList[i]!=NULL)
			{
				m_aKeyList[i]->RemoveAll();
				
				delete[] m_aKeyList[i];
				m_aKeyList[i] = NULL;
			}
		}
		m_aStldDataNum.RemoveAll();
	}
	m_bIsSetStld = FALSE;
}

BOOL CReportTreeText::WriteBodf(int nIndex, CStringArray& arWriteData)
{
	// self weight
	int nSize = m_aKeyList[D_AMR_BODF_IX][nIndex].GetSize();
	if (nSize <= 0) return FALSE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	
	CString strTemp=_T("");
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Self_Weight));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Self_Weight));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	
	T_BODF_K BodfK;
	T_BODF_D BodfD;
	CString csValue;
	
	BodfK = m_aKeyList[D_AMR_BODF_IX][nIndex].GetAt(0);
	m_pDoc->m_pAttrCtrl->GetBodf(BodfK, BodfD);
	csValue.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BODF_DATA_INFO)), 
		BodfD.SX, BodfD.SY, BodfD.SZ);
	
	arWriteData.Add(csValue);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
}

BOOL CReportTreeText::WriteCnld(int nIndex, CStringArray& arWriteData)
{
	// nodal load
	int nSize = m_aKeyList[D_AMR_CNLD_IX][nIndex].GetSize();
	if (nSize <= 0) return FALSE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	
	CString strTemp=_T("");
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Nodal_Load));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Nodal_Load));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CNLD_DATA_HEAD1)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_CNLD_DATA_HEAD2)));
	
	T_CNLD_K CnldK;
	T_CNLD_D CnldD;
	
	qsort((void*)m_aKeyList[D_AMR_CNLD_IX][nIndex].GetData(), nSize, sizeof(UINT), CCompFunc::i20x12Asc);
	for (int i=0; i<nSize; i++)
	{
		CnldK.keymap = m_aKeyList[D_AMR_CNLD_IX][nIndex].GetAt(i);
		m_pDoc->m_pAttrCtrl->GetCnld(CnldK, CnldD);
		strTemp.Format(_T("   %8d %10.4g %10.4g %10.4g %10.4g %10.4g %10.4g \n"),
			CnldK.key.entity, 
			CnldD.Force[0], CnldD.Force[1], CnldD.Force[2], 
			CnldD.Force[3], CnldD.Force[4], CnldD.Force[5]);
		
		arWriteData.Add(strTemp);
	}
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
}

BOOL CReportTreeText::WriteSdsp(int nIndex, CStringArray& arWriteData)
{
	// specified displacement
	int nSize = m_aKeyList[D_AMR_SDSP_IX][nIndex].GetSize();
	if (nSize <= 0) return TRUE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	CString strTemp=_T("");
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_SPDP_DATA_HEAD1)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_SPDP_DATA_HEAD2)));
	
	T_SDSP_K SdspK;
	T_SDSP_D SdspD;
	CString csFlag;
	
	qsort((void*)m_aKeyList[D_AMR_SDSP_IX][nIndex].GetData(), nSize, sizeof(UINT), CCompFunc::i20x12Asc);
	for (int i=0; i<nSize; i++)
	{
		SdspK.keymap = m_aKeyList[D_AMR_SDSP_IX][nIndex].GetAt(i);
		m_pDoc->m_pAttrCtrl->GetSdsp(SdspK, SdspD);
		
		csFlag.Format(_T("%c%c%c%c%c%c"), 
			SdspD.Flag[0], SdspD.Flag[1], SdspD.Flag[2],
			SdspD.Flag[3], SdspD.Flag[4], SdspD.Flag[5]);
		
		strTemp.Format(_T("   %8d %8s %10.4g %10.4g %10.4g %10.4g %10.4g %10.4g "),
			SdspK.key.entity, csFlag,
			SdspD.Displacement[0], SdspD.Displacement[1], SdspD.Displacement[2],
			SdspD.Displacement[3], SdspD.Displacement[4], SdspD.Displacement[5]);
		
		arWriteData.Add(strTemp);
	}
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
} 

BOOL CReportTreeText::WriteBmld(int nIndex, CStringArray& arWriteData)
{
	// beam load
	int nSize = m_aKeyList[D_AMR_BMLD_IX][nIndex].GetSize();
	if (nSize <= 0) return TRUE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	CString strTemp=_T("");
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BMLD_DATA_HEAD1)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BMLD_DATA_HEAD2)));
	
	T_BMLD_K BmldK;
	T_BMLD_D BmldD;
	
	CString aType[] = {ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BMLD_CENT_FORCE)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BMLD_CENT_MT)), 
		ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BMLD_UNIFORM_LD)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BMLD_UNIFORM_MT))};
	CString aDir[]  = {_T("LX"), _T("LY"), _T("LZ"), _T("GX"), _T("GY"), _T("GZ"), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BMLD_DIR_VECTOR))};
	
	qsort((void*)m_aKeyList[D_AMR_BMLD_IX][nIndex].GetData(), nSize, sizeof(UINT), CCompFunc::i20x12Asc);
	for (int i=0; i<nSize; i++)
	{
		BmldK.keymap = m_aKeyList[D_AMR_BMLD_IX][nIndex].GetAt(i);
		m_pDoc->m_pAttrCtrl->GetBmld(BmldK, BmldD);
		ASSERT(BmldD.LoadType >= 1 && BmldD.LoadType <= 4);
		
		strTemp.Format(_T("   %8d %20s %6s %5s %5.2g %8.3g %5.2g %8.3g %5.2g %8.3g %5.2g %8.3g "),
			BmldK.key.entity, aType[BmldD.LoadType-1], aDir[BmldD.LoadDirection-1], YesNo(BmldD.Projection),
			BmldD.DistanceRatio[0], BmldD.Force_Type[BmldD.LoadType-1][0],
			BmldD.DistanceRatio[1], BmldD.Force_Type[BmldD.LoadType-1][1],
			BmldD.DistanceRatio[2], BmldD.Force_Type[BmldD.LoadType-1][2],
			BmldD.DistanceRatio[3], BmldD.Force_Type[BmldD.LoadType-1][3]);
		
		arWriteData.Add(strTemp);
	}
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
}

BOOL CReportTreeText::WriteFmld(int nIndex, CStringArray& arWriteData)
{
	// finishing material load
	int nSize = m_aKeyList[D_AMR_FMLD_IX][nIndex].GetSize();
	if (nSize <= 0) return FALSE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	
	CString strTemp=_T("");
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Finishing_Material_Load));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Finishing_Material_Load));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_FMLD_DATA_HEAD1)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_FMLD_DATA_HEAD2)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_FMLD_DATA_HEAD3)));
	
	T_FMLD_K FmldK;
	T_FMLD_D FmldD;
	CString aType[] = {ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_FMLD_ENVL)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_FMLD_FILL)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_FMLD_SRND))};
	CString aFace[] = {ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_FMLD_NONE)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_FMLD_HALF)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_FMLD_FULL))};
	CString aDir[]  = {_T("GX"), _T("GY"), _T("GZ")};
	
	qsort((void*)m_aKeyList[D_AMR_FMLD_IX][nIndex].GetData(), nSize, sizeof(UINT), CCompFunc::i20x12Asc);
	for (int i=0; i<nSize; i++)
	{
		FmldK.keymap = m_aKeyList[D_AMR_FMLD_IX][nIndex].GetAt(i);
		m_pDoc->m_pAttrCtrl->GetFmld(FmldK, FmldD);
		
		strTemp.Format(_T("   %8d %13s %6s %6s %6s %6s %9.4g % 8.4g %5s %8.4g "),
			FmldK.key.entity, aType[FmldD.nCoveringType], aFace[FmldD.nCoveringRange[0]],
			aFace[FmldD.nCoveringRange[1]], aFace[FmldD.nCoveringRange[2]], aFace[FmldD.nCoveringRange[3]],
			FmldD.dThickness, FmldD.dDensity, aDir[FmldD.nDirection-1], FmldD.dScaleFactor);
		
		arWriteData.Add(strTemp);
	}
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
}

BOOL CReportTreeText::WriteStmp(int nIndex, CStringArray& arWriteData)
{
	// system temperature load
	int nSize = m_aKeyList[D_AMR_STMP_IX][nIndex].GetSize();
	if (nSize <= 0) return FALSE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	CString strTemp=_T("");
	
	CString csValue;
	T_STMP_K StmpK;
	T_STMP_D StmpD;
	T_STYP_D StypD;
	if (!m_pDoc->m_pAttrCtrl->GetStyp(StypD))
		StypD.Initialize();
	
	StmpK = m_aKeyList[D_AMR_STMP_IX][nIndex].GetAt(0);
	m_pDoc->m_pAttrCtrl->GetStmp(StmpK, StmpD);
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_System_Temperature));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_System_Temperature));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STMP_DATA_TITLE)));
	arWriteData.Add(strTemp);
	csValue.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STMP_DATA_INFO)), StypD.dblInitTemp, StmpD.dblSysTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	arWriteData.Add(csValue);
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
}

BOOL CReportTreeText::WriteNtmp(int nIndex, CStringArray& arWriteData)
{
	// nodal temperature load
	int nSize = m_aKeyList[D_AMR_NTMP_IX][nIndex].GetSize();
	if (nSize <= 0) return FALSE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	CString strTemp=_T("");
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Nodal_Temperature));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Nodal_Temperature));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NTMP_DATA_TITLE)));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NTMP_DATA_HEAD1)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_NTMP_DATA_HEAD2)));
	
	T_NTMP_K NtmpK;
	T_NTMP_D NtmpD;
	
	for (int i=0; i<nSize; i++)
	{
		NtmpK.keymap = m_aKeyList[D_AMR_NTMP_IX][nIndex].GetAt(i);
		m_pDoc->m_pAttrCtrl->GetNtmp(NtmpK, NtmpD);
		
		strTemp.Format(_T("   %8d  %11.4g \n"), 
			NtmpK.key.entity, NtmpD.dblNodalTemp);
		
		arWriteData.Add(strTemp);
	}
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
}

BOOL CReportTreeText::WriteEtmp(int nIndex, CStringArray& arWriteData)
{
	// element temperature load
	int nSize = m_aKeyList[D_AMR_ETMP_IX][nIndex].GetSize();
	if (nSize <= 0) return FALSE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	CString strTemp=_T("");
	
	// 8  11
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Element_Temperature));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Element_Temperature));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_ETMP_DATA_TITLE)));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_ETMP_DATA_HEAD1)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_ETMP_DATA_HEAD2)));
	
	T_ETMP_K EtmpK;
	T_ETMP_D EtmpD;
	
	for (int i=0; i<nSize; i++)
	{
		EtmpK.keymap = m_aKeyList[D_AMR_ETMP_IX][nIndex].GetAt(i);
		m_pDoc->m_pAttrCtrl->GetEtmp(EtmpK, EtmpD);
		
		strTemp.Format(_T("   %8d  %11.4g \n"), 
			EtmpK.key.entity, EtmpD.dblElemTemp);
		
		arWriteData.Add(strTemp);
	}
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
}

BOOL CReportTreeText::WriteBtmp(int nIndex, CStringArray& arWriteData)
{
	// beam section temperature load
	int nSize = m_aKeyList[D_AMR_BTMP_IX][nIndex].GetSize();
	if (nSize <= 0) return FALSE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	
	CString strTemp=_T("");
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Beam_Section_Temperature));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_COMMON_Beam_Section_Temperature));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BTMP_DATA_TITLE)));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BTMP_DATA_HEAD1)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BTMP_DATA_HEAD2)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BTMP_DATA_HEAD3)));
	
	T_BTMP_K BtmpK;
	T_BTMP_D BtmpD;
	CString aDir[] = {_T("LY"), _T("LZ")};
	CString aPos[] = {ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BTMP_CENT)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BTMP_ENDT)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_BTMP_ENDB))};
	
	T_BTMP_STMP *pStmp;
	for (int i=0; i<nSize; i++)
	{
		BtmpK.keymap = m_aKeyList[D_AMR_BTMP_IX][nIndex].GetAt(i);
		m_pDoc->m_pAttrCtrl->GetBtmp(BtmpK, BtmpD);
		
		pStmp = &BtmpD.SectTmp[0];
		strTemp.Format(_T("   %8d %5s %10s %10.4g %10.4g %8.3g %8.3g %8.3g %8.3g %8.3g \n"),
			BtmpK.key.entity, aDir[BtmpD.nDirection], aPos[BtmpD.nRefPosition], 
			pStmp->dElast, pStmp->dThermal, pStmp->dB, pStmp->dH1, pStmp->dH2, pStmp->dT1, pStmp->dT2);
		
		arWriteData.Add(strTemp);
		
		for (int k=1; k<BtmpD.SectTmp.GetSize(); k++)
		{
			pStmp = &BtmpD.SectTmp[k];
			if (pStmp->nMaterial == 0) break;
			strTemp.Format(_T("   %8s %5s %10s %10.4g %10.4g %8.3g %8.3g %8.3g %8.3g %8.3g \n"),
				_T(""), _T(""), _T(""), 
				pStmp->dElast, pStmp->dThermal, pStmp->dB, pStmp->dH1, pStmp->dH2, pStmp->dT1, pStmp->dT2);
			
			arWriteData.Add(strTemp);
		}
	}
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
}

// [WindCode] 추가시 수정
BOOL CReportTreeText::WriteWind(int nIndex, CStringArray& arWriteData)
{
	// wind load
	int nSize = m_aKeyList[D_AMR_WIND_IX][nIndex].GetSize();
	if (nSize <= 0) return FALSE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	
	CString strTemp=_T("");
	//strTemp.Format("%s\n", _LS(IDS_AMR_Wind_Load));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_Wind_Load));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	
	T_WIND_K WindK;
	T_WIND_D WindD;
	CLateralLoad load;
	// 1=KS.Arch.1992		2=JAPAN.1987		3=UBC.1997			4=ANSI.1982		5=KS.Arch.2000  6=IBC.2000
	// 7=EURO-1(1992)		8=BS6399(1997)	9=CH2002				10=JPN2000		11=NBC1995      12=IS1987
	// 13=TAIWAN86			14=JP2004				15=EURO-1(2005) 16=KBC2009		17=IBC.2009			18=IBC.2012
	// 19=CH2012				20=NSR-10				21=KBC2016      22=IS875(2015) 23 =CH2019
	CString aWindCode[] = {
			_T("KS.Arch.1992"), _T("JAPAN.1987")	, _T("UBC.1997")		, _T("ANSI.1982")	, _T("KS.Arch.2000"), _T("IBC.2000"),
			_T("EURO-1(1992)"), _T("BS6399(1997)"), _T("CH2002")			, _T("JPN2000")		, _T("NBC1995")			, _T("IS1987")	,
			_T("TAIWAN8")			, _T("JP2004")			, _T("EURO-1(2005)"), _T("KBC(2009)")	, _T("IBC.2009")		, _T("IBC.2012"),
			_T("CH2012")			, _T("NSR-10")			, _T("KBC(2016)")   , _T("IS875(2015)") , _T("CH2021"), _T("KDS(41-10-15:2019)")
	};
	
	for (int i=0; i<nSize; i++)
	{
		WindK = m_aKeyList[D_AMR_WIND_IX][nIndex].GetAt(i);
		m_pDoc->m_pAttrCtrl->GetWind(WindK, WindD);
		
		ASSERT(WindD.nCodeType >= 1 && WindD.nCodeType <= 22);
		strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_AMR_SUMMARY_Code_s__)), aWindCode[WindD.nCodeType-1]);
		arWriteData.Add(strTemp);
		arWriteData.Add(_T("------------------------------------------------------------------------------------------------------"));
		WriteWindProfile(WindD, arWriteData);
		//    arWriteData.Add("------------------------------------------------------------------------------------------------------");
	}
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
}

// [SeisCode] 추가시 수정
BOOL CReportTreeText::WriteSeis(int nIndex, CStringArray& arWriteData)
{
	// seis load
	int nSize = m_aKeyList[D_AMR_SEIS_IX][nIndex].GetSize();
	if (nSize <= 0) return FALSE;
	
	T_STLD_D StldD; 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	m_pDoc->m_pAttrCtrl->GetStld(aStldK[nIndex], StldD);
	
	CString strTemp=_T("");
	//strTemp.Format("%s\n", _LS(IDS_AMR_Seismic_Load));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_Seismic_Load));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_CASE_NAME)), StldD.LoadCaseName);
	arWriteData.Add(strTemp);
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	
	T_SEIS_K SeisK;
	T_SEIS_D SeisD;

	// 1=KS.Arch.1992	2=UBC.1991	3=UBC.1997	4=ATC3-06	5=KS2000	6=JIS			7=IBC2000
	// 8=EURO1996		9=CH2002	10=NBC1995  11=IS2002	12=TAIWAN86 13=KBC2005		14=CHSH2003
	// 15=TAIWAN02		16=EURO2004 17=KBC2009	18=TAIWAN11 19=NTC2008	20=CH2010		21=IBC2009
	// 22=IBC2012		23=NTC2012	24=NSR2010	25=KBC2016  26=P100-1   27=IS1893-2016	28=NTC2018
	// 29=KDS2019
	CString aSeisCode[] = {
		_T("KS.Arch.1992")			, _T("UBC.1991")				, _T("UBC.1997")	, _T("ATC3-06")		, _T("KS2000")		, _T("JIS")			, _T("IBC2000")	,
		_T("EURO1996")				, _T("CH2002")					, _T("NBC1995")		, _T("IS2002")		, _T("TAIWAN86")	, _T("KBC2005")		, _T("CHSH2003"),
		_T("TAIWAN02")				, _T("EURO2004")				, _T("KBC(2009)")	, _T("TAIWAN11")	, _T("NTC2008")		, _T("CH2010")		, _T("IBC2009")	,
		_T("IBC2012")				, _T("NTC2012")					, _T("NSR-10")		, _T("KBC(2016)")	, _T("P100-1(2013)"), _T("IS1893-2016")	, _T("NTC2018") ,
		_T("KDS(41-17-00:2019)")	, _T("DPT.1301/1302-61:2018")	, _T("Taiwan22") };
		
		for (int i=0; i<nSize; i++)
		{
			SeisK = m_aKeyList[D_AMR_SEIS_IX][nIndex].GetAt(i);
			m_pDoc->m_pAttrCtrl->GetSeis(SeisK, SeisD);
			
			ASSERT(SeisD.nCodeType >= 1 && SeisD.nCodeType <= 31);	//v690, v691....
			strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_AMR_SUMMARY_Code_s__)), aSeisCode[SeisD.nCodeType-1]);
			arWriteData.Add(strTemp);
			arWriteData.Add(_T("------------------------------------------------------------------------------------------------------"));
			WriteSeisProfile(SeisD, arWriteData);
			//    arWriteData.Add("------------------------------------------------------------------------------------------------------");
	}    
		arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
		return TRUE;
}

BOOL CReportTreeText::WriteWindProfile(T_WIND_D& rData, CStringArray& arWriteData)
{
	CString csFileName = m_pDoc->GetPathName();
	CFileCtrl fc(csFileName);
	csFileName = fc.GetFilePath();
	
	CString strFile = csFileName + _LSX(WindProfile.txt);
	CStdioFile file;
	
	CLateralLoad load;
	BOOL bCrossWind = FALSE;
	BOOL bTosionWind = FALSE;
	if (rData.nCodeType == CH_W_2012)
	{
		bCrossWind = rData.CodeParam.CH2012.bCross;
		bTosionWind = rData.CodeParam.CH2012.bTorsion;
		rData.CodeParam.CH2012.bTorsion = FALSE;
		rData.CodeParam.CH2012.bCross = FALSE;
	}
	else if (rData.nCodeType == CH_W_2021)
	{
		bCrossWind = rData.CodeParam.CH2019.bCross;
		bTosionWind = rData.CodeParam.CH2019.bTorsion;
		rData.CodeParam.CH2019.bTorsion = FALSE;
		rData.CodeParam.CH2019.bCross = FALSE;
	}
	load.MakeWindLoad(rData, TRUE, strFile, FALSE, TRUE);
	if (rData.nCodeType == CH_W_2012)
	{
		rData.CodeParam.CH2012.bTorsion = bCrossWind;
		rData.CodeParam.CH2012.bCross = bTosionWind;
	}
	else if (rData.nCodeType == CH_W_2021)
	{
		rData.CodeParam.CH2019.bTorsion = bCrossWind;
		rData.CodeParam.CH2019.bCross = bTosionWind;
	}

	TRY
	{
		CFileException FileException;
		if (!file.Open(strFile, CFile::modeRead, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
		else
		{
			CString strTemp;
			while(file.ReadString(strTemp))
			{
				if(strTemp==_T(""))
					arWriteData.Add(_T(" \n"));
				else
					arWriteData.Add(strTemp+_T("\n"));
			}
		}
		file.Close();
		DeleteFile(strFile);
	}
	CATCH(CFileException, e)
	{
		e->ReportError();
		file.Abort();
	}
	END_CATCH
		
		return TRUE;
}

BOOL CReportTreeText::WriteSeisProfile(T_SEIS_D& rData, CStringArray& arWriteData)
{
	CString csFileName = m_pDoc->GetPathName();
	CFileCtrl fc(csFileName);
	csFileName = fc.GetFilePath();
	
	CString strFile = csFileName + _LSX(SeisProfile.txt);
	CStdioFile file;
	
	CLateralLoad load;
	load.MakeSeismicLoad(rData, TRUE, strFile, FALSE, TRUE);
	
	TRY
	{
		CFileException FileException;
		if (!file.Open(strFile, CFile::modeRead, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
		else
		{
			CString strTemp;
			while(file.ReadString(strTemp))
			{
				if(strTemp==_T(""))
					arWriteData.Add(_T(" \n"));
				else
					arWriteData.Add(strTemp+_T("\n"));
			}
		}
		file.Close();
		DeleteFile(strFile);
	}
	CATCH(CFileException, e)
	{
		e->ReportError();
		file.Abort();
	}
	END_CATCH
		
		return TRUE;
}

BOOL CReportTreeText::WriteSplc(CStringArray& arWriteData)
{
	CArray<T_SPLC_K, T_SPLC_K> aSplcK;
	T_SPLC_D SplcD;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
	int nSize = aSplcK.GetSize();
	if (nSize <= 0) return FALSE;
	
	// 15 15 6 8 8 8 15
	CString strTemp=_T("");
	//strTemp.Format("%s\n", _LS(IDS_AMR_SUMMARY_RESPONSE_SPECTRUM_LOAD_DATA));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_SUMMARY_RESPONSE_SPECTRUM_LOAD_DATA));
	arWriteData.Add(strTemp);
	strTemp.Format(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_SPLC_DATA_TITLE)));
	arWriteData.Add(strTemp);
	//   arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_SPLC_DATA_TITLE)));
	// 	arWriteData.Add(" "); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_SPLC_DATA_HEAD1)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_SPLC_DATA_HEAD2)));
	arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_SPLC_DATA_HEAD3)));
	
	T_SPFC_D SpfcD;
	CString csEccent;
	CString aDir[] = { _T("X-Y"), _T("Z") };
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
	
	// *^^*
	// IDS_MAIN_MDL_SPLC_DATA_TITLE, "*** RESPONSE SPECTRUM LOAD CASE DATA                                               ");
	// IDS_MAIN_MDL_SPLC_DATA_HEAD1, "              NAME        FUNCTION   DIR.    ANGLE    SCALE   PERIOD      ACCIDENTAL ");
	// IDS_MAIN_MDL_SPLC_DATA_HEAD2, "                              NAME                            FACTOR    ECCENTRICITY ");
	// IDS_MAIN_MDL_SPLC_DATA_HEAD3, "   --------------- --------------- ------ -------- -------- -------- --------------- ");
	
	// IDS_MAIN_MDL_AUTOMATIC, "Automatic");
	// IDS_MAIN_MDL_USER_DEFINED, "User Defined");
}

BOOL CReportTreeText::WriteLcom(CStringArray& arWriteData)
{
	// load combination
	CArray<T_LCOM_K, T_LCOM_K> aGenLcomK, aStlLComK, aConLcomK, aSRCLcomK, aFdnLcomK, aAluLcomK, aScmLcomK;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL,  aGenLcomK);
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL,    aStlLComK);
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, aConLcomK);
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SRC,      aSRCLcomK);
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_FDN,      aFdnLcomK);
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_ALUMINUM, aAluLcomK);
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STLCOMP,  aScmLcomK);
	int nGenSize = aGenLcomK.GetSize();
	int nStlSize = aStlLComK.GetSize();
	int nConSize = aConLcomK.GetSize();
	int nSRCSize = aSRCLcomK.GetSize();
	int nFdnSize = aFdnLcomK.GetSize();
	int nAluSize = aAluLcomK.GetSize();
	int nScmSize = aScmLcomK.GetSize();
	if (nGenSize+nStlSize+nConSize+nSRCSize+nFdnSize+nAluSize+nScmSize <= 0) return FALSE;
	
	//   arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMBINATION_DATA_TITLE)));
	// 	arWriteData.Add(" "); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	
	T_LCOM_D LcomD;
	CString csActive;
	CString aLcomType[] = {ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_ADD)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_ENVELOPE)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_ABS)), ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_SRSS))};
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_Load_Combination));
	strTemp.Format(_T("[DRG_FONT]|FC| bold=true align=center[/DRG_FONT]< %s >\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_Load_Combination));
	arWriteData.Add(strTemp);
	
	// general
	if (nGenSize)
	{
		// 8 10 10 10 45
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_TITLE)));
		arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD1)));
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD2)));
		
		CString str1, str2;
		for (int i=0; i<nGenSize; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL, aGenLcomK[i], LcomD)) ASSERT(0);
			
#if defined(_CIVIL)
			if (LcomD.nActive == 1) csActive = ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STRENGTH));
#else
			csActive = m_pDoc->m_pAttrCtrl->GetActivName4MDL(LcomD.nActive);
#endif
			
			str1 = str2 = _T("");
			BOOL bNeed2ndLine = _ns_devide(LcomD.Description, 45, str1, str2);
			
			strTemp.Format(_T("   %8d %10s %10s %10s  %s \n"), 
				LcomD.LoadCombId, _ns(LcomD.LoadCombName,10), aLcomType[LcomD.LoadCombType], 
				csActive, str1);
			
			arWriteData.Add(strTemp);
			
			if(bNeed2ndLine)
			{
				strTemp.Format(_T("   %8s %10s %10s %10s  %s \n"), 
					_T(""), _T(""), _T(""), _T(""), str2);
				
				arWriteData.Add(strTemp);
			}
			
		}
	}
	
	// steel design
	if (nStlSize)
	{
		// 8 10 10 10 45
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_STEEL_TITLE)));
		arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD1)));
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD2)));
		
		CString str1, str2;
		for (int i=0; i<nStlSize; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, aStlLComK[i], LcomD)) ASSERT(0);
			
#if defined(_CIVIL)
			if (LcomD.nActive == 1) csActive = ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STRENGTH));
#else
			csActive = m_pDoc->m_pAttrCtrl->GetActivName4MDL(LcomD.nActive);
#endif
			
			str1 = str2 = _T("");
			BOOL bNeed2ndLine = _ns_devide(LcomD.Description, 45, str1, str2);
			
			strTemp.Format(_T("   %8d %10s %10s %10s  %s \n"), 
				LcomD.LoadCombId, _ns(LcomD.LoadCombName,10), aLcomType[LcomD.LoadCombType], 
				csActive, str1);
			arWriteData.Add(strTemp);
			
			if(bNeed2ndLine)
			{
				strTemp.Format(_T("   %8s %10s %10s %10s  %s \n"), 
					_T(""), _T(""), _T(""), _T(""), str2);
				arWriteData.Add(strTemp);
			}
		}
	}
	
	// concrete design
	if (nConSize)
	{
		// 8 10 10 10 45
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_CONCRETE_TITLE)));
		arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD1)));
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD2)));
		
		CString str1, str2;
		CString csActive;
		for (int i=0; i<nConSize; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, aConLcomK[i], LcomD)) ASSERT(0);
			
#if defined(_CIVIL)
			if (LcomD.nActive == 1) csActive = ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STRENGTH));
#else
			if (LcomD.nActive == 1) csActive = ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STRENGTH));
			else csActive = m_pDoc->m_pAttrCtrl->GetActivName4MDL(LcomD.nActive);
#endif
			
			str1 = str2 = _T("");
			BOOL bNeed2ndLine = _ns_devide(LcomD.Description, 45, str1, str2);
			
			strTemp.Format(_T("   %8d %10s %10s %10s  %s \n"), 
				LcomD.LoadCombId, _ns(LcomD.LoadCombName,10), aLcomType[LcomD.LoadCombType], 
				csActive, str1);
			arWriteData.Add(strTemp);
			
			if(bNeed2ndLine)
			{
				strTemp.Format(_T("   %8s %10s %10s %10s  %s \n"), 
					_T(""), _T(""), _T(""), _T(""), str2);
				arWriteData.Add(strTemp);
			}
		}
	}
	
	// SRC design
	if (nSRCSize)
	{
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_SRC_TITLE)));
		arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD1)));
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD2)));
		
		CString str1, str2;
		for (int i=0; i<nSRCSize; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SRC, aSRCLcomK[i], LcomD)) ASSERT(0);
			
#if defined(_CIVIL)
			if (LcomD.nActive == 1) csActive = ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STRENGTH));
#else
			csActive = m_pDoc->m_pAttrCtrl->GetActivName4MDL(LcomD.nActive);
#endif
			
			str1 = str2 = _T("");
			BOOL bNeed2ndLine = _ns_devide(LcomD.Description, 45, str1, str2);
			
			strTemp.Format(_T("   %8d %10s %10s %10s  %s \n"), 
				LcomD.LoadCombId, _ns(LcomD.LoadCombName,10), aLcomType[LcomD.LoadCombType], 
				csActive, str1);
			arWriteData.Add(strTemp);
			
			if(bNeed2ndLine)
			{
				strTemp.Format(_T("   %8s %10s %10s %10s  %s \n"), 
					_T(""), _T(""), _T(""), _T(""), str2);
				arWriteData.Add(strTemp);
			}
		}
	}
	
	// footing design
	if (nFdnSize)
	{
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_FOOT_TITLE)));
		arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD1)));
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD2)));
		
		CString str1, str2;
		for (int i=0; i<nFdnSize; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_FDN, aFdnLcomK[i], LcomD)) ASSERT(0);
			
#if defined(_CIVIL)
			if (LcomD.nActive == 1) csActive = ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STRENGTH));
#else
			csActive = m_pDoc->m_pAttrCtrl->GetActivName4MDL(LcomD.nActive);
#endif
			
			str1 = str2 = _T("");
			BOOL bNeed2ndLine = _ns_devide(LcomD.Description, 45, str1, str2);
			
			strTemp.Format(_T("   %8d %10s %10s %10s  %s \n"), 
				LcomD.LoadCombId, _ns(LcomD.LoadCombName,10), aLcomType[LcomD.LoadCombType], 
				csActive, str1);
			arWriteData.Add(strTemp);
			//  csActive, _ns(LcomD.Description,45));
			if(bNeed2ndLine)
			{
				strTemp.Format(_T("   %8s %10s %10s %10s  %s \n"), 
					_T(""), _T(""), _T(""), _T(""), str2);
				arWriteData.Add(strTemp);
			}
		}
	}

	// Aluminum design
	if (nAluSize)
	{
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_ALUMINUM_TITLE)));
		arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD1)));
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD2)));
		
		CString str1, str2;
		for (int i=0; i<nAluSize; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_ALUMINUM, aAluLcomK[i], LcomD)) ASSERT(0);
			
#if defined(_CIVIL)
			if (LcomD.nActive == 1) csActive = ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STRENGTH));
#else
			csActive = m_pDoc->m_pAttrCtrl->GetActivName4MDL(LcomD.nActive);
#endif
			
			str1 = str2 = _T("");
			BOOL bNeed2ndLine = _ns_devide(LcomD.Description, 45, str1, str2);
			
			strTemp.Format(_T("   %8d %10s %10s %10s  %s \n"), 
				LcomD.LoadCombId, _ns(LcomD.LoadCombName,10), aLcomType[LcomD.LoadCombType], 
				csActive, str1);
			arWriteData.Add(strTemp);
			//  csActive, _ns(LcomD.Description,45));
			if(bNeed2ndLine)
			{
				strTemp.Format(_T("   %8s %10s %10s %10s  %s \n"), 
					_T(""), _T(""), _T(""), _T(""), str2);
				arWriteData.Add(strTemp);
			}
		}
	}

	// Steel Composite design
	if (nScmSize)
	{
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_STLCOMP_TITLE)));
		arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD1)));
		arWriteData.Add(ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_LOAD_COMB_DATA_GENERAL_HEAD2)));
		
		CString str1, str2;
		for (int i=0; i<nScmSize; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, aScmLcomK[i], LcomD)) ASSERT(0);
			
#if defined(_CIVIL)
			if (LcomD.nActive == 1) csActive = ReplaceSymbolByTempChar(_LS(IDS_MAIN_MDL_STRENGTH));
#else
			csActive = m_pDoc->m_pAttrCtrl->GetActivName4MDL(LcomD.nActive);
#endif
			
			str1 = str2 = _T("");
			BOOL bNeed2ndLine = _ns_devide(LcomD.Description, 45, str1, str2);
			
			strTemp.Format(_T("   %8d %10s %10s %10s  %s \n"), 
				LcomD.LoadCombId, _ns(LcomD.LoadCombName,10), aLcomType[LcomD.LoadCombType], 
				csActive, str1);
			arWriteData.Add(strTemp);
			//  csActive, _ns(LcomD.Description,45));
			if(bNeed2ndLine)
			{
				strTemp.Format(_T("   %8s %10s %10s %10s  %s \n"), 
					_T(""), _T(""), _T(""), _T(""), str2);
				arWriteData.Add(strTemp);
			}
		}
	}
	
	arWriteData.Add(_T(" ")); // 한줄 내려쓰기!!.. 엔터키를 쳐서 한줄 내리기와 같다.
	return TRUE;
	
}

CString CReportTreeText::_ms(LPCTSTR lpStr)
{
	if (lpStr == 0) return CString(_T(""));
	
	CString csTemp = lpStr;
	BOOL bExistComma = (csTemp.Find(',') >= 0)? TRUE : FALSE;
	BOOL bExistSemiCol = (csTemp.Find(';') >= 0)? TRUE : FALSE;
	BOOL bExistQuote = (csTemp.Find('"') >= 0)? TRUE : FALSE;
	BOOL bExistBackSlash = (csTemp.Find('\\') >= 0)? TRUE : FALSE;

	CString csText;
	if (bExistBackSlash)
		csTemp.Replace(_T("\\"), _T("\\\\"));
	if (bExistQuote)
		csTemp.Replace(_T("\""), _T("\\\""));

	if (bExistComma || bExistSemiCol || bExistQuote || bExistBackSlash)
		csText.Format(_T("\"%s\""), csTemp);
	else return csTemp;

	return csText;
}

CString CReportTreeText::_ns(LPCTSTR lpStr, int n)
{
	if (lpStr == 0) return CString(_T(""));
	
	CString csText;
	csText = lpStr;
	if (csText.GetLength() > n)
	{
		csText = csText.Left(n-1);
		int nLen = csText.GetLength();
		if (nLen > 0)
		{
			if (_istleadbyte(csText.GetAt(nLen-1)) != 0)
				csText = csText.Left(nLen-1);
		}
		csText += _T("~");
	}
	return csText;
}

// Return TRUE: lpStr 길이가 n 보다 커서 나누어진 경우
BOOL CReportTreeText::_ns_devide(LPCTSTR lpStr, int n, CString& rStr1, CString& rStr2)
{
	rStr1 = _T("");
	rStr2 = _T("");
	if (lpStr == 0) return FALSE;
	if (n < 1) return FALSE;
	int nFirst = n;
	
	rStr1 = lpStr;
	if (rStr1.GetLength() > n)
	{
		// 구분문자를 발견할때까지 가서 나눈후 return TRUE
		while(TRUE)
		{
			char a = rStr1.GetAt(n-1);
			if(a==')' || a=='(' || a==' ' || a=='*' || a=='/' || a=='-' || a=='+')
			{
				rStr2 = rStr1.Mid(n);
				rStr1 = rStr1.Left(n);
				
				// 두번째 줄은 무조건 cut
				rStr2 = rStr2.Left(nFirst);
				
				return TRUE;
			}
			
			--n;
			if(n==0) break;
		}
		
		// 구분 문자를 발견 못한 경우
		rStr1 = rStr1.Left(nFirst-1);
		int nLen = rStr1.GetLength();
		if (nLen > 0)
		{
			if (_istleadbyte(rStr1.GetAt(nLen-1)) != 0)
				rStr1 = rStr1.Left(nLen-1);
		}
		rStr1 += _T("~");
		return FALSE;
	}
	else
	{
		// 크기가 n 보다 작은 경우는 바로 return
		return FALSE;
	}
}

// Line 끝에 ','+space+line continue sign 없이 list출력
// Mgt의 WriteList2와 달리 여기서는 ,가 없음
void CReportTreeText::WriteList2(CStringArray& arWriteData, CString &csPre, CString &csList, int nMaxLen, int nStartLen)
{
	ASSERT(nStartLen < nMaxLen);
	int nLen = csList.GetLength();
	int nStart = 0, nCount, nCount2;
	GetPartialList(csList, nMaxLen-nStartLen, nStart, nCount);
	
	CString strTemp=_T("");
	CString strTemp2=_T("");
	//if (nStart+nCount < nLen) nCount2 = nCount-2; // remove last ','+space
	if (nStart+nCount < nLen) nCount2 = nCount-1; // _tremove last space
	else nCount2 = nCount;
	strTemp.Format(_T("%s\n"), csList.Mid(nStart, nCount2));
	CString strList = csPre + _T(" ") + strTemp;
	arWriteData.Add(strList);
	
	nStart += nCount;
	while (nStart < nLen)
	{
		GetPartialList(csList, nMaxLen-nStartLen, nStart, nCount);
		//if (nStart+nCount < nLen) nCount2 = nCount-2; // remove last ','+space
		if (nStart+nCount < nLen) nCount2 = nCount-1; // _tremove last space
		else nCount2 = nCount;
		strTemp2.Format(_T("%*s%s\n"), nStartLen, _T(""), csList.Mid(nStart, nCount2));
		nStart += nCount;
		
		arWriteData.Add(strTemp2);
	}
	
	return;
}

void CReportTreeText::GetPartialList(CString &list, int nMaxLen, int nStart, int &nCount)
{
	int nPos, nPrevPos;
	nPos = list.Find(' ', nStart);
	if (nPos == -1) 
	{ 
		nCount = list.GetLength()-nStart; 
		return; 
	}
	nPrevPos = nPos;
	while (1)
	{
		nPos = list.Find(' ', nPrevPos+1);
		if (nPos == -1)
		{
			nPos = list.GetLength()-1;
			if (nPos - nStart + 1 <= nMaxLen) nPrevPos = nPos;
			break;
		}
		else if (nPos - nStart +1 > nMaxLen) break;
		nPrevPos = nPos;
	}
	nCount = nPrevPos - nStart + 1;
}

CString CReportTreeText::ReplaceSymbolByTempChar(LPCTSTR lpStr)
{
	CString strTemp = lpStr;
	//strTemp.Replace("\n", " "); 
	//   sParam.Replace("\\","_");// 
	//   sParam.Replace("*","_");// 
	//   sParam.Replace("?","_");// 
	//   sParam.Replace("|","_");// 
	//   sParam.Replace(":","_");// 
	//   sParam.Replace("/","_");// 
	//   sParam.Replace("<","_");// 
	//   sParam.Replace(">","_");// 
	return strTemp; 
}

CString CReportTreeText::ReplaceTempCharBySpace(LPCTSTR lpStr)
{
	CString strTemp = lpStr;
	strTemp.Replace(_T("_"), _T(" ")); 
	return strTemp; 
}

BOOL CReportTreeText::WriteBasicInfo_General(GenSegmentElements& genElements)
{
	CString strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_BasicInfo_General));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_BasicInfo_General);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_General);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_BasicInfo_General);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	
	return TRUE;
}

BOOL CReportTreeText::WriteBasicInfo_Standard(GenSegmentElements& genElements)
{
	CString strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_BasicInfo_Standard));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_BasicInfo_Standard);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Standard);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_BasicInfo_Standard);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	return TRUE;
}
CString CReportTreeText::GetChinaCodeName(const CString& strCode)
{
	CString strCodeLong = _T("");
	if(strCode == _T("JTG D60-2004"))
	{
		strCodeLong = _LS(IDS_AMR_CHRP_BasicInfo_Code_JTGD602004);
	}
	else if(strCode == _T("JTG D62-2004"))
	{
		strCodeLong = _LS(IDS_AMR_CHRP_BasicInfo_Code_JTGD622004);
	}
	else if(strCode == _T("JTG B01-2003"))
	{
		strCodeLong = _LS(IDS_AMR_CHRP_BasicInfo_Code_JTGB012003);
	}
	else if(strCode == _T("JTG D63-2007"))
	{
		strCodeLong = _LS(IDS_AMR_CHRP_BasicInfo_Code_JTGD632007);
	}
	else if(strCode == _T("JTG/T B02-01-2008"))
	{
		strCodeLong = _LS(IDS_AMR_CHRP_BasicInfo_Code_JTGTB02012008);
	}
	else if(strCode == _T("CJJ11-2011"))
	{
		strCodeLong = _LS(IDS_AMR_CHRP_BasicInfo_Code_CJJ112011);
	}
	else if(strCode == _T("GB50010-2010"))
	{
		strCodeLong = _LS(IDS_TM_CHRP_BasicInfo_Code_GB50010);
	}
	else if(strCode == _T("GB/T50011-2010"))
	{
		strCodeLong = _LS(IDS_TM_CHRP_BasicInfo_Code_GB50011);
	}
	else if(strCode == _T("JGJ3-2010"))
	{
		strCodeLong = _LS(IDS_TM_CHRP_BasicInfo_Code_JGJ3);
	}
	else if(strCode == _T("GB50009-2012"))
	{
		strCodeLong = _LS(IDS_TM_CHRP_BasicInfo_Code_GB50009);
	}
	else if(strCode == _T("JGJ138-2001"))
	{
		strCodeLong = _LS(IDS_TM_CHRP_BasicInfo_Code_JGJ138);
	}
	else if(strCode == _T("GB50017-2003"))
	{
		strCodeLong = _LS(IDS_TM_CHRP_BasicInfo_Code_GB50017);
	}
	else if(strCode == _T("JGJ99-98"))
	{
		strCodeLong = _LS(IDS_TM_CHRP_BasicInfo_Code_JGJ99);
	}
	else
	{
		strCodeLong = strCode;
	}
	return strCodeLong;
	
}
BOOL CReportTreeText::WriteBasicInfo_Code(GenSegmentElements& genElements )
{
	CString strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_BasicInfo_Code));
	strText += strTemp;
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_BasicInfo_Code);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Code);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_BasicInfo_Code);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	int iIndex = 1;
	strText = _T("");
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	int iDgnCode   = PscdD.iDgnCode;  // 1:JSCE02, 2:KSCE-USD03, 3:JTG D62-04
	CString strCodeName=_T("");
	strCodeName = CDBLib::GetPscCodeName(iDgnCode);

#if defined(_CIVIL)
	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("JTG D62-2004")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;
	
	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("JTG D60-2004")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;
	
	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("JTG B01-2003")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;
	
	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("JTG/T B02-01-2008")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;
	
	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("JTG D63-2007")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;
	
	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("CJJ11-2011")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;
 #else
	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("GB50010-2010")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;

	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("GB/T50011-2010")) + _T("\n"), iIndex);
	strText += strTemp;
	iIndex++;

	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("JGJ3-2010")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;

	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("GB50009-2012")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;

	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("JGJ138-2001")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;

	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("GB50017-2003")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;

	strTemp.Format(_T("   %d)") + GetChinaCodeName(_T("JGJ99-98")) + _T("\n"),iIndex);
	strText += strTemp;
	iIndex++;
#endif
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Code);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_BasicInfo_Code);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	return TRUE;
	
}

BOOL CReportTreeText::WriteBasicInfo_Structure(GenSegmentElements& genElements )
{
	CString strText = _LS(IDS_AMR_CHRP_BasicInfo_Structure);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_BasicInfo_Structure));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_BasicInfo_Structure);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Structure);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_BasicInfo_Structure);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	return TRUE;
	
}
BOOL CReportTreeText::Get_MatlOfConcrete(CString CodeMatlName, double& dfck, double& dftk, double& dfcd, double& dftd)
{
	// Always Code Units (N,mm).
	dfck=0.0;
	dftk=0.0;
	dfcd=0.0;
	dftd=0.0;
	
	if(CodeMatlName==_T("C15"))	{dfck=10.0; dftk=1.27; dfcd= 6.9; dftd=0.88;}
	else if(CodeMatlName==_T("C20"))	{dfck=13.4; dftk=1.54; dfcd= 9.2; dftd=1.06;}
	else if(CodeMatlName==_T("C25"))	{dfck=16.7; dftk=1.78; dfcd=11.5; dftd=1.23;}
	else if(CodeMatlName==_T("C30"))	{dfck=20.1; dftk=2.01; dfcd=13.8; dftd=1.39;}
	else if(CodeMatlName==_T("C35"))	{dfck=23.4; dftk=2.20; dfcd=16.1; dftd=1.52;}
	else if(CodeMatlName==_T("C40"))	{dfck=26.8; dftk=2.40; dfcd=18.4; dftd=1.65;}
	else if(CodeMatlName==_T("C45"))	{dfck=29.6; dftk=2.51; dfcd=20.5; dftd=1.74;}
	else if(CodeMatlName==_T("C50"))	{dfck=32.4; dftk=2.65; dfcd=22.4; dftd=1.83;}
	else if(CodeMatlName==_T("C55"))	{dfck=35.5; dftk=2.74; dfcd=24.4; dftd=1.89;}
	else if(CodeMatlName==_T("C60"))	{dfck=38.5; dftk=2.85; dfcd=26.5; dftd=1.96;}
	else if(CodeMatlName==_T("C65"))	{dfck=41.5; dftk=2.93; dfcd=28.5; dftd=2.02;}
	else if(CodeMatlName==_T("C70"))	{dfck=44.5; dftk=3.00; dfcd=30.5; dftd=2.07;}
	else if(CodeMatlName==_T("C75"))	{dfck=47.4; dftk=3.05; dfcd=32.4; dftd=2.10;}
	else if(CodeMatlName==_T("C80"))	{dfck=50.2; dftk=3.10; dfcd=34.6; dftd=2.14;}
	else	{ASSERT(0);	return FALSE;}
	
	return TRUE;
}
BOOL CReportTreeText::Get_MatlOfReBar(CString strRbarNa, double& dfsk, double& dfsd, double& dfsd1, double& dEs,double& dr)
{
	// Always Code Units (N,mm).
	dfsk=0.0;
	dfsd=0.0;
	dfsd1=0.0;
	dEs=0.0;
	dr = 76.98;//kn/m3
	
	if(strRbarNa==_T("R235"))	{dfsk=235.0; dfsd=195.0; dfsd1=195.0; dEs=210000.0;}
	else if(strRbarNa==_T("HRB335")) {dfsk=335.0; dfsd=280.0; dfsd1=280.0; dEs=200000.0;}
	else if(strRbarNa==_T("HRB400")) {dfsk=400.0; dfsd=330.0; dfsd1=330.0; dEs=200000.0;}
	else if(strRbarNa==_T("KL400"))	 {dfsk=400.0; dfsd=330.0; dfsd1=330.0; dEs=200000.0;}
	else {ASSERT(0);	return FALSE;}
	return TRUE;
}

double CReportTreeText::GetLengthUnitRatio(double dval)
{
	double dratio = CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH,  1.0);
	dval = dval/dratio;
	return dval;
}
double CReportTreeText::GetForceUnitRatio(double dval)
{
	double dratio =  CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_FORCE,  1.0);
	dval = dval/dratio;
	return dval;
}
double CReportTreeText::GetStressUnitRatio(double dval)
{
	double dratio =  CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS,  1.0);
	dval =  dval/dratio;
	return dval;
}
double CReportTreeText::GetMomentUnitRatio(double dval)
{
	double dratio =  CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_MOMENT,  1.0);
	dval = dval/dratio;
	return dval;
}
double CReportTreeText::GetAreaUnitRatio(double dval)
{
	double dratio =  CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_AREA,  1.0);
	dval = dval/dratio;
	return dval;
}
double CReportTreeText::GetDensityUnitRatio(double dval)
{
	double dratio =  CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY,  1.0);
	dval = dval/dratio;
	return dval;
}


BOOL CReportTreeText::WriteBasicInfo_Material(GenSegmentElements& genElements)
{
	CString strText = _LS(IDS_AMR_CHRP_BasicInfo_Material);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_BasicInfo_Material));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_BasicInfo_Material);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Material);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_BasicInfo_Material);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	
	
	CArray<T_MATL_K, T_MATL_K> rMatlKeyList;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(rMatlKeyList);
	
	CArray<T_MATL_K, T_MATL_K> rConMatlKeyList;
	//CArray<T_MATL_K, T_MATL_K> rSteelMatlKeyList;
	
	m_pDoc->m_pAttrCtrl->GetMatlKeyListUsedInElem(rConMatlKeyList);
	//m_pDoc->m_pAttrCtrl->GetMatlKeyListUsedInTdnt(rSteelMatlKeyList);
	
	T_MATD_D rMatlData;
	rMatlData.Initialize();
	
	int iIndex = 0;  //살퀸皐
	int iTitle = 0;
	
	
	
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 8;
	TableD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_StrengthGrade));
	TableD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_ElasticModulus));
	TableD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_Density));
	TableD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_LinearExpansionFactor));
	TableD.HeaderD.aTitleName.Add(_T("[DRG_MATH]|EQ|Sub1(f, ck)(MPa)[/DRG_MATH]"));
	TableD.HeaderD.aTitleName.Add(_T("[DRG_MATH]|EQ|Sub1(f, tk)(MPa)[/DRG_MATH]"));
	TableD.HeaderD.aTitleName.Add(_T("[DRG_MATH]|EQ|Sub1(f, cd)(MPa)[/DRG_MATH]"));
	TableD.HeaderD.aTitleName.Add(_T("[DRG_MATH]|EQ|Sub1(f, td)(MPa)[/DRG_MATH]"));
	//TableD.HeaderD.aTitleName.Add(_T("[DRG_MATH]|EQ|Sup1(f,')Sub1(,sd)[/DRG_MATH]"));
	
	TableD.RecordD.aRecordData.SetSize(rConMatlKeyList.GetSize());
	
	iIndex = 0;
	for (int i=0;i<rConMatlKeyList.GetSize();i++)
	{
		int nID = rConMatlKeyList.GetAt(i);
		if(m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,rMatlData))
		{
			if(rMatlData.Type == _T("C"))
			{
				double dfck;
				double dftk;
				double dfcd;
				double dftd;
				if(!Get_MatlOfConcrete(rMatlData.Data1.CodeMatlName, dfck,dftk,dfcd,dftd))
					continue;
				
				strTemp.Format(_T("%s"),rMatlData.Data1.CodeMatlName);
				TableD.RecordD.aRecordData[iIndex].Add(strTemp);
				
				
				strTemp.Format(_T("%0.0f"),GetStressUnitRatio(rMatlData.Data1.Analysis.Elast));
				TableD.RecordD.aRecordData[iIndex].Add(strTemp);
				
				
				strTemp.Format(_T("%0.2f"),GetDensityUnitRatio(rMatlData.Data1.Analysis.Density));
				TableD.RecordD.aRecordData[i].Add(strTemp);
				
				
				strTemp.Format(_T("%0.6f"),rMatlData.Data1.Analysis.Thermal);
				TableD.RecordD.aRecordData[iIndex].Add(strTemp);
				
				
				strTemp.Format(_T("%0.2f"),(dfck));
				TableD.RecordD.aRecordData[iIndex].Add(strTemp);
				
				
				strTemp.Format(_T("%0.2f"),(dftk));
				TableD.RecordD.aRecordData[iIndex].Add(strTemp);
				
				
				strTemp.Format(_T("%0.2f"),(dfcd));
				TableD.RecordD.aRecordData[iIndex].Add(strTemp);
				
				
				strTemp.Format(_T("%0.2f"),(dftd));
				TableD.RecordD.aRecordData[iIndex].Add(strTemp);
				
				iIndex++;
				
			}
			
		}
	}
	
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = iIndex;
	TableD.RecordD.nRecordColCount = 8;
	
	//add con table
	if(TableD.RecordD.nRecordRowCount > 0)
	{
		iTitle++;
		CString strTitle = _T("");
		strTitle.Format(_T("    %d)"),iTitle);
		//깃痙
		CString strSegTitle = _LS(IDS_AMR_CHRP_BasicInfo_Material_Concrete);
		genFtagStr.sContents     =  strTitle + strSegTitle + _T("\n");
		strSegTitle += _LS(IDS_AMR_CHRP_BasicInfo_Material);
		genFtagStr.sCaption = strSegTitle;
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = strSegTitle;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		//깊목
		//genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Material);
		genFtagStr.sCaption= _LS(IDS_AMR_CHRP_BasicInfo_Material_ConcreteTable);
		genFtagStr.sType         = _LSX(TBL);
		
		CReportTableProcess ReportTable;
		ReportTable.SetData(TableD);
		ReportTable.SetFilePathName(ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml"));
		ReportTable.GenerateReport();
		
		
		
		genFtagStr.sFullPath     = ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml");
		genFtagStr.sExtInfo      = genFtagStr.sCaption ;
		genFtagStr.sContents     = _T(" ");
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		
	}
	
	
	
	//steel
	T_TABLE_D TableSteelD;
	TableSteelD.Initialize();
	TableSteelD.HeaderD.nHeaderRowCount = 1;
	TableSteelD.HeaderD.nHeaderColCount = 6;
	TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_Rebar));
	TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_ElasticModulus));
	TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_Density));
	TableSteelD.HeaderD.aTitleName.Add(_T("[DRG_MATH]|EQ|Sub1(f, sk)(MPa)[/DRG_MATH]"));
	TableSteelD.HeaderD.aTitleName.Add(_T("[DRG_MATH]|EQ|Sub1(f, sd)(MPa)[/DRG_MATH]"));
	TableSteelD.HeaderD.aTitleName.Add(_T("[DRG_MATH]|EQ|Sup1(f,')Sub1(,sd)(MPa)[/DRG_MATH]"));
	
	TableSteelD.RecordD.aRecordData.SetSize(4);
	CString strRebar[] = {_T("R235"),_T("HRB335"),_T("HRB400"),_T("KL400")};
	iIndex = 0;
	
	for(int i=0;i<4;i++)
	{
		double dfsk;
		double dfsd;
		double dfsd1;
		double dEs;
		double dr;
		if(!Get_MatlOfReBar(strRebar[i],dfsk,dfsd,dfsd1,dEs,dr))
			continue;
		strTemp.Format(_T("%s"),strRebar[i]);
		TableSteelD.RecordD.aRecordData[iIndex].Add(strTemp);
		strTemp.Format(_T("%0.0f"),dEs);
		TableSteelD.RecordD.aRecordData[iIndex].Add(strTemp);
		strTemp.Format(_T("%0.2f"),dr);
		TableSteelD.RecordD.aRecordData[i].Add(strTemp);
		strTemp.Format(_T("%0.0f"),(dfsk));
		TableSteelD.RecordD.aRecordData[iIndex].Add(strTemp);
		strTemp.Format(_T("%0.0f"),(dfsd));
		TableSteelD.RecordD.aRecordData[iIndex].Add(strTemp);
		strTemp.Format(_T("%0.0f"),(dfsd1));
		TableSteelD.RecordD.aRecordData[iIndex].Add(strTemp); 
		iIndex++;
		
	}
	
	
	
	
	
	TableSteelD.RecordD.aRecordData.FreeExtra();
	TableSteelD.RecordD.nRecordRowCount = iIndex;
	TableSteelD.RecordD.nRecordColCount = 6;
	
	//add con table
	if(TableSteelD.RecordD.nRecordRowCount > 0)
	{
		iTitle++;
		CString strTitle = _T("");
		strTitle.Format(_T("    %d)"),iTitle);
		
		//깃痙
		CString strSegTitle = _LS(IDS_AMR_CHRP_BasicInfo_Material_Rebar);
		genFtagStr.sContents     = strTitle + strSegTitle + _T("\n");
		strSegTitle += _LS(IDS_AMR_CHRP_BasicInfo_Material);
		genFtagStr.sCaption = strSegTitle;
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = strSegTitle;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		//깊목
		//genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Material);
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Material_RebarTable);
		
		genFtagStr.sType         = _LSX(TBL);
		
		CReportTableProcess ReportTable;
		ReportTable.SetData(TableSteelD);
		ReportTable.SetFilePathName(ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml"));
		ReportTable.GenerateReport();
		
		
		
		genFtagStr.sFullPath     = ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = _T(" ");
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		
	}
	
	
	//tendon
	T_TABLE_D TableTendonD;
	TableTendonD.Initialize();
	TableTendonD.HeaderD.nHeaderRowCount = 1;
	TableTendonD.HeaderD.nHeaderColCount = 7;
	TableTendonD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_TendonName));
	TableTendonD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_ElasticModulus));
	TableTendonD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_YieldStrength));
	TableTendonD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_FrictionFactor));
	TableTendonD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_WobbleFactor));
	TableTendonD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_JTG04Factor));
	TableTendonD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_BasicInfo_Material_AnchoSlipBegin));
	
	
	TableTendonD.RecordD.aRecordData.SetSize(m_pDoc->m_pAttrCtrl->GetCountTdnt());
	
	iIndex = 0;
	
	T_TDNT_K Key;
	T_TDNT_D Data;
	POSITION pos=m_pDoc->m_pAttrCtrl->GetStartTdnt();
	while(pos != NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextTdnt(pos, Key, Data);
		strTemp.Format(_T("%s"),Data.TendonTypeName);
		TableTendonD.RecordD.aRecordData[iIndex].Add(strTemp);
		
		T_MATL_D rSteelData;
		if(!m_pDoc->m_pAttrCtrl->GetMatl(Data.MatlKey,rSteelData))
			continue;
		strTemp.Format(_T("%0.0f"),GetStressUnitRatio(rSteelData.Data1.Analysis.Elast));
		TableTendonD.RecordD.aRecordData[iIndex].Add(strTemp);

	 
			// Get Matl Data.
			T_MATD_D TndtMatdD;
			TndtMatdD.Initialize();
			double dzlyl = 0.75*Data.dFpk;
			if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(Data.MatlKey,TndtMatdD))	{ASSERT(0);}
			if(TndtMatdD.Data1.CodeName != MATLCODE_STL_JTG04)	{ASSERT(0); }
			int iRelxType = 0;
			if(TndtMatdD.Data1.CodeMatlName.Find(_T("Strand"))>=0)		iRelxType = 0;
			else if(TndtMatdD.Data1.CodeMatlName.Find(_T("Wire"))>=0)			iRelxType = 1;
			else if(TndtMatdD.Data1.CodeMatlName.Find(_T("Steelbar"))>=0)	iRelxType = 2;
			else	ASSERT(0);
	
		
		if(iRelxType <= 1)
		{
			dzlyl = Data.bOverStressRedFactor ? 0.8*Data.dFpk : 0.75*Data.dFpk;
		}
		else
		{
			dzlyl = Data.bOverStressRedFactor ? 0.95*Data.dFpk : 0.9*Data.dFpk;
		}

		strTemp.Format(_T("%0.0f"),GetStressUnitRatio(dzlyl));
		TableTendonD.RecordD.aRecordData[iIndex].Add(strTemp);
		
		
		strTemp.Format(_T("%0.3f"),Data.dFrictionFactor);
		TableTendonD.RecordD.aRecordData[iIndex].Add(strTemp);
		
		
		strTemp.Format(_T("%0.5f"),Data.dWobbleFactor);
		TableTendonD.RecordD.aRecordData[iIndex].Add(strTemp);
		
		
		strTemp.Format(_T("%0.1f"),Data.dJTG04Factor);
		TableTendonD.RecordD.aRecordData[iIndex].Add(strTemp);
		
		strTemp.Format(_T("%0.5f"),GetLengthUnitRatio(Data.dAnchoSlipBegin));
		TableTendonD.RecordD.aRecordData[iIndex].Add(strTemp);
		
		iIndex++;
	}
	
	
	
	TableTendonD.RecordD.aRecordData.FreeExtra();
	TableTendonD.RecordD.nRecordRowCount = iIndex;
	TableTendonD.RecordD.nRecordColCount = 7;
	
	//add con table
	if(TableTendonD.RecordD.nRecordRowCount > 0)
	{
		iTitle++;
		CString strTitle = _T("");
		strTitle.Format(_T("    %d)"),iTitle);
		
		//깃痙
		CString strSegTitle = _LS(IDS_AMR_CHRP_BasicInfo_Material_Tendon);
		genFtagStr.sContents     = strTitle + strSegTitle + _T("\n");
		strSegTitle += _LS(IDS_AMR_CHRP_BasicInfo_Material);
		genFtagStr.sCaption = strSegTitle;
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		//깊목
		//genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Material);
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Material_TendonTable);
		
		genFtagStr.sType         = _LSX(TBL);
		
		CReportTableProcess ReportTable;
		ReportTable.SetData(TableTendonD);
		ReportTable.SetFilePathName(ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml"));
		ReportTable.GenerateReport();
		
		
		
		genFtagStr.sFullPath     = ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml");
		genFtagStr.sExtInfo      = genFtagStr.sCaption ;
		genFtagStr.sContents     = _T(" ");
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		
	}
	/*
	//steel box beam
	T_TABLE_D TableSteelBoxBeamD;
	TableSteelBoxBeamD.Initialize();
	TableSteelBoxBeamD.HeaderD.nHeaderRowCount = 1;
	TableSteelBoxBeamD.HeaderD.nHeaderColCount = 7;
	TableSteelBoxBeamD.HeaderD.aTitleName.Add(_T("멀芎꼼죕"));
	TableSteelBoxBeamD.HeaderD.aTitleName.Add(_T("뎐昑친좆)"));
	TableSteelBoxBeamD.HeaderD.aTitleName.Add(_T("숏학친좆"));
	TableSteelBoxBeamD.HeaderD.aTitleName.Add(_T("휭路"));
	TableSteelBoxBeamD.HeaderD.aTitleName.Add(_T("菉蕨壇제"));
	TableSteelBoxBeamD.HeaderD.aTitleName.Add(_T("琨혓壇제"));
	TableSteelBoxBeamD.HeaderD.aTitleName.Add(_T("숏壇제"));
	
		TableSteelBoxBeamD.RecordD.aRecordData.SetSize(rSteelMatlKeyList.GetSize());
		
			iIndex = 0;
			
				for(int i=0;i<rSteelMatlKeyList.GetSize();i++)
				{
				if(m_pDoc->m_pAttrCtrl->GetMatlDesign(rSteelMatlKeyList.GetAt(i),rMatlData))
				{
				if(rMatlData.Type == _T("S"))
				{
				strTemp.Format(_T("%s"),rMatlData.Data1.CodeMatlName);
				TableSteelBoxBeamD.RecordD.aRecordData[iIndex].Add(strTemp);
				
					
						strTemp.Format(_T("%0.0f"),rMatlData.Data1.Analysis.Elast);
						TableSteelBoxBeamD.RecordD.aRecordData[iIndex].Add(strTemp);
						
							
								strTemp.Format(_T("%0.0f"),rMatlData.Data1.Analysis.Density);
								TableSteelBoxBeamD.RecordD.aRecordData[i].Add(strTemp);
								
									
										strTemp.Format(_T("%0.0f"),rMatlData.Data1.Design.C_fc);
										TableSteelBoxBeamD.RecordD.aRecordData[iIndex].Add(strTemp);
										
											
												strTemp.Format(_T("%0.0f"),rMatlData.Data1.Design.C_fc);
												TableSteelBoxBeamD.RecordD.aRecordData[iIndex].Add(strTemp);
												
													
														strTemp.Format(_T("%0.0f"),rMatlData.Data1.Design.C_fc);
														TableSteelBoxBeamD.RecordD.aRecordData[iIndex].Add(strTemp);
														
															iIndex++;
															}
															}
															}
															
																TableSteelBoxBeamD.RecordD.aRecordData.FreeExtra();
																TableSteelBoxBeamD.RecordD.nRecordRowCount = iIndex;
																TableSteelBoxBeamD.RecordD.nRecordColCount = 7;
																
																	//add con table
																	if(TableSteelBoxBeamD.RecordD.nRecordRowCount > 0)
																	{
																	iTitle++;
																	CString strTitle = _T("");
																	strTitle.Format(_T("    %d)"),iTitle);
																	
																		//깃痙
																		CString strSegTitle = _T("멀芎졺꼼죕");
																		genFtagStr.sContents     = strTitle + strSegTitle + _T("\n");
																		strSegTitle += _LS(IDS_AMR_CHRP_BasicInfo_Material);
																		genFtagStr.sCaption = strSegTitle;
																		genFtagStr.sType         = _LSX(TXT);
																		genFtagStr.sFullPath     = _T(" ");
																		genFtagStr.sExtInfo      = genFtagStr.sCaption;
																		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
																		genFtagStr.sExt          = _T(" ");
																		genFtagStr.iLevel        = 0;
																		
																			genElements.m_SegmentElements.push_back(genFtagStr);
																			
																				//깊목
																				genFtagStr.sCaption = genFtagStr.sCaption + _T("멀芎졺꼼죕깊목");
																				genFtagStr.sType         = _LSX(TBL);
																				
																					CReportTableProcess ReportTable;
																					ReportTable.SetData(TableSteelBoxBeamD);
																					ReportTable.SetFilePathName(ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml"));
																					ReportTable.GenerateReport();
																					
																						
																							
																								genFtagStr.sFullPath     = ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml");
																								genFtagStr.sExtInfo      = genFtagStr.sCaption ;
																								genFtagStr.sContents     = _T(" ");
																								genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
																								genFtagStr.sExt          = _T(" ");
																								genFtagStr.iLevel        = 0;
																								
																									genElements.m_SegmentElements.push_back(genFtagStr);
																									
																										
																											}
																											*/
return TRUE;

}

BOOL CReportTreeText::WriteBasicInfo_Calculation(GenSegmentElements& genElements )
{
	CString strText = _LS(IDS_AMR_CHRP_BasicInfo_Calculation);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_BasicInfo_Calculation));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_BasicInfo_Calculation);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Calculation);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_BasicInfo_Calculation);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	T_PSCD_D PscdD;
	PscdD.Initialize();
	CString strBrgType = _T("");
	if(m_pDoc->IsPscJtg04())
	{
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		if(PscdD.iBrgType == 0)
		{
			strBrgType = _LS(IDS_AMR_CHRP_BasicInfo_Calculation_AllPreStress);
		}
		else if(PscdD.iBrgType == 1)
		{
			strBrgType = _LS(IDS_AMR_CHRP_BasicInfo_Calculation_APreStress);
		}
		else
		{
			strBrgType = _LS(IDS_AMR_CHRP_BasicInfo_Calculation_BPreStress);
		}
	}
	else
		return TRUE;
	
	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_BasicInfo_Calculation_Explanation)
		, GetChinaCodeName(_T("JTG D60-2004")),GetChinaCodeName(_T("JTG D62-2004")),strBrgType);
	strText += strTemp;
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_BasicInfo_Calculation);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Explanation);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	
	
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteModel_Calculation     (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_Model_Calculation);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_Model_Calculation));
	strText += strTemp;
	
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Model_Calculation);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Calculation);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_Model_Calculation);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Calculation_ModelPicture);
	genFtagStr.sType         = _LSX(IMG);
	genFtagStr.sFullPath     = GetModelPictureByView();
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = genFtagStr.sCaption;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	int iIndex = 0 ;
	if(m_pDoc->m_pAttrCtrl->GetCountNode() > 0)
	{
		iIndex++;
		strText = _T("");
		strTemp.Format(_T("    %d)%s：%d ；\n"), iIndex,_LS(IDS_AMR_CHRP_Model_Calculation_Node),m_pDoc->m_pAttrCtrl->GetCountNode());
		strText += strTemp;
		
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Calculation);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP_Model_Calculation_Node);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
	}
	
	if(m_pDoc->m_pAttrCtrl->GetCountElem() > 0)
	{
		iIndex++;
		
		strText = _T("");
		
		strTemp.Format(_T("    %d)%s：%d ；\n"), iIndex,_LS(IDS_AMR_CHRP_Model_Calculation_Elem),m_pDoc->m_pAttrCtrl->GetCountElem());
		strText += strTemp;
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Calculation);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP_Model_Calculation_Elem);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
	}
	
	if(m_pDoc->m_pAttrCtrl->GetCountCons() > 0)
	{
		iIndex++;
		strText = _T("");
		
		strTemp.Format(_T("    %d)%s：%d ；\n"), iIndex,_LS(IDS_AMR_CHRP_Model_Calculation_Cons),m_pDoc->m_pAttrCtrl->GetCountCons());
		strText += strTemp;
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Calculation);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP_Model_Calculation_Cons);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
	}
	
	if(m_pDoc->m_pAttrCtrl->GetCountStag() > 0)
	{
		iIndex++;
		strTemp.Format(_T("    %d)"), iIndex);
		
		strText = strTemp;
		
		strTemp.Format(_LS(IDS_AMR_CHRP_Model_Calculation_StagExplanation), m_pDoc->m_pAttrCtrl->GetCountStag());
		strText += strTemp;
		CArray<T_STAG_K, T_STAG_K> rKeyList;
		m_pDoc->m_pAttrCtrl->GetStagKeyList(rKeyList);
		T_STAG_D stage ;
		
		for (int iStep=0;iStep<rKeyList.GetSize();iStep++)
		{
			stage.Initialize();
			if(m_pDoc->m_pAttrCtrl->GetStag(rKeyList.GetAt(iStep),stage))
			{
				//strTemp.Format(_LS(IDS_AMR_CHRP_Model_Calculation_AllStags), rKeyList.GetAt(iStep),stage.StageName,stage.dDuration);
				strTemp.Format(_LS(IDS_AMR_CHRP_Model_Calculation_AllStags), stage.nStageId,stage.StageName,stage.dDuration);
				strText += strTemp;
				
			}
		}
		
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Calculation);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP_Model_Calculation_Stag);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
	}
	return TRUE;
	
}
BOOL    CReportTreeText::WriteModel_Steel           (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_Model_Steel);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_Model_Steel));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Model_Steel);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Steel);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_Model_Steel);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	return TRUE;
	
}
BOOL    CReportTreeText::WriteModel_Section         (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_Model_Section);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_Model_Section));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Model_Section);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Section);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_Model_Section);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	int iIndex = 0;
	CArray<int,int> standradSectKeys;
	CChinaReportSetting::GetChinaReportSetting()->GetStandradSectKey(standradSectKeys);
	
	T_SECT_D sectData;
	sectData.Initialize();
	if(standradSectKeys.GetSize() > 0)
	{
		iIndex++;
		strText.Format(_T("    %d)") ,iIndex)  ;
		strText = strText + _LS(IDS_AMR_CHRP_Model_Section_Sect);
		strText += _T("\n");
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Section);
		genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Section_Sect);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		for (int iSect = 0 ; iSect < standradSectKeys.GetSize();iSect++)
		{
			
			if(m_pDoc->m_pAttrCtrl->GetSect(standradSectKeys.GetAt(iSect),sectData))
				InsertSSTable(standradSectKeys.GetAt(iSect),genElements);
		}
	}
	
	
	if(m_pDoc->m_pAttrCtrl->GetCountEwsf() > 0)
	{
		iIndex++;
		strText.Format(_T("    %d)") ,iIndex)  ;
		strText = strText + _LS(IDS_AMR_CHRP_Model_Section_Ewsf);
		strText += _T("\n");
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Section);
		genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Section_Ewsf);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		T_EWSF_K rKey ;
		T_EWSF_D rData ;
		POSITION Pos;
		Pos = m_pDoc->m_pAttrCtrl->GetStartEwsf();
		
		T_TABLE_D TableSteelD;
		TableSteelD.Initialize();
		TableSteelD.HeaderD.nHeaderRowCount = 1;
		TableSteelD.HeaderD.nHeaderColCount = 7;
		TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Section_Ewsf_Elem));
		TableSteelD.HeaderD.aTitleName.Add(_T("Iy(I)"));
		TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Section_Ewsf_ZUp_I));
		TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Section_Ewsf_ZUp_J));
		TableSteelD.HeaderD.aTitleName.Add(_T("Iy(J)"));
		TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Section_Ewsf_ZDown_I));
		TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Section_Ewsf_ZDown_J));
		
		
		int iIndexLine = 0;
		
		TableSteelD.RecordD.nRecordColCount = 7;
		TableSteelD.RecordD.aRecordData.SetSize(m_pDoc->m_pAttrCtrl->GetCountEwsf());
		TableSteelD.RecordD.nRecordRowCount = m_pDoc->m_pAttrCtrl->GetCountEwsf();
		
		int j=0;
		while(Pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextEwsf(Pos,rKey,rData);
			
			
			strTemp.Format(_T("%u"),rKey);
			TableSteelD.RecordD.aRecordData[j].Add(strTemp);
			
			strTemp.Format(_T("%0.3f"),rData.EwsfSubY.dScaleFactor);
			TableSteelD.RecordD.aRecordData[j].Add(strTemp);
			
			strTemp.Format(_T("%0.3f"),rData.EwsfSubY.dZtopFactor);
			TableSteelD.RecordD.aRecordData[j].Add(strTemp);
			
			strTemp.Format(_T("%0.3f"),rData.EwsfSubY.dZbotFactor);
			TableSteelD.RecordD.aRecordData[j].Add(strTemp);
			
			strTemp.Format(_T("%0.3f"),rData.EwsfSubY.dScaleFactor_j);
			TableSteelD.RecordD.aRecordData[j].Add(strTemp);
			
			strTemp.Format(_T("%0.3f"),rData.EwsfSubY.dZtopFactor_j);
			TableSteelD.RecordD.aRecordData[j].Add(strTemp);
			
			strTemp.Format(_T("%0.3f"),rData.EwsfSubY.dZbotFactor_j);
			TableSteelD.RecordD.aRecordData[j].Add(strTemp);
			
			j++;
			
		}
		
		CReportTableProcess ReportTable;
		ReportTable.SetData(TableSteelD);
		CString strTitle = _LS(IDS_AMR_CHRP_Model_Section_Ewsf);
		ReportTable.SetFilePathName(ReportTable.GetFilePathName() + strTitle + _T(".xml"));
		ReportTable.GenerateReport();
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Section);
		genFtagStr.sCaption = genFtagStr.sCaption + strTitle;
		genFtagStr.sType         = _LSX(TBL);
		genFtagStr.sFullPath     = ReportTable.GetFilePathName() + strTitle + _T(".xml");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = _T(" ");
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		
		
		
	}
	
	
	return TRUE;
	
}

BOOL    CReportTreeText::WriteModel_Load            (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_Model_Load);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_Model_Load));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Model_Load);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_Model_Load);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	int iIndex = 0;
	if(m_pDoc->m_pAttrCtrl->GetCountBodf()>0)
	{
		iIndex++;
		strText.Format(_T("    %d)") ,iIndex)  ;
		strText = strText + _LS(IDS_AMR_CHRP_Model_Load_Bodf);
		strText += _T("\n");
		T_BODF_K rKey ;
		T_BODF_D rData ;
		POSITION Pos;
		Pos = m_pDoc->m_pAttrCtrl->GetStartBodf();
		while(Pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextBodf(Pos,rKey,rData);
			strTemp.Format(_T("        %s：%0.2f\n"),_LS(IDS_AMR_CHRP_Model_Load_BodfFactor),rData.SZ);
			strText += strTemp;
			
		}
		
		
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
		genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Load_Bodf);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
	}
	
	if(m_pDoc->m_pAttrCtrl->GetCountStmp()>0)
	{
		
		iIndex++;
		strText.Format(_T("    %d)") ,iIndex)  ;
		strText = strText + _LS(IDS_AMR_CHRP_Model_Load_Stmp);
		strText += _T("\n");
		T_STMP_K rKey ;
		T_STMP_D rData ;
		POSITION Pos;
		Pos = m_pDoc->m_pAttrCtrl->GetStartStmp();
		while(Pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextStmp(Pos,rKey,rData);
			T_STLD_D LCaseData;
			if(m_pDoc->m_pAttrCtrl->GetStld(rData.LoadCaseKey,LCaseData))
			{
				strTemp.Format(_T("        %s：%0.2f（%s）\n"),LCaseData.LoadCaseName,rData.dblSysTemp,m_pDoc->m_pUnitCtrl->m_strTemper);
				strText += strTemp;
			}
			
			
		}
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
		genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Load_Stmp);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
	}
	CArray<int,int> standradSectKeys;
	CArray<T_ELEM_K,T_ELEM_K> standradElemKeys;
	CArray<T_SECT_K,T_SECT_K> TempSectKey;
	CArray<T_ELEM_K,T_ELEM_K> TempElemKey;
	
	CChinaReportSetting::GetChinaReportSetting()->GetStandradSectKey(standradSectKeys);
	
	for (int j=0;j<standradSectKeys.GetSize();j++)
	{
		TempSectKey.RemoveAll();
		TempElemKey.RemoveAll();
		TempSectKey.Add(standradSectKeys.GetAt(j));
		m_pDoc->m_pAttrCtrl->GetKeyListSectUser(&TempSectKey,NULL,&TempElemKey);
		if(TempElemKey.GetSize() > 0)
			standradElemKeys.Add(TempElemKey.GetAt(0));
	}
	if(standradSectKeys.GetSize() > 0 && m_pDoc->m_pAttrCtrl->GetCountBtmp() > 0)
	{
		iIndex++;
		strText.Format(_T("    %d)") ,iIndex)  ;
		strText = strText + _LS(IDS_AMR_CHRP_Model_Load_Btmp);
		strText += _T("\n");
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
		genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Load_Btmp);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		CString strElemName = _T("");
		for (int iElem=0;iElem<standradElemKeys.GetSize();iElem++)
		{
			T_ELEM_K standradElemKey = standradElemKeys.GetAt(iElem);
			strElemName.Format(_T("（%s%u）"),_LS(IDS_AMR_CHRP_Graphic_Elem),standradElemKey);
			CArray<T_BTMP_K, T_BTMP_K> rarstandradElemKey;
			CArray<T_BTMP_D, T_BTMP_D&> rarstandradElemData;
			m_pDoc->m_pAttrCtrl->GetBtmp(standradElemKey,rarstandradElemKey,rarstandradElemData);
			if(rarstandradElemData.GetSize()>0)
			{
				
				
				T_TABLE_D TableSteelD;
				TableSteelD.Initialize();
				TableSteelD.HeaderD.nHeaderRowCount = 1;
				TableSteelD.HeaderD.nHeaderColCount = 4;
				TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Load_Btmp_Number));
				TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Load_Btmp_Width));
				TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Load_Btmp_Height));
				strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_Btmp_Tempr),m_pDoc->m_pUnitCtrl->m_strTemper);
				TableSteelD.HeaderD.aTitleName.Add(strTemp);
				
				
				int iIndexLine = 0;
				
				TableSteelD.RecordD.nRecordColCount = 4;
				
				
				for (int iSmtp = 0;iSmtp<rarstandradElemData.GetSize();iSmtp++)
				{
					TableSteelD.RecordD.aRecordData.RemoveAll();
					T_BTMP_D& rData = rarstandradElemData.GetAt(iSmtp);
					int iBTMPSize = 0;
					for (iBTMPSize=0;iBTMPSize<rData.SectTmp.GetSize();iBTMPSize++)
					{
						if(rData.SectTmp[iBTMPSize].nMaterial == 0)
							break;
					}
					iBTMPSize++;
					TableSteelD.RecordD.aRecordData.SetSize(iBTMPSize);
					TableSteelD.RecordD.nRecordRowCount = iBTMPSize;
					
					T_STLD_D LCaseData;
					if(m_pDoc->m_pAttrCtrl->GetStld(rData.LoadCaseKey,LCaseData))
					{
						for (int j=0;j<rData.SectTmp.GetSize();j++)
						{
							
							{
								strTemp.Format(_T("%d"),j+1);
								TableSteelD.RecordD.aRecordData[j].Add(strTemp);
								
								strTemp.Format(_T("%0.2f"),GetLengthUnitRatio(rData.SectTmp[j].dB));
								TableSteelD.RecordD.aRecordData[j].Add(strTemp);
								
								strTemp.Format(_T("%0.2f"),GetLengthUnitRatio(rData.SectTmp[j].dH1));
								TableSteelD.RecordD.aRecordData[j].Add(strTemp);
								
								strTemp.Format(_T("%0.2f"),rData.SectTmp[j].dT1);
								TableSteelD.RecordD.aRecordData[j].Add(strTemp);
								
								
							}
							
							if(rData.SectTmp[j+1].nMaterial == 0)
							{
								strTemp.Format(_T("%d"),j+2);
								TableSteelD.RecordD.aRecordData[j+1].Add(strTemp);
								
								strTemp.Format(_T("%0.2f"),GetLengthUnitRatio(rData.SectTmp[j].dB));
								TableSteelD.RecordD.aRecordData[j+1].Add(strTemp);
								
								strTemp.Format(_T("%0.2f"),GetLengthUnitRatio(rData.SectTmp[j].dH2));
								TableSteelD.RecordD.aRecordData[j+1].Add(strTemp);
								
								strTemp.Format(_T("%0.2f"),rData.SectTmp[j].dT2);
								TableSteelD.RecordD.aRecordData[j+1].Add(strTemp);
								
								break;
							}
						}
						
					}
					CReportTableProcess ReportTable;
					ReportTable.SetData(TableSteelD);
					ReportTable.SetFilePathName(ReportTable.GetFilePathName() + LCaseData.LoadCaseName + strElemName+ _T(".xml"));
					ReportTable.GenerateReport();
					
					genFtagStr.sCaption = LCaseData.LoadCaseName + strElemName;
					genFtagStr.sType         = _LSX(TBL);
					genFtagStr.sFullPath     = ReportTable.GetFilePathName() + LCaseData.LoadCaseName + strElemName+ _T(".xml");
					genFtagStr.sExtInfo      = genFtagStr.sCaption;
					genFtagStr.sContents     = _T(" ");
					genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
					genFtagStr.sExt          = _T(" ");
					genFtagStr.iLevel        = 0;
					
					genElements.m_SegmentElements.push_back(genFtagStr);
					
					
				}
			}
		}
		
	}
	if(m_pDoc->m_pAttrCtrl->GetCountTdmt()>0)
	{
		iIndex++;
		strText.Format(_T("    %d)") ,iIndex)  ;
		strText = strText + _LS(IDS_AMR_CHRP_Model_Load_Tdmt);
		strText += _T("\n");
		T_TDMT_K rKey ;
		T_TDMT_D rData ;
		POSITION Pos;
		Pos = m_pDoc->m_pAttrCtrl->GetStartTdmt();
		while(Pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextTdmt(Pos,rKey,rData);
			if(rData.COMMON.nCode == 11)
			{
				strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_Tdmt_ConcreteAge),rData.CHJTG04.dConcreteAge);
				strText += strTemp;
				
				strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_Tdmt_MemberSize),rData.CHJTG04.dMemberSize);
				strText += strTemp;
				
				strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_Tdmt_EQ1));
				strTemp += _LS(IDS_AMR_CHRP_Model_Load_Tdmt_EQ2);
				strTemp += _LS(IDS_AMR_CHRP_Model_Load_Tdmt_EQ3);
				//strTemp += _LS(IDS_AMR_CHRP_Model_Load_Tdmt_EQ4);
				
				strTemp += _LS(IDS_AMR_CHRP_Model_Load_Tdmt_EQ5);
				strText += strTemp;
				
			}
			
		}
		
		
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
		genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Load_Tdmt);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
	}
	
	if(m_pDoc->m_pAttrCtrl->GetCountSmpt()>0)
	{
		iIndex++;
		strText.Format(_T("    %d)") ,iIndex)  ;
		strText = strText + _LS(IDS_AMR_CHRP_Model_Load_Smpt);
		strText += _T("\n");
		T_SMPT_K rKey ;
		T_SMPT_D rData ;
		POSITION Pos;
		Pos = m_pDoc->m_pAttrCtrl->GetStartSmpt();
		strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_Smpt_Explanation));
		strText += strTemp;
		int iIndexSettlement = 0;
		while(Pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextSmpt(Pos,rKey,rData);
			iIndexSettlement++;
			
			
			strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_Smpt_All),iIndexSettlement,rData.dSettle);
			strText += strTemp;
		}
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
		genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Load_Smpt);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
	}
	
	T_MVCD_D MvcdD;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD))
	{
		MvcdD.Initialize();
	}
	int nMCodeType = MvcdD.nCodeType;
	BOOL bHaveRQ = FALSE;
	if(nMCodeType == D_MOVE_CODE_TRANS)
	{
		if(m_pDoc->m_pAttrCtrl->GetCountMvhltr()>0)
		{
			iIndex++;
			strText.Format(_T("    %d)") ,iIndex)  ;
			strText = strText + _LS(IDS_AMR_CHRP_Model_Load_MoveLoad);
			strText += _T("\n");
			
			T_MVHLtr_K rKey ;
			T_MVHLtr_D rData ;
			POSITION Pos;
			int iIndexSettlement = 0;
			Pos = m_pDoc->m_pAttrCtrl->GetStartMvhltr();
			while(Pos != NULL)
			{
				m_pDoc->m_pAttrCtrl->GetNextMvhltr(Pos,rKey,rData);
				iIndexSettlement++;
				CString strTruckName = rData.VehicleLoadName;
				
				if(m_pDoc->m_pAttrCtrl->GetCountMvhltr() == 1)
				{
					
					strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_LiveLoad),strTruckName);
					strText += strTemp;
					
				}
				else
				{
					if(iIndexSettlement == 1)
					{
						strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_LiveLoad2),m_pDoc->m_pAttrCtrl->GetCountMvhltr(),strTruckName);
					}
					else
					{
						strTemp.Format(_T("%s，"),strTruckName);
						
					}
					strText += strTemp + _LS(IDS_AMR_CHRP_Model_Load_TrMoveLoad);
				} 
			}
			genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
			genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Load_MoveLoad);
			genFtagStr.sType         = _LSX(TXT);
			genFtagStr.sFullPath     = _T(" ");
			genFtagStr.sExtInfo      = genFtagStr.sCaption;
			genFtagStr.sContents     = strText;
			genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
			genFtagStr.sExt          = _T(" ");
			genFtagStr.iLevel        = 0;
			
			genElements.m_SegmentElements.push_back(genFtagStr);
			
		}
	}
	else if(nMCodeType == D_MOVE_CODE_CHINA)
	{
		if(m_pDoc->m_pAttrCtrl->GetCountMvhl()>0)
		{
			iIndex++;
			strText.Format(_T("    %d)") ,iIndex)  ;
			strText = strText + _LS(IDS_AMR_CHRP_Model_Load_MoveLoad);
			strText += _T("\n");
			T_MVHL_K rKey ;
			T_MVHL_D rData ;
			POSITION Pos;
			int iIndexSettlement = 0;
			Pos = m_pDoc->m_pAttrCtrl->GetStartMvhl();
			while(Pos != NULL)
			{
				m_pDoc->m_pAttrCtrl->GetNextMvhl(Pos,rKey,rData);
				iIndexSettlement++;
				CString strTruckName = _T("");
				if(rData.nLoadType == 0)
				{
					strTruckName = _LS(IDS_AMR_CHRP_Model_Load_TruckName);
				}
				else if(rData.nLoadType == 1)
				{
					strTruckName = _LS(IDS_AMR_CHRP_Model_Load_TrainName);
				}
				else
				{
					bHaveRQ = TRUE;
					strTruckName = _LS(IDS_AMR_CHRP_Model_Load_RQName);
				}
				if(m_pDoc->m_pAttrCtrl->GetCountMvhl() == 1)
				{
					
					strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_LiveLoad),strTruckName);
					strText += strTemp;
					
				}
				else
				{
					if(iIndexSettlement == 1)
					{
						strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_LiveLoad2),m_pDoc->m_pAttrCtrl->GetCountMvhl(),strTruckName);
					}
					else
					{
						strTemp.Format(_T("%s，"),strTruckName);
						
					}
					strText += strTemp;
				} 
			}
			T_MVCTch_D MVCTchData;
			m_pDoc->m_pAttrCtrl->GetMvctch(MVCTchData);
			if(MVCTchData.bBridgeClass)
			{
				strText += _LS(IDS_AMR_CHRP_Model_Load_BridgeType);
				if(MVCTchData.nBridgeClass == 0)
				{
					strText += _LS(IDS_AMR_CHRP_Model_Load_BridgeType_1);
				}
				else
				{
					strText += _LS(IDS_AMR_CHRP_Model_Load_BridgeType_2);
				}
			}
			
			if(MVCTchData.nCodeType == 0)
			{
				strText += _LS(IDS_AMR_CHRP_Model_Load_Frequency_EQ1);
				strText += _LS(IDS_AMR_CHRP_Model_Load_Frequency_EQ2);
				strText += _LS(IDS_AMR_CHRP_Model_Load_Frequency_EQ3);
				strText += _LS(IDS_AMR_CHRP_Model_Load_Frequency_EQ4);
				
				double df = 0.0;
				double df2 = 0.0;
				if(MVCTchData.nFrequencyType == 0)
				{
					df = MVCTchData.FreqUser.df;
				}
				else if(MVCTchData.nFrequencyType == 1)
				{
					df = 3.1415926*sqrt(MVCTchData.FreqSbem.dE*MVCTchData.FreqSbem.dIc/MVCTchData.FreqSbem.dmc)/(2.0*MVCTchData.FreqSbem.dL*MVCTchData.FreqSbem.dL);
				}
				else if(MVCTchData.nFrequencyType == 2)
				{
					df = MVCTchData.FreqCbem.da*sqrt(MVCTchData.FreqCbem.dE*MVCTchData.FreqCbem.dIc/MVCTchData.FreqCbem.dmc)/(2.0*3.1415926*MVCTchData.FreqCbem.dL*MVCTchData.FreqCbem.dL);
					df2 = MVCTchData.FreqCbem.db*sqrt(MVCTchData.FreqCbem.dE*MVCTchData.FreqCbem.dIc/MVCTchData.FreqCbem.dmc)/(2.0*3.1415926*MVCTchData.FreqCbem.dL*MVCTchData.FreqCbem.dL);
				}
				else if(MVCTchData.nFrequencyType == 3)
				{
					if(MVCTchData.nArchType == 0)
						df = 105*(5.4+50*MVCTchData.FreqArch.df*MVCTchData.FreqArch.df)/(16.45+334*pow(MVCTchData.FreqArch.df,2.0) +1867*pow(MVCTchData.FreqArch.df,4.0) );
					else
						df = 105*(5.4+50*MVCTchData.FreqArch.df*MVCTchData.FreqArch.df)/(16.45+334*pow(MVCTchData.FreqArch.df,2.0) +1867*pow(MVCTchData.FreqArch.df,4.0) );
					
				}
				else if(MVCTchData.nFrequencyType == 4)
				{
					df = MVCTchData.FreqCabl.da / MVCTchData.FreqCabl.dL;
				}
				else if(MVCTchData.nFrequencyType == 5)
				{
					df = sqrt((MVCTchData.FreqSusp.dE * MVCTchData.FreqSusp.dI * pow(2*3.1415926/MVCTchData.FreqSusp.dL,2.0)+2*MVCTchData.FreqSusp.dHg)/MVCTchData.FreqSusp.dm) / MVCTchData.FreqSusp.dL;
				}
				
				if(MVCTchData.nFrequencyType == 2)
				{
					strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_Frequency_Result1),df,GetU(df),df2,GetU(df2));
					strText += strTemp;
					
				}
				else
				{
					strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_Frequency_Result2),df,GetU(df));
					strText += strTemp;
				}
				
				
			}
			
			genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
			genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Load_MoveLoad);
			genFtagStr.sType         = _LSX(TXT);
			genFtagStr.sFullPath     = _T(" ");
			genFtagStr.sExtInfo      = genFtagStr.sCaption;
			genFtagStr.sContents     = strText;
			genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
			genFtagStr.sExt          = _T(" ");
			genFtagStr.iLevel        = 0;
			
			genElements.m_SegmentElements.push_back(genFtagStr);
		}
	}
	
	if(bHaveRQ)
	{
		iIndex++;
		strText.Format(_T("    %d)") ,iIndex)  ;
		strText = strText + _LS(IDS_AMR_CHRP_Model_Load_RQName);
		strText += _T("\n");
		strTemp.Format(_LS(IDS_AMR_CHRP_Model_Load_RQ_EQ));
		strText += strTemp;
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
		genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Load_RQName);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
	}
	
	if(m_pDoc->m_pAttrCtrl->GetCountLcom(D_LCOMTYPE_CONCRETE)>0)
	{
		iIndex++;
		strText.Format(_T("    %d)") ,iIndex)  ;
		strText = strText + _LS(IDS_AMR_CHRP_Model_Load_LoadComb);
		strText += _T("\n");
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
		genFtagStr.sCaption = genFtagStr.sCaption + _LS(IDS_AMR_CHRP_Model_Load_LoadComb);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		CMap<CString, LPCTSTR, CString, CString&> mapLcom;
		GetLoadCombinationCases(mapLcom);
		
		//
		T_TABLE_D TableSteelD;
		TableSteelD.Initialize();
		TableSteelD.HeaderD.nHeaderRowCount = 1;
		TableSteelD.HeaderD.nHeaderColCount = 3;
		TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Load_LoadCase_Number));
		TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Load_LoadCase_Name));
		TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_Model_Load_LoadCase_Explaination));
		
		
		int iIndexLine = 0;
		POSITION Pos = mapLcom.GetStartPosition();
		CString rKey;
		CString rValue;
		TableSteelD.RecordD.nRecordColCount = 3;
		TableSteelD.RecordD.aRecordData.SetSize(mapLcom.GetCount());
		TableSteelD.RecordD.nRecordRowCount = mapLcom.GetCount();
		
		while(Pos != NULL)
		{
			iIndexLine++;
			mapLcom.GetNextAssoc(Pos,rKey,rValue);
			
			strTemp.Format(_T("%d"),iIndexLine);
			TableSteelD.RecordD.aRecordData[iIndexLine-1].Add(strTemp);
			
			TableSteelD.RecordD.aRecordData[iIndexLine-1].Add(rKey);
			
			TableSteelD.RecordD.aRecordData[iIndexLine-1].Add(rValue);
			
		}
		
		CReportTableProcess ReportTable;
		ReportTable.SetData(TableSteelD);
		ReportTable.SetFilePathName(ReportTable.GetFilePathName() + CString(_LS(IDS_AMR_CHRP_Model_Load_LoadCase))+_T(".xml"));
		ReportTable.GenerateReport();
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP_Model_Load_LoadCase);
		genFtagStr.sType         = _LSX(TBL);
		genFtagStr.sFullPath     = ReportTable.GetFilePathName() + CString(_LS(IDS_AMR_CHRP_Model_Load_LoadCase)) +_T(".xml");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = _T(" ");
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		strText = _LS(IDS_AMR_CHRP_Model_Load_LoadCase_Table);
		T_LCOM_K rLKey ;
		T_LCOM_D rLData ;
		int iIndexSettlement = 0;
		CArray<T_LCOM_K, T_LCOM_K> rLKeyList;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE,rLKeyList);
		for(int ilc = 0;ilc <rLKeyList.GetSize();ilc++)
		{
			rLKey = rLKeyList.GetAt(ilc);
			if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE,rLKey,rLData))
			{
				strTemp.Format(_T("%s：%s\n"),rLData.LoadCombName,rLData.Description);
				strText += strTemp;
			}
		}
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Model_Load);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP_Model_Load_LoadComb);
		genFtagStr.sType         = _LSX(TXT);
		genFtagStr.sFullPath     = _T(" ");
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = strText;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T(" ");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		
	}
	return TRUE;
	
}
CString CReportTreeText::GetConstructionStageLoadCaseSymbol(T_SGLD_K sgldK)
{
	CString aSymbols[] = { _T("cD"), _T("cEL1"), _T("cEL2"), _T("cEL3"), _T("cEL4"), _T("cEL5"), _T("cEL6"), _T("cEL7"), _T("cEL8"), _T("cEL9"), _T("cEL10"), _T("cEL11"), _T("cEL12"), _T("cEL13"), _T("cEL14"), _T("cEL15"), _T("cEL16"), _T("cEL17"), _T("cEL18"), _T("cEL19"), _T("cEL20"),
					  _T("cTP"), _T("cTS"), _T("cCR"), _T("cCR"), _T("cSH"), _T("cSH"), _T("SUM") };
	return (sgldK >= D_SGLD_DL_CVL && sgldK <= D_SGLD_SUM_CVL)? aSymbols[sgldK-1] : _T("");
}
void CReportTreeText::GetLoadCombinationCases(CMap<CString, LPCTSTR, CString, CString&>& mapLcom)
{
	CString strDescript = _T("");
	CString strText     = _T("");
	
	// D_LCOM_STATIC : Static Load Case.
	CArray<T_STLD_K, T_STLD_K> arStldKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);
	
	int iStld = arStldKeyList.GetSize();
	for(int i=0; i<iStld; i++)
	{
		T_STLD_D stld;
		if(m_pDoc->m_pAttrCtrl->GetStld(arStldKeyList[i], stld) && stld.LoadCaseType != _LSX(CS))
		{      
			mapLcom.SetAt(stld.LoadCaseType, stld.LoadCaseName);
		}
	} 
	
	//D_LCOM_SPECTRUM	: Response Spectrum Case.
	CArray<T_SPLC_K, T_SPLC_K> arSplcKeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcKeyList);
	
	int iSplc = arSplcKeyList.GetSize();
	for(int i=0; i<iSplc; i++)
	{
		T_SPLC_D splcD;
		if(m_pDoc->m_pAttrCtrl->GetSplc(arSplcKeyList[i], splcD))
		{
			strText.Format(_T("RS[%d]"), splcD.LoadCaseId);
			mapLcom.SetAt(strText, splcD.LoadCaseName);
		}
	}
	
	//D_LCOM_HISTORY : Time Histroy Analysis.  
	CArray<T_THIS_K, T_THIS_K> arThisKeyList;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(arThisKeyList, FALSE);
	
	int iThis = arThisKeyList.GetSize();
	for(int i=0; i<iThis; i++)
	{
		T_THIS_D thisD;    
		if(m_pDoc->m_pAttrCtrl->GetThis(arThisKeyList[i], thisD))
		{      
			strText.Format(_T("TH[%d]"), thisD.LoadCaseId);
			mapLcom.SetAt(strText, thisD.LoadCaseName);
		}
	}
	
	//D_LCOM_MOVING : Moving Load Anlaysis.
	T_MVCD_D mvcdD;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdD)) mvcdD.Initialize();
	switch (mvcdD.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_JAPAN_RAIL:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		{
			CArray<T_MVLD_K, T_MVLD_K> arMvldKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMvldKeyList);
			
			int iMvld = arMvldKeyList.GetSize();
			for(int i=0; i<iMvld; i++)
			{
				T_MVLD_D mvldD;
				if(m_pDoc->m_pAttrCtrl->GetMvld(arMvldKeyList[i], mvldD))
				{          
					strText.Format(_T("M[%d]"), mvldD.LoadCaseId);
					mapLcom.SetAt(strText, mvldD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_CHINA:
		{
			CArray<T_MVLDch_K, T_MVLDch_K> arMvldchKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMvldchKeyList);
			
			int iMvldch = arMvldchKeyList.GetSize();
			for(int i=0; i<iMvldch; i++)
			{
				T_MVLDch_D mvldchD;
				if(m_pDoc->m_pAttrCtrl->GetMvldch(arMvldchKeyList[i], mvldchD))
				{
					strText.Format(_T("M[%d]"), mvldchD.LoadCaseId);
					mapLcom.SetAt(strText, mvldchD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_JAPAN:
		{
			CArray<T_MVLDjp_K, T_MVLDjp_K> arMvldjpKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMvldjpKeyList);
			
			int iMvldjp = arMvldjpKeyList.GetSize();
			for(int i=0; i<iMvldjp; i++)
			{
				T_MVLDjp_D mvldjpD;
				if(m_pDoc->m_pAttrCtrl->GetMvldjp(arMvldjpKeyList[i], mvldjpD))
				{
					strText.Format(_T("M[%d]"), mvldjpD.LoadCaseId);
					mapLcom.SetAt(strText, mvldjpD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_INDIA:
		{
			CArray<T_MVLDid_K, T_MVLDid_K> arMvldidKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMvldidKeyList);
			
			int iMvldid = arMvldidKeyList.GetSize();
			for(int i=0; i<iMvldid; i++)
			{
				T_MVLDid_D mvldidD;
				if(m_pDoc->m_pAttrCtrl->GetMvldid(arMvldidKeyList[i], mvldidD))
				{
					strText.Format(_T("M[%d]"), mvldidD.LoadCaseId);
					mapLcom.SetAt(strText, mvldidD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_BS:
		{
			CArray<T_MVLDbs_K, T_MVLDbs_K> arMvldbsKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(arMvldbsKeyList);
			
			int iMvldbs = arMvldbsKeyList.GetSize();
			for(int i=0; i<iMvldbs; i++)
			{
				T_MVLDbs_D mvldbsD;
				if(m_pDoc->m_pAttrCtrl->GetMvldbs(arMvldbsKeyList[i], mvldbsD))
				{
					strText.Format(_T("M[%d]"), mvldbsD.LoadCaseId);
					mapLcom.SetAt(strText, mvldbsD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_FRANCE:
		{
			CArray<T_MVLDfr_K, T_MVLDfr_K> arMvldfrKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(arMvldfrKeyList);
			
			int iMvldfr = arMvldfrKeyList.GetSize();
			for(int i=0; i<iMvldfr; i++)
			{
				T_MVLDfr_D mvldfrD;
				if(m_pDoc->m_pAttrCtrl->GetMvldfr(arMvldfrKeyList[i], mvldfrD))
				{
					strText.Format(_T("M[%d]"), mvldfrD.LoadCaseId);
					mapLcom.SetAt(strText, mvldfrD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_TRANS:
		{
			CArray<T_MVLDtr_K, T_MVLDtr_K> arMvldtrKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(arMvldtrKeyList);
			
			int iMvldtr = arMvldtrKeyList.GetSize();
			for(int i=0; i<iMvldtr; i++)
			{
				T_MVLDtr_D mvldtrD;
				if(m_pDoc->m_pAttrCtrl->GetMvldtr(arMvldtrKeyList[i], mvldtrD))
				{
					strText.Format(_T("M[%d]"), mvldtrD.LoadCaseId);
					mapLcom.SetAt(strText, mvldtrD.LoadCaseName);
				}
			} 
		}
		break;
	default:
		ASSERT(0);
		break;
	} 
	
	//D_LCOM_SETTLEMENT	: Settlement Displacement.
	CArray<T_SMLC_K, T_SMLC_K> arSmlcKeyList;
	m_pDoc->m_pAttrCtrl->GetSmlcKeyList(arSmlcKeyList);
	
	int iSmlc = arSmlcKeyList.GetSize();
	for(int i=0; i<iSmlc; i++)
	{
		T_SMLC_D smlcD;
		if(m_pDoc->m_pAttrCtrl->GetSmlc(arSmlcKeyList[i], smlcD))
		{
			mapLcom.SetAt(_T("SM"), smlcD.LoadCaseName);
		}
	}
	
	//D_LCOM_STAGE : Construction Stage Analysis.
	CArray<T_SGLD_K, T_SGLD_K> arSgldKeyList;
	m_pDoc->m_pAttrCtrl->GetSgldKeyList(arSgldKeyList);
	
	int iSgld = arSgldKeyList.GetSize();  
	for(int i=0; i<iSgld; i++)
	{    
		T_SGLD_D sgldD;
		if(m_pDoc->m_pAttrCtrl->GetSgld(arSgldKeyList[i], sgldD))
		{      
			CString strText = GetConstructionStageLoadCaseSymbol(arSgldKeyList[i]);
			mapLcom.SetAt(strText, sgldD.LoadCaseName + _LSX((CS)));
		}    
	}
	
	//D_LCOM_ECCENSPEC : 우발편심 응답 스펙트럼 하중 조건
	CArray<T_ESPC_K, T_ESPC_K> arEspcKeyList;
	m_pDoc->m_pAttrCtrl->GetEspcKeyList(arEspcKeyList);
	
	int iEspc = arEspcKeyList.GetSize();  
	for(int i=0; i<iEspc; i++)
	{
		T_ESPC_D espcD;    
		if(m_pDoc->m_pAttrCtrl->GetEspc(arEspcKeyList[i], espcD))
		{
			mapLcom.SetAt(espcD.LoadCaseName, espcD.LoadCaseName + _LSX((ES)));    
		}    
	}
	
	//D_LCOM_GRIDMODEL  : 격자모델 하중 조건    
	CArray<T_GILC_K, T_GILC_K> arGilcKeyList;
	m_pDoc->m_pAttrCtrl->GetGilcKeyList(arGilcKeyList);
	
	int iGlic = arGilcKeyList.GetSize();  
	for(int i=0; i<iGlic; i++)
	{
		T_GILC_D gilcD;
		if(m_pDoc->m_pAttrCtrl->GetGilc(arGilcKeyList[i], gilcD))
		{
			mapLcom.SetAt(gilcD.LoadCaseName, gilcD.LoadCaseName + _LSX((GI)));    
		}
	}  
}
void CReportTreeText::GetElementsForDesign()
{
	m_arElemKeyList.RemoveAll();
	m_arColumnElemKeyList.RemoveAll();
	
	
	CArray<T_ELEM_K,T_ELEM_K> atlist;
	atlist.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetElemKeyList(atlist);  
	for(int i=0; i<atlist.GetSize(); i++)
	{    
		T_ELEM_D ElemD;
		T_ELEM_K ElemK = atlist.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
		if (CDBLib::IsRcColumnDgn(ElemK,FALSE)) {
			m_arColumnElemKeyList.Add(ElemK);
		}
		
		if(CDBLib::IsPscDgnFor1st(ElemPairK(ElemK, EN_EL_BEAM),TRUE)){
			m_arElemKeyList.Add(ElemK);
		}    
	}
	
	m_arTdnaK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(m_arTdnaK);
	
}
CString CReportTreeText::GetResultFlgString(BOOL bTrue,int iType,CString& strChina,CString& strbTrue)
{
	CString strFlg = _T("");
	switch (iType)
	{
	case 1:
		strFlg = bTrue ? _LS(IDS_AMR_CHRP_Calculation_Result_Flag1) : _LS(IDS_AMR_CHRP_Calculation_Result_Flag2);
		strChina = bTrue ? _LS(IDS_AMR_CHRP_Calculation_Result_CHFlag1): _LS(IDS_AMR_CHRP_Calculation_Result_CHFlag2);
		strbTrue = bTrue ? _LS(IDS_AMR_CHRP_Calculation_Result_Yes) : _LS(IDS_AMR_CHRP_Calculation_Result_No);
		break;
		
	default:
		break;
	}
	return strFlg;
}
BOOL    CReportTreeText::WriteLoadCapacity_ComDepth (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_LoadCapacity_ComDepth);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_LoadCapacity_ComDepth));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_LoadCapacity_ComDepth);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_ComDepth);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T(" ");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_LoadCapacity_ComDepth);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	
	T_TABLE_D TableSteelD;
	TableSteelD.Initialize();
	TableSteelD.HeaderD.nHeaderRowCount = 1;
	TableSteelD.HeaderD.nHeaderColCount = 5;
	TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth_Steel));
	TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth_C50));
	TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth_C5560));
	TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth_C6570));
	TableSteelD.HeaderD.aTitleName.Add(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth_C7580));
	
	
	int iIndexLine = 0;
	TableSteelD.RecordD.nRecordColCount = 5;
	TableSteelD.RecordD.aRecordData.SetSize(5);
	TableSteelD.RecordD.nRecordRowCount = 5;
	
	
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("R235"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.62"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.60"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.58"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("-"));
	iIndexLine++;
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("HRB335"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.56"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.54"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.52"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("-"));
	iIndexLine++;
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("HRB400/KL400"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.53"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.51"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.49"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("-"));
	iIndexLine++;
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth_Tendon));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.40"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.38"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.36"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.35"));
	iIndexLine++;
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth_RenforcingBar));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.40"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.38"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("0.36"));
	TableSteelD.RecordD.aRecordData[iIndexLine].Add(_T("-"));
	iIndexLine++;
	
	
	
	
	CReportTableProcess ReportTable;
	ReportTable.SetData(TableSteelD);
	CString strTitle = _LS(IDS_AMR_CHRP_LoadCapacity_ComDepth_Title);
	ReportTable.SetFilePathName(ReportTable.GetFilePathName() + strTitle + _T(".xml"));
	ReportTable.GenerateReport();
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_ComDepth);
	genFtagStr.sCaption += strTitle;
	genFtagStr.sType         = _LSX(TBL);
	genFtagStr.sFullPath     = ReportTable.GetFilePathName() + strTitle + _T(".xml");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = _T(" ");
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteLoadCapacity_Bending  (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_LoadCapacity_Bending);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_LoadCapacity_Bending));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_LoadCapacity_Bending);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Bending);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_LoadCapacity_Bending);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	BOOL bResultFlag = TRUE;
	T_ELEM_K ElemK;
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();
	CPostDrawAdapter drawAdapter(strDocPath);
	CString strDetailTable = _T("");
	CArray<UINT,UINT> arElemList;
	if(m_pDoc->IsPscJtg04())
	{
		T_BDCY_JTG BdcyJtgD;
		std::map<T_ELEM_K,T_BDCY_JTG> mapBDCY_JTG_datas;
		if(!CChinaReportSetting::GetChinaReportSetting()->GetOutPutElemList(arElemList))
		{
			ArrElemPairKey aElemPairK;
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyJtgKeyList(aElemPairK);
			CDBLib::GetElemKeyList4ElemPairKey(aElemPairK, arElemList);
		}
		if(arElemList.GetSize() == 0)
			return TRUE;

		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			ElemPairK EPairK(ElemK, EN_EL_BEAM);
			if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyJtgData(EPairK,  BdcyJtgD))
				continue;
			ConvertUnit_BDCY(BdcyJtgD);
			mapBDCY_JTG_datas.insert(make_pair(ElemK, BdcyJtgD));
			
			for (int i=0;i<4;i++)
			{
				if(bResultFlag && !BdcyJtgD.BdcyBase[i].bOK)
					bResultFlag = FALSE;
			}
		}
		strDocPath = drawAdapter.Build_PSC_BDCY_Graphic(mapBDCY_JTG_datas);
		strDetailTable = drawAdapter.Build_PSC_BDCY_Table(mapBDCY_JTG_datas);
		
	}
	else if(m_pDoc->IsCHRcJtg04(TRUE))
	{
		T_CBCS_CH cbcs;    
		std::map<T_ELEM_K,T_CBCS_CH> mapCBCS_CH_datas;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCHRcCbcskeyList(arElemList);

		if(arElemList.GetSize() == 0)
			return TRUE;

		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCHRcCbcsData(ElemK, cbcs);
			ConvertUnit_CBCS(cbcs);
			mapCBCS_CH_datas.insert(make_pair(ElemK, cbcs));
			for (int i=0;i<4;i++)
			{
				if(bResultFlag && !cbcs.cbcsBase[i].bOK)
					bResultFlag = FALSE;
			}
		}
		strDocPath = drawAdapter.Build_CHRC_CBCS_Graphic(mapCBCS_CH_datas);
		strDetailTable = drawAdapter.Build_CHRC_CBCS_Table(mapCBCS_CH_datas);
	}
	else
	{
		ASSERT(0);
		return TRUE;
	}
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Bending);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
	genFtagStr.sType         = _LSX(IMG);
	genFtagStr.sFullPath     = strDocPath;
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = genFtagStr.sCaption;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	
	CString strResultFlag = _T("");
	CString strResultChina = _T("");
	CString strResultYesNo = _T("");
	
	strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	
	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Bending_Conclusion1),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Bending_Conclusion2));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Bending_Conclusion3),strResultChina,strResultYesNo);
	strText += strTemp;
	
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Bending);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Conclusion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	m_vctDetailTable.push_back(make_pair(_LS(IDS_AMR_CHRP_LoadCapacity_Bending),strDetailTable));
	return TRUE;
	
}
BOOL    CReportTreeText::WriteLoadCapacity_Shearing (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_LoadCapacity_Shearing);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_LoadCapacity_Shearing));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_LoadCapacity_Shearing);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Shearing);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_LoadCapacity_Shearing);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	BOOL bResultFlag = TRUE;
	BOOL bSectResultFlag = TRUE;
	//////////////////////////////////////////////////////////////////////////
	T_ELEM_K ElemK;
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();
	CPostDrawAdapter drawAdapter(strDocPath);
	CString strDetailTable = _T("");
	CArray<UINT,UINT> arElemList;

	if(m_pDoc->IsPscJtg04())
	{
		T_CUMS_JTG JtgD;
		std::map<T_ELEM_K,T_CUMS_JTG> mapJTG_datas;

		if(!CChinaReportSetting::GetChinaReportSetting()->GetOutPutElemList(arElemList))
		{
			ArrElemPairKey aElemPairK;
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCumsJtgKeyList(aElemPairK);
			CDBLib::GetElemKeyList4ElemPairKey(aElemPairK, arElemList);
		}
		if(arElemList.GetSize() == 0)
			return TRUE;

		
		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			ElemPairK EPairK(ElemK, EN_EL_BEAM);
			if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCumsJtgData(EPairK,  JtgD))
				continue;
			ConvertUnit_CUMS(JtgD);
			mapJTG_datas.insert(make_pair(ElemK, JtgD));
			
			for (int i=0;i<4;i++)
			{
				if(bResultFlag && !JtgD.CumsBase[i].bOK)
					bResultFlag = FALSE;
				if(fabs(JtgD.CumsBase[i].drVu)<=JtgD.CumsBase[i].dShrLoad)
				{
					 //쌔충駱炬契법
				}
				else
				{
					if(bSectResultFlag && !(fabs(JtgD.CumsBase[i].drVu) <= JtgD.CumsBase[i].dShrSect))
						bSectResultFlag = FALSE;

				}

				
			}
		}
		strDocPath = drawAdapter.Build_PSC_CUMS_Graphic(mapJTG_datas);
		strDetailTable = drawAdapter.Build_PSC_CUMS_Table(mapJTG_datas);
		
	}
	else if(m_pDoc->IsCHRcJtg04(TRUE))
	{
		T_CSIS_CH rcdata;    
		std::map<T_ELEM_K,T_CSIS_CH> mapRC_datas;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCHRcCsiskeyList(arElemList);
		if(arElemList.GetSize() == 0)
			return TRUE;
		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCHRcCsisData(ElemK, rcdata);
			ConvertUnit_CSIS(rcdata);
			mapRC_datas.insert(make_pair(ElemK, rcdata));
			for (int i=0;i<4;i++)
			{
				
				if(bResultFlag && !rcdata.CsisBase[i].bOK)
					bResultFlag = FALSE;
				if(fabs(rcdata.CsisBase[i].drVu)<=rcdata.CsisBase[i].dShrLoad)
				{
					//쌔충駱炬契법
				}
				else
				{
					if(bSectResultFlag && !(fabs(rcdata.CsisBase[i].drVu) <= rcdata.CsisBase[i].dShrSect))
						bSectResultFlag = FALSE;
					
				}
			}
		}
		strDocPath = drawAdapter.Build_CHRC_CSIS_Graphic(mapRC_datas);
		strDetailTable = drawAdapter.Build_CHRC_CSIS_Table(mapRC_datas);
		
	}
	else
	{
		ASSERT(0);
		return TRUE;
	}
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Shearing);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
	genFtagStr.sType         = _LSX(IMG);
	genFtagStr.sFullPath     = strDocPath;
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = genFtagStr.sCaption;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	//////////////////////////////////////////////////////////////////////////
	
	CString strResultFlag = _T("");
	CString strResultChina = _T("");
	CString strResultYesNo = _T("");
	
	strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	
	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Shearing_Conclusion1),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Shearing_Conclusion2));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Shearing_Conclusion3),strResultChina,strResultYesNo);
	strText += strTemp;
	
	strResultFlag = GetResultFlgString(bSectResultFlag,1,strResultChina,strResultYesNo);
	
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Shearing_Conclusion4),GetChinaCodeName(_T("JTG D62-2004")),strResultYesNo);
	strText += strTemp;
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Shearing);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Conclusion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	m_vctDetailTable.push_back(make_pair(_LS(IDS_AMR_CHRP_LoadCapacity_Shearing),strDetailTable));
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteLoadCapacity_Torsion  (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_LoadCapacity_Torsion);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_LoadCapacity_Torsion));
	strText += strTemp;
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_LoadCapacity_Torsion);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Torsion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_LoadCapacity_Torsion);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	BOOL bResultFlag = TRUE;
	BOOL bSectResultFlag = TRUE;
	
	//////////////////////////////////////////////////////////////////////////
	T_ELEM_K ElemK;
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();
	CPostDrawAdapter drawAdapter(strDocPath);
	CString strDetailTable = _T("");
	CArray<UINT,UINT> arElemList;
	CString strDocPath2 = _T("");
	if(m_pDoc->IsPscJtg04())
	{
		T_CRMT_JTG JtgD;
		std::map<T_ELEM_K,T_CRMT_JTG> mapJTG_datas;
		if(!CChinaReportSetting::GetChinaReportSetting()->GetOutPutElemList(arElemList))
		{
			ArrElemPairKey aElemPairK;
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCrmtJtgKeyList(aElemPairK);
			CDBLib::GetElemKeyList4ElemPairKey(aElemPairK, arElemList);
		}

		if(arElemList.GetSize() == 0)
			return TRUE;

		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			ElemPairK EPairK(ElemK, EN_EL_BEAM);
			if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCrmtJtgData(EPairK,  JtgD))
				continue;
			ConvertUnit_CRMT(JtgD);
			mapJTG_datas.insert(make_pair(ElemK, JtgD));
			
			for (int i=0;i<6;i++)
			{
				if(bResultFlag && !JtgD.CrmtBase[i].bOK)
					bResultFlag = FALSE;
				if(fabs(JtgD.CrmtBase[i].dShrTorVal) <= JtgD.CrmtBase[i].dTorLoad)
				{
					 //契법
				}
				else
				{
					if(bSectResultFlag && !(fabs(JtgD.CrmtBase[i].dShrTorVal) <= JtgD.CrmtBase[i].dTorSect))
						bSectResultFlag = FALSE;

				}
				
			}
		}
		strDocPath = drawAdapter.Build_PSC_CRMT_Graphic(mapJTG_datas);
		strDocPath2 = drawAdapter.Build_PSC_CRMT_Graphic2(mapJTG_datas);

		strDetailTable = drawAdapter.Build_PSC_CRMT_Table(mapJTG_datas);
		
		
	}
	else if(m_pDoc->IsCHRcJtg04(TRUE))
	{
		T_CTSS_CH rcdata;    
		std::map<T_ELEM_K,T_CTSS_CH> mapRC_datas;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCHRcCtsskeyList(arElemList);
		if(arElemList.GetSize() == 0)
			return TRUE;
		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCHRcCtssData(ElemK, rcdata);
			ConvertUnit_CTSS(rcdata);
			mapRC_datas.insert(make_pair(ElemK, rcdata));
			for (int i=0;i<6;i++)
			{
				if(bResultFlag && !rcdata.CtssBase[i].bOK)
					bResultFlag = FALSE;
				if(fabs(rcdata.CtssBase[i].dShrTorVal) <= rcdata.CtssBase[i].dTorLoad)
				{
					//契법
				}
				else
				{
					if(bSectResultFlag && !(fabs(rcdata.CtssBase[i].dShrTorVal) > rcdata.CtssBase[i].dTorSect))
						bSectResultFlag = FALSE;
					
				}        
			}
		}
		strDocPath = drawAdapter.Build_CHRC_CTSS_Graphic(mapRC_datas);
		strDetailTable = drawAdapter.Build_CHRC_CTSS_Table(mapRC_datas);
		
	}
	else
	{
		ASSERT(0);
		return TRUE;
	}
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Torsion);
	genFtagStr.sCaption +=  _T("――T");

	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
	genFtagStr.sType         = _LSX(IMG);
	genFtagStr.sFullPath     = strDocPath;
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = genFtagStr.sCaption;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);

	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Torsion);
	genFtagStr.sCaption +=  _T("――V");

	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
	genFtagStr.sType         = _LSX(IMG);
	genFtagStr.sFullPath     = strDocPath2;
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = genFtagStr.sCaption;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	//////////////////////////////////////////////////////////////////////////
	
	
	CString strResultFlag = _T("");
	CString strResultChina = _T("");
	CString strResultYesNo = _T("");
	
	strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	
	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Torsion_Conclusion1),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Torsion_Conclusion2));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Torsion_Conclusion3),strResultChina,strResultYesNo);
	strText += strTemp;
	
	strResultFlag = GetResultFlgString(bSectResultFlag,1,strResultChina,strResultYesNo);
	
	strTemp.Format(_LS(IDS_AMR_CHRP_LoadCapacity_Torsion_Conclusion4),GetChinaCodeName(_T("JTG D62-2004")),strResultYesNo);
	strText += strTemp;
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Torsion);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Conclusion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	m_vctDetailTable.push_back(make_pair(_LS(IDS_AMR_CHRP_LoadCapacity_Torsion),strDetailTable));
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteLoadCapacity_Reforce  (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_LoadCapacity_Reforce);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_LoadCapacity_Reforce));
	strText += strTemp;
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_LoadCapacity_Reforce);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Reforce);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_LoadCapacity_Reforce);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	if(!m_pDoc->IsPostMode())
		 return TRUE;
	CArray<T_LCOM_K, T_LCOM_K> rKeyList;
	CArray<T_LCOM_D, T_LCOM_D&> rDataList;
	m_pDoc->m_pAttrCtrl-> GetLcomKeyList(D_LCOMTYPE_CONCRETE,rKeyList );  //serial 번호로 정렬된 리스트
	for (int i=0;i<rKeyList.GetSize();i++)
	{
		T_LCOM_D rData;
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE,rKeyList.GetAt(i),rData);
		rDataList.Add(rData);
	}
	
	CArray<T_NODE_K, T_NODE_K> rNodeKeyList;
	m_pDoc->m_pAttrCtrl-> GetNodeKeyList(rNodeKeyList);  //serial 번호로 정렬된 리스트
	
	CArray<T_REAC_D, T_REAC_D&> ReacDataList;
	CArray<T_NODE_K, T_NODE_K> ValueNodeKeyList;
	
	for (int j=0;j<rNodeKeyList.GetSize();j++)  
	{
		T_REAC_D ReacMax;
		ReacMax.Initialize();
		BOOL bValueNode = TRUE;
		for(int k = 0 ; k<rDataList.GetSize();k++)
		{
			T_REAC_D Reac;
			Reac.Initialize();
			m_pDoc->m_pPostCtrl->SelectLoadCombNew(rDataList.GetAt(k));
			m_pDoc->m_pPostCtrl->GetReacNew(rNodeKeyList.GetAt(j), 0, 0, &Reac);
			
			int m = 0;
			for (m = 0; m < 6; m++)
			{  if (Reac.dblReac[m] != 0.0) break;}
			if (m != 6) 
			{
				ReacMax.AbsMaxwithSign(ReacMax,Reac);
			}
			else
			{ bValueNode = FALSE ; break;}
			
		}
		if(bValueNode)
		{
			ConvertUnit_REAC(ReacMax);
			ReacDataList.Add(ReacMax);
			ValueNodeKeyList.Add(rNodeKeyList.GetAt(j));
		}
	}
	
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();  
	CPostDrawAdapter drawAdapter(strDocPath);
	
	CString strDetailTable = _T("");
	strDetailTable = drawAdapter.Build_Bearing_Reac_Table(ValueNodeKeyList,ReacDataList);
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_LoadCapacity_Reforce);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultTable);
	genFtagStr.sType         = _LSX(TBL);
	genFtagStr.sFullPath     = strDetailTable;
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = _T("");
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteServiceAbility_Normal (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_ServiceAbility_Normal);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_ServiceAbility_Normal));
	strText += strTemp;
	
	T_PSCD_D PscdD; PscdD.Initialize();
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_ServiceAbility_Normal);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Normal);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_ServiceAbility_Normal);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	strText = _T("");
	BOOL bGiveUp = FALSE;
	if(m_pDoc->IsPscJtg04())
	{
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		if(PscdD.iBrgType == 2)
		{ 
			bGiveUp = TRUE;
		}
	}
	else if(m_pDoc->IsCHRcJtg04(TRUE))
	{
		bGiveUp = TRUE;
		return TRUE;
	}
	
	if(bGiveUp)
	{
		strText = _LS(IDS_AMR_CHRP__Pass);    
	}
	else
	{
		if(PscdD.iBrgType == 0)
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conditon1));
			strText += strTemp;
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conditon2));
			strText += strTemp;
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conditon3));
			strText += strTemp;
			
			
		}
		else if(PscdD.iBrgType == 1)
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conditon4));
			strText += strTemp;
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conditon5));
			strText += strTemp;
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conditon6));
			strText += strTemp;
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conditon7));
			strText += strTemp;
		}
	}
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Normal);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Explanation);
	
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	if(bGiveUp)
		return TRUE;
	
	BOOL bResultFlag = TRUE;
	BOOL bLongResultFlag = TRUE;
	BOOL bShortResultFlag = TRUE;
	
	double dAllow = 0.0;
	double dDesign = 0.0;
	double dShortAllow = 0.0;
	double dShortDesign = 0.0;
	double dLongAllow = 0.0;
	double dLongDesign = 0.0;
	
	//////////////////////////////////////////////////////////////////////////
	T_ELEM_K ElemK;
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();
	CString strDocPath2 = temp.GetFilePath();
	
	CPostDrawAdapter drawAdapter(strDocPath);
	CPostDrawAdapter drawAdapter2(strDocPath2);
	
	CString strDetailTable = _T("");
	CArray<UINT,UINT> arElemList;

	
	if(m_pDoc->IsPscJtg04())
	{
		T_FYCM_JTG JtgD;
		std::map<T_ELEM_K,T_FYCM_JTG> mapJTG_datas;
		if(!CChinaReportSetting::GetChinaReportSetting()->GetOutPutElemList(arElemList))
		{
			ArrElemPairKey aElemPairK;
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFycmJtgKeyList(aElemPairK);
			CDBLib::GetElemKeyList4ElemPairKey(aElemPairK, arElemList);
		}

		if(arElemList.GetSize() == 0)
			return TRUE;

		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			ElemPairK EPairK(ElemK, EN_EL_BEAM);
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFycmJtgData(EPairK,  JtgD);
			ConvertUnit_FYCM(JtgD);
			mapJTG_datas.insert(make_pair(ElemK, JtgD));
			
			for (int i=0;i<6;i++)
			{
				
				if(PscdD.iBrgType == 0)
				{
					if(i==1 || i==3)
					{
						if(bResultFlag && !JtgD.FycmBase[i].bOK)
						{
							bResultFlag = FALSE;
							dAllow = JtgD.FycmBase[i].dALW;
							dDesign = JtgD.FycmBase[i].dFMAX;
						}
						if(bResultFlag)
						{
							if(fabs(JtgD.FycmBase[i].dFMAX) > dDesign)
							{
								dAllow = JtgD.FycmBase[i].dALW;
								dDesign = JtgD.FycmBase[i].dFMAX;
							}
						}
						
					}
					
				}
				else if(PscdD.iBrgType == 1)
				{
					if(bShortResultFlag && (i == 1 || i ==3) && JtgD.FycmBase[i].iLoadTerm==1 && !JtgD.FycmBase[i].bOK)
					{
						bShortResultFlag = FALSE;
						dShortAllow = JtgD.FycmBase[i].dALW;
						dShortDesign = JtgD.FycmBase[i].dFMAX;
						
					}
					if(bShortResultFlag && (i == 1 || i ==3))
					{
						if(fabs(JtgD.FycmBase[i].dFMAX) > dShortDesign)
						{
							dShortAllow = JtgD.FycmBase[i].dALW;
							dShortDesign = JtgD.FycmBase[i].dFMAX;
						}
					}
					if(bLongResultFlag && (i == 4 || i ==5) && JtgD.FycmBase[i].iLoadTerm==2 && !JtgD.FycmBase[i].bOK)
					{
						bLongResultFlag = FALSE;
						dLongAllow = JtgD.FycmBase[i].dALW;
						dLongDesign = JtgD.FycmBase[i].dFMAX;
						
					}
					if(bLongResultFlag && (i == 4 || i ==5))
					{
						if(fabs(JtgD.FycmBase[i].dFMAX) > dLongDesign)
						{
							dLongAllow = JtgD.FycmBase[i].dALW;
							dLongDesign = JtgD.FycmBase[i].dFMAX;
						}
					}
					
					
				}
				
			}
		}
		
		if(PscdD.iBrgType == 0)
		{
			strDocPath = drawAdapter.Build_PSC_FYCM_Graphic(0,mapJTG_datas);
			strDetailTable = drawAdapter.Build_PSC_FYCM_Table(0,mapJTG_datas);
			
			genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Normal);
			genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
			genFtagStr.sType         = _LSX(IMG);
			genFtagStr.sFullPath     = strDocPath;
			genFtagStr.sExtInfo      = genFtagStr.sCaption;
			genFtagStr.sContents     = genFtagStr.sCaption;
			genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
			genFtagStr.sExt          = _T("");
			genFtagStr.iLevel        = 0;
			
			genElements.m_SegmentElements.push_back(genFtagStr);
			
		}
		else if(PscdD.iBrgType == 1)
		{
			strDocPath = drawAdapter.Build_PSC_FYCM_Graphic(1,mapJTG_datas);
			
			genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Normal);
			genFtagStr.sCaption += _LS(IDS_AMR_CHRP_ServiceAbility_Normal_ShortGraphic);
			genFtagStr.sType         = _LSX(IMG);
			genFtagStr.sFullPath     = strDocPath;
			genFtagStr.sExtInfo      = genFtagStr.sCaption;
			genFtagStr.sContents     = genFtagStr.sCaption;
			genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
			genFtagStr.sExt          = _T("");
			genFtagStr.iLevel        = 0;
			
			genElements.m_SegmentElements.push_back(genFtagStr);
			
			strDocPath2 = drawAdapter2.Build_PSC_FYCM_Graphic(2,mapJTG_datas);
			genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Normal);
			genFtagStr.sCaption += _LS(IDS_AMR_CHRP_ServiceAbility_Normal_LongGraphic);
			genFtagStr.sType         = _LSX(IMG);
			genFtagStr.sFullPath     = strDocPath2;
			genFtagStr.sExtInfo      = genFtagStr.sCaption;
			genFtagStr.sContents     = genFtagStr.sCaption;
			genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
			genFtagStr.sExt          = _T("");
			genFtagStr.iLevel        = 0;
			
			genElements.m_SegmentElements.push_back(genFtagStr);
			
			strDetailTable = drawAdapter.Build_PSC_FYCM_Table(2,mapJTG_datas);
			
			
			
		}
		
	}
	else
	{
		ASSERT(0);
		return TRUE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	
	CString strResultFlag = _T("");
	CString strResultChina = _T("");
	CString strResultYesNo = _T("");
	
	//strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	strResultFlag = dDesign >= 0.0 ? _T("箕") : _T("윗");

	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conclusion1),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	if(PscdD.iBrgType == 0)
	{
		if(PscdD.iCastType == 0)
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conclusion2),dDesign,strResultFlag);
		}
		else
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conclusion3),dDesign,strResultFlag);
		}
		strText += strTemp;
		strTemp.Format(_LS(IDS_AMR_CHRP_Code_YesNo),strResultYesNo);
		strText += strTemp;
		
	}
	else if(PscdD.iBrgType == 1)
	{
		GetResultFlgString(bShortResultFlag,1,strResultChina,strResultYesNo);
		strResultFlag = /*GetResultFlgString(bShortResultFlag,1,strResultChina,strResultYesNo)*/dShortDesign >= 0.0 ? _T("箕") : _T("윗");
		
		strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conclusion4),dShortDesign, strResultFlag,dShortAllow);
		strText += strTemp;
		strTemp.Format(_LS(IDS_AMR_CHRP_Code_YesNo),strResultYesNo);
		strText += strTemp;
		
		//strResultFlag = GetResultFlgString(bLongResultFlag,1,strResultChina,strResultYesNo);
		GetResultFlgString(bLongResultFlag,1,strResultChina,strResultYesNo);
		strResultFlag = /*GetResultFlgString(bShortResultFlag,1,strResultChina,strResultYesNo)*/dLongDesign >= 0.0 ? _T("箕") : _T("윗");

		strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Normal_Conclusion5),dLongDesign, strResultFlag,dLongAllow);
		strText += strTemp;
		strTemp.Format(_LS(IDS_AMR_CHRP_Code_YesNo),strResultYesNo);
		strText += strTemp;
		
		
	}
	
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Normal);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Conclusion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	m_vctDetailTable.push_back(make_pair(_LS(IDS_AMR_CHRP_ServiceAbility_Normal),strDetailTable));
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteServiceAbility_Oblique(GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_ServiceAbility_Oblique);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_ServiceAbility_Oblique));
	strText += strTemp;
	T_PSCD_D PscdD; PscdD.Initialize();
	
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_ServiceAbility_Oblique);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Oblique);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_ServiceAbility_Oblique);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	strText = _T("");
	BOOL bGiveUp = FALSE;
	if(m_pDoc->IsPscJtg04())
	{
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	}
	else if(m_pDoc->IsCHRcJtg04(TRUE))
	{
		bGiveUp = TRUE;
		return TRUE;
	}
	else
	{
		ASSERT(0);
		return TRUE;
	}
	
	if(bGiveUp)
	{
		strText = _LS(IDS_AMR_CHRP__Pass);
		
	}
	else
	{
		if(PscdD.iBrgType == 0)
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conditon_AllPre));
			strText += strTemp;
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conditon_AllPre1));
			strText += strTemp;
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conditon_AllPre2));
			strText += strTemp;
			
			
		}
		else
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conditon_ABPre));
			strText += strTemp;
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conditon_ABPre1));
			strText += strTemp;
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conditon_ABPre2));
			strText += strTemp;
		}
	}
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Oblique);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Explanation);
	
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	if(bGiveUp)
		return TRUE;
	
	BOOL bResultFlag = TRUE;
	
	double dAllow = 0.0;
	double dDesign = 0.0;
	
	//////////////////////////////////////////////////////////////////////////
	T_ELEM_K ElemK;
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();
	
	CPostDrawAdapter drawAdapter(strDocPath);
	CString strDetailTable = _T("");
	CArray<UINT,UINT> arElemList;

	if(m_pDoc->IsPscJtg04())
	{
		T_FPMS_JTG JtgD;
		std::map<T_ELEM_K,T_FPMS_JTG> mapJTG_datas;
		if(!CChinaReportSetting::GetChinaReportSetting()->GetOutPutElemList(arElemList))
		{
			ArrElemPairKey aElemPairK;
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFpmsJtgKeyList(aElemPairK);
			CDBLib::GetElemKeyList4ElemPairKey(aElemPairK, arElemList);
		}
		if(arElemList.GetSize() == 0)
			return TRUE;

		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			ElemPairK EPairK(ElemK, EN_EL_BEAM);
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFpmsJtgData(EPairK,  JtgD);
			ConvertUnit_FPMS(JtgD);
			mapJTG_datas.insert(make_pair(ElemK, JtgD));
			
			for (int i=0;i<4;i++)
			{
				if((i==1 || i==3))
				{
					if(bResultFlag && !JtgD.FpmsBase[i].bOK)
					{
						bResultFlag = FALSE;
						dAllow = JtgD.FpmsBase[i].dAFP;
						dDesign = JtgD.FpmsBase[i].dFMAX;
					}
					if(bResultFlag)
					{
						if(fabs(JtgD.FpmsBase[i].dFMAX) > fabs(dDesign))
						{
							dAllow = JtgD.FpmsBase[i].dAFP;
							dDesign = JtgD.FpmsBase[i].dFMAX;
							
						}
					}
				}
			}
		}
		
		
		strDocPath = drawAdapter.Build_PSC_FPMS_Graphic(0,mapJTG_datas);
		strDetailTable = drawAdapter.Build_PSC_FPMS_Table(0,mapJTG_datas);
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Oblique);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
		genFtagStr.sType         = _LSX(IMG);
		genFtagStr.sFullPath     = strDocPath;
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = genFtagStr.sCaption;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T("");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		
		
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	
	CString strResultFlag = _T("");
	CString strResultChina = _T("");
	CString strResultYesNo = _T("");
	
	strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	
	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conclusion),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	
	strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	
	if(PscdD.iBrgType == 0)
	{
		if(PscdD.iCastType == 0)
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conclusion1),dDesign, strResultFlag,dAllow);
		}
		else
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conclusion2),dDesign, strResultFlag,dAllow);
			
		}
	}
	else
	{
		if(PscdD.iCastType == 0)
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conclusion3),dDesign, strResultFlag,dAllow);
		}
		else
		{
			strTemp.Format(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique_Conclusion4),dDesign, strResultFlag,dAllow);
		}
	}
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Code_YesNo),strResultYesNo);
	strText += strTemp;
	
	
	
	
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_ServiceAbility_Oblique);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Conclusion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	m_vctDetailTable.push_back(make_pair(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique),strDetailTable));
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteStress_NormalComp     (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_Stress_NormalComp);
	strText = _T("");
	CString strTemp=_T("");
	
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_Stress_NormalComp));
	strText += strTemp;
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Stress_NormalComp);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_NormalComp);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_Stress_NormalComp);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	if(!m_pDoc->IsPscJtg04())
	{
		return TRUE;
	}
	
	
	T_PSCD_D PscdD;
	PscdD.Initialize();
	
	strText = _T("");
	
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_NormalComp_Conditon),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_NormalComp_ConditonStress));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_NormalComp_ConditonNoCrack));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_NormalComp_ConditonNoCrack_EQ));
	strText += strTemp;
	
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_NormalComp_ConditonCrack));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_NormalComp_ConditonCrack_EQ));
	strText += strTemp;
	
	
	
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_NormalComp);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Explanation);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	BOOL bResultFlag = TRUE;
	
	double dAllow =  -1.0E+50;
	double dDesign = -1.0E+50;
	
	//////////////////////////////////////////////////////////////////////////
	T_ELEM_K ElemK;
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();
	
	CPostDrawAdapter drawAdapter(strDocPath);
	CString strDetailTable = _T("");
	CArray<UINT,UINT> arElemList;

	
	if(m_pDoc->IsPscJtg04())
	{
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		
		T_FYCM_JTG JtgD;
		std::map<T_ELEM_K,T_FYCM_JTG> mapJTG_datas;
		if(!CChinaReportSetting::GetChinaReportSetting()->GetOutPutElemList(arElemList))
		{
			ArrElemPairKey aElemPairK;
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFycmJtgKeyList(aElemPairK);
			CDBLib::GetElemKeyList4ElemPairKey(aElemPairK, arElemList);
		}
		if(arElemList.GetSize() == 0)
			return TRUE;

		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			ElemPairK EPairK(ElemK, EN_EL_BEAM);
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFycmJtgData(EPairK,  JtgD);
			ConvertUnit_FYCM(JtgD);
			mapJTG_datas.insert(make_pair(ElemK, JtgD));
			
			for (int i=0;i<6;i++)
			{
				if((i==0 || i==2))
				{
					if( bResultFlag && !JtgD.FycmBase[i].bOK)
					{
						bResultFlag = FALSE;
						dAllow = JtgD.FycmBase[i].dALW;
						dDesign = JtgD.FycmBase[i].dFMAX;
					}
					if(bResultFlag)
					{
						if(JtgD.FycmBase[i].dFMAX > dDesign)
						{
							dAllow = JtgD.FycmBase[i].dALW;
							dDesign = JtgD.FycmBase[i].dFMAX;
						}
					}
				}
			}
		}
		
		
		strDocPath = drawAdapter.Build_PSC_FYCM_Graphic(3,mapJTG_datas);
		strDetailTable = drawAdapter.Build_PSC_FYCM_Table(3,mapJTG_datas);
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_NormalComp);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
		genFtagStr.sType         = _LSX(IMG);
		genFtagStr.sFullPath     = strDocPath;
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = genFtagStr.sCaption;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T("");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		
		
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	
	CString strResultFlag = _T("");
	CString strResultChina = _T("");
	CString strResultYesNo = _T("");
	
	
	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_NormalComp_Conclusion),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	
	strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	
	if(PscdD.iBrgType == 0 || PscdD.iBrgType == 1)
	{
		strTemp.Format(_LS(IDS_AMR_CHRP_Stress_NormalComp_Conclusion1),dDesign, strResultFlag,dAllow);
	}
	else
	{
		strTemp.Format(_LS(IDS_AMR_CHRP_Stress_NormalComp_Conclusion2),dDesign, strResultFlag,dAllow);
	}
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Code_YesNo),strResultYesNo);
	strText += strTemp;
	
	
	
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_NormalComp);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Conclusion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	m_vctDetailTable.push_back(make_pair(_LS(IDS_AMR_CHRP_Stress_NormalComp),strDetailTable));
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteStress_Tension        (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_Stress_Tension);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_Stress_Tension));
	strText += strTemp;
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Stress_Tension);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_Tension);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_Stress_Tension);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	if(!m_pDoc->IsPscJtg04())
	{
		return TRUE;
	}
	
	T_PSCD_D PscdD;
	PscdD.Initialize();
	
	strText = _T("");
	
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_Tension));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_Tendon));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_TendonNoCrack));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_TendonNoCrackEQ));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_TendonCrack));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_TendonCrackEQ));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_Renforcingbar));
	strText += strTemp;   
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_RenforcingbarNoCrack));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_RenforcingbarNoCrackEQ));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_RenforcingbarCrack));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conditon_RenforcingbarCrackEQ));
	strText += strTemp;
	
	
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_Tension);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Explanation);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	BOOL bResultFlag = TRUE;
	
	double dAllow =  -1.0E+50;
	double dDesign = -1.0E+50;
	
	//////////////////////////////////////////////////////////////////////////
	T_ELEM_K ElemK;
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();
	
	CPostDrawAdapter drawAdapter(strDocPath);
	CString strDetailTable = _T("");
	CArray<UINT,UINT> arElemList;

	
	if(m_pDoc->IsPscJtg04())
	{
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		
		T_FYPC_JTG JtgD;
		std::map<T_TDNA_K,T_FYPC_JTG> mapJTG_datas;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFypcJtgKeyList(arElemList);
		if(arElemList.GetSize() == 0)
			return TRUE;

		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFypcJtgData(ElemK,  JtgD);
			ConvertUnit_FYPC(JtgD);
			mapJTG_datas.insert(make_pair(ElemK, JtgD));
			
			for (int i=0;i<4;i++)
			{
				if(bResultFlag && !(JtgD.dFLL < JtgD.dAFLL))
				{
					bResultFlag = FALSE;
					dAllow = JtgD.dAFLL;
					dDesign = JtgD.dFLL;
				}
				if(bResultFlag)
				{
					if(JtgD.dFLL > dDesign)
					{
						dAllow  = JtgD.dAFLL;
						dDesign = JtgD.dFLL;
					}
				}
			}
		}
		
		strDetailTable = drawAdapter.Build_PSC_FYPC_Table(mapJTG_datas);
		
		
		//       strDocPath = drawAdapter.Build_PSC_FYCM_Graphic(3,mapJTG_datas);
		//       genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_NormalComp);
		//       genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
		//       genFtagStr.sType         = _LSX(IMG);
		//       genFtagStr.sFullPath     = strDocPath;
		//       genFtagStr.sExtInfo      = genFtagStr.sCaption;
		//       genFtagStr.sContents     = genFtagStr.sCaption;
		//       genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		//       genFtagStr.sExt          = "";
		//       genFtagStr.iLevel        = 0;
		//       
		//       genElements.m_SegmentElements.push_back(genFtagStr);
		// 
		
		
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	
	CString strResultFlag = _T("");
	CString strResultChina = _T("");
	CString strResultYesNo = _T("");
	
	
	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conclusion),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	
	strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	BOOL bSteelBar = FALSE;
	if(PscdD.iBrgType == 0 || PscdD.iBrgType == 1)
	{
		if(bSteelBar)
			strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conclusion1),dDesign, strResultFlag,dAllow);
		else
			strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conclusion2),dDesign, strResultFlag,dAllow);
	}
	else
	{
		if(bSteelBar)
			strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conclusion3),dDesign, strResultFlag,dAllow);
		else
			strTemp.Format(_LS(IDS_AMR_CHRP_Stress_Tension_Conclusion4),dDesign, strResultFlag,dAllow);
	}
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Code_YesNo),strResultYesNo);
	strText += strTemp;
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_Tension);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Conclusion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	m_vctDetailTable.push_back(make_pair(_LS(IDS_AMR_CHRP_Stress_Tension),strDetailTable));
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteStress_PrincipalComp  (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_Stress_PrincipalComp);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_Stress_PrincipalComp));
	strText += strTemp;
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Stress_PrincipalComp);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_PrincipalComp);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_Stress_PrincipalComp);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	if(!m_pDoc->IsPscJtg04())
	{
		return TRUE;
	}
	
	T_PSCD_D PscdD;
	PscdD.Initialize();
	
	strText = _T("");
	
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_PrincipalComp_Conditon),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_PrincipalComp_EQ));
	strText += strTemp;
	
	
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_PrincipalComp);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Explanation);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	BOOL bResultFlag = TRUE;
	
	double dAllow =  -1.0E+50;
	double dDesign = -1.0E+50;
	
	//////////////////////////////////////////////////////////////////////////
	T_ELEM_K ElemK;
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();
	
	CPostDrawAdapter drawAdapter(strDocPath);
	CString strDetailTable = _T("");
	
	CArray<UINT,UINT> arElemList;

	if(m_pDoc->IsPscJtg04())
	{
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		
		T_FPMS_JTG JtgD;
		std::map<T_ELEM_K,T_FPMS_JTG> mapJTG_datas;
		if(!CChinaReportSetting::GetChinaReportSetting()->GetOutPutElemList(arElemList))
		{
			ArrElemPairKey aElemPairK;
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFpmsJtgKeyList(aElemPairK);
			CDBLib::GetElemKeyList4ElemPairKey(aElemPairK, arElemList);
		}
		//m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFpmsJtgKeyList(arElemList);
		if(arElemList.GetSize() == 0)
			return TRUE;

		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			ElemPairK EPairK(ElemK, EN_EL_BEAM);
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFpmsJtgData(EPairK,  JtgD);
			ConvertUnit_FPMS(JtgD);
			mapJTG_datas.insert(make_pair(ElemK, JtgD));
			
			for (int i=0;i<4;i++)
			{
				if((i==0 || i==2))
				{
					if(bResultFlag && !JtgD.FpmsBase[i].bOK)
					{
						bResultFlag = FALSE;
						dAllow = JtgD.FpmsBase[i].dAFP;
						dDesign = JtgD.FpmsBase[i].dFMAX;
					}
					if(bResultFlag)
					{
						if(JtgD.FpmsBase[i].dFMAX > dDesign)
						{
							dAllow = JtgD.FpmsBase[i].dAFP;
							dDesign = JtgD.FpmsBase[i].dFMAX;
							
						}
					}
				}
			}
		}
		
		
		
		
		strDocPath = drawAdapter.Build_PSC_FPMS_Graphic(1,mapJTG_datas);
		strDetailTable = drawAdapter.Build_PSC_FPMS_Table(1,mapJTG_datas);
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_PrincipalComp);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
		genFtagStr.sType         = _LSX(IMG);
		genFtagStr.sFullPath     = strDocPath;
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = genFtagStr.sCaption;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T("");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		
		
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	
	CString strResultFlag = _T("");
	CString strResultChina = _T("");
	CString strResultYesNo = _T("");
	
	
	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_PrincipalComp_Conclusion),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	
	strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_PrincipalComp_Conclusion1),dDesign, strResultFlag,dAllow);
	
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Code_YesNo),strResultYesNo);
	strText += strTemp;
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_PrincipalComp);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Conclusion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	m_vctDetailTable.push_back(make_pair(_LS(IDS_AMR_CHRP_Stress_PrincipalComp),strDetailTable));
	
	return TRUE;
	
}
BOOL    CReportTreeText::WriteStress_ShortTerm      (GenSegmentElements& genElements) 
{
	CString strText = _LS(IDS_AMR_CHRP_Stress_ShortTerm);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_Stress_ShortTerm));
	strText += strTemp;
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Stress_ShortTerm);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_ShortTerm);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_Stress_ShortTerm);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	if(!m_pDoc->IsPscJtg04())
	{
		return TRUE;
	}
	
	T_PSCD_D PscdD;
	PscdD.Initialize();
	
	strText = _T("");
	
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_ShortTerm_Conditon),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_ShortTerm_EQ));
	strText += strTemp;
	
	
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_ShortTerm);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Explanation);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	BOOL bResultFlag = TRUE;
	
	double dAllow =  0;
	double dDesign = 0;
	
	//////////////////////////////////////////////////////////////////////////
	T_ELEM_K ElemK;
	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();
	
	CPostDrawAdapter drawAdapter(strDocPath);
	CString strDetailTable = _T("");
	
	CArray<UINT,UINT> arElemList;

	if(m_pDoc->IsPscJtg04())
	{
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		
		T_FYSG_D JtgD;
		std::map<T_ELEM_K,T_FYSG_D> mapJTG_datas;
		if(!CChinaReportSetting::GetChinaReportSetting()->GetOutPutElemList(arElemList))
		{
			ArrElemPairKey aElemPairK;
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFysgKeyList(aElemPairK);
			CDBLib::GetElemKeyList4ElemPairKey(aElemPairK, arElemList);
		}
		//m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFysgKeyList(arElemList);
		if(arElemList.GetSize() == 0)
			return TRUE;

		for (int i=0;i<arElemList.GetSize();i++)
		{
			ElemK = arElemList.GetAt(i);
			ElemPairK EPairK(ElemK, EN_EL_BEAM);
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFysgData(EPairK,  JtgD);
			ConvertUnit_FYSG(JtgD);
			mapJTG_datas.insert(make_pair(ElemK, JtgD));
			
			for (int i=0;i<4;i++)
			{
				if(bResultFlag && !JtgD.FysgBase[i].bOK)
				{
					bResultFlag = FALSE;
					dAllow = JtgD.FysgBase[i].dALW;
					dDesign = JtgD.FysgBase[i].dFMAX;
				}
				if(bResultFlag)
				{
					if(fabs(JtgD.FysgBase[i].dFMAX) > fabs(dDesign))
					{
						dDesign = JtgD.FysgBase[i].dFMAX;
						dAllow = JtgD.FysgBase[i].dALW;
					}
				}
			}
		}
		
		
		
		
		strDocPath = drawAdapter.Build_PSC_FYSG_Graphic(mapJTG_datas);
		strDetailTable = drawAdapter.Build_PSC_FYSG_Table(mapJTG_datas);
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_ShortTerm);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
		genFtagStr.sType         = _LSX(IMG);
		genFtagStr.sFullPath     = strDocPath;
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = genFtagStr.sCaption;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T("");
		genFtagStr.iLevel        = 0;
		
		genElements.m_SegmentElements.push_back(genFtagStr);
		
		
		
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	
	CString strResultFlag = _T("");
	CString strResultChina = _T("");
	CString strResultYesNo = _T("");
	
	
	strText = _T("");
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_ShortTerm_Conclusion),GetChinaCodeName(_T("JTG D62-2004")));
	strText += strTemp;
	
	strResultFlag = GetResultFlgString(bResultFlag,1,strResultChina,strResultYesNo);
	strTemp.Format(_LS(IDS_AMR_CHRP_Stress_ShortTerm_Conclusion1),dDesign, strResultFlag,dAllow);
	strText += strTemp;
	strTemp.Format(_LS(IDS_AMR_CHRP_Code_YesNo),strResultYesNo);
	strText += strTemp;
	
	
	
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Stress_ShortTerm);
	genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Conclusion);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = genFtagStr.sCaption;
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 0;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	m_vctDetailTable.push_back(make_pair(_LS(IDS_AMR_CHRP_Stress_ShortTerm),strDetailTable));
	
	return TRUE;
	
}

void CReportTreeText::WriteTitle(int iIndex,CString& strText)//iIndex --1  ,2  ,3   離뜩連넣힛섬깃痙
{
	if(iIndex <0 || iIndex > 3)
		return;
	
	CString strTemp;
	if(iIndex == 1)
	{
		strTemp.Format(_T("[DRG_FONT]|FC| size=18 bold=true align=center[/DRG_FONT]%s[DRG_FONT]|FI|[/DRG_FONT]"), strText);
		strText = strTemp;
	}
	else if(iIndex == 2)
	{
		strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=true align=left[/DRG_FONT]%s[DRG_FONT]|FI|[/DRG_FONT]"), strText);
		strText = strTemp;
	}
	else if(iIndex == 3)
	{
		strTemp.Format(_T("[DRG_FONT]|FC| size=14 bold=true align=left[/DRG_FONT]%s[DRG_FONT]|FI|[/DRG_FONT]"), strText);
		strText = strTemp;
	}
	else
	{
		
	}
	
}

CString CReportTreeText::GetNumberCharacterForChina( int iNumber )
{
	CString result = _T("");
	switch(iNumber)
	{
	case 0:
		result = _T("쥐");
		break;
	case 1:
		result = _T("寧");
		break;
	case 2:
		result = _T("랗");
		break;
	case 3:
		result = _T("힛");
		break;
	case 4:
		result = _T("愷");
		break;
	case 5:
		result = _T("巧");
		break;
	case 6:
		result = _T("짇");
		break;
	case 7:
		result = _T("펌");
		break;
	case 8:
		result = _T("검");
		break;
	case 9:
		result = _T("씽");
		break;
	default:
		result = _T("-");
		break;
	}
	return result;
}

BOOL CReportTreeText::WriteReportText_CH( const CString strTextItem,GenSegmentElements& SegmentElements)
{
	BOOL bRet = FALSE;
	//china report
	/** Add By JUHONGHUA(MITC) 2012-2-14 15:47:27**/
	if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_General))==0)
	{
		bRet = WriteBasicInfo_General(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Standard))==0)
	{
		bRet = WriteBasicInfo_Standard(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Code))==0)
	{
		bRet = WriteBasicInfo_Code(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Structure))==0)
	{
		bRet = WriteBasicInfo_Structure(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Material))==0)
	{
		bRet = WriteBasicInfo_Material(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_BasicInfo_Calculation))==0)
	{
		bRet = WriteBasicInfo_Calculation(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Model_Calculation))==0)
	{
		bRet = WriteModel_Calculation(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Model_Steel))==0)
	{
		bRet = WriteModel_Steel(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Model_Section))==0)
	{
		bRet = WriteModel_Section(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Model_Load))==0)
	{
		bRet = WriteModel_Load(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_ComDepth))==0)
	{
		bRet = WriteLoadCapacity_ComDepth(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_Bending))==0)
	{
		bRet = WriteLoadCapacity_Bending(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_Shearing))==0)
	{
		bRet = WriteLoadCapacity_Shearing(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_Torsion))==0)
	{
		bRet = WriteLoadCapacity_Torsion(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_LoadCapacity_Reforce))==0)
	{
		bRet = WriteLoadCapacity_Reforce(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_ServiceAbility_Normal))==0)
	{
		bRet = WriteServiceAbility_Normal(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_ServiceAbility_Oblique))==0)
	{
		bRet = WriteServiceAbility_Oblique(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Stress_NormalComp))==0)
	{
		bRet = WriteStress_NormalComp(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Stress_Tension))==0)
	{
		bRet = WriteStress_Tension(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Stress_PrincipalComp))==0)
	{
		bRet = WriteStress_PrincipalComp(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Stress_ShortTerm))==0)
	{
		bRet = WriteStress_ShortTerm(SegmentElements);
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_CHRP_Force_MemberForce))==0)
	{
		bRet = WriteStress_MemberForce(SegmentElements);
	}
	else
	{
		ASSERT(0); bRet = FALSE;
	}
	
	
	
	
	
	
	return bRet;
}



CString CReportTreeText::BuilderChapterTitle( int iChapterID,int iSegmentID,const CString& strText )
{
	CString strRet = strText;
	if(iChapterID == 0)
		return strRet;
	if(iSegmentID == 0)
	{
		strRet = GetNumberCharacterForChina(iChapterID) + _T("、") + strRet;
	}
	else
	{
		
		CString strPre = _T("");
		strPre.Format(_T("%d.%d"),iChapterID,iSegmentID);
		strRet = strPre + _T(" ") + strRet;
	}
	return strRet;
}

BOOL CReportTreeText::ReplaceTitileNumber( int iChapter,int iSegment,CString& strTexts )
{
	if(strTexts.Find(GENTITLEREPLACE) != -1)
	{
		CString strTitle = _T("");
		
		if(iChapter != 0 && iSegment != 0)
		{
			strTitle.Format(_T("%d.%d "),iChapter,iSegment);
			strTexts.Replace(GENTITLEREPLACE,strTitle);
		}
		else if(iChapter == 0 && iSegment == 0)
		{
			strTexts.Replace(GENTITLEREPLACE,strTitle);
		}
		else if(iChapter == 0)
		{
			strTitle.Format(_T("%d. "),iSegment);
			strTexts.Replace(GENTITLEREPLACE,strTitle);
		}
		else
			return FALSE;
	}
	else
		return FALSE;
	
	return TRUE;
}


// Inset Table Items (Special Tables - Section Summary)
void CReportTreeText::InsertSSTable(T_SECT_K sectKey,GenSegmentElements& genElements) 
{
	
	
	CArray<T_SECT_K, T_SECT_K> aSectK;
	aSectK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aSectK);
	int nSectNum = aSectK.GetSize();
	if(nSectNum<=0) 
	{ 
		ASSERT(0); return; 
	}
	
	T_SECT_K  SectK=sectKey;
	T_SECT_D  SectD;  SectD.Initialize();
	
	CString strFilePath=_T("");
	CString strImagePath=_T("");
	
	CStringArray  aName    ; // strName (추출정보이름)
	CStringArray  aCaption ; // Caption
	CStringArray  aFilePath; // *.xml Full Path
	CStringArray  aImagePath; // *.bmp Image Path
	aName.RemoveAll();
	aCaption.RemoveAll();
	aFilePath.RemoveAll();
	aImagePath.RemoveAll();
	
	
	
	T_RSCT_STIFF_D StiffInfo, AfterStiffInfo;
	
	int nType=0;  // section type
	CStringArray arImagePath;
	
	CDgnAmrSectionTb SectionTb;
	
	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	
	
	SectK = 0;
	SectD.Initialize();
	strImagePath=_T("");
	strFilePath=_T("");
	StiffInfo.Initialize();
	AfterStiffInfo.Initialize();
	nType = 0;
	arImagePath.RemoveAll();
	
	SectK = sectKey;
	if(!m_pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
		return;
	
	if(!m_pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo))
		return;
	
	nType = StiffInfo.nType;
	if(nType<1 || nType>7)
	{
		ASSERT(0); return;
	}
	CReportTableProcess temp;
	// set file path name
	CString strFilePathName=_T("");
	strFilePathName = temp.GetFilePath();
	CString strSName=_T("");
	strSName = SectD.SName;
	strSName.Replace(_T("\\"), _T(""));
	strSName.Replace(_T("/"),_T(""));
	strSName.Replace(_T(":"),_T(""));
	strSName.Replace(_T("*"),_T(""));
	strSName.Replace(_T("?"),_T(""));
	strSName.Replace(_T("\""),_T(""));
	strSName.Replace(_T("<"),_T(""));
	strSName.Replace(_T(">"),_T(""));
	strSName.Replace(_T("|"),_T(""));
	strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
	strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);
	
	// make section image
	MakeSectionFigure(strImagePath, SectK, nType, arImagePath);
	
	if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_SS, SectK, 0))
		return;
	
	CString strTemp=_T("");
	strTemp.Format(_T("%d : %s"), SectK, SectD.SName);
	aCaption.Add(strTemp);
	//aName.Add(ReplaceSymbolByTempChar(SectD.SName));
	strTemp.Format(_T("%d"), SectK);
	aName.Add(strTemp);
	aFilePath.Add(strFilePath);
	aImagePath.Add(strImagePath);
	
	
	aName.FreeExtra();
	aCaption.FreeExtra();
	aFilePath.FreeExtra();
	aImagePath.FreeExtra();
	
	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	if(nName<=0) return;
	
	int nItemCnt=0;
	nItemCnt = nName;
	
	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL
	
	
#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif
	
	CH_GEN_IFTAG_STR genFlagStr;
	
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = _LSX(TBL);
		sFullPath     = aFilePath[nItem];
		sExtInfo      = _ULS('SS ')+aName[nItem];
		sContents     = _T(" ");
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T(" ");
		
		genFlagStr.sProductInfo = (LPTSTR)(LPCTSTR)sProductInfo;
		genFlagStr.sCaption     = (LPTSTR)(LPCTSTR)sCaption    ;
		genFlagStr.sType        = (LPTSTR)(LPCTSTR)sType       ;
		genFlagStr.sFullPath    = (LPTSTR)(LPCTSTR)sFullPath   ;
		genFlagStr.sExtInfo     = (LPTSTR)(LPCTSTR)sExtInfo    ;
		genFlagStr.sContents    = (LPTSTR)(LPCTSTR)sContents   ;
		genFlagStr.sExt         = (LPTSTR)(LPCTSTR)sExt        ;
		genFlagStr.iAppendFlag  = iAppendFlag;
		genFlagStr.iLevel = 0 ;
		
		genElements.m_SegmentElements.push_back(genFlagStr);
	}
	
	
}

// Section Summary의 이미지 파일을 만든다.
void CReportTreeText::MakeSectionFigure(CString strImagePath, UINT SectK, int nType, CStringArray& arImagePath)
{
	arImagePath.RemoveAll();
	
	CDgnDrawSection DgnDrawSection;
	_DGN_DRAW_SECT_OPTION OptionD;
	OptionD.Initialize();
	OptionD.Canvas.SetRect(0,0,7500,7500);
	OptionD.DimSize = 500;
	
	if(nType==1 || nType==3 || nType==4)
	{
		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0/*int nIMJ*/, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
	}
	else if(nType==2)
	{
		strImagePath=_T("");
		arImagePath.RemoveAll();
	}
	else if(nType==5)
	{
		CString strTmp=_T(""), strTmp2=_T("");
		strTmp = strImagePath;
		strTmp.Replace(_T(".emf"), _T(""));
		strTmp2.Format(_T("%s_2.emf"), strTmp);
		
		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp2, SectK, 0, OptionD, FALSE, 1, TRUE);
		arImagePath.Add(strTmp2);
	}
	else if(nType==6)
	{
		CString strTmp=_T(""), strTmp2=_T("");
		strTmp = strImagePath;
		strTmp.Replace(_T(".emf"), _T(""));
		strTmp2.Format(_T("%s_2.emf"), strTmp);
		
		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp2, SectK, 2, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strTmp2);
	}
	else if(nType==7)
	{
		CString strTmp=_T(""), strTmp2=_T(""), strTmp3=_T(""), strTmp4=_T("");
		strTmp = strImagePath;
		strTmp.Replace(_T(".emf"), _T(""));
		strTmp2.Format(_T("%s_2.emf"), strTmp);
		strTmp3.Format(_T("%s_3.emf"), strTmp);
		strTmp4.Format(_T("%s_4.emf"), strTmp);
		
		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp2, SectK, 2, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strTmp2);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp3, SectK, 0, OptionD, FALSE, 1, TRUE);
		arImagePath.Add(strTmp3);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp4, SectK, 2, OptionD, FALSE, 1, TRUE);
		arImagePath.Add(strTmp4);
	}
	else
	{
		ASSERT(0);
	}
}

double CReportTreeText::GetU( double df )
{
	double uc = 0.0;
	if(df > 14.0)
	{
		uc = 0.45;
	}
	else if(df > 1.5)
	{
		uc = 0.1767*log(df) - 0.0157;
	}
	else
	{
		uc = 0.05;
	}
	return uc;
}

BOOL CReportTreeText::WriteDetailTables( GenSegmentElements& genElements )
{
	if(m_vctDetailTable.size() == 0 )
		return TRUE;
	
	CString strText = _LS(IDS_AMR_CHRP_Detail_Table);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_CHRP_Detail_Table));
	strText += strTemp;
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Detail_Table);
	CH_GEN_IFTAG_STR genFtagStr;
	
	int iIndexItem = 0;
	
	for (std::vector<pair<CString,CString> >::iterator pit = m_vctDetailTable.begin();
	pit != m_vctDetailTable.end();++pit)
	{
		if(CChinaReportSetting::GetChinaReportSetting()->IsExistItem(pit->first))
		{
			iIndexItem++;
			genFtagStr.sCaption = pit->first;
			genFtagStr.sCaption += _LS(IDS_AMR_CHRP_Detail_Table);
			genFtagStr.sType         = _LSX(TBL);
			genFtagStr.sFullPath     = pit->second;
			genFtagStr.sExtInfo      = genFtagStr.sCaption;
			genFtagStr.sContents     = _T("");
			genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
			genFtagStr.sExt          = _T("");
			genFtagStr.iLevel        = 0;
			
			genElements.m_SegmentElements.push_back(genFtagStr);
		}
	}
	return TRUE;
}

void CReportTreeText::ConvertUnit_BDCY( T_BDCY_JTG& _data )
{
	for(int i=0; i<4; i++)
	{
		_data.BdcyBase[i].dMuy = GetMomentUnitRatio(_data.BdcyBase[i].dMuy);
		_data.BdcyBase[i].drMuy = GetMomentUnitRatio(_data.BdcyBase[i].drMuy);
		_data.BdcyBase[i].dMny = GetMomentUnitRatio(_data.BdcyBase[i].dMny);
	}
}

void CReportTreeText::ConvertUnit_CBCS( T_CBCS_CH& _data )
{
	for(int i=0; i<4; i++)
	{
		_data.cbcsBase[i].dMuy = GetMomentUnitRatio(_data.cbcsBase[i].dMuy);
		_data.cbcsBase[i].drMuy = GetMomentUnitRatio(_data.cbcsBase[i].drMuy);
		_data.cbcsBase[i].dMny = GetMomentUnitRatio(_data.cbcsBase[i].dMny);
	}
}

void CReportTreeText::ConvertUnit_CUMS( T_CUMS_JTG& _data )
{
	
	
	for(int i=0; i<4; i++)
	{
		_data.CumsBase[i].dMu  = GetMomentUnitRatio(_data.CumsBase[i].dMu );
		_data.CumsBase[i].dMu_P = GetMomentUnitRatio(_data.CumsBase[i].dMu_P);
		_data.CumsBase[i].dVu  = GetForceUnitRatio(_data.CumsBase[i].dVu );
		_data.CumsBase[i].drVu  = GetForceUnitRatio(_data.CumsBase[i].drVu );
		_data.CumsBase[i].dVn  = GetForceUnitRatio(_data.CumsBase[i].dVn );
		_data.CumsBase[i].dShrSect  = GetForceUnitRatio(_data.CumsBase[i].dShrSect );
		_data.CumsBase[i].dShrLoad  = GetForceUnitRatio(_data.CumsBase[i].dShrLoad );
	}
}

void CReportTreeText::ConvertUnit_CSIS( T_CSIS_CH& _data )
{
	for(int i=0; i<4; i++)
	{
		_data.CsisBase[i].dMu  = GetMomentUnitRatio(_data.CsisBase[i].dMu );
		_data.CsisBase[i].dMu_P = GetMomentUnitRatio(_data.CsisBase[i].dMu_P);
		_data.CsisBase[i].dVu  = GetForceUnitRatio(_data.CsisBase[i].dVu );
		_data.CsisBase[i].drVu  = GetForceUnitRatio(_data.CsisBase[i].drVu );
		_data.CsisBase[i].dVn  = GetForceUnitRatio(_data.CsisBase[i].dVn );
		_data.CsisBase[i].dShrSect  = GetForceUnitRatio(_data.CsisBase[i].dShrSect );
		_data.CsisBase[i].dShrLoad  = GetForceUnitRatio(_data.CsisBase[i].dShrLoad );
	}
}

void CReportTreeText::ConvertUnit_CRMT( T_CRMT_JTG& _data )
{
	
	for(int i=0; i<6; i++)
	{
		_data.CrmtBase[i].dTu  = GetMomentUnitRatio(_data.CrmtBase[i].dTu );
		_data.CrmtBase[i].drTu  = GetMomentUnitRatio(_data.CrmtBase[i].drTu );
		_data.CrmtBase[i].dTn  = GetMomentUnitRatio(_data.CrmtBase[i].dTn );
		_data.CrmtBase[i].dMu  = GetMomentUnitRatio(_data.CrmtBase[i].dMu );
		_data.CrmtBase[i].dVu  = GetForceUnitRatio(_data.CrmtBase[i].dVu );
		_data.CrmtBase[i].drVu  = GetForceUnitRatio(_data.CrmtBase[i].drVu );
		_data.CrmtBase[i].dVn  = GetForceUnitRatio(_data.CrmtBase[i].dVn );
		_data.CrmtBase[i].dShrTorVal  = GetStressUnitRatio(_data.CrmtBase[i].dShrTorVal );
		_data.CrmtBase[i].dTorSect  = GetStressUnitRatio(_data.CrmtBase[i].dTorSect );
		_data.CrmtBase[i].dTorLoad  = GetStressUnitRatio(_data.CrmtBase[i].dTorLoad );
		
	}
}

void CReportTreeText::ConvertUnit_CTSS( T_CTSS_CH& _data )
{
	for(int i=0; i<6; i++)
	{
		_data.CtssBase[i].dTu  = GetMomentUnitRatio(_data.CtssBase[i].dTu );
		_data.CtssBase[i].drTu  = GetMomentUnitRatio(_data.CtssBase[i].drTu );
		_data.CtssBase[i].dTn  = GetMomentUnitRatio(_data.CtssBase[i].dTn );
		_data.CtssBase[i].dMu  = GetMomentUnitRatio(_data.CtssBase[i].dMu );
		_data.CtssBase[i].dVu  = GetForceUnitRatio(_data.CtssBase[i].dVu );
		_data.CtssBase[i].drVu  = GetForceUnitRatio(_data.CtssBase[i].drVu );
		_data.CtssBase[i].dVn  = GetForceUnitRatio(_data.CtssBase[i].dVn );
		_data.CtssBase[i].dShrTorVal  = GetStressUnitRatio(_data.CtssBase[i].dShrTorVal );
		_data.CtssBase[i].dTorSect  = GetStressUnitRatio(_data.CtssBase[i].dTorSect );
		_data.CtssBase[i].dTorLoad  = GetStressUnitRatio(_data.CtssBase[i].dTorLoad );
		
	}
}

void CReportTreeText::ConvertUnit_FYCM( T_FYCM_JTG& _data )
{
	
	for(int i=0; i<6; i++)
	{
		_data.FycmBase[i].dFT  = GetStressUnitRatio (_data.FycmBase[i].dFT );
		_data.FycmBase[i].dFB  = GetStressUnitRatio (_data.FycmBase[i].dFB );
		_data.FycmBase[i].dFTL  = GetStressUnitRatio(_data.FycmBase[i].dFTL );
		_data.FycmBase[i].dFBL  = GetStressUnitRatio(_data.FycmBase[i].dFBL );
		_data.FycmBase[i].dFTR  = GetStressUnitRatio(_data.FycmBase[i].dFTR );
		_data.FycmBase[i].dFBR  = GetStressUnitRatio(_data.FycmBase[i].dFBR );
		_data.FycmBase[i].dFMAX = GetStressUnitRatio(_data.FycmBase[i].dFMAX );
		_data.FycmBase[i].dALW  = GetStressUnitRatio(_data.FycmBase[i].dALW );
		
	}
}

void CReportTreeText::ConvertUnit_FPMS( T_FPMS_JTG& _data )
{
	for(int i=0; i<4; i++)
	{
		_data.FpmsBase[i].dFP1  = GetStressUnitRatio (_data.FpmsBase[i].dFP1  );
		_data.FpmsBase[i].dFP2  = GetStressUnitRatio (_data.FpmsBase[i].dFP2  );
		_data.FpmsBase[i].dFP3  = GetStressUnitRatio (_data.FpmsBase[i].dFP3  );
		_data.FpmsBase[i].dFP4  = GetStressUnitRatio (_data.FpmsBase[i].dFP4  );
		_data.FpmsBase[i].dFP5  = GetStressUnitRatio (_data.FpmsBase[i].dFP5  );
		_data.FpmsBase[i].dFP6  = GetStressUnitRatio (_data.FpmsBase[i].dFP6  );
		_data.FpmsBase[i].dFP7  = GetStressUnitRatio (_data.FpmsBase[i].dFP7  );
		_data.FpmsBase[i].dFP8  = GetStressUnitRatio (_data.FpmsBase[i].dFP8  );
		_data.FpmsBase[i].dFP9  = GetStressUnitRatio (_data.FpmsBase[i].dFP9  );
		_data.FpmsBase[i].dFP10 = GetStressUnitRatio (_data.FpmsBase[i].dFP10 );
		_data.FpmsBase[i].dFMAX = GetStressUnitRatio (_data.FpmsBase[i].dFMAX );
		_data.FpmsBase[i].dAFP  = GetStressUnitRatio (_data.FpmsBase[i].dAFP  );
	}
}

void CReportTreeText::ConvertUnit_FYPC( T_FYPC_JTG& _data )
{
	
	_data.dFDL1  = GetStressUnitRatio (_data.dFDL1 );
	_data.dFDL2  = GetStressUnitRatio (_data.dFDL2 );
	_data.dFLL   = GetStressUnitRatio (_data.dFLL  );
	_data.dAFDL1 = GetStressUnitRatio (_data.dAFDL1);
	_data.dAFDL2 = GetStressUnitRatio (_data.dAFDL2);
	_data.dAFLL  = GetStressUnitRatio (_data.dAFLL );
	
	
}

void CReportTreeText::ConvertUnit_FYSG( T_FYSG_D& _data )
{
	for(int i=0; i<4; i++)
	{
		_data.FysgBase[i].dFT  = GetStressUnitRatio (_data.FysgBase[i].dFT );
		_data.FysgBase[i].dFB  = GetStressUnitRatio (_data.FysgBase[i].dFB );
		_data.FysgBase[i].dFTL  = GetStressUnitRatio(_data.FysgBase[i].dFTL );
		_data.FysgBase[i].dFBL  = GetStressUnitRatio(_data.FysgBase[i].dFBL );
		_data.FysgBase[i].dFTR  = GetStressUnitRatio(_data.FysgBase[i].dFTR );
		_data.FysgBase[i].dFBR  = GetStressUnitRatio(_data.FysgBase[i].dFBR );
		_data.FysgBase[i].dFMAX = GetStressUnitRatio(_data.FysgBase[i].dFMAX );
		_data.FysgBase[i].dALW  = GetStressUnitRatio(_data.FysgBase[i].dALW );
		
	}
}

void CReportTreeText::ConvertUnit_REAC (T_REAC_D& _data)
{
	for(int i=0;i<3;i++)
	{
		_data.dblReac[i] = GetForceUnitRatio(_data.dblReac[i]);
	}
	for(int i=4;i<6;i++)
	{
		_data.dblReac[i] = GetMomentUnitRatio(_data.dblReac[i]);
	}
	
}
CString CReportTreeText::GetModelPictureByView()
{
	CString strPicPath = _T("");
	CString strFileName = _LS(IDS_AMR_CHRP_Model_Calculation_ModelPicture);
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	//   CString strPathName = pDoc->GetPathName();
	//   CFileCtrl FileCtrl(strPathName);
	//   CString strFilePathName=FileCtrl.GetFilePathAndNameWithoutExtension();
	//   int iIndex = strFilePathName.ReverseFind('\\');
	//   strPicPath = strFilePathName.Mid(0, iIndex+1)+strFileName+".bmp";
	
	T_UFIG_K UfigK=0;
	T_UFIG_D UfigD;
	
	UfigD.strName   = strFileName   ;
	UfigD.bBW       = FALSE  ;
	UfigD.bPre      = !pDoc->IsPostMode();
	
	// 동일한 이름이 있으면 여기서 무조건 return, 다음단계 진행하지 않게 함. 20110208 KIMJM
	//   if(pDoc->m_pAttrCtrl->ExistUfig(strFileName))
	//   { 
	//     
	//     return _T("");
	//   }
	
	if(pDoc->m_pStagCtrl->IsBaseStag() || pDoc->m_pStagCtrl->IsFinalStag())
	{
		UfigD.nStagStatus = 0;
		UfigD.CurStagK = pDoc->m_pStagCtrl->GetCurStag();
	}
	else if(pDoc->m_pStagCtrl->IsMinMaxStag())
	{
		UfigD.nStagStatus = 2;
		UfigD.CurStagK = pDoc->m_pStagCtrl->GetCurStag();
	}
	else if(pDoc->m_pStagCtrl->IsNormalStag())
	{
		UfigD.nStagStatus = 1;
		UfigD.CurStagK = pDoc->m_pStagCtrl->GetCurStag();
	}
	else
	{
		ASSERT(0);
	}
	
	CGRAMRUDFData   PreAmrUdf;
	CGPSAMRUDFData  PostAmrUdf;
	
	PreAmrUdf.GetData(UfigD.PrePrudD, UfigD.strName);
	if(pDoc->IsPostMode())
		PostAmrUdf.GetData(UfigD.PostFrsiD);
	if(pDoc->m_pAttrCtrl->ExistUfig(UfigD.strName))
		UfigK = pDoc->m_pDataCtrl->ModifyUfig(UfigD.strName,UfigD);
	else
		UfigK = pDoc->m_pDataCtrl->AddUfig(UfigD);
	
	
	if(!CGPSAMRUtil::GetInstance()->GetMakeFigureAndPath(UfigK, strPicPath))
		return _T("");
	
	return strPicPath;
	
}

BOOL CReportTreeText::WriteStress_MemberForce( GenSegmentElements& genElements )
{
	CString strText = _LS(IDS_AMR_CHRP_Force_MemberForce);
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), GENTITLEREPLACE + _LS(IDS_AMR_CHRP_Force_MemberForce));
	strText += strTemp;
	genElements.m_strSegmentName = _LS(IDS_AMR_CHRP_Force_MemberForce);
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Force_MemberForce);
	genFtagStr.sType         = _LSX(TXT);
	genFtagStr.sFullPath     = _T("");
	genFtagStr.sExtInfo      = _LS(IDS_AMR_CHRP_Force_MemberForce);
	genFtagStr.sContents     = strText;
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T("");
	genFtagStr.iLevel        = 2;
	
	genElements.m_SegmentElements.push_back(genFtagStr);
	
	if(!m_pDoc->IsPostMode())
		 return TRUE;
	CArray<T_ELEM_K, T_ELEM_K> ValueElemKeyList;

	CArray<sLoadCombIDType,sLoadCombIDType> _StandradLoadCombKey;
	CChinaReportSetting::GetChinaReportSetting()->GetStandradLoadCombKey(_StandradLoadCombKey);
	T_LCOM_D rCreateData;
	rCreateData.Initialize();

	CReportTableProcess temp;
	CString strDocPath = temp.GetFilePath();  
	CPostDrawAdapter drawAdapter(strDocPath);
	
	CChinaReportSetting::GetChinaReportSetting()->GetOutPutElemList(ValueElemKeyList);
 
	for (int j=0;j<_StandradLoadCombKey.GetSize();j++)
	{
		sLoadCombIDType& loadcmbtype = _StandradLoadCombKey.GetAt(j);
		if(loadcmbtype.m_bLoadCase)
		{
			if(loadcmbtype.m_LoadCaseType == LCT_MovingloadType)
			{
				if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_MOVING,loadcmbtype.m_iKeyID,rCreateData))continue;
			}
			else if(loadcmbtype.m_LoadCaseType == LCT_SettlementType)
			{
				if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_SETTLE,loadcmbtype.m_iKeyID,rCreateData))continue;
			}
			else
			{
				if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STATIC,loadcmbtype.m_iKeyID,rCreateData))continue;
			}
		}
		else
		{
			if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE,loadcmbtype.m_iKeyID,rCreateData))continue;
		}
		rCreateData.LoadCombName = loadcmbtype.m_strLoadCombName;
		CArray<T_STRB_D, T_STRB_D&> ForceDataList;
		T_STRB_D rStrbdata;

		for (int i = 0 ; i<ValueElemKeyList.GetSize();i++)
		{
			rStrbdata.Initialize();
			m_pDoc->m_pPostCtrl->SelectLoadCombNew(rCreateData);
			if(loadcmbtype.m_LiveLoadType == LiveLoadType_ALL)
				if(!m_pDoc->m_pPostCtrl->GetStrbNew(ValueElemKeyList.GetAt(i), 0, 0, &rStrbdata,FALSE))continue;
			else  if(loadcmbtype.m_LiveLoadType == LiveLoadType_MAX)
				 if(!m_pDoc->m_pPostCtrl->GetStrbNew(ValueElemKeyList.GetAt(i), &rStrbdata, 0, 0,FALSE))continue;
			else  if(loadcmbtype.m_LiveLoadType == LiveLoadType_MIN)
				 if(!m_pDoc->m_pPostCtrl->GetStrbNew(ValueElemKeyList.GetAt(i), 0, &rStrbdata, 0,FALSE))continue;

			ForceDataList.Add(rStrbdata);
		}
		CString strDetailTable = _T("");
		strDetailTable = drawAdapter.Build_Bearing_Force_Graphic(ValueElemKeyList,ForceDataList,loadcmbtype.m_strLoadCombName,5);
		
		
		
		genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Force_MemberForce);
		CString strWEnd = _T("");
		strWEnd.Format(_T(" %d"),j+1);
		genFtagStr.sCaption += _LS(IDS_AMR_CHRP__ResultGraphic);
		genFtagStr.sCaption +=  strWEnd;

		//genFtagStr.sCaption += rCreateData.LoadCombName /*+_T(":")+ strForceType[5-1]*/ ;
		genFtagStr.sType         = _LSX(IMG);
		genFtagStr.sFullPath     = strDetailTable;
		genFtagStr.sExtInfo      = genFtagStr.sCaption;
		genFtagStr.sContents     = genFtagStr.sCaption;
		genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
		genFtagStr.sExt          = _T("");
		genFtagStr.iLevel        = 0;
		
			genElements.m_SegmentElements.push_back(genFtagStr);
	}

	
	

	
	/*
	for (int j=0;j<ValueElemKeyList.GetSize();j++)  
	{
		T_STRB_D ReacMax;
		ReacMax.Initialize();
		BOOL bValueNode = TRUE;
		for(int k = 0 ; k<BeamLoadCombDataList.GetSize();k++)
		{
			T_REAC_D Reac;
			Reac.Initialize();
			m_pDoc->m_pPostCtrl->SelectLoadCombNew(BeamLoadCombDataList.GetAt(k));
			m_pDoc->m_pPostCtrl->GetStrbNew(ValueElemKeyList.GetAt(j), 0, 0, &Reac,TRUE);
			
			for (int m = 0; m < 6; m++)
			{  if (Reac.dblReac[m] != 0.0) break;}
			if (m != 6) 
			{
				ReacMax.AbsMaxwithSign(ReacMax,Reac);
			}
			else
			{ bValueNode = FALSE ; break;}
			
		}
		if(bValueNode)
		{
			ConvertUnit_REAC(ReacMax);
			ReacDataList.Add(ReacMax);
			ValueNodeKeyList.Add(rNodeKeyList.GetAt(j));
		}
	}
	*/
	

	
	
	return TRUE;
}

void CReportTreeText::ConvertUnit_Force( T_STRB_D& _data )
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<3;j++)
		{
			_data.dblForce[i][j] = GetForceUnitRatio(_data.dblForce[i][j]);
			_data.dblForce[i][j+6] = GetForceUnitRatio(_data.dblForce[i][j+6]);

			_data.dblForce[i][j+3] = GetMomentUnitRatio(_data.dblForce[i][j+3]);
			_data.dblForce[i][j+9] = GetMomentUnitRatio(_data.dblForce[i][j+9]);

		}
	}
	
}
