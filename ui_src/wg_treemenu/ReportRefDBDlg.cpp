// ReportRefDBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ReportRefDBDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ReadMGB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportRefDBDlg dialog


CReportRefDBDlg::CReportRefDBDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CReportRefDBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportRefDBDlg)
	m_bUfig = TRUE;
	m_bUtbl = TRUE;
	m_bUcht = TRUE;
	m_bUtpl = TRUE;
	m_bUimg = TRUE;
	m_nIDType = 0;
	m_nKeepDefaultUnit = 0;
	//}}AFX_DATA_INIT

	m_strAbsFileName=_T("");
	m_pDoc = CDBDoc::GetDocPoint();
}

void CReportRefDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportRefDBDlg)
	DDX_Check(pDX, IDC_TM_REPORT_REFDB_UFIG_CHK, m_bUfig);
	DDX_Check(pDX, IDC_TM_REPORT_REFDB_UTBL_CHK, m_bUtbl);
	DDX_Check(pDX, IDC_TM_REPORT_REFDB_UCHT_CHK, m_bUcht);
	DDX_Check(pDX, IDC_TM_REPORT_REFDB_UTPL_CHK, m_bUtpl);
	DDX_Check(pDX, IDC_TM_REPORT_REFDB_UIMG_CHK, m_bUimg);
	DDX_Radio(pDX, IDC_TM_REPORT_REFDB_KEEPID_RDO, m_nIDType);
	DDX_Radio(pDX, IDC_TM_REPORT_REFDB_KEEPUN_RDO, m_nKeepDefaultUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReportRefDBDlg, CDialogMove)
	//{{AFX_MSG_MAP(CReportRefDBDlg)
	ON_BN_CLICKED(IDC_TM_REPORT_REFDB_SEL_BTN   , OnRefDBSelectAllBtn)
	ON_BN_CLICKED(IDC_TM_REPORT_REFDB_UNSEL_BTN , OnRefDBUnselectAllBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportRefDBDlg message handlers

void CReportRefDBDlg::OnOK() 
{
	UpdateData(TRUE);
	Dlg2Data();

	CDialogMove::OnOK();
}

BOOL CReportRefDBDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	Data2Dlg();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportRefDBDlg::SetModelPathName(CString strFileName)
{
	m_strAbsFileName = strFileName;
	ReadModelFile();
}

void CReportRefDBDlg::Data2Dlg() 
{
	ReadData();
	SortData();

	m_nIDType = 0;  // Keep ID

	int nUfig = m_arUfig.GetSize();
	int nUtbl = m_arUtbl.GetSize();
	int nUcht = m_arUcht.GetSize();
	int nUtpl = m_arUtpl.GetSize();
	int nUimg = m_arUimg.GetSize();

	// Ufig
	if(nUfig>0)
	{
		m_bUfig = TRUE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UFIG_CHK)->EnableWindow(TRUE);
	}
	else
	{
		m_bUfig = FALSE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UFIG_CHK)->EnableWindow(FALSE);
	}

	// Utbl
	if(nUtbl>0)
	{
		m_bUtbl = TRUE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UTBL_CHK)->EnableWindow(TRUE);
	}
	else
	{
		m_bUtbl = FALSE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UTBL_CHK)->EnableWindow(FALSE);
	}

	// Ucht
	if(nUcht>0)
	{
		m_bUcht = TRUE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UCHT_CHK)->EnableWindow(TRUE);
	}
	else
	{
		m_bUcht = FALSE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UCHT_CHK)->EnableWindow(FALSE);
	}

	// Utpl
	if(nUtpl>0)
	{
		m_bUtpl = TRUE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UTPL_CHK)->EnableWindow(TRUE);
	}
	else
	{
		m_bUtpl = FALSE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UTPL_CHK)->EnableWindow(FALSE);
	}

	// Uimg
	if(nUimg>0)
	{
		m_bUimg = TRUE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UIMG_CHK)->EnableWindow(TRUE);
	}
	else
	{
		m_bUimg = FALSE;
		GetDlgItem(IDC_TM_REPORT_REFDB_UIMG_CHK)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

void CReportRefDBDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	InsertModelToDB();
}

