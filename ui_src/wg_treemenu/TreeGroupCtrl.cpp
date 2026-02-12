// TreeGroupCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "TreeGroupCtrl.h"

#include "TreeWorkCtrl.h"
#include "TreeMenuCtrl.h"
#include "BarTreeMenu.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_ViewBase.h"
#include "..\wg_base\XMultiLang.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\DesignResult.h"
#include "..\wg_cmd\wg_cmd_Dialog.h"
#include "..\wg_gr\wg_grDisplaySetting.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#include "SelectBndrTypeDlg.h"
#include "SelectLoadTypeDlg.h"

#include "..\wg_db\DispOptMgr.h"
#include "..\wg_db\DispOptColorObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_GRUPTREE_ASSIGN_REPLACE   0
#define D_GRUPTREE_ASSIGN_PLUS      1
#define D_GRUPTREE_ASSIGN_MINUS     2

using namespace mit::frx;

IMPLEMENT_DYNAMIC(CTreeGroupCtrl, MBaseTreeCtrl);
/////////////////////////////////////////////////////////////////////////////
// CTreeGroupCtrl

CTreeGroupCtrl::CTreeGroupCtrl()
{
	m_mFld2hItem.clear();

	m_mhItem2Key[STRT_GROUP_FLD] = std::map<HTREEITEM, T_KEY>();
	m_mhItem2Key[BNDR_GROUP_FLD] = std::map<HTREEITEM, T_KEY>();
	m_mhItem2Key[LOAD_GROUP_FLD] = std::map<HTREEITEM, T_KEY>();
	m_mhItem2Key[TEND_GROUP_FLD] = std::map<HTREEITEM, T_KEY>();
	m_mhItem2Key[LDAR_GROUP_FLD] = std::map<HTREEITEM, T_KEY>();

	m_mKey2hItem[STRT_GROUP_FLD] = std::map<T_KEY, HTREEITEM>();
	m_mKey2hItem[BNDR_GROUP_FLD] = std::map<T_KEY, HTREEITEM>();
	m_mKey2hItem[LOAD_GROUP_FLD] = std::map<T_KEY, HTREEITEM>();
	m_mKey2hItem[TEND_GROUP_FLD] = std::map<T_KEY, HTREEITEM>();
	m_mKey2hItem[LDAR_GROUP_FLD] = std::map<T_KEY, HTREEITEM>();

	m_hRootTi = NULL;
	m_hEditItem = NULL;

	m_BkColor = RGB(192, 220, 192);
	m_strBkColorName.Empty();
	m_strBkTitleColorName.Empty();

	m_pDoc = CDBDoc::GetDocPoint();
}

CTreeGroupCtrl::~CTreeGroupCtrl()
{
	m_strBkColorName.Empty();
	m_strBkTitleColorName.Empty();
}

#define CTreeCtrl MBaseTreeCtrl
BEGIN_MESSAGE_MAP(CTreeGroupCtrl, CTreeCtrl)
#undef CTreeCtrl
	//{{AFX_MSG_MAP(CTreeGroupCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_NCHITTEST()
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)


	ON_COMMAND(ID_STRT_GROUP_NEW, OnStrtGroupNew)
	ON_COMMAND(ID_BNDR_GROUP_NEW, OnBndrGroupNew)
	ON_COMMAND(ID_LOAD_GROUP_NEW, OnLoadGroupNew)
	ON_COMMAND(ID_TEND_GROUP_NEW, OnTendGroupNew) //tnsel
	ON_COMMAND(ID_LOAD_GROUP_RENAME, OnLoadGroupRename)
	ON_COMMAND(ID_STRT_GROUP_RENAME, OnStrtGroupRename)
	ON_COMMAND(ID_BNDR_GROUP_RENAME, OnBndrGroupRename)
	ON_COMMAND(ID_TEND_GROUP_RENAME, OnTendGroupRename) //tnsel
	ON_COMMAND(ID_BNDR_GROUP_NEW_MULTI, OnBndrGroupNewMulti)
	ON_COMMAND(ID_LOAD_GROUP_NEW_MULTI, OnLoadGroupNewMulti)
	ON_COMMAND(ID_TEND_GROUP_NEW_MULTI, OnTendGroupNewMulti)//tnsel

	ON_COMMAND(ID_STRT_GROUP_ASSIGN, OnStrtGroupAssign)
	ON_COMMAND(ID_STRT_GROUP_ASSIGN_PLUS, OnStrtGroupAssignPlus)
	ON_COMMAND(ID_STRT_GROUP_ASSIGN_MINUS, OnStrtGroupAssignMinus)
	ON_COMMAND(ID_STRT_GROUP_ACTIVE, OnStrtGroupActive)
	ON_COMMAND(ID_STRT_GROUP_DELETE, OnStrtGroupDelete)
	ON_COMMAND(ID_STRT_GROUP_INACTIVE, OnStrtGroupInactive)
	ON_COMMAND(ID_STRT_GROUP_SELECT, OnStrtGroupSelect)
	ON_COMMAND(ID_STRT_GROUP_UNSELECT, OnStrtGroupUnselect)
	ON_COMMAND(ID_STRT_GROUP_NEW_MULTI, OnStrtGroupNewMulti)

	ON_COMMAND(ID_BNDR_GROUP_ASSIGN, OnStrtGroupAssign)
	ON_COMMAND(ID_BNDR_GROUP_ASSIGN_PLUS, OnStrtGroupAssignPlus)
	ON_COMMAND(ID_BNDR_GROUP_ASSIGN_MINUS, OnStrtGroupAssignMinus)
	ON_COMMAND(ID_BNDR_GROUP_ACTIVE, OnStrtGroupActive)
	ON_COMMAND(ID_BNDR_GROUP_DELETE, OnStrtGroupDelete)
	ON_COMMAND(ID_BNDR_GROUP_INACTIVE, OnStrtGroupInactive)
	ON_COMMAND(ID_BNDR_GROUP_SELECT, OnStrtGroupSelect)
	ON_COMMAND(ID_BNDR_GROUP_UNSELECT, OnStrtGroupUnselect)
	ON_COMMAND(ID_BNDR_GROUP_NEW_MULTI, OnStrtGroupNewMulti)

	ON_COMMAND(ID_LOAD_GROUP_ASSIGN, OnStrtGroupAssign)
	ON_COMMAND(ID_LOAD_GROUP_ASSIGN_PLUS, OnStrtGroupAssignPlus)
	ON_COMMAND(ID_LOAD_GROUP_ASSIGN_MINUS, OnStrtGroupAssignMinus)
	ON_COMMAND(ID_LOAD_GROUP_ACTIVE, OnStrtGroupActive)
	ON_COMMAND(ID_LOAD_GROUP_DELETE, OnStrtGroupDelete)
	ON_COMMAND(ID_LOAD_GROUP_INACTIVE, OnStrtGroupInactive)
	ON_COMMAND(ID_LOAD_GROUP_SELECT, OnStrtGroupSelect)
	ON_COMMAND(ID_LOAD_GROUP_UNSELECT, OnStrtGroupUnselect)
	ON_COMMAND(ID_LOAD_GROUP_NEW_MULTI, OnStrtGroupNewMulti)

	//  ON_COMMAND(ID_TEND_GROUP_ASSIGN, OnStrtGroupAssign) //tnsel
	//	ON_COMMAND(ID_TEND_GROUP_ASSIGN_PLUS, OnStrtGroupAssignPlus)
	ON_COMMAND(ID_TEND_GROUP_ACTIVE, OnTendGroupActive) //show
	ON_COMMAND(ID_TEND_GROUP_DELETE, OnTendGroupDelete)
	ON_COMMAND(ID_TEND_GROUP_INACTIVE, OnTendGroupInactive) //hide
	//	ON_COMMAND(ID_TEND_GROUP_SELECT, OnStrtGroupSelect)
	//	ON_COMMAND(ID_TEND_GROUP_UNSELECT, OnStrtGroupUnselect)
	ON_COMMAND(ID_TEND_GROUP_NEW_MULTI, OnStrtGroupNewMulti)

	//Loading Area Group
	ON_COMMAND(ID_LDAR_GROUP_NEW, OnLdarGroupNew) //tnsel
	ON_COMMAND(ID_LDAR_GROUP_NEW_MULTI, OnLdarGroupNewMulti)
	ON_COMMAND(ID_LDAR_GROUP_SELECT, OnLdarGroupSelect)
	ON_COMMAND(ID_LDAR_GROUP_UNSELECT, OnLdarGroupUnselect)
	ON_COMMAND(ID_LDAR_GROUP_ACTIVE, OnLdarGroupActive)
	ON_COMMAND(ID_LDAR_GROUP_INACTIVE, OnLdarGroupInactive)
	ON_COMMAND(ID_LDAR_GROUP_DELETE, OnStrtGroupDelete)
	ON_COMMAND(ID_LDAR_GROUP_RENAME, OnLdarGroupRename)
	ON_COMMAND(ID_LDAR_GROUP_DISPLAY, OnLdarGroupDisplay)
	ON_COMMAND(ID_LDAR_GROUP_UNDISPLAY, OnLdarGroupUndisplay)

	ON_COMMAND(ID_BNDR_GROUP_DISPLAY, OnStrtGroupDisplay)
	ON_COMMAND(ID_BNDR_GROUP_UNDISPLAY, OnStrtGroupUndisplay)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeGroupCtrl message handlers
int CTreeGroupCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MBaseTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateTree();
	EnableMultiSelect();

	//Since updating images when updating the DB causes performance degradation, the following function registers all images used in the tree in advance.
	SetTreeImage();
	SetItemTree();

	EndInsertTreeItem();

	return 0;
}

void CTreeGroupCtrl::OnDestroy()
{
	MBaseTreeCtrl::OnDestroy();

	// TODO: Add your message handler code here
	//CreateTree();
	DestroyTree();
}

void CTreeGroupCtrl::DestroyTree()
{
// 	for (int i = 0; i < m_ImageList.GetImageCount(); i++)
// 		m_ImageList.Remove(0);
// 	m_ImageList.DeleteImageList();
}

void CTreeGroupCtrl::SaveEditItem(HTREEITEM hItem)
{
	m_hEditItem = hItem;
	/*
	CString StrT;
	StrT = GetItemText(hItem);
	int nFind = StrT.Find(_T("["));
	if(nFind > 0)
		m_csEditLabel = StrT.Left(nFind);
	else
		m_csEditLabel = StrT;
		*/
	m_csEditLabel = GetItemText(hItem);
}

void CTreeGroupCtrl::ToggleStyle(DWORD lStyleMask)
{
	long		lStyleOld;

	lStyleOld = GetStyle();
	BOOL bSetBits = !(lStyleMask&lStyleOld);
	if (bSetBits)
		lStyleOld |= lStyleMask;
	else
		lStyleOld &= ~(lStyleMask);

	SetWindowLongPtr(m_hWnd, GWL_STYLE, lStyleOld);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

	DWORD dwStyle, dwExStyle;
	GetTreeCtrlStyles(dwStyle, dwExStyle);
	if (dwStyle & lStyleMask)
		ModifyTreeCtrlStyles(lStyleMask, 0, 0, 0);
	else
		ModifyTreeCtrlStyles(0, lStyleMask, 0, 0);
}



// Update - Buffer

// Create Tree

void CTreeGroupCtrl::CreateTree()
{
	CDC* pDC = GetDC();
	int numColors = pDC->GetDeviceCaps(NUMCOLORS);
	ReleaseDC(pDC);
	COLORREF clrBack = (numColors == -1) ? m_BkColor : RGB(255, 255, 255);
	// change the background color
	SetBkColor(clrBack);

	// change the normal text color
	SetTextColor(RGB(0, 0, 0));

	// change the selected text color
	SetSelTextColor(RGB(255, 255, 255));
	   
	return;
}

