// CMEnvlItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMEnvlItemDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMEnvlItemDlg dialog


CCMEnvlItemDlg::CCMEnvlItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMEnvlItemDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	//{{AFX_DATA_INIT(CCMEnvlItemDlg)
	//}}AFX_DATA_INIT
}


void CCMEnvlItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMEnvlItemDlg)
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDesc);
	DDX_Control(pDX, IDC_CMD_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_LIST2, m_SelList);
	DDX_Control(pDX, IDC_CMD_LIST, m_UnselList);
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMEnvlItemDlg::Data2Dlg()
{
	m_wndName.SetWindowText(m_Data.EnvelopeName);
	m_wndDesc.SetWindowText(m_Data.Description);

	int nSize = m_Data.arEnvelope.GetSize();
	int nSelSize = m_aSelFlag.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		int j = 0;
		for (j = 0; j < nSelSize; j++)
			if (m_Data.arEnvelope[i].AnalType == m_aAnalType[j] &&
					m_Data.arEnvelope[i].LoadCaseKey == m_aKey[j]) break;
		if (j != nSelSize) m_aSelFlag[j] = TRUE;
	}

	MakeItemEx(TRUE);
	MakeItemEx(FALSE);
}

BOOL CCMEnvlItemDlg::Dlg2Data()
{
	m_wndName.GetWindowText(m_Data.EnvelopeName);
	m_wndDesc.GetWindowText(m_Data.Description);

	m_Data.EnvelopeName.TrimLeft(); m_Data.EnvelopeName.TrimRight();
	if (m_Data.EnvelopeName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Name_has_not_been_input_));
		return FALSE;
	}
	
	T_ENVL_BASE item;
	m_Data.arEnvelope.RemoveAll();
	int nSize = m_aSelFlag.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (m_aSelFlag[i])
		{
			item.AnalType = m_aAnalType[i];
			item.LoadCaseKey = m_aKey[i];
			m_Data.arEnvelope.Add(item);
		}
	}

	if (m_Data.arEnvelope.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_loadcase_));
		return FALSE;
	}

	return TRUE;
}