BOOL CReportRefDBDlg::DestroyWindow() 
{
	if(m_pFile)
		delete m_pFile;
	
	return CDialogMove::DestroyWindow();
}

void CReportRefDBDlg::ReadModelFile()
{
	if(m_strAbsFileName==_T("")) return;

	m_pFile = new CFile();
	m_pFile->Open(m_strAbsFileName, CFile::modeRead);
	
	int nITPF = 1;
	float FileVersion=1.0, RetFileVersion;
	char HeadName[4];
	memcpy(HeadName,D_MODEL_FILE_HEAD,4);
	
	BOOL bException = FALSE;
	TRY
	{
		m_BnFile.INIT_F(m_pFile, HeadName, nITPF, FileVersion);
		
		int nError = m_BnFile.CHKHED_F(RetFileVersion);
		if(nError != BN_ER_GOOD && nError != BN_ER_VRLO)
		{
			m_pDoc->m_pPostCtrl->DisplayErrorMessage(_LS(IDS_DB_MGB_ER_READ), nError);
			goto BAD;
		}
	}
	CATCH(CException, e)
	{
		bException=TRUE;
		e->ReportError();
	}
	END_CATCH
		
		if(bException)	goto BAD;
BAD:
	return;
}

BOOL CReportRefDBDlg::ReadData() 
{
	if(!RB_UFIG()) return FALSE;
	if(!RB_UTBL()) return FALSE;
	if(!RB_UCHT()) return FALSE;
	if(!RB_UTPL()) return FALSE;
	if(!RB_UIMG()) return FALSE;

	return TRUE;
}

BOOL CReportRefDBDlg::RB_UFIG() 
{
	m_arUfig.RemoveAll();	
	
	T_UFIG_UDRD_D UfigData;
	CArray<T_UFIG_K, T_UFIG_K> aKey;
	CArray<T_UFIG_D, T_UFIG_D&> aData;
	if (!CReadMGB::RB_UFIG(m_BnFile, aKey, aData)) return FALSE;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		m_pDoc->m_pUnitCtrl->ConvertUnitUfigOut(aData[i]);
		UfigData.key = aKey[i];
		UfigData.data = aData[i];
		m_arUfig.Add(UfigData);
	}
	
	return TRUE;
}

BOOL CReportRefDBDlg::RB_UTBL() 
{
	m_arUtbl.RemoveAll();	

	T_UTBL_UDRD_D UtblData;
	CArray<T_UTBL_K, T_UTBL_K> aKey;
	CArray<T_UTBL_D, T_UTBL_D&> aData;
	if (!CReadMGB::RB_UTBL(m_BnFile, aKey, aData)) return FALSE;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		m_pDoc->m_pUnitCtrl->ConvertUnitUtblOut(aData[i]);
		UtblData.key = aKey[i];
		UtblData.data = aData[i];

		if(m_nKeepDefaultUnit==1)
		{
			// Unit Option이 Default면 현재 단위계로 설정하여 가져옴
			T_UNIT_INDEX UnitIdxCur;
			m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIdxCur);
			UtblData.data.bDefinedUnit = FALSE;
			UtblData.data.nUnitLength = UnitIdxCur.nBase_Length;
			UtblData.data.nUnitForce  = UnitIdxCur.nBase_Force ;
			UtblData.data.nUnitHeat   = UnitIdxCur.nBase_Heat  ;
			UtblData.data.nUnitTemper = UnitIdxCur.nBase_Temper;
		}

		m_arUtbl.Add(UtblData);
	}

	return TRUE;
}

