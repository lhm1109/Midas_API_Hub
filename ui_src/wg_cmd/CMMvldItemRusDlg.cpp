// CMMvldItemRusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemRusDlg.h"
#include "CMMvldSubItemDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_base\TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key  : 31;
	} typekey;
};

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemRusDlg dialog

CCMMvldItemRusDlg::CCMMvldItemRusDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemRusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemRusDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aOptimCtrl.RemoveAll();
	m_aOptimCtrl.Add(IDC_CMD_MVLD_OPTIM_GRP);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_EDT);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_UNT);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_LCASE_GRP);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_LANE_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_LANE_CMB);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_EDT);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_EDT);

	m_aOptimCtrl.Add(IDC_CMD_LOAD_EFFECT_STC2);
	m_aOptimCtrl.Add(IDC_CMD_RADIO_COMBINED2);
	m_aOptimCtrl.Add(IDC_CMD_RADIO_INDEPENDENT2);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_ASSIGN_VEHL_GRP);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_SEL_VEHL_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_SEL_VEHL_CMB);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_SCALE_FACTOR_STC);
	m_aOptimCtrl.Add(IDC_CMD_MVLD_SCALE_FACTOR_EDT);
	m_aOptimCtrl.Add(IDC_CMD_ASSIGN_VEHL_LIST);
	m_aOptimCtrl.Add(IDC_CMD_ASSIGN_VEHL_ADD_BTN);
	m_aOptimCtrl.Add(IDC_CMD_ASSIGN_VEHL_MOD_BTN);
	m_aOptimCtrl.Add(IDC_CMD_ASSIGN_VEHL_DEL_BTN);

	m_aNotOptimCtrl.RemoveAll();
	m_aNotOptimCtrl.Add(IDC_CMD_SLC_FRM);
	m_aNotOptimCtrl.Add(IDC_CMD_LOAD_EFFECT_STC);
	m_aNotOptimCtrl.Add(IDC_CMD_RADIO_COMBINED);
	m_aNotOptimCtrl.Add(IDC_CMD_RADIO_INDEPENDENT);
	m_aNotOptimCtrl.Add(IDC_CMD_LIST);
	m_aNotOptimCtrl.Add(IDC_CMD_BTN_ADD);
	m_aNotOptimCtrl.Add(IDC_CMD_BTN_MODIFY);
	m_aNotOptimCtrl.Add(IDC_CMD_BTN_DELETE);

	m_aOkCancelCtrl.RemoveAll();
	m_aOkCancelCtrl.Add(IDOK);
	m_aOkCancelCtrl.Add(IDCANCEL);
	m_aOkCancelCtrl.Add(IDC_CMD_APPLY);		
	
	m_Data.Initialize();

	m_pDoc = NULL;
	m_bModify = FALSE;
	m_bSurface= FALSE;

	m_nLcomType = 0;
	m_nCombOption = 1;
	m_nCombOption2 = 1;
	m_bAutoOptim = FALSE;
}

void CCMMvldItemRusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemRusDlg)
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME,             m_wndLoadCase);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION,               m_wndDesc);
	DDX_Radio  (pDX, IDC_CMD_LCOM_1ST_RDO,              m_nLcomType);
	DDX_Radio  (pDX, IDC_CMD_RADIO_COMBINED,            m_nCombOption);
	DDX_Control(pDX, IDC_CMD_LIST,                      m_List);
	DDX_Check  (pDX, IDC_CMD_MVLD_OPTIM_CHK,            m_bAutoOptim);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_EDT,    m_edtMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_UNT,    m_untMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB,             m_cmbOptimLane);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_NUM_VEHL_EDT,     m_edtMinNumVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_MAX_NUM_VEHL_EDT,     m_edtMaxNumVehl);
	DDX_Radio  (pDX, IDC_CMD_RADIO_COMBINED2,           m_nCombOption2);
	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB,         m_cmbSelVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_SCALE_FACTOR_EDT,     m_edtScaleFactorOptim);
	DDX_Control(pDX, IDC_CMD_ASSIGN_VEHL_LIST,          m_AssignVehlList);
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldItemRusDlg::InitUnit()
{
	m_edtMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_untMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_edtMinNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtMaxNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactorOptim.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvldItemRusDlg::InitCombo()
{
	// Loaded Lane
	m_cmbOptimLane.ResetContent();

	if(m_bSurface)
	{
		T_SLANop_D SlanD; SlanD.Initialize();
		CArray<T_SLANop_K, T_SLANop_K> aSlanK; aSlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetSlanopKeyList(aSlanK);

		for(int i=0; i<aSlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSlanop(aSlanK[i], SlanD)) continue;
			m_cmbOptimLane.SetItemData(m_cmbOptimLane.AddString(SlanD.LaneName), aSlanK[i]);
		}
	}
	else
	{
		T_LLANop_D LlanD; LlanD.Initialize();
		CArray<T_LLANop_K, T_LLANop_K> aLlanK; aLlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetLlanopKeyList(aLlanK);

		for(int i=0; i<aLlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetLlanop(aLlanK[i], LlanD)) continue;
			m_cmbOptimLane.SetItemData(m_cmbOptimLane.AddString(LlanD.LineLaneName), aLlanK[i]);
		}
	}

	m_cmbOptimLane.SetCurSel(0);

	// Selected Vehicle
	m_cmbSelVehl.ResetContent();

	int nIndex = 0;
	T_MVLD_TYPE_KEY TypeKey;

	T_MVHC_D MvhcD; MvhcD.Initialize();
	T_MVHL_D MvhlD; MvhlD.Initialize();
	CArray<T_MVHC_K, T_MVHC_K> aMvhcK; aMvhcK.RemoveAll();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK; aMvhlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvhcKeyList(aMvhcK);
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);

	for(int i=0; i<aMvhcK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhc(aMvhcK[i], MvhcD)) continue;
		nIndex = m_cmbSelVehl.AddString(_T("VC:")+MvhcD.VehicleClassName);
		TypeKey.typekey.type = 0;
		TypeKey.typekey.key = aMvhcK[i];
		m_cmbSelVehl.SetItemData(nIndex, TypeKey.value);
	}

	for(int i=0; i<aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;
		if( m_pDoc->m_pAttrCtrl->IsPermitVehicle(MvhlD)) continue;

		nIndex = m_cmbSelVehl.AddString(_T("VL:")+MvhlD.VehicleLoadName);
		TypeKey.typekey.type = 1;
		TypeKey.typekey.key = aMvhlK[i];
		m_cmbSelVehl.SetItemData(nIndex, TypeKey.value);
	}

	m_cmbSelVehl.SetCurSel(0);
}

void CCMMvldItemRusDlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;
	int nDistY = 0;	

	GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aOptimCtrl, nDistY);

	if(m_bAutoOptim) GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rRef);
	else             GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aOkCancelCtrl, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMMvldItemRusDlg::ShowHideControls()
{
	UpdateData(TRUE);

	if(m_bAutoOptim)
	{
		CDlgUtil::CtrlShowHide(this, m_aOptimCtrl,    SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aNotOptimCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aOptimCtrl,    SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aNotOptimCtrl, SW_SHOW);
	}
}

void CCMMvldItemRusDlg::Data2Dlg()
{
	m_wndLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.SetWindowText(m_Data.Description);

	m_nLcomType = m_Data.nLoadCombType;
	m_nCombOption = m_Data.nCombOption;

	m_bAutoOptim = m_Data.bAutoOptimize;
	m_edtMinVehlDist.SetEditUnit(m_Data.dMinVehlDist);
	
	int nSelect = 0;
	int i = 0;
	for(i=0; i<m_cmbOptimLane.GetCount(); i++)
	{
		if(m_Data.OptimizeLane != m_cmbOptimLane.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbOptimLane.SetCurSel(i);

	m_edtMinNumVehl.SetEditUnit(m_Data.nMinNumVehicle);
	m_edtMaxNumVehl.SetEditUnit(m_Data.nMaxNumVehicle);
	m_nCombOption2 = m_Data.nCombOption;

	// Default Value
	m_edtScaleFactorOptim.SetWindowText(_T("1.0"));

	UpdateData(FALSE);
}

BOOL CCMMvldItemRusDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_wndLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_wndDesc.GetWindowText(m_Data.Description);

	m_Data.nLoadCombType = m_nLcomType;
	m_Data.nCombOption = m_nCombOption;

	m_Data.bAutoOptimize = m_bAutoOptim;
	m_Data.dMinVehlDist = m_edtMinVehlDist.GetEditValue();
	m_Data.OptimizeLane = m_cmbOptimLane.GetItemData(m_cmbOptimLane.GetCurSel());
	m_Data.nMinNumVehicle = m_edtMinNumVehl.GetEditValue();
	m_Data.nMaxNumVehicle = m_edtMaxNumVehl.GetEditValue();
	if(m_bAutoOptim) m_Data.nCombOption = m_nCombOption2;
	
	return TRUE;
}

BOOL CCMMvldItemRusDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyMvld(m_csOldName, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddMvld(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

#define COLCOUNT 8
#define COLCOUNT_ASSIGNVEHL 2
/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg implementation functions
void CCMMvldItemRusDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact), _LS(IDS_WG_CMD__ADDD__Lane1), _LS(IDS_WG_CMD__ADDD__Lane2), _LS(IDS_WG_CMD__ADDD__Lane3), _LS(IDS_WG_CMD__ADDD__Lane4), _LS(IDS_WG_CMD__ADDD__Lane5), _LS(IDS_WG_CMD__ADDD__More)};
	int nColWidth[COLCOUNT] = {108, 50, 60, 60, 60, 60, 60, 48};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemRusDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.arSubLoadData.GetSize();
	if(nItemCount == 0) return;

	if(nItemCount)
	{
		for(int nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
			InsertItem(nCount, m_Data.arSubLoadData[nCount]);
		}
	}
}