void CTreeGroupCtrl::SetItemTree()
{
	InitializeTree init_tree(this);

	if (m_hRootTi == NULL)
		m_hRootTi = InsertTreeItem(_LS(IDS_WG_TREEMENU_ADD3_Group), _T("ico24_treemenu_group_group"));

	if (!m_pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	//Set item List
	InitGroupItemTree();
	InitBoundaryGroupItemTree();
	InitLoadGroupItemTree();
#ifdef _IGEN
	InitLdarGroupItemTree();
#else
	if (CDBDoc::IsEnableWindPressure())
		InitLdarGroupItemTree();
	InitTendGroupItemTree(); //tnsel
#endif // _IGEN

	CArray<T_GRUP_K, T_GRUP_K> aGrupKey;
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(aGrupKey);
	RenewTreeItem_GRUP(std::set<T_GRUP_K>(aGrupKey.GetData(), aGrupKey.GetData() + aGrupKey.GetSize()));

	CArray<T_BNGR_K, T_BNGR_K> aBngrKey;
	m_pDoc->m_pAttrCtrl->GetBngrKeyList(aBngrKey);
	RenewTreeItem_BNGR(std::set<T_BNGR_K>(aBngrKey.GetData(), aBngrKey.GetData() + aBngrKey.GetSize()));

	CArray<T_LDGR_K, T_LDGR_K> aLdgrKey;
	m_pDoc->m_pAttrCtrl->GetLdgrKeyList(aLdgrKey);
	RenewTreeItem_LDGR(std::set<T_LDGR_K>(aLdgrKey.GetData(), aLdgrKey.GetData() + aLdgrKey.GetSize()));

	CArray<T_TDGR_K, T_TDGR_K> aTdgrKey;
	m_pDoc->m_pAttrCtrl->GetTdgrKeyList(aTdgrKey);
	RenewTreeItem_TDGR(std::set<T_TDGR_K>(aTdgrKey.GetData(), aTdgrKey.GetData() + aTdgrKey.GetSize()));

	CArray<T_LAGR_K, T_LAGR_K> aLagrKey;
	m_pDoc->m_pAttrCtrl2->GetLagrKeyList(aLagrKey);
	RenewTreeItem_LAGR(std::set<T_LAGR_K>(aLagrKey.GetData(), aLagrKey.GetData() + aLagrKey.GetSize()));

	SelectItem(m_hRootTi);
	ExpandCompletely(m_hRootTi, TRUE); // 최초에 보여줄 때는 모두 펼쳐서...
}

HTREEITEM CTreeGroupCtrl::InitSubTreeItem(UINT insItem, int nCount, UINT imageID)
{
	CString   tStr;
	CString   strSVGPath;
	switch (insItem)
	{
	case STRT_GROUP_FLD:
		tStr.Format(_LS(IDS_WG_TREEMENU_ADD3_Structure_Group____d), nCount);
		strSVGPath = _T("ico24_structure_group_structure");
		break;
	case BNDR_GROUP_FLD:
		tStr.Format(_LS(IDS_WG_TREEMENU_ADD3_Boundary_Group____d), nCount);
		strSVGPath = _T("ico24_structure_group_defineboundarygroup");
		break;
	case LOAD_GROUP_FLD:
		tStr.Format(_LS(IDS_WG_TREEMENU_ADD3_Load_Group____d), nCount);
		strSVGPath = _T("ico24_structure_group_defineloadgroup");
		break;
	case TEND_GROUP_FLD:
		tStr.Format(_LS(IDS_WG_TREEMENU_ADD3_Tend_Group____d), nCount); // tnsel  
		strSVGPath = _T("ico24_structure_group_definetendongroup");
		break;
	case LDAR_GROUP_FLD:
		tStr.Format(_LS(IDS_WG_TREEMENU_ADD3_Lagr_Group____d), nCount); // loading area group
		strSVGPath = _T("ico24_structure_group_defineloadingareagroup");
		break;
	default:
		break;
	}

	if (m_mFld2hItem.find(insItem) != m_mFld2hItem.end())
		SetItemText(m_mFld2hItem[insItem], tStr);
	else
		m_mFld2hItem[insItem] = InsertTreeItem(m_hRootTi, insItem, tStr, strSVGPath);

	return m_mFld2hItem[insItem];
}

void CTreeGroupCtrl::InitGroupItemTree()
{
	HTREEITEM hFieldItem = NULL;
	CArray<T_GRUP_K, T_GRUP_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(aKeyList);
	int nCount = aKeyList.GetSize();

	hFieldItem = InitSubTreeItem(STRT_GROUP_FLD, nCount, 2);
	if (hFieldItem == NULL) return;
	ExpandCompletely(hFieldItem, TRUE);
}

void CTreeGroupCtrl::InitBoundaryGroupItemTree()
{
	HTREEITEM hFieldItem = NULL;
	CArray<T_BNGR_K, T_BNGR_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetBngrKeyList(aKeyList);
	int nCount = aKeyList.GetSize();

	hFieldItem = InitSubTreeItem(BNDR_GROUP_FLD, nCount, 7);
	if (hFieldItem == NULL) return;
	ExpandCompletely(hFieldItem, TRUE);
}

void CTreeGroupCtrl::InitLoadGroupItemTree()
{
	HTREEITEM hFieldItem = NULL;
	CArray<T_LDGR_K, T_LDGR_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetLdgrKeyList(aKeyList);
	int nCount = aKeyList.GetSize();

	hFieldItem = InitSubTreeItem(LOAD_GROUP_FLD, nCount, 26);
	if (hFieldItem == NULL) return;
	ExpandCompletely(hFieldItem, TRUE);
}

void CTreeGroupCtrl::InitTendGroupItemTree() //tnsel
{
	HTREEITEM hFieldItem = NULL;
	CArray<T_TDGR_K, T_TDGR_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetTdgrKeyList(aKeyList);
	int nCount = aKeyList.GetSize();

	hFieldItem = InitSubTreeItem(TEND_GROUP_FLD, nCount, 73); //tnsel
	if (hFieldItem == NULL) return;
	ExpandCompletely(hFieldItem, TRUE);
}

void CTreeGroupCtrl::InitLdarGroupItemTree()
{
	//Gen에서만 나타나도록..... by yskim
#ifndef _MGEN
	return;
#endif
	HTREEITEM hFieldItem = NULL;
	CArray<T_LAGR_K, T_LAGR_K> aKeyList;
	m_pDoc->m_pAttrCtrl2->GetLagrKeyList(aKeyList);
	int nCount = aKeyList.GetSize();

	hFieldItem = InitSubTreeItem(LDAR_GROUP_FLD, nCount, 108); // 108 : Image Id... 마지막에 추가한것 by yskim
	if (hFieldItem == NULL) return;
	ExpandCompletely(hFieldItem, TRUE);
}

BOOL CTreeGroupCtrl::SelectEntity(UINT data, BOOL bReplace)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	BOOL bResult = GetSelectedItemKeyList(data, aNodeList, aElemList);


	m_pDoc->m_pViewCtrl->Select(NULL, aNodeList, aElemList, FALSE, bReplace);
	return bResult;
}


BOOL CTreeGroupCtrl::GetSelectedItemKeyList(UINT data,
	CArray<unsigned int, unsigned int> &rNode,
	CArray<unsigned int, unsigned int> &rElem) //tnsel
{
	rNode.RemoveAll();
	rElem.RemoveAll();

	HTREEITEM ht, prvHt;
	int pos = 0;

	ht = GetSelectedItem();
	for (prvHt = ht; prvHt != NULL; ht = prvHt, pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);


	switch (data)
	{
	case STRT_GROUP_LST:
	{
		CArray<T_GRUP_K, T_GRUP_K> aGrupKeyList;
		GetSelectedGrupKeyListWithMultiTreeItemSelect(aGrupKeyList);
		m_pDoc->m_pAttrCtrl->GetKeyListGrupUser(&aGrupKeyList, &rNode, &rElem);
		return TRUE;
	}
	case BNDR_GROUP_LST:
	{
		CArray<T_BNGR_K, T_BNGR_K> aBngrKeyList;
		this->GetSelectedBngrKeyListWithMultiTreeItemSelect(aBngrKeyList);
		m_pDoc->m_pAttrCtrl->GetKeyListBngrUser(&aBngrKeyList, &rNode, &rElem);
		return TRUE;
	}
	case LOAD_GROUP_LST:
	{
		CArray<T_LDGR_K, T_LDGR_K> aLdgrKeyList;
		this->GetSelectedLdgrKeyListWithMultiTreeItemSelect(aLdgrKeyList);
		m_pDoc->m_pAttrCtrl->GetKeyListLdgrUser(&aLdgrKeyList, &rNode, &rElem);
		return TRUE;
	}
	case LDAR_GROUP_LST:
	{
		CArray<T_LAGR_K, T_LAGR_K> aLagrKeyList;
		this->GetSelectedLagrKeyListWithMultiTreeItemSelect(aLagrKeyList);
		m_pDoc->m_pAttrCtrl2->GetKeyListLagrUser(&aLagrKeyList, &rNode, &rElem);
		return TRUE;
	}
	/*  case TEND_GROUP_LST: //tnsel
			{
			 CArray<T_TDGR_K, T_TDGR_K> aTdgrKeyList;
				this->GetSelectedTdgrKeyListWithMultiTreeItemSelect(aTdgrKeyList);
				m_pDoc->m_pAttrCtrl->GetKeyListTdgrUser(&aTdgrKeyList,&rNode, &rElem);

				return TRUE;

			}*/
	default:
		break;
	}
	return FALSE;
}

BOOL CTreeGroupCtrl::GetSelectedItemKeyList(UINT data, CArray<unsigned int, unsigned int> &rTdna) //tnsel
{
	rTdna.RemoveAll();

	HTREEITEM ht, prvHt;
	int pos = 0;

	ht = GetSelectedItem();
	for (prvHt = ht; prvHt != NULL; ht = prvHt, pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);

	if (data == TEND_GROUP_LST) //tnsel
	{
		CArray<T_TDGR_K, T_TDGR_K> aTdgrKeyList;
		this->GetSelectedTdgrKeyListWithMultiTreeItemSelect(aTdgrKeyList);
		m_pDoc->m_pAttrCtrl->GetKeyListTdgrUser(&aTdgrKeyList, &rTdna);
		return TRUE;
	}
	return FALSE;
}


int CTreeGroupCtrl::GetSelectedGrupKeyListWithMultiTreeItemSelect(CArray<T_GRUP_K, T_GRUP_K> &aKey)
{
	aKey.RemoveAll();
	if (ItemHasChildren(m_mFld2hItem[STRT_GROUP_FLD]))
	{
		HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[STRT_GROUP_FLD]);
		while (hChildItem != NULL)
		{
			if (GetItemState(hChildItem, TVIS_SELECTED) == TVIS_SELECTED)
			{
				if (m_mhItem2Key[STRT_GROUP_FLD].find(hChildItem) != m_mhItem2Key[STRT_GROUP_FLD].end())
					aKey.Add(m_mhItem2Key[STRT_GROUP_FLD][hChildItem]);
			}
			hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
		}
	}
	return aKey.GetSize();
}

int CTreeGroupCtrl::GetSelectedBngrKeyListWithMultiTreeItemSelect(CArray<T_BNGR_K, T_BNGR_K> &aKey)
{
	aKey.RemoveAll();
	if (ItemHasChildren(m_mFld2hItem[BNDR_GROUP_FLD]))
	{
		HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[BNDR_GROUP_FLD]);
		while (hChildItem != NULL)
		{
			if (GetItemState(hChildItem, TVIS_SELECTED) == TVIS_SELECTED)
			{
				if (m_mhItem2Key[BNDR_GROUP_FLD].find(hChildItem) != m_mhItem2Key[BNDR_GROUP_FLD].end())
					aKey.Add(m_mhItem2Key[BNDR_GROUP_FLD][hChildItem]);
			}
			hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
		}
	}
	return aKey.GetSize();
}

int CTreeGroupCtrl::GetSelectedLdgrKeyListWithMultiTreeItemSelect(CArray<T_LDGR_K, T_LDGR_K> &aKey)
{
	aKey.RemoveAll();
	if (ItemHasChildren(m_mFld2hItem[LOAD_GROUP_FLD]))
	{
		HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[LOAD_GROUP_FLD]);
		while (hChildItem != NULL)
		{
			if (GetItemState(hChildItem, TVIS_SELECTED) == TVIS_SELECTED)
			{
				if (m_mhItem2Key[LOAD_GROUP_FLD].find(hChildItem) != m_mhItem2Key[LOAD_GROUP_FLD].end())
					aKey.Add(m_mhItem2Key[LOAD_GROUP_FLD][hChildItem]);
			}
			hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
		}
	}
	return aKey.GetSize();
}

int CTreeGroupCtrl::GetSelectedTdgrKeyListWithMultiTreeItemSelect(CArray<T_TDGR_K, T_TDGR_K> &aKey) //tnsel
{
	aKey.RemoveAll();
	if (ItemHasChildren(m_mFld2hItem[TEND_GROUP_FLD]))
	{
		HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[TEND_GROUP_FLD]);
		while (hChildItem != NULL)
		{
			if (GetItemState(hChildItem, TVIS_SELECTED) == TVIS_SELECTED)
			{
				if (m_mhItem2Key[TEND_GROUP_FLD].find(hChildItem) != m_mhItem2Key[TEND_GROUP_FLD].end())
					aKey.Add(m_mhItem2Key[TEND_GROUP_FLD][hChildItem]);
			}
			hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
		}
	}
	return aKey.GetSize();
}

int CTreeGroupCtrl::GetSelectedLagrKeyListWithMultiTreeItemSelect(CArray<T_LAGR_K, T_LAGR_K> &aKey)
{
	aKey.RemoveAll();
	if (ItemHasChildren(m_mFld2hItem[LDAR_GROUP_FLD]))
	{
		HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[LDAR_GROUP_FLD]);
		while (hChildItem != NULL)
		{
			if (GetItemState(hChildItem, TVIS_SELECTED) == TVIS_SELECTED)
			{
				if (m_mhItem2Key[LDAR_GROUP_FLD].find(hChildItem) != m_mhItem2Key[LDAR_GROUP_FLD].end())
					aKey.Add(m_mhItem2Key[LDAR_GROUP_FLD][hChildItem]);
			}
			hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
		}
	}
	return aKey.GetSize();
}

void CTreeGroupCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if (selItemNum == 0) return;

	BOOL  bSelectPlus;
	short nKeyState = GetKeyState(VK_LCONTROL);
	if (nKeyState & 0x8000) bSelectPlus = FALSE;
	else bSelectPlus = TRUE;

	if (FALSE == ItemHasChildren(aItem[0]))
	{
		UINT Data = GetItemData(aItem[0]);

		if (Data == TEND_GROUP_LST)
		{
			SelectEntity(Data, bSelectPlus);
			OnTendGroupActive();
		}
		else
		{
			if (SelectEntity(Data, bSelectPlus) == FALSE) // Select 명령 없이 Property 명령만 있는 경우
			{
				if (aItem.GetSize() > 1)
				{
					AfxMessageBox(_LS(IDS_TMWK__MSG170));
					return;
				}
				else
				{
					//EditProperty(aItem[0]);
				}
			}
		}
	}
	*pResult = 1;
}

void CTreeGroupCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)  //tnsel
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	UINT Data = GetItemData(aItem[0]);
	if (selItemNum > 1) { Data = 0; } // 두 개 이상의 아이템이 선택되어 있으면 Assign 불가!!

	switch (Data)
	{
	case STRT_GROUP_LST:
		// iGen 전용메뉴에서 지정한 Strt Group은 수정 불가능 처리.
		if (IsJUDGroup(Data))
		{
			*pResult = 1;
			return;
		}
	case BNDR_GROUP_LST:
	case LOAD_GROUP_LST:
	case TEND_GROUP_LST:
		// 마우스 커서를 드래깅 모드로
		::SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (LONG_PTR)AfxGetApp()->LoadCursor(IDC_CURSOR_ASSIGN));
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ASSIGN));
		m_bDragPoss = TRUE;
		break;
	default:
		// 마우스 커서를 드래깅 불가능 모드로
		::SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (LONG_PTR)AfxGetApp()->LoadCursor(IDC_CURSOR_NOASSIGN));
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_NOASSIGN));
		m_bDragPoss = FALSE;
		break;
	}
	m_bDrag = TRUE;
	m_hDragItem = GetSelectedItem();

	CWnd* pWnd = GetCapture();
	if (pWnd == NULL)
	{
		BOOL bCheck = ReleaseCapture();
		if (bCheck == FALSE)
		{
			AfxMessageBox(_T("Error"));
		}
		else
		{
			SetCapture();
		}
	}
	else
		SetCapture();

	*pResult = 0;
}

void CTreeGroupCtrl::OnCaptureChanged(CWnd* pWnd)
{
	if (m_bDrag && pWnd == 0)
		::SetCapture(this->GetSafeHwnd());
}

LRESULT CTreeGroupCtrl::OnNcHitTest(CPoint point)
{
	BOOL bRet = MBaseTreeCtrl::OnNcHitTest(point);
	return bRet;
}

void CTreeGroupCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDrag) // 드래깅 모드이면 
	{
		SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_ARROW));
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
		m_bDrag = FALSE;
		ReleaseCapture();
		if (m_bDragPoss == TRUE) EndDrag(point);
		m_bDragPoss = FALSE;
	}

	MBaseTreeCtrl::OnLButtonUp(nFlags, point);
}


void CTreeGroupCtrl::EndDrag(CPoint point)
{
	ClientToScreen(&point);
	CWnd* pWnd = CWnd::WindowFromPoint(point);
	if (pWnd == NULL) return;

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CViewBase))) return;

	/*
	I_GENModelBase * pGENModelbase = ((CViewBase *)pWnd)->GetIGM();
	if(pGENModelbase==NULL) return;

	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	pGENModelbase->GetSelectedNodeKeyList(aNodeList);
	pGENModelbase->GetSelectedElemKeyList(aElemList);
	if(aNodeList.GetSize()+aElemList.GetSize()==0) return;
	AssignAttribute(m_hDragItem,aNodeList,aElemList,FALSE);
	*/
	AssignAttribute((CViewBase*)pWnd, FALSE, TRUE);
}