BOOL CReportRefDBDlg::RB_UCHT() 
{
	m_arUcht.RemoveAll();	

	T_UCHT_UDRD_D UchtData;
	CArray<T_UCHT_K, T_UCHT_K> aKey;
	CArray<T_UCHT_D, T_UCHT_D&> aData;
	if (!CReadMGB::RB_UCHT(m_BnFile, aKey, aData)) return FALSE;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		m_pDoc->m_pUnitCtrl->ConvertUnitUchtOut(aData[i]);
		UchtData.key = aKey[i];
		UchtData.data = aData[i];

		if(m_nKeepDefaultUnit==1)
		{
			// Unit Option이 Default면 현재 단위계로 설정하여 가져옴
			T_UNIT_INDEX UnitIdxCur;
			m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIdxCur);
			UchtData.data.bDefinedUnit = FALSE;
			UchtData.data.nUnitLength = UnitIdxCur.nBase_Length;
			UchtData.data.nUnitForce  = UnitIdxCur.nBase_Force ;
			UchtData.data.nUnitHeat   = UnitIdxCur.nBase_Heat  ;
			UchtData.data.nUnitTemper = UnitIdxCur.nBase_Temper;
		}

		m_arUcht.Add(UchtData);
	}

	return TRUE;
}

BOOL CReportRefDBDlg::RB_UTPL() 
{
	m_arUtpl.RemoveAll();	
	
	T_UTPL_UDRD_D UtplData;
	CArray<T_UTPL_K, T_UTPL_K> aKey;
	CArray<T_UTPL_D, T_UTPL_D&> aData;
	if (!CReadMGB::RB_UTPL(m_BnFile, aKey, aData)) return FALSE;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		m_pDoc->m_pUnitCtrl->ConvertUnitUtplOut(aData[i]);
		UtplData.key = aKey[i];
		UtplData.data = aData[i];
		m_arUtpl.Add(UtplData);
	}
	
	return TRUE;
}

BOOL CReportRefDBDlg::RB_UIMG() 
{
	m_arUimg.RemoveAll();	
	
	T_UIMG_UDRD_D UimgData;
	CArray<T_UIMG_K, T_UIMG_K> aKey;
	CArray<T_UIMG_D, T_UIMG_D&> aData;
	if (!CReadMGB::RB_UIMG(m_BnFile, aKey, aData)) return FALSE;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		m_pDoc->m_pUnitCtrl->ConvertUnitUimgOut(aData[i]);
		UimgData.key = aKey[i];
		UimgData.data = aData[i];
		m_arUimg.Add(UimgData);
	}
	
	return TRUE;
}

BOOL CReportRefDBDlg::ImportUfig(int nNumType)
{
	ASSERT(m_pDoc);

	T_UFIG_UDRD_D UfigData; 
	
	T_UFIG_K Key;
	T_UFIG_D Data;

	BOOL bAdd=FALSE;

	int nUfig=0;
	nUfig = m_arUfig.GetSize();

	if(nNumType==0) // Keep ID
	{
		for(int i=0; i<nUfig; i++)
		{
			UfigData.data.Initialize();
			UfigData = m_arUfig.GetAt(i);
			
			Key = UfigData.key;
			Data = UfigData.data;
			
			if(m_pDoc->m_pAttrCtrl->ExistUfig(Data.strName))
			{
				if(!m_pDoc->m_pEditData->DelUfig(Data.strName))
					continue;
			}
			InitUfig(Data);
			Data.PrePrudD.bReferenceDB = TRUE;  // Referece DB로 추가된 이미지는 체크해줌. 20110418 KIMJM
			if(!m_pDoc->m_pEditData->AddUfig(Data))
				continue;
			
			bAdd = TRUE;
		}
	}
	else if(nNumType==1) // New ID
	{
		for(int i=0; i<nUfig; i++)
		{
			UfigData.data.Initialize();
			UfigData = m_arUfig.GetAt(i);
			
			Key = UfigData.key;
			Data = UfigData.data;

			CString strName=_T(""), strTemp=_T(""), strName2=_T("");
			int nIdx=1;
			
			strName2 = Data.strName;
			strName = strName2;
			while(m_pDoc->m_pAttrCtrl->ExistUfig(strName))
			{
				strTemp.Format(_T("%d"), nIdx++);
				strName = strName2 + strTemp;
			}
			strName2 = strName;
			Data.strName = strName2;

			InitUfig(Data);
			Data.PrePrudD.bReferenceDB = TRUE;  // Referece DB로 추가된 이미지는 체크해줌. 20110418 KIMJM
			if(!m_pDoc->m_pEditData->AddUfig(Data))
				continue;
			
			bAdd = TRUE;
		}
	}

	CString strMsg=_T("");
#if defined(_ORG)
	strMsg = CString(_T("Smart Report의 Reference DB : User Defined Image가 추가되었습니다."));
#else
	strMsg = _LS(IDS_TM_DYNA_REPORT_REFDB_ADD_UFIG);
#endif

	if(bAdd)
		GSaveHistoryFormatNF(strMsg);

	return TRUE;
}

