// CMGroupDampingStrainDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGroupDampingStrainDlg.h"
#include "CMGroupDampingOptionDlg.h"

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
// CCMGroupDampingStrainDlg dialog


CCMGroupDampingStrainDlg::CCMGroupDampingStrainDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGroupDampingStrainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGroupDampingStrainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aGroupTypeRdo.Add(IDC_CMD_GRDP_MATL_RDO);
	m_aGroupTypeRdo.Add(IDC_CMD_GRDP_STRUCTURE_RDO);
	m_aGroupTypeRdo.Add(IDC_CMD_GRDP_BOUNDARY_RDO);
	
	m_aMaterialDirectRdo.Add(IDC_CMD_GRDP_DAMP_DEFAULT_RDO);
	m_aMaterialDirectRdo.Add(IDC_CMD_GRDP_DAMP_DIRECT_RDO);

	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}


void CCMGroupDampingStrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGroupDampingStrainDlg)
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_RATIO_EDT, m_edtDampingDef);
	DDX_Control(pDX, IDC_CMD_GRDP_NAME_CMB, m_cmbGroupName);
	DDX_Control(pDX, IDC_CMD_GRDP_RATIO_EDT, m_edtDamping);
	DDX_Control(pDX, IDC_CMD_LIST, m_wndList);
	DDX_Control(pDX, IDC_CMD_CALCULATE_CHK, m_chkCalculate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGroupDampingStrainDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGroupDampingStrainDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_GRDP_MATL_RDO, OnCmdGroupTypeRadio)
	ON_BN_CLICKED(IDC_CMD_GRDP_STRUCTURE_RDO, OnCmdGroupTypeRadio)
	ON_BN_CLICKED(IDC_CMD_GRDP_BOUNDARY_RDO, OnCmdGroupTypeRadio)
	ON_CBN_SELCHANGE(IDC_CMD_GRDP_NAME_CMB, OnSelChangeNameCmb)
	ON_BN_CLICKED(IDC_CMD_GRDP_DAMP_DEFAULT_RDO, OnChangeMaterialDirectRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_DAMP_DIRECT_RDO, OnChangeMaterialDirectRdo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnItemchangedCmdList)
	ON_BN_CLICKED(IDC_CMD_SEL_OPT_BTN, OnCmdSelectOptionBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingStrainDlg message handlers

BOOL CCMGroupDampingStrainDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_edtDampingDef.SetUnitType(0);
	m_edtDamping.SetUnitType(0);

	SetHeaderTitle();
	
	if(!m_pDoc->m_pAttrCtrl->GetGrdp(m_Data))   m_Data.Initialize();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGroupDampingStrainDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	m_Data.bExistStrain = TRUE;
	if (!m_pDoc->m_pDataCtrl->AddGrdp(m_Data)) return;

	CDialogMove::OnOK();
}

void CCMGroupDampingStrainDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMGroupDampingStrainDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	T_GRDP_BASE GrdpBase;
	if (!Dlg2BaseData(GrdpBase)) return;

	if(GrdpBase.nGrupType == 2 && GrdpBase.GrupBngrKey == 0)
	{
		CArray<T_MATL_K, T_MATL_K> aMatlK;
		m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlK);
		int nMatl = aMatlK.GetSize();
		for(int i = 0; i < nMatl; i++)
		{
			GrdpBase.GrupBngrKey = aMatlK[i];
			int nIndex = IsUsedGrupData(GrdpBase.nGrupType, GrdpBase.GrupBngrKey);
			
			T_MATL_D MatlD;
			if(!m_pDoc->m_pAttrCtrl->GetMatl(GrdpBase.GrupBngrKey, MatlD))      {  ASSERT(0);		return ;	}
			GrdpBase.dDampingRatio = MatlD.dDampingRatio;
			
			if (nIndex == -1) m_Data.aStrainGrupDamping.Add(GrdpBase);
			else m_Data.aStrainGrupDamping.SetAt(nIndex, GrdpBase);
		}
	}
	else
	{ 
		// add or modify
		int nIndex = IsUsedGrupData(GrdpBase.nGrupType, GrdpBase.GrupBngrKey);
		if (nIndex == -1) m_Data.aStrainGrupDamping.Add(GrdpBase);
		else m_Data.aStrainGrupDamping.SetAt(nIndex, GrdpBase);
	}
	
	SetGroupNameCombo();
	MakeItemEx();
}

void CCMGroupDampingStrainDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here

	// 현재 선택된 아이템 가져오기 
	int iItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_GRDP_No_Selected_Group_Damping));
		return;
	}
	if (iItem >= m_Data.aStrainGrupDamping.GetSize())
	{
		ASSERT(0);
		return;
	}
	// delete
	m_Data.aStrainGrupDamping.RemoveAt(iItem);

	SetGroupNameCombo();
	MakeItemEx();

	int nCount = m_wndList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_wndList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

}

void CCMGroupDampingStrainDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_GRDP_No_Selected_Group_Damping));
		return;
	}
	if (iItem >= m_Data.aStrainGrupDamping.GetSize())
	{
		ASSERT(0);
		return;
	}

	// modify
	T_GRDP_BASE GrdpBase;
	if (!Dlg2BaseData(GrdpBase)) return;
	if(GrdpBase.GrupBngrKey == 0)   return;
	m_Data.aStrainGrupDamping.SetAt(iItem, GrdpBase);

	SetGroupNameCombo();
	MakeItemEx();	

	m_wndList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMGroupDampingStrainDlg::OnSelChangeNameCmb() 
{
	int ix = m_cmbGroupName.GetCurSel();
	if (ix == CB_ERR)    return; 
	T_MATL_K MatlK = m_cmbGroupName.GetItemData(ix);

	GetDlgItem(IDC_CMD_GRDP_DAMP_DIRECT_RDO)->EnableWindow(MatlK != 0);
	GetDlgItem(IDC_CMD_BTN_MODIFY)->EnableWindow(MatlK != 0);
	GetDlgItem(IDC_CMD_BTN_DELETE)->EnableWindow(MatlK != 0);

	int nGroupType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);

	if(nGroupType == 0)
	{
		if(MatlK == 0)
		{
			m_edtDamping.SetWindowText(_T("--"));
			
			CDlgUtil::CtrlRadioSetCheck(this, m_aMaterialDirectRdo, 0);   // Use Material Data
			OnChangeMaterialDirectRdo();
			
			GetDlgItem(IDC_CMD_BTN_ADD)->SetWindowText(_LS(IDS_CMD_DEFINE));
			return;
		}
		
		int nMaterialDirect = 0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, nMaterialDirect);
		if(nMaterialDirect == 0)
		{
			T_MATL_D MatlD;
			if(!m_pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))   MatlD.Initialize();
			m_edtDamping.SetEditUnit(MatlD.dDampingRatio);
		}
	}
	GetDlgItem(IDC_CMD_BTN_ADD)->SetWindowText(_LS(IDS_CMD_ADD));
}

void CCMGroupDampingStrainDlg::OnCmdGroupTypeRadio() 
{
	// TODO: Add your control notification handler code here
	int nGroupType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);
	
	if(nGroupType != 0)
	{
		int nMaterialDirect = 0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, nMaterialDirect);
		
		if(nMaterialDirect == 0)
		{
			CDlgUtil::CtrlRadioSetCheck(this, m_aMaterialDirectRdo, 1);
			OnChangeMaterialDirectRdo();
		}
	}

	GetDlgItem(IDC_CMD_GRDP_DAMP_DEFAULT_RDO)->EnableWindow(nGroupType  == 0);
	SetGroupNameCombo();
}

void CCMGroupDampingStrainDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	T_GRDP_BASE BaseData;
	BaseData.Initialize();

	int iItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem != -1) 
	{
		if (iItem < m_Data.aStrainGrupDamping.GetSize())
			BaseData = m_Data.aStrainGrupDamping.GetAt(iItem);
		BaseData2Dlg(BaseData);
	}

	*pResult = 0;
}