/*
// pArray == T_GRUP_K Or T_BNGR_K Or T_LDGR_K Array
BOOL CTreeGroupCtrl::GetSelectedKeyList(UINT nWhatKind,void* pArray,CArray<HTREEITEM,HTREEITEM>* pItemArr)
{
	if(!pArray) return FALSE;

	//CArray<HTREEITEM,HTREEITEM> ArrhItem;

	switch(nWhatKind)
	{
	case STRT_GROUP_LIST:
		{
			CArray<T_GRUP_K,T_GRUP_K> * pA = (CArray<T_GRUP_K,T_GRUP_K>*)pArray;

			if(pItemArr)
				pItemArr->Copy(ArrhItem);
		}
		break;
	case BNDR_GROUP_LIST:
		{
			CArray<T_BNGR_K,T_BNGR_K> * pA = (CArray<T_BNGR_K,T_BNGR_K>*)pArray;

			if(pItemArr)
				pItemArr->Copy(ArrhItem);
		}
		break;
	case LOAD_GROUP_LIST:
		{
			CArray<T_LDGR_K,T_LDGR_K> * pA = (CArray<T_LDGR_K,T_LDGR_K>*)pArray;

			if(pItemArr)
				pItemArr->Copy(ArrhItem);
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}
*/

int CTreeGroupCtrl::GetSelectedItems(UINT nWhatKind, CArray<HTREEITEM, HTREEITEM>& arItems) //tnsel
{
	arItems.RemoveAll();

	switch (nWhatKind)
	{
	case STRT_GROUP_LST: // 
		if (ItemHasChildren(m_mFld2hItem[STRT_GROUP_FLD]))
		{
			HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[STRT_GROUP_FLD]);
			while (hChildItem != NULL)
			{
				if (GetItemState(hChildItem, TVIS_SELECTED) == TVIS_SELECTED)
				{
					arItems.Add(hChildItem);
				}
				hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);

			}
		}
		break;
	case BNDR_GROUP_LST: //
		/*
		if(ItemHasChildren(m_mFld2hItem[BNDR_GROUP_FLD]))
		{
			HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[STRT_GROUP_FLD]);
			*/
		if (ItemHasChildren(m_mFld2hItem[BNDR_GROUP_FLD]))
		{
			HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[BNDR_GROUP_FLD]);
			while (hChildItem != NULL)
			{
				if (GetItemState(hChildItem, TVIS_SELECTED) == TVIS_SELECTED)
				{
					arItems.Add(hChildItem);
				}
				hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);

			}
		}
		break;
	case LOAD_GROUP_LST:
		//if(ItemHasChildren(m_mFld2hItem[BNDR_GROUP_FLD]))
		//{
			//HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[LOAD_GROUP_FLD]);
		if (ItemHasChildren(m_mFld2hItem[LOAD_GROUP_FLD]))
		{
			HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[LOAD_GROUP_FLD]);
			while (hChildItem != NULL)
			{
				if (GetItemState(hChildItem, TVIS_SELECTED) == TVIS_SELECTED)
				{
					arItems.Add(hChildItem);
				}
				hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);

			}
		}
		break;
	case TEND_GROUP_LST: //tnsel
		//if(ItemHasChildren(m_mFld2hItem[BNDR_GROUP_FLD]))
		//{
			//HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[LOAD_GROUP_FLD]);
		if (ItemHasChildren(m_mFld2hItem[TEND_GROUP_FLD]))
		{
			HTREEITEM hChildItem = GetChildItem(m_mFld2hItem[TEND_GROUP_FLD]);
			while (hChildItem != NULL)
			{
				if (GetItemState(hChildItem, TVIS_SELECTED) == TVIS_SELECTED)
				{
					arItems.Add(hChildItem);
				}
				hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);

			}
		}
		break;
	default:
		break;
	}



	return arItems.GetSize();

}



// Drag의 대상 View  // Assign Plus // Drag Option
//void CTreeGroupCtrl::AssignAttribute(CViewBase * pView , BOOL bPlus, BOOL bDrag) //tnsel

// nOption : (0) Assign(replace) (1) Assign Plus (2) Assign Minus
void CTreeGroupCtrl::AssignAttribute(CViewBase * pView, int nOption, BOOL bDrag)
{
	HTREEITEM ht;
	CArray<T_NODE_K, T_NODE_K> aNodeKeyList;
	CArray<T_ELEM_K, T_ELEM_K> aElemKeyList;
	I_GENModelBase* pIGM;

	if (bDrag)
		ht = m_hDragItem;
	else
		ht = GetSelectedItem();

	if (!ht) return;


	T_GRUP_K CurGrupKey = NULL;
	T_BNGR_K CurBngrKey = NULL;
	T_LDGR_K CurLdgrKey = NULL;
	T_TDGR_K CurTdgrKey = NULL; //tnsel

	if (pView)
	{
		pIGM = pView->GetIGM();
		if (!pIGM) return;
		pIGM->GetSelectedNodeKeyList(aNodeKeyList);
		pIGM->GetSelectedElemKeyList(aElemKeyList);
	}
	else
	{
		pIGM = I_GENModelBase::GetCurMySelfST();
		if (!pIGM) return;
		pIGM->GetSelectedNodeKeyList(aNodeKeyList);
		pIGM->GetSelectedElemKeyList(aElemKeyList);
	}

	if (!m_pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	UINT data = GetItemData(ht);
	CArray<HTREEITEM, HTREEITEM> arhItems;
	GetSelectedItems(data, arhItems);
	//GetSelectedItemList(arhItems);

	CArray<T_GRUP_K, T_GRUP_K>  aAssGrupKeyList;
	CArray<T_GRUP_D, T_GRUP_D&> aAssGrupDataList;
	CArray<T_BNGR_K, T_BNGR_K>  aAssBngrKeyList;
	CArray<T_BNGR_D, T_BNGR_D&> aAssBngrDataList;
	CArray<T_GRUP_K, T_GRUP_K>  aAssLdgrKeyList;
	CArray<T_GRUP_D, T_GRUP_D&> aAssLdgrDataList;
	CArray<T_TDGR_K, T_TDGR_K>  aAssTdgrKeyList; // tnsel
	CArray<T_TDGR_D, T_TDGR_D&> aAssTdgrDataList; // tnsel

	//T_GRUP_K TempGrupKey;
	//T_BNGR_K TempBngrKey;
	//T_LDGR_K TempLdgrKey;
	T_GRUP_D TempGrupData;
	T_BNGR_D TempBngrData;
	T_LDGR_D TempLdgrData;
	T_TDGR_D TempTdgrData; // tnsel

	switch (data)
	{
	case STRT_GROUP_LST:
	{
		if (m_mhItem2Key[STRT_GROUP_FLD].find(ht) == m_mhItem2Key[STRT_GROUP_FLD].end())
			break;

		CurGrupKey = m_mhItem2Key[STRT_GROUP_FLD][ht];

		int nSize = GetSelectedGrupKeyListWithMultiTreeItemSelect(aAssGrupKeyList);
		if (nSize == 0)
		{
			AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_assign_));
			return;
		}

		if (nOption == D_GRUPTREE_ASSIGN_REPLACE)
		{
			for (int i = 0; i < nSize; i++)
			{
				if (m_pDoc->m_pAttrCtrl->GetGrup(aAssGrupKeyList[i], TempGrupData))
				{
					TempGrupData.arKeyNode.Copy(aNodeKeyList);
					TempGrupData.arKeyElem.Copy(aElemKeyList);
					aAssGrupDataList.Add(TempGrupData);
				}
			}
		}
		else if (nOption == D_GRUPTREE_ASSIGN_PLUS)
		{
			CArray<T_NODE_K, T_NODE_K> aResultNode;
			CArray<T_ELEM_K, T_ELEM_K> aResultElem;
			for (int i = 0; i < nSize; i++)
			{
				m_pDoc->m_pAttrCtrl->GetGrup(aAssGrupKeyList[i], TempGrupData);
				aResultNode.RemoveAll();
				aResultElem.RemoveAll();
				CQueryCtrl::UnionKey(TempGrupData.arKeyNode, aNodeKeyList, aResultNode);
				CQueryCtrl::UnionKey(TempGrupData.arKeyElem, aElemKeyList, aResultElem);
				TempGrupData.arKeyNode.Copy(aResultNode);
				TempGrupData.arKeyElem.Copy(aResultElem);
				aAssGrupDataList.Add(TempGrupData);
			}
		}
		else if (nOption == D_GRUPTREE_ASSIGN_MINUS)
		{
			CArray<T_NODE_K, T_NODE_K> aResultNode;
			CArray<T_ELEM_K, T_ELEM_K> aResultElem;
			for (int i = 0; i < nSize; i++)
			{
				m_pDoc->m_pAttrCtrl->GetGrup(aAssGrupKeyList[i], TempGrupData);
				aResultNode.RemoveAll();
				aResultElem.RemoveAll();
				CQueryCtrl::SubtractKey(TempGrupData.arKeyNode, aNodeKeyList, aResultNode);
				CQueryCtrl::SubtractKey(TempGrupData.arKeyElem, aElemKeyList, aResultElem);
				TempGrupData.arKeyNode.Copy(aResultNode);
				TempGrupData.arKeyElem.Copy(aResultElem);
				aAssGrupDataList.Add(TempGrupData);
			}
		}
	}
	break;
	case BNDR_GROUP_LST:
	{
		GetSelectedBngrKeyListWithMultiTreeItemSelect(aAssBngrKeyList);
	}
	break;
	case LOAD_GROUP_LST:
	{
		GetSelectedLdgrKeyListWithMultiTreeItemSelect(aAssLdgrKeyList);
	}
	break;
	case TEND_GROUP_LST: // tnsel
	{
		GetSelectedTdgrKeyListWithMultiTreeItemSelect(aAssTdgrKeyList);
	}
	break;
	default:
		return;
	}

	BOOL bSuccess = FALSE;

	switch (data)
	{
	case  STRT_GROUP_LST: // Group을 할당할 때 
	{
		bSuccess = m_pDoc->m_pDataCtrl->ModifyGrup(aAssGrupKeyList, aAssGrupDataList);
	}
	break;
	case BNDR_GROUP_LST:
	{
		//Assign 할 Boundary 종류를 얻어낸다. 
		CWnd* pWnd;

		if (pView)
			pWnd = pView;
		else
			pWnd = I_GENModelBase::GetCurMySelfST()->GetOwnerWnd();

		CSelectBndrTypeDlg Dlg(pWnd);
		if (IDOK == Dlg.DoModal())
		{
			CArray<int, int> Types;
			Dlg.GetTypes(Types);

			if (nOption == D_GRUPTREE_ASSIGN_MINUS)
				bSuccess = m_pDoc->m_pDataCtrl->AssignMinusBndrGroup(aAssBngrKeyList, aNodeKeyList, aElemKeyList, Types);
			else
				bSuccess = m_pDoc->m_pDataCtrl->AssignBndrGroup(aAssBngrKeyList, aNodeKeyList, aElemKeyList, Types);
		}
	}
	break;
	case LOAD_GROUP_LST:
	{
		CWnd* pWnd;

		if (pView)
			pWnd = pView;
		else
			pWnd = I_GENModelBase::GetCurMySelfST()->GetOwnerWnd();

		CSelectLoadTypeDlg Dlg(pWnd);
		if (IDOK == Dlg.DoModal())
		{
			CArray<int, int> Types;
			Dlg.GetTypes(Types);

			if (nOption == D_GRUPTREE_ASSIGN_MINUS)
				bSuccess = m_pDoc->m_pDataCtrl->AssignMinusLoadGroup(aAssLdgrKeyList, aNodeKeyList, aElemKeyList, Types);
			else
				bSuccess = m_pDoc->m_pDataCtrl->AssignLoadGroup(aAssLdgrKeyList, aNodeKeyList, aElemKeyList, Types);
		}
	}
	break;
	case  TEND_GROUP_LST: // tnsel ?????
	{

	}
	break;
	default:
		break;
	}

	if (bSuccess)
	{
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		UnselectAllItems();
		HTREEITEM TempHt = NULL;
		if (CurGrupKey)
			TempHt = m_mKey2hItem[STRT_GROUP_FLD][CurGrupKey];
		if (CurBngrKey)
			TempHt = m_mKey2hItem[BNDR_GROUP_FLD][CurBngrKey];
		if (CurLdgrKey)
			TempHt = m_mKey2hItem[LOAD_GROUP_FLD][CurLdgrKey];
		if (CurTdgrKey)
			TempHt = m_mKey2hItem[TEND_GROUP_FLD][CurTdgrKey];

		if (TempHt)
			Select(TempHt, TVGN_CARET);

		// MNET:XXXX-SHJUNG-20100503
		// Sturcture Group 변경시 설계 결과 삭제 : 설계 기준이 KCI-USD07 이거나 EuroCode2:04 인 경우에 적용.
		if (m_pDoc->m_pPostCtrl->IsPostEnable())
		{
			if (aElemKeyList.GetSize())
			{
				T_DCON_D DConD;
				CString strDesignCode = _T("");
				if(!m_pDoc->m_pAttrCtrl->GetDcon(DConD))
				{
					T_PREFERENCE rPref; rPref.Initialize();
					m_pDoc->m_pInitCtrl->GetPreference(rPref);
					strDesignCode = rPref.DgnCode.ConcCode;
				}
				else
				{
					strDesignCode = DConD.DesignCode;
				}

				if(strDesignCode==_T("KCI-USD07") && DConD.nClass==2) return;
				else if(strDesignCode==_T("KCI-USD12") && DConD.nClass==2) return;
				else if(strDesignCode == _T("Eurocode2:04") && !DConD.bSpecialSeismic) return;
				else if(strDesignCode!=_T("KCI-USD07") && strDesignCode!=_T("KCI-USD12") && strDesignCode != _T("Eurocode2:04")) return;

				BOOL bSpecialWall = (strDesignCode==_T("KCI-USD07") || strDesignCode==_T("KCI-USD12")) ? DConD.nSpecialWall : FALSE;
				CDesignResult* pDesignResult=m_pDoc->m_pPostCtrl->GetDesignResult();
				for(int i=0; i<aElemKeyList.GetSize(); i++)
				{
					T_ELEM_K ElemK = aElemKeyList[i];
					T_ELEM_D ElemD; ElemD.Initialize();
					if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); continue; }

					if (IsBeam(ElemK))
					{
						// Beam의 설계결과를 삭제한다.
						pDesignResult->ClearDesignResult(ElemK);
						pDesignResult->ClearDesignForce(ElemK);

						// Beam과 연결된 Column의 설계결과를 삭제한다.
						CArray<T_ELEM_K, T_ELEM_K> aConElemKeyList; aConElemKeyList.RemoveAll();
						m_pDoc->m_pAttrCtrl->GetConnectedElem(ElemD.elnod[0], aConElemKeyList);
						for (int j = 0; j < aConElemKeyList.GetSize(); j++)
						{
							T_ELEM_K ConElemK = aConElemKeyList[j];
							T_ELEM_D ConElemD; ConElemD.Initialize();
							if (!m_pDoc->m_pAttrCtrl->GetElem(ConElemK, ConElemD)) { ASSERT(0); continue; }
							if (IsColumn(ConElemK))
							{
								pDesignResult->ClearDesignResult(ConElemK);
								pDesignResult->ClearDesignForce(ConElemK);
							}
						}
						aConElemKeyList.RemoveAll();
						m_pDoc->m_pAttrCtrl->GetConnectedElem(ElemD.elnod[1], aConElemKeyList);
						for (int j = 0; j < aConElemKeyList.GetSize(); j++)
						{
							T_ELEM_K ConElemK = aConElemKeyList[j];
							T_ELEM_D ConElemD; ConElemD.Initialize();
							if (!m_pDoc->m_pAttrCtrl->GetElem(ConElemK, ConElemD)) { ASSERT(0); continue; }
							if (IsColumn(ConElemK))
							{
								pDesignResult->ClearDesignResult(ConElemK);
								pDesignResult->ClearDesignForce(ConElemK);
							}
						}
					}
					else if (IsColumn(ElemK))
					{
						pDesignResult->ClearDesignResult(ElemK);
						pDesignResult->ClearDesignForce(ElemK);
					}
					else if (bSpecialWall && m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))
					{
						UINT InternalWallID = 0;
						if (!m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, InternalWallID)) { ASSERT(0); continue; }

						T_WALL_K iWallKey;
						if (!m_pDoc->m_pPostCtrl->GetWallKey(InternalWallID, iWallKey.keymap)) { ASSERT(0); continue; }
						UINT nWallId = iWallKey.key.wallid;

						// 이 WallID를 갖는 Wall들의 설계 결과를 삭제한다.
						CArray<UINT, UINT> aInternalWallIdList; aInternalWallIdList.RemoveAll();
						m_pDoc->m_pPostCtrl->GetInternalWallIdList(nWallId, aInternalWallIdList);
						int iNumWall = aInternalWallIdList.GetSize();
						for (int j = 0; j < iNumWall; j++)
						{
							pDesignResult->ClearWallDesignResult(aInternalWallIdList[j]);
							pDesignResult->ClearWallDesignForce(aInternalWallIdList[j]);
						}
					}
				}
			}
		}
	}
}