BOOL CReportRefDBDlg::ImportUtbl(int nNumType)
{
	ASSERT(m_pDoc);
	
	T_UTBL_UDRD_D UtblData; 
	
	T_UTBL_K Key;
	T_UTBL_D Data;
	
	BOOL bAdd=FALSE;
	
	int nUtbl=0;
	nUtbl = m_arUtbl.GetSize();
	
	if(nNumType==0) // Keep ID
	{
		for(int i=0; i<nUtbl; i++)
		{
			UtblData.data.Initialize();
			UtblData = m_arUtbl.GetAt(i);
			
			Key = UtblData.key;
			Data = UtblData.data;
			
			if(m_pDoc->m_pAttrCtrl->ExistUtbl(Data.strName))
			{
				if(!m_pDoc->m_pEditData->DelUtbl(Data.strName))
					continue;
			}
			InitUtbl(Data);
			if(!m_pDoc->m_pEditData->AddUtbl(Data))
				continue;
			
			bAdd = TRUE;
		}
	}
	else if(nNumType==1) // New ID
	{
		for(int i=0; i<nUtbl; i++)
		{
			UtblData.data.Initialize();
			UtblData = m_arUtbl.GetAt(i);
			
			Key = UtblData.key;
			Data = UtblData.data;
			
			CString strName=_T(""), strTemp=_T(""), strName2=_T("");
			int nIdx=1;
			
			strName2 = Data.strName;
			strName = strName2;
			while(m_pDoc->m_pAttrCtrl->ExistUtbl(strName))
			{
				strTemp.Format(_T("%d"), nIdx++);
				strName = strName2 + strTemp;
			}
			strName2 = strName;
			Data.strName = strName2;

			InitUtbl(Data);
			if(!m_pDoc->m_pEditData->AddUtbl(Data))
				continue;
			
			bAdd = TRUE;
		}
	}
	
	CString strMsg=_T("");
#if defined(_ORG)
	strMsg = CString(_T("Smart Report의 Reference DB : User Defined Table이 추가되었습니다."));
#else
	strMsg = _LS(IDS_TM_DYNA_REPORT_REFDB_ADD_UTBL);
#endif
	
	if(bAdd)
		GSaveHistoryFormatNF(strMsg);
	
	return TRUE;
}

