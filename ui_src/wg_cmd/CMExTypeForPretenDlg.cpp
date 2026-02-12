// CMExTypeForPretenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMExTypeForPretenDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMExTypeForPretenDlg dialog


CCMExTypeForPretenDlg::CCMExTypeForPretenDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMExTypeForPretenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMExTypeForPretenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMExTypeForPretenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMExTypeForPretenDlg)
	DDX_Control(pDX, IDC_CMD_SELECTED_LIST, m_ExternalList);
	DDX_Control(pDX, IDC_CMD_ALL_LIST, m_StaticList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMExTypeForPretenDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMExTypeForPretenDlg)
	ON_BN_CLICKED(IDC_CMD_SEL_BUTTON, OnCmdSelButton)
	ON_BN_CLICKED(IDC_CMD_UNSEL_BUTTON, OnCmdUnselButton)
	ON_BN_CLICKED(IDC_CMD_REMOVE_DATA, OnCmdRemoveData)
	ON_LBN_DBLCLK(IDC_CMD_ALL_LIST, OnDblclkCmdAllList)
	ON_LBN_DBLCLK(IDC_CMD_SELECTED_LIST, OnDblclkCmdSelectedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMExTypeForPretenDlg message handlers

BOOL CCMExTypeForPretenDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetStldKeyList4Exld(m_arStldKeyList);

	int nSize = m_arStldKeyList.GetSize();
	m_arBExternal.SetSize(nSize);

	T_EXLD_D ExldD;
	if (!pDoc->m_pAttrCtrl->GetExld(ExldD))
	{
		for (int i=0; i<nSize; i++)
			m_arBExternal.SetAt(i, FALSE);
	}
	else
	{
		BOOL bFind;
		for (int i=0; i<nSize; i++)
		{
			bFind = FALSE;
			for (int j=0; j<ExldD.nNumExldCase; j++)
			{
				if (m_arStldKeyList[i] == ExldD.ExldCase[j])
				{
					bFind = TRUE;
					break;
				}
			}
			m_arBExternal.SetAt(i, bFind);
		}
	}

	GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(pDoc->m_pAttrCtrl->ExistExld());

	MakeListHeader();
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMExTypeForPretenDlg::MakeItemEx()
{
	m_ExternalList.DeleteAllItems();
	m_StaticList.DeleteAllItems();

	int nSize = m_arStldKeyList.GetSize();
	for (int i=0; i<nSize; i++)
		AddList(m_arStldKeyList[i], m_arBExternal[i]);
}

void CCMExTypeForPretenDlg::AddList(T_STLD_K key, BOOL bExternal)
{
	CListCtrl* pListCtrl;
	if (bExternal)
		pListCtrl = &m_ExternalList;
	else 
		pListCtrl = &m_StaticList;

	LVITEM lvitem;
	CString str;
	int nItem = pListCtrl->GetItemCount();

	for(int i = 0; i<1; i++)
	{
		lvitem.iItem = nItem;
		lvitem.iSubItem=i;
		str = DataToStr(i, key);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)key;
			nItem = pListCtrl->InsertItem(&lvitem);
		}
		else pListCtrl->SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

CString CCMExTypeForPretenDlg::DataToStr(int nIndex, T_STLD_K key)
{
	CString str = _T("");
	T_STLD_D StldD;
	StldD.Initialize();

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (pDoc->m_pAttrCtrl->GetStld(key, StldD))
		str = StldD.LoadCaseName;
	return str;
}

void CCMExTypeForPretenDlg::MakeListHeader()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Load_Case_Name)};
	int nColWidth[] = {130};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ExternalList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ExternalList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i=0; i<1; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ExternalList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}

	dwStyle = ListView_GetExtendedListViewStyle(m_StaticList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_StaticList.GetSafeHwnd(), dwStyle);

	for(i=0; i<1; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_StaticList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMExTypeForPretenDlg::OnCmdSelButton() 
{
	POSITION pos = m_StaticList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	int nIndex;
	CArray<T_STLD_K, T_STLD_K> arSelected;
	while (pos)
	{
		nIndex = m_StaticList.GetNextSelectedItem(pos);
		arSelected.Add(m_StaticList.GetItemData(nIndex));
	}

	for (int i=0; i<arSelected.GetSize(); i++)
		ChangeValue(arSelected[i]);

	MakeItemEx();
}

void CCMExTypeForPretenDlg::OnCmdUnselButton() 
{
	POSITION pos = m_ExternalList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	int nIndex;
	CArray<T_STLD_K, T_STLD_K> arSelected;
	while (pos)
	{
		nIndex = m_ExternalList.GetNextSelectedItem(pos);
		arSelected.Add(m_ExternalList.GetItemData(nIndex));
	}

	for (int i=0; i<arSelected.GetSize(); i++)
		ChangeValue(arSelected[i]);

	MakeItemEx();
}

void CCMExTypeForPretenDlg::OnCmdRemoveData() 
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pDataCtrl->DelExld()) return;
	CDialogMove::OnOK();
	/*
	int nSize = m_arStldKeyList.GetSize();
	for (int i=0; i<nSize; i++)
		m_arBExternal[i] = FALSE;
	
	MakeItemEx();
	GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(pDoc->m_pAttrCtrl->ExistExld());
	*/
}

void CCMExTypeForPretenDlg::OnDblclkCmdAllList() 
{
	int nIndex;
	if (!GetSelectedIndex(FALSE, nIndex))
		return;

	T_STLD_K StldK = (T_STLD_K)(m_StaticList.GetItemData(nIndex));
	ChangeValue(StldK);
	MakeItemEx();
}

void CCMExTypeForPretenDlg::OnDblclkCmdSelectedList() 
{
	int nIndex;
	if (!GetSelectedIndex(TRUE, nIndex))
		return;

	T_STLD_K StldK = (T_STLD_K)(m_ExternalList.GetItemData(nIndex));
	ChangeValue(StldK);
	MakeItemEx();
}

void CCMExTypeForPretenDlg::ChangeValue(T_STLD_K key)
{
	int nSize = m_arStldKeyList.GetSize();
	for (int i=0; i<nSize; i++)
	{
		if (key == m_arStldKeyList[i])
		{
			m_arBExternal[i] = !m_arBExternal[i];
			break;
		}
	}
}

BOOL CCMExTypeForPretenDlg::GetSelectedIndex(BOOL bExternal, int &nIndex)
{  
	int iItem;
	if (bExternal)
		iItem = m_ExternalList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	else
		iItem = m_StaticList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);

	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMExTypeForPretenDlg::OnOK() 
{
	T_EXLD_D ExldD;
	ExldD.Initialize();

	int nSize = m_arStldKeyList.GetSize();
	for(int i=0; i<nSize; i++)
	{
		if (m_arBExternal[i])
		{
			ExldD.ExldCase.Add(m_arStldKeyList[i]);
			ExldD.nNumExldCase++;
		}
	}

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pDataCtrl->AddExld(ExldD))
		return;
	
	CDialogMove::OnOK();
}