BOOL CCMMvldItemRusDlg::InsertItem(int nIndex, T_MVLD_BASE &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemRusDlg::DeleteItem(int nIndex, T_MVLD_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_List.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemRusDlg::ModifyItem(int nIndex, T_MVLD_BASE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Data);
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemRusDlg::DataToStr(int i, T_MVLD_BASE &Data)
{
	CString str;

	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if(i==0) 
	{
		if (Data.nVehicleType == 1)
		{
			m_pDoc->m_pAttrCtrl->GetMvhc(Data.VehicleKey, mvhc);
			str = _T("VC:")+mvhc.VehicleClassName;
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(Data.VehicleKey, mvhl);
			str = _T("VL:")+mvhl.VehicleLoadName;
		}
	}
	else if(i==1) str.Format(_T("%g"), Data.dVehicleScaleFactor);
	else if(i>=2 && i<=6)
	{
		if(Data.aSelectedLanes.GetSize() > i-2)
		{
			if(Data.aSelectedLanes[i-2] == 0) str = _T("");
			else
			{
				str = GetLaneName(Data.aSelectedLanes[i-2]);
			}
		}
		else str = _T("");
	}
	else if (i==7)
	{
		if(Data.aSelectedLanes.GetSize() > i-2)
		{
			if (Data.aSelectedLanes[i-2] == 0) str = _T("");
			else str = _T("...");
		}
		else str = _T("");
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

CString CCMMvldItemRusDlg::GetLaneName(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLAN_D slan;
		m_pDoc->m_pAttrCtrl->GetSlan(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_LLAN_D llan;
		m_pDoc->m_pAttrCtrl->GetLlan(nLaneKey, llan);
		return llan.LineLaneName;
	}
	return _LS(IDS_WG_CMD__ADDD__Error);
}

void CCMMvldItemRusDlg::SetHeaderTitle_AssignVehl()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact)};
	int nColWidth[COLCOUNT_ASSIGNVEHL] = {118, 80};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_AssignVehlList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_AssignVehlList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT_ASSIGNVEHL; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_AssignVehlList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemRusDlg::MakeItemEx_AssignVehl()
{
	m_AssignVehlList.DeleteAllItems();

	int nItemCount = m_Data.aOptimize.GetSize();
	if(nItemCount == 0) return;

	if(nItemCount)
	{
		for(int nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			InsertItem_AssignVehl(nCount, m_Data.aOptimize[nCount]);
		}
	}
}

BOOL CCMMvldItemRusDlg::InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr_AssignVehl(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			nItem = m_AssignVehlList.InsertItem(&lvitem);
		}
		else m_AssignVehlList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemRusDlg::DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_AssignVehlList.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemRusDlg::ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr_AssignVehl(i, Data);
			m_AssignVehlList.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemRusDlg::DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data)
{
	CString str;

	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if(i==0) 
	{
		if (Data.nVehicleType == 1)
		{
			m_pDoc->m_pAttrCtrl->GetMvhc(Data.VehicleK, mvhc);
			str = _T("VC:")+mvhc.VehicleClassName;
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(Data.VehicleK, mvhl);
			str = _T("VL:")+mvhl.VehicleLoadName;
		}
	}
	else if(i==1) str.Format(_T("%g"), Data.dScaleFactor);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

BEGIN_MESSAGE_MAP(CCMMvldItemRusDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemRusDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY,               OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_MVLD_OPTIM_CHK,      OnCmdAutoOptimChk)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,             OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY,          OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE,          OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_ADD_BTN, OnCmdBtnAdd_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_MOD_BTN, OnCmdBtnModify_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_DEL_BTN, OnCmdBtnDelete_AssignVehl)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST,         OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemRusDlg message handlers

BOOL CCMMvldItemRusDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	
	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0 || m_pDoc->m_pAttrCtrl->GetCountSlanop() > 0) m_bSurface = TRUE;
	else m_bSurface = FALSE;

	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dMinVehlDist = M_InitValueCurUnit(1.0, KN, M, CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	}

	InitUnit();
	InitCombo();

	SetHeaderTitle();
	SetHeaderTitle_AssignVehl();
	Data2Dlg();
	MakeItemEx();
	MakeItemEx_AssignVehl();

	AlignControl();
	ShowHideControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemRusDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldItemRusDlg::OnCmdApply() 
{
	ApplyOrOK();	
}

