// IehpListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpListDlg.h"

#include "..\wg_base\TextFileOut.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "IehpItemDlg.h"
#include "IehpItemDlg_MEC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIehpListDlg dialog
#define D_MAX_SIZE   35    // CSV의 Line 당 읽어들이는 변수의 개수

#define D_NAME        0    // strName
#define D_HGTP        1    // nHingeType
#define D_CDIR        2    // cDir[]
#define D_HGLO        3    // nHingeLocation[], nSectionNum[]
#define D_HYSM        4    // nHysModel[]
#define D_SYMT        5    // nSymmetryType
#define D_STTP        6    // nInitStiffType
#define D_STIF        7    // dInitStiffness
#define D_ALPA        8    // nPAlphaDelta
#define D_FRCU        9    // ForceUnit
#define D_LENU       10    // LengthUnit
#define D_P1PL       11    // P1(+)
#define D_D1PL       12    // A1(+) or D1(+)
#define D_P2PL       13    // P2(+)
#define D_D2PL       14    // A2(+) or D2(+)
#define D_P3PL       15    // P3(+)
#define D_D3PL       16    // A3(+) or D3(+)
#define D_P4PL       17    // P4(+)
#define D_D4PL       18    // A4(+) or D4(+)
#define D_P1MN       19    // P1(-)
#define D_D1MN       20    // A1(-) or D1(-)
#define D_P2MN       21    // P2(-)
#define D_D2MN       22    // A2(-) or D2(-)
#define D_P3MN       23    // P3(-)
#define D_D3MN       24    // A3(-) or D3(-)
#define D_P4MN       25    // P4(-)
#define D_D4MN       26    // A4(-) or D4(-)
#define D_EXPO       27    // dUnloadStiffCalcExpo
#define D_REDU       28    // dUnloadStiffReduFac
#define D_SLPP       29    // dInitGapPositive
#define D_SLPN       30    // dInitGapNegative
#define D_FLST       31    // Flexibility or Stiffness
#define D_STLC       32    // Stiffness Location
#define D_NLSY       33    // Nonlinear shear Y
#define D_NLSZ       34    // Nonlinear shear Z

CIehpListDlg::CIehpListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strInitSelect = _T("");
	m_bInitSelect = FALSE;
	m_aIehpData.RemoveAll();  
}


void CIehpListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpListDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 4

/////////////////////////////////////////////////////////////////////////////
// CIehpListDlg implementation functions
void CIehpListDlg::SetHeaderTitle()
{
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	CString aTitle[] = {_LS(IDS_CMD_STLD_name), _LS(IDS_CMD_HINGE_TYPE), _LS(IDS_CMD_IEHP_DEFINITION), 
											_LS(IDS_CMD_IEHP_COMPONENT)};
	int nColWidth[COLCOUNT] = {100, 84, 84, 84};

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CIehpListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountIehp();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;  
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_IEHP_K Key;
		T_IEHP_D Data;

	  nCount = 0;
		CArray<T_IEHP_K, T_IEHP_K> aIehpKeyList;
		aIehpKeyList.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetIehpKeyList(aIehpKeyList);
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
			Key = aIehpKeyList.GetAt(nCount);
			m_pDoc->m_pAttrCtrl->GetIehp(Key,Data);
			for(int i = 0; i < COLCOUNT; i++)
			{
				lvitem.iItem=nCount;
				lvitem.iSubItem=i;
				str = DataToStr(i, Key, Data);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT ;
				if( i == 0 )
				{
					lvitem.mask |= LVIF_PARAM;
					lvitem.lParam = (LPARAM)Key;
					m_List.InsertItem(&lvitem);
				}
				else
					m_List.SetItem(&lvitem);
			  str.ReleaseBuffer();
			}
		}
	}
}

void CIehpListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}
}

void CIehpListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_IEHP_K Key, KeyBak;
	T_IEHP_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_IEHP_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetIehp(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_IEHP_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetIehp(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_IEHP_MFD):
				// 여기서는 저장만 하고 UR_IEHP_MFS에서 처리한다.
				pViewBuff->GetIehp(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_IEHP_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetIehp(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

BOOL CIehpListDlg::InsertItem(T_IEHP_K Key, T_IEHP_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iItem = nItem;
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	return TRUE;
}

BOOL CIehpListDlg::DeleteItem(T_IEHP_K Key, T_IEHP_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CIehpListDlg::ModifyItem(T_IEHP_K KeyOld, T_IEHP_K Key, T_IEHP_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_List.SetItem(&lvitem);
	}
	return TRUE;
}

CString CIehpListDlg::DataToStr(int i, T_IEHP_K Key, T_IEHP_D &Data)
{  
	CString str, csDof=_T("");
	CString csType[] = {_LS(IDS_CMD_IEHP_LUMPED), _LS(IDS_CMD_IEHP_DISTR),
											_LS(IDS_CMD_IEHP_SPRING), _LS(IDS_CMD_IEHP_TRUSS), _LS(IDS_CMD_IEHP_WALL), _LS(IDS_WG_CMD_POINTSPRING_SUPPORT) };
	CString csDefinition[] = {_LS(IDS_CMD_IEHP_SKELETON), _LS(IDS_CMD_IEHP_FIBER)};
	CString csInteraction[] = {_T("None"), _T("P-M"), _T("P-M-M"),_T("P-M-Multi curve")};  
	ASSERT(Data.nHingeType >= 0 && Data.nHingeType < D_IEHP_HTYP_MAX);
	ASSERT(Data.nDefinition >= 0 && Data.nDefinition <= 1);		
	ASSERT(Data.nInteractionType >= 0 && Data.nInteractionType <= 3);		
	for(int j = 0; j < 6; j++)
	{
		if(Data.cDir[j] == '1') csDof += _T("1");
		else csDof += _T("0");
	}

	if(i==0) str = Data.strName;
	else if(i==1) str = csType[Data.nHingeType];
	else if(i==2) str = csDefinition[Data.nDefinition];
//  else if(i==3) str = csInteraction[Data.nInteractionType];
	else if(i==3) str = csDof;  
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;

}

BEGIN_MESSAGE_MAP(CIehpListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)	
	ON_BN_CLICKED(IDC_CMD_BTN_COPY, OnCmdBtnCopy)	
	ON_BN_CLICKED(IDC_CMD_BTN_IMPORT, OnCmdBtnImport)	
	ON_BN_CLICKED(IDC_CMD_BTN_EXPORT, OnCmdBtnExport)	
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)	
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpListDlg message handlers

BOOL CIehpListDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetHeaderTitle();
	MakeItemEx();	

	if (m_bInitSelect)
		SelectItem(m_strInitSelect);

	return TRUE;
}

void CIehpListDlg::SelectItem(CString str)
{
	int nSize = m_List.GetItemCount();
	for (int i=0; i<nSize; i++)
	{
		if (str.Compare(m_List.GetItemText(i, 0)) == 0)
		{
			m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			break;
		}
	}
}

void CIehpListDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here

#if defined (_MEC)
	CIehpItemDlg_MEC dlg;
#else
	CIehpItemDlg dlg;
#endif
	
	dlg.SetInitPos(D_INIT_POS_RT,5);

	T_IEHG_K IehgK;
	IehgK.key.objK = 0;
	dlg.SetAssignHingeMode(FALSE, IehgK);
	dlg.DoModal();		
}

void CIehpListDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_IEHP_K Key;
	T_IEHP_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_IEHP));  // _T("Error : No selected Inelastic Hinge Property!\nChoose one and try again.")
		return;
	}
 
	Key = (T_IEHP_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetIehp(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_IEHP_KEY_NOT_EXIST), Key);  // _T("Error : Inelastic Hinge Property key %d does not exits!")
		AfxMessageBox(msg);
		return;
	}

#if defined (_MEC)
	CIehpItemDlg_MEC* pDlg = new CIehpItemDlg_MEC();
#else
	CIehpItemDlg* pDlg = new CIehpItemDlg();
#endif
	T_IEHG_K IehgK;
	IehgK.key.objK = -1;
	pDlg->SetAssignHingeMode(FALSE, IehgK);
	pDlg->SetParamData(Key, Data);
	pDlg->SetInitPos(D_INIT_POS_RT,5);
	pDlg->DoModal();
	delete pDlg;

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CIehpListDlg::OnCmdBtnCopy() 
{
	// TODO: Add your control notification handler code here
	T_IEHP_K Key;
	T_IEHP_D Data;
	
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_IEHP));  // _T("Error : No selected Inelastic Hinge Property!\nChoose one and try again.")
		return;
	}

	Key = (T_IEHP_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetIehp(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_IEHP_KEY_NOT_EXIST), Key);  // _T("Error : Inelastic Hinge Property key %d does not exits!")
		AfxMessageBox(msg);
		return;
	}
	T_IEHP_D CopyIehp;
	CopyIehp = Data;
	
	// 새 이름 찾기
	CString strNameTemp;
	int nCopyNameCount = 1;
	while(TRUE)
	{
		strNameTemp.Format(_T("%s-%d"),Data.strName,nCopyNameCount++);
		if(!m_pDoc->m_pAttrCtrl->ExistIehp(strNameTemp))
			break;
	}
	CopyIehp.strName = strNameTemp;
	if(!m_pDoc->m_pDataCtrl->AddIehp(CopyIehp))	return;
	
	MakeItemEx();
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CIehpListDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_IEHP));  // _T("Error : No selected Inelastic Hinge Property!\nChoose one and try again.")
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelIehp(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CIehpListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_IEHP_K Key;
	T_IEHP_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_IEHP_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetIehp(Key, Data)) return;

#if defined (_MEC)
	CIehpItemDlg_MEC dlg;
#else
	CIehpItemDlg dlg;
#endif
	T_IEHG_K IehgK;
	IehgK.key.objK = -1;
	dlg.SetAssignHingeMode(FALSE, IehgK);
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT,5);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}

void CIehpListDlg::SetInitSelect(CString strInit)
{
	m_bInitSelect = TRUE;
	m_strInitSelect = strInit;
}

void CIehpListDlg::OnCmdBtnImport()
{
	OnIehpImportFile();
}

void CIehpListDlg::OnCmdBtnExport()
{
	OnIehpExportFile(); 
}

void CIehpListDlg::OnCmdBtnClose() 
{
	CDialogMove::OnOK();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// CSV Import
/////////////////////////////////////////////////////////////////////////////////////////////
void CIehpListDlg::OnIehpImportFile() 
{	  
#ifdef _JP
	_tsetlocale(LC_ALL, _T("jpn"));
#elif _CH
	_tsetlocale(LC_ALL, _T("CHS"));
#endif

	CString strFilter = _T("CSV File (*.csv)|*.csv||");
	TCHAR * szFilter = _T("CSV File (*.csv)|*.csv||");
	FILE* fileptr;
	CFileDialog	filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);
			
	m_aIehpData.RemoveAll();

	if(filedlg.DoModal() == IDOK) 
	{
		CString strExtension = filedlg.GetFileExt();
		strExtension.MakeUpper();
	  if(strExtension == _T("CSV"))
		{
			if(!(fileptr = _tfopen(filedlg.GetPathName().operator LPCTSTR(), _T("rb"))))
			{ 
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_opening_f));
				return;
			}   
			if(!ReadIehpCSVData(fileptr)) return;       
			if(!SaveIehpCSVData()) return;      
		}
		else 
		{
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_extension_));
			return;
		}
	}
}
	
void CIehpListDlg::OnIehpExportFile() 
{	  
#ifdef _JP
	_tsetlocale(LC_ALL, _T("jpn"));
#elif _CH
	_tsetlocale(LC_ALL, _T("CHS"));
#endif

	CString csFileName = CDBLib::MakeFileNameWithNewExt(_T(".csv"));
	LPCTSTR szFileName;
	if(csFileName.IsEmpty()) szFileName = NULL;
	else szFileName = csFileName;

	CFileDialog fd(FALSE, _T("*.csv"), szFileName,
								 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
								 _T("CSV Files(*.csv)|*.csv|All File(*.*)|*.*||"), NULL);
	if(fd.DoModal() == IDOK) csFileName = fd.GetPathName();
	else return;
	
	// Display Wait Cursor
	CWaitCursor wait;
	
	if(MakeIehpCSVFile(csFileName))
		CDBLib::RunTextEditor(csFileName, _T("CSV File Output of Inealstic Hinge Property"));
}

