// CMSectScaleFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectScaleFactorDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "CMSecPageStiffDlg.h"  
#include "CMLoadFireLoadAnalysDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorDlg dialog


CCMSectScaleFactorDlg::CCMSectScaleFactorDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectScaleFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectScaleFactorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectScaleFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectScaleFactorDlg)
	DDX_Control(pDX, IDC_CMD_SSCALE_SHOWSTIF, m_StiffBtn);
	DDX_Control(pDX, IDC_CMD_SSCALE_DEL, m_DelBtn);
	DDX_Control(pDX, IDC_CMD_SSCALE_ADD, m_AddBtn);
	DDX_Control(pDX, IDC_CMD_SSCALE_SFLIST, m_sflist);
	DDX_Control(pDX, IDC_CMD_SSCALE_SECTLIST, m_slist);
	DDX_Control(pDX, IDC_CMD_SSCALE_FZZZ, m_fzzz);
	DDX_Control(pDX, IDC_CMD_SSCALE_FZYY, m_fzyy);
	DDX_Control(pDX, IDC_CMD_SSCALE_FZXX, m_fzxx);
	DDX_Control(pDX, IDC_CMD_SSCALE_FASZ, m_fasz);
	DDX_Control(pDX, IDC_CMD_SSCALE_FASY, m_fasy);
	DDX_Control(pDX, IDC_CMD_SSCALE_FAREA, m_farea);
	DDX_Control(pDX, IDC_CMD_SSCALE_FWGT, m_fwgt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectScaleFactorDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectScaleFactorDlg)
	ON_BN_CLICKED(IDC_CMD_SSCALE_ADD, OnCmdSscaleAdd)
	ON_BN_CLICKED(IDC_CMD_SSCALE_DEL, OnCmdSscaleDel)
	ON_NOTIFY(NM_CLICK, IDC_CMD_SSCALE_SFLIST, OnClickCmdSscaleSflist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SSCALE_SFLIST, OnItemchangedCmdSscaleSflist)
	ON_NOTIFY(NM_CLICK, IDC_CMD_SSCALE_SECTLIST, OnClickCmdSscaleSectlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SSCALE_SECTLIST, OnItemchangedCmdSscaleSectlist)
	ON_BN_CLICKED(IDC_CMD_SSCALE_SHOWSTIF, OnCmdSscaleShowstif)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorDlg message handlers

BOOL CCMSectScaleFactorDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetHeaderTitle_Sect();
	SetHeaderTitle_Scal();

	// initialize scale factor //////////////
	T_SECF_D data;
	data.Initialize();
	m_farea.SetEditUnit(data.dScaleFactor[0]);
	m_fasy.SetEditUnit(data.dScaleFactor[1]);
	m_fasz.SetEditUnit(data.dScaleFactor[2]);
	m_fzxx.SetEditUnit(data.dScaleFactor[3]);
	m_fzyy.SetEditUnit(data.dScaleFactor[4]);
	m_fzzz.SetEditUnit(data.dScaleFactor[5]);
	m_fwgt.SetEditUnit(data.dScaleFactor[6]);
	/////////////////////////////////////////

	MakeItemEx_Sect();
	MakeItemEx_Scal();

	m_AddBtn.EnableWindow(FALSE);
	m_DelBtn.EnableWindow(FALSE);
	m_StiffBtn.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
// Section List dlg
void CCMSectScaleFactorDlg::SetHeaderTitle_Sect()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Name)};
	int nColWidth[2];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_slist.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_slist.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 30;
	nColWidth[1] = 200;

	// Set Title
	for(i=0; i < 2; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_slist.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMSectScaleFactorDlg::MakeItemEx_Sect()
{
	CWaitCursor Cursor;

	m_slist.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSect();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SECT_K Key;
		T_SECT_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartSect();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextSect(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetSect(Key,Data);
			InsertItem_Sect(Key, Data);
		}
		delete []KeyBuf;
	}
}