BOOL CReportRefDBDlg::ImportUcht(int nNumType)
{
	ASSERT(m_pDoc);
	
	T_UCHT_UDRD_D UchtData; 
	
	T_UCHT_K Key;
	T_UCHT_D Data;
	
	BOOL bAdd=FALSE;
	
	int nUcht=0;
	nUcht = m_arUcht.GetSize();
	
	if(nNumType==0) // Keep ID
	{
		for(int i=0; i<nUcht; i++)
		{
			UchtData.data.Initialize(0);
			UchtData = m_arUcht.GetAt(i);
			
			Key = UchtData.key;
			Data = UchtData.data;
			
			if(m_pDoc->m_pAttrCtrl->ExistUcht(Data.strName))
			{
				if(!m_pDoc->m_pEditData->DelUcht(Data.strName))
					continue;
			}
			if(!m_pDoc->m_pEditData->AddUcht(Data))
				continue;
			
			bAdd = TRUE;
		}
	}
	else if(nNumType==1) // New ID
	{
		for(int i=0; i<nUcht; i++)
		{
			UchtData.data.Initialize(0);
			UchtData = m_arUcht.GetAt(i);
			
			Key = UchtData.key;
			Data = UchtData.data;
			
			CString strName=_T(""), strTemp=_T(""), strName2=_T("");
			int nIdx=1;
			
			strName2 = Data.strName;
			strName = strName2;
			while(m_pDoc->m_pAttrCtrl->ExistUcht(strName))
			{
				strTemp.Format(_T("%d"), nIdx++);
				strName = strName2 + strTemp;
			}
			strName2 = strName;
			Data.strName = strName2;

			if(!m_pDoc->m_pEditData->AddUcht(Data))
				continue;
			
			bAdd = TRUE;
		}
	}
	
	CString strMsg=_T("");
#if defined(_ORG)
	strMsg = CString(_T("Smart Report의 Reference DB : User Defined Chart가 추가되었습니다."));
#else
	strMsg = _LS(IDS_TM_DYNA_REPORT_REFDB_ADD_UCHT);
#endif
	
	if(bAdd)
		GSaveHistoryFormatNF(strMsg);
	
	return TRUE;
}

BOOL CReportRefDBDlg::ImportUtpl(int nNumType)
{
	ASSERT(m_pDoc);
	
	T_UTPL_UDRD_D UtplData; 
	
	T_UTPL_K Key;
	T_UTPL_D Data;
	
	BOOL bAdd=FALSE;
	
	int nUtpl=0;
	nUtpl = m_arUtpl.GetSize();
	
	if(nNumType==0) // Keep ID
	{
		for(int i=0; i<nUtpl; i++)
		{
			UtplData.data.Initialize();
			UtplData = m_arUtpl.GetAt(i);
			
			Key = UtplData.key;
			Data = UtplData.data;
			
			if(m_pDoc->m_pAttrCtrl->ExistUtpl(Data.strName))
			{
				if(!m_pDoc->m_pEditData->DelUtpl(Data.strName))
					continue;
			}
			if(!m_pDoc->m_pEditData->AddUtpl(Data))
				continue;
			
			bAdd = TRUE;
		}
	}
	else if(nNumType==1) // New ID
	{
		for(int i=0; i<nUtpl; i++)
		{
			UtplData.data.Initialize();
			UtplData = m_arUtpl.GetAt(i);
			
			Key = UtplData.key;
			Data = UtplData.data;
			
			CString strName=_T(""), strTemp=_T(""), strName2=_T("");
			int nIdx=1;
			
			strName2 = Data.strName;
			strName = strName2;
			while(m_pDoc->m_pAttrCtrl->ExistUtpl(strName))
			{
				strTemp.Format(_T("%d"), nIdx++);
				strName = strName2 + strTemp;
			}
			strName2 = strName;
			Data.strName = strName2;

			if(!m_pDoc->m_pEditData->AddUtpl(Data))
				continue;
			
			bAdd = TRUE;
		}
	}
	
	CString strMsg=_T("");
#if defined(_ORG)
	strMsg = CString(_T("Smart Report의 Reference DB : Table Template이 추가되었습니다."));
#else
	strMsg = _LS(IDS_TM_DYNA_REPORT_REFDB_ADD_UTPL);
#endif
	
	if(bAdd)
		GSaveHistoryFormatNF(strMsg);
 
	return TRUE;
}