void CCMGroupDampingStrainDlg::OnChangeMaterialDirectRdo()
{
	int nMaterialDirect = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, nMaterialDirect);
	m_edtDamping.SetReadOnly(nMaterialDirect == 0);
	
	if(nMaterialDirect == 1)
	{
		m_edtDamping.SetEditUnit(m_edtDampingDef.GetEditValue());
		return;
	}
	
	int nGroupType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);
	if(nGroupType != 0)     return ;
	
	int ix = m_cmbGroupName.GetCurSel();
	if (ix == CB_ERR)    return;
	
	T_MATL_K MatlK = m_cmbGroupName.GetItemData(ix);
	if(MatlK == 0)
	{
		m_edtDamping.SetWindowText(_T("--"));
		return;
	}
	
	T_MATL_D MatlD;
	if(!m_pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))   MatlD.Initialize();
	m_edtDamping.SetEditUnit(MatlD.dDampingRatio);
}

void CCMGroupDampingStrainDlg::OnCmdSelectOptionBtn()
{
	CCMGroupDampingOptionDlg Dlg(1, &m_Data);
	Dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// User Defiend Init
#define COLCOUNT   3

void CCMGroupDampingStrainDlg::SetGroupNameCombo()
{
	m_cmbGroupName.ResetContent();

	int nType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nType);

	CStringArray aGrupName;
	CArray<UINT, UINT> aGrupKey;
	if (nType == 0)  // Material
	{
		T_MATL_D MatlD;
		CArray<T_MATL_K, T_MATL_K> aMatlK;
		m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlK);
		int nNum = aMatlK.GetSize();
		for (int i=0; i<nNum ; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetMatl(aMatlK[i], MatlD)) continue;
			aGrupName.Add(MatlD.Name);
			aGrupKey.Add(aMatlK[i]);
		}
		if(nNum)
		{
			int nIndex = m_cmbGroupName.AddString(_LS(IDS_CMD_ALL_MATL_DATA));
			m_cmbGroupName.SetItemData(nIndex, 0);
		}
	}
	else if (nType == 1)       // Structure Group
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
	else if (nType == 2)  // Boundary Group
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
	for (int i=0; i<aGrupName.GetSize(); i++)
	{
		nIndex = m_cmbGroupName.AddString(aGrupName[i]);
		m_cmbGroupName.SetItemData(nIndex, aGrupKey[i]);
	}
	m_cmbGroupName.SetCurSel(0);
	OnSelChangeNameCmb();
}

int CCMGroupDampingStrainDlg::IsUsedGrupData(int nType, UINT GrupK)
{
	for (int i=0; i<m_Data.aStrainGrupDamping.GetSize(); i++)
	{
		if (m_Data.aStrainGrupDamping[i].nGrupType == nType && 
				m_Data.aStrainGrupDamping[i].GrupBngrKey == GrupK) return i;
	}
	return -1;
}

void CCMGroupDampingStrainDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_CMD_NAME), _LS(IDS_CMD_GRDP_GROUP_TYPE), _LS(IDS_CMD_GRDP_DAMPING_RATIO)};
	int nColWidth[] = { 80, 70, 95};

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
CString CCMGroupDampingStrainDlg::DataToStr(int i, int nIndex, T_GRDP_BASE &Data)
{
	CString str;

	if (i == 0) str = GetGroupName(Data.nGrupType, Data.GrupBngrKey);
	else if (i == 1) str = GetGroupType(Data.nGrupType);
	else if (i == 2) str.Format(_T("%g"), Data.dDampingRatio);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

CString CCMGroupDampingStrainDlg::GetGroupName(int nGrupType, UINT GrupK)
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
	else if (nGrupType == 2)
	{
		T_MATL_D MatlD;
		if (m_pDoc->m_pAttrCtrl->GetMatl(GrupK, MatlD))
			return MatlD.Name;
	}
	else ASSERT(0);

	return _T("");
}

