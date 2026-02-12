// CMMvldListJPDlg.cpp : implementation file
// 2002. 7. 19    by TAE
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldListJPDlg.h"
#include "CMMvldItemJPDlg.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 3

/////////////////////////////////////////////////////////////////////////////
// CCMMvldListJPDlg dialog
//

CCMMvldListJPDlg::CCMMvldListJPDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldListJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldListJPDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
 
	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();
}


void CCMMvldListJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldListJPDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvldListJPDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldListJPDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCMMvldListJPDlg message handlers


////////////////////////////////////////////////////////////////////////////
//
// Initialize
//

// Set List Style && Write Header 
void CCMMvldListJPDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_CMD_MVLD_loadcase), _LS(IDS_CMD_MVLD_Load_Type), _LS(IDS_CMD_MVLD_description)};
	int nColWidth[] = {82, 82, 100};

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

// 리스트에 Item 넣을 위치 찾기(Loadcase ID 기준)
int CCMMvldListJPDlg::FindInsertionPos(int nID)
{
	T_MVLDjp_K key;
	T_MVLDjp_D data;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetMvldjp(key, data);
		if (nID < data.LoadCaseId) break;
	}
	return i;    
}

CString CCMMvldListJPDlg::GetstrLiveLoadType(int nLiveLoadType)
{
	if(nLiveLoadType == 0) return (_LS(IDS_CMD_MVLDjp__Live_Load_Type_L));
	else if(nLiveLoadType == 1) return (_LS(IDS_CMD_MVLDjp__Live_Load_Type_Y));
	else if(nLiveLoadType == 2) return (_LS(IDS_CMD_MVLDjp__Live_Load_Type_T));
	else return (_LS(IDS_CMD_MVLDjp__Live_Load_Type_Err));
}

// Data를 List에 넣을 수 있도록 String으로 변환
CString CCMMvldListJPDlg::DataToStr(int i, T_MVLDjp_K Key, T_MVLDjp_D &Data)
{
	CString str;

	if(i==0) str = Data.LoadCaseName;
	else if(i==1) str = GetstrLiveLoadType(Data.nLiveLoadType);
	else if (i==2) str = Data.Description;
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

// List에 Item 넣기(한줄씩)
BOOL CCMMvldListJPDlg::InsertItem(T_MVLDjp_K Key, T_MVLDjp_D &Data)
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

// List에 Item 채워넣기(전체)
void CCMMvldListJPDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMvldjp();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_MVLDjp_K Key;
		T_MVLDjp_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMvldjp();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextMvldjp(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetMvldjp(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

// 다이얼로그 초기화 - 리스트 초기화
BOOL CCMMvldListJPDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetHeaderTitle();
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////
//
// Add / Modify / Delete
//

//--------------------------------------------------------------------------
// Add
//--------------------------------------------------------------------------
void CCMMvldListJPDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMMvldItemJPDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();	
}

//--------------------------------------------------------------------------
// Delete
//--------------------------------------------------------------------------
BOOL CCMMvldListJPDlg::DeleteItem(T_MVLDjp_K Key, T_MVLDjp_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

void CCMMvldListJPDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_moving_load_c));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelMvldjp(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

//--------------------------------------------------------------------------
// Modify
//--------------------------------------------------------------------------

BOOL CCMMvldListJPDlg::ModifyItem(T_MVLDjp_K KeyOld, T_MVLDjp_K Key, T_MVLDjp_D &Data)
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

void CCMMvldListJPDlg::OnCmdBtnModify() 
{
	
	// TODO: Add your control notification handler code here
	T_MVLDjp_K Key;
	T_MVLDjp_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_moving_load_c));
		return;
	}
 
	Key = (T_MVLDjp_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMvldjp(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Moving_load_case_key__d__), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMMvldItemJPDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);		
}

void CCMMvldListJPDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();	
}


////////////////////////////////////////////////////////////////////////////
//
// DB Update
//
void CCMMvldListJPDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MVLDjp_K Key, KeyBak;
	T_MVLDjp_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MVLDjp_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetMvldjp(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_MVLDjp_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetMvldjp(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_MVLDjp_MFD):
			{
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetMvldjp(nKey, KeyBak, DataBak);
				bMFD = TRUE;
			}
			break;
		case(UR_MVLDjp_MFS): 
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			{
				ASSERT(bMFD);
				pViewBuff->GetMvldjp(nKey, Key, Data);
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

void CCMMvldListJPDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMMvldListJPDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_MVLDjp_K Key;
	T_MVLDjp_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_MVLDjp_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMvldjp(Key, Data)) return;

	CCMMvldItemJPDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}
