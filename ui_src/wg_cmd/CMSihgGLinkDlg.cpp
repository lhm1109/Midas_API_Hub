// CMSihgGLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSihgGLinkDlg.h"

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
// CCMSihgGLinkDlg dialog

//IMPLEMENT_DYNCREATE(CCMSihgGLinkDlg, CCMChildBarBase)
enum LIST_IDX { IDX_NAME };

CCMSihgGLinkDlg::CCMSihgGLinkDlg()
	: CCMChildBarBase(CCMSihgGLinkDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSihgGLinkDlg)
	//}}AFX_DATA_INIT
}


void CCMSihgGLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSihgGLinkDlg)
	DDX_Control(pDX, IDC_CMD_SEL_LIST, m_listSel);
	DDX_Control(pDX, IDC_CMD_DATA_LIST, m_listData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSihgGLinkDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMSihgGLinkDlg)
	ON_BN_CLICKED(IDC_CMD_ADD, OnAddBtn)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnDeleteBtn)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_DATA_LIST, OnClickSelList)
	ON_BN_CLICKED(IDC_CMD_SEL_VIEW_BTN	, OnSelBtn)
	ON_BN_CLICKED(IDC_CMD_UNSEL_BTN		, OnUnselBtn)
	ON_NOTIFY(NM_CLICK, IDC_CMD_SEL_LIST, OnNMClickShelllist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSihgGLinkDlg message handlers

BOOL CCMSihgGLinkDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();	 
	InitList();
	MakeListItem();

	return TRUE;	// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSihgGLinkDlg::OnAddBtn() 
{
	UpdateData(TRUE);
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess=FALSE;
	CArray<T_NLNK_K, T_NLNK_K> aSelKey;
	GetSelectedKey(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_4));  // No General Link is selected
		return;
	}

	T_SIHG_D SihgD;
	bSuccess = pDoc->m_pDataCtrl->AddSihg(aSelKey, SihgD);		

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMSihgGLinkDlg::OnDeleteBtn() 
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess=FALSE;
		
	// 뷰에 선택된 것 기준으로 삭제
	CArray<T_NODE_K,T_NODE_K> aNodeK;
	pDoc->m_pViewCtrl->GetAllSelectedNode(aNodeK);

	std::set<T_NODE_K> sNodeK;
	for(int i=0; i<aNodeK.GetSize(); i++)
		sNodeK.insert(aNodeK[i]);

	// General Link 절점 2개가 모두 선택된 것만 필터링
	CArray<T_SIHG_K,T_SIHG_K> aSihgK;
	pDoc->m_pAttrCtrl->GetSihgKeyList(aSihgK);
	T_NLNK_D NlnkD;
	for(int i=aSihgK.GetSize()-1; i>=0; i--)
	{
		pDoc->m_pAttrCtrl->GetNlnk(aSihgK[i], NlnkD);

		if(sNodeK.find(NlnkD.Node1)==sNodeK.end() || sNodeK.find(NlnkD.Node2)==sNodeK.end())
			aSihgK.RemoveAt(i);
	}
	bSuccess = pDoc->m_pDataCtrl->DelSihg(aSihgK);	

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMSihgGLinkDlg::OnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);		
}

void CCMSihgGLinkDlg::GetElementByKind(CArray<UINT,UINT>& aKey, int nElemType)
{
	CQueryCtrl QueryCtrl;
	CString strElement;
	strElement.Format(_T("%ld"),nElemType);
	CArray<UINT,UINT> ElemKeys;
	CArray<UINT,UINT> SelKeys;
	SelKeys.Copy(aKey);
	aKey.RemoveAll();

	CQueryCondition QCond;          // 요소의 종류에 따른 쿼리 조건을 만든다.
	QCond.m_nCategory = CAT_ELEM_TYPE;
	QCond.m_nOperation = QC_OP_AND;
	QCond.m_aValue.Add(strElement);

	QueryCtrl.SelectKeyFromElem(ElemKeys,QCond);
	QueryCtrl.IntersectKey(ElemKeys,SelKeys,aKey);
}	

void CCMSihgGLinkDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		break;
	}
}

void CCMSihgGLinkDlg::UpdateBuffer()
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	BOOL bSihg = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SIHG_ADD): case(UR_SIHG_DEL):
			bSihg = TRUE;
			break;
		default:
			break;
		}
	} // end of while
	if(bSihg)
		MakeListItem();
}

void CCMSihgGLinkDlg::InitList()
{
	m_listSel.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_listSel.InsertColumn(0, _LS(IDS_WG_CMD__ADDD__Name), LVCFMT_LEFT, 200);

	m_listData.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listData.InsertColumn(0, _LS(IDS_WG_CMD__ADDD__Name), LVCFMT_LEFT, 200);
}

void CCMSihgGLinkDlg::MakeListItem()
{
	MakeListItem_sel();
	MakeListItem_data();
}

void CCMSihgGLinkDlg::MakeListItem_sel()
{
	// 힌지 입력된 G-Link 표시
	m_listSel.DeleteAllItems();

	CArray<T_NLNK_K,T_NLNK_K> aNlnkK;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetNlnkKeyList(aNlnkK);
	
	CString str;
	T_NLNK_D NlnkD;
	int nIdx=0;
	for(int i=aNlnkK.GetSize()-1; i>=0; i--)
	{
		if(!pDoc->m_pAttrCtrl->GetNlnk(aNlnkK[i], NlnkD))
		{
			ASSERT(0);
			continue;
		}

		if(NlnkD.IehpKey==0)
			continue;

		str.Format(_T("%d (n1:%d, n2:%d)"), aNlnkK[i], NlnkD.Node1, NlnkD.Node2);
		m_listSel.InsertItem(nIdx, str);
		m_listSel.SetItemData(nIdx, aNlnkK[i]);
		nIdx++;
	}
}

void CCMSihgGLinkDlg::MakeListItem_data()
{
	// 선택한 요소타입의 모든 요소 표시. 힌지설명 유무와 관계없이 선택 가능
	m_listData.DeleteAllItems();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CArray<T_SIHG_K,T_SIHG_K> aSihgK;
	pDoc->m_pAttrCtrl->GetSihgKeyList(aSihgK);

	CString str;
	T_NLNK_D NlnkD;
	int nIdx=0;
	for(int i=0; i<aSihgK.GetSize(); i++)
	{
		if(!pDoc->m_pAttrCtrl->GetNlnk(aSihgK[i], NlnkD))
		{
			ASSERT(0);
			continue;
		}

		str.Format(_T("%d (n1:%d, n2:%d)"), aSihgK[i], NlnkD.Node1, NlnkD.Node2);
		m_listData.InsertItem(nIdx, str);
		m_listData.SetItemData(nIdx, aSihgK[i]);
		nIdx++;
	}
}

void CCMSihgGLinkDlg::OnClickSelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// 리스트 선택된 요소를 뷰에서 선택
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pViewCtrl->UnselectAll(NULL);

	T_NLNK_D NlnkD;
	CArray<T_NODE_K,T_NODE_K> aNodeK;
	for(int i=0; i<m_listData.GetItemCount(); i++)
	{
		if (m_listData.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
		{
			T_NLNK_K NlnkK = m_listData.GetItemData(i);
			pDoc->m_pAttrCtrl->GetNlnk(NlnkK, NlnkD);
			aNodeK.Add(NlnkD.Node1);
			aNodeK.Add(NlnkD.Node2);
		}
	}
	CArrayUtil::RemoveDup(aNodeK);
	pDoc->m_pViewCtrl->SelectNode(NULL, aNodeK, TRUE, TRUE);

	*pResult = 0;
}