void CCMMvldItemRusDlg::OnCmdAutoOptimChk() 
{
	AlignControl();
	ShowHideControls();
}

void CCMMvldItemRusDlg::OnCmdBtnAdd() 
{
	CCMMvldSubItemDlg dlg(this);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemRusDlg::OnCmdBtnModify() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
 
	CCMMvldSubItemDlg dlg(this);
	dlg.SetParamData(m_Data.arSubLoadData[iItem], iItem);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

BOOL CCMMvldItemRusDlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
{
	if (!bModify)
	{
		int nCount = m_Data.arSubLoadData.GetSize();
		m_Data.arSubLoadData.Add(data);
		InsertItem(nCount, data);
	}
	else
	{
		m_Data.arSubLoadData.SetAt(nPos, data);
		ModifyItem(nPos, data);
	}
	return TRUE;
}

void CCMMvldItemRusDlg::OnCmdBtnDelete() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
	m_Data.arSubLoadData.RemoveAt(iItem);
	m_List.DeleteItem(iItem);

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvldItemRusDlg::OnCmdBtnAdd_AssignVehl() 
{
	UpdateData(TRUE);

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cmbSelVehl.GetItemData(m_cmbSelVehl.GetCurSel());

	Data.nVehicleType = TypeKey.typekey.type + 1;
	Data.VehicleK = TypeKey.typekey.key;
	Data.dScaleFactor = m_edtScaleFactorOptim.GetEditValue();

	if(Data.nVehicleType == 1)
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhc(Data.VehicleK)) return;
	}
	else
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhl(Data.VehicleK)) return;
	}

	int nCount = m_Data.aOptimize.GetSize();
	m_Data.aOptimize.Add(Data);
	InsertItem_AssignVehl(nCount, Data);
}

void CCMMvldItemRusDlg::OnCmdBtnModify_AssignVehl() 
{
	UpdateData(TRUE);

	int iItem = m_AssignVehlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cmbSelVehl.GetItemData(m_cmbSelVehl.GetCurSel());

	Data.nVehicleType = TypeKey.typekey.type + 1;
	Data.VehicleK = TypeKey.typekey.key;
	Data.dScaleFactor = m_edtScaleFactorOptim.GetEditValue();

	if(Data.nVehicleType == 1)
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhc(Data.VehicleK)) return;
	}
	else
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhl(Data.VehicleK)) return;
	}

	m_Data.aOptimize.SetAt(iItem, Data);
	ModifyItem_AssignVehl(iItem, Data);
}

void CCMMvldItemRusDlg::OnCmdBtnDelete_AssignVehl() 
{
	int iItem = m_AssignVehlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	m_Data.aOptimize.RemoveAt(iItem);
	m_AssignVehlList.DeleteItem(iItem);

	int nCount = m_AssignVehlList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_AssignVehlList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvldItemRusDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdBtnModify();	
	*pResult = 0;
}