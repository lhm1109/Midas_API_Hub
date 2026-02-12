// CMMvldItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\VehlDef.h"

#include "CMMvldSubItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 8
#define COLCOUNT_ASSIGNVEHL 2

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemDlg dialog


union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key  : 31;
	} typekey;
};

CCMMvldItemDlg::CCMMvldItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_nLaneFactorType = 1;
	m_bInitDlg = FALSE;
	//{{AFX_DATA_INIT(CCMMvldItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aCombOption.RemoveAll();
	m_aCombOption.Add(IDC_CMD_RADIO_COMBINED);
	m_aCombOption.Add(IDC_CMD_RADIO_INDEPENDENT);

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

	m_aMPF.RemoveAll();
	m_aMPF.Add(IDC_CMD_MPF_FRM);
	m_aMPF.Add(IDC_CMD_MPF_FRM_NUM_LANES);
	m_aMPF.Add(IDC_CMD_MPF_FRM_FACTOR);
	m_aMPF.Add(IDC_MULTI_STATIC1);
	m_aMPF.Add(IDC_CMD_SF1);
	m_aMPF.Add(IDC_MULTI_STATIC2);
	m_aMPF.Add(IDC_CMD_SF2);
	m_aMPF.Add(IDC_MULTI_STATIC3);
	m_aMPF.Add(IDC_CMD_SF3);
	m_aMPF.Add(IDC_MULTI_STATIC4);
	m_aMPF.Add(IDC_CMD_SF4);
	m_aMPF.Add(IDC_MULTI_STATIC5);
	m_aMPF.Add(IDC_CMD_SF5);
	m_aMPF.Add(IDC_MULTI_STATIC6);
	m_aMPF.Add(IDC_CMD_SF6);

	m_aFoldGroup.RemoveAll();
	m_aFoldGroup.Add(IDC_CMD_MPF_FRM_NUM_LANES);
	m_aFoldGroup.Add(IDC_CMD_MPF_FRM_FACTOR);
	m_aFoldGroup.Add(IDC_MULTI_STATIC1);
	m_aFoldGroup.Add(IDC_CMD_SF1);
	m_aFoldGroup.Add(IDC_MULTI_STATIC2);
	m_aFoldGroup.Add(IDC_CMD_SF2);
	m_aFoldGroup.Add(IDC_MULTI_STATIC3);
	m_aFoldGroup.Add(IDC_CMD_SF3);
	m_aFoldGroup.Add(IDC_MULTI_STATIC4);
	m_aFoldGroup.Add(IDC_CMD_SF4);
	m_aFoldGroup.Add(IDC_MULTI_STATIC5);
	m_aFoldGroup.Add(IDC_CMD_SF5);
	m_aFoldGroup.Add(IDC_MULTI_STATIC6);
	m_aFoldGroup.Add(IDC_CMD_SF6);

	m_aCtrlLCPV.RemoveAll();
	m_aCtrlLCPV.Add(IDC_CMD_PV_FRM);
	m_aCtrlLCPV.Add(IDC_CMD_PV1_TXT);
	m_aCtrlLCPV.Add(IDC_CMD_VEHICLE_CMB);
	m_aCtrlLCPV.Add(IDC_CMD_PV2_TXT);
	m_aCtrlLCPV.Add(IDC_CMD_REF_LANE_CMB);
	m_aCtrlLCPV.Add(IDC_CMD_PV3_TXT);
	m_aCtrlLCPV.Add(IDC_CMD_ECCEN_EDT);
	m_aCtrlLCPV.Add(IDC_CMD_ECCEN_UNT);
	m_aCtrlLCPV.Add(IDC_CMD_PV4_TXT);
	m_aCtrlLCPV.Add(IDC_CMD_SCALE_FACTOR_EDT);

	m_aKSRail.RemoveAll();
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_GRP);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_LANES_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_FACTOR_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_2LANE_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_2LANE_1_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_2LANE_1_EDT);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_2LANE_2_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_2LANE_2_EDT);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_1_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_1_EDT);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_2_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_2_EDT);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_3_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_3_EDT);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_4_STC);
	m_aKSRail.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_4_EDT);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDC_CMD_SLC_FRM);
	m_aEtc.Add(IDC_CMD_LOAD_EFFECT_STC);
	m_aEtc.Add(IDC_CMD_RADIO_COMBINED);
	m_aEtc.Add(IDC_CMD_RADIO_INDEPENDENT);
	m_aEtc.Add(IDC_CMD_LIST);
	m_aEtc.Add(IDC_CMD_BTN_ADD);
	m_aEtc.Add(IDC_CMD_BTN_MODIFY);
	m_aEtc.Add(IDC_CMD_BTN_DELETE);
	
	m_aCtrlBtn.RemoveAll();
	m_aCtrlBtn.Add(IDOK);
	m_aCtrlBtn.Add(IDCANCEL);
	m_aCtrlBtn.Add(IDC_CMD_APPLY);
}

void CCMMvldItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemDlg)
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR_EDT, m_edtScaleFactor);
	DDX_Control(pDX, IDC_CMD_ECCEN_UNT, m_untEccen);
	DDX_Control(pDX, IDC_CMD_ECCEN_EDT, m_edtEccen);
	DDX_Control(pDX, IDC_CMD_REF_LANE_CMB, m_cmbRefLane);
	DDX_Control(pDX, IDC_CMD_VEHICLE_CMB, m_cmbVehicle);
	DDX_Control(pDX, IDC_CMD_LCPV_CHK, m_chkLCPV);  
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDesc);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_SF6, m_wndSF6);
	DDX_Control(pDX, IDC_CMD_SF5, m_wndSF5);
	DDX_Control(pDX, IDC_CMD_SF4, m_wndSF4);
	DDX_Control(pDX, IDC_CMD_SF3, m_wndSF3);
	DDX_Control(pDX, IDC_CMD_SF2, m_wndSF2);
	DDX_Control(pDX, IDC_CMD_SF1, m_wndSF1);
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadCase);
	DDX_Radio  (pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO,         m_nLaneFactorType);
	DDX_Control(pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_2LANE_1_EDT, m_edt2LaneFactor1);
	DDX_Control(pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_2LANE_2_EDT, m_edt2LaneFactor2);
	DDX_Control(pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_1_EDT, m_edt3LaneFactor1);
	DDX_Control(pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_2_EDT, m_edt3LaneFactor2);
	DDX_Control(pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_3_EDT, m_edt3LaneFactor3);
	DDX_Control(pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_4_EDT, m_edt3LaneFactor4);

	// Optm
	DDX_Control(pDX, IDC_CMD_MVLD_OPTIM_CHK,            m_chkAutoOptim);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_EDT,    m_edtMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_UNT,    m_untMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB,             m_cmbOptimLane);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_NUM_VEHL_EDT,     m_edtMinNumVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_MAX_NUM_VEHL_EDT,     m_edtMaxNumVehl);
	DDX_Radio  (pDX, IDC_CMD_RADIO_COMBINED2,           m_nCombOption2);
	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB,         m_cmbSelVehl);  
	DDX_Control(pDX, IDC_CMD_ASSIGN_VEHL_LIST,          m_AssignVehlList);
	DDX_Control(pDX, IDC_CMD_MVLD_SCALE_FACTOR_EDT,     m_edtScaleFactorOptim);

	DDX_Control(pDX, IDC_CMD_MPF_FRM,  m_chkLoadFactor);
	
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldItemDlg::Data2Dlg()
{
	m_wndLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.SetWindowText(m_Data.Description);
	CString csVal;
	csVal.Format(_T("%g"), m_Data.dScaleFactor[0]);
	m_wndSF1.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[1]);
	m_wndSF2.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[2]);
	m_wndSF3.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[3]);
	m_wndSF4.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[4]);
	m_wndSF5.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[5]);
	m_wndSF6.SetWindowText(csVal);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCombOption, m_Data.nCombOption);

	if (m_Data.bLoadCaseForPermitLoad)
	{
		m_chkLCPV.SetCheck(1);
		m_Data.bLoadCaseForPermitLoad = TRUE;
		if (!CDlgUtil::CobxSetCurSelItemData(m_cmbVehicle, (DWORD&)m_Data.PermitVehicleK))
			m_Data.PermitVehicleK = 0;
		if (!CDlgUtil::CobxSetCurSelItemData(m_cmbRefLane, (DWORD&)m_Data.RefLaneK))
			m_Data.RefLaneK = 0;
		m_edtEccen.SetEditUnit(m_Data.dEccentricity);
		m_edtScaleFactor.SetEditUnit(m_Data.dPermitScaleFactor);
	}
	else 
	{
		m_chkLCPV.SetCheck(0);
		m_edtEccen.SetEditUnit(m_Data.dEccentricity);
		m_edtScaleFactor.SetEditUnit(m_Data.dPermitScaleFactor);
		MakeItemEx();
	}

	m_nLaneFactorType = m_Data.nLaneFactorType;
	m_edt2LaneFactor1.SetEditUnit(m_Data.d2LaneFactor1);
	m_edt2LaneFactor2.SetEditUnit(m_Data.d2LaneFactor2);
	m_edt3LaneFactor1.SetEditUnit(m_Data.d3LaneFactor1);
	m_edt3LaneFactor2.SetEditUnit(m_Data.d3LaneFactor2);
	m_edt3LaneFactor3.SetEditUnit(m_Data.d3LaneFactor3);
	m_edt3LaneFactor4.SetEditUnit(m_Data.d3LaneFactor4);

	Data2DlgOptm();

	UpdateData(FALSE);
	
	EnableCtrl();
	ShowHideCtrl();
	AlignControl();
	ResizeDialog();
}

