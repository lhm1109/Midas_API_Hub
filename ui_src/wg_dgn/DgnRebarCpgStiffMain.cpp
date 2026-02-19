// DgnRebarCpgStiffMain.cpp : implementation file
//
// Add by sshan. ('20061227)
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRebarCpgStiffMain.h"

#include "DgnRebarCpgStiff.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "DgnRebarCpgCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiffMain dialog

CDgnRebarCpgStiffMain::CDgnRebarCpgStiffMain(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnRebarCpgStiffMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRebarCpgStiffMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_Key = 0;
	m_Data.Initialize();

	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_pStiffDlg = 0;
}

/*
void CDgnRebarCpgStiffMain::SetParamData(T_CLRS_K ClrsK)
{
	m_Key = ClrsK;
	m_bModify = TRUE;
}*/

void CDgnRebarCpgStiffMain::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRebarCpgStiffMain)
	DDX_Control(pDX, IDC_DGN_DELETE, m_btnDel);
	DDX_Control(pDX, IDOK, m_btnAdd);
	DDX_Control(pDX, IDC_DGN_SECT_LIST, m_wndList);
	DDX_Control(pDX, IDC_DGN_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRebarCpgStiffMain, CChildDialog)
	//{{AFX_MSG_MAP(CDgnRebarCpgStiffMain)
	ON_BN_CLICKED(IDC_DGN_DELETE, OnDgnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SECT_LIST, OnItemchangedDgnSectList)
	ON_NOTIFY(NM_CLICK, IDC_DGN_SECT_LIST, OnClickDgnSectList)
	ON_BN_CLICKED(IDC_DGN_COPY_BTN, OnClickCopyBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiffMain message handlers

BOOL CDgnRebarCpgStiffMain::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIgnoreBottomBtnArea();
	
	// preview control
	CWnd* pWnd = GetDlgItem(IDC_DGN_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	this->LayoutChildDlg();

	SetHeaderTitle();
	MakeItemEx();

	m_btnAdd.EnableWindow(FALSE);
	m_btnDel.EnableWindow(FALSE);

	if (m_bModify)
	{
		int nItemCount = m_wndList.GetItemCount();
        int ix = 0;
		for (ix = 0; ix < nItemCount; ix++)
			if (m_Key == m_wndList.GetItemData(ix)) break;
		if (ix != nItemCount)
		{
			m_wndList.SetItemState(ix, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			m_btnAdd.EnableWindow(TRUE);
			m_btnDel.EnableWindow(TRUE);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// add/replace
void CDgnRebarCpgStiffMain::OnOK() 
{
	// TODO: Add extra validation here
	
	// get data
	if (!m_pStiffDlg->GetCurData(&m_Data))
	{
		AfxMessageBox(_LS(IDS_DGN_CPG_ERR_STIFF_REBAR));
		return;
	}
	
	CArray<T_CLRS_K, T_CLRS_K>  aKey;
	CArray<T_CLRS_D, T_CLRS_D&> aData;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	aKey.SetSize(nSelCount);
	aData.SetSize(nSelCount);
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aKey[i] = m_wndList.GetItemData(nItem);
		aData[i] = m_Data;
	}
	if (!m_pDoc->m_pDataCtrl->AddClrs(aKey, aData))
		return;

	MakeItemEx();

	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);

	//CChildDialog::OnOK();
}

// close
void CDgnRebarCpgStiffMain::OnCancel() 
{
	// TODO: Add extra cleanup here
	((CDialog*)m_pParent)->EndDialog(IDCANCEL); // need change! added by mylee
	//CChildDialog::OnCancel();
}

// delete
void CDgnRebarCpgStiffMain::OnDgnDelete() 
{
	//Design에서는 Delete기능 막음
	// TODO: Add your control notification handler code here
	
	T_CLRS_K key;
	CArray<T_CLRS_K, T_CLRS_K> aKey;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		key = m_wndList.GetItemData(nItem);
		if (m_pDoc->m_pAttrCtrl->ExistClrs(key)) aKey.Add(key);
	}

	if (!m_pDoc->m_pDataCtrl->DelClrs(aKey)) return;
	 
	MakeItemEx();
	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);	
}

void CDgnRebarCpgStiffMain::OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);

	*pResult = 0;
}

void CDgnRebarCpgStiffMain::OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	/*
	if ((pNMListView->uNewState & LVIS_SELECTED) == 0)
	{
		nIndex = -1;
		//pResult = 0;
		//return;
	}
	else 
	*/
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);

	*pResult = 0;
}

void CDgnRebarCpgStiffMain::ChangeItem(int nIndex)
{
	// enable/disable
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);

	// get data
	m_Key = 0;
	m_Data.Initialize();
	m_SectD.Initialize();
	m_SectViewD.Initialize();
	if (nIndex != -1)
	{
	  m_Key = m_wndList.GetItemData(nIndex);
		if (!m_pDoc->m_pAttrCtrl->GetClrs(m_Key, m_Data))
			m_Data.Initialize();
		m_pDoc->m_pAttrCtrl->GetSect(m_Key, m_SectD);
	}

	// display data
	m_pStiffDlg->SetCurData(&m_Data);
	
	// update view
	//if (m_SectD.nStype == D_SECT_TYPE_TAPERED)
	//{
	//  CSectUtil::GetSectFromTapPsc(m_SectD, m_SectViewD, TRUE);
	//}
	m_SectViewD = m_SectD;
	m_wndSecView.SetDataSource(&m_SectViewD);
	m_wndSecView.SetCentroidFlag(TRUE);
	m_wndSecView.Invalidate();
}