BOOL CReportRefDBDlg::ImportUimg(int nNumType)
{
	ASSERT(m_pDoc);
	
	T_UIMG_UDRD_D UimgData; 
	
	T_UIMG_K Key;
	T_UIMG_D Data;
	
	BOOL bAdd=FALSE;
	
	int nUimg=0;
	nUimg = m_arUimg.GetSize();
	
	if(nNumType==0) // Keep ID
	{
		for(int i=0; i<nUimg; i++)
		{
			UimgData.data.Initialize();
			UimgData = m_arUimg.GetAt(i);
			
			Key = UimgData.key;
			Data = UimgData.data;
			
			if(m_pDoc->m_pAttrCtrl->ExistUimg(Data.strName))
			{
				if(!m_pDoc->m_pEditData->DelUimg(Data.strName))
					continue;
			}
			if(!m_pDoc->m_pEditData->AddUimg(Data))
				continue;
			
			bAdd = TRUE;
		}
	}
	else if(nNumType==1) // New ID
	{
		for(int i=0; i<nUimg; i++)
		{
			UimgData.data.Initialize();
			UimgData = m_arUimg.GetAt(i);
			
			Key = UimgData.key;
			Data = UimgData.data;
			
			CString strName=_T(""), strTemp=_T(""), strName2=_T("");
			int nIdx=1;
			
			strName2 = Data.strName;
			strName = strName2;
			while(m_pDoc->m_pAttrCtrl->ExistUimg(strName))
			{
				strTemp.Format(_T("%d"), nIdx++);
				strName = strName2 + strTemp;
			}
			strName2 = strName;
			Data.strName = strName2;

			if(!m_pDoc->m_pEditData->AddUimg(Data))
				continue;
			
			bAdd = TRUE;
		}
	}
	
	CString strMsg=_T("");
#if defined(_ORG)
	strMsg = CString(_T("Smart Report의 Reference DB : Image File이 추가되었습니다."));
#else
	strMsg = _LS(IDS_TM_DYNA_REPORT_REFDB_ADD_UIMG);
#endif
	
	if(bAdd)
		GSaveHistoryFormatNF(strMsg);
	
	return TRUE;
}

void CReportRefDBDlg::InitUfig(T_UFIG_D& Data)
{
//   Data.nStagStatus = 0;
//   Data.CurStagK = 0;
//   Data.PrePrudD.nActiveKind = 0;
}

void CReportRefDBDlg::InitUtbl(T_UTBL_D& Data)
{
}

void CReportRefDBDlg::SortData()  //Sort
{
	int nSize;
	int nMinIndex=0;
	int nOldKey = 0;
	int nMinKey = 0;
	int i,j;

	// UFIG
	T_UFIG_UDRD_D UfigUD;
		
	nSize = m_arUfig.GetSize();
	for(i=0; i < nSize-1; i++)
	{
		for(j=i; j < nSize; j++)
		{
			UfigUD = m_arUfig.GetAt(j);
			nOldKey = UfigUD.key;
					
			if(j == i)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
				continue;
			}

			if(nOldKey < nMinKey)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
			}
		}
		UfigUD = m_arUfig.GetAt(nMinIndex);
		m_arUfig.RemoveAt(nMinIndex);
		m_arUfig.InsertAt(i, UfigUD);
	}

	// UTBL
	T_UTBL_UDRD_D UtblUD;
		
	nSize = m_arUtbl.GetSize();
	for(i=0; i < nSize-1; i++)
	{
		for(j=i; j < nSize; j++)
		{
			UtblUD = m_arUtbl.GetAt(j);
			nOldKey = UtblUD.key;
			
			if(j == i)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
				continue;
			}
			
			if(nOldKey < nMinKey)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
			}
		}
		UtblUD = m_arUtbl.GetAt(nMinIndex);
		m_arUtbl.RemoveAt(nMinIndex);
		m_arUtbl.InsertAt(i, UtblUD);
	}

	// UCHT
	T_UCHT_UDRD_D UchtUD;
		
	nSize = m_arUcht.GetSize();
	for(i=0; i < nSize-1; i++)
	{
		for(j=i; j < nSize; j++)
		{
			UchtUD = m_arUcht.GetAt(j);
			nOldKey = UchtUD.key;
			
			if(j == i)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
				continue;
			}
			
			if(nOldKey < nMinKey)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
			}
		}
		UchtUD = m_arUcht.GetAt(nMinIndex);
		m_arUcht.RemoveAt(nMinIndex);
		m_arUcht.InsertAt(i, UchtUD);
	}

	// UTPL
	T_UTPL_UDRD_D UtplUD;
		
	nSize = m_arUtpl.GetSize();
	for(i=0; i < nSize-1; i++)
	{
		for(j=i; j < nSize; j++)
		{
			UtplUD = m_arUtpl.GetAt(j);
			nOldKey = UtplUD.key;
			
			if(j == i)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
				continue;
			}
			
			if(nOldKey < nMinKey)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
			}
		}
		UtplUD = m_arUtpl.GetAt(nMinIndex);
		m_arUtpl.RemoveAt(nMinIndex);
		m_arUtpl.InsertAt(i, UtplUD);
	}

	// UIMG
	T_UIMG_UDRD_D UimgUD;
		
	nSize = m_arUimg.GetSize();
	for(i=0; i < nSize-1; i++)
	{
		for(j=i; j < nSize; j++)
		{
			UimgUD = m_arUimg.GetAt(j);
			nOldKey = UimgUD.key;
			
			if(j == i)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
				continue;
			}
			
			if(nOldKey < nMinKey)
			{
				nMinKey = nOldKey;
				nMinIndex = j;
			}
		}
		UimgUD = m_arUimg.GetAt(nMinIndex);
		m_arUimg.RemoveAt(nMinIndex);
		m_arUimg.InsertAt(i, UimgUD);
	}
}

