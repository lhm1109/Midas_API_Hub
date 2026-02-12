// SeisInfillWall_ReduceStiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisInfillWall_ReduceStiffDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisInfillWall_ReduceStiffDlg dialog


CSeisInfillWall_ReduceStiffDlg::CSeisInfillWall_ReduceStiffDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisInfillWall_ReduceStiffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisInfillWall_ReduceStiffDlg)
	m_nOption = 0;
	m_bSentMessage = FALSE;
	//}}AFX_DATA_INIT
}


void CSeisInfillWall_ReduceStiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisInfillWall_ReduceStiffDlg)
	DDX_Radio(pDX, IDC_TM_ADDREP_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_IGNORE_MEMB_LIST, m_List);
	//}}AFX_DATA_MAP
}

void CSeisInfillWall_ReduceStiffDlg::Execute()
{

}

BEGIN_MESSAGE_MAP(CSeisInfillWall_ReduceStiffDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisInfillWall_ReduceStiffDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_IGNORE_MEMB_LIST, OnItemchanged)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisInfillWall_ReduceStiffDlg message handlers

BOOL CSeisInfillWall_ReduceStiffDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_nOption = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	SetListCtrlHeader();
	MakeItemEx();

	UpdateData(FALSE);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisInfillWall_ReduceStiffDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	T_ELEM_K ElemK=NULL;
	T_ELEM_D ElemD;
	CArray<T_ELEM_K, T_ELEM_K> aFiltedSelKey;
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		ElemK = aSelKey.GetAt(i);
// 		if(nMembType!=m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)) continue;
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
		aFiltedSelKey.Add(ElemK);
	}

	if (aFiltedSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_No_Selected_Member));
		return;
	}

	T_RISS_D data;
	data.Initialize();
	data.bReduceStiff = TRUE;
	
	BOOL bSuccess=FALSE;
	if (m_nOption == 0) // Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddRiss(aFiltedSelKey, data);	
	else if (m_nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelRiss(aFiltedSelKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CSeisInfillWall_ReduceStiffDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CSeisInfillWall_ReduceStiffDlg::InitSelectedItem() // 선택된 요소와 절점을 해제한다.
{  
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}


void CSeisInfillWall_ReduceStiffDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

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
	case D_UPDATE_SEL_ADD: 
	case D_UPDATE_SEL_DEL:
		break;
	default: ASSERT(TRUE);
	}
}

void CSeisInfillWall_ReduceStiffDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_RISS_K Key;
	T_RISS_D Data;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_RISS_ADD):
			{
				pViewBuff->GetRiss(nKey, Key, Data);
				InsertItem(Key);
			}
			break;
		case(UR_RISS_DEL):
			{
				pViewBuff->GetRiss(nKey, Key, Data);
				DeleteItem(Key);
			}
			break;
//     case(UR_RISS_MFS):
//     case(UR_RISS_MFD):
//       {
//         pViewBuff->GetRiss(nKey, Key, Data);
//         DeleteItem(Key);
//         InsertItem(Key);        
//       }
			break;
		default:
			break;
		}
	} 
}

void CSeisInfillWall_ReduceStiffDlg::InsertItem(T_RISS_K Key)
{
	// 삽입 위치를 찾는다.
	T_RISS_D Data;
	int nSize = m_List.GetItemCount();
	if (!m_pDoc->m_pAttrCtrl->GetRiss(Key, Data)) return;
	CString strRissIndex;     
	int nElemKey;  
	int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		strRissIndex = m_List.GetItemText(nCount, 0);
		nElemKey = _ttoi(strRissIndex);
		if(nElemKey > Key) break;
	}  
	strRissIndex.Format(_T("%d"), Key);
	int nIndex = m_List.InsertItem(nCount, strRissIndex);
	
	CString strReduce;
	if(Data.bReduceStiff) strReduce = _LS(IDS_WG_TREEMENU_Reduce_In);
	else strReduce.Empty();

	m_List.SetItemText(nIndex, 1, strReduce);      
	m_List.SetItemData(nIndex, (DWORD)Key);
}

void CSeisInfillWall_ReduceStiffDlg::DeleteItem(T_RISS_K Key)
{
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		if(m_List.GetItemData(nCount) == Key) break;
	}  
	if(nCount == nSize) return; //GSaveHistoryFormatNF(_T("Error : Risser not found!"));
	else m_List.DeleteItem(nCount);  
}

