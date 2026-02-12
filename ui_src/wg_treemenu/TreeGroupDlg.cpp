// TreeGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "TreeGroupDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_common\wg_commonAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CGroupTreeCtrl::ProcessReturnKey(CEdit* pEdit)
{
	pEdit->DestroyWindow();
	return TRUE;
}

BOOL CGroupTreeCtrl::ProcessEscapeKey(CEdit* pEdit)
{
	SetItemText(m_hEditItem, m_csEditLabel);
	pEdit->SetWindowText(m_csEditLabel);
	pEdit->DestroyWindow();
	return TRUE;
}

void CGroupTreeCtrl::SaveEditItem(HTREEITEM hItem)
{
	m_hEditItem = hItem;
	m_csEditLabel = GetItemText(hItem);
}

HTREEITEM CGroupTreeCtrl::FindNextItem(TV_ITEM* pItem, HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	
	TV_ITEM hNextItem;

	//Clear Item data
	ZeroMemory(&hNextItem, sizeof(hNextItem));
		 
	//The mask is used to retrieve the data to compare
	hNextItem.mask = pItem->mask;
	hNextItem.hItem = (hItem) ? GetNextItem(hItem) : GetRootItem();
	
	//Prepare to compare pszText
	//Testing pItem->pszText protects the code from a client setting the
	//TVIF_TEXT bit but passing in a NULL pointer.
	if((pItem->mask & TVIF_TEXT) && pItem->pszText)
	{
		hNextItem.cchTextMax = static_cast<int>(_tcslen(pItem->pszText));

		if(hNextItem.cchTextMax)
			 hNextItem.pszText = new TCHAR[++hNextItem.cchTextMax];
	}

	while(hNextItem.hItem)
	{
		if(Compare(pItem, hNextItem))
		{         
			//Copy all the information into pItem and return
			memcpy(pItem, &hNextItem, sizeof(TV_ITEM));

			//Free resources
			if(hNextItem.pszText)
				 delete hNextItem.pszText;
			
			return pItem->hItem;
		}

		//The mask is used to retrieve the data to compare and must be
		//reset before calling Compare
		hNextItem.mask = pItem->mask;
		hNextItem.hItem = GetNextItem(hNextItem.hItem);
	}   
	
	//Set hItem in pItem
	pItem->hItem = NULL;

	//Free resources
	if(hNextItem.pszText)
		 delete hNextItem.pszText;
	
	return NULL;
}

HTREEITEM CGroupTreeCtrl::GetNextItem(HTREEITEM hItem, UINT nCode)
{
	return CBaseTreeCtrl::GetNextItem(hItem, nCode); 
}

HTREEITEM CGroupTreeCtrl::GetNextItem(HTREEITEM hItem)
{
	HTREEITEM hti;

	if(ItemHasChildren(hItem))
		return GetChildItem(hItem);           // return first child
	else
	{
		// return next sibling item
		// Go up the tree to find a parent's sibling if needed.
		while((hti = GetNextSiblingItem(hItem)) == NULL)
		{
			if((hItem = GetParentItem(hItem)) == NULL) return NULL;
		}
	}
	return hti;
}

BOOL CGroupTreeCtrl::Compare(TV_ITEM* pItem, TV_ITEM& tvTempItem)
{
	 //This call uses the .mask setting to just retrieve the values
	 //that the client wants to compare.
	 //Get all the data passed in by pItem
	 GetItem(&tvTempItem);

	 //Reset the mask so I can keep track of the matching attributes
	 tvTempItem.mask = 0;

	 if((pItem->mask & TVIF_STATE) && 
			(pItem->state == tvTempItem.state))
			tvTempItem.mask |= TVIF_STATE;

	 if((pItem->mask & TVIF_IMAGE) && 
			(pItem->iImage == tvTempItem.iImage))
			tvTempItem.mask |= TVIF_IMAGE;

	 if((pItem->mask & TVIF_PARAM) && 
			(pItem->lParam == tvTempItem.lParam))
			tvTempItem.mask |= TVIF_PARAM;

	 if((pItem->mask & TVIF_TEXT) &&
			pItem->pszText && tvTempItem.pszText && //Don't compare if either is NULL
			!_tcscmp(pItem->pszText, tvTempItem.pszText))
			tvTempItem.mask |= TVIF_TEXT;

	 if((pItem->mask & TVIF_CHILDREN) && 
			(pItem->cChildren == tvTempItem.cChildren))
			tvTempItem.mask |= TVIF_CHILDREN;

	 if((pItem->mask & TVIF_SELECTEDIMAGE) && 
			(pItem->iSelectedImage == tvTempItem.iSelectedImage))
			tvTempItem.mask |= TVIF_SELECTEDIMAGE;
	 
	 //If by this point these two values are the same.
	 //tvTempItem.hItem is the desired item
	 return (pItem->mask == tvTempItem.mask);
}

int CGroupTreeCtrl::GetSelectedItemList(CArray<HTREEITEM, HTREEITEM> &aItem)
{
	aItem.RemoveAll();
	HTREEITEM hItem = GetFirstSelectedItem();
	while (hItem != NULL)
	{
		aItem.Add(hItem);
		hItem = GetNextSelectedItem(hItem);
	}
	return aItem.GetSize();
}


////////////////////////////////////////////////////////////////////////////
// CTreeGroupDlg
#define CFormView CFormViewInBar
IMPLEMENT_DYNCREATE(CTreeGroupDlg, CFormView)