BOOL CCMSectScaleFactorDlg::InsertItem_Sect(T_SECT_K Key, T_SECT_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_slist.GetItemCount();

	lvitem.iItem = FindInsertionPos_Sect(Key);
	for(int i=0; i < 2; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr_Sect(i, Key, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if(i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_slist.InsertItem(&lvitem);
		}
		else m_slist.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	m_slist.SetItemData(nItem, (DWORD)Key);
	return TRUE;
}

int CCMSectScaleFactorDlg::FindInsertionPos_Sect(T_SECT_K nID)
{
	T_SECT_K key;

	int nCount = m_slist.GetItemCount();
	int i = 0;
	for (i=0; i < nCount; i++)
	{
		key = m_slist.GetItemData(i);
		if (nID < key) break;
	}
	return i;
}

CString CCMSectScaleFactorDlg::DataToStr_Sect(int i, T_SECT_K Key, T_SECT_D &Data)
{
	CString str;
	if (i == 0) str.Format(_T("%d"), Key);
	else str = Data.SName;

	return str;
}

void CCMSectScaleFactorDlg::OnItemchangedCmdSscaleSectlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ((pNMListView->uNewState & LVIS_SELECTED) == 0)
	{
		*pResult = 0;
		return;
	}

	m_AddBtn.EnableWindow(TRUE);

	T_SECT_K Sectkey;
	Sectkey = m_slist.GetItemData(pNMListView->iItem);

	// Scale factor 리스트에 해당 항목 있으면 선택해준다.
	T_SECF_K SecfKey;
	T_SECF_D SecfData;
	int nItem = m_sflist.GetItemCount();
	for (int i = 0; i < nItem; i++)
	{
/*작업필요*/    SecfKey.keymap = m_sflist.GetItemData(i);
/*작업필요*/    if(SecfKey.key.entity == Sectkey)
		{
			m_sflist.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	    m_pDoc->m_pAttrCtrl->GetSecf(SecfKey, SecfData);
	    m_farea.SetEditUnit(SecfData.dScaleFactor[0]);
	    m_fasy.SetEditUnit(SecfData.dScaleFactor[1]);
	    m_fasz.SetEditUnit(SecfData.dScaleFactor[2]);
	    m_fzxx.SetEditUnit(SecfData.dScaleFactor[3]);
	    m_fzyy.SetEditUnit(SecfData.dScaleFactor[4]);
	    m_fzzz.SetEditUnit(SecfData.dScaleFactor[5]);
	    m_fwgt.SetEditUnit(SecfData.dScaleFactor[6]);
		}
		else
			m_sflist.SetItemState(i, 0,  LVIS_SELECTED | LVIS_FOCUSED);
	}
	m_DelBtn.EnableWindow(m_sflist.GetSelectedCount() != 0);
	m_StiffBtn.EnableWindow(m_sflist.GetSelectedCount() != 0);


	*pResult = 0;
}

void CCMSectScaleFactorDlg::OnClickCmdSscaleSectlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	int iItem = m_slist.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		m_AddBtn.EnableWindow(FALSE);
	}
	else
	{
		m_AddBtn.EnableWindow(TRUE);
	}

	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Scale Factor List dlg
void CCMSectScaleFactorDlg::SetHeaderTitle_Scal()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Name), _T("fArea"), _T("fAsy"), _T("fAsz"), _T("fIxx"), _T("fIyy"), _T("fIzz"), _T("fWgt"), _T("Group")};
	int nColWidth[10];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_sflist.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_sflist.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 30;
	nColWidth[1] = 100;
	nColWidth[2] = 45;
	nColWidth[3] = 40;
	nColWidth[4] = 40;
	nColWidth[5] = 40;
	nColWidth[6] = 40;
	nColWidth[7] = 40;
	nColWidth[8] = 40;
	nColWidth[9] = 45;

	// Set Title
	for(i=0; i < sizeof(nColWidth)/sizeof(int); i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_sflist.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMSectScaleFactorDlg::MakeItemEx_Scal()
{
	CWaitCursor Cursor;

	m_sflist.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSecf();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SECF_K Key;
		T_SECF_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartSecf();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextSecf(pos, Key, Data);
/*작업필요*/		  KeyBuf[nCount++] = Key.keymap;
		}

		for(nCount=0 ; nCount < nItemCount ; nCount++)
	  {
/*작업필요*/		  Key.keymap = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetSecf(Key, Data);
			InsertItem_Scal(Key, Data);
		}
		delete []KeyBuf;
	}
}