BOOL CIehpListDlg::MakeIehpCSVFile(LPCTSTR lpFileName)
{  
	TRY
	{
		CFileException FileException;
		if(!m_cfile.Open(lpFileName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);

		if(!WriteIehpCSVData()) 
		{
			m_cfile.Close();      
			return FALSE;
		}

		m_cfile.Close();
	}
	CATCH(CFileException, e)
	{
		e->ReportError();
		m_cfile.Abort();
	}
	END_CATCH  
	return TRUE;  
}

int CIehpListDlg::GetHysModel(CString& strHysModel)
{
	int nHysModel = -1;

	if     (strHysModel == _T("KINEMATIC HARDENING")          || strHysModel == _T("KH"))   nHysModel = 0;
	else if(strHysModel == _T("ORIGIN ORIENTED")              || strHysModel == _T("OO"))   nHysModel = 1;
	else if(strHysModel == _T("PEAK ORIENTED")                || strHysModel == _T("PO"))   nHysModel = 2;
	else if(strHysModel == _T("CLOUGH")                       || strHysModel == _T("C"))    nHysModel = 3;
	else if(strHysModel == _T("DEGRADING TRILINEAR")          || strHysModel == _T("DT"))   nHysModel = 4;
	else if(strHysModel == _T("TAKEDA")                       || strHysModel == _T("T"))    nHysModel = 5;
	else if(strHysModel == _T("TAKEDA TETRALINEAR")           || strHysModel == _T("TT"))   nHysModel = 14;
	else if(strHysModel == _T("MODIFIED TAKEDA")              || strHysModel == _T("MT"))   nHysModel = 6;
	else if(strHysModel == _T("MODIFIED TAKEDA TETRALINEAR")  || strHysModel == _T("MTT"))  nHysModel = 15;
	else if(strHysModel == _T("NORMAL BILINEAR")              || strHysModel == _T("NB"))   nHysModel = 10;
	else if(strHysModel == _T("ELASTIC BILINEAR")             || strHysModel == _T("EB"))   nHysModel = 11;
	else if(strHysModel == _T("ELASTIC TRILINEAR")            || strHysModel == _T("E"))    nHysModel = 12;
	else if(strHysModel == _T("ELASTIC TETRALINEAR")          || strHysModel == _T("ET"))   nHysModel = 13;  
	else if(strHysModel == _T("SLIP BILINEAR")                || strHysModel == _T("SB"))   nHysModel = 30;  
	else if(strHysModel == _T("SLIP BILINEAR/TENSILE")        || strHysModel == _T("SBT"))  nHysModel = 31;  
	else if(strHysModel == _T("SLIP BILINEAR/COMPRESSIVE")    || strHysModel == _T("SBC"))  nHysModel = 32;  
	else if(strHysModel == _T("SLIP TRILINEAR")               || strHysModel == _T("ST"))   nHysModel = 33;  
	else if(strHysModel == _T("SLIP TRILINEAR/TENSILE")       || strHysModel == _T("STT"))  nHysModel = 34;  
	else if(strHysModel == _T("SLIP TRILINEAR/COMPRESSIVE")   || strHysModel == _T("STC"))  nHysModel = 35;
	else if(strHysModel == _T("TAKEDA SLIP")                  || strHysModel == _T("TS"))   nHysModel = D_IEHP_TAKS;
	else if(strHysModel == _T("AXIAL STIFFNESS")              || strHysModel == _T("AX"))   nHysModel = D_IEHP_AXIAL;
	else if(strHysModel == _T("STEEL BUCKLING")               || strHysModel == _T("BCL"))  nHysModel = D_IEHP_BUCK;
	else ASSERT(0);

	return nHysModel;
}

CString CIehpListDlg::GetHysModel(int nHysModel)
{
	CString strHysModel = _T("");

	switch (nHysModel)
	{
	case 0            :   strHysModel = _T("KH");   break;
	case 1            :   strHysModel = _T("OO");   break;
	case 2            :   strHysModel = _T("PO");   break;
	case 3            :   strHysModel = _T("C");   break;
	case 4            :   strHysModel = _T("DT");   break;
	case 5            :   strHysModel = _T("T");   break;
	case 6            :   strHysModel = _T("MT");   break;
	case 7            :   strHysModel = _T("KH");   break;
	case 8            :   strHysModel = _T("T");   break;
	case 9            :   strHysModel = _T("T");   break;
	case 10           :   strHysModel = _T("NB");   break;
	case 11           :   strHysModel = _T("EB");   break;
	case 12           :   strHysModel = _T("E");   break;
	case 13           :   strHysModel = _T("ET");   break;
	case 14           :   strHysModel = _T("TT");   break;
	case 15           :   strHysModel = _T("MTT");   break;
	case 30           :   strHysModel = _T("SB");   break;
	case 31           :   strHysModel = _T("SBT");   break;
	case 32           :   strHysModel = _T("SBC");   break;
	case 33           :   strHysModel = _T("ST");   break;
	case 34           :   strHysModel = _T("STT");   break;
	case 35           :   strHysModel = _T("STC");   break;
	case D_IEHP_TAKS  :   strHysModel = _T("TS");   break;
	case D_IEHP_AXIAL :   strHysModel = _T("AX");   break;
	case D_IEHP_BUCK  :   strHysModel = _T("BCL");   break;

	default:
		ASSERT(0);
		break;
	}

	return strHysModel;
}

BOOL CIehpListDlg::WriteIehpCSVData()
{
	CArray<T_IEHP_K, T_IEHP_K> aIehpKey;
	T_IEHP_D IehpData;
	m_pDoc->m_pAttrCtrl->GetIehpKeyList(aIehpKey);
	int nDataCount = aIehpKey.GetSize();
	if (nDataCount == 0) return TRUE;
		
	CString strData[D_MAX_SIZE];
	CString strHysModelData[D_MAX_SIZE];
	for(int i = 0; i < D_MAX_SIZE; i++) strData[i] = _T("");
		
	CArray<CString, CString&> aLineData;
	aLineData.RemoveAll();  

	CString aHingeType[]  = {_T("L"), _T("D"), _T("S"), _T("T"), _T("W")};
	CString aHingeType_stl[]  = {_T("SL"), _T("SD"), _T("SS"), _T("ST"), _T("SW")};
	CString aHingeLoc[]   = {_T("I"), _T("J"), _T("IJ"), _T("C")};  
	CString aComponent[]  = {_T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6")};
	CString aForceUnit[]  = {_T("kgf"), _T("tonf"), _T("N"), _T("kN"), _T("lbf"), _T("kips")};  
	CString aLengthUnit[] = {_T("mm"), _T("cm"), _T("m"), _T("in"), _T("ft")};
	CString aHingeStiffLoc[] = { _T("I"), _T("J"), _T("C") };

	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	strData[D_FRCU] = aForceUnit[UnitIndex.nBase_Force];
	strData[D_LENU] = aLengthUnit[UnitIndex.nBase_Length];

	for(int i = 0; i < nDataCount; i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetIehp(aIehpKey[i], IehpData)) continue;
		if(!CheckDataValidityforExport(IehpData)) continue;     // 유효한 데이터만 Export 한다.
		strData[D_NAME] = IehpData.strName;    
		strData[D_HGTP] = IehpData.nMaterialType==0? aHingeType[IehpData.nHingeType] : aHingeType_stl[IehpData.nHingeType];

		strData[D_FLST] = (IehpData.nFlexSitf == 0)? _T("YES") : _T("NO");

		for(int nDof = 0; nDof < 6; nDof++)
		{
			if(IehpData.cDir[nDof] != '1') continue;      
			strData[D_CDIR] = aComponent[nDof];
			if(IehpData.nHingeType == 0)       // Lumped
				strData[D_HGLO] = aHingeLoc[IehpData.nHingeLocation[nDof]];
			else if(IehpData.nHingeType == 1)  // Distributed
			{ 
				CString strItem;
				strItem.Format(_T("%d"), IehpData.nSectionNum[nDof]);
				strData[D_HGLO] = strItem;

				if (IehpData.nFlexSitf != 0)
				{
					strData[D_STLC] = aHingeStiffLoc[IehpData.nStifLoc[nDof]];
					strData[D_NLSY] = IehpData.bUseNonlinearShearY ? _T("YES") : _T("NO");
					strData[D_NLSZ] = IehpData.bUseNonlinearShearZ ? _T("YES") : _T("NO");
				}
			}
			else
				strData[D_HGLO] = _T("");

			strData[D_HYSM] = GetHysModel(IehpData.nHysModel[nDof]);
			
			for(int k = 0; k < D_MAX_SIZE; k++) strHysModelData[k] = _T("");

			GetHysModelData4Export(IehpData, nDof, strHysModelData);
						
			strData[D_SYMT] = strHysModelData[D_SYMT];
			strData[D_STTP] = strHysModelData[D_STTP];
			strData[D_STIF] = strHysModelData[D_STIF];
			strData[D_ALPA] = strHysModelData[D_ALPA];
			strData[D_P1PL] = strHysModelData[D_P1PL];
			strData[D_D1PL] = strHysModelData[D_D1PL];
			strData[D_P2PL] = strHysModelData[D_P2PL];
			strData[D_D2PL] = strHysModelData[D_D2PL];
			strData[D_P3PL] = strHysModelData[D_P3PL];
			strData[D_D3PL] = strHysModelData[D_D3PL];
			strData[D_P4PL] = strHysModelData[D_P4PL];
			strData[D_D4PL] = strHysModelData[D_D4PL];
			strData[D_P1MN] = strHysModelData[D_P1MN];
			strData[D_D1MN] = strHysModelData[D_D1MN];
			strData[D_P2MN] = strHysModelData[D_P2MN];
			strData[D_D2MN] = strHysModelData[D_D2MN];
			strData[D_P3MN] = strHysModelData[D_P3MN];
			strData[D_D3MN] = strHysModelData[D_D3MN];
			strData[D_P4MN] = strHysModelData[D_P4MN];
			strData[D_D4MN] = strHysModelData[D_D4MN];
			strData[D_EXPO] = strHysModelData[D_EXPO];
			strData[D_REDU] = strHysModelData[D_REDU];
			strData[D_SLPP] = strHysModelData[D_SLPP];
			strData[D_SLPN] = strHysModelData[D_SLPN];

			double dtempvalue;      
			if(strData[D_STIF] != _T("")) {dtempvalue = _tstof(strData[D_STIF]); strData[D_STIF].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_P1PL] != _T("")) {dtempvalue = _tstof(strData[D_P1PL]); strData[D_P1PL].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_D1PL] != _T("")) {dtempvalue = _tstof(strData[D_D1PL]); strData[D_D1PL].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_P2PL] != _T("")) {dtempvalue = _tstof(strData[D_P2PL]); strData[D_P2PL].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_D2PL] != _T("")) {dtempvalue = _tstof(strData[D_D2PL]); strData[D_D2PL].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_P3PL] != _T("")) {dtempvalue = _tstof(strData[D_P3PL]); strData[D_P3PL].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_D3PL] != _T("")) {dtempvalue = _tstof(strData[D_D3PL]); strData[D_D3PL].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_P4PL] != _T("")) {dtempvalue = _tstof(strData[D_P4PL]); strData[D_P4PL].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_D4PL] != _T("")) {dtempvalue = _tstof(strData[D_D4PL]); strData[D_D4PL].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_P1MN] != _T("")) {dtempvalue = _tstof(strData[D_P1MN]); strData[D_P1MN].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_D1MN] != _T("")) {dtempvalue = _tstof(strData[D_D1MN]); strData[D_D1MN].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_P2MN] != _T("")) {dtempvalue = _tstof(strData[D_P2MN]); strData[D_P2MN].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_D2MN] != _T("")) {dtempvalue = _tstof(strData[D_D2MN]); strData[D_D2MN].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_P3MN] != _T("")) {dtempvalue = _tstof(strData[D_P3MN]); strData[D_P3MN].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_D3MN] != _T("")) {dtempvalue = _tstof(strData[D_D3MN]); strData[D_D3MN].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_P4MN] != _T("")) {dtempvalue = _tstof(strData[D_P4MN]); strData[D_P4MN].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_D4MN] != _T("")) {dtempvalue = _tstof(strData[D_D4MN]); strData[D_D4MN].Format(_T("%.6e"), dtempvalue);}
			if(strData[D_EXPO] != _T("")) {dtempvalue = _tstof(strData[D_EXPO]); strData[D_EXPO].Format(_T("%.3e"), dtempvalue);}
			if(strData[D_REDU] != _T("")) {dtempvalue = _tstof(strData[D_REDU]); strData[D_REDU].Format(_T("%.3e"), dtempvalue);}
			if(strData[D_SLPP] != _T("")) {dtempvalue = _tstof(strData[D_SLPP]); strData[D_SLPP].Format(_T("%.3e"), dtempvalue);}
			if(strData[D_SLPN] != _T("")) {dtempvalue = _tstof(strData[D_SLPN]); strData[D_SLPN].Format(_T("%.3e"), dtempvalue);}
			
			CString strExportData = _T("");
			for(int j = 0; j < D_MAX_SIZE; j++)
			{
				if(j < D_MAX_SIZE - 1) strExportData += strData[j] + _T(", ");
				else strExportData += strData[j];
			}

			//m_cfile.WriteString(strExportData);
			//m_cfile.WriteString(_T("\n"));

			// 변환된 MBCS 문자열 출력
			CStringA strMBCS = CTextFileOut::CString2Mbcs(strExportData);
			m_cfile.Write((const void*)strMBCS, strMBCS.GetLength() * sizeof(char));

			strMBCS = CTextFileOut::CString2Mbcs(_T("\n"));
			m_cfile.Write((const void*)strMBCS, strMBCS.GetLength() * sizeof(char));

		}    
	}

	return TRUE;  
}