CTreeGroupDlg::CTreeGroupDlg()
	: CFormView(CTreeGroupDlg::IDD)
{
	m_bInitialized = FALSE;
	m_hEdit = NULL;
	//{{AFX_DATA_INIT(CTreeGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTreeGroupDlg::~CTreeGroupDlg()
{
}

void CTreeGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreeGroupDlg)
	DDX_Control(pDX, IDC_TM_GROUP_ASNPLUS, m_btnAsnPlus);
	DDX_Control(pDX, IDC_TM_GROUP_INACTIVE, m_btnInactive);
	DDX_Control(pDX, IDC_TM_GROUP_ACTIVE, m_btnActive);
	DDX_Control(pDX, IDC_TM_GROUP_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_TM_GROUP_UNSELECT, m_btnUnselect);
	DDX_Control(pDX, IDC_TM_GROUP_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_TM_GROUP_ASSIGN, m_btnAssign);
	DDX_Control(pDX, IDC_TM_GROUP_RENAME, m_btnRename);
	DDX_Control(pDX, IDC_TM_GROUP_NEW, m_btnNew);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TM_GROUP_TREE, m_wndTree);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
void CTreeGroupDlg::ResizeTreeCtrl(WINDOWPOS FAR* lpwndpos)
{
	if (m_wndTree.GetSafeHwnd() == 0) return;
	if (lpwndpos->flags & SWP_NOSIZE) return;
	if (lpwndpos->cx == 0 && lpwndpos->cy == 0) return;

	CRect rDlg, rTree, rOld;
	
	rDlg.left = lpwndpos->x;
	rDlg.top = lpwndpos->y;
	rDlg.right = lpwndpos->x + lpwndpos->cx;
	rDlg.bottom = lpwndpos->y + lpwndpos->cy;
	GetParent()->ClientToScreen(rDlg);

	m_wndTree.GetWindowRect(rTree);
	rTree.right = rDlg.right;
	rTree.bottom = rDlg.bottom;
	ScreenToClient(rTree);
	m_wndTree.MoveWindow(rTree);  
}

// 계층 구조 없이 일렬로 만든다.
BOOL CTreeGroupDlg::SetItemTree()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return FALSE;
	}
	// Clear List Box
	m_wndTree.DeleteAllItems();

	// Set item list

	HTREEITEM hParent = TVI_ROOT, hInsertAfter = TVI_LAST;
	HTREEITEM hRoot, hItem;
	CString csRootName = _LS(IDS_TMGD_Global_Model_Group_List);

	// Insert Root
	hRoot = m_wndTree.InsertItem(csRootName, hParent, hInsertAfter);
	if (hRoot == NULL) return FALSE;
	m_wndTree.SetItemData(hRoot, 0); // Item Data is Group Key

	CArray<T_GRUP_K, T_GRUP_K> aKeyList;
	T_GRUP_D data;
	pDoc->m_pAttrCtrl->GetGrupKeyList(aKeyList);
	int nCount = aKeyList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		pDoc->m_pAttrCtrl->GetGrup(aKeyList[i], data);
		hItem = m_wndTree.InsertItem(data.GroupName, hRoot, hInsertAfter);
		if (hItem == NULL) return FALSE;
		m_wndTree.SetItemData(hItem, aKeyList[i]);
	}
	m_wndTree.Expand(hRoot, TVE_EXPAND);

	return TRUE;
}

void CTreeGroupDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		SetItemTree();
		//UpdateBuffer();
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

void CTreeGroupDlg::UpdateBuffer()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc)
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_GRUP_K Key, KeyBak;
	T_GRUP_D Data, DataBak;

	BOOL bMFD = FALSE;
	BOOL bModified = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_GRUP_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetGrup(nKey, Key, Data);
				InsertGroupItem(Key, Data);
				bModified = TRUE;
			}
			break;
		case(UR_GRUP_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetGrup(nKey, Key, Data);
				DeleteGroupItem(Key, Data);
				bModified = TRUE;
			}
			break;
		case(UR_GRUP_MFD):
			{
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetGrup(nKey, KeyBak, DataBak);
				bMFD = TRUE;
			}
			break;
		case(UR_GRUP_MFS):  // Static Load Case의 Name 변경 처리
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			{
				ASSERT(bMFD);
				pViewBuff->GetGrup(nKey, Key, Data);
				// Group ID가 변경되었으면 삭제하고 다시 추가(순서 때문)
				if (Data.nGroupId != DataBak.nGroupId)
				{
					DeleteGroupItem(Key, Data); 
					InsertGroupItem(KeyBak, DataBak);
				}
				// Key나 이름이 변경된 경우에는 키, 이름만 재 설정
				else if (Key != KeyBak || Data.GroupName != DataBak.GroupName)
				{
					HTREEITEM hItem;
					BOOL bFound = FindGroupItem(Key, &hItem);
					ASSERT(bFound);
					m_wndTree.SetItemData(hItem, KeyBak);
					m_wndTree.SetItemText(hItem, DataBak.GroupName);
				}
				// 플래그 수정
				bMFD = FALSE;
				bModified = TRUE;
			}
			break;
		case(UR_GRUP_ITEM_ADD): // nothing to do, 
		case(UR_GRUP_ITEM_DEL): // because I don't save item locally
			break;
		default:
			break;
		}
	} // end of while
	if (bModified)
	{
		HTREEITEM hItem = m_wndTree.GetNextItem(TVI_ROOT, TVGN_CHILD);  // Root Item(Global Model)
		ASSERT(hItem);
		m_wndTree.Expand(hItem, TVE_EXPAND);
		m_wndTree.Invalidate(TRUE);
		m_wndTree.UpdateWindow();
	}
}

