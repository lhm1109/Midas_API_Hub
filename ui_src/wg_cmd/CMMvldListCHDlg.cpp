// CMMvldListDlg.cpp : implementation file
// 2002. 8. 2    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldListCHDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "CMMvldItemCHDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvldListCHDlg dialog


CCMMvldListCHDlg::CCMMvldListCHDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldListCHDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldListCHDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMvldListCHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldListCHDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 2
/////////////////////////////////////////////////////////////////////////////
// CCMMvldListCHDlg implementation functions
void CCMMvldListCHDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_CMD_MVLD_loadcase), _LS(IDS_CMD_MVLD_description) };
	int nColWidth[] = {100, 164};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

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
		title.ReleaseBuffer();
	}
}

void CCMMvldListCHDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMvldch();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_MVLDch_K Key;
		T_MVLDch_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMvldch();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextMvldch(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetMvldch(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

void CCMMvldListCHDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MVLDch_K Key, KeyBak;
	T_MVLDch_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MVLDch_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetMvldch(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_MVLDch_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetMvldch(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_MVLDch_MFD):
			{
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetMvldch(nKey, KeyBak, DataBak);
				bMFD = TRUE;
			}
			break;
		case(UR_MVLDch_MFS): 
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			{
				ASSERT(bMFD);
				pViewBuff->GetMvldch(nKey, Key, Data);
				DeleteItem(Key, Data);
				InsertItem(KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
			}
			break;
		default:
			break;
		}
	} // end of while
}

int CCMMvldListCHDlg::FindInsertionPos(int nID)
{
	T_MVLDch_K key;
	T_MVLDch_D data;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetMvldch(key, data);
		if (nID < data.LoadCaseId) break;
	}
	return i;    
}

BOOL CCMMvldListCHDlg::InsertItem(T_MVLDch_K Key, T_MVLDch_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Data.LoadCaseId);
	for(int i = 0; i < COLCOUNT; i++)
	{
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

BOOL CCMMvldListCHDlg::DeleteItem(T_MVLDch_K Key, T_MVLDch_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMMvldListCHDlg::ModifyItem(T_MVLDch_K KeyOld, T_MVLDch_K Key, T_MVLDch_D &Data)
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

CString CCMMvldListCHDlg::DataToStr(int i, T_MVLDch_K Key, T_MVLDch_D &Data)
{
	CString str;

	if(i==0) str = Data.LoadCaseName;
	else if (i==1) str = Data.Description;
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

void CCMMvldListCHDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		//ASSERT(FALSE);
		break;
	}
}

BEGIN_MESSAGE_MAP(CCMMvldListCHDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldListCHDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldListCHDlg message handlers

BOOL CCMMvldListCHDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	SetHeaderTitle();
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldListCHDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMMvldItemCHDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
}

void CCMMvldListCHDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_MVLDch_K Key;
	T_MVLDch_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_moving_load_c));
		return;
	}
 
	Key = (T_MVLDch_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMvldch(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Moving_load_case_key__d__), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMMvldItemCHDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvldListCHDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_moving_load_c));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelMvldch(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvldListCHDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CCMMvldListCHDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_MVLDch_K Key;
	T_MVLDch_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_MVLDch_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMvldch(Key, Data)) return;

	CCMMvldItemCHDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}