BOOL CIehpListDlg::CheckDataValidityforExport(T_IEHP_D& rData)
{  
	if(rData.nDefinition != 0) return FALSE;
	if(rData.nInteractionType != 0) return FALSE;
	return TRUE;
}

void CIehpListDlg::GetHysModelData4Export(T_IEHP_D& IehpData, int nDof, CString* strData)
{
	
	CString aSymmType[]   = {_T("0"), _T("1")};
	CString aInitStiff[]  = {_T("6EI/L"), _T("3EI/L"), _T("2EI/L"), _T("U"), _T("E"), _T("S")};
	CString aAlphaDelta[] = {_T("R"), _T("D")};
	CString strItem;
	if(IehpData.nHysModel[nDof] == 0) // Kinematic Hardening
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].KINEMA.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].KINEMA.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;    
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].KINEMA.nPAlphaDelta];
		if(IehpData.AllProp.PROP[nDof].KINEMA.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;      
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldForce[1]);
			strData[D_P2MN] = strItem;      
			if(IehpData.AllProp.PROP[nDof].KINEMA.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dUltimateForce[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dUltimateForce[1]);
				strData[D_P3MN] = strItem;        
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].KINEMA.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dUltimateMoment[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dUltimateMoment[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].KINEMA.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
	}
	else if(IehpData.nHysModel[nDof] == 1) // Origin Oriented
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].ORIGIN.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].ORIGIN.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].ORIGIN.nPAlphaDelta];
		if(IehpData.AllProp.PROP[nDof].ORIGIN.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldForce[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].ORIGIN.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dUltimateForce[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dUltimateForce[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].ORIGIN.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dUltimateMoment[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dUltimateMoment[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ORIGIN.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
	}
	else if(IehpData.nHysModel[nDof] == 2) // Peak Oriented
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].PEAK.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].PEAK.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].PEAK.nPAlphaDelta];
		if(IehpData.AllProp.PROP[nDof].PEAK.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldForce[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].PEAK.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dUltimateForce[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dUltimateForce[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].PEAK.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dUltimateMoment[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dUltimateMoment[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].PEAK.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
	}
	else if(IehpData.nHysModel[nDof] == 4) // Degrading Trilinear
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].DEGRAD.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].DEGRAD.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].DEGRAD.nPAlphaDelta];
		if(IehpData.AllProp.PROP[nDof].DEGRAD.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldForce[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].DEGRAD.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dUltimateForce[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dUltimateForce[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].DEGRAD.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dUltimateMoment[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dUltimateMoment[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].DEGRAD.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
	}
	else if(IehpData.nHysModel[nDof] == 5 || IehpData.nHysModel[nDof] == 6 || IehpData.nHysModel[nDof] == D_IEHP_TAKS) // 5: Takeda 6: Modified Takeda 74: Takeda Slip
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].TAKEDA.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].TAKEDA.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].TAKEDA.nPAlphaDelta];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dUnloadStiffCalcExpo);
		strData[D_EXPO] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dUnloadStiffReduFac);
		strData[D_REDU] = strItem;    
		if(IehpData.AllProp.PROP[nDof].TAKEDA.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldForce[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].TAKEDA.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dUltimateForce[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dUltimateForce[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].TAKEDA.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dUltimateMoment[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dUltimateMoment[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}

		if(IehpData.nHysModel[nDof] == D_IEHP_TAKS)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKEDA.dPinchingRuleFac);
			strData[D_SLPP] = strItem;
		}
	}
	else if(IehpData.nHysModel[nDof] == 12) // Elastic Trilinear
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].ELATRI.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].ELATRI.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].ELATRI.nPAlphaDelta];
		if(IehpData.AllProp.PROP[nDof].ELATRI.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldForce[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].ELATRI.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dUltimateForce[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dUltimateForce[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].ELATRI.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dUltimateMoment[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dUltimateMoment[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATRI.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
	}
	else if(IehpData.nHysModel[nDof] == 3) // Clougn
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].CLOUGH.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].CLOUGH.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].CLOUGH.nPAlphaDelta];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dUnloadStiffCalcExpo);
		strData[D_EXPO] = strItem;
		if(IehpData.AllProp.PROP[nDof].ELATRI.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dStiffRatio2nd[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dStiffRatio2nd[1]);
			strData[D_D1MN] = strItem;      
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldForce[1]);
			strData[D_P1MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].CLOUGH.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dUltimateForce[0]);
				strData[D_P2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dUltimateForce[1]);
				strData[D_P2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldDisp2nd[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldDisp3rd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldDisp2nd[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldDisp3rd[1]);
				strData[D_D2MN] = strItem;        
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldMoment[1]);
			strData[D_P1MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].CLOUGH.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dUltimateMoment[0]);
				strData[D_P2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dUltimateMoment[1]);
				strData[D_P2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldRotn2nd[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldRotn3rd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldRotn2nd[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].CLOUGH.dYieldRotn3rd[1]);
				strData[D_D2MN] = strItem;        
			}
		}
	}
	else if(IehpData.nHysModel[nDof] == 10) // Normal Bilinear
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].NORBIL.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].NORBIL.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].NORBIL.nPAlphaDelta];
		if(IehpData.AllProp.PROP[nDof].NORBIL.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dStiffRatio2nd[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dStiffRatio2nd[1]);
			strData[D_D1MN] = strItem;      
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldForce[1]);
			strData[D_P1MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].NORBIL.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dUltimateForce[0]);
				strData[D_P2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dUltimateForce[1]);
				strData[D_P2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldDisp2nd[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldDisp3rd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldDisp2nd[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldDisp3rd[1]);
				strData[D_D2MN] = strItem;        
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldMoment[1]);
			strData[D_P1MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].NORBIL.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dUltimateMoment[0]);
				strData[D_P2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dUltimateMoment[1]);
				strData[D_P2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldRotn2nd[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldRotn3rd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldRotn2nd[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].NORBIL.dYieldRotn3rd[1]);
				strData[D_D2MN] = strItem;        
			}
		}
	}
	else if(IehpData.nHysModel[nDof] == 11) // Elastic Bilinear
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].ELABIL.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].ELABIL.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].ELABIL.nPAlphaDelta];
		if(IehpData.AllProp.PROP[nDof].ELATRI.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dStiffRatio2nd[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dStiffRatio2nd[1]);
			strData[D_D1MN] = strItem;      
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldForce[1]);
			strData[D_P1MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].ELABIL.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dUltimateForce[0]);
				strData[D_P2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dUltimateForce[1]);
				strData[D_P2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldDisp2nd[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldDisp3rd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldDisp2nd[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldDisp3rd[1]);
				strData[D_D2MN] = strItem;        
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldMoment[1]);
			strData[D_P1MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].ELABIL.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dUltimateMoment[0]);
				strData[D_P2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dUltimateMoment[1]);
				strData[D_P2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldRotn2nd[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldRotn3rd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldRotn2nd[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELABIL.dYieldRotn3rd[1]);
				strData[D_D2MN] = strItem;        
			}
		}
	}
	else if(IehpData.nHysModel[nDof] == 13) // Elastic Tetralinear
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].ELATET.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].ELATET.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].ELATET.nPAlphaDelta];
		if(IehpData.AllProp.PROP[nDof].ELATET.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dStiffRatio3rd[0]);
			strData[D_D3PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dStiffRatio3rd[1]);
			strData[D_D3MN] = strItem;
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dUltimateForce[0]);
			strData[D_P3PL] = strItem;      
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldForce[1]);
			strData[D_P2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dUltimateForce[1]);
			strData[D_P3MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].ELATET.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dFractureForce[0]);
				strData[D_P4PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dFractureForce[1]);
				strData[D_P4MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldDisp4th[0]);
				strData[D_D4PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldDisp4th[1]);
				strData[D_D4MN] = strItem;
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dUltimateMoment[0]);
			strData[D_P3PL] = strItem;      
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dUltimateMoment[1]);
			strData[D_P3MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].ELATET.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dFractureMoment[0]);
				strData[D_P4PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dFractureMoment[1]);
				strData[D_P4MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldRotn4th[0]);
				strData[D_D4PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].ELATET.dYieldRotn4th[1]);
				strData[D_D4MN] = strItem;
			}
		}
	}
	else if(IehpData.nHysModel[nDof] == 14 || IehpData.nHysModel[nDof] == 15) // 14 : Takeda Tetralinear 15: Modified Takeda Tetralinear
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].TAKTET.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].TAKTET.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].TAKTET.nPAlphaDelta];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dUnloadStiffCalcExpo);
		strData[D_EXPO] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dUnloadStiffReduFac);
		strData[D_REDU] = strItem;    
		if(IehpData.AllProp.PROP[nDof].TAKTET.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dStiffRatio3rd[0]);
			strData[D_D3PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dStiffRatio3rd[1]);
			strData[D_D3MN] = strItem;
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dUltimateForce[0]);
			strData[D_P3PL] = strItem;      
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldForce[1]);
			strData[D_P2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dUltimateForce[1]);
			strData[D_P3MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].TAKTET.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dFractureForce[0]);
				strData[D_P4PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dFractureForce[1]);
				strData[D_P4MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldDisp4th[0]);
				strData[D_D4PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldDisp4th[1]);
				strData[D_D4MN] = strItem;
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dUltimateMoment[0]);
			strData[D_P3PL] = strItem;      
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dUltimateMoment[1]);
			strData[D_P3MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].TAKTET.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dFractureMoment[0]);
				strData[D_P4PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dFractureMoment[1]);
				strData[D_P4MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldRotn4th[0]);
				strData[D_D4PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].TAKTET.dYieldRotn4th[1]);
				strData[D_D4MN] = strItem;
			}
		}
	}
	else if (IehpData.nHysModel[nDof] == 92) // 92: SRC Tetralinear
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].SRCTET.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].SRCTET.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dInitStiffness);
		if (strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].SRCTET.nPAlphaDelta];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dUnloadStiffCalcExpo);
		strData[D_EXPO] = strItem;
		if (IehpData.AllProp.PROP[nDof].SRCTET.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dStiffRatio3rd[0]);
			strData[D_D3PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dStiffRatio3rd[1]);
			strData[D_D3MN] = strItem;
		}
		if (nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dUltimateForce[0]);
			strData[D_P3PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldForce[1]);
			strData[D_P2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dUltimateForce[1]);
			strData[D_P3MN] = strItem;
			if (IehpData.AllProp.PROP[nDof].SRCTET.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dFractureForce[0]);
				strData[D_P4PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dFractureForce[1]);
				strData[D_P4MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldDisp4th[0]);
				strData[D_D4PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldDisp4th[1]);
				strData[D_D4MN] = strItem;
			}
		}
		else if (nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dUltimateMoment[0]);
			strData[D_P3PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dUltimateMoment[1]);
			strData[D_P3MN] = strItem;
			if (IehpData.AllProp.PROP[nDof].SRCTET.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dFractureMoment[0]);
				strData[D_P4PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dFractureMoment[1]);
				strData[D_P4MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldRotn4th[0]);
				strData[D_D4PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SRCTET.dYieldRotn4th[1]);
				strData[D_D4MN] = strItem;
			}
		}
	}
	// 30:SLIP Bilinear, 31:SLIP Bilinear/Tensile, 32:SLIP Bilinear/Compressive
	else if(IehpData.nHysModel[nDof]==30 || IehpData.nHysModel[nDof]==31 || IehpData.nHysModel[nDof]==32)
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].SLIP.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].SLIP.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].SLIP.nPAlphaDelta];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dInitGapPositive);
		strData[D_SLPP] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dInitGapNegative);
		strData[D_SLPN] = strItem;
		if(IehpData.AllProp.PROP[nDof].SLIP.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dStiffRatio2nd[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dStiffRatio2nd[1]);
			strData[D_D1MN] = strItem;      
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldForce[1]);
			strData[D_P1MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].SLIP.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dUltimateForce[0]);
				strData[D_P2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dUltimateForce[1]);
				strData[D_P2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp2nd[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp3rd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp2nd[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp3rd[1]);
				strData[D_D2MN] = strItem;        
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldMoment[1]);
			strData[D_P1MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].SLIP.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dUltimateMoment[0]);
				strData[D_P2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dUltimateMoment[1]);
				strData[D_P2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn2nd[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn3rd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn2nd[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn3rd[1]);
				strData[D_D2MN] = strItem;        
			}
		}
	}
	else if(IehpData.nHysModel[nDof]==33 || IehpData.nHysModel[nDof]==34 || IehpData.nHysModel[nDof]==35)
	{
		strData[D_SYMT] = aSymmType[IehpData.AllProp.PROP[nDof].SLIP.nSymmetryType];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].SLIP.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;
		strData[D_ALPA] = aAlphaDelta[IehpData.AllProp.PROP[nDof].SLIP.nPAlphaDelta];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dInitGapPositive);
		strData[D_SLPP] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dInitGapNegative);
		strData[D_SLPN] = strItem;
		if(IehpData.AllProp.PROP[nDof].SLIP.nPAlphaDelta == 0) // P-Alpha
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dStiffRatio1st[0]);
			strData[D_D1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dStiffRatio2nd[0]);
			strData[D_D2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dStiffRatio1st[1]);
			strData[D_D1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dStiffRatio2nd[1]);
			strData[D_D2MN] = strItem;
		}      
		if(nDof == 0 || nDof == 1 || nDof == 2)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dCrackForce[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldForce[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dCrackForce[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldForce[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].SLIP.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dUltimateForce[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dUltimateForce[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldDisp3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
		else if(nDof == 3 || nDof == 4 || nDof == 5)
		{
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dCrackMoment[0]);
			strData[D_P1PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldMoment[0]);
			strData[D_P2PL] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dCrackMoment[1]);
			strData[D_P1MN] = strItem;
			strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldMoment[1]);
			strData[D_P2MN] = strItem;
			if(IehpData.AllProp.PROP[nDof].SLIP.nPAlphaDelta == 1)  // P-Delta
			{
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dUltimateMoment[0]);
				strData[D_P3PL] = strItem;      
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dUltimateMoment[1]);
				strData[D_P3MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn1st[0]);
				strData[D_D1PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn2nd[0]);
				strData[D_D2PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn3rd[0]);
				strData[D_D3PL] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn1st[1]);
				strData[D_D1MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn2nd[1]);
				strData[D_D2MN] = strItem;
				strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].SLIP.dYieldRotn3rd[1]);
				strData[D_D3MN] = strItem;
			}
		}
	}
	else if(IehpData.nHysModel[nDof]==D_IEHP_AXIAL)
	{
		strData[D_SYMT] = aSymmType[0];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].AXIAL.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].AXIAL.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;

		strItem.Format(_T("%d"), IehpData.AllProp.PROP[nDof].AXIAL.nCompStiffOPt);
		strData[D_ALPA] = strItem;

		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].AXIAL.dStiffRatio1st[0]);
		strData[D_D1PL] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].AXIAL.dStiffRatio2nd[0]);
		strData[D_D2PL] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].AXIAL.dStiffRatio1st[1]);
		strData[D_D1MN] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].AXIAL.dStiffRatio2nd[1]);
		strData[D_D2MN] = strItem;
	 
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].AXIAL.dYieldForce[0]);
		strData[D_P1PL] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].AXIAL.dYieldForce[1]);
		strData[D_P1MN] = strItem;

		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].AXIAL.dYieldReduFac);
		strData[D_D3PL] = strItem;
	}
	else if(IehpData.nHysModel[nDof]==D_IEHP_BUCK)
	{
		strData[D_SYMT] = aSymmType[0];
		strData[D_STTP] = aInitStiff[IehpData.AllProp.PROP[nDof].BUCKL.nInitStiffType];
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].BUCKL.dInitStiffness);
		if(strData[D_STTP] == _T("U")) strData[D_STIF] = strItem;

		strItem.Format(_T("%d"), IehpData.AllProp.PROP[nDof].BUCKL.nBucklingOpt);
		strData[D_ALPA] = strItem;

		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].BUCKL.dStiffRatio1st[0]);
		strData[D_D1PL] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].BUCKL.dStiffRatio1st[1]);
		strData[D_D1MN] = strItem;

		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].BUCKL.dSlednerRatio);
		strData[D_D2PL] = strItem;

		strItem.Format(_T("%d"), IehpData.AllProp.PROP[nDof].BUCKL.bUserKFactor? 1: 0);
		strData[D_D3PL] = strItem;

		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].BUCKL.dKFactor);
		strData[D_D4PL] = strItem;

		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].BUCKL.dYieldForce[0]);
		strData[D_P1PL] = strItem;
		strItem.Format(_T("%g"), IehpData.AllProp.PROP[nDof].BUCKL.dYieldForce[1]);
		strData[D_P1MN] = strItem;
	}
}

