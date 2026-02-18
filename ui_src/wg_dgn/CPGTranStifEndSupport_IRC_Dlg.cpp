// CPGTranStifEndSupport_IRC_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGTranStifEndSupport_IRC_Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define _PROFILING
//#include "profilewrite.h"

/////////////////////////////////////////////////////////////////////////////
// CCPGTranStifEndSupport_IRC_Dlg dialog


CCPGTranStifEndSupport_IRC_Dlg::CCPGTranStifEndSupport_IRC_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCPGTranStifEndSupport_IRC_Dlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();	



	//{{AFX_DATA_INIT(CCPGTranStifEndSupport_IRC_Dlg)
	m_iEndpost = 0;
	//}}AFX_DATA_INIT
}

CCPGTranStifEndSupport_IRC_Dlg::~CCPGTranStifEndSupport_IRC_Dlg()
{
	
}

void CCPGTranStifEndSupport_IRC_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGTranStifEndSupport_IRC_Dlg)
	DDX_Control(pDX, IDC_DGN_CPG_STIF_SUPPORT_LIST, m_list);
	DDX_Control(pDX, IDC_DGN_CPG_STIF_SUPPORT_CHECK_LIST, m_checkListSelectSupport);
	DDX_Radio(pDX, IDC_DGN_CPG_STIF_SUPPORT_RIGID_RADIO, m_iEndpost);
// 	DDX_Control(pDX, IDC_DGN_CPG_STIF_SUPPORT_HT_EDIT, m_dHt);
// 	DDX_Control(pDX, IDC_DGN_CPG_STIF_SUPPORT_T_EDIT, m_dT);
// 	DDX_Control(pDX, IDC_DGN_CPG_STIF_SUPPORT_E_EDIT, m_dE);
// 	DDX_Control(pDX, IDC_DGN_CPG_STIF_SUPPORT_HT_UNIT, m_dHtUnit);
// 	DDX_Control(pDX, IDC_DGN_CPG_STIF_SUPPORT_T_UNIT, m_dTUnit);
// 	DDX_Control(pDX, IDC_DGN_CPG_STIF_SUPPORT_E_UNIT, m_dEUnit);
	//}}AFX_DATA_MAP
}

BOOL CCPGTranStifEndSupport_IRC_Dlg::OnInitDialog() 
{
	//PROFILING(_T("CCPGTranStifEndSupport_IRC_Dlg::OnInitDialog()"));

	CDialogMove::OnInitDialog();

	m_editSelectNode.Connect(SC_ID_NODE, &m_editSelectNode);

	InitSelectSupportList();

	SetListCtrlHeader();
	
	GetDBCgte();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPGTranStifEndSupport_IRC_Dlg::InitSelectSupportList() 
{
	//CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
	// Modify by sshan,안재오  CPG.('20070316)  모든 노드가 들어올수 있도록 변경
	//m_pDoc->m_pAttrCtrl->GetConsNodeKeyList(m_arNodeKeyList);	
	m_pDoc->m_pAttrCtrl->GetNodeKeyList(m_arNodeKeyList);	

	for(int i=0 ; i<m_arNodeKeyList.GetSize() ; i++)
	{
		CString strGrupName = _T("");
		strGrupName.Format(_T("%d"), m_arNodeKeyList[i]);
		
		int nIndex = m_checkListSelectSupport.AddString(strGrupName);
		m_checkListSelectSupport.SetItemData(nIndex, DWORD(m_arNodeKeyList[i]));
				
		m_checkListSelectSupport.SetCheck(nIndex, false);
	
		m_mapListKey.SetAt(m_arNodeKeyList[i], nIndex);
	}
}

void CCPGTranStifEndSupport_IRC_Dlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_CPG_NODE), _LS(IDS_DGN_CPG_TYPE)};  //_T("Node"), _T("Type")
	int nColWidth[] = {50, 120};
	int nAlign[] = {LVCFMT_LEFT, LVCFMT_LEFT};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_list.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_list.GetSafeHwnd(), dwStyle);

	for(int i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_list.InsertColumn(i,&lvcolumn);
		m_list.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

int CCPGTranStifEndSupport_IRC_Dlg::FindInsertionPos(T_CGTE_IRC_K nID)
{
	int nCount = m_list.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_CGTE_IRC_K key = m_list.GetItemData(i);
        if ( nID < key ) { return i; }
	}
	return nCount;
}