void CCMSihgGLinkDlg::OnSelBtn()
{
	CArray<T_NODE_K,T_NODE_K> aNodeK;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pViewCtrl->GetAllSelectedNode(aNodeK);

	std::set<T_NODE_K> sNodeK;
	for(int i=0; i<aNodeK.GetSize(); i++)
		sNodeK.insert(aNodeK[i]);

	// General Link 절점 2개가 모두 선택된 것만 필터링
	CArray<T_NLNK_K,T_NLNK_K> aNlnkK;
	pDoc->m_pAttrCtrl->GetNlnkKeyList(aNlnkK);
	std::set<T_NLNK_K> sNlnkK;
	T_NLNK_D NlnkD;
	for(int i=aNlnkK.GetSize()-1; i>=0; i--)
	{
		pDoc->m_pAttrCtrl->GetNlnk(aNlnkK[i], NlnkD);
		if(sNodeK.find(NlnkD.Node1)==sNodeK.end() || sNodeK.find(NlnkD.Node2)==sNodeK.end())
			continue;

		sNlnkK.insert(aNlnkK[i]);
	}

	// 얘네들을 리스트에서 선택하자...
	for(int i=0; i<m_listSel.GetItemCount(); i++)
	{
		BOOL bSel = sNlnkK.find(m_listSel.GetItemData(i)) != sNlnkK.end();
		m_listSel.SetCheck(i, bSel);
	}
}

void CCMSihgGLinkDlg::OnUnselBtn()
{
	for(int i=0; i<m_listSel.GetItemCount(); i++)
	{
		m_listSel.SetCheck(i, FALSE);
	}
}

void CCMSihgGLinkDlg::GetSelectedKey(CArray<T_NLNK_K,T_NLNK_K>& aNlnkK)
{
	aNlnkK.RemoveAll();
	for(int i=0; i<m_listSel.GetItemCount(); i++)
	{
		if(m_listSel.GetCheck(i)==FALSE) continue;
		CString strText = m_listSel.GetItemText(i,0);
		aNlnkK.Add(_ttoi(strText));
	}
}

BOOL CCMSihgGLinkDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE && (m_listSel.m_hWnd==pMsg->hwnd) ) 
	{
		ReverseCheck();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CCMSihgGLinkDlg::OnNMClickShelllist(NMHDR *pNMHDR, LRESULT *pResult)
{    
	int rc = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// Checkbox HitTest
	LVHITTESTINFO oInfo ;
	oInfo.pt = pNMListView->ptAction ;
	int nIdxClick = m_listSel.HitTest(&oInfo) ;

	/// Check Box Icon Click
	if( oInfo.flags == LVHT_ONITEMSTATEICON ) 
	{
		// 현재 셀이 선택된 셀일 때 ReverseCheck()
		BOOL bIdxSelected=FALSE;
		POSITION pos = m_listSel.GetFirstSelectedItemPosition();
		for(int i=0; i<m_listSel.GetSelectedCount(); i++) 
		{
			if(nIdxClick==m_listSel.GetNextSelectedItem(pos))
			{
				bIdxSelected=TRUE;
				break;
			}
		}

		if(bIdxSelected)
		{
			ReverseCheck();
			rc = 1;
		}
	}

	*pResult = rc;
}

// 선택된 아이템이 모두 체크돼있을 때 -> 전체선택해제
// 그 외의 경우 -> 전체선택
void CCMSihgGLinkDlg::ReverseCheck()
{
	BOOL bCheckedAll=TRUE;
	POSITION pos = m_listSel.GetFirstSelectedItemPosition();
	for(int i=0; i<m_listSel.GetSelectedCount(); i++) 
	{
		int nIdx= m_listSel.GetNextSelectedItem(pos);
		BOOL bCheck = m_listSel.GetCheck(nIdx);
		if(!bCheck)
		{
			bCheckedAll=FALSE;
			break;
		}
	}

	pos = m_listSel.GetFirstSelectedItemPosition();
	for(int i=0; i<m_listSel.GetSelectedCount(); i++) 
	{
		int nIdx= m_listSel.GetNextSelectedItem(pos);
		m_listSel.SetCheck(nIdx, !bCheckedAll);
	}

	//UpdateCtrls();
}