///////////////////////////////////////////////////
/*
* IEHP
(1)  strName
(2)  nHingeType
			 Lumped(L)       : 0
			 Distributed(D)  : 1
			 Spring(S)       : 2
			 Truss(T)        : 3
(3)  cDir[]        : Component
			 1           : cDir[0] = '1'  Fx
			 2           : cDir[1] = '1'  Fy
			 3           : cDir[2] = '1'  Fz
			 4           : cDir[3] = '1'  Mx
			 5           : cDir[4] = '1'  My
			 6           : cDir[5] = '1'  Mz
(4)  nHingeLocation[]  : HingeType == Lumped(L)인 경우
			 I           : 0
			 J           : 1
			 I&J(IJ)     : 2
			 Center(C)   : 3
		 nSectionNum[]     : HingeType == Distributed(D)인 경우
		 Blank             : HingeType == Spring, Truss인 경우
(5)  nHysModel[]
			 Kinematic Hardening(KH)           :  0   (Trilinear)
			 Origin Oriented(OO)               :  1   (Trilinear)
			 Peak Oriented(PO)                 :  2   (Trilinear)
			 Clough(C)                         :  3   (Bilinear)
			 Degrading Trilinear(DT)           :  4   (Trilinear)
			 Takeda(T)                         :  5   (Trilinear)
			 Takeda Tetralinear(TT)            : 14   (Tetralinear) 
			 Modified Takeda(MT)               :  6   (Trilinear)  
			 Modified Takeda Tetralinear(MTT)  : 15   (Tetralinear) 
			 Normal Bilinear(NB)               : 10   (Bilinear)
			 Elastic Bilinear(EB)              : 11   (Bilinear)
			 Elastic Trilinear(E)              : 12   (Trilinear) 
			 Elastic Tetralinear(ET)           : 13   (Tetralinear) 
(6)  nSymmetryType
			 0    : 0 (Symmetry)
			 1    : 1 (Asymmetry)
(7)  nInitStiffType 
			 6EI/L    (6EI/L) : 0
			 3EI/L    (3EI/L) : 1
			 2EI/L    (2EI/L) : 2
			 User     (U)     : 3
			 Elastic  (E)     : 4
			 Skeleton (S)     : 5
(8)  dInitStiffness
(9)  nPAlphaDelta
			 Ratio(R)        : 0
			 Displacement(D) : 1
(10) ForceUnit
			 kgf      : 0
			 tonf     : 1
			 N        : 2
			 kN       : 3
			 lbf      : 4
			 kips     : 5 
(11) LengthUnit
			 mm       : 0
			 cm       : 1
			 m        : 2
			 in       : 3  
			 ft       : 4
(12) P1(+)
(13) A1(+)  (nPAlphaDelta == 0) 인 경우 
		 D1(+)  (nPAlphaDelta == 1) 인 경우
(14) P2(+)
(15) A2(+)  (nPAlphaDelta == 0) 인 경우 
		 D2(+)  (nPAlphaDelta == 1) 인 경우
(16) P3(+)
(17) A3(+)  (nPAlphaDelta == 0) 인 경우 
		 D3(+)  (nPAlphaDelta == 1) 인 경우
(18) P4(+)
(19) A4(+)  (nPAlphaDelta == 0) 인 경우 
		 D4(+)  (nPAlphaDelta == 1) 인 경우
(20) P1(-)
(21) A1(-)  (nPAlphaDelta == 0) 인 경우 
		 D1(-)  (nPAlphaDelta == 1) 인 경우
(22) P2(-)
(23) A2(-)  (nPAlphaDelta == 0) 인 경우 
		 D2(-)  (nPAlphaDelta == 1) 인 경우
(24) P3(-)
(25) A3(-)  (nPAlphaDelta == 0) 인 경우 
		 D3(-)  (nPAlphaDelta == 1) 인 경우
(26) P4(-)
(27) A4(-)  (nPAlphaDelta == 0) 인 경우 
		 D4(-)  (nPAlphaDelta == 1) 인 경우
(28) dUnloadStiffCalcExpo
(29) dUnloadStiffReduFac

// 강제설정 항목
** nDefinition      = 0(Skeleton)
** nInteractionType = 0(None)
*/
//////////////////////////////////////////////////
BOOL CIehpListDlg::ReadIehpCSVData(FILE *file)
{
	fpos_t  fpos;
	char   sLine[500];
	CString strType;
	int     nBlankLine = 0;
	int     nTotalBlankLine = 0;
	fgetpos(file, &fpos);

	// Read Iehp CSV Data  
	CArray<CString, CString&> aCSVLineData;
	aCSVLineData.RemoveAll();
	int nLineCount = 0;
	while(TRUE)
	{    
		++nLineCount;
		if(fgets(sLine, 500, file) != NULL)
		{
			CStringA sLineA(sLine);
			CString strLine(sLineA);
			if(strLine.GetLength() < 3)
			{
				nBlankLine++;
				nTotalBlankLine++;
				if(nBlankLine > 10 || nTotalBlankLine > 100) break;  // 연속하여 10 라인 이상 읽지 못하거나, 전체적으로 100 라인 이상 읽기 실패하면 멈춘다.
				else continue;            
			}    
			aCSVLineData.RemoveAll();    
			GetCSVDatabyLine(strLine, aCSVLineData);  
			if(!CheckAndGetIehpData(aCSVLineData, nLineCount)) return FALSE;      
			nBlankLine = 0;
		}
		else 
		{
			nBlankLine++;
			if(nBlankLine > 10 || nTotalBlankLine > 100) break;  // 연속하여 10 라인 이상 읽지 못하거나, 전체적으로 100 라인 이상 읽기 실패하면 멈춘다.
			else continue;            
		}
	}
	fsetpos(file, &fpos);  
	fclose(file);
	
	return TRUE;
}
/*
struct T_IEHP_D
{  
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=Filled type SRC, 3=Encased type SRC, 4=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
															// HYSTERESIS MODEL
															//  0: Kinematic Hardening, 1: Origin-oriented,     2: Peak-oriented,
															//  3: Clough,              4: Degrading Trilinear, 5: Takeda
															//  6: Modified Takeda      7: NonlElaSymD2E,       8: NonlInvDirD2E,       
															//  9: NonlElaAsymD2E      
															// 10: Normal Bilinear(6.6.0추가)     11: Elastic Bilinear(6.6.0추가)
															// 12: Elastic Trilinear(6.6.0추가)   13: Elastic Tetralinear(6.6.0추가) 
															// 14: Takeda Tetralinear(6.6.0추가)  15: Modified Takeda Tetralinear(6.6.0추가)

															// ISOLATOR
															// 51: LRB Isolator Bilinear Type   52: LRB Isolator Trilinear Type
															// 53: High Damping Rubber Isolator 
	T_IEHP_PROP PROP[7];    
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
*/
BOOL CIehpListDlg::CheckAndGetIehpData(CArray<CString, CString&>& aLineData, int nLineCount)
{  
	T_IEHP_D IehpData;
	IehpData.Initialize();
		 
	// Default Data Setting
	IehpData.nDefinition      = 0;  // (Skeleton)
	IehpData.nInteractionType = 0;  // (None)

#define CheckErrorMain(x) CheckErrorVar(nLineCount, x)

	// CSV로부터 읽어들인 Data Setting
	IehpData.strName = aLineData[D_NAME];
	if(IehpData.strName == _T("")) return CheckErrorMain(_LS(IDS_CMD_IEHP_NAME));
	
	CString strHingeType = aLineData[D_HGTP];      
	strHingeType.MakeUpper();
	if     (strHingeType == _T("LUMPED")      || strHingeType == _T("L")) IehpData.nHingeType = 0;
	else if(strHingeType == _T("DISTRIBUTED") || strHingeType == _T("D")) IehpData.nHingeType = 1;
	else if(strHingeType == _T("SPRING")      || strHingeType == _T("S")) IehpData.nHingeType = 2;
	else if(strHingeType == _T("TRUSS")       || strHingeType == _T("T")) IehpData.nHingeType = 3;
	else if(strHingeType == _T("WALL")        || strHingeType == _T("W")) IehpData.nHingeType = 4;
	else if(strHingeType == _T("SL")) { IehpData.nMaterialType = 1; IehpData.nHingeType = 0; }
	else if(strHingeType == _T("SD")) { IehpData.nMaterialType = 1; IehpData.nHingeType = 1; }
	else if(strHingeType == _T("SS")) { IehpData.nMaterialType = 1; IehpData.nHingeType = 2; }
	else if(strHingeType == _T("ST")) { IehpData.nMaterialType = 1; IehpData.nHingeType = 3; }
	else if(strHingeType == _T("SW")) { IehpData.nMaterialType = 1; IehpData.nHingeType = 4; }
	else return CheckErrorMain(_LS(IDS_CMD_IEHP_HINGE_TYPE));

	CString strDir = aLineData[D_CDIR];
	int nDof = 0;
	if     (strDir == _T("1")) {nDof = 0; IehpData.cDir[0] = '1';}
	else if(strDir == _T("2")) {nDof = 1; IehpData.cDir[1] = '1';}
	else if(strDir == _T("3")) {nDof = 2; IehpData.cDir[2] = '1';}
	else if(strDir == _T("4")) {nDof = 3; IehpData.cDir[3] = '1';}
	else if(strDir == _T("5")) {nDof = 4; IehpData.cDir[4] = '1';}
	else if(strDir == _T("6")) {nDof = 5; IehpData.cDir[5] = '1';}  
	else return CheckErrorMain(_LS(IDS_CMD_IEHP_COMPONENT));

#define CheckErrorSub(x)  CheckErrorNameDofVar(IehpData.strName, strDir, x)
	
	if(IehpData.nHingeType == 0) // Lumped
	{
		CString strHingeLocation = aLineData[D_HGLO];
		strHingeLocation.MakeUpper();
		if     (strHingeLocation == _T("I"))                                  IehpData.nHingeLocation[nDof] = 0;
		else if(strHingeLocation == _T("J"))                                  IehpData.nHingeLocation[nDof] = 1;
		else if(strHingeLocation == _T("I&J")    || strHingeLocation == _T("IJ")) IehpData.nHingeLocation[nDof] = 2;
		else if(strHingeLocation == _T("CENTER") || strHingeLocation == _T("C"))  IehpData.nHingeLocation[nDof] = 3;
		else return CheckErrorSub(_LS(IDS_CMD_IEHP_HNGE_LOC));
	}
	else if(IehpData.nHingeType == 1) // Distributed
	{
		CString strSectionNum = aLineData[D_HGLO];     
		IehpData.nSectionNum[nDof] = _ttoi(strSectionNum);
		if(IehpData.nSectionNum[nDof] == 2 || (IehpData.nSectionNum[nDof] < 1 || IehpData.nSectionNum[nDof] > D_IEHP_SECTION_NUM_MAX))
			return CheckErrorSub(_LS(IDS_CMD_IEHP_SECTION_NUM));

		if (aLineData.GetSize() > D_FLST)
		{
			CString strFlexSitf = aLineData[D_FLST];
			strFlexSitf.MakeUpper();
			IehpData.nFlexSitf = (strFlexSitf == _T("NO")) ? 1 : 0;
			if (IehpData.nFlexSitf != 0)
			{
				CString strShear;
				if (aLineData.GetSize() > D_NLSY)
				{
					strShear = aLineData[D_NLSY];
					strShear.MakeUpper();
					IehpData.bUseNonlinearShearY = (strShear == _T("NO")) ? FALSE : TRUE;
				}

				if (aLineData.GetSize() > D_NLSZ)
				{
					strShear = aLineData[D_NLSZ];
					strShear.MakeUpper();
					IehpData.bUseNonlinearShearZ = (strShear == _T("NO")) ? FALSE : TRUE;
				}

				if (aLineData.GetSize() > D_STLC)
				{
					CString strStiffLoc = aLineData[D_STLC];
					if     (strStiffLoc == _T("I"))		IehpData.nStifLoc[nDof] = 0;
					else if(strStiffLoc == _T("J"))		IehpData.nStifLoc[nDof] = 1;
					else															IehpData.nStifLoc[nDof] = 2;
				}
				if(IehpData.nSectionNum[nDof] != 1)
				{
					return CheckErrorSub(_LS(IDS_CMD_IEHP_SECTION_NUM));
				}
			}
		}
	}
	else if(IehpData.nHingeType == 2) // Spring
	{
	}
	else if(IehpData.nHingeType == 3) // truss
	{
	}

	CString strHysModel = aLineData[D_HYSM];
	strHysModel.MakeUpper();
	int nHysModel = GetHysModel(strHysModel);
	if(nHysModel<0) return CheckErrorSub(_LS(IDS_CMD_IEHP_HYS_MODEL));
	IehpData.nHysModel[nDof] = nHysModel;
		
	// 해당 DOF의 Hysteresis Model Data를 채운다.
	if(!SetIehpHysModelData(IehpData, nDof, aLineData)) return FALSE;

	// 이미 읽어서 m_aIehpData에 저장되어 있는 데이터에 새로 읽은 데이터를 통합하여 하나의 완전한 
	// T_IEHP_D 데이터를 완성한다. 이 때, 같은 DB에 저장되어야 하는지의 비교 기준은 strName으로 한다.  
	// 같은 strName에 대해 HingeType등 기본적인 데이터가 다를 경우 사용자에게 처리방법을 묻는다.(유지, 덮어씀, 취소)
	// 각 데이터는 하나의 DOF에 대한 이력 모델을 가지므로, 새로 읽은 데이터가 기존 데이터에서 
	// 사용되지 않는 DOF인 경우에는 기존 데이터에 추가하고, 이미 존재하는 DOF인 경우에는 사용자에게 처리방법을 묻는다.(유지, 덮어씀, 취소)
	T_IEHP_D PrevData;
	BOOL bFindPrevData = FALSE;
	for(int i = 0; i < m_aIehpData.GetSize(); i++)
	{
		PrevData = m_aIehpData[i];
		if(PrevData.strName == IehpData.strName)
		{
			bFindPrevData = TRUE;
			if(!CompareOldandNewData(PrevData, IehpData, nDof)) return FALSE;
			m_aIehpData.SetAt(i, PrevData);    // PrevData = PrevData + NewData
		}
	}   
	if(!bFindPrevData)
	{
		m_aIehpData.Add(IehpData);
	}
		
#undef CheckErrorMain
#undef CheckErrorSub

	return TRUE;
}

