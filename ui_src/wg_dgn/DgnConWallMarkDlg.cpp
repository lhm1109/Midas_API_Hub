// DgnConWallMarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConWallMarkDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
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
// CDgnConWallMarkDlg dialog


CDgnConWallMarkDlg::CDgnConWallMarkDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConWallMarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConWallMarkDlg)
	m_strMarkName = _T("");
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConWallMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConWallMarkDlg)
	DDX_Control(pDX, IDC_DGN_WALLMARK_LIST, m_MarkList);
	DDX_Control(pDX, IDC_DGN_WALL_IDLIST, m_IdList);
	DDX_Text(pDX, IDC_DGN_WALL_MARKNAME, m_strMarkName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConWallMarkDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConWallMarkDlg)
	ON_BN_CLICKED(IDC_DGN_WALLMARK_ADD, OnDgnWallmarkAdd)
	ON_BN_CLICKED(IDC_DGN_WALLMARK_DELETE, OnDgnWallmarkDelete)
	ON_BN_CLICKED(IDC_DGN_WALLMARK_MODIFY, OnDgnWallmarkModify)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_WALLMARK_LIST, OnItemchangedDgnWallmarkList)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,         OnNodeListEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,        OnNodeListEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallMarkDlg message handlers

BOOL CDgnConWallMarkDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	// Unselected elements.
	if(m_pDoc==NULL)	return TRUE;
//**/	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	Initial_Data();
	Initial_MouseEdit();
	Initial_ListCtrlBox();
	if(m_pDoc->m_pAttrCtrl->GetCountWmak() > 0)	Initial_ExistListData();

	return TRUE;
}

void CDgnConWallMarkDlg::Initial_ExistListData()
{
	CArray<CString,CString&> strWmakList;
	m_pDoc->m_pAttrCtrl->GetWmakList(strWmakList);
	int iCount = strWmakList.GetSize();
	for(int i=0; i<iCount; i++)
	{
		CString strName = strWmakList.GetAt(i);
		T_WMAK_D mData;
		mData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetWmak(strName,mData))	continue;

		CString strIDs=_T("");
		int idCount = mData.arUserWallId.GetSize();
		for(int j=0; j<idCount; j++)
		{
			int nID = mData.arUserWallId.GetAt(j);
			CString temp;
			if(j+1 == idCount)  temp.Format(_T("%d"),nID);
			else                temp.Format(_T("%d,"),nID);
			strIDs = strIDs+temp;
		}
		int ListLineNo = m_MarkList.GetItemCount();
		Write_ListCtrlBox(ListLineNo,strName,strIDs);
	}
}

void CDgnConWallMarkDlg::Initial_ListCtrlBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_MarkList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_MarkList.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[2]	= {(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_WALL_ID)};
	int width[2]	= {60,120};
	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_LEFT;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_MarkList.InsertColumn(i,&lvcolumn);
	}
}

void CDgnConWallMarkDlg::Write_ListCtrlBox(int Index,CString strName,CString strIDs)
{
	LV_ITEM lvitem;

	// Input data at ListCtrl.
	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 0;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strName;
	m_MarkList.InsertItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 1;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strIDs;
	m_MarkList.SetItem(&lvitem);
}

void CDgnConWallMarkDlg::Initial_MouseEdit()
{
//	m_IdList.SetAttElemList();
	m_IdList.SetAttNothing();
	m_IdList.SetLButtonDownNotifyWindow(this);
	m_IdList.SetEnterNotifyWindow(this);
	m_IdList.SetEscNotifyWindow(this);
	m_IdList.SetModeToUse(MOUSEEDIT_USE_SET_WALL_ID);
}

void CDgnConWallMarkDlg::Initial_Data()
{
	m_strIDList   = _T("");
	m_strMarkName = _T("");
	if(m_nIDList.GetSize() > 0)	m_nIDList.RemoveAll();

	UpdateData(FALSE);
}

void CDgnConWallMarkDlg::OnDgnWallmarkAdd() 
{
	UpdateData(TRUE);
	if(ErrorCheck())
	{
		T_WMAK_D wmData;
		wmData.Initialize();
		wmData.MarkName = m_strMarkName;
		m_IdList.GetWindowText(m_strIDList);
		CStrParser::GetNodeList(m_strIDList,wmData.arUserWallId);
		BOOL bCheck = m_pDoc->m_pDataCtrl->AddWmak(wmData);
		if(bCheck)
		{
			m_IdList.ClearIDList();
			m_strIDList = _T("");
		}
	}
}

void CDgnConWallMarkDlg::OnDgnWallmarkDelete() 
{
	int nItem = m_MarkList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_WALL_MARK));
		return;
	}

	CString strName = m_MarkList.GetItemText(nItem,0);
	BOOL bCheck = m_pDoc->m_pDataCtrl->DelWmak(strName);
	if(bCheck)
	{
		//Modified By RSH. 2005.04.27
		//이미 Update를 통해 Initial_ExistListData()함수가 call된 뒤이므로 이미 list에는 반영되어 있음.
		//m_MarkList.DeleteItem(nItem);
		SetDlgItemText(IDC_DGN_WALL_MARKNAME,_T(""));
		SetDlgItemText(IDC_DGN_WALL_IDLIST,_T(""));
	}
}

