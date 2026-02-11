// CMSihpGLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSihpPSSPOTDlg.h" // Changed from "CMSihpGLinkDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_common\wg_common_Query.h"
#include "..\wg_main\wg_mainRes2.h"

#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSihpGLinkDlg dialog

//IMPLEMENT_DYNCREATE(CCMSihpGLinkDlg, CCMChildBarBase)

enum LIST_IDX { IDX_NAME };

CCMSihpPSSPOTDlg::CCMSihpPSSPOTDlg() // Changed from CCMSihpGLinkDlg::CCMSihpGLinkDlg()
	: CCMChildBarBase(CCMSihpPSSPOTDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSihpGLinkDlg)
	//}}AFX_DATA_INIT
}


void CCMSihpPSSPOTDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSihpGLinkDlg)
	DDX_Control(pDX, IDC_CMD_SEL_LIST, m_listSel);
	DDX_Control(pDX, IDC_CMD_DATA_LIST, m_listData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSihpPSSPOTDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMSihpGLinkDlg)
	ON_BN_CLICKED(IDC_CMD_ADD, OnAddBtn)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnDeleteBtn)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_DATA_LIST, OnClickSelList)
	ON_BN_CLICKED(IDC_CMD_SEL_VIEW_BTN, OnSelBtn)
	ON_BN_CLICKED(IDC_CMD_UNSEL_BTN, OnUnselBtn)
	ON_NOTIFY(NM_CLICK, IDC_CMD_SEL_LIST, OnNMClickShelllist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSihpGLinkDlg message handlers

BOOL CCMSihpPSSPOTDlg::OnInitDialog()
{
	CCMChildBarBase::OnInitDialog();
	InitList();
	MakeListItem();

	return TRUE;	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSihpPSSPOTDlg::OnAddBtn()
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess = FALSE;
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	GetSelectedKey(aSelKey);
	if ( aSelKey.GetSize() == 0 )
	{
		GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_POINTSPRING_SUPPORT_4));  // No Point Spring Support is selected.
		return;
	}

	T_SIHP_D SihpD;
	bSuccess = pDoc->m_pDataCtrl->AddSihp(aSelKey, SihpD);

	if ( bSuccess ) pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMSihpPSSPOTDlg::OnDeleteBtn()
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess = FALSE;

	// 뷰에 선택된 것 기준으로 삭제
	CArray<T_NODE_K, T_NODE_K> aNodeK;
	pDoc->m_pViewCtrl->GetAllSelectedNode(aNodeK);

	std::set<T_NODE_K> sNodeK;
	for ( int i = 0; i < aNodeK.GetSize(); i++ )
		sNodeK.insert(aNodeK[i]);

	CArray<T_NODE_K, T_NODE_K> aSihpK;
	pDoc->m_pAttrCtrl->GetSihpKeyList(aSihpK);
	T_IEHG_K IehgK;
	for ( int i = aSihpK.GetSize() - 1; i >= 0; i-- )
	{
		pDoc->m_pAttrCtrl->MakeKeyIehg(D_IEHG_TYPE_POINTSPRING, aSihpK[i], IehgK);
		if ( !pDoc->m_pAttrCtrl->ExistIehg(IehgK) )
		{
			continue;
		}

		if ( sNodeK.find(aSihpK[i]) == sNodeK.end() )
			aSihpK.RemoveAt(i);
	}
	bSuccess = pDoc->m_pDataCtrl->DelSihp(aSihpK);

	if ( bSuccess ) pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMSihpPSSPOTDlg::OnClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CCMSihpPSSPOTDlg::GetElementByKind(CArray<UINT, UINT>& aKey, int nElemType)
{
	CQueryCtrl QueryCtrl;
	CString strElement;
	strElement.Format(_T("%ld"), nElemType);
	CArray<UINT, UINT> ElemKeys;
	CArray<UINT, UINT> SelKeys;
	SelKeys.Copy(aKey);
	aKey.RemoveAll();

	CQueryCondition QCond;          // 요소의 종류에 따른 쿼리 조건을 만든다.
	QCond.m_nCategory = CAT_ELEM_TYPE;
	QCond.m_nOperation = QC_OP_AND;
	QCond.m_aValue.Add(strElement);

	QueryCtrl.SelectKeyFromElem(ElemKeys, QCond);
	QueryCtrl.IntersectKey(ElemKeys, SelKeys, aKey);
}

void CCMSihpPSSPOTDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ( (hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd) ) return;

	switch ( lHint )
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

void CCMSihpPSSPOTDlg::UpdateBuffer()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if ( nCount == 0 ) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	BOOL bSihp = FALSE;
	while ( pos != NULL )
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch ( nCmd )
		{
		case(UR_SIHP_ADD): case(UR_SIHP_DEL):
			bSihp = TRUE;
			break;
		default:
			break;
		}
	} // end of while
	if ( bSihp )
		MakeListItem();
}

void CCMSihpPSSPOTDlg::InitList()
{
	m_listSel.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_listSel.InsertColumn(0, _LS(IDS_WG_CMD__ADDD__Name), LVCFMT_LEFT, 200);

	m_listData.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listData.InsertColumn(0, _LS(IDS_WG_CMD__ADDD__Name), LVCFMT_LEFT, 200);
}

void CCMSihpPSSPOTDlg::MakeListItem()
{
	MakeListItem_sel();
	MakeListItem_data();
}

void CCMSihpPSSPOTDlg::MakeListItem_sel()
{
	// 힌지 입력된 Point Spring Support 표시
	m_listSel.DeleteAllItems();

	CArray<T_KEY, T_KEY> aIehg_PS_List;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetIehgObjKeyList(D_IEHG_TYPE_POINTSPRING, aIehg_PS_List);

	CString str;
	int nIdx = 0;
	const auto nSize = aIehg_PS_List.GetSize();
	for ( int i = 0; i < nSize; ++i )
	{
		const auto nNodeK = aIehg_PS_List.GetAt(i);
		str.Format(_T("node:%d"), nNodeK);
		m_listSel.InsertItem(nIdx, str);
		m_listSel.SetItemData(nIdx, nNodeK);
		nIdx++;
	}
}

void CCMSihpPSSPOTDlg::MakeListItem_data()
{
	// 선택한 요소타입의 모든 요소 표시. 힌지설명 유무와 관계없이 선택 가능
	m_listData.DeleteAllItems();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_SIHP_K, T_SIHP_K> aSihpK;
	pDoc->m_pAttrCtrl->GetSihpKeyList(aSihpK);

	CString str;
	T_IEHG_K IehgK;
	T_IEHG_D IehgD;
	int nIdx = 0;
	for ( int i = 0; i < aSihpK.GetSize(); i++ )
	{
		if ( !pDoc->m_pAttrCtrl->GetIehgObj(D_IEHG_TYPE_POINTSPRING, aSihpK[i], IehgK, IehgD) )
		{
			ASSERT(0);
			continue;
		}

		str.Format(_T("node:%d"), aSihpK[i]);
		m_listData.InsertItem(nIdx, str);
		m_listData.SetItemData(nIdx, aSihpK[i]);
		nIdx++;
	}
}

void CCMSihpPSSPOTDlg::OnClickSelList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 리스트 선택된 요소를 뷰에서 선택
	const CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pViewCtrl->UnselectAll(NULL);

	CArray<T_NODE_K, T_NODE_K> aNodeK;
	for ( int i = 0; i < m_listData.GetItemCount(); i++ )
	{
		if ( m_listData.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED )
		{
			const auto nNodeK = m_listData.GetItemData(i);
			aNodeK.Add(nNodeK);
		}
	}
	CArrayUtil::RemoveDup(aNodeK);
	pDoc->m_pViewCtrl->SelectNode(NULL, aNodeK, TRUE, TRUE);

	*pResult = 0;
}

