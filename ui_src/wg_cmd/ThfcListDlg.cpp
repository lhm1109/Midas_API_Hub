// ThfcListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ThfcListDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "ThfcBatchImportItemDlg.h"

#include "ThfcItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThfcListDlg dialog


CThfcListDlg::CThfcListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CThfcListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThfcListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CThfcListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThfcListDlg)
	DDX_Control(pDX, IDC_THFC_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 3
/////////////////////////////////////////////////////////////////////////////
// CThfcListDlg implementation functions
void CThfcListDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Forcing_Function), _LS(IDS_WG_CMD__ADDD__Function_Type), _LS(IDS_WG_CMD__ADDD__Data_Type)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	if (bInit)
	{
		DWORD dwStyle;
		dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	  dwStyle |= LVS_EX_FULLROWSELECT; 
	  ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	}

	// Calculate width for each column
	nColWidth[0] = 128; nColWidth[1] = 94; nColWidth[2] = 72;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		// 제일 처음 시작일 때면 Column 추가
		// 아니면 Column Title만 변경
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		if (bInit)
			m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CThfcListDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	if(!bUnitChanged)
		m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountThfc();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_THFC_K Key;
		T_THFC_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartThfc();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextThfc(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_THFC_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetThfc(Key,Data);
			for(int i = 0; i < COLCOUNT; i++)
			{
				lvitem.iItem=nCount;
				lvitem.iSubItem=i;
				str = DataToStr(i, Key, Data);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT ;
			  if(!bUnitChanged)
			  {
				  if( i == 0 )
					{
						lvitem.mask |= LVIF_PARAM;
						lvitem.lParam = (LPARAM)Key;
					  m_List.InsertItem(&lvitem);
					}
				  else
					  m_List.SetItem(&lvitem);
			  }
			  else
			  {
				  m_List.SetItemText(lvitem.iItem,lvitem.iSubItem,lvitem.pszText);
			  }
			  str.ReleaseBuffer();
			}
		}
		delete []KeyBuf;
	}
}

void CThfcListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		UnitChanged();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CThfcListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_THFC_K Key, KeyBak;
	T_THFC_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_THFC_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetThfc(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_THFC_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetThfc(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_THFC_MFD):
				// 여기서는 저장만 하고 UR_THFC_MFS에서 처리한다.
				pViewBuff->GetThfc(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_THFC_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetThfc(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

void CThfcListDlg::UnitChanged()
{
	// unit 변경과 관련 없다.
	return;
}


BOOL CThfcListDlg::InsertItem(T_THFC_K Key, T_THFC_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iItem = nItem;
		lvitem.iSubItem=i;
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

BOOL CThfcListDlg::DeleteItem(T_THFC_K Key, T_THFC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CThfcListDlg::ModifyItem(T_THFC_K KeyOld, T_THFC_K Key, T_THFC_D &Data)
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

CString CThfcListDlg::DataToStr(int i, T_THFC_K Key, T_THFC_D &Data)
{
	CString str;
	CString csFuncType[] = {_LS(IDS_WG_CMD__ADDD__Time), _LS(IDS_WG_CMD__ADDD__Sinusoidal)};
	CString csDataType[] = {_LS(IDS_WG_CMD__ADDD__NormAcc), _LS(IDS_WG_CMD__ADDD__Accel), 
													_LS(IDS_WG_CMD__ADDD__Force), _LS(IDS_WG_CMD__ADDD__Moment),
													_LS(IDS_WG_CMD__ADDD__Normal)};

	ASSERT(Data.nThisFuncType >= 1 && Data.nThisFuncType <= 2);
	ASSERT(Data.nThisDataType >= 1 && Data.nThisDataType <= 5);

	if(i==0) str = Data.ThisFuncName;
	else if(i==1) str = csFuncType[Data.nThisFuncType-1];
	else if(i==2) str = csDataType[Data.nThisDataType-1];
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}


BEGIN_MESSAGE_MAP(CThfcListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CThfcListDlg)
	ON_BN_CLICKED(IDC_THFC_BTN_ADD, OnThfcBtnAdd)
	ON_BN_CLICKED(IDC_THFC_BTN_ADD2, OnThfcBtnAddSinusoidal)
	ON_BN_CLICKED(IDC_THFC_BTN_ADD3, OnThfcImportBatchesCH)
	ON_BN_CLICKED(IDC_THFC_BTN_MODIFY, OnThfcBtnModify)
	ON_BN_CLICKED(IDC_THFC_BTN_DELETE, OnThfcBtnDelete)
	ON_BN_CLICKED(IDC_THFC_BTN_CLOSE, OnThfcBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_THFC_LIST, OnDblclkThfcList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThfcListDlg message handlers

BOOL CThfcListDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetHeaderTitle(TRUE);
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThfcListDlg::OnThfcBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CThfcItemDlg dlg;
	dlg.m_bSinusoidal = FALSE;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();	
}

void CThfcListDlg::OnThfcBtnAddSinusoidal() 
{
	// TODO: Add your control notification handler code here
	CThfcItemDlg dlg;
	dlg.m_bSinusoidal = TRUE;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();		
}

void CThfcListDlg::OnThfcBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_THFC_K Key;
	T_THFC_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_function));
		return;
	}
 
	Key = (T_THFC_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetThfc(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Time_history_function_key__d_does), Key);
		AfxMessageBox(msg);
		return;
	}

	CThfcItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CThfcListDlg::OnThfcBtnDelete() 
{
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	int nIndex;
	CArray<CString, CString&> arName;
	while (pos)
	{
		nIndex = m_List.GetNextSelectedItem(pos);
		arName.Add(m_List.GetItemText(nIndex, 0));
	}
	if (!m_pDoc->m_pDataCtrl->DelThfc(arName)) return;
	/**
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_function));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelThfc(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);		
		**/
}

void CThfcListDlg::OnDblclkThfcList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_THFC_K Key;
	T_THFC_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_THFC_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetThfc(Key, Data)) return;

	CThfcItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}