CString CCMGroupDampingStrainDlg::GetGroupType(int nGrupType)
{
	if (nGrupType == 0) return _LS(IDS_CMD_GRDP_TYPE_STRUCTURE);
	else if (nGrupType == 1) return _LS(IDS_CMD_GRDP_TYPE_BOUNDARY);
	else if (nGrupType == 2) return _LS(IDS_CMD_CSCS__Material);
	return _T("");
}

// List에 Item 넣기(한줄씩)
BOOL CCMGroupDampingStrainDlg::InsertItem(int nIndex, T_GRDP_BASE &Data)
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
void CCMGroupDampingStrainDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_wndList.DeleteAllItems();

	int nItemCount = m_Data.aStrainGrupDamping.GetSize();
	if (nItemCount == 0) return;

	for (int nIndex = 0 ; nIndex < nItemCount ; nIndex++)
	{
		InsertItem(nIndex, m_Data.aStrainGrupDamping[nIndex]);
	}
}

void CCMGroupDampingStrainDlg::BaseData2Dlg(T_GRDP_BASE& rBaseData)
{
	int nGroupType = rBaseData.nGrupType+1;
	if(rBaseData.nGrupType == 2)    nGroupType = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aGroupTypeRdo, nGroupType);
	OnCmdGroupTypeRadio();

//   SetGroupNameCombo();
//   if (rBaseData.GrupBngrKey == 0) return;
	for (int i=0; i<m_cmbGroupName.GetCount(); i++)
	{
		if (rBaseData.GrupBngrKey == m_cmbGroupName.GetItemData(i))
		{
			m_cmbGroupName.SetCurSel(i);
			break;
		}
	}
	OnSelChangeNameCmb();

	CDlgUtil::CtrlRadioSetCheck(this, m_aMaterialDirectRdo, rBaseData.nDampInputType);
	OnChangeMaterialDirectRdo();
	
	int nMaterialDirect = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, nMaterialDirect);
	if(nMaterialDirect == 1)
	{
		m_edtDamping.SetEditUnit(rBaseData.dDampingRatio);
	}
}

BOOL CCMGroupDampingStrainDlg::Dlg2BaseData(T_GRDP_BASE& rBaseData)
{
	int nGroupType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);
	if(nGroupType == 0)   rBaseData.nGrupType = 2;
	else                  rBaseData.nGrupType = nGroupType-1;

	int ix = m_cmbGroupName.GetCurSel();
	if (ix == CB_ERR)
	{
		AfxMessageBox(_LS(IDS_CMD_GRDP_No_Selected_Name_of_Group));
		return FALSE;
	}
	rBaseData.GrupBngrKey = m_cmbGroupName.GetItemData(ix);

	CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, rBaseData.nDampInputType);

	rBaseData.dDampingRatio = m_edtDamping.GetEditValue();

	return TRUE;
}

void CCMGroupDampingStrainDlg::Data2Dlg()
{
	// MNET:4015-HSSHIM-20110615
	//m_edtDampingDef.SetEditUnit(m_Data.dDampingDefault);
	m_edtDampingDef.SetEditUnit(m_Data.dStrainDampingDef);
	
	MakeItemEx();

//   if (m_Data.aStrainGrupDamping.GetSize() > 0)
//   {
//     m_wndList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
//   }
//   else
//   {
		T_GRDP_BASE BaseData;
		BaseData.Initialize();
		BaseData.nGrupType = 2;
		BaseData2Dlg(BaseData);
//   }

	m_chkCalculate.SetCheck(m_Data.bCalculateWhenUsed);
}

BOOL CCMGroupDampingStrainDlg::Dlg2Data()
{
	// MNET:4015-HSSHIM-20110615
	//m_Data.dDampingDefault = m_edtDampingDef.GetEditValue();
	m_Data.dStrainDampingDef = m_edtDampingDef.GetEditValue();
	m_Data.bCalculateWhenUsed = m_chkCalculate.GetCheck();
	
	return TRUE;
}