BOOL CCPGTranStifEndSupport_IRC_Dlg::InsertListItem(T_CGTE_IRC_K Key)
{
	//DB에서 Data를 가져온다

	T_CGTE_IRC_D Data;

	m_pDoc->m_pAttrCtrl->GetCgte_IRC(Key, Data);
	

	LVITEM lvitem;
	CString str;
	int nItem = m_list.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 2; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_list.InsertItem(&lvitem);
		}
		else m_list.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	m_list.SetItemData(nItem, (DWORD)Key);
	return TRUE;
}

CString CCPGTranStifEndSupport_IRC_Dlg::DataToStr(int i, T_CGTE_IRC_K Key, T_CGTE_IRC_D &Data)
{  
	CString str;

	if(i == 0)
	str.Format(_T("%d"), Key);
	else if(i == 1)
	{
		if(Data.iEndpost == 0)
			str = _LS(IDS_DGN_CPG_RIGID_END_POST);
		else if(Data.iEndpost == 1)
			str = _LS(IDS_DGN_CPG_NONRIGID_END_POST);
		else
			str = _LS(IDS_DGN_CPG_NO_END_POST);
	}
	else ASSERT(0);

	return str;
}

BOOL CCPGTranStifEndSupport_IRC_Dlg::DeleteListItem(T_CGTE_IRC_K key)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_list.FindItem(&FindInfo);
	if(nItem==-1) return TRUE;

	m_list.DeleteItem(nItem);

	return TRUE;
}

BOOL CCPGTranStifEndSupport_IRC_Dlg::ModifyListItem(T_CGTE_IRC_K key)
{
	T_CGTE_IRC_D Data;

	m_pDoc->m_pAttrCtrl->GetCgte_IRC(key, Data);

	LVFINDINFO FindInfo;
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_list.FindItem(&FindInfo);
	if(nItem == -1) return TRUE;

	for(int i=0; i<2; i++)
	{
		CString str = DataToStr(i, key, Data);
		m_list.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem    = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask     = LVIF_PARAM;
	lvitem.lParam   = (LPARAM)key;
	m_list.SetItem(&lvitem);

	return TRUE;
}

void CCPGTranStifEndSupport_IRC_Dlg::LoadDlgData()
{
	m_iEndpost = m_MainData.iEndpost;

// 	m_dHt.SetEditUnit(m_MainData.dht);
// 	m_dT.SetEditUnit(m_MainData.dt);
// 	m_dE.SetEditUnit(m_MainData.de);

	UpdateData(FALSE);
}

BOOL CCPGTranStifEndSupport_IRC_Dlg::SaveDlgFactorPosition()
{
	UpdateData(TRUE);

	m_MainData.iEndpost = m_iEndpost;

// 	m_MainData.dht = m_dHt.GetEditValue();
// 	m_MainData.dt = m_dT.GetEditValue();
// 	m_MainData.de = m_dE.GetEditValue();	

	return TRUE;
}

BOOL CCPGTranStifEndSupport_IRC_Dlg::SaveDlgSelectSupport()
{
	UpdateData(TRUE);

	DWORD nID;
	m_arSelectNodeKey.RemoveAll();

	int nCount = m_checkListSelectSupport.GetCount();
	
	for (int c = 0; c < nCount ; c++)
	{
		if(m_checkListSelectSupport.GetCheck(c) == 1)
		{
			nID = m_checkListSelectSupport.GetItemData(c);			
			m_arSelectNodeKey.Add(nID);
		}
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCPGTranStifEndSupport_IRC_Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CCPGTranStifEndSupport_IRC_Dlg)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_SUPPORT_ADD_BTN, OnDgnAddBtn)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_SUPPORT_MODIFY_BTN, OnDgnModifyBtn)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_SUPPORT_DELETE_BTN, OnDgnDeleteBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_CPG_STIF_SUPPORT_LIST, OnItemchangedDgnList)
	ON_LBN_SELCHANGE(IDC_DGN_CPG_STIF_SUPPORT_CHECK_LIST, OnItemchangedDgnCheckList)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_SUPPORT_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_SUPPORT_RIGID_RADIO, OnPostRadio)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_SUPPORT_NONRIGID_RADIO, OnPostRadio)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_SUPPORT_NOEND_RADIO, OnPostRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPGTranStifEndSupport_IRC_Dlg message handlers

void CCPGTranStifEndSupport_IRC_Dlg::OnDgnAddBtn() 
{
	// TODO: Add your control notification handler code here
	if(!SaveDlgSelectSupport())
		return;
	if(!SaveDlgFactorPosition())
		return;

	for(int i=0 ; i<m_arSelectNodeKey.GetSize() ; i++)
	{
		T_CGTE_IRC_K key = m_arSelectNodeKey.GetAt(i);
		
		if(!m_pDoc->m_pAttrCtrl->ExistCgte(key))
		{
			if(m_pDoc->m_pDataCtrl->AddCgte_IRC(key, m_MainData))
				InsertListItem(key);
		}		
	}	
}

