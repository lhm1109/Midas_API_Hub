// DgnRebarCpgMomentMain.cpp : implementation file
//
// Add by sshan. ('20061227)
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRebarCpgMomentMain.h"

#include "DgnRebarPscMoment.h"

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
// CDgnRebarCpgMomentMain dialog

CDgnRebarCpgMomentMain::CDgnRebarCpgMomentMain(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnRebarCpgMomentMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRebarCpgMomentMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_Key = 0;
	m_Data.Initialize();

	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_pMomentDlg = 0;  
}

/*
void CDgnRebarCpgMomentMain::SetParamData(T_RPSC_K RpscK)
{
	m_Key = RpscK;
	m_bModify = TRUE;
}*/

void CDgnRebarCpgMomentMain::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRebarCpgMomentMain)
	DDX_Control(pDX, IDC_DGN_DELETE, m_btnDel);
	DDX_Control(pDX, IDOK, m_btnAdd);
	DDX_Control(pDX, IDC_DGN_SECT_LIST, m_wndList);
	DDX_Control(pDX, IDC_DGN_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRebarCpgMomentMain, CChildDialog)
	//{{AFX_MSG_MAP(CDgnRebarCpgMomentMain)
	ON_BN_CLICKED(IDC_DGN_DELETE, OnDgnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SECT_LIST, OnItemchangedDgnSectList)
	ON_NOTIFY(NM_CLICK, IDC_DGN_SECT_LIST, OnClickDgnSectList)
	ON_BN_CLICKED(IDC_DGN_COPY_BTN, OnClickCopyBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgMomentMain message handlers

BOOL CDgnRebarCpgMomentMain::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
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
void CDgnRebarCpgMomentMain::OnOK() 
{
	// TODO: Add extra validation here
	
	// get data
	if (!m_pMomentDlg->GetCurData(&m_Data))
	{
		AfxMessageBox(_LS(IDS_DGN_RPSC_ERR_MAIN_REBAR));
		return;
	}
	
	// Modify by sshan. Composite_I Section만 들어오므로 nPart는 2.('20070209)
	// 재오씨와 협의 하여 결정
	for(int c=0 ; c<m_Data.Mbar[0].GetSize() ; c++)
	{
		m_Data.Mbar[0][c].nPart = 2;
	}
	for(int c=0 ; c<m_Data.Mbar[1].GetSize() ; c++)
	{
		m_Data.Mbar[1][c].nPart = 2;
	}

	CArray<T_RPSC_K, T_RPSC_K>  aKey;
	CArray<T_RPSC_D, T_RPSC_D&> aData;
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
	if (!m_pDoc->m_pDataCtrl->AddRpsd(aKey, aData))
		return;

	MakeItemEx();

	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);

	//CChildDialog::OnOK();
}

// close
void CDgnRebarCpgMomentMain::OnCancel() 
{
	// TODO: Add extra cleanup here
	((CDialog*)m_pParent)->EndDialog(IDCANCEL); // need change! added by mylee
	//CChildDialog::OnCancel();
}

// delete
void CDgnRebarCpgMomentMain::OnDgnDelete() 
{
	//Design에서는 Delete기능 막음
	// TODO: Add your control notification handler code here
	
	T_RPSC_K key;
	CArray<T_RPSC_K, T_RPSC_K> aKey;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		key = m_wndList.GetItemData(nItem);
		if (m_pDoc->m_pAttrCtrl->ExistRpsd(key)) aKey.Add(key);
	}

	if (!m_pDoc->m_pDataCtrl->DelRpsd(aKey)) return;
	 
	MakeItemEx();
	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	ChangeItem(nIndex);	
}

void CDgnRebarCpgMomentMain::OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);

	*pResult = 0;
}