void CCMMvldItemDlg::InitCombo()
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

void CCMMvldItemDlg::Data2DlgOptm()
{
	m_chkAutoOptim.SetCheck(m_Data.bAutoOptimize);
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
}

BOOL CCMMvldItemDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_wndLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_wndDesc.GetWindowText(m_Data.Description);
	if (m_chkLCPV.GetCheck() != 0)
	{
		m_Data.arSubLoadData.RemoveAll();
		m_Data.bLoadCaseForPermitLoad = TRUE;
		CDlgUtil::CobxGetItemDataByCurSel(m_cmbVehicle, (DWORD&)m_Data.PermitVehicleK);
		CDlgUtil::CobxGetItemDataByCurSel(m_cmbRefLane, (DWORD&)m_Data.RefLaneK);
		m_Data.dEccentricity = m_edtEccen.GetEditValue();
		m_Data.dPermitScaleFactor = m_edtScaleFactor.GetEditValue();
	}
	else
	{
		T_MVCD_D DataMvcd;
		if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		int nCodeType = DataMvcd.nCodeType;

		m_Data.bLoadCaseForPermitLoad = FALSE;
		CFormulaEdit::GetEditValue(&m_wndSF1, m_Data.dScaleFactor[0]);
		CFormulaEdit::GetEditValue(&m_wndSF2, m_Data.dScaleFactor[1]);
		CFormulaEdit::GetEditValue(&m_wndSF3, m_Data.dScaleFactor[2]);
		CFormulaEdit::GetEditValue(&m_wndSF4, m_Data.dScaleFactor[3]);
		CFormulaEdit::GetEditValue(&m_wndSF5, m_Data.dScaleFactor[4]);
		CFormulaEdit::GetEditValue(&m_wndSF6, m_Data.dScaleFactor[5]);
		CDlgUtil::CtrlRadioGetCheck(this, m_aCombOption, m_Data.nCombOption);

		if (nCodeType == D_MOVE_CODE_JAPAN_RAIL)
		{
			for (int i = 0; i < 6; i++)
			{
				m_Data.dScaleFactor[i] = 1.0;
			}
		}
	}

	m_Data.nLaneFactorType = m_nLaneFactorType;
	m_Data.d2LaneFactor1 = m_edt2LaneFactor1.GetEditValue();
	m_Data.d2LaneFactor2 = m_edt2LaneFactor2.GetEditValue();
	m_Data.d3LaneFactor1 = m_edt3LaneFactor1.GetEditValue();
	m_Data.d3LaneFactor2 = m_edt3LaneFactor2.GetEditValue();
	m_Data.d3LaneFactor3 = m_edt3LaneFactor3.GetEditValue();
	m_Data.d3LaneFactor4 = m_edt3LaneFactor4.GetEditValue();

	// m_Data.arSubLoadData는 Add/Modify/Delete시 갱신

	Dlg2DataOptm();

	return TRUE;
}

BOOL CCMMvldItemDlg::Dlg2DataOptm()
{
	m_Data.bAutoOptimize = m_chkAutoOptim.GetCheck();
	m_Data.dMinVehlDist = m_edtMinVehlDist.GetEditValue();
	m_Data.OptimizeLane = m_cmbOptimLane.GetItemData(m_cmbOptimLane.GetCurSel());
	m_Data.nMinNumVehicle = m_edtMinNumVehl.GetEditValue();
	m_Data.nMaxNumVehicle = m_edtMaxNumVehl.GetEditValue();
	if(m_Data.bAutoOptimize) m_Data.nCombOption = m_nCombOption2;

	return TRUE;
}

BOOL CCMMvldItemDlg::ApplyOrOK()
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

/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg implementation functions
void CCMMvldItemDlg::SetHeaderTitle()
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

void CCMMvldItemDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.arSubLoadData.GetSize();
	if(nItemCount == 0) return;

	CString str;
	int nCount;

	if(nItemCount)
	{
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
			InsertItem(nCount, m_Data.arSubLoadData[nCount]);
		}
	}
}

BOOL CCMMvldItemDlg::InsertItem(int nIndex, T_MVLD_BASE &Data)
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

BOOL CCMMvldItemDlg::DeleteItem(int nIndex, T_MVLD_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_List.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemDlg::ModifyItem(int nIndex, T_MVLD_BASE &Data)
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

CString CCMMvldItemDlg::DataToStr(int i, T_MVLD_BASE &Data)
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

CString CCMMvldItemDlg::GetLaneName(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLAN_D slan;
		m_pDoc->m_pAttrCtrl->GetSlan(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_MVCD_D DataMvcd;
		if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		int nCodeType = DataMvcd.nCodeType;
		if (nCodeType == D_MOVE_CODE_JAPAN_RAIL)
		{
			T_LLANjp_D llan;
			m_pDoc->m_pAttrCtrl->GetLlanjp(nLaneKey, llan);
			return llan.LineLaneName;
		}
		else
		{
			T_LLAN_D llan;
			m_pDoc->m_pAttrCtrl->GetLlan(nLaneKey, llan);
			return llan.LineLaneName;
		}
	}
	return _LS(IDS_WG_CMD__ADDD__Error);
}


BEGIN_MESSAGE_MAP(CCMMvldItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_LCPV_CHK, OnCmdLcpvChk)
	ON_BN_CLICKED(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO, OnCmdMvldLaneFactorTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVLD_ITEM_MPF_RDO, OnCmdMvldLaneFactorTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_OPTIM_CHK, OnCmdOptmChk)  
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_ADD_BTN, OnCmdBtnAdd_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_MOD_BTN, OnCmdBtnModify_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_DEL_BTN, OnCmdBtnDelete_AssignVehl)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemDlg message handlers

BOOL CCMMvldItemDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	m_chkLoadFactor.InitControl(this, CCMMvldItemDlg::IDD, IDC_CMD_MPF_FRM,TRUE,TRUE);
	m_chkLoadFactor.SetFoldCurHeight(TRUE);

	InitCtrlPos();

	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0 || m_pDoc->m_pAttrCtrl->GetCountSlanop() > 0) m_bSurface = TRUE;
	else m_bSurface = FALSE;

	// TODO: Add extra initialization here
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	int nCodeType = DataMvcd.nCodeType;
			
	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dMinVehlDist = M_InitValueCurUnit(1.0, KN, M, CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
		m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, nCodeType);		
	}

	InitCtrl();
	InitUnit();
	InitCombo();

	SetHeaderTitle();
	SetHeaderTitle_AssignVehl();
	Data2Dlg();
	MakeItemEx_AssignVehl();
	
	if(nCodeType == D_MOVE_CODE_AUSTRALIA)
	{
		GetDlgItem(IDC_CMD_MPF_FRM)->SetWindowText(_LS(IDS_CMD_MVLD_AUSTRALIA_ACCOMPANYING_LANE_FACTOR));
	}

	m_bInitDlg = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemDlg::SetHeaderTitle_AssignVehl()
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

void CCMMvldItemDlg::MakeItemEx_AssignVehl()
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

void CCMMvldItemDlg::InitCtrlPos()
{
	// LCPV
	CRect rRef, rToMove;
	GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_PV_FRM)->GetWindowRect(rToMove);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlLCPV, rRef.top - rToMove.top);

	// KSRail
	int nDistY = 0.;
	CArray<UINT, UINT> aMoveCtrl; 
	aMoveCtrl.RemoveAll();
	aMoveCtrl.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO);

	GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);

	aMoveCtrl.RemoveAll();
	aMoveCtrl.Add(IDC_CMD_ML_MVLD_ITEM_MPF_RDO);

	GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ML_MVLD_ITEM_MPF_RDO)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);

	GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aKSRail, nDistY);
}