BOOL CIehpListDlg::SetIehpHysModelData(T_IEHP_D& rData, int nDof, CArray<CString, CString&>& aLineData) 
{
	CString strDir;
	strDir.Format(_T("%d"), nDof + 1);
#define CheckErrorSub(x)  CheckErrorNameDofVar(rData.strName, strDir, x)
	
	CString strSymmetry = aLineData[D_SYMT];
	int nSymmetryType = 0;
	if     (strSymmetry == _T("0")) nSymmetryType = 0;
	else if(strSymmetry == _T("1")) nSymmetryType = 1;
	else return CheckErrorSub(_LS(IDS_CMD_IEHP_SYM_TYPE));

	CString strInitStiffType = aLineData[D_STTP];
	strInitStiffType.MakeUpper();
	int nInitStiffType = 0;
	if     (strInitStiffType == _T("6EI/L")    || strInitStiffType == _T("6")) nInitStiffType = 0;
	else if(strInitStiffType == _T("3EI/L")    || strInitStiffType == _T("3")) nInitStiffType = 1;
	else if(strInitStiffType == _T("2EI/L")    || strInitStiffType == _T("2")) nInitStiffType = 2;
	else if(strInitStiffType == _T("USER")     || strInitStiffType == _T("U")) nInitStiffType = 3;
	else if(strInitStiffType == _T("ELASTIC")  || strInitStiffType == _T("E")) nInitStiffType = 4;
	else if(strInitStiffType == _T("SKELETON") || strInitStiffType == _T("S")) nInitStiffType = 5;
	else return CheckErrorSub(_LS(IDS_CMD_IEHP_INIT_STIFF));
	
	double dInitStiffness = _tstof(aLineData[D_STIF]);

	CString strPAlphaDelta = aLineData[D_ALPA];
	strPAlphaDelta.MakeUpper();
	int nPAlphaDelta = 0;
	int nSubOption = 0;

	if(rData.nHysModel[nDof]==D_IEHP_AXIAL || rData.nHysModel[nDof]==D_IEHP_BUCK)
	{
		nSubOption = _ttoi(strPAlphaDelta);
	}
	else
	{
		if     (strPAlphaDelta == _T("RATIO")        || strPAlphaDelta == _T("R")) nPAlphaDelta = 0;
		else if(strPAlphaDelta == _T("DISPLACEMENT") || strPAlphaDelta == _T("D")) nPAlphaDelta = 1;
		else return CheckErrorSub(_LS(IDS_CMD_IEHP_ALPHA_DELTA));
	}

	CString strForceUnit = aLineData[D_FRCU];
	strForceUnit.MakeUpper();
	int nForceUnit = 0;
	if     (strForceUnit == _T("KGF")  || strForceUnit == _T("KG"))  nForceUnit = 0;
	else if(strForceUnit == _T("TONF") || strForceUnit == _T("TON")) nForceUnit = 1;
	else if(strForceUnit == _T("N"))                             nForceUnit = 2;
	else if(strForceUnit == _T("KN"))                            nForceUnit = 3;
	else if(strForceUnit == _T("LBF")  || strForceUnit == _T("LB"))  nForceUnit = 4;
	else if(strForceUnit == _T("KIPS") || strForceUnit == _T("KIP")) nForceUnit = 5;
	else return CheckErrorSub(_LS(IDS_CMD_IEHP_FORCE_UNIT));

	CString strLengthUnit = aLineData[D_LENU];
	strLengthUnit.MakeUpper();
	int nLengthUnit = 0;
	if     (strLengthUnit == _T("MM"))                            nLengthUnit = 0;
	else if(strLengthUnit == _T("CM"))                            nLengthUnit = 1;
	else if(strLengthUnit == _T("M"))                             nLengthUnit = 2;
	else if(strLengthUnit == _T("IN") || strLengthUnit == _T("INCH")) nLengthUnit = 3;
	else if(strLengthUnit == _T("FT") || strLengthUnit == _T("FEET")) nLengthUnit = 4;  
	else return CheckErrorSub(_LS(IDS_CMD_IEHP_LENGTH_UNIT));
	
	double  dP1[2], dP2[2], dP3[2], dP4[2], dD1[2], dD2[2], dD3[2], dD4[2], dA1[2], dA2[2], dA3[2];
	for(int i = 0; i < 2; i++)
	{
		dP1[i] = dP2[i] = dP3[i] = dP4[i] = dD1[i] = dD2[i] = dD3[i] = dD4[i] = dA1[i] = dA2[i] = dA3[i] = 0.;
	}
	int nStartNum = D_P1PL;
	for(int i = 0; i < 2; i++)
	{
		dP1[i] = _tstof(aLineData[nStartNum + 0 + i * 8]);
		dP2[i] = _tstof(aLineData[nStartNum + 2 + i * 8]);
		dP3[i] = _tstof(aLineData[nStartNum + 4 + i * 8]);
		dP4[i] = _tstof(aLineData[nStartNum + 6 + i * 8]);    
		if(nPAlphaDelta == 0)  // Alpha
		{
			dA1[i] = _tstof(aLineData[nStartNum + 1 + i * 8]);
			dA2[i] = _tstof(aLineData[nStartNum + 3 + i * 8]);
			dA3[i] = _tstof(aLineData[nStartNum + 5 + i * 8]);      
		}
		else if(nPAlphaDelta == 1)  // Delta
		{
			dD1[i] = _tstof(aLineData[nStartNum + 1 + i * 8]);
			dD2[i] = _tstof(aLineData[nStartNum + 3 + i * 8]);
			dD3[i] = _tstof(aLineData[nStartNum + 5 + i * 8]);
			dD4[i] = _tstof(aLineData[nStartNum + 7 + i * 8]);    
		}
	}

	if (nSymmetryType == 0)  // Symmetric
	{
		dP1[1] = dP1[0];
		dP2[1] = dP2[0];
		dP3[1] = dP3[0];
		dP4[1] = dP4[0];

		dA1[1] = dA1[0];
		dA2[1] = dA2[0];
		dA3[1] = dA3[0];

		dD1[1] = dD1[0];
		dD2[1] = dD2[0];
		dD3[1] = dD3[0];
		dD4[1] = dD4[0];
	}

	if(aLineData[D_EXPO] == _T("")) aLineData[D_EXPO] = _T("0.4");  // 입력 않은 경우 Default 값으로 설정
	if(aLineData[D_REDU] == _T("")) aLineData[D_REDU] = _T("1.0");  // 입력 않은 경우 Default 값으로 설정
	double dUnloadStiffCalcExpo = _tstof(aLineData[D_EXPO]);
	double dUnloadStiffReduFac  = _tstof(aLineData[D_REDU]);
	double dInitGapPositive     = _tstof(aLineData[D_SLPP]);
	double dInitGapNegative     = _tstof(aLineData[D_SLPN]);

	int nCtrlType, nModifiedStiffType;  // Dummy Variable

	int nUserAutoSub     = 0;  // (User Input)
	if(!m_pDoc->m_pEditData->CheckIehpInitStiff(nDof, nUserAutoSub, nInitStiffType, nPAlphaDelta, 
		nUserAutoSub, rData.nHingeType, nCtrlType, nModifiedStiffType))  
		return CheckErrorSub(_LS(IDS_CMD_IEHP_INIT_STIFF));  

	// 각 이력모델별 데이터 셋팅
	BOOL bForce = FALSE;
	if(nDof == 0 || nDof == 1 || nDof == 2) bForce = TRUE;  
	if(rData.nHysModel[nDof] == 0)  // 0:Kinematic Hardening
	{    
		rData.AllProp.PROP[nDof].KINEMA.Initialize();
		rData.AllProp.PROP[nDof].KINEMA.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].KINEMA.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].KINEMA.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].KINEMA.nYieldStrengthOpt = nUserAutoSub;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].KINEMA.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].KINEMA.dCrackForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].KINEMA.dYieldForce[i]     = dP2[i];
			rData.AllProp.PROP[nDof].KINEMA.dUltimateForce[i]  = dP3[i];

			rData.AllProp.PROP[nDof].KINEMA.dCrackMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].KINEMA.dYieldMoment[i]    = dP2[i];
			rData.AllProp.PROP[nDof].KINEMA.dUltimateMoment[i] = dP3[i];

			rData.AllProp.PROP[nDof].KINEMA.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].KINEMA.dStiffRatio2nd[i]  = dA2[i];

			rData.AllProp.PROP[nDof].KINEMA.dYieldDisp1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].KINEMA.dYieldDisp2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].KINEMA.dYieldDisp3rd[i]   = dD3[i];

			rData.AllProp.PROP[nDof].KINEMA.dYieldRotn1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].KINEMA.dYieldRotn2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].KINEMA.dYieldRotn3rd[i]   = dD3[i];
		}      
	}  
	else if(rData.nHysModel[nDof] == 1)  // Origin-oriented
	{     
		rData.AllProp.PROP[nDof].ORIGIN.Initialize();
		rData.AllProp.PROP[nDof].ORIGIN.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].ORIGIN.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].ORIGIN.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].ORIGIN.nYieldStrengthOpt = nUserAutoSub;    
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].ORIGIN.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].ORIGIN.dCrackForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].ORIGIN.dYieldForce[i]     = dP2[i];
			rData.AllProp.PROP[nDof].ORIGIN.dUltimateForce[i]  = dP3[i];

			rData.AllProp.PROP[nDof].ORIGIN.dCrackMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].ORIGIN.dYieldMoment[i]    = dP2[i];
			rData.AllProp.PROP[nDof].ORIGIN.dUltimateMoment[i] = dP3[i];

			rData.AllProp.PROP[nDof].ORIGIN.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].ORIGIN.dStiffRatio2nd[i]  = dA2[i];

			rData.AllProp.PROP[nDof].ORIGIN.dYieldDisp1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].ORIGIN.dYieldDisp2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].ORIGIN.dYieldDisp3rd[i]   = dD3[i];

			rData.AllProp.PROP[nDof].ORIGIN.dYieldRotn1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].ORIGIN.dYieldRotn2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].ORIGIN.dYieldRotn3rd[i]   = dD3[i];
		}
	}  
	else if(rData.nHysModel[nDof] == 2)  // Peak
	{     
		rData.AllProp.PROP[nDof].PEAK.Initialize();
		rData.AllProp.PROP[nDof].PEAK.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].PEAK.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].PEAK.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].PEAK.nYieldStrengthOpt = nUserAutoSub;    
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].PEAK.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].PEAK.dCrackForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].PEAK.dYieldForce[i]     = dP2[i];
			rData.AllProp.PROP[nDof].PEAK.dUltimateForce[i]  = dP3[i];

			rData.AllProp.PROP[nDof].PEAK.dCrackMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].PEAK.dYieldMoment[i]    = dP2[i];
			rData.AllProp.PROP[nDof].PEAK.dUltimateMoment[i] = dP3[i];

			rData.AllProp.PROP[nDof].PEAK.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].PEAK.dStiffRatio2nd[i]  = dA2[i];

			rData.AllProp.PROP[nDof].PEAK.dYieldDisp1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].PEAK.dYieldDisp2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].PEAK.dYieldDisp3rd[i]   = dD3[i];

			rData.AllProp.PROP[nDof].PEAK.dYieldRotn1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].PEAK.dYieldRotn2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].PEAK.dYieldRotn3rd[i]   = dD3[i];
		}
	}  
	else if(rData.nHysModel[nDof] == 3)  // Clough
	{     
		rData.AllProp.PROP[nDof].CLOUGH.Initialize();
		rData.AllProp.PROP[nDof].CLOUGH.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].CLOUGH.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].CLOUGH.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].CLOUGH.nYieldStrengthOpt = nUserAutoSub;
		rData.AllProp.PROP[nDof].CLOUGH.dUnloadStiffCalcExpo = dUnloadStiffCalcExpo;   
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].CLOUGH.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].CLOUGH.dYieldForce[i]     = dP1[i];      
			rData.AllProp.PROP[nDof].CLOUGH.dUltimateForce[i]  = dP2[i];
			
			rData.AllProp.PROP[nDof].CLOUGH.dYieldMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].CLOUGH.dUltimateMoment[i] = dP2[i];

			rData.AllProp.PROP[nDof].CLOUGH.dStiffRatio2nd[i]  = dA1[i];      
			
			rData.AllProp.PROP[nDof].CLOUGH.dYieldDisp2nd[i]   = dD1[i];
			rData.AllProp.PROP[nDof].CLOUGH.dYieldDisp3rd[i]   = dD2[i];
			
			rData.AllProp.PROP[nDof].CLOUGH.dYieldRotn2nd[i]   = dD1[i];
			rData.AllProp.PROP[nDof].CLOUGH.dYieldRotn3rd[i]   = dD2[i];
		}
	}  
	else if(rData.nHysModel[nDof] == 4)  // Degrading Trilinear
	{     
		rData.AllProp.PROP[nDof].DEGRAD.Initialize();
		rData.AllProp.PROP[nDof].DEGRAD.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].DEGRAD.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].DEGRAD.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].DEGRAD.nYieldStrengthOpt = nUserAutoSub;    
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].DEGRAD.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].DEGRAD.dCrackForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].DEGRAD.dYieldForce[i]     = dP2[i];
			rData.AllProp.PROP[nDof].DEGRAD.dUltimateForce[i]  = dP3[i];

			rData.AllProp.PROP[nDof].DEGRAD.dCrackMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].DEGRAD.dYieldMoment[i]    = dP2[i];
			rData.AllProp.PROP[nDof].DEGRAD.dUltimateMoment[i] = dP3[i];

			rData.AllProp.PROP[nDof].DEGRAD.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].DEGRAD.dStiffRatio2nd[i]  = dA2[i];

			rData.AllProp.PROP[nDof].DEGRAD.dYieldDisp1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].DEGRAD.dYieldDisp2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].DEGRAD.dYieldDisp3rd[i]   = dD3[i];

			rData.AllProp.PROP[nDof].DEGRAD.dYieldRotn1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].DEGRAD.dYieldRotn2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].DEGRAD.dYieldRotn3rd[i]   = dD3[i];
		}
	}  
	else if(rData.nHysModel[nDof] == 5 || rData.nHysModel[nDof] == 6 || rData.nHysModel[nDof] == D_IEHP_TAKS)  
	{       // 5: Takeda, 6 : Modified Takeda
		rData.AllProp.PROP[nDof].TAKEDA.Initialize();
		rData.AllProp.PROP[nDof].TAKEDA.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].TAKEDA.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].TAKEDA.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].TAKEDA.nYieldStrengthOpt = nUserAutoSub;
		rData.AllProp.PROP[nDof].TAKEDA.dUnloadStiffCalcExpo = dUnloadStiffCalcExpo;
		rData.AllProp.PROP[nDof].TAKEDA.dUnloadStiffReduFac  = dUnloadStiffReduFac;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].TAKEDA.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].TAKEDA.dCrackForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].TAKEDA.dYieldForce[i]     = dP2[i];
			rData.AllProp.PROP[nDof].TAKEDA.dUltimateForce[i]  = dP3[i];

			rData.AllProp.PROP[nDof].TAKEDA.dCrackMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].TAKEDA.dYieldMoment[i]    = dP2[i];
			rData.AllProp.PROP[nDof].TAKEDA.dUltimateMoment[i] = dP3[i];

			rData.AllProp.PROP[nDof].TAKEDA.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].TAKEDA.dStiffRatio2nd[i]  = dA2[i];

			rData.AllProp.PROP[nDof].TAKEDA.dYieldDisp1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].TAKEDA.dYieldDisp2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].TAKEDA.dYieldDisp3rd[i]   = dD3[i];

			rData.AllProp.PROP[nDof].TAKEDA.dYieldRotn1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].TAKEDA.dYieldRotn2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].TAKEDA.dYieldRotn3rd[i]   = dD3[i];
		}

		if(rData.nHysModel[nDof] == D_IEHP_TAKS)
		{
			rData.AllProp.PROP[nDof].TAKEDA.dPinchingRuleFac = dInitGapPositive;
		}
	}  
	else if(rData.nHysModel[nDof] == 10)  // Normal Bilinear
	{     
		rData.AllProp.PROP[nDof].NORBIL.Initialize();
		rData.AllProp.PROP[nDof].NORBIL.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].NORBIL.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].NORBIL.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].NORBIL.nYieldStrengthOpt = nUserAutoSub;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].NORBIL.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].NORBIL.dYieldForce[i]     = dP1[i];      
			rData.AllProp.PROP[nDof].NORBIL.dUltimateForce[i]  = dP2[i];
			
			rData.AllProp.PROP[nDof].NORBIL.dYieldMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].NORBIL.dUltimateMoment[i] = dP2[i];

			rData.AllProp.PROP[nDof].NORBIL.dStiffRatio2nd[i]  = dA1[i];      
			
			rData.AllProp.PROP[nDof].NORBIL.dYieldDisp2nd[i]   = dD1[i];
			rData.AllProp.PROP[nDof].NORBIL.dYieldDisp3rd[i]   = dD2[i];
			
			rData.AllProp.PROP[nDof].NORBIL.dYieldRotn2nd[i]   = dD1[i];
			rData.AllProp.PROP[nDof].NORBIL.dYieldRotn3rd[i]   = dD2[i];
		}
	}  
	else if(rData.nHysModel[nDof] == 11)  // Elastic Bilinear    
	{     
		rData.AllProp.PROP[nDof].ELABIL.Initialize();
		rData.AllProp.PROP[nDof].ELABIL.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].ELABIL.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].ELABIL.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].ELABIL.nYieldStrengthOpt = nUserAutoSub;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].ELABIL.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].ELABIL.dYieldForce[i]     = dP1[i];      
			rData.AllProp.PROP[nDof].ELABIL.dUltimateForce[i]  = dP2[i];
			
			rData.AllProp.PROP[nDof].ELABIL.dYieldMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].ELABIL.dUltimateMoment[i] = dP2[i];

			rData.AllProp.PROP[nDof].ELABIL.dStiffRatio2nd[i]  = dA1[i];      
			
			rData.AllProp.PROP[nDof].ELABIL.dYieldDisp2nd[i]   = dD1[i];
			rData.AllProp.PROP[nDof].ELABIL.dYieldDisp3rd[i]   = dD2[i];
			
			rData.AllProp.PROP[nDof].ELABIL.dYieldRotn2nd[i]   = dD1[i];
			rData.AllProp.PROP[nDof].ELABIL.dYieldRotn3rd[i]   = dD2[i];
		}
	}  
	else if(rData.nHysModel[nDof] == 12)  // Elastic Trilinear
	{     
		rData.AllProp.PROP[nDof].ELATRI.Initialize();
		rData.AllProp.PROP[nDof].ELATRI.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].ELATRI.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].ELATRI.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].ELATRI.nYieldStrengthOpt = nUserAutoSub;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].ELATRI.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].ELATRI.dCrackForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].ELATRI.dYieldForce[i]     = dP2[i];
			rData.AllProp.PROP[nDof].ELATRI.dUltimateForce[i]  = dP3[i];

			rData.AllProp.PROP[nDof].ELATRI.dCrackMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].ELATRI.dYieldMoment[i]    = dP2[i];
			rData.AllProp.PROP[nDof].ELATRI.dUltimateMoment[i] = dP3[i];

			rData.AllProp.PROP[nDof].ELATRI.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].ELATRI.dStiffRatio2nd[i]  = dA2[i];

			rData.AllProp.PROP[nDof].ELATRI.dYieldDisp1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].ELATRI.dYieldDisp2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].ELATRI.dYieldDisp3rd[i]   = dD3[i];

			rData.AllProp.PROP[nDof].ELATRI.dYieldRotn1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].ELATRI.dYieldRotn2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].ELATRI.dYieldRotn3rd[i]   = dD3[i];
		}
	}  
	else if(rData.nHysModel[nDof] == 13)  // Elastic Tetralinear
	{     
		rData.AllProp.PROP[nDof].ELATET.Initialize();
		rData.AllProp.PROP[nDof].ELATET.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].ELATET.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].ELATET.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].ELATET.nYieldStrengthOpt = nUserAutoSub;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].ELATET.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].ELATET.dCrackForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].ELATET.dYieldForce[i]     = dP2[i];
			rData.AllProp.PROP[nDof].ELATET.dUltimateForce[i]  = dP3[i];
			rData.AllProp.PROP[nDof].ELATET.dFractureForce[i]  = dP4[i];

			rData.AllProp.PROP[nDof].ELATET.dCrackMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].ELATET.dYieldMoment[i]    = dP2[i];
			rData.AllProp.PROP[nDof].ELATET.dUltimateMoment[i] = dP3[i];
			rData.AllProp.PROP[nDof].ELATET.dFractureMoment[i] = dP4[i];

			rData.AllProp.PROP[nDof].ELATET.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].ELATET.dStiffRatio2nd[i]  = dA2[i];
			rData.AllProp.PROP[nDof].ELATET.dStiffRatio3rd[i]  = dA3[i];

			rData.AllProp.PROP[nDof].ELATET.dYieldDisp1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].ELATET.dYieldDisp2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].ELATET.dYieldDisp3rd[i]   = dD3[i];
			rData.AllProp.PROP[nDof].ELATET.dYieldDisp4th[i]   = dD4[i];

			rData.AllProp.PROP[nDof].ELATET.dYieldRotn1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].ELATET.dYieldRotn2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].ELATET.dYieldRotn3rd[i]   = dD3[i];
			rData.AllProp.PROP[nDof].ELATET.dYieldRotn4th[i]   = dD4[i];
		}
	}  
	else if(rData.nHysModel[nDof] == 14 || rData.nHysModel[nDof] == 15)  
	{     // 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear
		rData.AllProp.PROP[nDof].TAKTET.Initialize();
		rData.AllProp.PROP[nDof].TAKTET.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].TAKTET.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].TAKTET.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].TAKTET.nYieldStrengthOpt = nUserAutoSub;
		rData.AllProp.PROP[nDof].TAKTET.dUnloadStiffCalcExpo = dUnloadStiffCalcExpo;
		rData.AllProp.PROP[nDof].TAKTET.dUnloadStiffReduFac  = dUnloadStiffReduFac;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].TAKTET.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].TAKTET.dCrackForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].TAKTET.dYieldForce[i]     = dP2[i];
			rData.AllProp.PROP[nDof].TAKTET.dUltimateForce[i]  = dP3[i];
			rData.AllProp.PROP[nDof].TAKTET.dFractureForce[i]  = dP4[i];

			rData.AllProp.PROP[nDof].TAKTET.dCrackMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].TAKTET.dYieldMoment[i]    = dP2[i];
			rData.AllProp.PROP[nDof].TAKTET.dUltimateMoment[i] = dP3[i];
			rData.AllProp.PROP[nDof].TAKTET.dFractureMoment[i] = dP4[i];

			rData.AllProp.PROP[nDof].TAKTET.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].TAKTET.dStiffRatio2nd[i]  = dA2[i];
			rData.AllProp.PROP[nDof].TAKTET.dStiffRatio3rd[i]  = dA3[i];

			rData.AllProp.PROP[nDof].TAKTET.dYieldDisp1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].TAKTET.dYieldDisp2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].TAKTET.dYieldDisp3rd[i]   = dD3[i];
			rData.AllProp.PROP[nDof].TAKTET.dYieldDisp4th[i]   = dD4[i];

			rData.AllProp.PROP[nDof].TAKTET.dYieldRotn1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].TAKTET.dYieldRotn2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].TAKTET.dYieldRotn3rd[i]   = dD3[i];
			rData.AllProp.PROP[nDof].TAKTET.dYieldRotn4th[i]   = dD4[i];
		}
	}
	else if (rData.nHysModel[nDof] == 92)
	{     // 92:SRC Tetralinear
		rData.AllProp.PROP[nDof].SRCTET.Initialize();
		rData.AllProp.PROP[nDof].SRCTET.nSymmetryType = nSymmetryType;
		rData.AllProp.PROP[nDof].SRCTET.nInitStiffType = nInitStiffType;
		rData.AllProp.PROP[nDof].SRCTET.nPAlphaDelta = nPAlphaDelta;
		rData.AllProp.PROP[nDof].SRCTET.nYieldStrengthOpt = nUserAutoSub;
		rData.AllProp.PROP[nDof].SRCTET.dUnloadStiffCalcExpo = dUnloadStiffCalcExpo;
		if (nInitStiffType == 3) rData.AllProp.PROP[nDof].SRCTET.dInitStiffness = dInitStiffness;
		for (int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].SRCTET.dCrackForce[i] = dP1[i];
			rData.AllProp.PROP[nDof].SRCTET.dYieldForce[i] = dP2[i];
			rData.AllProp.PROP[nDof].SRCTET.dUltimateForce[i] = dP3[i];
			rData.AllProp.PROP[nDof].SRCTET.dFractureForce[i] = dP4[i];

			rData.AllProp.PROP[nDof].SRCTET.dCrackMoment[i] = dP1[i];
			rData.AllProp.PROP[nDof].SRCTET.dYieldMoment[i] = dP2[i];
			rData.AllProp.PROP[nDof].SRCTET.dUltimateMoment[i] = dP3[i];
			rData.AllProp.PROP[nDof].SRCTET.dFractureMoment[i] = dP4[i];

			rData.AllProp.PROP[nDof].SRCTET.dStiffRatio1st[i] = dA1[i];
			rData.AllProp.PROP[nDof].SRCTET.dStiffRatio2nd[i] = dA2[i];
			rData.AllProp.PROP[nDof].SRCTET.dStiffRatio3rd[i] = dA3[i];

			rData.AllProp.PROP[nDof].SRCTET.dYieldDisp1st[i] = dD1[i];
			rData.AllProp.PROP[nDof].SRCTET.dYieldDisp2nd[i] = dD2[i];
			rData.AllProp.PROP[nDof].SRCTET.dYieldDisp3rd[i] = dD3[i];
			rData.AllProp.PROP[nDof].SRCTET.dYieldDisp4th[i] = dD4[i];

			rData.AllProp.PROP[nDof].SRCTET.dYieldRotn1st[i] = dD1[i];
			rData.AllProp.PROP[nDof].SRCTET.dYieldRotn2nd[i] = dD2[i];
			rData.AllProp.PROP[nDof].SRCTET.dYieldRotn3rd[i] = dD3[i];
			rData.AllProp.PROP[nDof].SRCTET.dYieldRotn4th[i] = dD4[i];
		}
		}
	else if(rData.nHysModel[nDof]==30 || rData.nHysModel[nDof]==31 || rData.nHysModel[nDof]==32)
	{
		rData.AllProp.PROP[nDof].SLIP.Initialize();
		rData.AllProp.PROP[nDof].SLIP.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].SLIP.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].SLIP.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].SLIP.nYieldStrengthOpt = nUserAutoSub;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].SLIP.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].SLIP.dYieldForce[i]     = dP1[i];      
			rData.AllProp.PROP[nDof].SLIP.dUltimateForce[i]  = dP2[i];
			
			rData.AllProp.PROP[nDof].SLIP.dYieldMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].SLIP.dUltimateMoment[i] = dP2[i];

			rData.AllProp.PROP[nDof].SLIP.dStiffRatio2nd[i]  = dA1[i];      
			
			rData.AllProp.PROP[nDof].SLIP.dYieldDisp2nd[i]   = dD1[i];
			rData.AllProp.PROP[nDof].SLIP.dYieldDisp3rd[i]   = dD2[i];
			
			rData.AllProp.PROP[nDof].SLIP.dYieldRotn2nd[i]   = dD1[i];
			rData.AllProp.PROP[nDof].SLIP.dYieldRotn3rd[i]   = dD2[i];
		}
		rData.AllProp.PROP[nDof].SLIP.dInitGapPositive = dInitGapPositive;
		rData.AllProp.PROP[nDof].SLIP.dInitGapNegative = dInitGapNegative;
	}
	else if(rData.nHysModel[nDof]==33 || rData.nHysModel[nDof]==34 || rData.nHysModel[nDof]==35)
	{
		rData.AllProp.PROP[nDof].SLIP.Initialize();
		rData.AllProp.PROP[nDof].SLIP.nSymmetryType     = nSymmetryType;
		rData.AllProp.PROP[nDof].SLIP.nInitStiffType    = nInitStiffType;
		rData.AllProp.PROP[nDof].SLIP.nPAlphaDelta      = nPAlphaDelta;
		rData.AllProp.PROP[nDof].SLIP.nYieldStrengthOpt = nUserAutoSub;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].SLIP.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{
			rData.AllProp.PROP[nDof].SLIP.dCrackForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].SLIP.dYieldForce[i]     = dP2[i];
			rData.AllProp.PROP[nDof].SLIP.dUltimateForce[i]  = dP3[i];

			rData.AllProp.PROP[nDof].SLIP.dCrackMoment[i]    = dP1[i];
			rData.AllProp.PROP[nDof].SLIP.dYieldMoment[i]    = dP2[i];
			rData.AllProp.PROP[nDof].SLIP.dUltimateMoment[i] = dP3[i];

			rData.AllProp.PROP[nDof].SLIP.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].SLIP.dStiffRatio2nd[i]  = dA2[i];

			rData.AllProp.PROP[nDof].SLIP.dYieldDisp1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].SLIP.dYieldDisp2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].SLIP.dYieldDisp3rd[i]   = dD3[i];

			rData.AllProp.PROP[nDof].SLIP.dYieldRotn1st[i]   = dD1[i];
			rData.AllProp.PROP[nDof].SLIP.dYieldRotn2nd[i]   = dD2[i];
			rData.AllProp.PROP[nDof].SLIP.dYieldRotn3rd[i]   = dD3[i];
		}
		rData.AllProp.PROP[nDof].SLIP.dInitGapPositive = dInitGapPositive;
		rData.AllProp.PROP[nDof].SLIP.dInitGapNegative = dInitGapNegative;
	}
	else if(rData.nHysModel[nDof]==D_IEHP_AXIAL)
	{
		rData.AllProp.PROP[nDof].AXIAL.Initialize();
		rData.AllProp.PROP[nDof].AXIAL.nInitStiffType     = nInitStiffType;
		rData.AllProp.PROP[nDof].AXIAL.nCompStiffOPt      = nSubOption;
		rData.AllProp.PROP[nDof].AXIAL.nYieldStrengthOpt  = nUserAutoSub;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].AXIAL.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{      
			rData.AllProp.PROP[nDof].AXIAL.dYieldForce[i]     = dP1[i];
			rData.AllProp.PROP[nDof].AXIAL.dStiffRatio1st[i]  = dA1[i];
			rData.AllProp.PROP[nDof].AXIAL.dStiffRatio2nd[i]  = dA2[i];
		}
		rData.AllProp.PROP[nDof].AXIAL.dYieldReduFac = dA3[0];
	}
	else if(rData.nHysModel[nDof]==D_IEHP_BUCK)
	{
		rData.AllProp.PROP[nDof].BUCKL.Initialize();
		rData.AllProp.PROP[nDof].BUCKL.nInitStiffType     = nInitStiffType;
		rData.AllProp.PROP[nDof].BUCKL.nBucklingOpt       = nSubOption;
		rData.AllProp.PROP[nDof].BUCKL.nYieldStrengthOpt  = nUserAutoSub;
		if(nInitStiffType == 3) rData.AllProp.PROP[nDof].BUCKL.dInitStiffness = dInitStiffness;
		for(int i = 0; i < 2; i++)  // Force, Moment 구분하지 않음
		{      
			rData.AllProp.PROP[nDof].BUCKL.dYieldForce[i]    = dP1[i];
			rData.AllProp.PROP[nDof].BUCKL.dStiffRatio1st[i]  = dA1[i];
		}
		rData.AllProp.PROP[nDof].BUCKL.dSlednerRatio = dA2[0];
		rData.AllProp.PROP[nDof].BUCKL.bUserKFactor = _ttoi(aLineData[D_D3PL]);
		rData.AllProp.PROP[nDof].BUCKL.dKFactor = _tstof(aLineData[D_D4PL]);
	}

	// 단위 변환
	T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
	UnitIndexNew=UnitIndexOld;
	UnitIndexNew.nBase_Length = nLengthUnit;
	UnitIndexNew.nBase_Force  = nForceUnit;

	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew); // 읽은 단위계에서 현재 단위계로
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld); // conversion 을 하기 위해서...

	m_pDoc->m_pUnitCtrl->ConvertUnitIehpPrevious(rData, rData.AllProp.PROP, TRUE);
	m_pDoc->m_pUnitCtrl->ConvertUnitIehpPrevious(rData, rData.AllSubProp.PROP, FALSE);
	