BOOL CTreeGroupCtrl::IsBeam(int key)
{
	////////////////////////////////////////////////////////////////////////////////////////////
	// member type
	//2=beam(수평-1/100 오차허용)
	////////////////////////////////////////////////////////////////////////////////////////////
	BOOL bBeam = FALSE;
	if (m_pDoc->m_pAttrCtrl->ExistMbtp(key))
	{
		T_MBTP_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetMbtp(key, rData);							// 사용자 정의방식
		if (rData.nMbType == 2) bBeam = TRUE;
	}
	else
	{
		// 일반적인 방식
		if (m_pDoc->m_pAttrCtrl->GetMemberType(key) == 2) bBeam = TRUE;
	}

	return bBeam;
}


BOOL CTreeGroupCtrl::IsColumn(int key)
{
	////////////////////////////////////////////////////////////////////////////////////////////
	// member type
	//1=column(수직-1/100 오차허용)
	////////////////////////////////////////////////////////////////////////////////////////////
	BOOL bColumn = FALSE;
	if (m_pDoc->m_pAttrCtrl->ExistMbtp(key))
	{
		T_MBTP_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetMbtp(key, rData);							// 사용자 정의방식
		if (rData.nMbType == 1) bColumn = TRUE;
	}
	else
	{
		// 일반적인 방식
		if (m_pDoc->m_pAttrCtrl->GetMemberType(key) == 1) bColumn = TRUE;
	}

	return bColumn;
}

void CTreeGroupCtrl::UnselectEntity(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data, aNodeList, aElemList);

	if (!m_pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}
	m_pDoc->m_pViewCtrl->Unselect(NULL, aNodeList, aElemList, FALSE);
}

void CTreeGroupCtrl::ActiveEntity(UINT data) //tnsel11
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data, aNodeList, aElemList);

	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	if (pIGM) pIGM->ActiveThisObjects(aNodeList, aElemList);
}

void CTreeGroupCtrl::InactiveEntity(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data, aNodeList, aElemList);

	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	if (pIGM) pIGM->InActiveThisObjects(aNodeList, aElemList);
}

void CTreeGroupCtrl::DeleteItemEntity(UINT data) // tnsel11
{
	ASSERT(m_pDoc);

	//HTREEITEM parentItem = GetParentItem(aItem[0]);
	//UINT parentData = GetItemData(parentItem);
	//UINT data = GetItemData(aItem[0]);
	//BOOL bSuccess=FALSE;
	switch (data)
	{
	case STRT_GROUP_LST:
	{
		CArray<T_GRUP_K, T_GRUP_K> aSelGrupKey;
		GetSelectedGrupKeyListWithMultiTreeItemSelect(aSelGrupKey);
		if (m_pDoc->m_pDataCtrl->DelGrup(aSelGrupKey))
		{
			UnselectAllItems();
			Select(m_mFld2hItem[STRT_GROUP_FLD], TVGN_CARET);
		}
	}
	break;
	case BNDR_GROUP_LST:
	{
		CArray<T_BNGR_K, T_BNGR_K> aSelGrupKey;
		GetSelectedBngrKeyListWithMultiTreeItemSelect(aSelGrupKey);
		if (m_pDoc->m_pDataCtrl->DelBngr(aSelGrupKey))
		{
			UnselectAllItems();
			Select(m_mFld2hItem[BNDR_GROUP_FLD], TVGN_CARET);
		}
	}
	break;
	case LOAD_GROUP_LST:
	{
		CArray<T_LDGR_K, T_LDGR_K> aSelGrupKey;
		GetSelectedLdgrKeyListWithMultiTreeItemSelect(aSelGrupKey);
		if (m_pDoc->m_pDataCtrl->DelLdgr(aSelGrupKey))
		{
			UnselectAllItems();
			Select(m_mFld2hItem[LOAD_GROUP_FLD], TVGN_CARET);
		}
	}
	break;
	case TEND_GROUP_LST: // tnsel
	{
		CArray<T_TDGR_K, T_TDGR_K> aSelGrupKey;
		GetSelectedTdgrKeyListWithMultiTreeItemSelect(aSelGrupKey); //tnsel11
		if (m_pDoc->m_pDataCtrl->DelTdgr(aSelGrupKey))
		{
			UnselectAllItems();
			Select(m_mFld2hItem[TEND_GROUP_FLD], TVGN_CARET);
		}
	}
	break;
	case LDAR_GROUP_LST:
	{
		CArray<T_LAGR_K, T_LAGR_K> aSelGrupKey;
		GetSelectedLagrKeyListWithMultiTreeItemSelect(aSelGrupKey);
		if (m_pDoc->m_pDataCtrl->DelLagr(aSelGrupKey))
		{
			UnselectAllItems();
			Select(m_mFld2hItem[LDAR_GROUP_FLD], TVGN_CARET);
		}
	}
	break;
	default:
		break;
	}
}


void CTreeGroupCtrl::OnContextMenu(CWnd* pWnd, CPoint point)  // tnsel
{

	CMenu     menu;
	CMenu     *pPopup;
	HMENU			hMenu;
	INIResourceManager::LoadMenuToINITranslate(hMenu,IDR_TREEGROUP_CONTEXT, _T("wg_treemenu"), &menu);
	
	
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if (selItemNum == 0) return;
	UINT Data = GetItemData(aItem[0]);

	if (selItemNum == 1 && aItem[0] == m_hRootTi) return;

	auto TrackPopupMenu = [this, &pPopup, point]()
	{
		if (g_pContextMenuManager != nullptr)
		{
			int nMenuID = g_pContextMenuManager->TrackPopupMenu(pPopup->GetSafeHmenu(), point.x, point.y, this);
			PostMessage(WM_COMMAND, MAKEWPARAM(nMenuID, 0), 0);
		}
		else
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	};

	switch (Data)
	{
	case STRT_GROUP_FLD:
		pPopup = menu.GetSubMenu(0);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case STRT_GROUP_LST:
		pPopup = menu.GetSubMenu(1);

		if (IsJUDGroup(Data))
		{
			pPopup->EnableMenuItem(ID_STRT_GROUP_ASSIGN, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_STRT_GROUP_ASSIGN_PLUS, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_STRT_GROUP_ASSIGN_MINUS, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_STRT_GROUP_DELETE, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
			pPopup->EnableMenuItem(ID_STRT_GROUP_RENAME, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
		}

		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case BNDR_GROUP_FLD:
		pPopup = menu.GetSubMenu(2);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case BNDR_GROUP_LST:
		pPopup = menu.GetSubMenu(3);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case LOAD_GROUP_FLD:
		pPopup = menu.GetSubMenu(4);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case LOAD_GROUP_LST:
		pPopup = menu.GetSubMenu(5);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case TEND_GROUP_FLD: //tnsel
		pPopup = menu.GetSubMenu(6);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case TEND_GROUP_LST: //tnsel
		pPopup = menu.GetSubMenu(7);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case LDAR_GROUP_FLD: // Loading Area Group Field
		pPopup = menu.GetSubMenu(8);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case LDAR_GROUP_LST: // Loading Area Group List
		pPopup = menu.GetSubMenu(9);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	default:
		break;
	}
}

void CTreeGroupCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	BOOL  bMulti = FALSE;

	short nKeyState = GetKeyState(VK_LCONTROL);
	if (nKeyState & 0x8000) bMulti = TRUE;
	if (bMulti == FALSE) { nKeyState = GetKeyState(VK_LSHIFT); if (nKeyState & 0x8000) bMulti = TRUE; }

	if (selItemNum && bMulti)
	{
		HTREEITEM preParItem = GetParentItem(aItem[0]);
		UINT preParData = GetItemData(preParItem);
		HTREEITEM newParItem = GetParentItem(pNMTreeView->itemNew.hItem);
		UINT newParData = GetItemData(newParItem);
		if (preParData != newParData) { *pResult = 1; return; }
	}

	*pResult = 0;
}

void CTreeGroupCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	// Root는 편집 안되게 한다.
	if (pTVDispInfo->item.hItem == this->m_hRootTi ||
		pTVDispInfo->item.hItem == this->m_mFld2hItem[BNDR_GROUP_FLD] ||
		pTVDispInfo->item.hItem == this->m_mFld2hItem[STRT_GROUP_FLD] ||
		pTVDispInfo->item.hItem == this->m_mFld2hItem[LOAD_GROUP_FLD] ||
		pTVDispInfo->item.hItem == this->m_mFld2hItem[TEND_GROUP_FLD])
	{
		*pResult = 1;
		return;
	}

	// iGen 전용메뉴에서 지정한 Strt Group은 수정 불가능 처리.
	if (IsJUDGroup(pTVDispInfo->item.hItem))
	{
		*pResult = 1;
		return;
	}

	SaveEditItem(pTVDispInfo->item.hItem);

	*pResult = 0;
}

void CTreeGroupCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{

/*
	CEdit* pEdit = GetEditControl();

	if (pEdit != NULL)
	{
		 pEdit->SetWindowText(_T("Test"));
	}
*/

	
	UINT ItemData = GetItemData(m_hEditItem);

	switch (ItemData)
	{
	case BNDR_GROUP_LST:
		EndEditLabelBndrGroup(pNMHDR, pResult);
		break;
	case LOAD_GROUP_LST:
		EndEditLabelLoadGroup(pNMHDR, pResult);
		break;
	case STRT_GROUP_LST:
		EndEditLabelStrtGroup(pNMHDR, pResult);
		break;
	case TEND_GROUP_LST:
		EndEditLabelTendGroup(pNMHDR, pResult);
		break;
	case LDAR_GROUP_LST:
		EndEditLabelLdarGroup(pNMHDR, pResult);
		break;
	default:
		break;
	}
}

void CTreeGroupCtrl::EndEditLabelLdarGroup(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	// TODO: Add your control notification handler code here
	BOOL bModify = (GetItemData(m_hEditItem) != 0);
	if (pTVDispInfo->item.pszText == 0 ||
		!IsLabelChanged(pTVDispInfo->item.pszText))
	{
		//GSaveHistoryFormatNF(_LS(IDS_TMGD_Canceled_));
		// 수정 취소된 경우 Modify 아니면 New Item을 지운다.
		if (!bModify) DeleteItem(m_hEditItem);
		m_hEditItem = NULL;
		*pResult = 1;
		return;
	}
	else
	{
		try
		{
			T_LDGR_K Key;
			T_LDGR_D Data;

			// Group Name이 공백이면 에러
			CString csGroupName = pTVDispInfo->item.pszText;
			csGroupName.TrimLeft(); csGroupName.TrimRight();
			if (csGroupName == "")
				throw 0;

			if (bModify) // Modify이면
			{
				CArray<T_LDGR_K, T_LDGR_K> arGroupKey;
				CArray<T_LDGR_D, T_LDGR_D&> arDataNew;

				if (m_mhItem2Key[LOAD_GROUP_FLD].find(m_hEditItem) == m_mhItem2Key[LOAD_GROUP_FLD].end())
					throw 0;

				Key = m_mhItem2Key[LOAD_GROUP_FLD][m_hEditItem];
				if (!m_pDoc->m_pAttrCtrl->GetLdgr(Key, Data))
					throw 0;

				Data.GroupName = pTVDispInfo->item.pszText;
				arGroupKey.Add(Key);
				arDataNew.Add(Data);

				if (!m_pDoc->m_pDataCtrl->ModifyLdgr(arGroupKey, arDataNew))
					throw 0;

				*pResult = 0;
				m_hEditItem = NULL;
			}
			else  // Add이면
			{
				Data.Initialize();
				Data.GroupName = pTVDispInfo->item.pszText;
				if (!m_pDoc->m_pDataCtrl->AddLdgr(Data))
					throw 0;

				m_hEditItem = NULL;
				*pResult = 0;
			}
		}
		catch (...)
		{
			PostMessage(TVM_EDITLABEL, 0, (LPARAM)m_hEditItem);
			*pResult = 1;
			return;
		}
	}
}