void CThfcListDlg::OnThfcBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CThfcListDlg::OnThfcImportBatchesCH()
{
	TCHAR*  szFilter = (LPTSTR)(LPCTSTR)_LS(IDS_WG_CMD__ADDD__DBS_or_text_File);
	//FILE* fp;
	CString strFilePath = GetFilePath();
	CFileDialog	fd(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, szFilter);
	fd.m_ofn.lpstrInitialDir = strFilePath;

	DWORD MAXFILE = 4000;
	fd.m_ofn.nMaxFile = MAXFILE;
	TCHAR* pc = new TCHAR[MAXFILE];
	fd.m_ofn.lpstrFile = pc;
	fd.m_ofn.lpstrFile[0] = NULL;
	CStringArray strFileExtArray;
	if (fd.DoModal() == IDOK)
	{
		POSITION pos = fd.GetStartPosition();
		while (pos)
		{
			CString strFileExt = fd.GetNextPathName(pos);
			strFileExtArray.Add(strFileExt);
		}
	}
	delete[]pc;
	if (strFileExtArray.IsEmpty())
	{
		return;
	}
	CThfcBatchImportItemDlg dlg;
	dlg.m_arStrFileName.Copy(strFileExtArray);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
	
}

CString CThfcListDlg::GetFilePath()
{
	//Remove the extra characters "..", It will affect the default path.
	TCHAR lpFilename[_MAX_PATH + 1];
	GetModuleFileName(NULL, lpFilename, _MAX_PATH);
	TCHAR path_buffer[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath(lpFilename, drive, dir, fname, ext);
	_tcscpy(path_buffer, drive);
	_tcscat(path_buffer, dir);
	CString strFullPath = path_buffer;
	int nFind = 0;// strFullPath.ReverseFind('\\');

	CString s = strFullPath + _T("DBase");
	CString l;
	CString r;

	while (1)
	{
		nFind = s.Find(_T("\\..\\"));
		if (nFind == -1)
			break;
		l = s.Left(nFind);
		int nSubFind = l.ReverseFind('\\');
		l = l.Left(nSubFind);
		r = s.Mid(nFind + 3, s.GetLength());
		s = l + r;
	}

	while (1)
	{
		nFind = s.Find(_T("\\.\\"));
		if (nFind == -1)
			break;
		l = s.Left(nFind);
		r = s.Mid(nFind + 2, s.GetLength());
		s = l + r;
	}

	return s;
}