void CTreeGroupDlg::InsertGroupItem(T_GRUP_K Key, T_GRUP_D& Data)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc)
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}
	//ViewBuff에 있는 Group ID는 바르지 않으므로 Requery한다.
	pDoc->m_pAttrCtrl->GetGrup(Key, Data);

	T_GRUP_K GrupKey;
	T_GRUP_D GrupData;
	HTREEITEM hRoot, hItem, hInsertAfter = TVI_FIRST;
	hRoot = m_wndTree.GetNextItem(TVI_ROOT, TVGN_CHILD);
	ASSERT(hRoot != NULL);  // Root Item (Global Model)
	hItem = m_wndTree.GetNextItem(hRoot, TVGN_CHILD); // Get First Child
	while (hItem != NULL)
	{
		GrupKey = (T_GRUP_K)m_wndTree.GetItemData(hItem);
		if (GrupKey != 0)
		{
			pDoc->m_pAttrCtrl->GetGrup(GrupKey, GrupData);
			if (GrupData.nGroupId > Data.nGroupId) break;
		}
		hInsertAfter = hItem;
		hItem = m_wndTree.GetNextItem(hItem, TVGN_NEXT);  // next sibling
	}
	if (hItem == NULL)
	{
		hInsertAfter = TVI_LAST;
	}
	hItem = m_wndTree.InsertItem(Data.GroupName, hRoot, hInsertAfter);
	m_wndTree.SetItemData(hItem, Key);
}

void CTreeGroupDlg::DeleteGroupItem(T_GRUP_K Key, T_GRUP_D& Data)
{
	HTREEITEM hItem;
	BOOL bFound = FindGroupItem(Key, &hItem);
	if (!bFound) 
	{
		TRACE(_LS(IDS_TMGD_Warning___Can_t_delete_group__Group__s_is_), Data.GroupName);
		return;
	}
	m_wndTree.DeleteItem(hItem);
}

BOOL CTreeGroupDlg::FindGroupItem(T_GRUP_K Key, HTREEITEM* phItem)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc)
	{
		*phItem = NULL;
		return FALSE;
	}

	T_GRUP_K GrupKey;
	HTREEITEM hItem;
	hItem = m_wndTree.GetNextItem(TVI_ROOT, TVGN_CHILD);  // Root Item (Global Model)
	ASSERT(hItem);
	hItem = m_wndTree.GetNextItem(hItem, TVGN_CHILD); // first child of Global Model
	while (hItem != NULL)
	{
		GrupKey = (T_GRUP_K)m_wndTree.GetItemData(hItem);
		if (GrupKey == Key)  
		{
			*phItem = hItem;
			return TRUE;
		}
		hItem = m_wndTree.GetNextItem(hItem, TVGN_NEXT);  // next sibling
	}
	*phItem = NULL;
	return FALSE;
}

int CTreeGroupDlg::GetSelectedGroupKeyList(CArray<T_GRUP_K, T_GRUP_K> &aKey)
{
	aKey.RemoveAll();

	CArray<HTREEITEM, HTREEITEM> aItem;
	m_wndTree.GetSelectedItemList(aItem);
	int nSize = aItem.GetSize();
	T_GRUP_K Key;
	for (int i = 0; i < nSize; i++)
	{
		Key = (T_GRUP_K)m_wndTree.GetItemData(aItem[i]);
		if (Key > 0) aKey.Add(Key);
	}
	return aKey.GetSize();
}

void CTreeGroupDlg::GetSelAndUnselGroupKeyList(CArray<T_GRUP_K, T_GRUP_K> &aSelKey,
																							 CArray<T_GRUP_K, T_GRUP_K> &aUnselKey)
{
	aSelKey.RemoveAll();
	aUnselKey.RemoveAll();

	HTREEITEM hRoot = m_wndTree.GetNextItem(TVI_ROOT, TVGN_CHILD);
	ASSERT(hRoot);
	HTREEITEM hItem = m_wndTree.GetNextItem(hRoot, TVGN_CHILD);
	while (hItem)
	{
		if (m_wndTree.GetItemState(hItem, TVIS_SELECTED) == TVIS_SELECTED)
			aSelKey.Add(m_wndTree.GetItemData(hItem));
		else aUnselKey.Add(m_wndTree.GetItemData(hItem));
		hItem = m_wndTree.GetNextItem(hItem, TVGN_NEXT);
	}
}

void CTreeGroupDlg::SelectItemListByKey(CArray<T_GRUP_K, T_GRUP_K> &aSelKey)
{
	if (aSelKey.GetSize() == 0) return;

	// 모든 리스트를 순차적으로 방문하면서 Select, Unselect를 한다.
	// Search를 빨리하기 위해 Sorting을 한다.(bsearch 이용)
	int nSize = aSelKey.GetSize();
	qsort((void*)aSelKey.GetData(), nSize, sizeof(UINT), CCompFunc::UINTAsc);
	
	HTREEITEM hLast = NULL;
	T_GRUP_K Key;
	HTREEITEM hRoot = m_wndTree.GetNextItem(TVI_ROOT, TVGN_CHILD);
	ASSERT(hRoot);
	m_wndTree.SetItemState(hRoot, 0, TVIS_SELECTED);

	HTREEITEM hItem = m_wndTree.GetNextItem(hRoot, TVGN_CHILD);
	while (hItem)
	{
		Key = m_wndTree.GetItemData(hItem);
		if (bsearch((const void*)&Key, (const void*)aSelKey.GetData(), nSize, sizeof(UINT), CCompFunc::UINTAsc))
		{
			// Found then Select
			m_wndTree.SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			hLast = hItem;
		}
		else
		{
			// Not Found then Unselect
			m_wndTree.SetItemState(hItem, 0, TVIS_SELECTED);
		}
		hItem = m_wndTree.GetNextItem(hItem, TVGN_NEXT);
	}
	if (hLast) m_wndTree.Select(hLast, TVGN_CARET);
	m_wndTree.Invalidate();
}