void CTreeGroupCtrl::EndEditLabelStrtGroup(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	// TODO: Add your control notification handler code here
	BOOL bModify = (GetItemData(m_hEditItem) != 0);
	if (pTVDispInfo->item.pszText == 0 ||
		!IsLabelChanged(pTVDispInfo->item.pszText))
	{
		//GSaveHistoryFormatNF(_LS(IDS_TMGD_Canceled_));
		// 수정 취소된 경우 Modify 아니면 New Item을 지운다.
		if (!bModify) DeleteItem(m_hEditItem);
		m_hEditItem = NULL;
		*pResult = 1;
		return;
	}
	else
	{
		try
		{
			T_GRUP_K Key;
			T_GRUP_D Data;

			// Group Name이 공백이면 에러
			CString csGroupName = pTVDispInfo->item.pszText;
			csGroupName.TrimLeft(); csGroupName.TrimRight();
			if (csGroupName == "")
				throw 0;

			if (bModify) // Modify이면
			{
				if (m_mhItem2Key[STRT_GROUP_FLD].find(m_hEditItem) == m_mhItem2Key[STRT_GROUP_FLD].end())
					throw 0;

				Key = m_mhItem2Key[STRT_GROUP_FLD][m_hEditItem];
				if (!m_pDoc->m_pAttrCtrl->GetGrup(Key, Data))
					throw 0;

				CString StrT = pTVDispInfo->item.pszText;
				StrT.TrimLeft();
				StrT.TrimRight();

				int nFind = StrT.Find(_T("["));
				if (nFind > 0)
					Data.GroupName = StrT.Left(nFind);
				else if (nFind == 0)
				{
					int nF = m_strOriginalValue.Find(_T("["));
					if (nF > 0)
						Data.GroupName = m_strOriginalValue.Left(nF);
					else
						Data.GroupName = m_strOriginalValue;
				}
				else
				{
					Data.GroupName = StrT;
				}

				Data.GroupName.TrimLeft();
				Data.GroupName.TrimRight();

				if (!m_pDoc->m_pDataCtrl->AddGrup(Key, Data, TRUE))
					throw 0;

				*pResult = 0;
				m_hEditItem = NULL;
			}
			else  // Add이면
			{
				Data.Initialize();
				Data.GroupName = pTVDispInfo->item.pszText;
				if (!m_pDoc->m_pDataCtrl->AddGrup(Data))
					throw 0;

				m_hEditItem = NULL;
				*pResult = 0;
			}
		}
		catch(...)
		{
			PostMessage(TVM_EDITLABEL, 0, (LPARAM)m_hEditItem);
			*pResult = 1;
		}

	}
}

void CTreeGroupCtrl::EndEditLabelBndrGroup(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	// TODO: Add your control notification handler code here
	BOOL bModify = (GetItemData(m_hEditItem) != 0);
	if (pTVDispInfo->item.pszText == 0 ||
		!IsLabelChanged(pTVDispInfo->item.pszText))
	{
		//GSaveHistoryFormatNF(_LS(IDS_TMGD_Canceled_));
		// 수정 취소된 경우 Modify 아니면 New Item을 지운다.
		if (!bModify) DeleteItem(m_hEditItem);
		m_hEditItem = NULL;
		*pResult = 1;
		return;
	}
	else
	{
		try
		{
			T_BNGR_K Key;
			T_BNGR_D Data;

			// Group Name이 공백이면 에러
			CString csGroupName = pTVDispInfo->item.pszText;
			csGroupName.TrimLeft(); csGroupName.TrimRight();
			if (csGroupName == "")
				throw 0;

			if (bModify) // Modify이면
			{
				CArray<T_BNGR_K, T_BNGR_K> arGroupKey;
				CArray<T_BNGR_D, T_BNGR_D&> arDataNew;

				if (m_mhItem2Key[BNDR_GROUP_FLD].find(m_hEditItem) == m_mhItem2Key[BNDR_GROUP_FLD].end())
					throw 0;

				Key = m_mhItem2Key[BNDR_GROUP_FLD][m_hEditItem];
				if (!m_pDoc->m_pAttrCtrl->GetBngr(Key, Data))
					throw 0;

				Data.GroupName = pTVDispInfo->item.pszText;
				arGroupKey.Add(Key);
				arDataNew.Add(Data);

				if (!m_pDoc->m_pDataCtrl->ModifyBngr(arGroupKey, arDataNew))
					throw 0;

				*pResult = 0;
				m_hEditItem = NULL;
			}
			else  // Add이면
			{
				Data.Initialize();
				Data.GroupName = pTVDispInfo->item.pszText;
				if (!m_pDoc->m_pDataCtrl->AddBngr(Data))
					throw 0;

				m_hEditItem = NULL;
				*pResult = 0;
			}
		}
		catch (...)
		{
			PostMessage(TVM_EDITLABEL, 0, (LPARAM)m_hEditItem);
			*pResult = 1;
			return;
		}
	}
}

void CTreeGroupCtrl::EndEditLabelLoadGroup(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	// TODO: Add your control notification handler code here
	BOOL bModify = (GetItemData(m_hEditItem) != 0);
	if (pTVDispInfo->item.pszText == 0 ||
		!IsLabelChanged(pTVDispInfo->item.pszText))
	{
		//GSaveHistoryFormatNF(_LS(IDS_TMGD_Canceled_));
		// 수정 취소된 경우 Modify 아니면 New Item을 지운다.
		if (!bModify) DeleteItem(m_hEditItem);
		m_hEditItem = NULL;
		*pResult = 1;
		return;
	}
	else
	{
		try
		{
			T_LDGR_K Key;
			T_LDGR_D Data;

			// Group Name이 공백이면 에러
			CString csGroupName = pTVDispInfo->item.pszText;
			csGroupName.TrimLeft(); csGroupName.TrimRight();
			if (csGroupName == "")
				throw 0;

			if (bModify) // Modify이면
			{
				CArray<T_LDGR_K, T_LDGR_K> arGroupKey;
				CArray<T_LDGR_D, T_LDGR_D&> arDataNew;

				if (m_mhItem2Key[LOAD_GROUP_FLD].find(m_hEditItem) == m_mhItem2Key[LOAD_GROUP_FLD].end())
					throw 0;

				Key = m_mhItem2Key[LOAD_GROUP_FLD][m_hEditItem];
				if (!m_pDoc->m_pAttrCtrl->GetLdgr(Key, Data))
					throw 0;

				Data.GroupName = pTVDispInfo->item.pszText;
				arGroupKey.Add(Key);
				arDataNew.Add(Data);

				if (!m_pDoc->m_pDataCtrl->ModifyLdgr(arGroupKey, arDataNew))
					throw 0;

				*pResult = 0;
				m_hEditItem = NULL;
			}
			else  // Add이면
			{
				Data.Initialize();
				Data.GroupName = pTVDispInfo->item.pszText;
				if (!m_pDoc->m_pDataCtrl->AddLdgr(Data))
					throw 0;

				m_hEditItem = NULL;
				*pResult = 0;
			}
		}
		catch (...)
		{
			PostMessage(TVM_EDITLABEL, 0, (LPARAM)m_hEditItem);
			*pResult = 1;
			return;
		}
	}
}

void CTreeGroupCtrl::EndEditLabelTendGroup(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	// TODO: Add your control notification handler code here
	BOOL bModify = (GetItemData(m_hEditItem) != 0);
	if (pTVDispInfo->item.pszText == 0 ||
		!IsLabelChanged(pTVDispInfo->item.pszText))
	{
		//GSaveHistoryFormatNF(_LS(IDS_TMGD_Canceled_));
		// 수정 취소된 경우 Modify 아니면 New Item을 지운다.
		if (!bModify) DeleteItem(m_hEditItem);
		m_hEditItem = NULL;
		*pResult = 1;
		return;
	}
	else
	{
		try
		{
			T_TDGR_K Key;
			T_TDGR_D Data;

			// Group Name이 공백이면 에러
			CString csGroupName = pTVDispInfo->item.pszText;
			csGroupName.TrimLeft(); csGroupName.TrimRight();
			if (csGroupName == "")
				throw 0;

			if (bModify) // Modify이면
			{
				CArray<T_TDGR_K, T_TDGR_K> arGroupKey;
				CArray<T_TDGR_D, T_TDGR_D&> arDataNew;

				if (m_mhItem2Key[TEND_GROUP_FLD].find(m_hEditItem) == m_mhItem2Key[TEND_GROUP_FLD].end())
					throw 0;

				Key = m_mhItem2Key[TEND_GROUP_FLD][m_hEditItem];
				if (!m_pDoc->m_pAttrCtrl->GetTdgr(Key, Data))
					throw 0;

				Data.GroupName = pTVDispInfo->item.pszText;
				arGroupKey.Add(Key);
				arDataNew.Add(Data);

				if (!m_pDoc->m_pDataCtrl->ModifyTdgr(arGroupKey, arDataNew))
					throw 0;

				*pResult = 0;
				m_hEditItem = NULL;
			}
			else  // Add이면
			{
				Data.Initialize();
				Data.GroupName = pTVDispInfo->item.pszText;
				if (!m_pDoc->m_pDataCtrl->AddTdgr(Data))
					throw 0;

				m_hEditItem = NULL;
				*pResult = 0;
			}
		}
		catch (...)
		{
			PostMessage(TVM_EDITLABEL, 0, (LPARAM)m_hEditItem);
			*pResult = 1;
			return;
		}
	}
}

void CTreeGroupCtrl::OnStrtGroupNew()
{
	if (!m_pDoc) return;
	int nCount = m_pDoc->m_pAttrCtrl->GetCountGrup();
	T_GRUP_D grup;
	CString csGroupName;
	do
	{
		nCount++;          // New Group %d 
		csGroupName.Format(_LS(IDS_WG_TREEMENU_ADD3_Strt_Group__d), nCount);
	} while (m_pDoc->m_pAttrCtrl->GetGrup(csGroupName, grup));

	grup.Initialize();
	grup.GroupName = csGroupName;

	if (!m_pDoc->m_pDataCtrl->AddGrup(grup))
		return;

	T_GRUP_K TempGrupK = m_pDoc->m_pAttrCtrl->GetGrupKey(csGroupName);
	if (TempGrupK == 0)
		return;

	if (m_mKey2hItem[STRT_GROUP_FLD].find(TempGrupK) != m_mKey2hItem[STRT_GROUP_FLD].end())
	{
		HTREEITEM hCurItem = m_mKey2hItem[STRT_GROUP_FLD][TempGrupK];
		EnsureVisible(hCurItem);
		EditLabel(hCurItem);
		
		SelectEditText();
	}
}

void CTreeGroupCtrl::OnBndrGroupNew()
{
	if (!m_pDoc) return;
	int nCount = m_pDoc->m_pAttrCtrl->GetCountBngr();
	T_BNGR_D bngr;
	CString csGroupName;
	do
	{
		nCount++;          // New Group %d 
		csGroupName.Format(_LS(IDS_WG_TREEMENU_ADD3_Bndr_Group__d), nCount);
	} while (m_pDoc->m_pAttrCtrl->GetBngr(csGroupName, bngr));

	bngr.Initialize();
	bngr.GroupName = csGroupName;

	if (!m_pDoc->m_pDataCtrl->AddBngr(bngr))
		return;

	T_BNGR_K TempBngrK = m_pDoc->m_pAttrCtrl->GetBngrKey(csGroupName);
	if (TempBngrK == 0)
		return;

	if (m_mKey2hItem[BNDR_GROUP_FLD].find(TempBngrK) != m_mKey2hItem[BNDR_GROUP_FLD].end())
	{
		HTREEITEM hCurItem = m_mKey2hItem[BNDR_GROUP_FLD][TempBngrK];
		EnsureVisible(hCurItem);
		EditLabel(hCurItem);

		SelectEditText();
	}
}

void CTreeGroupCtrl::OnLoadGroupNew()
{
	if (!m_pDoc) return;
	int nCount = m_pDoc->m_pAttrCtrl->GetCountLdgr();
	T_LDGR_D ldgr;
	CString csGroupName;
	do
	{
		nCount++;          // New Group %d 
		csGroupName.Format(_LS(IDS_WG_TREEMENU_ADD3_Load_Group__d), nCount);
	} while (m_pDoc->m_pAttrCtrl->GetLdgr(csGroupName, ldgr));

	ldgr.Initialize();
	ldgr.GroupName = csGroupName;

	if (!m_pDoc->m_pDataCtrl->AddLdgr(ldgr))
		return;

	T_LDGR_K TempLdgrK = m_pDoc->m_pAttrCtrl->GetLdgrKey(csGroupName);
	if (TempLdgrK == 0)
		return;

	if (m_mKey2hItem[LOAD_GROUP_FLD].find(TempLdgrK) != m_mKey2hItem[LOAD_GROUP_FLD].end())
	{
		HTREEITEM hCurItem = m_mKey2hItem[LOAD_GROUP_FLD][TempLdgrK];
		EnsureVisible(hCurItem);
		EditLabel(hCurItem);

		SelectEditText();
	}
}

void CTreeGroupCtrl::OnTendGroupNew() //tnsel
{
	if (!m_pDoc) return;
	int nCount = m_pDoc->m_pAttrCtrl->GetCountTdgr();
	T_TDGR_D tdgr;
	CString csGroupName;
	do
	{
		nCount++;          // New Group %d 
		csGroupName.Format(_LS(IDS_WG_TREEMENU_ADD3_Tend_Group__d), nCount);
	} while (m_pDoc->m_pAttrCtrl->GetTdgr(csGroupName, tdgr));

	tdgr.Initialize();
	tdgr.GroupName = csGroupName;

	if (!m_pDoc->m_pDataCtrl->AddTdgr(tdgr))
		return;

	T_TDGR_K TempTdgrK = m_pDoc->m_pAttrCtrl->GetTdgrKey(csGroupName);
	if (TempTdgrK == 0)
		return;

	if (m_mKey2hItem[TEND_GROUP_FLD].find(TempTdgrK) != m_mKey2hItem[TEND_GROUP_FLD].end())
	{
		HTREEITEM hCurItem = m_mKey2hItem[TEND_GROUP_FLD][TempTdgrK];
		EnsureVisible(hCurItem);
		EditLabel(hCurItem);

		SelectEditText();
	}
}

void CTreeGroupCtrl::OnLdarGroupNew()
{
	if (!m_pDoc) return;
	int nCount = m_pDoc->m_pAttrCtrl2->GetCountLagr();
	T_LAGR_D lagr;
	CString csGroupName;
	do
	{
		nCount++;          // New Group %d 
		csGroupName.Format(_LS(IDS_WG_TREEMENU_ADD3_Ldar_Group__d), nCount);
	} while (m_pDoc->m_pAttrCtrl2->GetLagr(csGroupName, lagr));

	lagr.Initialize();
	lagr.GroupName = csGroupName;

	if (!m_pDoc->m_pDataCtrl->AddLagr(lagr))
		return;

	T_LAGR_K TempLagrK = m_pDoc->m_pAttrCtrl2->GetLagrKey(csGroupName);
	if (TempLagrK == 0)
		return;

	if (m_mKey2hItem[LDAR_GROUP_FLD].find(TempLagrK) != m_mKey2hItem[LDAR_GROUP_FLD].end())
	{
		HTREEITEM hCurItem = m_mKey2hItem[LDAR_GROUP_FLD][TempLagrK];
		EnsureVisible(hCurItem);
		EditLabel(hCurItem);

		SelectEditText();
	}
}
//////////
//////////
void CTreeGroupCtrl::OnStrtGroupRename()
{
	HTREEITEM hModify = GetSelectedItem();
	if (hModify == 0)
	{

		AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_rename_));
		return;
	}
	EditLabel(hModify);
}

void CTreeGroupCtrl::OnBndrGroupRename()
{
	HTREEITEM hModify = GetSelectedItem();
	if (hModify == 0)
	{
		//AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_rename_));
		return;
	}
	EditLabel(hModify);
}

void CTreeGroupCtrl::OnLoadGroupRename()
{
	HTREEITEM hModify = GetSelectedItem();
	if (hModify == 0)
	{
		//AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_rename_));
		return;
	}
	EditLabel(hModify);
}

