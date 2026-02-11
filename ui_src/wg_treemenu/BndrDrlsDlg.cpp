// BndrDrlsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrDrlsDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_cmd\wg_cmdAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrDrlsDlg dialog


CBndrDrlsDlg::CBndrDrlsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrDrlsDlg::IDD, pParent)
{
	m_pDoc = 0;
	//{{AFX_DATA_INIT(CBndrDrlsDlg)
	//}}AFX_DATA_INIT
	m_aCtrlOp.Add(IDC_TM_OPTION_ADD);
	m_aCtrlOp.Add(IDC_TM_OPTION_DELETE);
}


void CBndrDrlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrDrlsDlg)
	DDX_Control(pDX, IDC_TM_LIST, m_List);
	//}}AFX_DATA_MAP
}

//--------------------------------------------------------------------------
// Overridables
void CBndrDrlsDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_STAGE_CHANGED     :
	case D_UPDATE_STAGE_DBALL       :
	case D_UPDATE_POST_STAGE_CHANGED:
		MakeItemEx();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}


void CBndrDrlsDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_DRLS_K Key;

	BOOL bDrlsChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_DRLS_ADD):
			{
				pViewBuff->GetDrls(nKey, Key);
				InsertItem(Key);
				bDrlsChanged = TRUE;
			}
			break;
		case(UR_DRLS_DEL):
			{
				pViewBuff->GetDrls(nKey, Key);
				DeleteItem(Key);
				bDrlsChanged = TRUE;
			}
			break;
		default:
			break;
		}
	} // end of while
	if (bDrlsChanged)
	{
		CString csNo;
		int nCount = m_List.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			csNo.Format(_LS(IDS_WG_TREEMENU__d), i+1);
			m_List.SetItemText(i, 0, csNo);
		}
	}
}

//--------------------------------------------------------------------------
// Implementations
void CBndrDrlsDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_TREEMENU_No), _LS(IDS_WG_TREEMENU_Node_Number)};
	int nColWidth[] = {50, 146};
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

void CBndrDrlsDlg::SetItem(int nIndex, T_DRLS_K Key)
{
	CString str;
	for (int i = 0; i < 2; i++)
	{
		str = DataToStr(i, nIndex+1, Key);
		m_List.SetItemText(nIndex, i, str);
	}
	m_List.SetItemData(nIndex, Key);
}

void CBndrDrlsDlg::InsertItem(T_DRLS_K Key)
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

void CBndrDrlsDlg::DeleteItem(T_DRLS_K Key)
{
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (m_List.GetItemData(nCount) == Key) break;
	}
	if (nCount == nSize) GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Key__d__not_found_), Key);
	else m_List.DeleteItem(nCount);
}

void CBndrDrlsDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountDrls();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount;

	if(nItemCount)
	{
		T_DRLS_K Key;
		T_DRLS_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartDrls();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextDrls(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_DRLS_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_List.InsertItem(nCount, _T(""));
			SetItem(nCount, Key);
		}
		delete []KeyBuf;
	}
}

CString CBndrDrlsDlg::DataToStr(int i, int nNo, T_DRLS_K key)
{
	CString value;
	if (i == 0) value.Format(_LS(IDS_WG_TREEMENU__d), nNo);
	else if (i == 1) value.Format(_LS(IDS_WG_TREEMENU__d), key);
	else value = _LS(IDS_WG_TREEMENU_error);
	return value;
}

void CBndrDrlsDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
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

void CBndrDrlsDlg::OnDelKeyPressedInList()
{
	CArray<T_DRLS_K, T_DRLS_K> aKey;
	CArray<int, int> aSelItem;
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	for (int i = 0; i < nSize; i++)
	{
		aKey.Add(m_List.GetItemData(aSelItem[i]));
	}
	m_pDoc->m_pDataCtrl->DelDrls(aKey);
}

BEGIN_MESSAGE_MAP(CBndrDrlsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrDrlsDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrDrlsDlg message handlers

BOOL CBndrDrlsDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOp, 0);

	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();
	// ListCtrl의 값을 설정
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBndrDrlsDlg::PreTranslateMessage(MSG* pMsg) 
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
	return CMenuBarChildDlg::PreTranslateMessage(pMsg);
}

void CBndrDrlsDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CBndrDrlsDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	int nOp;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);

	CArray<T_NODE_K, T_NODE_K> aKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedNodeKeyList(aKey);

	if (aKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}


	BOOL bSuccess=FALSE;

	if (nOp == 0)       // Add
		bSuccess = m_pDoc->m_pDataCtrl->AddDrls(aKey);
	else if (nOp == 1)  // Delete
		bSuccess = m_pDoc->m_pDataCtrl->DelDrls(aKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}
