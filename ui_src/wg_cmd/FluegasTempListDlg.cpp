// FlueListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "FluegasTempListDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "FluegasTempItemDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlueListDlg dialog


CFlueListDlg::CFlueListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CFlueListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlueListDlg)
	//}}AFX_DATA_INIT
}


void CFlueListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlueListDlg)
	DDX_Control(pDX, IDC_FLUE_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 2
/////////////////////////////////////////////////////////////////////////////
// CFlueListDlg implementation functions
void CFlueListDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = { _LS(IDS_CMD_FLUE_name), _LS(IDS_CMD_FLUE_type) };
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
	nColWidth[0] = 160; nColWidth[1] = 68;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
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

void CFlueListDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	if(!bUnitChanged)
		m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountFlue();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_FLUE_K Key;
		T_FLUE_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartFlue();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextFlue(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_FLUE_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetFlue(Key,Data);
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

void CFlueListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_FLUE_K Key, KeyBak;
	T_FLUE_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_FLUE_ADD):
				pViewBuff->GetFlue(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_FLUE_DEL):
				pViewBuff->GetFlue(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_FLUE_MFD):
				pViewBuff->GetFlue(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_FLUE_MFS):
				ASSERT(bMFD);
				pViewBuff->GetFlue(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

void CFlueListDlg::UnitChanged()
{
	return;
}


BOOL CFlueListDlg::InsertItem(T_FLUE_K Key, T_FLUE_D &Data)
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

BOOL CFlueListDlg::DeleteItem(T_FLUE_K Key, T_FLUE_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CFlueListDlg::ModifyItem(T_FLUE_K KeyOld, T_FLUE_K Key, T_FLUE_D &Data)
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

CString CFlueListDlg::DataToStr(int i, T_FLUE_K Key, T_FLUE_D &Data)
{
	CString str;
	CString csType[] = {_LS(IDS_WG_CMD_FLUE_TYPE_CODE), _LS(IDS_WG_CMD_FLUE_TYPE_USER)};

	if(i==0)str = Data.strFlueName;
	else if(i==1) str = csType[Data.nType];
	return str;
}


BEGIN_MESSAGE_MAP(CFlueListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CFlueListDlg)
	ON_BN_CLICKED(IDC_FLUE_BTN_ADD, OnFlueBtnAdd)
	ON_BN_CLICKED(IDC_FLUE_BTN_MODIFY, OnFlueBtnModify)
	ON_BN_CLICKED(IDC_FLUE_BTN_DELETE, OnFlueBtnDelete)
	ON_BN_CLICKED(IDC_FLUE_BTN_CLOSE, OnFlueBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_FLUE_LIST, OnDblclkFlueList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlueListDlg message handlers

BOOL CFlueListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	SetHeaderTitle(TRUE);
	MakeItemEx();
	
	// attach Graph window fhere.
	//SubclassDlgItem(IDC_PLACEHOLDER, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFlueListDlg::OnFlueBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CFlueItemDlg dlg;
	//dlg.SetSeismicEvaluation(m_bSeismicEvaluation);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
}

void CFlueListDlg::OnFlueBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_FLUE_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_flue_function__nC));
		return;
	}
 
	Key = (T_FLUE_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->ExistFlue(Key)) 
	{
		return;
	}

	CFlueItemDlg dlg(Key);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CFlueListDlg::OnFlueBtnDelete() 
{
	// TODO: Add your control notification handler code here

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_flue_function__nC));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelFlue(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CFlueListDlg::OnDblclkFlueList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_FLUE_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_FLUE_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->ExistFlue(Key)) return;

	//CString csOldName = Data.SpecFuncName;
	CFlueItemDlg dlg(Key);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}

void CFlueListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CFlueListDlg::OnFlueBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}