void CCMSihpPSSPOTDlg::OnSelBtn()
{
	CArray<T_NODE_K, T_NODE_K> aNodeK;
	const CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pViewCtrl->GetAllSelectedNode(aNodeK);

	std::set<T_NODE_K> sNodeK;
	for ( int i = 0; i < aNodeK.GetSize(); i++ )
		sNodeK.insert(aNodeK[i]);

	T_IEHG_K IehgK;
	std::set<T_NODE_K> sNodeK_Filtering;
	for ( T_NODE_K NodeK : sNodeK )
	{
		pDoc->m_pAttrCtrl->MakeKeyIehg(D_IEHG_TYPE_POINTSPRING, NodeK, IehgK);
		if ( !pDoc->m_pAttrCtrl->ExistIehg(IehgK) )
		{
			continue;
		}
		sNodeK_Filtering.insert(NodeK);
	}

	for ( int i = 0; i < m_listSel.GetItemCount(); i++ )
	{
		const BOOL bSel = sNodeK_Filtering.find(m_listSel.GetItemData(i)) != sNodeK_Filtering.end();
		m_listSel.SetCheck(i, bSel);
	}
}

void CCMSihpPSSPOTDlg::OnUnselBtn()
{
	for ( int i = 0; i < m_listSel.GetItemCount(); i++ )
	{
		m_listSel.SetCheck(i, FALSE);
	}
}

void CCMSihpPSSPOTDlg::GetSelectedKey(CArray<T_NODE_K, T_NODE_K>& aNodeK) const
{
	aNodeK.RemoveAll();
	for ( int i = 0; i < m_listSel.GetItemCount(); i++ )
	{
		if ( m_listSel.GetCheck(i) == FALSE ) continue;
		const auto nNodeK = m_listSel.GetItemData(i);
		aNodeK.Add(nNodeK);
	}
}

BOOL CCMSihpPSSPOTDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE && (m_listSel.m_hWnd == pMsg->hwnd) )
	{
		ReverseCheck();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CCMSihpPSSPOTDlg::OnNMClickShelllist(NMHDR* pNMHDR, LRESULT* pResult)
{
	int rc = 0;
	const NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// Checkbox HitTest
	LVHITTESTINFO oInfo;
	oInfo.pt = pNMListView->ptAction;
	const int nIdxClick = m_listSel.HitTest(&oInfo);

	/// Check Box Icon Click
	if ( oInfo.flags == LVHT_ONITEMSTATEICON )
	{
		// 현재 셀이 선택된 셀일 때 ReverseCheck()
		BOOL bIdxSelected = FALSE;
		POSITION pos = m_listSel.GetFirstSelectedItemPosition();
		for ( int i = 0; i < m_listSel.GetSelectedCount(); i++ )
		{
			if ( nIdxClick == m_listSel.GetNextSelectedItem(pos) )
			{
				bIdxSelected = TRUE;
				break;
			}
		}

		if ( bIdxSelected )
		{
			ReverseCheck();
			rc = 1;
		}
	}

	*pResult = rc;
}

// 선택된 아이템이 모두 체크돼있을 때 -> 전체선택해제
// 그 외의 경우 -> 전체선택
void CCMSihpPSSPOTDlg::ReverseCheck()
{
	BOOL bCheckedAll = TRUE;
	POSITION pos = m_listSel.GetFirstSelectedItemPosition();
	for ( int i = 0; i < m_listSel.GetSelectedCount(); i++ )
	{
		const int nIdx = m_listSel.GetNextSelectedItem(pos);
		if ( !m_listSel.GetCheck(nIdx) )
		{
			bCheckedAll = FALSE;
			break;
		}
	}

	pos = m_listSel.GetFirstSelectedItemPosition();
	for ( int i = 0; i < m_listSel.GetSelectedCount(); i++ )
	{
		const int nIdx = m_listSel.GetNextSelectedItem(pos);
		m_listSel.SetCheck(nIdx, !bCheckedAll);
	}

	//UpdateCtrls();
}