void CTreeGroupCtrl::OnTendGroupRename() //tnsel
{
	HTREEITEM hModify = GetSelectedItem();
	if (hModify == 0)
	{
		//AfxMessageBox(_LS(IDS_TMGD_Error___No_selected_group_to_rename_));
		return;
	}
	EditLabel(hModify);
}

void CTreeGroupCtrl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	if (lHint == D_UPDATE_SEL_ADD || lHint == D_UPDATE_SEL_DEL ||
		lHint == D_UPDATE_SEL_PRE || lHint == D_UPDATE_SEL_POST)
		return;

	CBCGPGridCtrl::m_pHighlightedItem = nullptr;
	UnselectAllItems();

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	{
		InitializeTree init_tree(this);
		SetItemTree();
		//UpdateBuffer();
	// do something...
		break;
	}
	case D_UPDATE_BUFFER_BEFORE:
		// do something...
		break;
	case D_UPDATE_BUFFER_AFTER:
	{
		UpdateBuffer();
		// SetItemTree(FALSE);
		break;
	}
	case D_UPDATE_UNIT:
		// do something...
		break;
	default:
		//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
		ASSERT(TRUE);
	}
}

void CTreeGroupCtrl::UpdateBuffer()
{
	BOOL bStrtGroupItem = FALSE;
	BOOL bBndrGroupItem = FALSE;
	BOOL bLoadGroupItem = FALSE;
	BOOL bTendGroupItem = FALSE;
	BOOL bLdarGroupItem = FALSE;

	int nModified = 0;

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	//if (nCount == 0) { SetBoundaryItemTree(m_pDoc,FALSE); return; } // Panel Zone 때문에... DBALL로 넘어오는 데... 그때는 nCount==0... 으~~
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	std::set<T_GRUP_K> stModGRUPKey, stDelGRUPKey;
	std::set<T_BNGR_K> stModBNGRKey, stDelBNGRKey;
	std::set<T_LDGR_K> stModLDGRKey, stDelLDGRKey;
	std::set<T_TDGR_K> stModTDGRKey, stDelTDGRKey;
	std::set<T_LAGR_K> stModLAGRKey, stDelLAGRKey;

	T_GRUP_K GrupK; T_GRUP_D GrupD;
	T_BNGR_K BngrK; T_BNGR_D BngrD;
	T_LDGR_K LdgrK; T_LDGR_D LdgrD;
	T_TDGR_K TdgrK; T_TDGR_D TdgrD;
	T_LAGR_K LagrK; T_LAGR_D LagrD;

	SetInUpdateBuffer(TRUE);

	InitializeTree init_tree(this);

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_GRUP_ADD):  case(UR_GRUP_MFS): case(UR_GRUP_MFD):
		{
			pViewBuff->GetGrup(nKey, GrupK, GrupD);
			stModGRUPKey.insert(GrupK);
			break;
		}
		case(UR_GRUP_DEL):
		{
			pViewBuff->GetGrup(nKey, GrupK, GrupD);
			stDelGRUPKey.insert(GrupK);
			break;
		}
		case(UR_GRUP_ITEM_ADD): case(UR_GRUP_ITEM_DEL):
		{
			T_NODE_K nodeK;
			T_ELEM_K elemK;
			pViewBuff->GetGrupItem(nKey, GrupK, nodeK, elemK);
			stModGRUPKey.insert(GrupK);
			break;
		}
		case(UR_BNGR_ADD): case(UR_BNGR_MFS): case(UR_BNGR_MFD):
		{
			pViewBuff->GetBngr(nKey, BngrK, BngrD);
			stModBNGRKey.insert(BngrK);
			break;
		}
		case(UR_BNGR_DEL):
		{
			pViewBuff->GetBngr(nKey, BngrK, BngrD);
			stDelBNGRKey.insert(BngrK);
			break;
		}
		case(UR_LDGR_ADD): case(UR_LDGR_MFS): case(UR_LDGR_MFD):
		{
			pViewBuff->GetLdgr(nKey, LdgrK, LdgrD);
			stModLDGRKey.insert(LdgrK);
			break;
		}
		case(UR_LDGR_DEL):
		{
			pViewBuff->GetLdgr(nKey, LdgrK, LdgrD);
			stDelLDGRKey.insert(LdgrK);
			break;
		}
		case(UR_TDGR_ADD): case(UR_TDGR_MFS): case(UR_TDGR_MFD):
		{
			pViewBuff->GetTdgr(nKey, TdgrK, TdgrD);
			stModTDGRKey.insert(TdgrK);
			break;
		}
		case(UR_TDGR_DEL):
		{
			pViewBuff->GetTdgr(nKey, TdgrK, TdgrD);
			stDelTDGRKey.insert(TdgrK);
			break;
		}
		case(UR_LAGR_ADD): case(UR_LAGR_MFS): case(UR_LAGR_MFD):
		{
			pViewBuff->GetLagr(nKey, LagrK, LagrD);
			stModLAGRKey.insert(LagrK);
			break;
		}
		case(UR_LAGR_DEL):
		{
			pViewBuff->GetLagr(nKey, LagrK, LagrD);
			stDelLAGRKey.insert(LagrK);
			break;
		}
		default:
			break;
		}
	} // end of while


	if (stDelGRUPKey.size() > 0)  DeleteTreeItem_GRUP(stDelGRUPKey);
	if (stDelBNGRKey.size() > 0)  DeleteTreeItem_BNGR(stDelBNGRKey);
	if (stDelLDGRKey.size() > 0)  DeleteTreeItem_LDGR(stDelLDGRKey);
	if (stDelTDGRKey.size() > 0)  DeleteTreeItem_TDGR(stDelTDGRKey); //Loading Area Group
	if (stDelLAGRKey.size() > 0)  DeleteTreeItem_LAGR(stDelLAGRKey); // tnsel

	if (stModGRUPKey.size() > 0)  RenewTreeItem_GRUP(stModGRUPKey);
	if (stModBNGRKey.size() > 0)  RenewTreeItem_BNGR(stModBNGRKey);
	if (stModLDGRKey.size() > 0)  RenewTreeItem_LDGR(stModLDGRKey);
	if (stModTDGRKey.size() > 0)  RenewTreeItem_TDGR(stModTDGRKey); //Loading Area Group
	if (stModLAGRKey.size() > 0)  RenewTreeItem_LAGR(stModLAGRKey); // tnsel


	SetInUpdateBuffer(FALSE);
}

void CTreeGroupCtrl::OnStrtGroupNewMulti()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMStrtGrupDefDlg::IDD);
}

void CTreeGroupCtrl::OnBndrGroupNewMulti()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMBndrGrupDefDlg::IDD);
}

void CTreeGroupCtrl::OnLoadGroupNewMulti()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLoadGroupDlg::IDD);
}

void CTreeGroupCtrl::OnTendGroupNewMulti() //tnsel
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMTendonDefDlg::IDD);
}

////////** 공통사항 **////////////////
void CTreeGroupCtrl::OnStrtGroupAssign()
{
	AssignAttribute(NULL, 0, FALSE);
}

void CTreeGroupCtrl::OnStrtGroupAssignPlus()
{
	AssignAttribute(NULL, 1, FALSE);
}

void CTreeGroupCtrl::OnStrtGroupAssignMinus()
{
	AssignAttribute(NULL, 2, FALSE);
}

void CTreeGroupCtrl::OnStrtGroupActive()
{
	// TODO: Add your command handler code here
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	ActiveEntity(nWhat);
}

void CTreeGroupCtrl::OnTendGroupActive() // Tendon Group Show 
{
	I_GENModelBase::UTIL_DeleteAllDisplayObjST();
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Load.bTendonProp = FALSE;
	dspOpt.m_Load.bTendonPropTxt = FALSE;
	ExportDisplaySetting::SET(&dspOpt);

	CArray<T_TDNA_K, T_TDNA_K> aTdnaKeyList;
	CArray<T_TDGR_K, T_TDGR_K> aTdgrKeyList;

	aTdnaKeyList.RemoveAll();
	aTdgrKeyList.RemoveAll();

	HTREEITEM ht, prvHt;
	int pos = 0;
	ht = GetSelectedItem();
	for (prvHt = ht; prvHt != NULL; ht = prvHt, pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);

	this->GetSelectedTdgrKeyListWithMultiTreeItemSelect(aTdgrKeyList);

	m_pDoc->m_pAttrCtrl->GetKeyListTdgrUser(&aTdgrKeyList, &aTdnaKeyList);

	for (int j = 0; j < aTdnaKeyList.GetSize(); j++)
	{
		I_GENModelBase::GetCurMySelfST()->UTIL_SetDisplayObj(UR_TDNA_ADD, &aTdnaKeyList[j]);
	}

	I_GENModelBase::GetCurMySelfST()->ViewInvalidate4DispSettingChange(FALSE);
}
void CTreeGroupCtrl::OnStrtGroupDelete()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	DeleteItemEntity(nWhat);
}

void CTreeGroupCtrl::OnTendGroupDelete()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	DeleteItemEntity(nWhat);
}

void CTreeGroupCtrl::OnStrtGroupInactive() // Tendon Group Hide
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	InactiveEntity(nWhat);
}

/* 규남이가 만든것 */

void CTreeGroupCtrl::OnTendGroupInactive()
{
	I_GENModelBase::UTIL_DeleteAllDisplayObjST();
	// ex) Display 여러번 하여도 UnDisplay함수 한번만 하여도  Hide 되도록 하는 함수 

	CArray<T_TDNA_K, T_TDNA_K> aTdnaKeyList;
	CArray<T_TDGR_K, T_TDGR_K> aTdgrKeyList;

	aTdnaKeyList.RemoveAll();
	aTdgrKeyList.RemoveAll();

	HTREEITEM ht, prvHt;
	int pos = 0;
	ht = GetSelectedItem();
	for (prvHt = ht; prvHt != NULL; ht = prvHt, pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);


	this->GetSelectedTdgrKeyListWithMultiTreeItemSelect(aTdgrKeyList);
	m_pDoc->m_pAttrCtrl->GetKeyListTdgrUser(&aTdgrKeyList, &aTdnaKeyList);

	for (int j = 0; j < aTdnaKeyList.GetSize(); j++)
	{
		I_GENModelBase::GetCurMySelfST()->UTIL_DelDisplayObj(UR_TDNA_ADD, &aTdnaKeyList[j]);
	}

	I_GENModelBase::GetCurMySelfST()->ViewInvalidate4DispSettingChange(FALSE);
}
/***************************/

void CTreeGroupCtrl::OnStrtGroupSelect()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	SelectEntity(nWhat, TRUE);
}

void CTreeGroupCtrl::OnStrtGroupUnselect()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	UnselectEntity(nWhat);
}
/////////////////////////////////////////////

void CTreeGroupCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_DELETE)
	{
		// iGen 전용메뉴에서 지정한 Strt Group은 삭제 불가능 처리. (Insert:추가는 상관없음)
		CArray<HTREEITEM, HTREEITEM> aItem;
		int selItemNum = GetSelectedItemList(aItem);
		for (int i = 0; i < selItemNum; i++)
		{
			if (IsJUDGroup(aItem[i])) return;
		}

		HTREEITEM hTM = GetSelectedItem();
		if (hTM)
		{
			UINT nWhat = GetItemData(hTM);
			DeleteItemEntity(nWhat);
		}
	}

	if (nChar == VK_INSERT)
	{
		HTREEITEM hTM = GetSelectedItem();
		if (hTM)
		{
			UINT nWhat = GetItemData(hTM);

			switch (nWhat)
			{
			case STRT_GROUP_FLD:
			case STRT_GROUP_LST:
				OnStrtGroupNew();
				break;
			case BNDR_GROUP_FLD:
			case BNDR_GROUP_LST:
				OnBndrGroupNew();
				break;
			case LOAD_GROUP_FLD:
			case LOAD_GROUP_LST:
				OnLoadGroupNew();
				break;
			case TEND_GROUP_FLD: //tnsel
			case TEND_GROUP_LST: //tnsel
				OnTendGroupNew();
				break;
			case LDAR_GROUP_FLD: //Loading Area
			case LDAR_GROUP_LST: //Loading Area
				OnLdarGroupNew();
				break;
			default:
				break;
			}
		}
	}

	MBaseTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

// MNET1199, KJH20050218
void CTreeGroupCtrl::OnStrtGroupDisplay()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;

	UINT nWhat = GetItemData(hTM);
	if (nWhat != BNDR_GROUP_LST) return; // 현재는 boundary group만 display

	DisplayEntity(nWhat, TRUE);
}

void CTreeGroupCtrl::OnStrtGroupUndisplay()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;

	UINT nWhat = GetItemData(hTM);
	if (nWhat != BNDR_GROUP_LST) return; // 현재는 boundary group만 display

	UndisplayEntity(nWhat);
}

BOOL CTreeGroupCtrl::DisplayEntity(UINT data, BOOL bReplace)
{
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);

	dspOpt.m_Bndr.SetDefault();
	dspOpt.m_Node.bSkew = FALSE;

	dspOpt.m_Bndr.bAllGroup = FALSE;
	CArray<T_BNGR_K, T_BNGR_K> aBngrK;
	this->GetSelectedBngrKeyListWithMultiTreeItemSelect(aBngrK);
	dspOpt.m_Bndr.m_GroupKeys.Copy(aBngrK);

	dspOpt.m_Bndr.bCons = TRUE; // support
	dspOpt.m_Bndr.bNspr = TRUE; // point spring
	dspOpt.m_Bndr.bGspr = TRUE; // general spring
	dspOpt.m_Bndr.bElasticLink = TRUE; // elastic link
	dspOpt.m_Bndr.bElasticLinkAxis = TRUE;
	dspOpt.m_Bndr.bNonlinearLink = TRUE; // general link
	dspOpt.m_Bndr.bNonlinearLinkAxis = TRUE;
	dspOpt.m_Bndr.bCGLP = TRUE; // change general link property
	dspOpt.m_Bndr.bFrls = TRUE; // beam end release
	dspOpt.m_Bndr.bFrlsTxt = FALSE;
	dspOpt.m_Bndr.bFrlsSym = TRUE;
	dspOpt.m_Bndr.bOffs = TRUE; // beam end offset
	dspOpt.m_Bndr.bPlateEndR = TRUE; // plate end release
	dspOpt.m_Bndr.bPlateEndRTxt = FALSE;
	dspOpt.m_Bndr.bPlateEndRSym = TRUE;
	dspOpt.m_Bndr.bRigd = TRUE; // rigid link
	dspOpt.m_Bndr.bMcon = TRUE; // Linear Constraints
	dspOpt.m_Bndr.bStoryDia = TRUE; // Story Diaphragm

	ExportDisplaySetting::SET(&dspOpt);

	return TRUE;
}