BEGIN_MESSAGE_MAP(CTreeGroupDlg, CFormView)
	//{{AFX_MSG_MAP(CTreeGroupDlg)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TM_GROUP_TREE, OnEndlabeleditTmGroupTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TM_GROUP_TREE, OnBeginlabeleditTmGroupTree)
	ON_BN_CLICKED(IDC_TM_GROUP_RENAME, OnTmGroupRename)
	ON_BN_CLICKED(IDC_TM_GROUP_NEW, OnTmGroupNew)
	ON_BN_CLICKED(IDC_TM_GROUP_ASSIGN, OnTmGroupAssign)
	ON_BN_CLICKED(IDC_TM_GROUP_DELETE, OnTmGroupDelete)
	ON_BN_CLICKED(IDC_TM_GROUP_SELECT, OnTmGroupSelect)
	ON_NOTIFY(NM_DBLCLK, IDC_TM_GROUP_TREE, OnDblclkTmGroupTree)
	ON_BN_CLICKED(IDC_TM_GROUP_UNSELECT, OnTmGroupUnselect)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TM_GROUP_TREE, OnBegindragTmGroupTree)
	ON_BN_CLICKED(IDC_TM_GROUP_ACTIVE, OnTmGroupActive)
	ON_BN_CLICKED(IDC_TM_GROUP_INACTIVE, OnTmGroupInactive)
	ON_BN_CLICKED(IDC_TM_GROUP_ASNPLUS, OnTmGroupAsnplus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeGroupDlg diagnostics

#ifdef _DEBUG
void CTreeGroupDlg::AssertValid() const
{
	CFormView::AssertValid();
}

void CTreeGroupDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTreeGroupDlg message handlers

void CTreeGroupDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	ResizeTreeCtrl(lpwndpos);
}

void CTreeGroupDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	ResizeTreeCtrl(lpwndpos);
	ShowScrollBar(SB_BOTH, FALSE);
}

void CTreeGroupDlg::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	if (m_bInitialized) return;
	m_bInitialized = TRUE;

	CDC* pDC = GetDC();
	int numColors = pDC->GetDeviceCaps(NUMCOLORS);
	ReleaseDC(pDC);
	COLORREF clrBack = (numColors==-1) ? RGB(192,220,192):RGB(255,255,255);
	m_wndTree.SetBkColor(clrBack);      // change the background color
	m_wndTree.SetTextColor(RGB(0,0,0));

	m_btnNew.LoadBitmaps(IDB_TM_GROUP_NEW,IDB_TM_GROUP_NEW,IDB_TM_GROUP_NEW);
	m_btnNew.SetTextAlignment(CTrackLookButton::AlignBelow);
	m_btnRename.LoadBitmaps(IDB_TM_GROUP_RENAME,IDB_TM_GROUP_RENAME,IDB_TM_GROUP_RENAME);
	m_btnRename.SetTextAlignment(CTrackLookButton::AlignBelow);
	m_btnAssign.LoadBitmaps(IDB_TM_GROUP_ASSIGN,IDB_TM_GROUP_ASSIGN,IDB_TM_GROUP_ASSIGN);
	m_btnAssign.SetTextAlignment(CTrackLookButton::AlignBelow);
	m_btnAsnPlus.LoadBitmaps(IDB_TM_GROUP_ASNPLUS,IDB_TM_GROUP_ASNPLUS,IDB_TM_GROUP_ASNPLUS);
	m_btnAsnPlus.SetTextAlignment(CTrackLookButton::AlignBelow);
	m_btnSelect.LoadBitmaps(IDB_TM_GROUP_SELECT,IDB_TM_GROUP_SELECT,IDB_TM_GROUP_SELECT);
	m_btnSelect.SetTextAlignment(CTrackLookButton::AlignBelow);
	m_btnUnselect.LoadBitmaps(IDB_TM_GROUP_UNSELECT,IDB_TM_GROUP_UNSELECT,IDB_TM_GROUP_UNSELECT);
	m_btnUnselect.SetTextAlignment(CTrackLookButton::AlignBelow);
	m_btnActive.LoadBitmaps(IDB_TM_GROUP_ACTIVE,IDB_TM_GROUP_ACTIVE,IDB_TM_GROUP_ACTIVE);
	m_btnActive.SetTextAlignment(CTrackLookButton::AlignBelow);
	m_btnInactive.LoadBitmaps(IDB_TM_GROUP_INACTIVE,IDB_TM_GROUP_INACTIVE,IDB_TM_GROUP_INACTIVE);
	m_btnInactive.SetTextAlignment(CTrackLookButton::AlignBelow);
	m_btnDelete.LoadBitmaps(IDB_TM_GROUP_DELETE,IDB_TM_GROUP_DELETE,IDB_TM_GROUP_DELETE);
	m_btnDelete.SetTextAlignment(CTrackLookButton::AlignBelow);

	CString csToolTip;
	m_ToolTipCtrl.Create(this);
	m_btnNew.GetWindowText(csToolTip);
	m_ToolTipCtrl.AddWindowTool(&m_btnNew, csToolTip);
	m_btnRename.GetWindowText(csToolTip);
	m_ToolTipCtrl.AddWindowTool(&m_btnRename, csToolTip);
	m_btnAssign.GetWindowText(csToolTip);
	m_ToolTipCtrl.AddWindowTool(&m_btnAssign, csToolTip);
	m_btnAsnPlus.GetWindowText(csToolTip);
	m_ToolTipCtrl.AddWindowTool(&m_btnAsnPlus, csToolTip);
	m_btnSelect.GetWindowText(csToolTip);
	m_ToolTipCtrl.AddWindowTool(&m_btnSelect, csToolTip);
	m_btnUnselect.GetWindowText(csToolTip);
	m_ToolTipCtrl.AddWindowTool(&m_btnUnselect, csToolTip);
	m_btnActive.GetWindowText(csToolTip);
	m_ToolTipCtrl.AddWindowTool(&m_btnActive, csToolTip);
	m_btnInactive.GetWindowText(csToolTip);
	m_ToolTipCtrl.AddWindowTool(&m_btnInactive, csToolTip);
	m_btnDelete.GetWindowText(csToolTip);
	m_ToolTipCtrl.AddWindowTool(&m_btnDelete, csToolTip);
	// TODO: Add your specialized code here and/or call the base class
	SetItemTree();

	// Enable Multi Select
	m_wndTree.EnableMultiSelect();

	// OLE Drag and Drop
	m_OleDropTarget.Register(this);
}