BOOL CCMEnvlItemDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyEnvl(m_csOldName, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddEnvl(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

#define COLCOUNT 2
/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg implementation functions
void CCMEnvlItemDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_WG_CMD__ADDD__Type)};
	int nColWidth[COLCOUNT] = {100, 72};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_UnselList.GetSafeHwnd());
						ListView_GetExtendedListViewStyle(m_SelList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_UnselList.GetSafeHwnd(), dwStyle);
	ListView_SetExtendedListViewStyle(m_SelList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_UnselList.InsertColumn(i,&lvcolumn);
		m_SelList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMEnvlItemDlg::MakeItemEx(BOOL bUnsel)
{
	CListCtrl* pList;
	if (bUnsel) pList = &m_UnselList;
	else pList = &m_SelList;

	int nCount = m_aSelFlag.GetSize();

	pList->DeleteAllItems();

	int nItem = 0;
	for (int i = 0; i < nCount; i++)
	{
		if (m_aSelFlag[i] != bUnsel)
		{
			pList->InsertItem(nItem, _T(""));
			pList->SetItemText(nItem, 0, m_aName[i]);
			switch(m_aAnalType[i])
			{
			case D_LCOM_STATIC: pList->SetItemText(nItem, 1, _T("ST")); break;
			case D_LCOM_SPECTRUM: pList->SetItemText(nItem, 1, _T("RS")); break;
			case D_LCOM_HISTORY: pList->SetItemText(nItem, 1, _T("TH")); break;
			case D_LCOM_MOVING: pList->SetItemText(nItem, 1, _T("MV")); break;
			case D_LCOM_SETTLEMENT: pList->SetItemText(nItem, 1, _T("SM")); break;
			case D_LCOM_STAGE: pList->SetItemText(nItem, 1, _T("CS")); break;
			case D_LCOM_CB_GENERAL: pList->SetItemText(nItem, 1, _T("CB")); break;
			case D_LCOM_CB_STEEL: pList->SetItemText(nItem, 1, _T("CBS")); break;
			case D_LCOM_CB_CONCRETE: pList->SetItemText(nItem, 1, _T("CBC")); break;
			case D_LCOM_CB_SRC: pList->SetItemText(nItem, 1, _T("CBR")); break;
			case D_LCOM_CB_FOOTING: pList->SetItemText(nItem, 1, _T("CBF")); break;
			case D_LCOM_CB_ALUMINUM: pList->SetItemText(nItem, 1, _T("CBA")); break;
			case D_LCOM_CB_STLCOMP: pList->SetItemText(nItem, 1, _LS(IDS_WG_CMD__ADDD__CBSC)); break;
			}
			pList->SetItemData(nItem, i);
			nItem++;
		}
	}
}

void CCMEnvlItemDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;

	// Update all of the selected items.
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for (UINT i=0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

void CCMEnvlItemDlg::MakeLoadComb()
{
	int i;
	// Load case 종류 추가시 아래에 추가
	// Stld
	T_STLD_D stld;
	CArray<T_STLD_K, T_STLD_K> aStldKey;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldKey);
	int nSize = aStldKey.GetSize();
	for (i = 0; i < nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(aStldKey.GetAt(i), stld);
		if (stld.LoadCaseType == _T("CS") || stld.LoadCaseType == _T("TCS")) continue;  // 시공타입은 제외
		m_aKey.Add(aStldKey.GetAt(i));
		m_aAnalType.Add(D_LCOM_STATIC);
		m_aName.Add(stld.LoadCaseName);
	}
	// spectrum loadcase
	T_SPLC_D splc;
	CArray<T_SPLC_K, T_SPLC_K> aSplcKey;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcKey);
	nSize = aSplcKey.GetSize();
	for (i = 0; i < nSize; i++)
	{
		m_aKey.Add(aSplcKey.GetAt(i));
		m_aAnalType.Add(D_LCOM_SPECTRUM);
		m_pDoc->m_pAttrCtrl->GetSplc(aSplcKey.GetAt(i), splc);
		m_aName.Add(splc.LoadCaseName);
	}
	// time history loadcase
	T_THIS_D thisData;
	CArray<T_THIS_K, T_THIS_K> aThisKey;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey, FALSE);
	nSize = aThisKey.GetSize();
	for (i = 0; i < nSize; i++)
	{
		m_aKey.Add(aThisKey.GetAt(i));
		m_aAnalType.Add(D_LCOM_HISTORY);
		m_pDoc->m_pAttrCtrl->GetThis(aThisKey.GetAt(i), thisData);
		m_aName.Add(thisData.LoadCaseName);
	}
	// moving loadcase
	T_MVLD_D mvld;
	CArray<T_MVLD_K, T_MVLD_K> aMvldKey;
	m_pDoc->m_pAttrCtrl->GetMvldKeyList(aMvldKey);
	nSize = aMvldKey.GetSize();
	for (i = 0; i < nSize; i++)
	{
		m_aKey.Add(aMvldKey.GetAt(i));
		m_aAnalType.Add(D_LCOM_MOVING);
		m_pDoc->m_pAttrCtrl->GetMvld(aMvldKey.GetAt(i), mvld);
		m_aName.Add(mvld.LoadCaseName);
	}
	// settlement
	T_SMLC_D smlc;
	CArray<T_SMLC_K, T_SMLC_K> aSmlcKey;
	m_pDoc->m_pAttrCtrl->GetSmlcKeyList(aSmlcKey);
	nSize = aSmlcKey.GetSize();
	for (i = 0; i < nSize; i++)
	{
		m_aKey.Add(aSmlcKey.GetAt(i));
		m_aAnalType.Add(D_LCOM_SETTLEMENT);
		m_pDoc->m_pAttrCtrl->GetSmlc(aSmlcKey.GetAt(i), smlc);
		m_aName.Add(smlc.LoadCaseName);
	}
	// construction stage
	T_SGLD_D sgld;
	CArray<T_SGLD_K, T_SGLD_K> aSgldKey;
	m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldKey);
	nSize = aSgldKey.GetSize();
	for (i = 0; i < nSize; i++)
	{
		m_aKey.Add(aSgldKey.GetAt(i));
		m_aAnalType.Add(D_LCOM_STAGE);
		m_pDoc->m_pAttrCtrl->GetSgld(aSgldKey.GetAt(i), sgld);
		m_aName.Add(sgld.LoadCaseName);
	}

	// Load Combination
	CArray<T_LCOM_K, T_LCOM_K> aLcomKey;
	int nLcomType[] = { 
		D_LCOMTYPE_GENERAL, D_LCOMTYPE_STEEL, D_LCOMTYPE_CONCRETE, D_LCOMTYPE_SRC, D_LCOMTYPE_FDN, D_LCOMTYPE_ALUMINUM, D_LCOMTYPE_STLCOMP, 
	};
	int nAnalType[] = {
		D_LCOM_CB_GENERAL, D_LCOM_CB_STEEL, D_LCOM_CB_CONCRETE, D_LCOM_CB_SRC, D_LCOM_CB_FOOTING, D_LCOM_CB_ALUMINUM, D_LCOM_CB_STLCOMP,
	};

	T_LCOM_D lcom;
	for (int k = 0; k < sizeof(nLcomType)/sizeof(int); k++)
	{
		aLcomKey.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomType[k], aLcomKey);
		int nSize = aLcomKey.GetSize();
		for (i = 0; i < nSize; i++)
		{
			m_aKey.Add(aLcomKey.GetAt(i));
			m_aAnalType.Add(nAnalType[k]);
			m_pDoc->m_pAttrCtrl->GetLcom(nLcomType[k], aLcomKey.GetAt(i), lcom);
			m_aName.Add(lcom.LoadCombName);
		}
	}

	nSize = m_aKey.GetSize();
	m_aSelFlag.SetSize(nSize);
	for (i = 0; i < nSize; i++) m_aSelFlag[i] = FALSE;
}

BEGIN_MESSAGE_MAP(CCMEnvlItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMEnvlItemDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST2, OnDblclkCmdList2)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMEnvlItemDlg message handlers

BOOL CCMEnvlItemDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_bModify)
	{
		m_Data.Initialize();
	}

	MakeLoadComb();

	SetHeaderTitle();
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMEnvlItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;

	CDialogMove::OnOK();
}

void CCMEnvlItemDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();
}

void CCMEnvlItemDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMEnvlItemDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnAdd();
	*pResult = 0;
}

void CCMEnvlItemDlg::OnDblclkCmdList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnDelete();
	*pResult = 0;
}

void CCMEnvlItemDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CArray<int, int> aItem;
	GetSelectedItemList(&m_UnselList, aItem);
	if (aItem.GetSize() == 0) return;

	int nIx;
	int nSize = aItem.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		nIx = m_UnselList.GetItemData(aItem[i]);
		m_aSelFlag[nIx] = TRUE;
	}
	MakeItemEx(TRUE);
	MakeItemEx(FALSE);
}

void CCMEnvlItemDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	CArray<int, int> aItem;
	GetSelectedItemList(&m_SelList, aItem);
	if (aItem.GetSize() == 0) return;

	int nIx;
	int nSize = aItem.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		nIx = m_SelList.GetItemData(aItem[i]);
		m_aSelFlag[nIx] = FALSE;
	}
	MakeItemEx(TRUE);
	MakeItemEx(FALSE);
}