void CTreeGroupCtrl::UndisplayEntity(UINT data)
{
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);

	int nSize = dspOpt.m_Bndr.m_GroupKeys.GetSize();
	if (nSize > 0)
	{
		CArray<T_BNGR_K, T_BNGR_K> aTemp, aBngrK;
		CString strSel(_T(""));
		this->GetSelectedBngrKeyListWithMultiTreeItemSelect(aTemp);

		BOOL bTmp = TRUE;
		CMap<T_BNGR_K, T_BNGR_K, BOOL, BOOL> mTmp;
		for (int i = 0; i < nSize; i++) mTmp.SetAt(dspOpt.m_Bndr.m_GroupKeys[i], bTmp);
		for (int i = 0; i < aTemp.GetSize(); i++) if (mTmp.Lookup(aTemp[i], bTmp))
			mTmp.RemoveKey(aTemp[i]);
		POSITION pos = mTmp.GetStartPosition();
		while (pos)
		{
			T_BNGR_K key;
			mTmp.GetNextAssoc(pos, key, bTmp);
			aBngrK.Add(key);
		}
		qsort(aBngrK.GetData(), aBngrK.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
		dspOpt.m_Bndr.bAllGroup = FALSE;
		dspOpt.m_Bndr.m_GroupKeys.Copy(aBngrK);

		dspOpt.m_Bndr.bCons = TRUE; // support
		dspOpt.m_Bndr.bNspr = TRUE; // point spring
		dspOpt.m_Bndr.bGspr = TRUE; // general spring
		dspOpt.m_Bndr.bElasticLink = TRUE; // elastic link
		dspOpt.m_Bndr.bElasticLinkAxis = TRUE;
		dspOpt.m_Bndr.bNonlinearLink = TRUE; // general link
		dspOpt.m_Bndr.bNonlinearLinkAxis = TRUE;
		dspOpt.m_Bndr.bCGLP = TRUE; // change general link property
		dspOpt.m_Bndr.bFrls = TRUE; // beam end release
		dspOpt.m_Bndr.bFrlsTxt = FALSE;
		dspOpt.m_Bndr.bFrlsSym = TRUE;
		dspOpt.m_Bndr.bOffs = TRUE; // beam end offset
		dspOpt.m_Bndr.bPlateEndR = TRUE; // plate end release
		dspOpt.m_Bndr.bPlateEndRTxt = FALSE;
		dspOpt.m_Bndr.bPlateEndRSym = TRUE;
		dspOpt.m_Bndr.bRigd = TRUE; // rigid link
		dspOpt.m_Bndr.bMcon = TRUE; // Linear Constraints
		dspOpt.m_Bndr.bStoryDia = TRUE; // Story Diaphragm
	}
	else
	{
		dspOpt.m_Bndr.SetDefault();
		dspOpt.m_Node.bSkew = FALSE;
	}

	ExportDisplaySetting::SET(&dspOpt);
}