void CTreeGroupDlg::OnEndlabeleditTmGroupTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}

	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	BOOL bModify = (m_wndTree.GetItemData(m_hEdit) != 0);
	if (pTVDispInfo->item.pszText == 0 ||
			!m_wndTree.IsLabelChanged(pTVDispInfo->item.pszText))
	{
		//GSaveHistoryFormatNF(_LS(IDS_TMGD_Canceled_));
		// 수정 취소된 경우 Modify 아니면 New Item을 지운다.
		if (!bModify) m_wndTree.DeleteItem(m_hEdit);
		m_hEdit = NULL;
		*pResult = 0;
	}
	else
	{
		//GSaveHistoryFormatNF(_LS(IDS_TMGD_Modified_));
		// 수정된 경우
		T_GRUP_K Key;
		T_GRUP_D Data;

		// Group Name이 공백이면 에러
		/*
		CString csGroupName = pTVDispInfo->item.pszText;
		csGroupName.TrimLeft(); csGroupName.TrimRight();
		if (csGroupName == _T("")) 
		{
			m_wndTree.PostMessage(TVM_EDITLABEL, 0, (LPARAM)m_hEdit);
		  *pResult = 0;
			return;
		}
		*/

		if (bModify) // Modify이면
		{
			Key = m_wndTree.GetItemData(m_hEdit);
			pDoc->m_pAttrCtrl->GetGrup(Key, Data);
			Data.GroupName = pTVDispInfo->item.pszText;
			if (pDoc->m_pDataCtrl->AddGrup(Key, Data, TRUE))
			{
				m_hEdit = NULL;
				HTREEITEM hItem;
				BOOL bFound = FindGroupItem(Key, &hItem);
				if (bFound) m_wndTree.Select(hItem, TVGN_CARET);
				*pResult = 1;
			}
			else
			{
				m_wndTree.PostMessage(TVM_EDITLABEL, 0, (LPARAM)m_hEdit);
				*pResult = 0;
			}
		}
		else  // Add이면
		{
			Data.Initialize();
			Data.GroupName = pTVDispInfo->item.pszText;
			if (pDoc->m_pDataCtrl->AddGrup(Data))
			{
				m_wndTree.DeleteItem(m_hEdit);
				m_hEdit = NULL;
			}
			else
			{
				m_wndTree.PostMessage(TVM_EDITLABEL, 0, (LPARAM)m_hEdit);
			}
		  *pResult = 1;
		}
	}
}

void CTreeGroupDlg::OnBeginlabeleditTmGroupTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	// Root는 편집 안되게 한다.
	HTREEITEM hRoot = m_wndTree.GetNextItem(TVI_ROOT, TVGN_CHILD);
	if (pTVDispInfo->item.hItem == hRoot) 
	{
		*pResult = 1;
		return;
	}

	m_hEdit = pTVDispInfo->item.hItem;
	m_wndTree.SaveEditItem(pTVDispInfo->item.hItem);
	
	/*
	CEdit* pEdit = m_wndTree.GetEditControl();
	if (pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()))
	{
		m_wndLabelEdit.SubclassWindow(pEdit->GetSafeHwnd());
		m_wndLabelEdit.SaveInitialText();
	}
	*/

	*pResult = 0;
}