void CDgnRebarCpgStiffMain::UpdateView(BOOL bEndI)
{
	if (m_SectD.nStype != D_SECT_TYPE_TAPERED) return;
	m_SectViewD.Initialize();
	CSectUtil::GetSectFromTapPsc(m_SectD, m_SectViewD, bEndI);
	m_wndSecView.SetDataSource(&m_SectViewD);
	m_wndSecView.Invalidate();
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CDgnRebarCpgStiffMain::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pStiffDlg = new CDgnRebarCpgStiff(&m_wndSecView, this);
	m_ctrlTab.AddTab(m_pStiffDlg, _LS(IDS_DGN_CPG_STIFF_REBAR), CDgnRebarCpgStiff::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CDgnRebarCpgStiffMain::SetHeaderTitle()
{
	CStringArray aTitle;
	CArray<int, int> aWidth;

#define M_ADD_LIST_INFO(TITLE, WIDTH) aTitle.Add(TITLE); aWidth.Add(WIDTH)
	M_ADD_LIST_INFO(_LS(IDS_DGN_CPG_ID),           35);
	M_ADD_LIST_INFO(_LS(IDS_DGN_CPG_NAME),         90);
	M_ADD_LIST_INFO(_LS(IDS_DGN_CPG_LIST_REINF),	 90);  
	M_ADD_LIST_INFO(_LS(IDS_DGN_CPG_LIST_STIFF),	 90);  
#undef M_ADD_LIST_INFO

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_wndList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_wndList.GetSafeHwnd(), dwStyle);

	// Set Title
	int nNum = aTitle.GetSize();
	for (i=0; i < nNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_wndList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDgnRebarCpgStiffMain::MakeItemEx()
{
	CWaitCursor Cursor;

	m_wndList.DeleteAllItems();

	T_SECT_D Data;
	Data.Initialize();

	CArray<T_SECT_K, T_SECT_K> arSectKeyList;
	
	m_pDoc->m_pAttrCtrl->GetSectKeyList(arSectKeyList);

	for(int i=0 ; i<arSectKeyList.GetSize() ; i++)
	{
		m_pDoc->m_pAttrCtrl->GetSect(arSectKeyList[i], Data);

		//Modify by sshan. CPG.('20070307)
		// Composite I 와 D_SECT_TYPE_TAPERED이면서 Composite I 인것으로 한정
		BOOL bCompoI = FALSE;
		BOOL bCompoTapI = FALSE;

		if(Data.nStype == D_SECT_TYPE_COMPO_I)
			bCompoI = TRUE;
		if(Data.nStype == D_SECT_TYPE_TAPERED && Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I)
			bCompoTapI = TRUE;
		
		if(bCompoI || bCompoTapI)
			InsertItem(arSectKeyList[i], Data);
	}
}

BOOL CDgnRebarCpgStiffMain::InsertItem(T_SECT_K Key, T_SECT_D &Data)
{
	T_CLRS_D RData;
	if (!m_pDoc->m_pAttrCtrl->GetClrs(Key, RData))
		RData.Initialize();

	LVITEM lvitem;
	CString str;
	int nItem = m_wndList.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 4; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data, RData);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_wndList.InsertItem(&lvitem);
		}
		else m_wndList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	m_wndList.SetItemData(nItem, (DWORD)Key);
	return TRUE;
}

int CDgnRebarCpgStiffMain::FindInsertionPos(T_SECT_K nID)
{
	int nCount = m_wndList.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_SECT_K key = m_wndList.GetItemData(i);
		if (nID < key) { return i;}
	}
	return nCount;
}

CString CDgnRebarCpgStiffMain::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_CLRS_D &RData)
{
	CString str;
	if (i == 0) str.Format(_T("%d"), Key);
	else if (i == 1) str = Data.SName;
	else if (i == 2) 
	{
		str = _T("X");
		T_RPSC_D RpsdData;
		RpsdData.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetRpsd(Key, RpsdData))
		{
			if(RpsdData.Mbar[0].GetSize()>0 || RpsdData.Mbar[1].GetSize()>0)
				str = _T("O");
		}
	}
	else if (i == 3) 
	{
		str = _T("X");
		if(RData.LStif[0].GetSize()>0 || RData.LStif[1].GetSize()>0)
			str = _T("O");
	}  
	else ASSERT(0);

	return str;
}

void CDgnRebarCpgStiffMain::OnClickCopyBtn()
{
	int nItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_CLRS_K ClrsK = m_wndList.GetItemData(nItem);

	CDgnRebarCpgCopyDlg dlg(ClrsK);
	dlg.SetInitPos(D_INIT_POS_RT);
	if(dlg.DoModal()==IDOK)
	{
		MakeItemEx();
	}
}