// CMGroupDampingDlg.cpp : implementation file
//
// 2004. 01. 12      by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGroupDampingDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingDlg dialog


CCMGroupDampingDlg::CCMGroupDampingDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGroupDampingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGroupDampingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlTypeRadio.Add(IDC_CMD_GROUP_TYPE_RD1);
	m_aCtrlTypeRadio.Add(IDC_CMD_GROUP_TYPE_RD2);

	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}


void CCMGroupDampingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGroupDampingDlg)
	DDX_Control(pDX, IDC_CMD_DAMPING_RATIO, m_editDamping);
	DDX_Control(pDX, IDC_CMD_DEF_DAMP_RATIO_ALPHA_EDIT, m_editDampingAlpha);
	DDX_Control(pDX, IDC_CMD_DEF_DAMP_RATIO_BETA_EDIT, m_editDampingBeta);
	DDX_Control(pDX, IDC_CMD_DAMP_RATIO_EDIT, m_editRatio);
	DDX_Control(pDX, IDC_CMD_DAMP_RATIO_ALPHA_EDIT, m_editAlpha);
	DDX_Control(pDX, IDC_CMD_DAMP_RATIO_BETA_EDIT, m_editBeta);
	DDX_Control(pDX, IDC_CMD_GROUP_NAME_CB, m_cobxName);
	DDX_Control(pDX, IDC_CMD_LIST, m_wndList);
	DDX_Control(pDX, IDC_CMD_CALCULATE_CHK, m_CalculateChk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGroupDampingDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGroupDampingDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_GROUP_TYPE_RD1, OnCmdGroupTypeRadio)
	ON_BN_CLICKED(IDC_CMD_GROUP_TYPE_RD2, OnCmdGroupTypeRadio)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnItemchangedCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingDlg message handlers

BOOL CCMGroupDampingDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editDamping.SetUnitType(0);
	m_editRatio.SetUnitType(0);

	SetHeaderTitle();
	
	m_Data.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistGrdp())
	{
		m_pDoc->m_pAttrCtrl->GetGrdp(m_Data);
	}

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGroupDampingDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	if (!m_pDoc->m_pDataCtrl->AddGrdp(m_Data)) return;

	CDialogMove::OnOK();
}

void CCMGroupDampingDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMGroupDampingDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	T_GRDP_BASE GrdpBase;
	if (!Dlg2BaseData(GrdpBase)) return;

	// add or modify
	int nIndex = IsUsedGrupData(GrdpBase.nGrupType, GrdpBase.GrupBngrKey);
	if (nIndex == -1) m_Data.aGrupDamping.Add(GrdpBase);
	else m_Data.aGrupDamping.SetAt(nIndex, GrdpBase);
	
	SetGroupNameCombo();
	MakeItemEx();
}

void CCMGroupDampingDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here

	// 현재 선택된 아이템 가져오기 
	int iItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_GRDP_No_Selected_Group_Damping));
		return;
	}
	if (iItem >= m_Data.aGrupDamping.GetSize())
	{
		ASSERT(0);
		return;
	}
	// delete
	m_Data.aGrupDamping.RemoveAt(iItem);

	SetGroupNameCombo();
	MakeItemEx();

	int nCount = m_wndList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_wndList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

}

void CCMGroupDampingDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_GRDP_No_Selected_Group_Damping));
		return;
	}
	if (iItem >= m_Data.aGrupDamping.GetSize())
	{
		ASSERT(0);
		return;
	}

	// modify
	T_GRDP_BASE GrdpBase;
	if (!Dlg2BaseData(GrdpBase)) return;
	m_Data.aGrupDamping.SetAt(iItem, GrdpBase);

	SetGroupNameCombo();
	MakeItemEx();	

	m_wndList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMGroupDampingDlg::OnCmdGroupTypeRadio() 
{
	// TODO: Add your control notification handler code here
	SetGroupNameCombo();
}

void CCMGroupDampingDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	T_GRDP_BASE BaseData;
	BaseData.Initialize();

	int iItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem != -1) 
	{
		if (iItem < m_Data.aGrupDamping.GetSize())
			BaseData = m_Data.aGrupDamping.GetAt(iItem);
	}
	BaseData2Dlg(BaseData);

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// User Defiend Init
#define COLCOUNT   5

void CCMGroupDampingDlg::SetGroupNameCombo()
{
	int nType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlTypeRadio, nType);

	CStringArray aGrupName;
	CArray<UINT, UINT> aGrupKey;
	if (nType == 0)       // Structure Group
	{
		T_GRUP_D GrupD;
		CArray<T_GRUP_K, T_GRUP_K> aGrupK;
		m_pDoc->m_pAttrCtrl->GetGrupKeyList(aGrupK);
		int nNum = aGrupK.GetSize();
		for (int i=0; i<nNum ; i++)
		{
			//if (IsUsedGrupData(0, aGrupK[i])) continue;;
			if (!m_pDoc->m_pAttrCtrl->GetGrup(aGrupK[i], GrupD)) continue;
			aGrupName.Add(GrupD.GroupName);
			aGrupKey.Add(aGrupK[i]);
		}
	}
	else if (nType == 1)  // Boundary Group
	{
		T_BNGR_D BngrD;
		CArray<T_BNGR_K, T_BNGR_K> aBngrK;
		m_pDoc->m_pAttrCtrl->GetBngrKeyList(aBngrK);
		int nNum = aBngrK.GetSize();
		for (int i=0; i<nNum ; i++)
		{
			//if (IsUsedGrupData(1, aBngrK[i])) continue;
			if (!m_pDoc->m_pAttrCtrl->GetBngr(aBngrK[i], BngrD)) continue;
			aGrupName.Add(BngrD.GroupName);
			aGrupKey.Add(aBngrK[i]);
		}
	}
	else ASSERT(0);

	int nIndex;
	m_cobxName.ResetContent();
	for (int i=0; i<aGrupName.GetSize(); i++)
	{
		nIndex = m_cobxName.AddString(aGrupName[i]);
		m_cobxName.SetItemData(nIndex, aGrupKey[i]);
	}
	m_cobxName.SetCurSel(0);
}

int CCMGroupDampingDlg::IsUsedGrupData(int nType, UINT GrupK)
{
	for (int i=0; i<m_Data.aGrupDamping.GetSize(); i++)
	{
		if (m_Data.aGrupDamping[i].nGrupType == nType && 
				m_Data.aGrupDamping[i].GrupBngrKey == GrupK) return i;
	}
	return -1;
}

void CCMGroupDampingDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_CMD_GRDP_GROUP_NAME), _LS(IDS_CMD_GRDP_GROUP_TYPE), _LS(IDS_CMD_GRDP_DAMPING_RATIO), 
											 _LS(IDS_CMD_GRDP_GROUP_ALPHA), _LS(IDS_CMD_GRDP_GROUP_BETA)};
	int nColWidth[] = { 60, 60, 95, 50, 50 };

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_wndList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_wndList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_wndList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

// Data를 List에 넣을 수 있도록 String으로 변환
CString CCMGroupDampingDlg::DataToStr(int i, int nIndex, T_GRDP_BASE &Data)
{
	CString str;

	if (i == 0) str = GetGroupName(Data.nGrupType, Data.GrupBngrKey);
	else if (i == 1) str = GetGroupType(Data.nGrupType);
	else if (i == 2) str.Format(_T("%g"), Data.dDampingRatio);
	else if (i == 3) str.Format(_T("%g"), Data.dMassCoef);
	else if (i == 4) str.Format(_T("%g"), Data.dStifCoef);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

CString CCMGroupDampingDlg::GetGroupName(int nGrupType, UINT GrupK)
{
	if (nGrupType == 0)
	{
		T_GRUP_D GrupD;
		if (m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD))
			return GrupD.GroupName;
	}
	else if (nGrupType == 1)
	{
		T_BNGR_D BngrD;
		if (m_pDoc->m_pAttrCtrl->GetBngr(GrupK, BngrD))
			return BngrD.GroupName;
	}
	else ASSERT(0);

	return _T("");
}