#undef CheckErrorSub

	return TRUE;
}

BOOL CIehpListDlg::CheckErrorNameDofVar(LPCTSTR lpszName, LPCTSTR lpszDof, LPCTSTR lpszVar)
{
	// "Inelastic Hinge Property(Name : %s)의 성분 %s 에 정의된 %s이(가) 올바르지 않습니다."
	CString strMsg;
	strMsg.Format(_LS(IDS_CMD_IEHP_IMPORT_ERROR), lpszName, lpszDof, lpszVar);
	AfxMessageBox(strMsg);
	return FALSE;  
}

BOOL CIehpListDlg::CheckErrorVar(int nLineCount, LPCTSTR lpszVar)
{  
	// "Inelastic Hinge Property의 %d 번째 줄에 오류가 있습니다.(항목 : %s)"
	CString strMsg;
	strMsg.Format(_LS(IDS_CMD_IEHP_ERROR_LINE), nLineCount, lpszVar);
	AfxMessageBox(strMsg);  
	return FALSE;
}

BOOL CIehpListDlg::CompareOldandNewData(T_IEHP_D& rPrevData, T_IEHP_D& rNewData, int nDof)
{
	if(rPrevData.nHingeType != rNewData.nHingeType)
	{ // "Inelastic Hinge Property(이름 : %s)의 %d 성분을 Import 하는 중 오류가 발생하였습니다. 
		// \n 이전에 Import한 데이터와 Hinge Type이 일치하지 않습니다. 
		// \n 이전의 Hinge Type을 유지할까요?"
		CString strMsg;
		strMsg.Format(_LS(IDS_CMD_IEHP_HINGE_TYPE_MISMATCH), rPrevData.strName, nDof + 1);
		int nMsgType =AfxMessageBox(strMsg, MB_YESNOCANCEL);
		if(nMsgType == IDYES)  // 넘어감
		{       
		}
		else if(nMsgType == IDNO)
		{
			rPrevData.nHingeType = rNewData.nHingeType;// HingeType 데이터 덮어씀
		}
		else//  if(nMsgType == IDCANCEL)  파일 읽기 중지
			return FALSE;
	}
	if(rPrevData.cDir[nDof] == '1') 
	{
		// "Inelastic Hinge Property(이름 : %s)의 %d 성분을 Import 하는 중 오류가 발생하였습니다. 
		// \n 이미 Import한 성분입니다. 이전의 데이터를 유지할까요?"
		CString strMsg;
		strMsg.Format(_LS(IDS_CMD_IEHP_DOF_ALREADY_EXIST), rPrevData.strName, nDof + 1);
		int nMsgType =AfxMessageBox(strMsg, MB_YESNOCANCEL);
		if(nMsgType == IDYES)  // PrevData를 변경하지 않고 넘어감
		{
			return TRUE;
		}
		else if(nMsgType == IDNO)  // PrevData의 해당 DOF 데이터를 NewData로 채움 
		{
			rPrevData.nHysModel[nDof] = rNewData.nHysModel[nDof];
			rPrevData.AllProp.PROP[nDof] = rNewData.AllProp.PROP[nDof];      
			if(rNewData.nHingeType == 0)  // Lumped
			{
				rPrevData.nHingeLocation[nDof] = rNewData.nHingeLocation[nDof];        
			}
			else if(rNewData.nHingeType == 1)  // Distributed
			{
				rPrevData.nSectionNum[nDof] = rNewData.nSectionNum[nDof];        
			}
		}
		else //   if(nMsgType == IDCANCEL)  // 파일 읽기 중지
			return FALSE;      
	}
	else
	{
		rPrevData.cDir[nDof] = '1';
		rPrevData.nHysModel[nDof] = rNewData.nHysModel[nDof];
		rPrevData.AllProp.PROP[nDof] = rNewData.AllProp.PROP[nDof];      
		if(rNewData.nHingeType == 0)  // Lumped
		{
			rPrevData.nHingeLocation[nDof] = rNewData.nHingeLocation[nDof];        
		}
		else if(rNewData.nHingeType == 1)  // Distributed
		{
			rPrevData.nSectionNum[nDof] = rNewData.nSectionNum[nDof];        
		}
	}
	return TRUE;
}