void CTreeGroupCtrl::RenewTreeItem_GRUP(std::set<T_GRUP_K> stKey)
{
	if (m_mFld2hItem.find(STRT_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[STRT_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[STRT_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[STRT_GROUP_FLD];

	for (T_GRUP_K GrupK : stKey)
	{
		T_GRUP_D GrupD;
		if (!m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD))
			continue;

		CString tStr;
		tStr.Format(_LS(IDS_WG_TREEMENU_ADD3__s___Node__d___Element__d__), GrupD.GroupName, GrupD.arKeyNode.GetSize(), GrupD.arKeyElem.GetSize());

		if (GrupD.nPlaneType != 0)
			tStr += "*";

		if (mKey2hItem.find(GrupK) == mKey2hItem.end())
		{
			//Insert
			HTREEITEM hCurItem = InsertTreeItem(hItemParent, STRT_GROUP_LST, tStr, _T("ico24_treemenu_group_structuregroup"));
			mKey2hItem[GrupK] = hCurItem;
			mhItem2Key[hCurItem] = GrupK;
		}
		else
		{
			// Modify
			SetItemText(mKey2hItem[GrupK], tStr);
		}
	}
	ArrangeOrderTreeItem_GRUP();
	InitGroupItemTree();
}

void CTreeGroupCtrl::RenewTreeItem_BNGR(std::set<T_BNGR_K> stKey)
{
	if (m_mFld2hItem.find(BNDR_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[BNDR_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[BNDR_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[BNDR_GROUP_FLD];

	for (T_BNGR_K BngrK : stKey)
	{
		T_BNGR_D BngrD;
		if (!m_pDoc->m_pAttrCtrl->GetBngr(BngrK, BngrD))
			continue;

		CString tStr;
		tStr.Format(_T("%s"), BngrD.GroupName);

		if (mKey2hItem.find(BngrK) == mKey2hItem.end())
		{
			//Insert
			HTREEITEM hCurItem = InsertTreeItem(hItemParent, BNDR_GROUP_LST, tStr, _T("ico24_treemenu_group_boundarygroup"));
			mKey2hItem[BngrK] = hCurItem;
			mhItem2Key[hCurItem] = BngrK;
		}
		else
		{
			// Modify
			SetItemText(mKey2hItem[BngrK], tStr);
		}
	}
	ArrangeOrderTreeItem_BNGR();
	InitBoundaryGroupItemTree();
}

void CTreeGroupCtrl::RenewTreeItem_LDGR(std::set<T_LDGR_K> stKey)
{
	if (m_mFld2hItem.find(LOAD_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[LOAD_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[LOAD_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[LOAD_GROUP_FLD];

	for (T_LDGR_K LdgrK : stKey)
	{
		T_LDGR_D LdgrD;
		if (!m_pDoc->m_pAttrCtrl->GetLdgr(LdgrK, LdgrD))
			continue;

		CString tStr;
		tStr.Format(_T("%s"), LdgrD.GroupName);

		if (mKey2hItem.find(LdgrK) == mKey2hItem.end())
		{
			//Insert
			HTREEITEM hCurItem = InsertTreeItem(hItemParent, LOAD_GROUP_LST, tStr, _T("ico24_treemenu_group_loadgroup"));
			mKey2hItem[LdgrK] = hCurItem;
			mhItem2Key[hCurItem] = LdgrK;
		}
		else
		{
			// Modify
			SetItemText(mKey2hItem[LdgrK], tStr);
		}
	}
	ArrangeOrderTreeItem_LDGR();
	InitLoadGroupItemTree();
}

void CTreeGroupCtrl::RenewTreeItem_TDGR(std::set<T_TDGR_K> stKey)
{
	if (m_mFld2hItem.find(TEND_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[TEND_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[TEND_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[TEND_GROUP_FLD];

	for (T_TDGR_K TdgrK : stKey)
	{
		T_TDGR_D TdgrD;
		if (!m_pDoc->m_pAttrCtrl->GetTdgr(TdgrK, TdgrD))
			continue;

		CString tStr;
		tStr.Format(_T("%s"), TdgrD.GroupName);

		if (mKey2hItem.find(TdgrK) == mKey2hItem.end())
		{
			//Insert
			HTREEITEM hCurItem = InsertTreeItem(hItemParent, TEND_GROUP_LST, tStr, _T("ico24_treemenu_group_tendongroup"));
			mKey2hItem[TdgrK] = hCurItem;
			mhItem2Key[hCurItem] = TdgrK;
		}
		else
		{
			// Modify
			SetItemText(mKey2hItem[TdgrK], tStr);
		}
	}
	ArrangeOrderTreeItem_TDGR();
	InitTendGroupItemTree();
}

void CTreeGroupCtrl::RenewTreeItem_LAGR(std::set<T_LAGR_K> stKey)
{
	if (m_mFld2hItem.find(LDAR_GROUP_FLD) == m_mFld2hItem.end())
		return; // Gen에서만 보임

	HTREEITEM hItemParent = m_mFld2hItem[LDAR_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[LDAR_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[LDAR_GROUP_FLD];

	for (T_LAGR_K LagrK : stKey)
	{
		T_LAGR_D LagrD;
		if (!m_pDoc->m_pAttrCtrl2->GetLagr(LagrK, LagrD))
			continue;

		CString tStr;
		tStr.Format(_T("%s"), LagrD.GroupName);

		if (mKey2hItem.find(LagrK) == mKey2hItem.end())
		{
			//Insert
			HTREEITEM hCurItem = InsertTreeItem(hItemParent, LDAR_GROUP_LST, tStr, _T("ico24_treemenu_group_loadingareaplane"));
			mKey2hItem[LagrK] = hCurItem;
			mhItem2Key[hCurItem] = LagrK;
		}
		else
		{
			// Modify
			SetItemText(mKey2hItem[LagrK], tStr);
		}
	}
	ArrangeOrderTreeItem_LAGR();
	InitLdarGroupItemTree();
}

void CTreeGroupCtrl::ArrangeOrderTreeItem_GRUP()
{
	if (m_mFld2hItem.find(STRT_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[STRT_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[STRT_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[STRT_GROUP_FLD];

	CArray<T_GRUP_K, T_GRUP_K> aGrupKey;
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(aGrupKey);

	if (mKey2hItem.size() < 1 && mhItem2Key.size() < 1 && aGrupKey.GetCount() < 1)
	{
		return;
	}

	int childIndex = 0;
	try
	{
		HTREEITEM hChildItem = GetChildItem(hItemParent);
		while (hChildItem != NULL)
		{
			T_BNGR_K GrupK = aGrupKey[childIndex];

			mKey2hItem[GrupK] = hChildItem;
			mhItem2Key[hChildItem] = GrupK;

			T_GRUP_D GrupD;
			if (!m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD))
				continue;

			CString tStr;
			tStr.Format(_LS(IDS_WG_TREEMENU_ADD3__s___Node__d___Element__d__), GrupD.GroupName, GrupD.arKeyNode.GetSize(), GrupD.arKeyElem.GetSize());

			if (GrupD.nPlaneType != 0)
				tStr += "*";

			SetItemText(hChildItem, tStr);

			hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
			childIndex++;
		}
	}
	catch (...)
	{
		//exist some groupitems no delete,why?
		//So that there is an array out of bounds when undo.
		ASSERT(0);
	}

	ASSERT(aGrupKey.GetCount() == childIndex);
}

void CTreeGroupCtrl::ArrangeOrderTreeItem_BNGR()
{
	if (m_mFld2hItem.find(BNDR_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[BNDR_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[BNDR_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[BNDR_GROUP_FLD];

	CArray<T_BNGR_K, T_BNGR_K> aBngrKey;
	m_pDoc->m_pAttrCtrl->GetBngrKeyList(aBngrKey);

	if (mKey2hItem.size() < 1 && mhItem2Key.size() < 1 && aBngrKey.GetCount() < 1)
	{
		return;
	}

	int childIndex = 0;
	HTREEITEM hChildItem = GetChildItem(hItemParent);
	while (hChildItem != NULL)
	{
		T_BNGR_K BngrK = aBngrKey[childIndex];

		mKey2hItem[BngrK] = hChildItem;
		mhItem2Key[hChildItem] = BngrK;

		T_BNGR_D BngrD;
		if (!m_pDoc->m_pAttrCtrl->GetBngr(BngrK, BngrD))
			continue;

		CString tStr;
		tStr.Format(_T("%s"), BngrD.GroupName);

		SetItemText(hChildItem, tStr);

		hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
		childIndex++;
	}

	ASSERT(aBngrKey.GetCount() == childIndex);
}

void CTreeGroupCtrl::ArrangeOrderTreeItem_LDGR()
{
	if (m_mFld2hItem.find(LOAD_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[LOAD_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[LOAD_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[LOAD_GROUP_FLD];

	CArray<T_LDGR_K, T_LDGR_K> aLdgrKey;
	m_pDoc->m_pAttrCtrl->GetLdgrKeyList(aLdgrKey);

	if (mKey2hItem.size() < 1 && mhItem2Key.size() < 1 && aLdgrKey.GetCount() < 1)
	{
		return;
	}

	int childIndex = 0;
	HTREEITEM hChildItem = GetChildItem(hItemParent);
	while (hChildItem != NULL)
	{
		T_LDGR_K LdgrK = aLdgrKey[childIndex];

		mKey2hItem[LdgrK] = hChildItem;
		mhItem2Key[hChildItem] = LdgrK;

		T_LDGR_D LdgrD;
		if (!m_pDoc->m_pAttrCtrl->GetLdgr(LdgrK, LdgrD))
			continue;

		CString tStr;
		tStr.Format(_T("%s"), LdgrD.GroupName);

		SetItemText(hChildItem, tStr);

		hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
		childIndex++;
	}

	ASSERT(aLdgrKey.GetCount() == childIndex);
}

void CTreeGroupCtrl::ArrangeOrderTreeItem_TDGR()
{
	if (m_mFld2hItem.find(TEND_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[TEND_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[TEND_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[TEND_GROUP_FLD];

	CArray<T_TDGR_K, T_TDGR_K> aTdgrKey;
	m_pDoc->m_pAttrCtrl->GetTdgrKeyList(aTdgrKey);

	if (mKey2hItem.size() < 1 && mhItem2Key.size() < 1 && aTdgrKey.GetCount() < 1)
	{
		return;
	}

	int childIndex = 0;
	HTREEITEM hChildItem = GetChildItem(hItemParent);
	while (hChildItem != NULL)
	{
		T_TDGR_K TdgrK = aTdgrKey[childIndex];

		mKey2hItem[TdgrK] = hChildItem;
		mhItem2Key[hChildItem] = TdgrK;

		T_TDGR_D TdgrD;
		if (!m_pDoc->m_pAttrCtrl->GetTdgr(TdgrK, TdgrD))
			continue;

		CString tStr;
		tStr.Format(_T("%s"), TdgrD.GroupName);

		SetItemText(hChildItem, tStr);

		hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
		childIndex++;
	}

	ASSERT(aTdgrKey.GetCount() == childIndex);
}

void CTreeGroupCtrl::ArrangeOrderTreeItem_LAGR()
{
	if (m_mFld2hItem.find(LDAR_GROUP_FLD) == m_mFld2hItem.end())
		return; // Gen에서만 보임

	HTREEITEM hItemParent = m_mFld2hItem[LDAR_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[LDAR_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[LDAR_GROUP_FLD];

	CArray<T_LAGR_K, T_LAGR_K> aLagrKey;
	m_pDoc->m_pAttrCtrl2->GetLagrKeyList(aLagrKey);

	if (mKey2hItem.size() < 1 && mhItem2Key.size() < 1 && aLagrKey.GetCount() < 1)
	{
		return;
	}

	int childIndex = 0;
	HTREEITEM hChildItem = GetChildItem(hItemParent);
	while (hChildItem != NULL)
	{
		T_LAGR_K LagrK = aLagrKey[childIndex];

		mKey2hItem[LagrK] = hChildItem;
		mhItem2Key[hChildItem] = LagrK;

		T_LAGR_D LagrD;
		if (!m_pDoc->m_pAttrCtrl2->GetLagr(LagrK, LagrD))
			continue;

		CString tStr;
		tStr.Format(_T("%s"), LagrD.GroupName);

		SetItemText(hChildItem, tStr);

		hChildItem = GetNextSiblingItemWithoutFilter(hChildItem);
		childIndex++;
	}

	ASSERT(aLagrKey.GetCount() == childIndex);
}

void CTreeGroupCtrl::DeleteTreeItem_GRUP(std::set<T_GRUP_K> stKey)
{
	if (m_mFld2hItem.find(STRT_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[STRT_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[STRT_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[STRT_GROUP_FLD];

	for (T_GRUP_K GrupK : stKey)
	{
		if (m_pDoc->m_pAttrCtrl->ExistGrup(GrupK))
		{
			ASSERT(FALSE);
			continue;
		}

		if (mKey2hItem.find(GrupK) == mKey2hItem.end())
		{
			//ASSERT(FALSE);
			continue;
		}

		HTREEITEM hItem = mKey2hItem[GrupK];

		mKey2hItem.erase(GrupK);
		mhItem2Key.erase(hItem);

		DeleteItem(hItem);
	}
	InitGroupItemTree();
}

void CTreeGroupCtrl::DeleteTreeItem_BNGR(std::set<T_BNGR_K> stKey)
{
	if (m_mFld2hItem.find(BNDR_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[BNDR_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[BNDR_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[BNDR_GROUP_FLD];

	for (T_BNGR_K BngrK : stKey)
	{
		if (m_pDoc->m_pAttrCtrl->ExistBngr(BngrK))
		{
			ASSERT(FALSE);
			continue;
		}

		if (mKey2hItem.find(BngrK) == mKey2hItem.end())
		{
			//ASSERT(FALSE);
			continue;
		}

		HTREEITEM hItem = mKey2hItem[BngrK];

		mKey2hItem.erase(BngrK);
		mhItem2Key.erase(hItem);

		DeleteItem(hItem);
	}
	InitBoundaryGroupItemTree();
}

void CTreeGroupCtrl::DeleteTreeItem_LDGR(std::set<T_LDGR_K> stKey)
{
	if (m_mFld2hItem.find(LOAD_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[LOAD_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[LOAD_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[LOAD_GROUP_FLD];

	for (T_LDGR_K LdgrK : stKey)
	{
		if (m_pDoc->m_pAttrCtrl->ExistLdgr(LdgrK))
		{
			ASSERT(FALSE);
			continue;
		}

		if (mKey2hItem.find(LdgrK) == mKey2hItem.end())
		{
			//ASSERT(FALSE);
			continue;
		}

		HTREEITEM hItem = mKey2hItem[LdgrK];

		mKey2hItem.erase(LdgrK);
		mhItem2Key.erase(hItem);

		DeleteItem(hItem);
	}
	InitLoadGroupItemTree();
}

void CTreeGroupCtrl::DeleteTreeItem_TDGR(std::set<T_TDGR_K> stKey)
{
	if (m_mFld2hItem.find(TEND_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[TEND_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[TEND_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[TEND_GROUP_FLD];

	for (T_TDGR_K TdgrK : stKey)
	{
		if (m_pDoc->m_pAttrCtrl->ExistTdgr(TdgrK))
		{
			ASSERT(FALSE);
			continue;
		}

		if (mKey2hItem.find(TdgrK) == mKey2hItem.end())
		{
			//ASSERT(FALSE);
			continue;
		}

		HTREEITEM hItem = mKey2hItem[TdgrK];

		mKey2hItem.erase(TdgrK);
		mhItem2Key.erase(hItem);

		DeleteItem(hItem);
	}
	InitTendGroupItemTree();
}

void CTreeGroupCtrl::DeleteTreeItem_LAGR(std::set<T_LAGR_K> stKey)
{
	if (m_mFld2hItem.find(LDAR_GROUP_FLD) == m_mFld2hItem.end())
		return;

	HTREEITEM hItemParent = m_mFld2hItem[LDAR_GROUP_FLD];
	if (hItemParent == NULL)
		return;

	std::map<T_KEY, HTREEITEM>& mKey2hItem = m_mKey2hItem[LDAR_GROUP_FLD];
	std::map<HTREEITEM, T_KEY>& mhItem2Key = m_mhItem2Key[LDAR_GROUP_FLD];

	for (T_LAGR_K LagrK : stKey)
	{
		if (m_pDoc->m_pAttrCtrl2->ExistLagr(LagrK))
		{
			ASSERT(FALSE);
			continue;
		}

		if (mKey2hItem.find(LagrK) == mKey2hItem.end())
		{
			//ASSERT(FALSE);
			continue;
		}

		HTREEITEM hItem = mKey2hItem[LagrK];

		mKey2hItem.erase(LagrK);
		mhItem2Key.erase(hItem);

		DeleteItem(hItem);
	}
	InitGroupItemTree();
}

void CTreeGroupCtrl::SetTreeImage()
{
	AddSVGImage(_T("ico24_structure_group_structure"));
	AddSVGImage(_T("ico24_structure_group_defineboundarygroup"));
	AddSVGImage(_T("ico24_structure_group_defineloadgroup"));
	AddSVGImage(_T("ico24_structure_group_defineloadingareagroup"));
	AddSVGImage(_T("ico24_structure_group_definetendongroup"));

	AddSVGImage(_T("ico24_treemenu_group_group"));
	AddSVGImage(_T("ico24_treemenu_group_structuregroup"));
	AddSVGImage(_T("ico24_treemenu_group_boundarygroup"));
	AddSVGImage(_T("ico24_treemenu_group_loadgroup"));
	AddSVGImage(_T("ico24_treemenu_group_loadingareaplane"));
	AddSVGImage(_T("ico24_treemenu_group_tendongroup"));
}

// END KJH20050218

void CTreeGroupCtrl::SetBkGroundColor(COLORREF BkColor)
{
	m_BkColor = BkColor;
	if (GetSafeHwnd())
	{
		SetBkColor(m_BkColor);
		Invalidate();
	}
}

BOOL CTreeGroupCtrl::IsJUDGroup(UINT data)
{
#ifdef _IGEN
	if (data != STRT_GROUP_LST) return FALSE;


	CArray<T_GRUP_K, T_GRUP_K> aGrupKeyList;
	int nGroup = GetSelectedGrupKeyListWithMultiTreeItemSelect(aGrupKeyList);

	T_GRUP_D GrupD;
	for (int i = 0; i < nGroup; i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetGrup(aGrupKeyList[i], GrupD)) continue;
		if (GrupD.nPlaneType != 0) return TRUE;
	}
#endif

	return FALSE;
}

BOOL CTreeGroupCtrl::IsJUDGroup(HTREEITEM hItem)
{
#ifdef _IGEN
	UINT Data = GetItemData(hItem);
	if (Data != STRT_GROUP_LST) return FALSE;

	if (!m_pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}

	HTREEITEM ht, prvHt;
	int pos = 0;
	ht = hItem;
	for (prvHt = ht; prvHt != NULL; ht = prvHt, pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);

	CArray<T_GRUP_K, T_GRUP_K> aGrupKeyList;
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(aGrupKeyList);
	if (aGrupKeyList.GetSize() < 1) return FALSE;

	T_GRUP_D GrupD;
	if (m_pDoc->m_pAttrCtrl->GetGrup(aGrupKeyList[pos - 1], GrupD))
	{
		if (GrupD.nPlaneType != 0) return TRUE;
	}
#endif
	return FALSE;
}

BOOL CTreeGroupCtrl::IsNeedColorPalette(HTREEITEM hTreeItem)
{
	UINT Data = GetItemData(hTreeItem);
	return Data == STRT_GROUP_LST;
}

COLORREF CTreeGroupCtrl::GetColorPalette(HTREEITEM hTreeItem)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bWireFrame = I_GENModelBase::GetCurMySelfST()->I_IsWireFrameMode();

	T_COLOR_D ColorD;
	ColorD.Initialize();
	
	UINT Data = GetItemData(hTreeItem);
	if (Data == STRT_GROUP_LST)
	{
		T_GRUP_K GrupK = m_mhItem2Key[STRT_GROUP_FLD][hTreeItem];
		pDoc->m_pAttrCtrl->GetCo_g(GrupK, ColorD);

		if (bWireFrame == TRUE)
			return ColorD.WireFrame;
		else
			return ColorD.HiddenFill;
	}

	return (COLORREF)-1;
}

void CTreeGroupCtrl::OnChangeColorPalette(HTREEITEM hTreeItem, COLORREF clr)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bWireFrame = I_GENModelBase::GetCurMySelfST()->I_IsWireFrameMode();

	T_COLOR_D ColorD;
	ColorD.Initialize();

	UINT Data = GetItemData(hTreeItem);
	if (Data == STRT_GROUP_LST)
	{
		T_GRUP_K GrupK = m_mhItem2Key[STRT_GROUP_FLD][hTreeItem];
		pDoc->m_pAttrCtrl->GetCo_g(GrupK, ColorD);

		if (bWireFrame == TRUE)
			ColorD.WireFrame = clr;
		else
			ColorD.HiddenFill = clr;

		pDoc->m_pDataCtrl->ModifyCo_g(GrupK, ColorD);
	}
}

BOOL CTreeGroupCtrl::GetCustomColor(HTREEITEM hItem)
{
	MBaseTreeCtrl::GetCustomColor(hItem);

	if (m_pDoc == 0) return FALSE;

	UINT Data = GetItemData(hItem);

	int bJUD = FALSE;

	if (STRT_GROUP_LST == Data)
	{
		if (IsJUDGroup(hItem))
		{
			bJUD = TRUE;
		}
	}

	if (bJUD == TRUE)  m_ColorData.m_clrText = (RGB(0, 0, 255));  // 2차설계 Structure Group이라면..
	else if (bJUD == FALSE)  m_ColorData.m_clrText = (RGB(0, 0, 0));
	else ASSERT(0);
	return TRUE;
}

COLORREF CTreeGroupCtrl::GetBkColor()
{
// 	COLORREF Color;
// 	if (m_pDoc != 0 && m_pDoc->GetDispOpt() != 0)
// 	{
// 		if (m_strBkColorName.IsEmpty())
// 		{
// 			COptColorObjNames CC;
// 			m_strBkColorName = CC.m_TreeMenu_Background;
// 		}
// 		CDispOptColorObj* pColor = m_pDoc->GetDispOpt()->GetColor(m_strBkColorName);
// 		Color = pColor->GetCurVal(0);
// 	}
// 	else
// 	{
// 		CWinApp* pApp = AfxGetApp(); ASSERT(pApp);
// 		CString csSection, csEntry, csValue;
// 		csSection = "DispOpt-Color";
// 		csEntry = "Treemenu	Background:Value";
// 		csValue = pApp->GetProfileString(csSection, csEntry, NULL);
// 		Color = RGB(128, 128, 128);
// 		if (!csValue.IsEmpty()) Color = (COLORREF)atoi(csValue);
// 	}
// 	return Color;
	return RGB(233, 235, 239);
}

COLORREF CTreeGroupCtrl::GetBkColorTitle()
{
	COLORREF clrColor2;
	if (m_pDoc != 0 && m_pDoc->GetDispOpt() != 0)
	{
		if (m_strBkTitleColorName.IsEmpty())
		{
			COptColorObjNames CC;
			m_strBkTitleColorName = CC.m_MV_WorktreeBkTitle;
		}
		CDispOptColorObj* pColor = m_pDoc->GetDispOpt()->GetColor(m_strBkTitleColorName);
		clrColor2 = pColor->GetCurVal(0);
	}
	else
	{
		CWinApp* pApp = AfxGetApp(); ASSERT(pApp);
		CString csSection, csEntry, csValue;
		csSection = _T("DispOpt-Color");
		csEntry = _T("Tree Menu	Background Title:Value");
		csValue = pApp->GetProfileString(csSection, csEntry, NULL);
		clrColor2 = RGB(128,128,128);
		if (!csValue.IsEmpty()) clrColor2 = (COLORREF)_ttoi(csValue);
	}
	return clrColor2;
}

void CTreeGroupCtrl::OnLdarGroupNewMulti()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLdarGrupDefDlg::IDD);
}

void CTreeGroupCtrl::OnLdarGroupSelect()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	SelectEntity(nWhat, TRUE);
}

void CTreeGroupCtrl::OnLdarGroupUnselect()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	UnselectEntity(nWhat);
}

void CTreeGroupCtrl::OnLdarGroupInactive()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	InactiveEntity(nWhat);
}

void CTreeGroupCtrl::OnLdarGroupRename()
{
	HTREEITEM hModify = GetSelectedItem();
	if (hModify == 0)
	{
		return;
	}
	EditLabel(hModify);
}

void CTreeGroupCtrl::OnLdarGroupActive()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;
	UINT nWhat = GetItemData(hTM);
	ActiveEntity(nWhat);
}

void CTreeGroupCtrl::OnLdarGroupDisplay()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;

	UINT nWhat = GetItemData(hTM);
	if (nWhat == LDAR_GROUP_LST)
	{
		CArray<T_LAGR_K, T_LAGR_K> aLagrKeyList;
		this->GetSelectedLagrKeyListWithMultiTreeItemSelect(aLagrKeyList);
		GDisplaySetting dspOpt;
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Load.nTypeLaplLagr = 2;
		int nSize, i;
		nSize = aLagrKeyList.GetSize();
		dspOpt.m_Load.m_LoadingAreaKeys.SetSize(nSize);
		for (i = 0; i < nSize; i++)
		{
			dspOpt.m_Load.m_LoadingAreaKeys[i] = aLagrKeyList[i];
		}
		ExportDisplaySetting::SET(&dspOpt);
	}
}

void CTreeGroupCtrl::OnLdarGroupUndisplay()
{
	HTREEITEM hTM = GetSelectedItem();
	if (!hTM) return;

	UINT nWhat = GetItemData(hTM);
	if (nWhat == LDAR_GROUP_LST)
	{
		CArray<T_LAGR_K, T_LAGR_K> aLagrKeyList;
		this->GetSelectedLagrKeyListWithMultiTreeItemSelect(aLagrKeyList);
		GDisplaySetting dspOpt;
		ExportDisplaySetting::GET(&dspOpt);

		if (dspOpt.m_Load.nTypeLaplLagr == 2) //켜져있다면.. 선택된것만 끈다.
		{
			int i, j, nSizeI, nSizeJ;
			nSizeI = dspOpt.m_Load.m_LoadingAreaKeys.GetSize();
			for (i = nSizeI - 1; i >= 0; i--)
			{
				nSizeJ = aLagrKeyList.GetSize();
				for (j = 0; j < nSizeJ; j++)
				{
					if (dspOpt.m_Load.m_LoadingAreaKeys[i] == aLagrKeyList[j])
					{
						dspOpt.m_Load.m_LoadingAreaKeys.RemoveAt(i);
						break;
					}
				}
			}

			if (dspOpt.m_Load.m_LoadingAreaKeys.GetSize() == 0)
			{
				dspOpt.m_Load.nTypeLaplLagr = 0;
			}
		}
		else //꺼져있다면 다끈다.
		{
			dspOpt.m_Load.nTypeLaplLagr = 0;
			dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
		}

		ExportDisplaySetting::SET(&dspOpt);
	}
}

void CTreeGroupCtrl::SelectEditText()
{
	CEdit* pEdit = GetEditControl();
	if (pEdit != NULL)
	{
		pEdit->SetSel(0, -1);
	}
}