void CCMMvldItemDlg::InitCtrl()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	int nCodeType = DataMvcd.nCodeType;

	// Vehicle : Permit Vehicle만
	m_cmbVehicle.ResetContent();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK;
	pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVHL_D MvhlD;
	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD);
		if ((MvhlD.nStandardCode == D_MVHL_AASHTO_LRFD || MvhlD.nStandardCode == D_MVHL_CANCSA_S600 ||
			MvhlD.nStandardCode == D_MVHL_AUSTRALIA_AS_5100_2 || MvhlD.nStandardCode == D_MVHL_BS_BD3701 ||
			MvhlD.nStandardCode == D_MVHL_PN_85S_10030_ROAD_BRG || MvhlD.nStandardCode == D_MVHL_PN_85S_10030_WALKWALY_BRG) &&
			MvhlD.nLoadType == 2) // Permit Load
			CDlgUtil::CobxAddItem(m_cmbVehicle, MvhlD.VehicleLoadName, aMvhlK[i]);
	}
	if (aMvhlK.GetSize() > 0) m_cmbVehicle.SetCurSel(0);

	// Ref. Lane
	m_cmbRefLane.ResetContent();
	if (pDoc->m_pAttrCtrl->GetCountSlan() > 0)
	{
		CArray<T_SLAN_K, T_SLAN_K> aSlanK;
		pDoc->m_pAttrCtrl->GetSlanKeyList(aSlanK);
		T_SLAN_D SlanD;
		for (int i = 0; i < aSlanK.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetSlan(aSlanK[i], SlanD);
			CDlgUtil::CobxAddItem(m_cmbRefLane, SlanD.LaneName, aSlanK[i]);
		}
		if (aSlanK.GetSize() > 0) m_cmbRefLane.SetCurSel(0);
	}
	else
	{
		CArray<UINT, UINT> aLlanK;
		if (nCodeType == D_MOVE_CODE_JAPAN_RAIL)
		{
			pDoc->m_pAttrCtrl->GetLlanjpKeyList(aLlanK);
			T_LLANjp_D LlanD;
			for (int i = 0; i < aLlanK.GetSize(); i++)
			{
				pDoc->m_pAttrCtrl->GetLlanjp(aLlanK[i], LlanD);
				CDlgUtil::CobxAddItem(m_cmbRefLane, LlanD.LineLaneName, aLlanK[i]);
			}
		}
		else
		{
			pDoc->m_pAttrCtrl->GetLlanKeyList(aLlanK);
			T_LLAN_D LlanD;
			for (int i = 0; i < aLlanK.GetSize(); i++)
			{
				pDoc->m_pAttrCtrl->GetLlan(aLlanK[i], LlanD);
				CDlgUtil::CobxAddItem(m_cmbRefLane, LlanD.LineLaneName, aLlanK[i]);
			}
		}
		if (aLlanK.GetSize() > 0) m_cmbRefLane.SetCurSel(0);
	}

	if(nCodeType == D_MOVE_CODE_KOREA)
	{
		GetDlgItem(IDC_CMD_MPF_FRM)->SetWindowText(_T(""));
		GetDlgItem(IDC_CMD_MPF_FRM)->ShowWindow(FALSE);

	}
}

void CCMMvldItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	
	m_chkLoadFactor.SetFoldState(FALSE);
	CDialogMove::OnOK();
}


void CCMMvldItemDlg::OnCancel()
{
	m_chkLoadFactor.SetFoldState(FALSE);
	CDialogMove::OnCancel();
}


void CCMMvldItemDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();	
}

void CCMMvldItemDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMMvldSubItemDlg dlg(this);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
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

BOOL CCMMvldItemDlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
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

void CCMMvldItemDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
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

void CCMMvldItemDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();	
	*pResult = 0;
}

void CCMMvldItemDlg::MyCtrlShowHideByRect(UINT nFrmID, BOOL bShow, BOOL bIncRectWnd)
{
	CArray<CWnd*, CWnd*> apTgtWnd;
	CWnd* pwnd ,*pTWnd;
	CWnd* pwndR;
	pwndR = GetDlgItem(nFrmID);
	if (bIncRectWnd) apTgtWnd.Add(pwndR);

	if(pwndR)
	{
		CRect RRect,TRect;
		pwndR->GetWindowRect(RRect);
		
		pwnd = pwndR;
		while(pwnd)
		{
			pTWnd = pwnd->GetNextWindow(GW_HWNDPREV);
			if(pTWnd)
				pwnd = pTWnd;
			else
				break;
		}

		UINT Tid;

		while(pwnd)
		{
			Tid = pwnd->GetDlgCtrlID();
			if(Tid != nFrmID)
			{
				pwnd->GetWindowRect(TRect);
				if(RRect.left  <= TRect.left  && RRect.top    <= TRect.top &&
					 RRect.right >= TRect.right && RRect.bottom >= TRect.bottom)
				{
					apTgtWnd.Add(pwnd);
					//IDCtrls.Add(Tid);
				}
			}
			pwnd = pwnd->GetNextWindow(GW_HWNDNEXT);
			if(!pwnd) break;
		}
	}
	int nCmd = SW_SHOW;
	if (!bShow) nCmd = SW_HIDE;
	for (int i = 0; i < apTgtWnd.GetSize(); i++)
		apTgtWnd[i]->ShowWindow(nCmd);
}