BOOL CIehpListDlg::SaveIehpCSVData()
{
	T_IEHP_D CSVData;  // CSV로 부터 읽어들인 데이터
	T_IEHP_D OldData;  // 기존의 DB에 있는 데이터
	CArray<CString, CString&> aModifyName;
	CArray<T_IEHP_D, T_IEHP_D&> aModifyData;
	CArray<T_IEHP_D, T_IEHP_D&> aAddData;
	aModifyName.RemoveAll();  
	aModifyData.RemoveAll();
	aAddData.RemoveAll();
	for(int i = 0; i < m_aIehpData.GetSize(); i++)
	{
		CSVData = m_aIehpData.GetAt(i);
		if(m_pDoc->m_pAttrCtrl->ExistIehp(CSVData.strName))
		{      
			if(!m_pDoc->m_pAttrCtrl->GetIehp(CSVData.strName, OldData)) return FALSE;      
			CSVData.nSeq = OldData.nSeq;      
			ConvertDeformIndex(CSVData, OldData);  // Deformation Index 데이터는 기존 데이터의 값을 그대로 사용한다.
			aModifyName.Add(OldData.strName);      
			aModifyData.Add(CSVData);      
		}
		else
		{
			aAddData.Add(CSVData);
		}
	}
	
	// 기존에 DB 에 있는 데이터는 수정한다.
	if(aModifyName.GetSize() > 0)
	{
		if(aModifyName.GetSize() != aModifyData.GetSize()) ASSERT(FALSE);
		if(!m_pDoc->m_pDataCtrl->ModifyIehp(aModifyName, aModifyData)) return FALSE;
	}
	// 새로운 데이터는 DB에 추가한다.
	if(aAddData.GetSize() > 0)
	{
		if(!m_pDoc->m_pDataCtrl->AddIehp(aAddData)) return FALSE;    
	}
	return TRUE;
}