void CDgnConWallMarkDlg::OnDgnWallmarkModify() 
{
	int nItem = m_MarkList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_WALL_MARK));
		return;
	}
	// Get saved data at now.
	GetDlgItemText(IDC_DGN_WALL_MARKNAME,m_strMarkName);
	GetDlgItemText(IDC_DGN_WALL_IDLIST,m_strIDList);
	CString strOldName = m_MarkList.GetItemText(nItem,0);
	// Get previous wall mark.
	T_WMAK_D wData;
	wData.Initialize();
	BOOL bCheck = m_pDoc->m_pAttrCtrl->GetWmak(strOldName,wData);
	if(bCheck && ErrorCheck())
	{
		T_WMAK_D wmData;
		wmData.Initialize();
		wmData.MarkName = m_strMarkName;
		wmData.MarkId   = wData.MarkId;
		CStrParser::GetNodeList(m_strIDList,wmData.arUserWallId);

		bCheck = m_pDoc->m_pDataCtrl->ModifyWmak(strOldName,wmData);
		if(bCheck) 
		{
			m_MarkList.SetItemText(nItem,0,m_strMarkName);
			m_MarkList.SetItemText(nItem,1,m_strIDList);

			SetDlgItemText(IDC_DGN_WALL_MARKNAME,m_strMarkName);
			SetDlgItemText(IDC_DGN_WALL_IDLIST,m_strIDList);
			m_IdList.ClearIDList();
			m_strIDList = _T("");
		}
	}
}

void CDgnConWallMarkDlg::OnDgnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

BOOL CDgnConWallMarkDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	if(m_strMarkName==_T("")) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_NG_WALL_MARK_NAME);}
	m_IdList.GetWindowText(m_strIDList);
	if(m_strIDList==_T(""))		{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_NG_WALL_ID_LIST);}

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);
	return bCheck;
}

LRESULT CDgnConWallMarkDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(m_IdList.GetSafeHwnd()==(HWND)lParam)
	{
//		CString str;
//		m_IdList.GetWindowText(str);
//		int LastElemNo = Cutting_LastElemNo(str);
//		unsigned int nID = m_pDoc->m_pPostCtrl->GetUserWallId(LastElemNo);
//
//		CString strIDs=_T("");
//		if(nID != 0)
//		{
//			if(!Check_EQWallID(nID))    //  있는 ID면 참, 없으면 거짓
//			{
//				strIDs.Format(_T("%d"),nID);
//				if(m_strIDList == _T("")) m_strIDList = m_strIDList+strIDs;
//				else                  m_strIDList = m_strIDList+_T(",")+strIDs;
//			}
//		}
//		m_IdList.SetWindowText(m_strIDList);
	}
	return 0L;
}

LRESULT CDgnConWallMarkDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_IdList.GetSafeHwnd()==(HWND)lParam)
	{
//		CString str;
//		m_MousePos.GetWindowText(str);     
//		CArray<unsigned int, unsigned int> m_iaJointList;
//		if(CStrParser::GetNodeList(str,m_iaJointList))
//		{
//			GSaveHistoryFormatNF(_T("Node Key iCount: %d"),m_iaJointList.GetSize());
//		}
//		else
//		{
//			GSaveHistoryNF(_T("Node List Error !!!"));
//		}
	}
	return 0L;
}

LRESULT CDgnConWallMarkDlg::OnNodeListEscape(WPARAM wParam, LPARAM lParam)
{
	if(m_IdList.GetSafeHwnd()==(HWND)lParam)
	{
//		GSaveHistoryNF(_T("Escape !!!"));
//		m_MousePos.SetWindowText(_T(""));
	}
	return 0L;
}

void CDgnConWallMarkDlg::OnItemchangedDgnWallmarkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int ItemNo = pNMListView->iItem;
	Select_ListCtrlItem(ItemNo);

	*pResult = 0;
}

void CDgnConWallMarkDlg::Select_ListCtrlItem(int nItem)
{
	if(nItem != -1)
	{
		CString strName = m_MarkList.GetItemText(nItem,0);
		CString strIDs  = m_MarkList.GetItemText(nItem,1);

		SetDlgItemText(IDC_DGN_WALL_MARKNAME,strName);
		SetDlgItemText(IDC_DGN_WALL_IDLIST,strIDs);
	}
}

void CDgnConWallMarkDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd())==0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_UNIT:
			// do something...
			break;
		case D_UPDATE_SEL_ADD:
			// do something...
			break;
		case D_UPDATE_SEL_DEL:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		default:
			//ASSERT(FALSE);
			break;
	}
}

void CDgnConWallMarkDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount==0)	return; 
	ASSERT(nCount==1);

	BOOL bUR_WMAK = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
			case(UR_WMAK_ADD):
			case(UR_WMAK_MFS):
			case(UR_WMAK_DEL):	{bUR_WMAK = TRUE;	break;}
			default:	break;
		}
	}
	if(bUR_WMAK)
	{
		m_MarkList.DeleteAllItems();
		Initial_ExistListData();
	}
}