void CCMMvldItemDlg::EnableCtrl()
{
	//EnableCtrl()은 꼭 ShowHideCtrl()을 부르기 전에 불러야 함
	BOOL bAutoOptimEnable = FALSE;
	BOOL bAutoOptm = m_chkAutoOptim.GetCheck();
	BOOL bLCPV = m_chkLCPV.GetCheck();    

	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	int nCodeType = DataMvcd.nCodeType;
	if (nCodeType != D_MOVE_CODE_AASHTO_LRFD && nCodeType != D_MOVE_CODE_CANADA && nCodeType != D_MOVE_CODE_AUSTRALIA && nCodeType != D_MOVE_CODE_POLAND) 
	{
		m_chkLCPV.SetCheck(FALSE);
		GetDlgItem(IDC_CMD_LCPV_CHK)->EnableWindow(FALSE);
	}
	else
	{    
		GetDlgItem(IDC_CMD_LCPV_CHK)->EnableWindow(!bAutoOptm);
	}

#ifdef _CIVIL
	if(nCodeType == D_MOVE_CODE_AASHTO_STAN || nCodeType == D_MOVE_CODE_AASHTO_LRFD || 
		 nCodeType == D_MOVE_CODE_PENDOT      || nCodeType == D_MOVE_CODE_CANADA      ||
		 nCodeType == D_MOVE_CODE_AUSTRALIA)
	{
		bAutoOptimEnable = TRUE;
	}
	else
	{
		if ((CProduct::GetTestEnvValue(_T("Civil v860 Test")) == _T("yes")) && (nCodeType == D_MOVE_CODE_KOREA))
		{ 
			bAutoOptimEnable = TRUE;
		}
		else
		{
			m_chkAutoOptim.SetCheck(FALSE);
			bAutoOptimEnable = FALSE;
		} 
	}
#endif // _CIVIL

	
	
	GetDlgItem(IDC_CMD_MVLD_OPTIM_CHK)->EnableWindow(!bLCPV && bAutoOptimEnable);
}