void CIehpListDlg::ConvertDeformIndex(T_IEHP_D& CSVData, T_IEHP_D& OldData)
{  
	for(int nDof = 0; nDof < 6; nDof++)
	{
		// Import 데이터와 기존 데이터가 해당 DOF를 모두 사용할 때 
		// 기존 데이터의 deformation index 값을 Import 데이터로 가져온다. 
		if(CSVData.cDir[nDof] == '1' && OldData.cDir[nDof] == '1')  
		{
			// 해당 DOF의 이력 모델이 같은 경우에만 deformation index를 가져온다.
			if(CSVData.nHysModel[nDof] == OldData.nHysModel[nDof]) 
			{
				if(CSVData.nHysModel[nDof] == 0)  //  Kinematic Hardening
				{
					CSVData.AllProp.PROP[nDof].KINEMA.nDeformDefineType = OldData.AllProp.PROP[nDof].KINEMA.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].KINEMA.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].KINEMA.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 1)  //  Origin-oriented
				{
					CSVData.AllProp.PROP[nDof].ORIGIN.nDeformDefineType = OldData.AllProp.PROP[nDof].ORIGIN.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].ORIGIN.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].ORIGIN.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 2)  //  Peak
				{
					CSVData.AllProp.PROP[nDof].PEAK.nDeformDefineType = OldData.AllProp.PROP[nDof].PEAK.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].PEAK.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].PEAK.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 3)  //  Clough
				{
					CSVData.AllProp.PROP[nDof].CLOUGH.nDeformDefineType = OldData.AllProp.PROP[nDof].CLOUGH.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].CLOUGH.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].CLOUGH.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 4)  //  Degrading Trilinear
				{
					CSVData.AllProp.PROP[nDof].DEGRAD.nDeformDefineType = OldData.AllProp.PROP[nDof].DEGRAD.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].DEGRAD.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].DEGRAD.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 5 || CSVData.nHysModel[nDof] == 6 || CSVData.nHysModel[nDof] == D_IEHP_TAKS)  //  5: Takeda, 6 : Modified Takeda, 74: Takeda Slip
				{
					CSVData.AllProp.PROP[nDof].TAKEDA.nDeformDefineType = OldData.AllProp.PROP[nDof].TAKEDA.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].TAKEDA.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].TAKEDA.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 10)  //  Normal Bilinear
				{
					CSVData.AllProp.PROP[nDof].NORBIL.nDeformDefineType = OldData.AllProp.PROP[nDof].NORBIL.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].NORBIL.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].NORBIL.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 11)  //  Elastic Bilinear    g
				{
					CSVData.AllProp.PROP[nDof].ELABIL.nDeformDefineType = OldData.AllProp.PROP[nDof].ELABIL.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].ELABIL.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].ELABIL.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 12)  //  Elastic Trilinear
				{
					CSVData.AllProp.PROP[nDof].ELATRI.nDeformDefineType = OldData.AllProp.PROP[nDof].ELATRI.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].ELATRI.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].ELATRI.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 13)  //  Elastic Tetralinear
				{
					CSVData.AllProp.PROP[nDof].ELATET.nDeformDefineType = OldData.AllProp.PROP[nDof].ELATET.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].ELATET.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].ELATET.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == 14 || CSVData.nHysModel[nDof] == 15)  //  14:Takeda Tetralinear, 15:Modified Takeda Tetralinear
				{
					CSVData.AllProp.PROP[nDof].TAKTET.nDeformDefineType = OldData.AllProp.PROP[nDof].TAKTET.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].TAKTET.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].TAKTET.dDeformCapacity[i][j];
				}
				else if (CSVData.nHysModel[nDof] == 92)  //  92:SRC Tetralinear
				{
					CSVData.AllProp.PROP[nDof].SRCTET.nDeformDefineType = OldData.AllProp.PROP[nDof].SRCTET.nDeformDefineType;
					for (int i = 0; i < 2; i++)
						for (int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].SRCTET.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].SRCTET.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof]==30 || CSVData.nHysModel[nDof]==31 || CSVData.nHysModel[nDof]==32 ||
								CSVData.nHysModel[nDof]==33 || CSVData.nHysModel[nDof]==34 || CSVData.nHysModel[nDof]==35)
				{
					CSVData.AllProp.PROP[nDof].SLIP.nDeformDefineType = OldData.AllProp.PROP[nDof].SLIP.nDeformDefineType;
					for(int i = 0; i < 2; i++) for(int j = 0; j < 5; j++)
						CSVData.AllProp.PROP[nDof].SLIP.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].SLIP.dDeformCapacity[i][j];
				}
				else if(CSVData.nHysModel[nDof] == D_IEHP_AXIAL)
				{
					CSVData.AllProp.PROP[nDof].AXIAL.nDeformDefineType = OldData.AllProp.PROP[nDof].AXIAL.nDeformDefineType;
					for(int i = 0; i < 2; i++)
						for(int j = 0; j < 5; j++)
							CSVData.AllProp.PROP[nDof].AXIAL.dDeformCapacity[i][j] = OldData.AllProp.PROP[nDof].AXIAL.dDeformCapacity[i][j];
				}
			}
		}
	}  
}

void CIehpListDlg::GetCSVDatabyLine(CString strLine, CArray<CString, CString&>& aCSVLineData)
{
	aCSVLineData.RemoveAll();
	strLine.TrimLeft(); 
	strLine.TrimRight();
	
	int i = 0, j = 0;
	int nLength = 0;
	CString	sVal;	
	
	nLength = strLine.GetLength();
	for(i = 0; i < nLength; i++)
	{
		strLine.Remove('\t');         // tab 문자를 제거한다.
	}

	if(strLine.Find(',') >= 0)      // ','가 있는 최초의 위치를 찾는다.(0부터 시작)
	{		
		while(TRUE)                   // an infinite loop
		{
			i = strLine.Find(',');      // ','가 있는 최초의 위치를 찾는다.(0부터 시작)
			if(i >= 0)                  // ','가 있는 경우
			{
				sVal = strLine.Left(i);
				sVal.TrimLeft();
				sVal.TrimRight();        
				aCSVLineData.Add(sVal);     
				strLine.Delete(0, i+1);   // delete a character or characters from a string : 0에서 시작하여 i+1개 지움 
			}
			else                        // ','가 없는 경우 : 제일 마지막 데이터를 저장한다.
			{
				sVal = strLine;				
				sVal.TrimLeft();
				sVal.TrimRight();        
				aCSVLineData.Add(sVal);
				break;
			}
		}
	}
	else 
	{
		sVal = strLine; 
		sVal.TrimLeft();
		sVal.TrimRight();    
		aCSVLineData.Add(sVal);
	}

	// 필요한 데이터 개수 만틈 채우기 위하여 입력받지 않은 값은 공백으로 채운다.
	int nDataCount = aCSVLineData.GetSize();  
	CString strBlank = _T("");
	if(nDataCount < D_MAX_SIZE)
	{
		for(int i = nDataCount; i < D_MAX_SIZE; i++)
			aCSVLineData.Add(strBlank);
	}
	aCSVLineData.FreeExtra();  // Frees any extra memory that was allocated while the array was grown
}