void CDgnRebarCpgMomentMain::OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CDgnRebarCpgMomentMain::ChangeItem(int nIndex)
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
		if (!m_pDoc->m_pAttrCtrl->GetRpsd(m_Key, m_Data))
			m_Data.Initialize();
		m_pDoc->m_pAttrCtrl->GetSect(m_Key, m_SectD);
	}

	// display data
	m_pMomentDlg->SetCurData(&m_Data);

	// Modify by sshan. Composite_I Section만 들어오므로 nPart는 2.('20070209)
	// 재오씨와 협의 하여 결정
	for(int c=0 ; c<m_Data.Mbar[0].GetSize() ; c++)
	{
		m_Data.Mbar[0][c].nPart = 2;
	}
	for(int c=0 ; c<m_Data.Mbar[1].GetSize() ; c++)
	{
		m_Data.Mbar[1][c].nPart = 2;
	}
	
	// update view
	if (m_SectD.nStype == D_SECT_TYPE_TAPERED)
	{
		CSectUtil::GetSectFromTapPsc(m_SectD, m_SectViewD, TRUE);
	}
	else m_SectViewD = m_SectD;
	m_wndSecView.SetDataSource(&m_SectViewD);
	m_wndSecView.SetCentroidFlag(TRUE);
	m_wndSecView.Invalidate();
}


void CDgnRebarCpgMomentMain::UpdateView(BOOL bEndI)
{
	if (m_SectD.nStype != D_SECT_TYPE_TAPERED) return;
	m_SectViewD.Initialize();
	CSectUtil::GetSectFromTapPsc(m_SectD, m_SectViewD, bEndI);
	m_wndSecView.SetDataSource(&m_SectViewD);
	m_wndSecView.Invalidate();
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CDgnRebarCpgMomentMain::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pMomentDlg = new CDgnRebarPscMoment(&m_wndSecView, 2, this);//CPG일경우 nPart=2
	m_ctrlTab.AddTab(m_pMomentDlg, _LS(IDS_DGN_RPSC_MAIN_REBAR), CDgnRebarPscMoment::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CDgnRebarCpgMomentMain::SetHeaderTitle()
{
	CStringArray aTitle;
	CArray<int, int> aWidth;

#define M_ADD_LIST_INFO(TITLE, WIDTH) aTitle.Add(TITLE); aWidth.Add(WIDTH)
	M_ADD_LIST_INFO(_LS(IDS_DGN_RPSC_ID),           35);
	M_ADD_LIST_INFO(_LS(IDS_DGN_RPSC_NAME),         90);
	M_ADD_LIST_INFO(_LS(IDS_DGN_CPG_LIST_REINF),		90);  
	M_ADD_LIST_INFO(_LS(IDS_DGN_CPG_LIST_STIFF),		90);  
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

void CDgnRebarCpgMomentMain::MakeItemEx()
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

		//Composite I Only
		//Modify by sshan. CPG.('20070307)
		//Composite I 와 D_SECT_TYPE_TAPERED이면서 Composite I 인것으로 한정
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

BOOL CDgnRebarCpgMomentMain::InsertItem(T_SECT_K Key, T_SECT_D &Data)
{
	T_RPSC_D RData;
	if (!m_pDoc->m_pAttrCtrl->GetRpsd(Key, RData))
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

int CDgnRebarCpgMomentMain::FindInsertionPos(T_SECT_K nID)
{
	int nCount = m_wndList.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_SECT_K key = m_wndList.GetItemData(i);
		if (nID < key) { return i;}
	}
	return nCount;
}

CString CDgnRebarCpgMomentMain::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RPSC_D &RData)
{
	CString str;
	if (i == 0) str.Format(_T("%d"), Key);
	else if (i == 1) str = Data.SName;
	else if (i == 2) 
	{
		str = _T("X");
		if(RData.Mbar[0].GetSize()>0 || RData.Mbar[1].GetSize()>0)
			str = _T("O");
	}
	else if (i == 3) 
	{
		str = _T("X");
		T_CLRS_D ClrsData;
		ClrsData.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetClrs(Key, ClrsData))
		{
			if(ClrsData.LStif[0].GetSize()>0 || ClrsData.LStif[1].GetSize()>0)
				str = _T("O");
		}
	}
	
	else ASSERT(0);

	return str;
}

void CDgnRebarCpgMomentMain::OnClickCopyBtn()
{
	int nItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_RPSC_K RpscK = m_wndList.GetItemData(nItem);

	CDgnRebarCpgCopyDlg dlg(RpscK);
	dlg.SetInitPos(D_INIT_POS_RT);
	if(dlg.DoModal()==IDOK)
	{
		MakeItemEx();
	}
}