BOOL CReportRefDBDlg::InsertModelToDB() 
{ 
	ASSERT(m_pDoc);
	if(!m_bUfig && !m_bUtbl && !m_bUcht && !m_bUtpl && !m_bUimg)
		return FALSE;

	CString strCmdName=_T("");
#if defined(_ORG)
	strCmdName = CString(_T("추가 : Smart Report의 Reference DB"));
#else
	strCmdName = _LS(IDS_TM_DYNA_REPORT_REFDB_ADD);
#endif

	if(!m_pDoc->m_pUndoCtrl->StartEditDB(strCmdName, CMDTYPE_FREE)) return FALSE;

	if(m_bUfig)
	{
		if(!ImportUfig(m_nIDType)) return EndEdit(FALSE);
	}
	if(m_bUtbl)
	{
		if(!ImportUtbl(m_nIDType)) return EndEdit(FALSE);
	}
	if(m_bUcht)
	{
		if(!ImportUcht(m_nIDType)) return EndEdit(FALSE);
	}
	if(m_bUtpl)
	{
		if(!ImportUtpl(m_nIDType)) return EndEdit(FALSE);
	}
	if(m_bUimg)
	{
		if(!ImportUimg(m_nIDType)) return EndEdit(FALSE);
	}

	EndEdit(TRUE);
	
	return TRUE;
}

BOOL CReportRefDBDlg::EndEdit(BOOL bEnd)
{
	ASSERT(m_pDoc);

	if(bEnd && m_pDoc->m_pDataCtrl->CheckModifiedModel())
	{
		BOOL bUpdateDefault=TRUE;  // Modify by pig
		m_pDoc->m_pUndoCtrl->CloseEditDB(bUpdateDefault, FALSE, 0);
		return TRUE;
	}
	else
	{
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	return FALSE;
}

void CReportRefDBDlg::OnRefDBSelectAllBtn()
{
	int nUfig = m_arUfig.GetSize();
	int nUtbl = m_arUtbl.GetSize();
	int nUcht = m_arUcht.GetSize();
	int nUtpl = m_arUtpl.GetSize();
	int nUimg = m_arUimg.GetSize();

	if(nUfig>0) m_bUfig = TRUE;
	if(nUtbl>0) m_bUtbl = TRUE;
	if(nUcht>0) m_bUcht = TRUE;
	if(nUtpl>0) m_bUtpl = TRUE;
	if(nUimg>0) m_bUimg = TRUE;

	UpdateData(FALSE);
}

void CReportRefDBDlg::OnRefDBUnselectAllBtn()
{
	m_bUfig = FALSE;
	m_bUtbl = FALSE;
	m_bUcht = FALSE;
	m_bUtpl = FALSE;
	m_bUimg = FALSE;
	
	UpdateData(FALSE);
}