void CTreeGroupDlg::OnTmGroupNew() 
{
	// TODO: Add your control notification handler code here
	/*
	HTREEITEM hItem = m_wndTree.GetNextItem(TVI_ROOT, TVGN_CHILD);  // Root Item(Global Model)
	ASSERT(hItem);

	HTREEITEM hNew = m_wndTree.InsertItem(_T("New Group"), hItem, TVI_LAST);
	if (hNew == NULL) return;
	m_wndTree.Expand(hItem, TVE_EXPAND);
	m_wndTree.SetItemData(hNew, 0);
	m_wndTree.SetItemState(hNew, TVIS_SELECTED, TVIS_SELECTED);
	m_wndTree.Select(hNew, TVGN_CARET);
	m_wndTree.EditLabel(hNew);
	*/

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) return;
	int nCount = pDoc->m_pAttrCtrl->GetCountGrup();
	T_GRUP_D grup;
	CString csGroupName;
	do
	{
		nCount++;
		csGroupName.Format(_LS(IDS_TMGD_New_Group__d), nCount);
	}
	while (pDoc->m_pAttrCtrl->GetGrup(csGroupName, grup));
	grup.Initialize();
	grup.GroupName = csGroupName;
	if (pDoc->m_pDataCtrl->AddGrup(grup))
	{
	  HTREEITEM hItem = m_wndTree.GetNextItem(TVI_ROOT, TVGN_CHILD);  // Root Item(Global Model)
		ASSERT(hItem);
		m_wndTree.Expand(hItem, TVE_EXPAND);
		
		CArray<T_GRUP_K, T_GRUP_K> aSelKey;
		aSelKey.Add(pDoc->m_pAttrCtrl->GetGrupKey(csGroupName));
		SelectItemListByKey(aSelKey);

		HTREEITEM hNew;
		BOOL bFound = FindGroupItem(aSelKey[0], &hNew);
		ASSERT(bFound);
		m_wndTree.EditLabel(hNew);
	}
}

void CTreeGroupDlg::OnTmGroupRename() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hModify = m_wndTree.GetSelectedItem();
	if (hModify == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_rename_));
		return;
	}
	m_wndTree.EditLabel(hModify);
}

void CTreeGroupDlg::OnTmGroupAssign() 
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	CArray<T_GRUP_D, T_GRUP_D&> aData;
	int nSize = GetSelectedGroupKeyList(aKey);
	if (nSize == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_assign_));
		return;
	}

	CArray<T_NODE_K, T_NODE_K> aNodeKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemKey;
	pDoc->m_pViewCtrl->GetAllSelectedNode(aNodeKey);
	pDoc->m_pViewCtrl->GetAllSelectedElem(aElemKey);
	if (aNodeKey.GetSize() == 0 && aElemKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_node_and_element_));
		return;
	}
	T_GRUP_D Data;
	for (int i = 0; i < nSize; i++)
	{
		pDoc->m_pAttrCtrl->GetGrup(aKey[i], Data);
		Data.arKeyNode.Copy(aNodeKey);
		Data.arKeyElem.Copy(aElemKey);
		aData.Add(Data);
	}
	if (pDoc->m_pDataCtrl->ModifyGrup(aKey, aData))
	{
		SelectItemListByKey(aKey);
		pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CTreeGroupDlg::OnTmGroupAsnplus() 
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	CArray<T_GRUP_D, T_GRUP_D&> aData;
	int nSize = GetSelectedGroupKeyList(aKey);
	if (nSize == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_assign_));
		return;
	}

	CArray<T_NODE_K, T_NODE_K> aNodeKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemKey;
	pDoc->m_pViewCtrl->GetAllSelectedNode(aNodeKey);
	pDoc->m_pViewCtrl->GetAllSelectedElem(aElemKey);
	if (aNodeKey.GetSize() == 0 && aElemKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_node_and_element_));
		return;
	}
	T_GRUP_D Data;
	CArray<T_NODE_K, T_NODE_K> aResultNode;
	CArray<T_ELEM_K, T_ELEM_K> aResultElem;
	for (int i = 0; i < nSize; i++)
	{
		pDoc->m_pAttrCtrl->GetGrup(aKey[i], Data);
		aResultNode.RemoveAll(); 
		aResultElem.RemoveAll();
		CQueryCtrl::UnionKey(Data.arKeyNode, aNodeKey, aResultNode);
		CQueryCtrl::UnionKey(Data.arKeyElem, aElemKey, aResultElem);
		Data.arKeyNode.Copy(aResultNode);
		Data.arKeyElem.Copy(aResultElem);
		aData.Add(Data);
	}
	if (pDoc->m_pDataCtrl->ModifyGrup(aKey, aData))
	{
		SelectItemListByKey(aKey);
		pDoc->m_pViewCtrl->UnselectAll(NULL);
	}	
}

void CTreeGroupDlg::OnTmGroupDelete()
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	if (GetSelectedGroupKeyList(aKey) == 0) 
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_delete_)); 
		return;
	}
	
	pDoc->m_pDataCtrl->DelGrup(aKey);
}

void CTreeGroupDlg::OnTmGroupSelect() 
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	if (GetSelectedGroupKeyList(aKey) == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_select_model_));
		return;
	}

	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	pDoc->m_pAttrCtrl->GetKeyListGrupUser(&aKey, &aNodeList, &aElemList);

	pDoc->m_pViewCtrl->Select(NULL, aNodeList, aElemList, FALSE, FALSE);
}

void CTreeGroupDlg::OnDblclkTmGroupTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		*pResult = 0;
		return;
	}

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	if (GetSelectedGroupKeyList(aKey) == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_select_model_));
		HTREEITEM hItem = m_wndTree.GetSelectedItem();
		if (m_wndTree.GetItemData(hItem) == 0) *pResult = 1;
		else *pResult = 0;
		return;
	}

	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	pDoc->m_pAttrCtrl->GetKeyListGrupUser(&aKey, &aNodeList, &aElemList);

	pDoc->m_pViewCtrl->Select(NULL, aNodeList, aElemList, FALSE, TRUE);
	*pResult = 0;
}