CString CCMGroupDampingDlg::GetGroupType(int nGrupType)
{
	if (nGrupType == 0) return _LS(IDS_CMD_GRDP_TYPE_STRUCTURE);
	else if (nGrupType == 1) return _LS(IDS_CMD_GRDP_TYPE_BOUNDARY);
	return _T("");
}

// List에 Item 넣기(한줄씩)
BOOL CCMGroupDampingDlg::InsertItem(int nIndex, T_GRDP_BASE &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_wndList.GetItemCount();

	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, nIndex, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;
		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)nIndex;
			nItem = m_wndList.InsertItem(&lvitem);
		}
		else m_wndList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	return TRUE;
}

// List에 Item 채워넣기(전체)
void CCMGroupDampingDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_wndList.DeleteAllItems();

	int nItemCount = m_Data.aGrupDamping.GetSize();
	if (nItemCount == 0) return;

	for (int nIndex = 0 ; nIndex < nItemCount ; nIndex++)
	{
		InsertItem(nIndex, m_Data.aGrupDamping[nIndex]);
	}
}

void CCMGroupDampingDlg::BaseData2Dlg(T_GRDP_BASE& rBaseData)
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlTypeRadio, rBaseData.nGrupType);
	m_editRatio.SetEditUnit(rBaseData.dDampingRatio);
	m_editAlpha.SetEditUnit(rBaseData.dMassCoef);
	m_editBeta.SetEditUnit(rBaseData.dStifCoef);
	SetGroupNameCombo();
	if (rBaseData.GrupBngrKey == 0) return;
	for (int i=0; i<m_cobxName.GetCount(); i++)
	{
		if (rBaseData.GrupBngrKey == m_cobxName.GetItemData(i))
			m_cobxName.SetCurSel(i);
	}
}

BOOL CCMGroupDampingDlg::Dlg2BaseData(T_GRDP_BASE& rBaseData)
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlTypeRadio, rBaseData.nGrupType);
	int ix = m_cobxName.GetCurSel();
	if (ix == CB_ERR)
	{
		AfxMessageBox(_LS(IDS_CMD_GRDP_No_Selected_Name_of_Group));
		return FALSE;
	}
	rBaseData.GrupBngrKey = m_cobxName.GetItemData(ix);
	rBaseData.dDampingRatio = m_editRatio.GetEditValue();
	rBaseData.dMassCoef = m_editAlpha.GetEditValue();
	rBaseData.dStifCoef = m_editBeta.GetEditValue();

	return TRUE;
}

void CCMGroupDampingDlg::Data2Dlg()
{
	m_editDamping.SetEditUnit(m_Data.dDampingDefault);
	m_editDampingAlpha.SetEditUnit(m_Data.dMassCoefDefault);
	m_editDampingBeta.SetEditUnit(m_Data.dStifCoefDefault);
	
	MakeItemEx();

	if (m_Data.aGrupDamping.GetSize() > 0)
	{
		m_wndList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
	else
	{
		T_GRDP_BASE BaseData;
		BaseData.Initialize();
		BaseData2Dlg(BaseData);
	}
	m_CalculateChk.SetCheck(m_Data.bCalculateWhenUsed);
}

BOOL CCMGroupDampingDlg::Dlg2Data()
{
	m_Data.dDampingDefault = m_editDamping.GetEditValue();
	m_Data.dMassCoefDefault = m_editDampingAlpha.GetEditValue();
	m_Data.dStifCoefDefault = m_editDampingBeta.GetEditValue();

	m_Data.bCalculateWhenUsed = m_CalculateChk.GetCheck();

	return TRUE;
}