//================================================================
//================= List 관련 작업 함수 ==========================
//================================================================
void CSeisInfillWall_ReduceStiffDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_TREEMENU_Element), _LS(IDS_WG_TREEMENU_Reduce)};  // _T("Index"), _T("Element List")
	int nColWidth[] = {100, 100};
	int nAlign[] = {LVCFMT_CENTER, LVCFMT_CENTER};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	for(int i = 0; i < nColNum; i++)
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

void CSeisInfillWall_ReduceStiffDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	CArray<T_RISS_K, T_RISS_K> aRissK;
	m_pDoc->m_pAttrCtrl->GetRissKeyList(aRissK);

	int nItemCount = aRissK.GetSize();
	if(nItemCount == 0) return;

	int nCount, nIndex;  
	if(nItemCount)
	{
		T_RISS_K Key;
		T_RISS_D Data;
		CString strRissIndex, strReduce;

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key = aRissK[nCount];
			m_pDoc->m_pAttrCtrl->GetRiss(Key, Data);
			strRissIndex.Format(_T("%d"), Key);
			nIndex = m_List.InsertItem(nCount, strRissIndex);      
			
			if(Data.bReduceStiff) strReduce = _LS(IDS_WG_TREEMENU_Reduce_In);
			else strReduce.Empty();

			m_List.SetItemText(nIndex, 1, strReduce);		
			m_List.SetItemData(nIndex, (DWORD)Key);
		}
	}
}

void CSeisInfillWall_ReduceStiffDlg::OnDelKeyPressedInList()
{  
	CArray<T_ELEM_K, T_ELEM_K> aKeyElem;
	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	aKeyElem.RemoveAll();  

	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	for(int i = 0; i < nSize; i++)
	{
		DWORD(KeyElem) = m_List.GetItemData(aSelItem[i]);    
		if(m_pDoc->m_pAttrCtrl->ExistRiss(KeyElem)) aKeyElem.Add(KeyElem);
	}  
	if(m_pDoc->m_pDataCtrl->DelRiss(aKeyElem)) InitSelectedItem();

	SetListFocus(aSelItem);
}

// member를 삭제한 경우 List 박스의 Focus를 다시 맞춤
void CSeisInfillWall_ReduceStiffDlg::SetListFocus(CArray<int, int>& aSelItem)
{  
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	int nFocusItem = aSelItem[0];
	for(int i = 0; i < nSize; i++)
	{    
		if(aSelItem[i] < nFocusItem) nFocusItem = aSelItem[i];
	}
	int nCount = m_List.GetItemCount();
	if(nFocusItem >= nCount) nFocusItem -= 1;
	if(nFocusItem >= 0)
		m_List.SetItemState(nFocusItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

BOOL CSeisInfillWall_ReduceStiffDlg::PreTranslateMessage(MSG* pMsg) 
{
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

void CSeisInfillWall_ReduceStiffDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	 
	if (pNMListView->iSubItem != 999)
	{
		if (!m_bSentMessage)
		{
			m_TempListView = *pNMListView;
			m_TempListView.iSubItem = 999;
			PostMessage(WM_NOTIFY, IDC_TM_IGNORE_MEMB_LIST, (LPARAM)&m_TempListView);
			m_bSentMessage = TRUE;
		}
		*pResult = 0;
		return;
	}
	DisplaySelElemList();  // 선택된 요소를 Display한다. 
	m_bSentMessage = FALSE;
	*pResult = 0;
}

void CSeisInfillWall_ReduceStiffDlg::DisplaySelElemList()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;  
	aSelElem.RemoveAll();
	GetSelElemList(aSelElem);  
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}

void CSeisInfillWall_ReduceStiffDlg::GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem)
{
	CArray<int, int> aSelItem;

	aSelItem.RemoveAll();
	aSelElem.RemoveAll();    

	GetSelectedItemList(&m_List, aSelItem);

	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	for(int i = 0; i < nSize; i++)
	{
		DWORD(KeyMemb) = m_List.GetItemData(aSelItem[i]);    
		aSelElem.Add(KeyMemb);
	} 
}  

void CSeisInfillWall_ReduceStiffDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for(UINT i = 0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}