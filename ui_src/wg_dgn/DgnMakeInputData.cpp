// DgnMakeInputData.cpp: implementation of the CDgnMakeInputData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnMakeInputData.h"
#include "DgnDataCtrl.h"
#include "DgnForceCtrl.h"

#include "..\wg_base\wg_base_AppBase.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#include "..\wg_dgn\wg_dgn_DgnProgressDlg.h"

#if _MSC_VER >= 1700  // vc++11.0 
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include "..\MIT_Lib\rptostream.h"	// for rptwofstream
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnMakeInputData::CDgnMakeInputData()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_strPath = _T("");
	m_pForceCtrl = new CDgnForceCtrl;
	
	m_iMembType = 0;
}

CDgnMakeInputData::CDgnMakeInputData(int iLcomType)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_iLcomType = iLcomType;
	m_strPath = _T("");
	m_pForceCtrl = new CDgnForceCtrl;
}

CDgnMakeInputData::~CDgnMakeInputData()
{
	if(m_pForceCtrl!=NULL) 
	{ 
		delete m_pForceCtrl;
		m_pForceCtrl = NULL;
	}
}

void CDgnMakeInputData::SetLcomType(int iLcomType)
{
	m_iLcomType = iLcomType;
}

BOOL CDgnMakeInputData::SetCommandCtrl(int iCommand, int iOption)
{
	if(m_pDoc == NULL) m_pDoc = CDBDoc::GetDocPoint();
	
	if (iCommand == 100) {
		m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
	}
	else if (iCommand == 101) {
		CArray<T_ELEM_K, T_ELEM_K> ElemKey;
		m_pDoc->m_pViewCtrl->GetAllSelectedElem(ElemKey);
		
		ElemKey.Add(iOption);
		m_pDoc->m_pViewCtrl->SelectElem(NULL, ElemKey, FALSE);
	}
	else if (iCommand == 201) {//벽체의 WID
		//기존에 선택된것.
		CArray<T_ELEM_K, T_ELEM_K> ElemKey;
		m_pDoc->m_pViewCtrl->GetAllSelectedElem(ElemKey);
		
		//새로 추가하는것.
		CArray<unsigned int, unsigned int> arInternalWallIdList;
		m_pDoc->m_pPostCtrl->GetInternalWallIdList(iOption, arInternalWallIdList);
		
		T_WALL_KEY WallKey;
		CArray<T_ELEM_K, T_ELEM_K> arElemList;
		int iSize = arInternalWallIdList.GetSize();
		for (int i=0; i<iSize; i++) {
			m_pDoc->m_pPostCtrl->GetWallKey(arInternalWallIdList[i], WallKey);
			arElemList.RemoveAll();
			m_pDoc->m_pPostCtrl->GetWallElemList(WallKey, arElemList);
			
			int iNum = arElemList.GetSize();
			for (int k=0; k<iNum; k++) {
				ElemKey.Add(arElemList[k]);
			}
		}
		
		m_pDoc->m_pViewCtrl->SelectElem(NULL, ElemKey, FALSE);
	}
	else if (iCommand == 301) {//Memb별 선택.
		//기존에 선택된것.
		CArray<T_ELEM_K, T_ELEM_K> ElemKey;
		m_pDoc->m_pViewCtrl->GetAllSelectedElem(ElemKey);
		
		//새로 추가하는것.
		CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
		
		BOOL bReverseDir=FALSE;
		T_ELEM_K ShowKey;
		CArray<T_ELEM_K,T_ELEM_K> arElemList;
		pMembCtrl->GetElemListByIncludeElem(iOption, bReverseDir, ShowKey, arElemList);
		
		int iNum = arElemList.GetSize();
		for (int k=0; k<iNum; k++) {
			ElemKey.Add(arElemList[k]);
		}
		
		m_pDoc->m_pViewCtrl->SelectElem(NULL, ElemKey, FALSE);
	}
	else if (iCommand == 1000) {
		m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
	}
	else if (iCommand == 1001) {
		CArray<T_NODE_K, T_NODE_K> NodeKey;
		m_pDoc->m_pViewCtrl->GetAllSelectedNode(NodeKey);
		
		NodeKey.Add(iOption);
		m_pDoc->m_pViewCtrl->SelectNode(NULL, NodeKey);
	}
	
	return TRUE;
}

