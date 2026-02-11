// HydrMaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "HydrMaterialDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHydrMaterialDlg dialog


CHydrMaterialDlg::CHydrMaterialDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CHydrMaterialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHydrMaterialDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CHydrMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHydrMaterialDlg)
	DDX_Control(pDX, IDC_TM_HMAT_TYPECOMBO, m_CobxMatType);
	DDX_Control(pDX, IDC_TM_HMAT_MATLIST, m_matlist);
	DDX_Control(pDX, IDC_TM_HMAT_RLIST, m_rlist);
	DDX_Control(pDX, IDC_TM_HMAT_LLIST, m_llist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHydrMaterialDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CHydrMaterialDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_HMAT_RMOVEBTN, OnTmHmatRmovebtn)
	ON_NOTIFY(NM_DBLCLK, IDC_TM_HMAT_LLIST, OnDblclkTmHmatLlist)
	ON_BN_CLICKED(IDC_TM_HMAT_LMOVEBTN, OnTmHmatLmovebtn)
	ON_NOTIFY(NM_DBLCLK, IDC_TM_HMAT_RLIST, OnDblclkTmHmatRlist)
	ON_BN_CLICKED(IDC_TM_HMAT_ADDBTN, OnTmHmatAddbtn)
	ON_BN_CLICKED(IDC_TM_HMAT_DELBTN, OnTmHmatDelbtn)
	ON_BN_CLICKED(IDC_TM_HMAT_TYPEBTN, OnTmHmatTypebtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHydrMaterialDlg message handlers

BOOL CHydrMaterialDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	// TODO: Add extra initialization here
	InitLRList();
	InitMatList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHydrMaterialDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CHydrMaterialDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	 	InitHmatData();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		InitHmatData();
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

void CHydrMaterialDlg::InitHmatData()
{
	// left list //////////////////////
	m_llist.DeleteAllItems();
	CArray<T_MATL_K, T_MATL_K> aMatlKeyList;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlKeyList);
	int nMatCount = aMatlKeyList.GetSize();
	T_MATL_D matData;
	CString str;

	for (int i=0; i < nMatCount; i++)
	{
		int ix = m_llist.InsertItem(i, _T(""));
		m_pDoc->m_pAttrCtrl->GetMatl(aMatlKeyList[i], matData);
		str.Format(_LS(IDS_WG_TREEMENU__d__s), aMatlKeyList[i], matData.Name);

		m_llist.SetItemText(ix, 0, str);
		m_llist.SetItemData(ix, aMatlKeyList[i]);
	}
	///////////////////////////////////


	// right list /////////////////////
	CArray<T_MATL_K, T_MATL_K> aRightKeyList;
	for(int i=0; i < m_rlist.GetItemCount(); i++)
	{
		int key = m_rlist.GetItemData(i);
		if(m_pDoc->m_pAttrCtrl->ExistMatl(key)) aRightKeyList.Add(key);
	}
	m_rlist.DeleteAllItems();
	int nRCount = aRightKeyList.GetSize();
	for(int i=0; i < nRCount; i++)
	{
		int ix = m_rlist.InsertItem(i, _T(""));
		m_pDoc->m_pAttrCtrl->GetMatl(aRightKeyList[i], matData);
		str.Format(_LS(IDS_WG_TREEMENU__d__s), aRightKeyList[i], matData.Name);

		m_rlist.SetItemText(ix, 0, str);
		m_rlist.SetItemData(ix, aRightKeyList[i]);
	}
	///////////////////////////////////


	// mat list ///////////////////////
	MakeItemExMatlist();
	///////////////////////////////////
}







// Left List, Right List
void CHydrMaterialDlg::InitLRList()
{
	CRect listRect;
	m_llist.GetClientRect(&listRect);
	int nListWidth = (listRect.right - listRect.left);
	m_llist.InsertColumn(0, _T(""), LVCFMT_LEFT, nListWidth);
	m_rlist.InsertColumn(0, _T(""), LVCFMT_LEFT, nListWidth);

	CArray<T_MATL_K, T_MATL_K> aMatlKeyList;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlKeyList);
	int nMatCount = aMatlKeyList.GetSize();
	T_MATL_D matData;
	CString str;

	for (int i=0; i<nMatCount; i++)
	{
		int ix = m_llist.InsertItem(i, _T(""));
		m_pDoc->m_pAttrCtrl->GetMatl(aMatlKeyList[i], matData);
		str.Format(_LS(IDS_WG_TREEMENU__d__s), aMatlKeyList[i], matData.Name);

		m_llist.SetItemText(ix, 0, str);
		m_llist.SetItemData(ix, aMatlKeyList[i]);
	}
}

void CHydrMaterialDlg::OnTmHmatRmovebtn() 
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	int curpos;
	bool bEqual = false;

	int nNum = m_llist.GetSelectedCount();
	pos = m_llist.GetFirstSelectedItemPosition();
	for(int i=0; i < nNum; i++)
	{
		curpos = m_llist.GetNextSelectedItem(pos);
		CString str = m_llist.GetItemText(curpos, 0);
		int key = m_llist.GetItemData(curpos);
		int j = 0;
		for (j = 0; j < m_rlist.GetItemCount(); j++)
		{
			int k = m_rlist.GetItemData(j);
			if (k == key) { bEqual = true; break; }
			if (k > key) break;
		}
		if (bEqual) { bEqual = false; continue; }
		int ix = m_rlist.InsertItem(j, str);
		m_rlist.SetItemData(ix, key);
	}
}