BOOL CCMSectScaleFactorDlg::DeleteItem_Scal(T_SECF_K Key, T_SECF_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
/*작업필요*/  FindInfo.lParam = (LPARAM)Key.keymap;
	int nItem = m_sflist.FindItem(&FindInfo);
	if (nItem != -1) m_sflist.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMSectScaleFactorDlg::InsertItem_Scal(T_SECF_K Key, T_SECF_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_sflist.GetItemCount();

	lvitem.iItem = FindInsertionPos_Scal(Key);
	for(int i = 0; i < 10; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr_Scal(i, Key, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if(i == 0)
		{
			lvitem.mask |= LVIF_PARAM;
/*작업필요*/      lvitem.lParam = (LPARAM)Key.keymap;
			nItem = m_sflist.InsertItem(&lvitem);
		}
		else m_sflist.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

/*작업필요*/	m_sflist.SetItemData(nItem, (DWORD)Key.keymap);
	return TRUE;
}

int CCMSectScaleFactorDlg::FindInsertionPos_Scal(T_SECF_K nID)
{
	T_SECF_K key;

	int nCount = m_sflist.GetItemCount();
	int i = 0;
	for (i=0; i < nCount; i++)
	{
/*작업필요*/    key.keymap = m_sflist.GetItemData(i);
/*작업필요*/    if (nID.keymap < key.keymap) break;
	}
	return i;
}

CString CCMSectScaleFactorDlg::DataToStr_Scal(int i, T_SECF_K Key, T_SECF_D &Data)
{
	T_SECT_D sdata;
	CString str;

	if(i == 0)
	{
		str.Format(_T("%d"), Key);
	}
	else if(i == 1)
	{
		m_pDoc->m_pAttrCtrl->GetSect(Key.key.entity, sdata);
		str = sdata.SName;
	}
	else if(i == 9)
	{		
		T_GRUP_D GrupData;
		str = m_pDoc->m_pAttrCtrl->GetGrupName(Data.GroupKey);    
	}
	else
	{
		int nNum = i - 2;
		str.Format(_T("%g"), Data.dScaleFactor[nNum]);
	}

	return str;
}

void CCMSectScaleFactorDlg::OnCmdSscaleAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	POSITION pos;
	int curpos;
	CArray<T_SECF_K, T_SECF_K> akey;
	T_SECT_K SectKey;
	T_SECF_K SecfKey;
	T_SECF_D data;
	data.Initialize();

	data.dScaleFactor[0] = m_farea.GetEditValue();
	data.dScaleFactor[1] = m_fasy.GetEditValue();
	data.dScaleFactor[2] = m_fasz.GetEditValue();
	data.dScaleFactor[3] = m_fzxx.GetEditValue();
	data.dScaleFactor[4] = m_fzyy.GetEditValue();
	data.dScaleFactor[5] = m_fzzz.GetEditValue();
	data.dScaleFactor[6] = m_fwgt.GetEditValue();
/*임시*/  data.GroupKey = 0;

	int nNum = m_slist.GetSelectedCount();
	pos = m_slist.GetFirstSelectedItemPosition();
	for(int i=0; i < nNum; i++)
	{
		curpos = m_slist.GetNextSelectedItem(pos);
		SectKey = m_slist.GetItemData(curpos);
		if(!m_pDoc->m_pAttrCtrl->MakeKeySecf(SectKey, data.GroupKey, SecfKey)) continue;
		akey.Add(SecfKey);
	}
	
	if(!m_pDoc->m_pDataCtrl->AddSecf(akey, data)) return;
	MakeItemEx_Scal();
}

void CCMSectScaleFactorDlg::OnCmdSscaleDel() 
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	int curpos, startpos;
	CArray<T_SECF_K, T_SECF_K> akey;
	T_SECF_K key;
	int nNum = m_sflist.GetSelectedCount();
	pos = m_sflist.GetFirstSelectedItemPosition();
	for(int i=0; i < nNum; i++)
	{
		curpos = m_sflist.GetNextSelectedItem(pos);
		key.keymap = m_sflist.GetItemData(curpos);
		akey.Add(key);
		if(i == 0) startpos = curpos;
	}

	if (!m_pDoc->m_pDataCtrl->DelSecf(akey)) return;
	MakeItemEx_Scal();

	int nCount = m_sflist.GetItemCount();
	if (nCount <= startpos) startpos -= 1;
	if (startpos >= 0) 
		m_sflist.SetItemState(startpos, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMSectScaleFactorDlg::OnItemchangedCmdSscaleSflist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	// Section List에서 해당 되는 것 선택해 준다.
	if ((pNMListView->uNewState & LVIS_SELECTED) == 0)
	{
		*pResult = 0;
		return;
	}

	T_SECF_K key;
	T_SECF_D data;
/*작업필요*/	key.keymap = m_sflist.GetItemData(pNMListView->iItem);
	m_pDoc->m_pAttrCtrl->GetSecf(key, data);

	m_farea.SetEditUnit(data.dScaleFactor[0]);
	m_fasy.SetEditUnit(data.dScaleFactor[1]);
	m_fasz.SetEditUnit(data.dScaleFactor[2]);
	m_fzxx.SetEditUnit(data.dScaleFactor[3]);
	m_fzyy.SetEditUnit(data.dScaleFactor[4]);
	m_fzzz.SetEditUnit(data.dScaleFactor[5]);
	m_fwgt.SetEditUnit(data.dScaleFactor[6]);

	T_SECT_K SectKey;
	int nItem = m_slist.GetItemCount();
	for (int i = 0; i < nItem; i++)
	{
		SectKey = m_slist.GetItemData(i);
		if (SectKey == key.key.entity)
			m_slist.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		else
			m_slist.SetItemState(i, 0,  LVIS_SELECTED | LVIS_FOCUSED);
	}
 
	*pResult = 0;
}

void CCMSectScaleFactorDlg::OnClickCmdSscaleSflist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	int iItem = m_sflist.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		m_DelBtn.EnableWindow(FALSE);
		m_StiffBtn.EnableWindow(FALSE);
	}
	else
	{
		m_DelBtn.EnableWindow(TRUE);
		m_StiffBtn.EnableWindow(TRUE);
	}

	*pResult = 0;
}