void CTreeGroupDlg::OnTmGroupUnselect() 
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	if (GetSelectedGroupKeyList(aKey) == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_select_model_));
		return;
	}

	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	pDoc->m_pAttrCtrl->GetKeyListGrupUser(&aKey, &aNodeList, &aElemList);

	pDoc->m_pViewCtrl->Unselect(NULL, aNodeList, aElemList, FALSE);	
}

void CTreeGroupDlg::OnTmGroupActive() 
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	if (GetSelectedGroupKeyList(aKey) == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_activate_mode));
		return;
	}

	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	pDoc->m_pAttrCtrl->GetKeyListGrupUser(&aKey, &aNodeList, &aElemList);

	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	if (pIGM) pIGM->ActiveThisObjects(aNodeList, aElemList);
}

void CTreeGroupDlg::OnTmGroupInactive() 
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return;
	}

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	if (GetSelectedGroupKeyList(aKey) == 0)
	{
		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_deactivate_mo));
		return;
	}

	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	pDoc->m_pAttrCtrl->GetKeyListGrupUser(&aKey, &aNodeList, &aElemList);

	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	if (pIGM) pIGM->InActiveThisObjects(aNodeList, aElemList);
}


void CTreeGroupDlg::OnBegindragTmGroupTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_hItemDragged = pNMTreeView->itemNew.hItem;
	HTREEITEM hRoot = m_wndTree.GetNextItem(TVI_ROOT, TVGN_CHILD);  // Root Item(Global Model)
	if (hRoot == m_hItemDragged) return;  // Root Item은 Drag 대상에서 제외

	m_wndTree.StartAutoScroll();

	CString strDbObjPtr;
	strDbObjPtr=_LS(IDS_TMGD_Group_Drag_And_Drop_);
	COleDataSource OleDataSource;

	//Copy above CSting in global allocated shareable memory
	HGLOBAL hGlobal    = GlobalAlloc(GMEM_SHARE|GMEM_FIXED, strDbObjPtr.GetLength()+1);
	LPTSTR lpszDbObjPtr = (LPTSTR)GlobalLock(hGlobal);
	_tcscpy(lpszDbObjPtr, (LPCTSTR)strDbObjPtr);

	//Put reference to global memory into clipboard
	OleDataSource.CacheGlobalData(CF_TEXT, hGlobal);
	COleDropSource OleDropSource;
	OleDataSource.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE,
	                         NULL, 
	                         &OleDropSource);

	m_wndTree.StopAutoScroll();
	*pResult = 0;
}

DROPEFFECT CTreeGroupDlg::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	return OnDragOver(pDataObject, dwKeyState, point);
}

DROPEFFECT CTreeGroupDlg::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	DROPEFFECT de=DROPEFFECT_NONE;

	if( !pDataObject->IsDataAvailable(CF_TEXT))
	{
		return de;
	}

	/*  무조건 MOVE로 처리
	if( dwKeyState & MK_CONTROL )
		de=DROPEFFECT_COPY;
	else
		de=DROPEFFECT_MOVE;
	*/
	de = DROPEFFECT_MOVE;

	//hit-test stuff
	HTREEITEM hti;
	UINT      htflags;

	ClientToScreen(&point);
	CPoint ptX=point;

	// MFC control
	m_wndTree.ScreenToClient(&point);
	hti = m_wndTree.HitTest(point, &htflags);
	if (hti && TVHT_ONITEM & htflags) m_wndTree.SelectDropTarget(hti);	
	else { de = DROPEFFECT_NONE; m_wndTree.SelectDropTarget(NULL); }
	
	return de;
}

void CTreeGroupDlg::OnDragLeave() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndTree.SelectDropTarget(NULL);		
	CFormView::OnDragLeave();
}