void CCPGTranStifEndSupport_IRC_Dlg::OnDgnModifyBtn() 
{
	// TODO: Add your control notification handler code here
	if(!SaveDlgSelectSupport())
		return;
	if(!SaveDlgFactorPosition())
		return;
	T_CGTE_IRC_D Data;
	Data = m_MainData;

	for(int i=0 ; i<m_arSelectNodeKey.GetSize() ; i++)
	{
		T_CGTE_IRC_K key = m_arSelectNodeKey.GetAt(i);
		
		if(m_pDoc->m_pAttrCtrl->ExistCgte_IRC(key))
		{
			if(m_pDoc->m_pDataCtrl->ModifyCgte_IRC(key, key, Data))
				ModifyListItem(key);
		}		
	}	
}

void CCPGTranStifEndSupport_IRC_Dlg::OnDgnDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	if(!SaveDlgSelectSupport())
		return;

	int size = m_arSelectNodeKey.GetSize();
	
	m_pDoc->m_pDataCtrl->DelCgte_IRC(m_arSelectNodeKey);
	
	for(int i=0 ; i<size ; i++)
	{
		T_CGTE_IRC_K key = m_arSelectNodeKey[0];
		
		DeleteListItem(key);				
	}	
}

void CCPGTranStifEndSupport_IRC_Dlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
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

void CCPGTranStifEndSupport_IRC_Dlg::OnItemchangedDgnList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;

	OnItemchangedDgnCheckList();
}

void CCPGTranStifEndSupport_IRC_Dlg::OnItemchangedDgnCheckList() 
{
	if(!SaveDlgSelectSupport())
		return;	

	CString	strSelectNode;
	strSelectNode.Empty();
	CString TempString;

	for(int i=0 ; i<m_arSelectNodeKey.GetSize() ; i++)
	{
		if(strSelectNode.GetLength()==0)
				strSelectNode.Format(_T("%d"), m_arSelectNodeKey[i]);
		else
		{
			TempString.Format(_T(" %d"), m_arSelectNodeKey[i]);
			strSelectNode += TempString;
		}		
	}	

	if(strSelectNode.GetLength() > 0)
		m_editSelectNode.SelectByStr(strSelectNode);
}

void CCPGTranStifEndSupport_IRC_Dlg::ChangeItem(int nIndex)
{
	//PROFILING(_T("CCPGTranStifEndSupport_IRC_Dlg::ChangeItem(int nIndex)"));

	T_CGTE_IRC_K nodeKey;
	int listKey;

	POSITION Pos = m_mapListKey.GetStartPosition();
	while(Pos)
	{
		m_mapListKey.GetNextAssoc(Pos,nodeKey,listKey);
		m_checkListSelectSupport.SetCheck(listKey, false);
	}  

	CArray<int, int> arSelItem;
	arSelItem.RemoveAll();	
	GetSelectedItemList(&m_list, arSelItem);

	for(int i=0 ; i<arSelItem.GetSize() ; i++)
	{
		nodeKey = m_list.GetItemData(arSelItem[i]);
		m_mapListKey.Lookup(nodeKey, listKey);	
		m_checkListSelectSupport.SetCheck(listKey, true);
	}
	
	m_pDoc->m_pAttrCtrl->GetCgte_IRC(nodeKey, m_MainData);
	LoadDlgData();
	OnPostRadio();
}

void CCPGTranStifEndSupport_IRC_Dlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CCPGTranStifEndSupport_IRC_Dlg::GetDBCgte() 
{
	CArray<T_CGTE_IRC_K, T_CGTE_IRC_K> CgteKeyList;

	m_pDoc->m_pAttrCtrl->GetCgteKeyList_IRC(CgteKeyList);

	for(int i=0 ; i<CgteKeyList.GetSize() ; i++)
	{
		InsertListItem(CgteKeyList[i]);
	}	
}


void CCPGTranStifEndSupport_IRC_Dlg::OnPostRadio() 
{
	UpdateData(TRUE);

	if(m_iEndpost == 0)
	{
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_STATIC3)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_HT_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_T_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_E_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_HT_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_T_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_E_UNIT)->EnableWindow(true);
	}
	else if(m_iEndpost == 1)
	{
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_STATIC3)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_HT_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_T_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_E_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_HT_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_T_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_E_UNIT)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_STATIC3)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_HT_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_T_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_E_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_HT_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_T_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_SUPPORT_E_UNIT)->EnableWindow(false);
	}
}