void CCMMvldItemDlg::ShowHideCtrl()
{
	//ShowHideCtrl()은 꼭 EnableCtrl를 부른 후에 불러야 함
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	int nCodeType = DataMvcd.nCodeType;
	BOOL bLCPV = m_chkLCPV.GetCheck() && m_chkLCPV.IsWindowEnabled();;
	BOOL bAutoOptim = m_chkAutoOptim.GetCheck();
	
	if (nCodeType == D_MOVE_CODE_JAPAN_RAIL)
	{
		m_chkLCPV.SetCheck(FALSE);
		GetDlgItem(IDC_CMD_LCPV_CHK)->ShowWindow(FALSE);

		m_chkAutoOptim.SetCheck(FALSE);
		GetDlgItem(IDC_CMD_MVLD_OPTIM_CHK)->ShowWindow(FALSE);
	}

	// KS Rail + m_aMPF
	GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_ML_MVLD_ITEM_MPF_RDO)->ShowWindow(FALSE);

	if(nCodeType == D_MOVE_CODE_KOREA)
	{
		CDlgUtil::CtrlShowHide(this, m_aKSRail, m_nLaneFactorType == 0);
		CDlgUtil::CtrlShowHide(this, m_aMPF, m_nLaneFactorType != 0 && !bLCPV);

		GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_ML_MVLD_ITEM_MPF_RDO)->ShowWindow(TRUE);

		GetDlgItem(IDC_CMD_MPF_FRM)->SetWindowText(_T(""));
		GetDlgItem(IDC_CMD_MPF_FRM)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_MPF_FRM2)->ShowWindow(FALSE);

	}
	else if(nCodeType == D_MOVE_CODE_POLAND)
	{
		CDlgUtil::CtrlShowHide(this, m_aKSRail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMPF, FALSE);
	}
	else if (nCodeType == D_MOVE_CODE_JAPAN_RAIL)
	{
		CDlgUtil::CtrlShowHide(this, m_aKSRail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMPF, FALSE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aKSRail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMPF, !bLCPV);
	}
	
	if (!bLCPV)
	{
		// Fold 상태 설정
		if(nCodeType == D_MOVE_CODE_KOREA)
		{
			// skip
		}
		else if(nCodeType == D_MOVE_CODE_POLAND)
		{
			CDlgUtil::CtrlShowHide(this, m_aFoldGroup, FALSE);
		}
		else if (nCodeType == D_MOVE_CODE_JAPAN_RAIL)
		{
			CDlgUtil::CtrlShowHide(this, m_aFoldGroup, FALSE);
		}
		else
		{
			BOOL bFold = m_chkLoadFactor.GetFoldState();
			CDlgUtil::CtrlShowHide(this, m_aFoldGroup, !bFold);
		}


		T_MVCD_D DataMvcd;
		if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		int nCodeType = DataMvcd.nCodeType;

		BOOL bChekc = FALSE;
		int nMoveIDC = IDC_MULTI_STATIC4;
		if(nCodeType == D_MOVE_CODE_CANADA)
		{
			GetDlgItem(IDC_MULTI_STATIC4)->SetWindowText(_T("4"));
			//GetDlgItem(IDC_MULTI_STATIC5)->ShowWindow(TRUE);
			//GetDlgItem(IDC_MULTI_STATIC6)->ShowWindow(TRUE);
			//GetDlgItem(IDC_CMD_SF5)->ShowWindow(TRUE);
			//GetDlgItem(IDC_CMD_SF6)->ShowWindow(TRUE);
		}
		else if(nCodeType == D_MOVE_CODE_AUSTRALIA)
		{
			GetDlgItem(IDC_MULTI_STATIC3)->SetWindowText(_T("3 or more"));
			GetDlgItem(IDC_MULTI_STATIC4)->ShowWindow(FALSE);
			GetDlgItem(IDC_MULTI_STATIC5)->ShowWindow(FALSE);
			GetDlgItem(IDC_MULTI_STATIC6)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_SF4)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_SF5)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_SF6)->ShowWindow(FALSE);

			bChekc = TRUE;
			nMoveIDC = IDC_MULTI_STATIC4;
		}
		else
		{
			GetDlgItem(IDC_MULTI_STATIC4)->SetWindowText(_T("> 3"));
			GetDlgItem(IDC_MULTI_STATIC5)->ShowWindow(FALSE);
			GetDlgItem(IDC_MULTI_STATIC6)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_SF5)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_SF6)->ShowWindow(FALSE);

			bChekc = TRUE;
			nMoveIDC = IDC_MULTI_STATIC5;
		}

		BOOL bFold = m_chkLoadFactor.GetFoldState();
		if(!bFold && bChekc)
		{
			CRect rRef, rToMove;
			int nDistY = 0.;
			GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
			GetDlgItem(nMoveIDC       )->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top - globalUtils.ScaleByDPI(8);

			CRect FrmRect;
			GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(FrmRect);
			FrmRect.bottom -= nDistY;
			ScreenToClient(FrmRect);
			GetDlgItem(IDC_CMD_MPF_FRM)->MoveWindow(FrmRect);
		}
	}

	// PermitVehicle
	CDlgUtil::CtrlShowHide(this, m_aCtrlLCPV, bLCPV && !bAutoOptim); 
	
	// Optim
	CDlgUtil::CtrlShowHide(this, m_aOptimCtrl, bAutoOptim);
	CDlgUtil::CtrlShowHide(this, m_aEtc, !bLCPV && !bAutoOptim);

	GetDlgItem(IDOK)->ShowWindow(TRUE);
	GetDlgItem(IDCANCEL)->ShowWindow(TRUE);
	GetDlgItem(IDC_CMD_APPLY)->ShowWindow(TRUE);
}

void CCMMvldItemDlg::ResizeDialog()
{
	BOOL bLCPV = m_chkLCPV.GetCheck();
	BOOL bOptm = m_chkAutoOptim.GetCheck();
	if(bLCPV && bOptm)
	{
		ASSERT(0);
		return;
	}
	CRect rSLC, rPV, rOP, rToMove;
	GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rSLC);
	GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rOP);  
	GetDlgItem(IDC_CMD_PV_FRM)->GetWindowRect(rPV);
	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);

	int nDistY;
	if (bLCPV)      nDistY = (rPV.bottom + globalUtils.ScaleByDPI(16)) - rToMove.top;
	else if(bOptm)  nDistY = (rOP.bottom + globalUtils.ScaleByDPI(16)) - rToMove.top;
	else            nDistY = (rSLC.bottom + globalUtils.ScaleByDPI(16)) - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlBtn, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);

	MoveWindow(r);
	RedrawWindow();
}

void CCMMvldItemDlg::OnCmdLcpvChk() 
{
	// TODO: Add your control notification handler code here
	EnableCtrl();
	ShowHideCtrl();
	AlignControl();
	ResizeDialog();
}