BOOL CTreeGroupDlg::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	m_wndTree.StopAutoScroll();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMGD_Error___Document_is_not_exist_));
		return FALSE;
	}

	HTREEITEM hSrc, hTarget;
	hSrc = m_hItemDragged;	
	hTarget = m_wndTree.GetDropHilightItem();
	m_wndTree.SelectDropTarget(NULL);	

	BOOL bResult = CFormView::OnDrop(pDataObject, dropEffect, point);

	if (hTarget)
	{
		T_GRUP_K TgtKey;

		// Select 된 것과 Unselect된 것을 가려낸다.
		CArray<T_GRUP_K, T_GRUP_K> aSelKey, aUnselKey;
		GetSelAndUnselGroupKeyList(aSelKey, aUnselKey);
		int nSize = aSelKey.GetSize();
		if (nSize == 0)
		{
			AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_move_));
			return bResult;
		}
		// Target이 Move List에 포함되어 있으면 에러
		TgtKey = m_wndTree.GetItemData(hTarget);
		int i = 0;
		for (i = 0; i < nSize; i++)
			if (TgtKey == aSelKey[i]) break;
		if (i != nSize)
		{
			AfxMessageBox(_LS(IDS_TMGD_Error___Select_target_item_which_is_not_se));
			return bResult;
		}

		// Make Result Data List
		CArray<T_GRUP_K, T_GRUP_K> aResultKey;
		CArray<T_GRUP_D, T_GRUP_D&> aResultData;

		int nGrupCount = pDoc->m_pAttrCtrl->GetCountGrup();
		aResultKey.SetSize(nGrupCount);
		aResultData.SetSize(nGrupCount);

		T_GRUP_K tKey;
		T_GRUP_D tData;
		int nUnselIx = 0;
		int nResultIx = 0;
		// Unselect 된 것 중에서 Select 된 것의 앞에 있는 부분
		if (TgtKey != 0)
		{
			do
			{
				tKey = aUnselKey[nUnselIx++];
				aResultKey[nResultIx] = tKey;
				pDoc->m_pAttrCtrl->GetGrup(tKey, tData);
				tData.nGroupId = nResultIx+1;
				aResultData[nResultIx++] = tData;
			}
			while(tKey != TgtKey);
		}
		// Select 된 부분
		nSize = aSelKey.GetSize();
		for (int j = 0; j < nSize; j++)
		{
			tKey = aSelKey[j];
			aResultKey[nResultIx] = tKey;
			pDoc->m_pAttrCtrl->GetGrup(tKey, tData);
			tData.nGroupId = nResultIx+1;
			aResultData[nResultIx++] = tData;
		}
		// Unselect 된 것 중에서 Select 된 것의 뒤에 있는 부분
		while (nUnselIx < nGrupCount-nSize)
		{
			tKey = aUnselKey[nUnselIx++];
			aResultKey[nResultIx] = tKey;
			pDoc->m_pAttrCtrl->GetGrup(tKey, tData);
			tData.nGroupId = nResultIx+1;
			aResultData[nResultIx++] = tData;
		}
		if (pDoc->m_pDataCtrl->ModifyGrup(aResultKey, aResultData))
		{
			SelectItemListByKey(aSelKey);
		}
	}

	return bResult;
}

/*




////////////////////////// REGISTER ID & STRING //////////////////////////
REG_STR_RES(IDS_TMGD_Error___Document_is_not_exist_      , _LS(IDS_WG_TREEMENU_Error___Document_is_not_exist_));
REG_STR_RES(IDS_TMGD_Global_Model_Group_List      , _LS(IDS_WG_TREEMENU_Global_Model_Group_List));
REG_STR_RES(IDS_TMGD_Warning___Can_t_delete_group__Group__s_is_      , _LS(IDS_WG_TREEMENU_Warning___Can_t_delete_group__Group));
REG_STR_RES(IDS_TMGD_Canceled_      , _LS(IDS_WG_TREEMENU_Canceled_));
REG_STR_RES(IDS_TMGD_Modified_      , _LS(IDS_WG_TREEMENU_Modified_));
REG_STR_RES(IDS_TMGD_New_Group__d      , _LS(IDS_WG_TREEMENU_New_Group__d));
REG_STR_RES(IDS_TMGD_Error___No_selected_group_to_rename_      , _LS(IDS_WG_TREEMENU_Error___No_selected_group_to_rename));
REG_STR_RES(IDS_TMGD_Error___No_selected_group_to_assign_      , _LS(IDS_WG_TREEMENU_Error___No_selected_group_to_assign));
REG_STR_RES(IDS_TMGD_Error___No_selected_node_and_element_      , _LS(IDS_WG_TREEMENU_Error___No_selected_node_and_elemen));
REG_STR_RES(IDS_TMGD_Error___No_selected_group_to_delete_      , _LS(IDS_WG_TREEMENU_Error___No_selected_group_to_delete));
REG_STR_RES(IDS_TMGD_Error___No_selected_group_to_select_model_      , _LS(IDS_WG_TREEMENU_Error___No_selected_group_to_select));
REG_STR_RES(IDS_TMGD_Error___No_selected_group_to_activate_mode      , _LS(IDS_WG_TREEMENU_Error___No_selected_group_to_activa));
REG_STR_RES(IDS_TMGD_Error___No_selected_group_to_deactivate_mo      , _LS(IDS_WG_TREEMENU_Error___No_selected_group_to_deacti));
REG_STR_RES(IDS_TMGD_Group_Drag_And_Drop_      , _LS(IDS_WG_TREEMENU_Group_Drag_And_Drop_));
REG_STR_RES(IDS_TMGD_Error___No_selected_group_to_move_      , _LS(IDS_WG_TREEMENU_Error___No_selected_group_to_move_));
REG_STR_RES(IDS_TMGD_Error___Select_target_item_which_is_not_se      , _LS(IDS_WG_TREEMENU_Error___Select_target_item_which_is));




////////////////////////// ENUM ID REGISTERING  //////////////////////////
IDS_TMGD_Error___Document_is_not_exist_,
IDS_TMGD_Global_Model_Group_List,
IDS_TMGD_Warning___Can_t_delete_group__Group__s_is_,
IDS_TMGD_Canceled_,
IDS_TMGD_Modified_,
IDS_TMGD_New_Group__d,
IDS_TMGD_Error___No_selected_group_to_rename_,
IDS_TMGD_Error___No_selected_group_to_assign_,
IDS_TMGD_Error___No_selected_node_and_element_,
IDS_TMGD_Error___No_selected_group_to_delete_,
IDS_TMGD_Error___No_selected_group_to_select_model_,
IDS_TMGD_Error___No_selected_group_to_activate_mode,
IDS_TMGD_Error___No_selected_group_to_deactivate_mo,
IDS_TMGD_Group_Drag_And_Drop_,
IDS_TMGD_Error___No_selected_group_to_move_,
IDS_TMGD_Error___Select_target_item_which_is_not_se,
*/