void CHydrMaterialDlg::OnDblclkTmHmatLlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnTmHmatRmovebtn();
	*pResult = 0;
}

void CHydrMaterialDlg::OnTmHmatLmovebtn() 
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	int curpos;

	int nNum = m_rlist.GetSelectedCount();
	pos = m_rlist.GetFirstSelectedItemPosition();
	for(int i=0; i < nNum; i++)
	{
		curpos = m_rlist.GetNextSelectedItem(pos);
		m_rlist.DeleteItem(curpos);
	}
}

void CHydrMaterialDlg::OnDblclkTmHmatRlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnTmHmatLmovebtn();
	*pResult = 0;
}








// Mat List
void CHydrMaterialDlg::InitMatList()
{
	SetHeaderTitleMatlist();
	MakeItemExMatlist();
}

void CHydrMaterialDlg::SetHeaderTitleMatlist()
{
	CString aTitle[] = {_LS(IDS_WG_TREEMENU_No), _LS(IDS_WG_TREEMENU_Mat_Name), _LS(IDS_WG_TREEMENU_Hd_Mat_Name)};
	int nColWidth[3];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_matlist.GetSafeHwnd());
	dwStyle = dwStyle | LVS_EX_FULLROWSELECT; //| LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(m_matlist.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 29;
	nColWidth[1] = 77;
	nColWidth[2] = 90;

	// Set Title
	for(i = 0; i < 3; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_matlist.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CHydrMaterialDlg::MakeItemExMatlist()
{
	CWaitCursor Cursor;

	m_matlist.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountHmat();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_HMAT_K Key;
		T_HMAT_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartHmat();
		while(pos != NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextHmat(pos, Key, Data);
			if(!m_pDoc->m_pAttrCtrl->ExistMatl(Key)) continue;
		  KeyBuf[nCount++] = Key;
		}

		for(nCount=0 ; nCount < nItemCount ; nCount++)
	  {
		  Key = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetHmat(Key, Data);
			InsertItemMatlist(Key, Data);
		}
		delete []KeyBuf;
	}
}

BOOL CHydrMaterialDlg::InsertItemMatlist(T_HMAT_K Key, T_HMAT_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_matlist.GetItemCount();

	lvitem.iItem = FindInsertionPosMatlist(Key);
	for(int i=0; i < 3; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStrMatlist(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_matlist.InsertItem(&lvitem);
		}
		else m_matlist.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	m_matlist.SetItemData(lvitem.iItem, Key);

	return TRUE;
}

int CHydrMaterialDlg::FindInsertionPosMatlist(T_HMAT_K nID)
{
	T_HMAT_K key;

	int nCount = m_matlist.GetItemCount();
	int i = 0;
	for (i=0; i < nCount; i++)
	{
		key = m_matlist.GetItemData(i);
		if (nID <= key) break;
	}

	if (nID == key) m_matlist.DeleteItem(i);
	return i;
}

CString CHydrMaterialDlg::DataToStrMatlist(int i, T_HMAT_K Key, T_HMAT_D &Data)
{
	UpdateData();
	CString str;

	if (i == 0) str.Format(_LS(IDS_WG_TREEMENU__d), Key);
	else if(i == 1)
	{
		T_MATL_D matdata;
		m_pDoc->m_pAttrCtrl->GetMatl(Key, matdata);
		str.Format(_LS(IDS_WG_TREEMENU__s), 	matdata.Name);
	}
	else if(i == 2)
	{
		T_HMTP_D hmtpdata;
		m_pDoc->m_pAttrCtrl->GetHmtp(Data.HyMatlTypeKey, hmtpdata);
		str.Format(_LS(IDS_WG_TREEMENU__s), 	hmtpdata.strMatlName);
//		m_CobxMatType.GetWindowText(str);
	}

	return str;
}

void CHydrMaterialDlg::OnTmHmatAddbtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString str;
	m_CobxMatType.GetWindowText(str);
	if(str.CompareNoCase(_LS(IDS_WG_TREEMENU_NONE)) == 0) return;

	T_MATL_K key;
	T_HMTP_K hmtpkey;
	T_HMAT_D data;

	int nNum = m_rlist.GetItemCount();
	for(int i=0; i < nNum; i++)
	{
		key = m_rlist.GetItemData(i);
		hmtpkey = m_pDoc->m_pAttrCtrl->GetHmtpKey(str);
		data.HyMatlTypeKey = hmtpkey;
		m_pDoc->m_pDataCtrl->AddHmat(key, data);
	}
	MakeItemExMatlist();
	m_rlist.DeleteAllItems();
}

void CHydrMaterialDlg::OnTmHmatDelbtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_matlist.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_No_selected_Hydration_Material_type));
		return;
	}

	T_HMAT_K key;
	key = m_matlist.GetItemData(iItem);
	if(!m_pDoc->m_pDataCtrl->DelHmat(key)) return;
	MakeItemExMatlist();

	int nCount = m_matlist.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_matlist.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CHydrMaterialDlg::OnTmHmatTypebtn() 
{
	// TODO: Add your control notification handler code here
	//AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HYDRATION_MATERIAL_TYPE,0));	
}
