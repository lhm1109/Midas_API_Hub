// CMMlsrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMlsrDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMlsrDlg dialog
#define CFormView CCMChildBarBase

//IMPLEMENT_DYNCREATE(CCMMlsrDlg, CFormView)

CCMMlsrDlg::CCMMlsrDlg()
	: CFormView(CCMMlsrDlg::IDD)
{
//  m_pDoc = 0;
	//{{AFX_DATA_INIT(CCMMlsrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlOp.Add(IDC_CMD_OPTION_ADD);
	m_aCtrlOp.Add(IDC_CMD_OPTION_DELETE);
}

CCMMlsrDlg::~CCMMlsrDlg()
{
}

BOOL CCMMlsrDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CFormView::OnInitDialog();
	
	// TODO: Add your specialized code here and/or call the base class

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOp, 0);

	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();
	// ListCtrl의 값을 설정
	MakeItemEx();
	UpdateData(TRUE);  
	return TRUE;
}

void CCMMlsrDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMlsrDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);		
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMlsrDlg, CFormView)
	//{{AFX_MSG_MAP(CCMMlsrDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnClicklist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMMlsrDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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


void CCMMlsrDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MLSR_K Key;

	BOOL bMlsrChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MLSR_ADD):
			{
				pViewBuff->GetMlsr(nKey, Key);
				InsertItem(Key);
				bMlsrChanged = TRUE;
			}
			break;
		case(UR_MLSR_DEL):
			{
				pViewBuff->GetMlsr(nKey, Key);
				DeleteItem(Key);
				bMlsrChanged = TRUE;
			}
			break;
		default:
			break;
		}
	} // end of while
	if (bMlsrChanged)
	{
		CString csNo;
		int nCount = m_List.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			csNo.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3), i+1);
			m_List.SetItemText(i, 0, csNo);
		}
	}
}

//--------------------------------------------------------------------------
// Implementations
void CCMMlsrDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Node_Number)};
	int nColWidth[] = {45, 135};
	int nAlign[] = {LVCFMT_RIGHT, LVCFMT_CENTER};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMlsrDlg::SetItem(int nIndex, T_MLSR_K Key)
{
	CString str;
	for (int i = 0; i < 2; i++)
	{
		str = DataToStr(i, nIndex+1, Key);
		m_List.SetItemText(nIndex, i, str);
	}
	m_List.SetItemData(nIndex, Key);
}

void CCMMlsrDlg::InsertItem(T_MLSR_K Key)
{
	// 삽입 위치를 찾는다.
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (m_List.GetItemData(nCount) > Key) break;
	}
	m_List.InsertItem(nCount, _T(""));
	SetItem(nCount, Key);
}

void CCMMlsrDlg::DeleteItem(T_MLSR_K Key)
{
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (m_List.GetItemData(nCount) == Key) break;
	}
	if (nCount == nSize) GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Key__d__not_found_), Key);
	else m_List.DeleteItem(nCount);
}

void CCMMlsrDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMlsr();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount;

	if(nItemCount)
	{
		T_MLSR_K Key;
		T_MLSR_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMlsr();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextMlsr(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_MLSR_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_List.InsertItem(nCount, _T(""));
			SetItem(nCount, Key);
		}
		delete []KeyBuf;
	}
}

CString CCMMlsrDlg::DataToStr(int i, int nNo, T_MLSR_K key)
{
	CString value;
	if (i == 0) value.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3), nNo);
	else if (i == 1) value.Format(_LS(IDS_CCMLLANJPINFODLG_0829_MSG3), key);
	else value = _LS(IDS_WG_CMD__ADDD__error);
	return value;
}

void CCMMlsrDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;

	// Update all of the selected items.
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for (UINT i=0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

void CCMMlsrDlg::OnDelKeyPressedInList()
{
	CArray<T_MLSR_K, T_MLSR_K> aKey;
	CArray<int, int> aSelItem;
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	for (int i = 0; i < nSize; i++)
	{
		aKey.Add(m_List.GetItemData(aSelItem[i]));
	}
	m_pDoc->m_pDataCtrl->DelMlsr(aKey);
}

BOOL CCMMlsrDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DELETE && GetFocus() == &m_List)
		{
			OnDelKeyPressedInList();
			return TRUE;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////
// CCMMlsrDlg message handlers

void CCMMlsrDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nOp = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);

	CArray<T_NODE_K, T_NODE_K> aKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedNodeKeyList(aKey);

	if (aKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_node_));
		return;
	}

	BOOL bSuccess = FALSE;
	T_MLSR_D MlsrD; MlsrD.Initialize();  
	
	if (nOp == 0)       // Add
		bSuccess = m_pDoc->m_pDataCtrl->AddMlsr(aKey,MlsrD);
	else if (nOp == 1)  // Delete
		bSuccess = m_pDoc->m_pDataCtrl->DelMlsr(aKey);
	
	if(bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMMlsrDlg::OnCmdClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

void CCMMlsrDlg::OnClicklist(NMHDR* pNMHDR, LRESULT* pResult)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	CArray<T_NODE_K, T_NODE_K> aKey;
	CArray<int, int> aSelItem;
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	for (int i = 0; i < nSize; i++)
	{
		aKey.Add(m_List.GetItemData(aSelItem[i]));
	}

	pIGM->UnselectAllNode();
	pIGM->AddSelectedNodeKeyList(aKey);
}