BOOL CDgnMakeInputData::WriteInputData4Dgn(int iMembType, HWND pWndSet)
{//iMembType=0:All, 1:Column, 2:Brace, 3:Wall, 4:Beam, 91~99절점반력.
	CFileException FileException;
	m_iMembType = iMembType;
	
	int iBufferLength = 256;
	TCHAR strBuffer[256];
	int iRand = rand();
	
	GetTempPath(iBufferLength, strBuffer);
	iBufferLength = (int)lstrlen(strBuffer);
	if (strBuffer[iBufferLength-1] == '\\') m_strPath.Format(_T("%s%08i.TMP"), strBuffer, iRand);
	else m_strPath.Format(_T("%s\\%08i.TMP"), strBuffer, iRand);
	
	TRY
	{
		if (!m_cfile.Open(m_strPath, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);

		if (iMembType >= 91) Write_Node_Data(iMembType);
		else Write_Data();

		m_cfile.Close();
		
		if (pWndSet != NULL) {
			UINT WM_DESIGNPRINT_READ = RegisterWindowMessage(_T("DMSG_DESIGNREAD_SET"));
			::PostMessage(pWndSet, WM_DESIGNPRINT_READ, (WPARAM)iRand, (LPARAM)iMembType);	
		}
	}
	CATCH(CFileException, e)
	{
		e->ReportError();
		m_cfile.Abort();
		CFileStatus status;
		if(CFile::GetStatus(m_strPath, status))CFile::Remove(m_strPath);
	m_strPath.Empty();

		return FALSE;
	}
	END_CATCH
		
		return TRUE;
}

void CDgnMakeInputData::Write_Data()
{
	if(m_pDoc == NULL) m_pDoc = CDBDoc::GetDocPoint();

	if (m_iMembType == 51) m_iLcomType = D_LCOMTYPE_STEEL;
	else if (m_iMembType == 52) m_iLcomType = D_LCOMTYPE_CONCRETE;
	else if (m_iMembType == 53) m_iLcomType = D_LCOMTYPE_SRC;
	else if (m_iMembType == 54) m_iLcomType = D_LCOMTYPE_FDN;
	
	if (m_iMembType > 50) m_iMembType = 0;//전체.
	
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;//D_UNITSYS_LENGTH_INDEX_CM;
	CngIndex.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;//D_UNITSYS_FORCE_INDEX_TON;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data를 받아올때는 Code Unit기준
	//////////////////////////////////////////////
	
	COleDateTime CurrentTime;
	CurrentTime = COleDateTime::GetCurrentTime();
	int Year  = CurrentTime.GetYear();
	int Month = CurrentTime.GetMonth();
	int Day   = CurrentTime.GetDay();
	CString strDate=_T("");
	strDate.Format(_T("%d/%d/%d"), Year, Month, Day);
	
	CString strProdNa = D_PRODUCT_NAME;
	Write_String(_T("!=============================================\n"));
	Write_String(_T("!  Input Data for DESIGN in ")+strProdNa+_T("\n"));
	Write_String(_T("!  Date : ")+strDate+_T("\n"));
	Write_String(_T("! \n"));
	Write_String(_T("!  !: comment \n"));
	Write_String(_T("!  *: command \n"));
	Write_String(_T("!  $: variable comment \n"));
	Write_String(_T("!=============================================\n"));
	
	// ####################################
	// INFO
	Write_String(_T("\n*INFO \n"));
	Write_String(_T("VER, 1000, Gen \n"));//Format Version
	Write_String(_T("ELEM-TYPE, 1 \n"));// 0:Element,  1:Member
	Write_String(_T("LOAD-TYPE, 1 \n"));// 0:LoadCase, 1:LoadComb
	
	// ####################################
	// UNIT
	Write_String(_T("\n*UNIT \n"));
	Write_String(_T("! FORCE, LENGTH \n"));
	Write_String(_T("! FORCE  : 1:KGF, 2:TONF, 3:N, 4:KN, 5:LBF, 6:KIP \n"));
	Write_String(_T("! LENGTH : 1:MM, 2:CM, 3:M, 4:IN, 5:FT \n"));
	Write_String(_T("  3,  3 \n"));
	
	// ####################################
	// LOADCASE
	WriteLoadCase();
	// LOADCOMB
	WriteLoadComb();
	// STORY
	WriteStoryData();
	// MATERIAL
	WriteMaterialData();
	// SECTION
	WriteSectData();
	// THICK
	WriteThickData();
	
	
	// Selected Member Lists
	m_pForceCtrl->Set_LcomDataForDesign(m_iLcomType, FALSE);
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	m_pForceCtrl->m_pDgnDataCtrl->m_pMembCtrl->GetElemListForDgn(FALSE,aElemList);
	T_ELEM_D ElemD; ElemD.Initialize();
	for(int i=0; i<aElemList.GetSize(); i++) 
	{
		T_ELEM_K ElemK = aElemList[i];
		// Get Element Data
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
		// Beam or Truss
		if(ElemD.eltyp == BEAM_EL || ElemD.eltyp == TRUSS_EL) 
		{
			// Get Member Type
			int MembType = m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenMbtp(ElemK);
			if     (MembType == D_MBTP_BEAM)		m_arBeamList.Add(ElemK);
			else if(MembType == D_MBTP_COLUMN)	m_arColumnList.Add(ElemK);
			else if(MembType == D_MBTP_BRACE)		m_arBraceList.Add(ElemK);
		}
	}
	
	// Get Wall Data
	m_pForceCtrl->m_pDgnDataCtrl->m_pMembCtrl->GetElemListForDgn(TRUE, m_arWallList);
	// FORCE
	WriteForceData();
	
	Write_String(_T("\n*END \n"));
	
	//////////////////////////////////////////////
	// <Remember> Data를 받고나서는 User Unit기준
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	// <Remember> Data를 받고나서는 User Unit기준
	//////////////////////////////////////////////
}

void CDgnMakeInputData::WriteLoadCase()
{
	// Static Load-Case 리스트 불러오기.
	CString strFout=_T("");
	m_arLcaseList.RemoveAll();
	_LCASE_DATA LcaseData;
	CArray<CString,CString&> arLCaseList;
	arLCaseList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetStldList(arLCaseList);
	
	// ####################################
	// STATIC LOADCASE
	strFout.Format(_T("\n*STLOADCASE, %i \n"), arLCaseList.GetSize());
	Write_String(strFout);
	Write_String(_T("! iLC, NAME, LCTYPE \n"));
	
	for(int i=0; i<arLCaseList.GetSize(); i++) {
		CString strLcaseName = arLCaseList[i];
		T_STLD_D LcaseD;
		LcaseD.Initialize();
		m_pDoc->m_pAttrCtrl->GetStld(strLcaseName, LcaseD);
		strFout.Format(_T("  %2d, %5s, %3s"), LcaseD.LoadCaseId, strLcaseName, LcaseD.LoadCaseType);
		Write_String(strFout+_T("\n"));
		
		// 전체 LoadCase 저장하기.
		LcaseData.Initialize();
		LcaseData.nAnalType = D_LOADCASE_STATIC;
		LcaseData.nLcaseKey = LcaseD.LoadCaseId;
		m_arLcaseList.Add(LcaseData);
	}
	// ####################################
	// RESPONSE SPECTRUM LOADCASE
	arLCaseList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSplcList(arLCaseList);
	if(arLCaseList.GetSize() != 0) {
		strFout.Format(_T("\n*SPLOADCASE, %i \n"), arLCaseList.GetSize());
		Write_String(strFout);
		Write_String(_T("! iLC, NAME, SCALE \n"));
		
		for(int i=0; i<arLCaseList.GetSize(); i++) {
			CString strLcaseName = arLCaseList[i];
			T_SPLC_D LcaseD;
			LcaseD.Initialize();
			m_pDoc->m_pAttrCtrl->GetSplc(strLcaseName, LcaseD);
			strFout.Format(_T("  %2d, %5s, %g"), LcaseD.LoadCaseId, strLcaseName, LcaseD.dblFactor);
			Write_String(strFout+_T("\n"));
			// 전체 LoadCase 저장하기.
			LcaseData.Initialize();
			LcaseData.nAnalType = D_LOADCASE_SPECTRUM;
			LcaseData.nLcaseKey = LcaseD.LoadCaseId;
			m_arLcaseList.Add(LcaseData);
		}
	}
	// ####################################
	// TIME HISTORY LOADCASE
	arLCaseList.RemoveAll();
#ifdef _MEC
	m_pDoc->GetThisListAppendDummyInit(arLCaseList);
#else
	m_pDoc->m_pAttrCtrl->GetThisList(arLCaseList);
#endif
	if(arLCaseList.GetSize() != 0) {
		strFout.Format(_T("\n*THLOADCASE, %i \n"), arLCaseList.GetSize());
		Write_String(strFout);
		Write_String(_T("! iLC, NAME \n"));
		
		for(int i=0; i<arLCaseList.GetSize(); i++) {
			CString strLcaseName = arLCaseList[i];
			T_THIS_D LcaseD;
			LcaseD.Initialize();
			m_pDoc->m_pAttrCtrl->GetThis(strLcaseName, LcaseD);
			strFout.Format(_T("  %2d, %5s"), LcaseD.LoadCaseId, strLcaseName);
			Write_String(strFout+_T("\n"));
			// 전체 LoadCase 저장하기.
			LcaseData.Initialize();
			LcaseData.nAnalType = D_LOADCASE_HISTORY;
			LcaseData.nLcaseKey = LcaseD.LoadCaseId;
			m_arLcaseList.Add(LcaseData);
		}
	}
	// ####################################
	// MOVING LOADCASE
	arLCaseList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvldList(arLCaseList);
	if(arLCaseList.GetSize() != 0) {
		strFout.Format(_T("\n*MVLOADCASE, %i \n"), arLCaseList.GetSize());
		Write_String(strFout);
		Write_String(_T("! iLC, NAME, SCALE \n"));
		
		for(int i=0; i<arLCaseList.GetSize(); i++) {
			CString strLcaseName = arLCaseList[i];
			T_MVLD_D LcaseD;
			LcaseD.Initialize();
			m_pDoc->m_pAttrCtrl->GetMvld(strLcaseName, LcaseD);
			strFout.Format(_T("  %2d, %5s, %g"), LcaseD.LoadCaseId, strLcaseName, LcaseD.dScaleFactor);
			Write_String(strFout+_T("\n"));
			// 전체 LoadCase 저장하기.
			LcaseData.Initialize();
			LcaseData.nAnalType = D_LOADCASE_MOVING;
			LcaseData.nLcaseKey = LcaseD.LoadCaseId;
			m_arLcaseList.Add(LcaseData);
		}
	}
	// ####################################
	// SETTLEMENT LOADCASE
	arLCaseList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSmlcList(arLCaseList);
	if(arLCaseList.GetSize() != 0) {
		strFout.Format(_T("\n*SMLOADCASE, %i \n"), arLCaseList.GetSize());
		Write_String(strFout);
		Write_String(_T("! iLC, NAME, SCALE \n"));
		
		for(int i=0; i<arLCaseList.GetSize(); i++) {
			CString strLcaseName = arLCaseList[i];
			T_SMLC_D LcaseD;
			LcaseD.Initialize();
			m_pDoc->m_pAttrCtrl->GetSmlc(strLcaseName, LcaseD);
			strFout.Format(_T("  %2d, %5s, %g"), LcaseD.LoadCaseId, strLcaseName, LcaseD.dFactor);
			Write_String(strFout+_T("\n"));
			// 전체 LoadCase 저장하기.
			LcaseData.Initialize();
			LcaseData.nAnalType = D_LOADCASE_SETTLE;
			LcaseData.nLcaseKey = LcaseD.LoadCaseId;
			m_arLcaseList.Add(LcaseData);
		}
	}
}

void CDgnMakeInputData::WriteLoadComb()
{
	// Load-Combination 리스트 불러오기.
	CString strFout=_T("");
	strFout.Empty();
	// Change by ZINU.('02.2.15). To Speed up (CString -> T_LCOM_K).
	CArray<T_LCOM_K, T_LCOM_K> arLcombList;
	arLcombList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(m_iLcomType, arLcombList);
	
	// ####################################
	// LOADCOMB
	strFout.Format(_T("\n*LOADCOMB, %i \n"), arLcombList.GetSize());
	Write_String(strFout);
	Write_String(_T("! iLCB, NAME, [DATA], ... \n"));
	Write_String(_T("! [DATA] : ANALTYPE, LCNAME, FACT, bActive \n"));
	for(int i=0; i<arLcombList.GetSize(); i++) {
		T_LCOM_D LcomD;
		LcomD.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(m_iLcomType, arLcombList[i], LcomD);

		 // Change by ZINU.('02.2.15). T_LCOM_D -> T_LCOM_D_UL
		T_LCOM_D_UL LcomUlData;
		LcomUlData.Initialize();
		m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomD, LcomUlData);
	 
		strFout.Format(_T("  %2d, %s"), LcomUlData.LoadCombId, LcomUlData.LoadCombName);
		// Load Combination 내에 포함된 Load Case 개수를 얻는다.
		int nLcaseNum = LcomUlData.Combination.GetSize();
		// Load Combination 내에 포함된 Load Case를 분리하여 Factor를 얻는다.
		for(int LcaseNo=0; LcaseNo<nLcaseNum; LcaseNo++) {
			unsigned int iAnalType = LcomUlData.Combination[LcaseNo].AnalType;
			double dFactor = LcomUlData.Combination[LcaseNo].Factor;
			CString strLcnameFact=_T("");
			
			if(iAnalType==D_LCOM_STATIC) {
				T_STLD_D LcaseD;
				LcaseD.Initialize();  // Get Load Case Type(D,L,W,E,...).
				m_pDoc->m_pAttrCtrl->GetStld(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
				strLcnameFact.Format(_T(", ST, %5s, %g"), LcaseD.LoadCaseName, dFactor);
			}
			else if(iAnalType==D_LCOM_SPECTRUM || iAnalType==D_LCOM_ECCENSPEC) {
				T_SPLC_D LcaseD;
				LcaseD.Initialize();
				m_pDoc->m_pAttrCtrl->GetSplc(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
				strLcnameFact.Format(_T(", SP, %5s, %g"), LcaseD.LoadCaseName, dFactor);
			}
			else if(iAnalType==D_LCOM_HISTORY) {
				T_THIS_D LcaseD;
				LcaseD.Initialize();
				m_pDoc->m_pAttrCtrl->GetThis(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
				strLcnameFact.Format(_T(", TH, %5s, %g"), LcaseD.LoadCaseName, dFactor);
			}
			else if(iAnalType==D_LCOM_MOVING) {
				// 코드에 따른 방식으로 수정 
				T_MVCD_D DataMvcd;
				if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
				switch (DataMvcd.nCodeType)
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
				case D_MOVE_CODE_NEWZEALAND:
				case D_MOVE_CODE_BRAZIL:
					{
						T_MVLD_D LcaseD;
						LcaseD.Initialize();
						m_pDoc->m_pAttrCtrl->GetMvld(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
						strLcnameFact.Format(_T(", MV, %5s, %g"), LcaseD.LoadCaseName, dFactor);
					}
					break;
				case D_MOVE_CODE_CHINA:
					{
						T_MVLDch_D LcaseD;
						LcaseD.Initialize();
						m_pDoc->m_pAttrCtrl->GetMvldch(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
						strLcnameFact.Format(_T(", MV, %5s, %g"), LcaseD.LoadCaseName, dFactor);
					}
					break;
				case D_MOVE_CODE_JAPAN:
					{
						T_MVLDjp_D LcaseD;
						m_pDoc->m_pAttrCtrl->InitializeMvldjp(LcaseD);
						m_pDoc->m_pAttrCtrl->GetMvldjp(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
						strLcnameFact.Format(_T(", MV, %5s, %g"), LcaseD.LoadCaseName, dFactor);
					}
					break;
				case D_MOVE_CODE_INDIA:
					{
						T_MVLDid_D LcaseD;
						LcaseD.Initialize();
						m_pDoc->m_pAttrCtrl->GetMvldid(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
						strLcnameFact.Format(_T(", MV, %5s, %g"), LcaseD.LoadCaseName, dFactor);
					}
					break;
				case D_MOVE_CODE_BS:
					{
						T_MVLDbs_D LcaseD;
						LcaseD.Initialize();
						m_pDoc->m_pAttrCtrl->GetMvldbs(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
						strLcnameFact.Format(_T(", MV, %5s, %g"), LcaseD.LoadCaseName, dFactor);
					}
					break;
				case D_MOVE_CODE_FRANCE:
					{
						T_MVLDfr_D LcaseD;
						LcaseD.Initialize();
						m_pDoc->m_pAttrCtrl->GetMvldfr(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
						strLcnameFact.Format(_T(", MV, %5s, %g"), LcaseD.LoadCaseName, dFactor);
					}
					break;
				case D_MOVE_CODE_TRANS:
					{
						T_MVLDtr_D LcaseD;
						LcaseD.Initialize();
						m_pDoc->m_pAttrCtrl->GetMvldtr(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
						strLcnameFact.Format(_T(", MV, %5s, %g"), LcaseD.LoadCaseName, dFactor);
					}
					break;
				default:
					ASSERT(0);
					break;
				}
			}
			else if(iAnalType==D_LCOM_SETTLEMENT) {
				T_SMLC_D LcaseD;
				LcaseD.Initialize();
				m_pDoc->m_pAttrCtrl->GetSmlc(LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseD);
				strLcnameFact.Format(_T(", SM, %5s, %g"), LcaseD.LoadCaseName, dFactor);
			}
			strFout += strLcnameFact;
		}
		 Write_String(strFout);
	 
		strFout.Format(_T(", %2d \n"), LcomUlData.nActive);
		Write_String(strFout);
	}
}

void CDgnMakeInputData::WriteLoadComb_Description()
{
	// Load-Combination 리스트 불러오기.
	CString strFout=_T("");
	strFout.Empty();
	// Change by ZINU.('02.2.15). To Speed up (CString -> T_LCOM_K).
	CArray<T_LCOM_K, T_LCOM_K> arLcombList;
	arLcombList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(m_iLcomType, arLcombList);
	
	// ####################################
	// LOADCOMB
	strFout.Format(_T("\n*LOADCOMB, %i \n"), arLcombList.GetSize());
	Write_String(strFout);
	Write_String(_T("! iLCB, NAME, Description, bActive \n"));
	for(int i=0; i<arLcombList.GetSize(); i++) {
		T_LCOM_D LcomD;
		LcomD.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(m_iLcomType, arLcombList[i], LcomD);
		
		// Change by ZINU.('02.2.15). T_LCOM_D -> T_LCOM_D_UL
		T_LCOM_D_UL LcomUlData;
		LcomUlData.Initialize();
		m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomD, LcomUlData);

		strFout.Format(_T("  %2d, %s, %s, %2d "), 
			LcomUlData.LoadCombId, LcomUlData.LoadCombName, LcomUlData.Description, LcomUlData.nActive);

		Write_String(strFout+_T("\n"));
	}
}

void CDgnMakeInputData::WriteStoryData()
{
	// Story 리스트 불러오기.
	CString strFout=_T("");
	CArray<T_STOR_K, T_STOR_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(arKeyList);
	
	// ####################################
	// STORY
	strFout.Format(_T("\n*STORY, %i \n"), arKeyList.GetSize());
	Write_String(strFout);
	Write_String(_T("! iSTR, NAME, LEVEL \n"));
	T_STOR_D storD;
	for(int i=0; i<arKeyList.GetSize(); i++) {
		storD.Initialize();
		m_pDoc->m_pAttrCtrl->GetStor(arKeyList[i], storD);
		strFout.Format(_T("  %2d, %10s, %g"), arKeyList[i], storD.StoryName, storD.dStoryLevel);
		// 출력.
		Write_String(strFout+_T("\n"));
	}
	
	// ####################################
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	m_arStoryID.RemoveAll();
	for(int i=0; i<arKeyList.GetSize(); i++) {
		// nSelectionType == 0   // 해당층과 층하부로 연결된 절점, 요소
		//                == 1   // 해당층과 층상부로 연결된 절점, 요소
		//                == 2   // 해당층과 층상부와 층하부로 연결된 모든 절점, 요소
		//                == 3   // 해당층과 층하부의 모든 절점, 요소
		//                == 4   // 해당층과 층상부의 모든 절점, 요소
		//                == 5   // 해당층만
		m_pDoc->m_pAttrCtrl->GetKeyListStorUser(arKeyList[i], 1, NULL, &arKeyElem);
		int iSize = arKeyElem.GetSize();
		for (int s=0; s<iSize; s++) {
			m_arStoryID.SetAt(arKeyElem[s], arKeyList[i]);
		}
	}
	
}

void CDgnMakeInputData::WriteMaterialData()
{
	// Material 리스트 불러오기.
	CString strFout=_T("");
	double Ec=0.0,Es=0.0,Po=0.0,Fu=0.0,Fy1=0.0,Fy2=0.0,Fy3=0.0,Fy4=0.0,Fc=0.0;
	CString MatlName=_T("");
	T_MATL_K matlK;
	T_MATD_D matdD;
	CArray<T_MATL_K,T_MATL_K> arMatlKList;
	arMatlKList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(arMatlKList);
	
	// ####################################
	// MATERIAL
	strFout.Format(_T("\n*MATERIAL, %i \n"), arMatlKList.GetSize());
	Write_String(strFout);
	Write_String(_T("! iMAT, TYPE(STL/CON/SRC), MNAME, [DATA] \n"));
	Write_String(_T("! [DATA] : ELAST, POISN, FU, FY1, FY2, FY3, FY4            ; STEEL \n"));
	Write_String(_T("! [DATA] : ELAST, POISN, FC                                ; CONC \n"));
	Write_String(_T("! [DATA] : ELAST(S), FU, FY1, FY2, FY3, FY4, ELAST(C), FC  ; SRC \n"));
	for(int i=0; i<arMatlKList.GetSize(); i++) {
		matdD.Initialize();
		matlK = arMatlKList[i];
		m_pDoc->m_pAttrCtrl->GetMatlDesign(matlK, matdD);
		if(matdD.Type == _T("S")) {
			MatlName = matdD.Data1.CodeMatlName;
			if(MatlName==_T("")) MatlName = matdD.Name;
			Es  = matdD.Data1.Analysis.Elast;
			Po  = matdD.Data1.Analysis.Poisson;
			Fu  = matdD.Data1.Design.S_Fu;
			Fy1 = matdD.Data1.Design.S_Fy1;
			Fy2 = matdD.Data1.Design.S_Fy2;
			Fy3 = matdD.Data1.Design.S_Fy3;
			Fy4 = matdD.Data1.Design.S_Fy4;
			strFout.Format(_T("  %2d, STL, %8s, %e, %g, %g, %g, %g, %g, %g"), matlK,MatlName,Es,Po,Fu,Fy1,Fy2,Fy3,Fy4);
		}
		else if(matdD.Type == _T("C")) {
			MatlName = matdD.Data1.CodeMatlName;
			if(MatlName==_T("")) MatlName = matdD.Name;
			Ec  = matdD.Data1.Analysis.Elast;
			Po  = matdD.Data1.Analysis.Poisson;
			Fc  = matdD.Data1.Design.C_fc;
			strFout.Format(_T("  %2d, CON, %8s, %e, %g, %g"), matlK,MatlName,Es,Po,Fc);
		}
		else if(matdD.Type == _T("SRC")) {
			MatlName = matdD.Name;
			Es  = matdD.Data1.Analysis.Elast;
			Fu  = matdD.Data1.Design.S_Fu;
			Fy1 = matdD.Data1.Design.S_Fy1;
			Fy2 = matdD.Data1.Design.S_Fy2;
			Fy3 = matdD.Data1.Design.S_Fy3;
			Fy4 = matdD.Data1.Design.S_Fy4;
			Ec  = matdD.Data2.Analysis.Elast;
			Fc  = matdD.Data2.Design.C_fc;
			strFout.Format(_T("  %2d, SRC, %8s, %e, %g, %g, %g, %g, %g, %e, %g"), matlK,MatlName,Es,Fu,Fy1,Fy2,Fy3,Fy4,Ec,Fc);
		}
		else if(matdD.Type == _T("U")) {
			// Not Design or Checking
		}
		// 출력.
		Write_String(strFout+_T("\n"));
	}
}

void CDgnMakeInputData::WriteSectData()
{
	// Section 리스트 불러오기.
	CString strFout=_T("");
	CString Shape1=_T(""),Shape2=_T(""),SectName=_T(""),Dim=_T("");
	double Hc=0.0,Bc=0.0,D1=0.0,D2=0.0,D3=0.0,D4=0.0,D5=0.0,D6=0.0;
	T_SECT_K sectK;
	T_SECT_D sectD;
	CArray<T_SECT_K,T_SECT_K> arSectKList;
	arSectKList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSectKeyList(arSectKList);
	
	// ####################################
	// SECTION
	strFout.Format(_T("\n*SECTION, %i \n"), arSectKList.GetSize());
	Write_String(strFout);
	Write_String(_T("! iSEC, TYPE, SNAME, SHAPE, [DATA]                         ; DB/USER \n"));
	Write_String(_T("! iSEC, TYPE, SNAME, SHAPE-OUT, D1, D2, SHAPE-IN, [DATA]   ; SRC \n"));
	Write_String(_T("! iSEC, TYPE, SNAME, SHAPE, [DIM1], [DIM2]                 ; TAPERED \n"));
	Write_String(_T("! [DATA],[DIM1],[DIM2] : D1, D2, D3, D4, D5, D6 \n"));
	for(int i=0; i<arSectKList.GetSize(); i++) {
		sectK = arSectKList[i];
		m_pDoc->m_pAttrCtrl->GetSect(sectK, sectD);
		
		D1 = sectD.SectBefore.SectI.Size[0];
		D2 = sectD.SectBefore.SectI.Size[1];
		D3 = sectD.SectBefore.SectI.Size[2];
		D4 = sectD.SectBefore.SectI.Size[3];
		D5 = sectD.SectBefore.SectI.Size[4];
		D6 = sectD.SectBefore.SectI.Size[5];
		Dim.Format(_T(", %g, %g, %g, %g, %g, %g"), D1,D2,D3,D4,D5,D6);
		
		if(sectD.nStype == D_SECT_TYPE_REGULAR || sectD.nStype == D_SECT_TYPE_USER) {
			SectName = sectD.SectBefore.SectI.SName;
			if(SectName == _T("")) SectName = sectD.SName;
			Shape1   = sectD.SectBefore.Shape;
			strFout.Format(_T("  %2d,  DBUSER, %15s, %5s"), sectK,SectName,Shape1);
			strFout = strFout+Dim;
		}
		else if(sectD.nStype == D_SECT_TYPE_SRC) {
			SectName = sectD.SName;
			Shape1   = sectD.SectBefore.Shape;
			Hc       = sectD.SectBefore.SectJ.Size[0];
			Bc       = sectD.SectBefore.SectJ.Size[1];
			Shape2   = sectD.SectBefore.SectI.Shape;
			strFout.Format(_T("  %2d,     SRC, %15s, %5s, %g, %g, %s"), sectK,SectName,Shape1,Hc,Bc,Shape2);
			strFout = strFout+Dim;
		}
		else if(sectD.nStype == D_SECT_TYPE_TAPERED) {
			SectName = sectD.SName;
			Shape1   = sectD.SectBefore.Shape;
			strFout.Format(_T("  %2d, TAPERED, %15s, %5s"), sectK,SectName,Shape1);
			strFout = strFout+Dim;
			
			D1 = sectD.SectBefore.SectJ.Size[0];
			D2 = sectD.SectBefore.SectJ.Size[1];
			D3 = sectD.SectBefore.SectJ.Size[2];
			D4 = sectD.SectBefore.SectJ.Size[3];
			D5 = sectD.SectBefore.SectJ.Size[4];
			D6 = sectD.SectBefore.SectJ.Size[5];
			Dim.Format(_T(", %g, %g, %g, %g, %g, %g"), D1,D2,D3,D4,D5,D6);
			strFout = strFout+Dim;
		}
		else {
			// Not Design or Checking
		}
		// 출력.
		Write_String(strFout+_T("\n"));
	}
}

void CDgnMakeInputData::WriteThickData()
{
	// Thick 리스트 불러오기.
	CString strFout=_T("");
	T_THIK_K thickK;
	T_THIK_D thickD;
	CArray<T_THIK_K,T_THIK_K> arThickKList;
	arThickKList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetThikKeyList(arThickKList);
	
	// ####################################
	// THICK
	strFout.Format(_T("\n*THICK, %i \n"), arThickKList.GetSize());
	Write_String(strFout);
	Write_String(_T("! iTHK, TYPE, THIK-IN, THIK-OUT    ; VALUE \n"));
	for(int i=0; i<arThickKList.GetSize(); i++) {
		thickK = arThickKList[i];
		thickD.Initialize();
		m_pDoc->m_pAttrCtrl->GetThik(thickK, thickD);
		if(thickD.nType == 0)
			strFout.Format(_T("  %2d, VALUE, %g, %g"), thickK, thickD.Value.ThickIn, thickD.Value.ThickOut);
		// 출력.
		Write_String(strFout+_T("\n"));
	}
}

void CDgnMakeInputData::WriteForceData()
{
	int i;
	CString strProg = _T("");
	//Set ForceCtrl to calculate member force
	T_DCON_D DconD; DconD.Initialize();
	m_pForceCtrl->m_pDgnDataCtrl->Get_DgnConDcon(DconD);
	m_pForceCtrl->m_strDgnCode = DconD.DesignCode; 
	m_pForceCtrl->m_bEqSpecial	= FALSE;//DconD.bSpecialSeismic;//박종배:함수내에서 직접 적용함.
	m_bSpecialEQ = DconD.bSpecialSeismic;
	m_pForceCtrl->m_iDgnClass	= DconD.nClass+1;
	//Scale Up Factor for Shear.
	m_pForceCtrl->m_iAlphaType	= DconD.iAlphaType;
	m_pForceCtrl->m_dAlpha1		  = DconD.dAlpha1;
	m_pForceCtrl->m_dAlpha2     = DconD.dAlpha2;
	m_pForceCtrl->m_dAlpha2_Colm= DconD.dAlpha2_Colm;
	
	double Axial=0.0,V_maj=0.0,V_min=0.0,Torsn=0.0,M_maj=0.0,M_min=0.0;
	double dLeng=0.0;
	CString strFout=_T("");
	T_ELEM_D ElemD;
	T_LENG_D LengD;
	
	m_pForceCtrl->m_nCode_Length = D_UNITSYS_LENGTH_INDEX_M;
	m_pForceCtrl->m_nCode_Force  = D_UNITSYS_FORCE_INDEX_N;
	// Change by ZINU.('05.08.09) Keep Remember.
	// 1. Call Set_DgnCtrlDataforCodeCheck().
	// 2. Call Set_LcomForTotl() before Set_LcomDataForElem(). 
	m_pForceCtrl->m_pDgnDataCtrl->m_pMembCtrl->Set_LcomForTotl(D_LCOMTYPE_CONCRETE, FALSE, TRUE, TRUE, FALSE);
	
	// ####################################
	// FORCE-BEAM
	// Change by ZINU.('05.08.02) Set m_iRcsTypeKind. 
	m_pForceCtrl->m_iElemKind=D_MBTP_BEAM;
	
	m_pForceCtrl->Set_LcomDataForElem(0);
	if ((m_iMembType == 0)||(m_iMembType == 4)) {
		strFout.Format(_T("\n*FORCE-BEAM, %i, %i \n"), m_arBeamList.GetSize(), m_pForceCtrl->m_iLCBAddDesignNum);
		Write_String(strFout);
		Write_String(_T("! ID, iSTR, iMATL, iSECT, LEN, iPOS, iLC, [FORCE] \n"));
		Write_String(_T("! [FORCE] : Axial, V_maj, V_min, Torsn, M_maj, M_min \n"));
		
		// Force-Beam List
		for(i=0; i<m_arBeamList.GetSize(); i++) 
		{
			strProg.Format(_T("    Write Beam Member :  %i / %i"), i+1, m_arBeamList.GetSize());
			GDisplayAnalStringSamePos(strProg);
			
			Axial=0.0,V_maj=0.0,V_min=0.0,Torsn=0.0,M_maj=0.0,M_min=0.0,dLeng=0.0;
			LengD.Initialize();
			T_ELEM_K ElemK = m_arBeamList[i];
			ElemD.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
			T_MBTP_D MbtpData;
			MbtpData.Initialize();
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenMbtp(ElemK, MbtpData);	// Not used.
			// Get Material.
			T_MATD_D MatlData;
			MatlData.Initialize();
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenMatd(ElemK, MatlData);
			
			m_pForceCtrl->m_iElemNo = ElemK;
			m_pForceCtrl->m_iElemKind		= MbtpData.nMbType;
			m_pForceCtrl->m_iElemSubKind = MbtpData.nSubType;
			m_pForceCtrl->m_iElemType = ElemD.eltyp;
			m_pForceCtrl->m_strMatType = MatlData.Type; // S, C, SRC, USER
			// Check Cantilever.
			m_pForceCtrl->m_iCantilever = m_pForceCtrl->IsCantilever(ElemK);
			m_pForceCtrl->m_bSubBeamForNoMpr = m_pForceCtrl->IsSubBeamForNoMpr(ElemK, m_pForceCtrl->m_iCantilever);
			// For 2D or 3D Design.
			m_pForceCtrl->m_pDgnDataCtrl->m_pMembCtrl->GetMembAxisForDgn(ElemK, m_pForceCtrl->m_iDgnFrameType, m_pForceCtrl->m_iDgnMembAxis);
			
			UINT iStorID = 0;
			m_arStoryID.Lookup(ElemK, iStorID);
			
			m_pForceCtrl->Set_LcomDataForElem(ElemK);
			// Get Load Combination Data.
			for(int j=0; j<m_pForceCtrl->m_iLCBAddDesignNum; j++)
			{
				// Set End Moment and Force and Moment Data at All Position.
				_DGN_LCOM LcomDesign; LcomDesign.Initialize();
				m_pForceCtrl->m_parLcomData->Lookup(j+1, LcomDesign);
				
				//2005.03.26 Park JongBae Add.
				_DGN_LCOM LcomDgnShear; LcomDgnShear.Initialize();
				if ((LcomDesign.iSeismicType == 1)&&(m_bSpecialEQ)) {
					m_pForceCtrl->m_parLcomData->Lookup(j+1, LcomDgnShear);
					int iSize = LcomDgnShear.LcomUlData.Combination.GetSize();
					for (int c=0; c<iSize; c++) {
						int iAnalType = LcomDgnShear.LcomUlData.Combination[c].AnalType;
						unsigned int iLcaseKey = LcomDgnShear.LcomUlData.Combination[c].LoadCaseKey;
						if(iAnalType==D_LCOM_STATIC)
						{
							T_STLD_D StldD; StldD.Initialize();
							m_pDoc->m_pAttrCtrl->GetStld(iLcaseKey, StldD);
							if(CDBLib::IsStldEL(StldD.LoadCaseType))	LcomDgnShear.LcomUlData.Combination[c].Factor *= 2.0;
						}
						else if(iAnalType==D_LCOM_SPECTRUM || iAnalType==D_LCOM_ECCENSPEC)
						{
							T_SPLC_D SplcD; SplcD.Initialize();
							m_pDoc->m_pAttrCtrl->GetSplc(iLcaseKey, SplcD);
							LcomDgnShear.LcomUlData.Combination[c].Factor *= 2.0;
						}
					}
				}
				
				//exclude this code by cylee from 'for' functions below 05.08.09
				//m_pForceCtrl->Set_ForceMomentBySeperate(ElemK, -1, LcomDesign);
				for(int p=0; p<5; p++) 
				{
					m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenLeng(ElemK, p, LengD, dLeng);
					m_pForceCtrl->Set_ForceMomentBySeperate(ElemK, p, LcomDesign);
					//int iLcomType = LcomDesign.LcomUlData.nActive;
					//double dMp=0.0, dMm=0.0, dVu=0.0;
					//m_pForceCtrl->GetMomByEqSpecial(iLcomType,dMp,dMm,dVu);
					//V_maj = dVu;
					//M_maj = (fabs(dMp) >= fabs(dMm) ? dMp : dMm); //절대값이 큰것 사용
					M_maj = m_pForceCtrl->m_BMy[p];
					
					Axial = m_pForceCtrl->m_Fxx[p];
					V_min = m_pForceCtrl->m_Fyy[p];
					Torsn = m_pForceCtrl->m_BMx[p];
					M_min = m_pForceCtrl->m_BMz[p];
					
					//2005.03.26 Park JongBae Modify.
					if ((LcomDesign.iSeismicType == 1)&&(m_bSpecialEQ)) {
						m_pForceCtrl->Set_ForceMomentBySeperate(ElemK, p, LcomDgnShear);
					}
					V_maj = m_pForceCtrl->m_Fzz[p];
					
					strFout.Format(_T(" %5d, %2d, %2d, %2d, %g, %2d, %s, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f"), 
						ElemK, iStorID, ElemD.elmat, ElemD.elpro, dLeng, p, LcomDesign.DesignLcomNa, 
						Axial, V_maj, V_min, Torsn, M_maj, M_min);
					// 출력.
					Write_String(strFout+_T("\n"));
				}
			}
		}
	}
	
	// ####################################
	// FORCE-COL
	// Change by ZINU.('05.08.02) Set m_iRcsTypeKind. 
	m_pForceCtrl->m_iElemKind=D_MBTP_COLUMN;
	
	m_pForceCtrl->Set_LcomDataForElem(0);
	if ((m_iMembType == 0)||(m_iMembType == 1)) {
		strFout.Format(_T("\n*FORCE-COL, %i, %i \n"), m_arColumnList.GetSize(), m_pForceCtrl->m_iLCBAddDesignNum);
		Write_String(strFout);
		Write_String(_T("! ID, iSTR, iMATL, iSECT, LEN, iPOS, iLC, [FORCE] \n"));
		Write_String(_T("! [FORCE] : Axial, V_maj, V_min, Torsn, M_maj, M_min \n"));
		// Force-Column List
		for(i=0; i<m_arColumnList.GetSize(); i++) 
		{
			strProg.Format(_T("    Write Column Member :  %i / %i"), i+1, m_arColumnList.GetSize());
			GDisplayAnalStringSamePos(strProg);
			
			Axial=0.0,V_maj=0.0,V_min=0.0,Torsn=0.0,M_maj=0.0,M_min=0.0,dLeng=0.0;
			ElemD.Initialize();
			LengD.Initialize();
			T_ELEM_K ElemK = m_arColumnList[i];
			m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
			T_MBTP_D MbtpData;
			MbtpData.Initialize();
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenMbtp(ElemK, MbtpData);	// Not used.
			// Get Material.
			T_MATD_D MatlData;
			MatlData.Initialize();
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenMatd(ElemK, MatlData);
			
			m_pForceCtrl->m_iElemNo = ElemK;
			m_pForceCtrl->m_iElemKind		= MbtpData.nMbType;
			m_pForceCtrl->m_iElemSubKind = MbtpData.nSubType;
			m_pForceCtrl->m_iElemType = ElemD.eltyp;
			m_pForceCtrl->m_strMatType = MatlData.Type; // S, C, SRC, USER
			// Check Cantilever.
			m_pForceCtrl->m_iCantilever = m_pForceCtrl->IsCantilever(ElemK);
			m_pForceCtrl->m_bSubBeamForNoMpr = m_pForceCtrl->IsSubBeamForNoMpr(ElemK, m_pForceCtrl->m_iCantilever);
			// For 2D or 3D Design.
			m_pForceCtrl->m_pDgnDataCtrl->m_pMembCtrl->GetMembAxisForDgn(ElemK, m_pForceCtrl->m_iDgnFrameType, m_pForceCtrl->m_iDgnMembAxis);
			
			UINT iStorID = 0;
			m_arStoryID.Lookup(ElemK, iStorID);
			
			m_pForceCtrl->Set_LcomDataForElem(ElemK);
			// Get Load Combination Data.
			for(int j=0; j<m_pForceCtrl->m_iLCBAddDesignNum; j++)
			{
				// Set End Moment and Force and Moment Data at All Position.
				_DGN_LCOM LcomDesign; LcomDesign.Initialize();
				m_pForceCtrl->m_parLcomData->Lookup(j+1, LcomDesign);
				
				//2005.03.26 Park JongBae Add.
				_DGN_LCOM LcomDgnShear; LcomDgnShear.Initialize();
				if ((LcomDesign.iSeismicType == 1)&&(m_bSpecialEQ)) {
					m_pForceCtrl->m_parLcomData->Lookup(j+1, LcomDgnShear);
					int iSize = LcomDgnShear.LcomUlData.Combination.GetSize();
					for (int c=0; c<iSize; c++) {
						int iAnalType = LcomDgnShear.LcomUlData.Combination[c].AnalType;
						unsigned int iLcaseKey = LcomDgnShear.LcomUlData.Combination[c].LoadCaseKey;
						if(iAnalType==D_LCOM_STATIC)
						{
							T_STLD_D StldD; StldD.Initialize();
							m_pDoc->m_pAttrCtrl->GetStld(iLcaseKey, StldD);
							if(CDBLib::IsStldEL(StldD.LoadCaseType))	LcomDgnShear.LcomUlData.Combination[c].Factor *= 2.0;
						}
						else if(iAnalType==D_LCOM_SPECTRUM || iAnalType==D_LCOM_ECCENSPEC)
						{
							T_SPLC_D SplcD; SplcD.Initialize();
							m_pDoc->m_pAttrCtrl->GetSplc(iLcaseKey, SplcD);
							LcomDgnShear.LcomUlData.Combination[c].Factor *= 2.0;
						}
					}
				}
				
				//exclude this code by cylee from 'for' functions below 05.08.09
				for(int p=0; p<5; p+=2) 
				{
					m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenLeng(ElemK, p, LengD, dLeng);
					m_pForceCtrl->Set_ForceMomentBySeperate(ElemK, p, LcomDesign);
					Axial = m_pForceCtrl->m_Fxx[p];
					Torsn = m_pForceCtrl->m_BMx[p];
					M_maj = m_pForceCtrl->m_BMy[p];
					M_min = m_pForceCtrl->m_BMz[p];
					
					//2005.03.26 Park JongBae Modify.
					if ((LcomDesign.iSeismicType == 1)&&(m_bSpecialEQ)) {
						m_pForceCtrl->Set_ForceMomentBySeperate(ElemK, p, LcomDgnShear);
					}
					V_min = m_pForceCtrl->m_Fyy[p];
					V_maj = m_pForceCtrl->m_Fzz[p];
					
					strFout.Format(_T(" %5d, %2d, %2d, %2d, %g, %2d, %s, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f"), 
						ElemK, iStorID, ElemD.elmat, ElemD.elpro, dLeng, p, LcomDesign.DesignLcomNa, 
						Axial, V_maj, V_min, Torsn, M_maj, M_min);
					// 출력.
					Write_String(strFout+_T("\n"));
				}
			}
		}
	}
	
	// ####################################
	// FORCE-BRACE
	// Change by ZINU.('05.08.02) Set m_iRcsTypeKind. 
	m_pForceCtrl->m_iElemKind=D_MBTP_BRACE;
	
	m_pForceCtrl->Set_LcomDataForElem(0);
	if ((m_iMembType == 0)||(m_iMembType == 2)||(m_iMembType == 1)) {
		strFout.Format(_T("\n*FORCE-BRACE, %i, %i \n"), m_arBraceList.GetSize(), m_pForceCtrl->m_iLCBAddDesignNum);
		Write_String(strFout);
		Write_String(_T("! ID, iSTR, iMATL, iSECT, LEN, iPOS, iLC, [FORCE] \n"));
		Write_String(_T("! [FORCE] : Axial, V_maj, V_min, Torsn, M_maj, M_min \n"));
		// Force-Brace List
		for(i=0; i<m_arBraceList.GetSize(); i++) 
		{
			strProg.Format(_T("    Write Brace Member :  %i / %i"), i+1, m_arBraceList.GetSize());
			GDisplayAnalStringSamePos(strProg);
			
			Axial=0.0,V_maj=0.0,V_min=0.0,Torsn=0.0,M_maj=0.0,M_min=0.0,dLeng=0.0;
			ElemD.Initialize();
			LengD.Initialize();
			T_ELEM_K ElemK = m_arBraceList[i];
			m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
			
			T_MBTP_D MbtpData;
			MbtpData.Initialize();
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenMbtp(ElemK, MbtpData);	// Not used.
			// Get Material.
			T_MATD_D MatlData;
			MatlData.Initialize();
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenMatd(ElemK, MatlData);
			
			m_pForceCtrl->m_iElemNo = ElemK;
			m_pForceCtrl->m_iElemKind		= MbtpData.nMbType;
			m_pForceCtrl->m_iElemSubKind = MbtpData.nSubType;
			m_pForceCtrl->m_iElemType = ElemD.eltyp;
			m_pForceCtrl->m_strMatType = MatlData.Type; // S, C, SRC, USER
			// Check Cantilever.
			m_pForceCtrl->m_iCantilever = m_pForceCtrl->IsCantilever(ElemK);
			m_pForceCtrl->m_bSubBeamForNoMpr = m_pForceCtrl->IsSubBeamForNoMpr(ElemK, m_pForceCtrl->m_iCantilever);
			// For 2D or 3D Design.
			m_pForceCtrl->m_pDgnDataCtrl->m_pMembCtrl->GetMembAxisForDgn(ElemK, m_pForceCtrl->m_iDgnFrameType, m_pForceCtrl->m_iDgnMembAxis);
			
			UINT iStorID = 0;
			m_arStoryID.Lookup(ElemK, iStorID);
			
			m_pForceCtrl->Set_LcomDataForElem(ElemK);
			// Get Load Combination Data.
			for(int j=0; j<m_pForceCtrl->m_iLCBAddDesignNum; j++)
			{
				// Set End Moment and Force and Moment Data at All Position.
				_DGN_LCOM LcomDesign; LcomDesign.Initialize();
				m_pForceCtrl->m_parLcomData->Lookup(j+1, LcomDesign);
				
				//2005.03.26 Park JongBae Add.
				_DGN_LCOM LcomDgnShear; LcomDgnShear.Initialize();
				if ((LcomDesign.iSeismicType == 1)&&(m_bSpecialEQ)) {
					m_pForceCtrl->m_parLcomData->Lookup(j+1, LcomDgnShear);
					int iSize = LcomDgnShear.LcomUlData.Combination.GetSize();
					for (int c=0; c<iSize; c++) 
					{
						int iAnalType = LcomDgnShear.LcomUlData.Combination[c].AnalType;
						unsigned int iLcaseKey = LcomDgnShear.LcomUlData.Combination[c].LoadCaseKey;
						if(iAnalType==D_LCOM_STATIC)
						{
							T_STLD_D StldD; StldD.Initialize();
							m_pDoc->m_pAttrCtrl->GetStld(iLcaseKey, StldD);
							if(CDBLib::IsStldEL(StldD.LoadCaseType))	LcomDgnShear.LcomUlData.Combination[c].Factor *= 2.0;
						}
						else if(iAnalType==D_LCOM_SPECTRUM || iAnalType==D_LCOM_ECCENSPEC)
						{
							T_SPLC_D SplcD; SplcD.Initialize();
							m_pDoc->m_pAttrCtrl->GetSplc(iLcaseKey, SplcD);
							LcomDgnShear.LcomUlData.Combination[c].Factor *= 2.0;
						}
					}
				}
				
				
				for(int p=0; p<5; p++) 
				{
					m_pForceCtrl->m_pDgnDataCtrl->Get_DgnGenLeng(ElemK, p, LengD, dLeng);
					m_pForceCtrl->Set_ForceMomentBySeperate(ElemK, p, LcomDesign);
					Axial = m_pForceCtrl->m_Fxx[p];
					Torsn = m_pForceCtrl->m_BMx[p];
					M_maj = m_pForceCtrl->m_BMy[p];
					M_min = m_pForceCtrl->m_BMz[p];
					
					//2005.03.26 Park JongBae Modify.
					if ((LcomDesign.iSeismicType == 1)&&(m_bSpecialEQ)) {
						m_pForceCtrl->Set_ForceMomentBySeperate(ElemK, p, LcomDgnShear);
					}
					V_min = m_pForceCtrl->m_Fyy[p];
					V_maj = m_pForceCtrl->m_Fzz[p];
					
					strFout.Format(_T(" %5d, %2d, %2d, %2d, %g, %2d, %s, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f"), 
						ElemK, iStorID, ElemD.elmat, ElemD.elpro, dLeng, p, LcomDesign.DesignLcomNa, 
						Axial, V_maj, V_min, Torsn, M_maj, M_min);
					// 출력.
					Write_String(strFout+_T("\n"));
				}
			}
		}
	}
	
	// ####################################
	// FORCE-WALL
	// Change by ZINU.('05.08.02) Set m_iRcsTypeKind. 
	m_pForceCtrl->m_iElemKind=D_MBTP_WALL;
	
	m_pForceCtrl->Set_LcomDataForElem(0);
	if ((m_iMembType == 0)||(m_iMembType == 3)) {
		strFout.Format(_T("\n*FORCE-WALL, %i, %i \n"), m_arWallList.GetSize(), m_pForceCtrl->m_iLCBAddDesignNum);
		Write_String(strFout);
		Write_String(_T("! WMARK, ID, iSTR, iMATL, THK, HEI, LEN, iPOS, iLC, [FORCE] \n"));
		Write_String(_T("! [FORCE] : Axial, V_maj, V_min, Torsn, M_maj, M_min \n"));
		// Force-Wall List
		for(i=0; i<m_arWallList.GetSize(); i++) 
		{
			strProg.Format(_T("    Write Wall Member :  %i / %i"), i+1, m_arWallList.GetSize());
			GDisplayAnalStringSamePos(strProg);
			
			Axial=0.0,V_maj=0.0,V_min=0.0,Torsn=0.0,M_maj=0.0,M_min=0.0,dLeng=0.0;
			ElemD.Initialize();
			LengD.Initialize();
			T_ELEM_K ElemK = m_arWallList[i];
			
			// Get UserWallID.
			T_WALL_K WallKey;
			m_pDoc->m_pPostCtrl->GetWallKey(ElemK, WallKey.keymap);
			T_WALL_KEY KeyWall = (T_WALL_KEY)WallKey.keymap;
			T_WALL_D WallData;
			WallData.Initialize();
			m_pDoc->m_pPostCtrl->GetWallData(KeyWall, WallData);
			
			T_ELEM_K PlateKey = (T_ELEM_K)WallData.arElemKey.GetAt(0);
			if(!m_pDoc->m_pAttrCtrl->GetElem(PlateKey, ElemD))	ASSERT(0);
			
			int iMatlNo=0;
			T_MATD_D MatlData;
			MatlData.Initialize();
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnWalMatd(KeyWall, iMatlNo, MatlData);
			T_MBTP_D MbtpData;
			MbtpData.Initialize();
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnWalMbtp(KeyWall, MbtpData);	// Not used.
			
			m_pForceCtrl->m_iElemNo = ElemK;
			m_pForceCtrl->m_iElemType = ElemD.eltyp;
			m_pForceCtrl->m_iElemKind		= MbtpData.nMbType;	// D_MBTP_WALL.
			m_pForceCtrl->m_iElemSubKind = MbtpData.nSubType;
			m_pForceCtrl->m_strMatType = MatlData.Type; // S, C, SRC, USER
			m_pForceCtrl->m_iDgnMembAxis = 3;	// Always 3D.
			
			int iWallID = WallKey.key.wallid;
			int iStorID = WallKey.key.storid;
			
			// Get Wall Mark.
			T_WMAK_D WallMarkData;
			WallMarkData.Initialize();
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnWalWmak(KeyWall, WallMarkData);
			// Get Material.
			T_MATD_D MatData;
			MatData.Initialize();
			int iMatNo = 0;
			m_pForceCtrl->m_pDgnDataCtrl->Get_DgnWalMatd(KeyWall, iMatNo, MatData);
			
			double dThk = WallData.dThickness;
			double dHgh = WallData.dHeight;
			double dLen = WallData.dLength;
			
			unsigned int nInternalWallId=0;
			m_pDoc->m_pPostCtrl->GetInternalWallId(PlateKey,nInternalWallId);
			m_pForceCtrl->Set_LcomDataForElem(nInternalWallId);
			// Get Load Combination Data.
			for(int j=0; j<m_pForceCtrl->m_iLCBAddDesignNum; j++)
			{
				// Set End Moment and Force and Moment Data at All Position.
				_DGN_LCOM LcomDesign; LcomDesign.Initialize();
				m_pForceCtrl->m_parLcomData->Lookup(j+1, LcomDesign);
				
				//exclude this code by cylee from 'for' functions below 05.08.09
				m_pForceCtrl->Set_ForceMomentBySeperate(ElemK, -1, LcomDesign);
				for(int p=0; p<5; p+=4) 
				{
					//m_pForceCtrl->Set_ForceMomentBySeperate(ElemK, p, LcomDesign);
					
					Axial = m_pForceCtrl->m_Fxx[p];
					V_min = m_pForceCtrl->m_Fyy[p];
					V_maj = m_pForceCtrl->m_Fzz[p];
					Torsn = m_pForceCtrl->m_BMx[p];
					M_maj = m_pForceCtrl->m_BMy[p];
					M_min = m_pForceCtrl->m_BMz[p];
					
					strFout.Format(_T(" %s, %5d, %2d, %2d, %g, %g, %g, %2d, %s, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f"), 
						WallMarkData.MarkName, iWallID, iStorID, iMatNo, dThk, dHgh, dLen, p, LcomDesign.DesignLcomNa, 
						Axial, V_maj, V_min, Torsn, M_maj, M_min);
					// 출력.
					Write_String(strFout+_T("\n"));
				}
			}
		}
	}
}

void CDgnMakeInputData::Write_Node_Data(int iMembType)
{
	if(m_pDoc == NULL) m_pDoc = CDBDoc::GetDocPoint();

	if (iMembType == 91) m_iLcomType = D_LCOMTYPE_STEEL;
	else if (iMembType == 92) m_iLcomType = D_LCOMTYPE_CONCRETE;
	else if (iMembType == 93) m_iLcomType = D_LCOMTYPE_SRC;
	else if (iMembType == 94) m_iLcomType = D_LCOMTYPE_FDN;
 
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;//D_UNITSYS_LENGTH_INDEX_CM;
	CngIndex.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;//D_UNITSYS_FORCE_INDEX_TON;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data를 받아올때는 Code Unit기준
	//////////////////////////////////////////////
	
	COleDateTime CurrentTime;
	CurrentTime = COleDateTime::GetCurrentTime();
	int Year  = CurrentTime.GetYear();
	int Month = CurrentTime.GetMonth();
	int Day   = CurrentTime.GetDay();
	CString strDate=_T("");
	strDate.Format(_T("%d/%d/%d"), Year, Month, Day);
	
	CString strProdNa = D_PRODUCT_NAME;
	Write_String(_T("!=============================================\n"));
	Write_String(_T("!  Input Data for DESIGN in ")+strProdNa+_T("\n"));
	Write_String(_T("!  Date : ")+strDate+_T("\n"));
	Write_String(_T("! \n"));
	Write_String(_T("!  !: comment \n"));
	Write_String(_T("!  *: command \n"));
	Write_String(_T("!  $: variable comment \n"));
	Write_String(_T("!=============================================\n"));
	
	// ####################################
	// INFO
	Write_String(_T("\n*INFO \n"));
	Write_String(_T("VER, 1000 \n"));//Format Version
	Write_String(_T("ELEM-TYPE, 1 \n"));// 0:Element,  1:Member
	Write_String(_T("LOAD-TYPE, 1 \n"));// 0:LoadCase, 1:LoadComb
	
	// ####################################
	// UNIT
	Write_String(_T("\n*UNIT \n"));
	Write_String(_T("! FORCE, LENGTH \n"));
	Write_String(_T("! FORCE  : 1:KGF, 2:TONF, 3:N, 4:KN, 5:LBF, 6:KIP \n"));
	Write_String(_T("! LENGTH : 1:MM, 2:CM, 3:M, 4:IN, 5:FT \n"));
	Write_String(_T("  3,  3 \n"));
	
	// ####################################
	// LOADCOMB
	WriteLoadComb_Description();
	// SECTION
	WriteSectData();
	
	// Selected Member Lists
	m_pForceCtrl->Set_LcomDataForDesign(m_iLcomType, FALSE);

	m_arNodeList.RemoveAll();
	m_pDoc->m_pViewCtrl->GetAllSelectedNode(m_arNodeList);

	
	// FORCE
	Write_ReactionForce(iMembType);
	
	Write_String(_T("\n*END \n"));
	
	//////////////////////////////////////////////
	// <Remember> Data를 받고나서는 User Unit기준
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	// <Remember> Data를 받고나서는 User Unit기준
	//////////////////////////////////////////////
}


void CDgnMakeInputData::Write_ReactionForce(int iMembType)
{
	double dP, dMx, dMy, dVx, dVy; 
	CString strFout=_T("");
	T_LCOM_D LcomD;
	
	// Change by ZINU.('02.2.15). To Speed up (CString -> T_LCOM_K).
	CArray<T_LCOM_K, T_LCOM_K> arLcombList;
	arLcombList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(m_iLcomType, arLcombList);
	
	strFout.Format(_T("\n*FORCE-NODE, %i, %i \n"), m_arNodeList.GetSize(), arLcombList.GetSize());
	Write_String(strFout);
	if (iMembType == 94) {//Footing
		Write_String(_T("! ID, iLC, Axial, Vx, Vy, Mx, My, iSECT, Beta-Ang \n"));
	}
	else {
		Write_String(_T("! ID, iLC, Axial, Vx, Vy, Mx, My \n"));
	}
	
	for(int n=0; n<m_arNodeList.GetSize(); n++) {
		int iNode = m_arNodeList[n];
		
		for(int c=0; c<arLcombList.GetSize(); c++) {
			int iLcom  = arLcombList[c];
			
			LcomD.Initialize();
			m_pDoc->m_pAttrCtrl->GetLcom(m_iLcomType, iLcom, LcomD); //Load Combination을 가져옴 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			
			// Change by ZINU.('01.11.27).
			m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD); // Load Combi를 셋팅시킴.
			T_REAC_D ReacMaxData;
			ReacMaxData.Initialize();
			m_pDoc->m_pPostCtrl->GetReacNew(iNode, &ReacMaxData, NULL, NULL);	// reaction을 가져옴.
			
			dVx = ReacMaxData.dblReac[0];
			dVy = ReacMaxData.dblReac[1];
			dP = ReacMaxData.dblReac[2];
			dMx = ReacMaxData.dblReac[3];
			dMy = ReacMaxData.dblReac[4];
			
			strFout.Format(_T(" %5d, %2d, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f"), 
				iNode, iLcom, dP, dVx, dVy, dMx, dMy);
			Write_String(strFout);
			
			if (iMembType == 94) {//Footing
				CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
				T_ELEM_D ElemD;
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(iNode, arKeyElem);//절점에 연결된 요소번호.
				
				int nSize = arKeyElem.GetSize();
				for (int p=0; p<nSize; p++) {
					int ElemNo = arKeyElem.GetAt(p);
					
					int iMembCol = m_pDoc->m_pAttrCtrl->GetMemberType(ElemNo);// 1=column(수직-1/100 오차허용).
					if (iMembCol == 1) {
						BOOL bCheck = m_pDoc->m_pAttrCtrl->GetElem(ElemNo, ElemD);
						strFout.Format(_T(", %2d, %5.0f \n"), ElemD.elpro, ElemD.angle);
						Write_String(strFout);
						
						break;
					}
				}
			}
			
			Write_String(_T(" \n"));
		}
	}
	
}

BOOL CDgnMakeInputData::Write_String(CString strValue)
{
	CString strChgValue=_T("");
	
	strChgValue.Format(_T("%s"), strValue);
	
	int nLen = strChgValue.GetLength();
	m_cfile.WriteString(strChgValue);
	
	return TRUE;
}



#include "oncSectionRebarViewDlg.h" // 한상순씨 작업을 위해 임시로 추가한것. 

void DoTempForHanSangSun(CView* pView)
{
	ConcSectionRebarViewDlg dlg(AfxGetMainWnd());
	dlg.DoModal();
}