void CCMMvldItemDlg::OnCmdOptmChk() 
{
	EnableCtrl();
	ShowHideCtrl();
	AlignControl();
	ResizeDialog();
}

void CCMMvldItemDlg::OnCmdMvldLaneFactorTypeRdo() 
{
	UpdateData(TRUE);

	ShowHideCtrl();
	AlignControl();
	ResizeDialog();
}

LRESULT CCMMvldItemDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	if(!m_bInitDlg) return 0L;

	InitCtrlPos();
	//InitCtrl();

	EnableCtrl();
	ShowHideCtrl();
	AlignControl();
	ResizeDialog();

	return 0L;
}


void CCMMvldItemDlg::AlignControl()
{
	CRect rRef, rToMove;
	int nDistY = 0.;
	int nDistX = 0.;
	CArray<UINT, UINT> aMoveCtrl;

	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	int nCodeType = DataMvcd.nCodeType;

	// KS Rail
	if(nCodeType == D_MOVE_CODE_KOREA)
	{
		if(m_nLaneFactorType == 0) // KS Rail
		{
			GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_GRP)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aKSRail, nDistY);      

			aMoveCtrl.RemoveAll();
			aMoveCtrl.Add(IDC_CMD_ML_MVLD_ITEM_MPF_RDO);

			GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVLD_ITEM_MPF_RDO)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);      
			
			GetDlgItem(IDC_CMD_ML_MVLD_ITEM_MPF_RDO)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);      
		}
		else
		{
			aMoveCtrl.RemoveAll();
			aMoveCtrl.Add(IDC_CMD_ML_MVLD_ITEM_MPF_RDO);

			GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVLD_ITEM_MPF_RDO)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);    

			GetDlgItem(IDC_CMD_ML_MVLD_ITEM_MPF_RDO)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aMPF, nDistY);      

			GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

			//
			CArray<UINT, UINT> m_aFRM2; m_aFRM2.RemoveAll();
			m_aFRM2.Add(IDC_CMD_MPF_FRM2);
			GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MPF_FRM2)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			CDlgUtil::CtrlMoveDistX(this, m_aFRM2, nDistX);      
		}

		// Optim    
		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);    
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aOptimCtrl, nDistY);
	}
	else if(nCodeType == D_MOVE_CODE_POLAND)
	{
		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

		// Optim
		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);    
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aOptimCtrl, nDistY);
	}
	else if (nCodeType == D_MOVE_CODE_JAPAN_RAIL)
	{
// 		GetDlgItem(IDC_CMD_LCPV_CHK)->GetWindowRect(rRef);
// 		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rToMove);
// 		nDistY = rRef.top - rToMove.top;
// 		CDlgUtil::CtrlMoveDistY(this, m_aMPF, nDistY);

		GetDlgItem(IDC_CMD_LCPV_CHK)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top + 8;
		CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

// 		// Optim
// 		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
// 		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
// 		nDistY = rRef.bottom - rToMove.top + 8;
// 		CDlgUtil::CtrlMoveDistY(this, m_aOptimCtrl, nDistY);

	}
	else
	{
		GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_RDO)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aMPF, nDistY);    

		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

		// Optim
		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);    
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aOptimCtrl, nDistY);
	}

	// TextChange
	if(nCodeType == D_MOVE_CODE_AUSTRALIA)
	{
		GetDlgItem(IDC_CMD_MPF_FRM)->SetWindowText(_LS(IDS_CMD_MVLD_AUSTRALIA_ACCOMPANYING_LANE_FACTOR));
	}
	else
	{
		GetDlgItem(IDC_CMD_MPF_FRM)->SetWindowText(_LS(IDS_CMD_ML_MVLD_ITEM_MULTIPLE_PRESENCE_FACTOR));
	}
}

void CCMMvldItemDlg::InitUnit()
{
	m_edtEccen.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dEccentricity);
	m_untEccen.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dEccentricity);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);

	InitUnitOptm();
}

void CCMMvldItemDlg::InitUnitOptm()
{
	m_edtMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_untMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_edtMinNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtMaxNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactorOptim.SetUnitType(D_UNITSYS_NONE);
}


void CCMMvldItemDlg::OnCmdBtnAdd_AssignVehl() 
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

void CCMMvldItemDlg::OnCmdBtnModify_AssignVehl() 
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

void CCMMvldItemDlg::OnCmdBtnDelete_AssignVehl() 
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

BOOL CCMMvldItemDlg::InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
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

BOOL CCMMvldItemDlg::DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_AssignVehlList.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemDlg::ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
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

CString CCMMvldItemDlg::DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data)
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