void CCMSectScaleFactorDlg::OnCmdSscaleShowstif() 
{
	// TODO: Add your control notification handler code here
	T_SECF_K SecfKey;
	int nNum = m_sflist.GetSelectedCount();
	if (nNum == 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Error___No_selected_section_scale_fact));
		return;
	}
	POSITION pos = m_sflist.GetFirstSelectedItemPosition();
	int ix;
	for(int i=0; i < nNum; i++)
	{
		ix = m_sflist.GetNextSelectedItem(pos);
/*작업필요*/    SecfKey.keymap = m_sflist.GetItemData(ix);
	}
	T_SECT_D SectData;
	T_SECF_D SecfData;
	if (!m_pDoc->m_pAttrCtrl->GetSect(SecfKey.key.entity, SectData)) { ASSERT(0); return; }
	if (!m_pDoc->m_pAttrCtrl->GetSecf(SecfKey, SecfData)) { ASSERT(0); return; }

	// scale
	ApplyScaleFactor(SectData.SectBefore.Stiffness, SecfData);
	ApplyScaleFactor(SectData.SectBefore.SectI.Stiffness, SecfData);
	ApplyScaleFactor(SectData.SectBefore.SectJ.Stiffness, SecfData);
	ApplyScaleFactor(SectData.SectAfter.Stiffness, SecfData);
	ApplyScaleFactor(SectData.SectAfter.SectI.Stiffness, SecfData);
	ApplyScaleFactor(SectData.SectAfter.SectJ.Stiffness, SecfData);
	
	CCMStiffDlg dlg(m_pDoc, &SectData, NULL, this);
	dlg.DoModal();
}

void CCMSectScaleFactorDlg::ApplyScaleFactor(T_SECT_STIFFNESS& Stiff, T_SECF_D& Secf)
{
	Stiff.Area *= Secf.dScaleFactor[0];
	Stiff.Asy  *= Secf.dScaleFactor[1];
	Stiff.Asz  *= Secf.dScaleFactor[2];
	Stiff.Rxx  *= Secf.dScaleFactor[3];
	Stiff.Ryy  *= Secf.dScaleFactor[4];
	Stiff.Rzz  *= Secf.dScaleFactor[5];
	Stiff.WArea *= Secf.dScaleFactor[6];
}