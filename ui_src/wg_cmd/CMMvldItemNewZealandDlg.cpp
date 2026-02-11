// CMMvldItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemNewZealandDlg.h"

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
// CCMMvldItemNewZealandDlg dialog
union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key : 31;
	} typekey;
};
CCMMvldItemNewZealandDlg::CCMMvldItemNewZealandDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemNewZealandDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_bInitDlg = FALSE;

	m_aLaneList.RemoveAll();

	//{{AFX_DATA_INIT(CCMMvldItemNewZealandDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aCombOption.RemoveAll();
	m_aCombOption.Add(IDC_CMD_RADIO_COMBINED);
	m_aCombOption.Add(IDC_CMD_RADIO_INDEPENDENT);

	m_aCombOption2.RemoveAll();
	m_aCombOption2.Add(IDC_CMD_RADIO_COMBINED2);
	m_aCombOption2.Add(IDC_CMD_RADIO_INDEPENDENT2);

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

	m_aMPFCtrl.RemoveAll();
	m_aMPFCtrl.Add(IDC_CMD_MPF_FRM_NUM_LANES);
	m_aMPFCtrl.Add(IDC_CMD_MPF_FRM_FACTOR);
	m_aMPFCtrl.Add(IDC_MULTI_STATIC1);
	m_aMPFCtrl.Add(IDC_CMD_SF1);
	m_aMPFCtrl.Add(IDC_MULTI_STATIC2);
	m_aMPFCtrl.Add(IDC_CMD_SF2);
	m_aMPFCtrl.Add(IDC_MULTI_STATIC3);
	m_aMPFCtrl.Add(IDC_CMD_SF3);
	m_aMPFCtrl.Add(IDC_MULTI_STATIC4);
	m_aMPFCtrl.Add(IDC_CMD_SF4);
	m_aMPFCtrl.Add(IDC_MULTI_STATIC5);
	m_aMPFCtrl.Add(IDC_CMD_SF5);
	m_aMPFCtrl.Add(IDC_MULTI_STATIC6);
	m_aMPFCtrl.Add(IDC_CMD_SF6);

	m_aMPF.RemoveAll();
	m_aMPF.Add(IDC_CMD_MPF_FRM);
	m_aMPF.Append(m_aMPFCtrl);

	m_aAccompanyingCtrl.RemoveAll();
	m_aAccompanyingCtrl.Add(IDC_CMD_MPF_FRM_NUM_LANES);
	m_aAccompanyingCtrl.Add(IDC_CMD_MPF_FRM_FACTOR);
	m_aAccompanyingCtrl.Add(IDC_MULTI_STATIC1);
	m_aAccompanyingCtrl.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_1_EDT);
	m_aAccompanyingCtrl.Add(IDC_MULTI_STATIC2);
	m_aAccompanyingCtrl.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_2_EDT);
	m_aAccompanyingCtrl.Add(IDC_MULTI_STATIC3);
	m_aAccompanyingCtrl.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_3_EDT);

	m_aAccompanying.RemoveAll();
	m_aAccompanying.Add(IDC_CMD_MPF_FRM2);
	m_aAccompanying.Append(m_aAccompanyingCtrl);

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

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDC_CMD_SLC_FRM);
	m_aEtc.Add(IDC_CMD_LOAD_EFFECT_STC);
	m_aEtc.Add(IDC_CMD_RADIO_COMBINED);
	m_aEtc.Add(IDC_CMD_RADIO_INDEPENDENT);
	m_aEtc.Add(IDC_CMD_LIST);
	m_aEtc.Add(IDC_CMD_BTN_ADD);
	m_aEtc.Add(IDC_CMD_BTN_MODIFY);
	m_aEtc.Add(IDC_CMD_BTN_DELETE);

	m_aHOLaneCtrl.RemoveAll();
	m_aHOLaneCtrl.Add(IDC_CMD_LANES_GRUP);
	m_aHOLaneCtrl.Add(IDC_CMD_LIST_TXT);
	m_aHOLaneCtrl.Add(IDC_CMD_SELECTED_TXT);
	m_aHOLaneCtrl.Add(IDC_CMD_STRAD_LANE_STC);
	m_aHOLaneCtrl.Add(IDC_CMD_UNSEL_LIST);
	m_aHOLaneCtrl.Add(IDC_CMD_SEL_LIST);
	m_aHOLaneCtrl.Add(IDC_CMD_STRAD_LANE_LST);
	m_aHOLaneCtrl.Add(IDC_CMD_SUB_BTN_ADD);
	m_aHOLaneCtrl.Add(IDC_CMD_SUB_BTN_DEL);
	m_aHOLaneCtrl.Add(IDC_CMD_SUB_STRAD_ADD_BTN);
	m_aHOLaneCtrl.Add(IDC_CMD_SUB_STRAD_DEL_BTN);

	m_aCtrlBtn.RemoveAll();
	m_aCtrlBtn.Add(IDOK);
	m_aCtrlBtn.Add(IDCANCEL);
	m_aCtrlBtn.Add(IDC_CMD_APPLY);

	m_aHOVehicle.RemoveAll();
	m_aHOVehicle.Add(IDC_CMD_LOAD_CASE_TXT);
	m_aHOVehicle.Add(IDC_ST_LOAD_TXT);
	m_aHOVehicle.Add(IDC_ST_LOAD_CBX);
	m_aHOVehicle.Add(IDC_CMD_MVLD_SEL_VEHL_STC2);
	m_aHOVehicle.Add(IDC_CMD_MVLD_SEL_VEHL_CMB2);

	m_aLoadModel.RemoveAll();
	m_aLoadModel.Add(IDC_SEL_LOAD_MODEL_GRP);
	m_aLoadModel.Add(IDC_GENERAL_LOAD_RDO);
	m_aLoadModel.Add(IDC_TRAFFIC_LOAD_RDO);
	m_aLoadModel.Add(IDC_RATING_POSTING_RDO);
	m_aLoadModel.Add(IDC_FATIGUE_LOAD_RDO);

	m_aLoadModelRdo.RemoveAll();
	m_aLoadModelRdo.Add(IDC_GENERAL_LOAD_RDO);
	m_aLoadModelRdo.Add(IDC_TRAFFIC_LOAD_RDO);
	m_aLoadModelRdo.Add(IDC_RATING_POSTING_RDO);
	m_aLoadModelRdo.Add(IDC_FATIGUE_LOAD_RDO);
}

void CCMMvldItemNewZealandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemNewZealandDlg)
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
	DDX_Control(pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_3_EDT, m_wndAccompanying3);
	DDX_Control(pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_2_EDT, m_wndAccompanying2);
	DDX_Control(pDX, IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_1_EDT, m_wndAccompanying1);
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadCase);
	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB2, m_cmbSelHOVehl);
	DDX_Control(pDX, IDC_ST_LOAD_CBX, m_cmbSelHNVehl);

	DDX_Control(pDX, IDC_CMD_UNSEL_LIST, m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST, m_wndSelList);
	DDX_Control(pDX, IDC_CMD_STRAD_LANE_LST, m_wndHOSelList);

	// Optm
	DDX_Control(pDX, IDC_CMD_MVLD_OPTIM_CHK, m_chkAutoOptim);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_EDT, m_edtMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_UNT, m_untMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB, m_cmbOptimLane);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_NUM_VEHL_EDT, m_edtMinNumVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_MAX_NUM_VEHL_EDT, m_edtMaxNumVehl);
	DDX_Radio  (pDX, IDC_CMD_RADIO_COMBINED2, m_nCombOption2);
	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB, m_cmbSelVehl);
	DDX_Control(pDX, IDC_CMD_ASSIGN_VEHL_LIST, m_AssignVehlList);
	DDX_Control(pDX, IDC_CMD_MVLD_SCALE_FACTOR_EDT, m_edtScaleFactorOptim);

	DDX_Control(pDX, IDC_SEL_LOAD_MODEL_GRP, m_chkLoadModel);
	DDX_Control(pDX, IDC_CMD_MPF_FRM, m_chkMultiLaneFactor);
	DDX_Control(pDX, IDC_CMD_MPF_FRM2, m_chkAccompanying);

	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldItemNewZealandDlg::Data2Dlg()
{
	m_wndLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.SetWindowText(m_Data.Description);
	CString csVal;
	csVal.Format(_T("%g"), m_Data.dMultiLaneFactor[0]);
	m_wndSF1.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dMultiLaneFactor[1]);
	m_wndSF2.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dMultiLaneFactor[2]);
	m_wndSF3.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dMultiLaneFactor[3]);
	m_wndSF4.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dMultiLaneFactor[4]);
	m_wndSF5.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dMultiLaneFactor[5]);
	m_wndSF6.SetWindowText(csVal);

	csVal.Format(_T("%g"), m_Data.dScaleFactor[0]);
	m_wndAccompanying1.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[1]);
	m_wndAccompanying2.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dScaleFactor[2]);
	m_wndAccompanying3.SetWindowText(csVal);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCombOption, m_Data.nCombOption);
	CDlgUtil::CtrlRadioSetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);

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
		if (m_cmbSelHOVehl.GetCount() > 0 && m_Data.Vehicle1 != 0 && m_Data.nLoadModel == 1)
		{
			T_MVLD_TYPE_KEY TypeKey;
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = m_Data.Vehicle1;
			CDlgUtil::CobxSetCurSelItemData(m_cmbSelHOVehl, (DWORD&)TypeKey.value);
		}
		else
		{
			m_Data.Vehicle1 = 0;
		}

		if (m_cmbSelHNVehl.GetCount() > 0 && m_Data.Vehicle2 != 0 && m_Data.nLoadModel == 1)
		{
			T_MVLD_TYPE_KEY TypeKey;
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = m_Data.Vehicle2;
			CDlgUtil::CobxSetCurSelItemData(m_cmbSelHNVehl, (DWORD&)TypeKey.value);
		}
		else
		{
			m_Data.Vehicle2 = 0;
		}

		m_chkLCPV.SetCheck(0);
		m_edtEccen.SetEditUnit(m_Data.dEccentricity);
		m_edtScaleFactor.SetEditUnit(m_Data.dPermitScaleFactor);
		MakeItemEx();
	}

	Data2DlgOptm();
	Data2Dlg_LaneListByVehicleIndex();

	UpdateData(FALSE);

	EnableCtrl();
	ShowHideCtrl();
	AlignControl();
	ResizeDialog();
}

void CCMMvldItemNewZealandDlg::UpdateCombo()
{
	// Selected Vehicle
	m_cmbSelVehl.ResetContent();
	int nIndex = 0;
	T_MVHL_D MvhlD; MvhlD.Initialize();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK; aMvhlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVLD_TYPE_KEY TypeKey;

	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;
		if (m_pDoc->m_pAttrCtrl->IsPermitVehicle(MvhlD)) continue;

		if (MvhlD.bStandard)
		{
			if ((MvhlD.VehicleTypeName == _T("HO (overload) loading (a)") || MvhlD.VehicleTypeName == _T("HO (overload) loading (b)")) && (m_Data.nLoadModel != 0 && m_Data.nLoadModel != 2)) continue;
			if (m_Data.nLoadModel == 1 && MvhlD.nStandardCode != D_MVHL_TRAFFIC_LOAD_NZ) continue;
			else if (m_Data.nLoadModel == 2)
			{
				if (MvhlD.VehicleTypeName != _T("HO (overload) loading (a)") && 
					MvhlD.VehicleTypeName != _T("HO (overload) loading (b)") &&
					MvhlD.nStandardCode != D_MVHL_GENERAL_ACCESS_NZ && MvhlD.nStandardCode != D_MVHL_50MAX_NZ &&
					MvhlD.nStandardCode != D_MVHL_HPMV_NZ && MvhlD.nStandardCode != D_MVHL_DECK_EVALUATION)
				{
					continue;
				}
			}
			else if (m_Data.nLoadModel == 3 && MvhlD.nStandardCode != D_MVHL_FATIGUE_NZ) continue;
		}

		nIndex = m_cmbSelVehl.AddString(MvhlD.VehicleLoadName);
		TypeKey.typekey.type = 1;
		TypeKey.typekey.key = aMvhlK[i];
		m_cmbSelVehl.SetItemData(nIndex, TypeKey.value);
	}
	m_cmbSelVehl.SetCurSel(0);
}

void CCMMvldItemNewZealandDlg::InitCombo()
{
	// Loaded Lane
	m_cmbOptimLane.ResetContent();

	if (m_bSurface)
	{
		T_SLANop_D SlanD; SlanD.Initialize();
		CArray<T_SLANop_K, T_SLANop_K> aSlanK; aSlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetSlanopKeyList(aSlanK);

		for (int i = 0; i < aSlanK.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetSlanop(aSlanK[i], SlanD)) continue;
			m_cmbOptimLane.SetItemData(m_cmbOptimLane.AddString(SlanD.LaneName), aSlanK[i]);
		}
	}
	else
	{
		T_LLANop_D LlanD; LlanD.Initialize();
		CArray<T_LLANop_K, T_LLANop_K> aLlanK; aLlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetLlanopKeyList(aLlanK);

		for (int i = 0; i < aLlanK.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetLlanop(aLlanK[i], LlanD)) continue;
			m_cmbOptimLane.SetItemData(m_cmbOptimLane.AddString(LlanD.LineLaneName), aLlanK[i]);
		}
	}

	m_cmbOptimLane.SetCurSel(0);

	// Selected Vehicle
	m_cmbSelVehl.ResetContent();
	m_cmbSelHOVehl.ResetContent();

	int nIndex = 0, nIndexHN=0;
	T_MVHL_D MvhlD; MvhlD.Initialize();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK; aMvhlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVLD_TYPE_KEY TypeKey;

	nIndex  = m_cmbSelHOVehl.AddString(_T("None"));
	nIndexHN = m_cmbSelHNVehl.AddString(_T("None"));

	TypeKey.value = 0;
	m_cmbSelHOVehl.SetItemData(nIndex, TypeKey.value);
	m_cmbSelHNVehl.SetItemData(nIndexHN, TypeKey.value);

	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;
		if (m_pDoc->m_pAttrCtrl->IsPermitVehicle(MvhlD)) continue;

		if (MvhlD.bStandard && (MvhlD.VehicleTypeName == _T("HO (overload) loading (a)") || MvhlD.VehicleTypeName == _T("HO (overload) loading (b)")) )
		{
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = aMvhlK[i];
			nIndex = m_cmbSelHOVehl.AddString(_T("VL:") + MvhlD.VehicleLoadName);
			m_cmbSelHOVehl.SetItemData(nIndex, TypeKey.value);
		}

		if (MvhlD.bStandard && MvhlD.VehicleTypeName == _T("HN (normal) loading"))
		{
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = aMvhlK[i];
			nIndexHN = m_cmbSelHNVehl.AddString(_T("VL:") + MvhlD.VehicleLoadName);
			m_cmbSelHNVehl.SetItemData(nIndexHN, TypeKey.value);
		}

		if (MvhlD.bStandard)
		{
			if ((MvhlD.VehicleTypeName == _T("HO (overload) loading (a)") || MvhlD.VehicleTypeName == _T("HO (overload) loading (b)")) && (m_Data.nLoadModel != 0 && m_Data.nLoadModel != 2)) continue;
			if (m_Data.nLoadModel == 1 && MvhlD.nStandardCode != D_MVHL_TRAFFIC_LOAD_NZ) continue;
			else if (m_Data.nLoadModel == 2)
			{
				if (MvhlD.VehicleTypeName != _T("HO (overload) loading (a)") && MvhlD.VehicleTypeName != _T("HO (overload) loading (b)") &&
					MvhlD.nStandardCode != D_MVHL_GENERAL_ACCESS_NZ && MvhlD.nStandardCode != D_MVHL_50MAX_NZ &&
					MvhlD.nStandardCode != D_MVHL_HPMV_NZ && MvhlD.nStandardCode != D_MVHL_DECK_EVALUATION)
				{
					continue;
				}
			}
			else if (m_Data.nLoadModel == 3 && MvhlD.nStandardCode != D_MVHL_FATIGUE_NZ) continue;
		}

		nIndex = m_cmbSelVehl.AddString(MvhlD.VehicleLoadName);
		TypeKey.typekey.type = 1;
		TypeKey.typekey.key = aMvhlK[i];
		m_cmbSelVehl.SetItemData(nIndex, TypeKey.value);
	}
	m_cmbSelVehl.SetCurSel(0);
	m_cmbSelHOVehl.SetCurSel(0);
	m_cmbSelHNVehl.SetCurSel(0);
}

void CCMMvldItemNewZealandDlg::Data2DlgOptm()
{
	m_chkAutoOptim.SetCheck(m_Data.bAutoOptimize);
	m_edtMinVehlDist.SetEditUnit(m_Data.dMinVehlDist);

	int nSelect = 0;
	int i = 0;
	for (i = 0; i < m_cmbOptimLane.GetCount(); i++)
	{
		if (m_Data.OptimizeLane != m_cmbOptimLane.GetItemData(i)) continue;
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

BOOL CCMMvldItemNewZealandDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_wndLoadCase.GetWindowText(m_Data.LoadCaseName);
	if (m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_wndDesc.GetWindowText(m_Data.Description);
	if (m_chkLCPV.GetCheck() != 0)
	{
		m_Data.nLoadModel = 0;
		m_Data.arSubLoadData.RemoveAll();
		m_Data.bLoadCaseForPermitLoad = TRUE;
		CDlgUtil::CobxGetItemDataByCurSel(m_cmbVehicle, (DWORD&)m_Data.PermitVehicleK);
		CDlgUtil::CobxGetItemDataByCurSel(m_cmbRefLane, (DWORD&)m_Data.RefLaneK);
		m_Data.dEccentricity = m_edtEccen.GetEditValue();
		m_Data.dPermitScaleFactor = m_edtScaleFactor.GetEditValue();
	}
	else
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);

		if (m_Data.nLoadModel == 1)
		{
			T_MVLD_TYPE_KEY TypeKey;
			CDlgUtil::CobxGetItemDataByCurSel(m_cmbSelHOVehl, (DWORD&)TypeKey.value);
			m_Data.Vehicle1 = TypeKey.typekey.key;

			CDlgUtil::CobxGetItemDataByCurSel(m_cmbSelHNVehl, (DWORD&)TypeKey.value);
			m_Data.Vehicle2 = TypeKey.typekey.key;

			m_Data.aNALanes.RemoveAll();
			m_Data.aNCLanes.RemoveAll();

			for (int i = 0; i < m_aLaneList.GetSize(); ++i)
			{
				if (m_aSelFlag[i])
				{
					m_Data.aNALanes.Add(m_aLaneList[i]);
				}
				if (m_aHOSelFlag[i])
				{
					m_Data.aNCLanes.Add(m_aLaneList[i]);
				}
			}

		}
		else
		{
			m_Data.Vehicle1 = 0;
			m_Data.Vehicle2 = 0;
		}

		m_Data.bLoadCaseForPermitLoad = FALSE;
		if (m_Data.nLoadModel == 2)
		{
			CFormulaEdit::GetEditValue(&m_wndAccompanying1, m_Data.dScaleFactor[0]);
			CFormulaEdit::GetEditValue(&m_wndAccompanying2, m_Data.dScaleFactor[1]);
			CFormulaEdit::GetEditValue(&m_wndAccompanying3, m_Data.dScaleFactor[2]);
		}
		else
		{
			CFormulaEdit::GetEditValue(&m_wndSF1, m_Data.dMultiLaneFactor[0]);
			CFormulaEdit::GetEditValue(&m_wndSF2, m_Data.dMultiLaneFactor[1]);
			CFormulaEdit::GetEditValue(&m_wndSF3, m_Data.dMultiLaneFactor[2]);
			CFormulaEdit::GetEditValue(&m_wndSF4, m_Data.dMultiLaneFactor[3]);
			CFormulaEdit::GetEditValue(&m_wndSF5, m_Data.dMultiLaneFactor[4]);
			CFormulaEdit::GetEditValue(&m_wndSF6, m_Data.dMultiLaneFactor[5]);
		}
		CDlgUtil::CtrlRadioGetCheck(this, m_aCombOption, m_Data.nCombOption);
	}

	Dlg2DataOptm();

	return TRUE;
}

BOOL CCMMvldItemNewZealandDlg::Dlg2DataOptm()
{
	m_Data.bAutoOptimize = m_chkAutoOptim.GetCheck();
	m_Data.dMinVehlDist = m_edtMinVehlDist.GetEditValue();
	m_Data.OptimizeLane = m_cmbOptimLane.GetItemData(m_cmbOptimLane.GetCurSel());
	m_Data.nMinNumVehicle = m_edtMinNumVehl.GetEditValue();
	m_Data.nMaxNumVehicle = m_edtMaxNumVehl.GetEditValue();
	if (m_Data.bAutoOptimize)
	{
		m_Data.nCombOption = m_nCombOption2;
		m_Data.Vehicle2 = 0;
		m_Data.aNALanes.RemoveAll();
		m_Data.aNCLanes.RemoveAll();
	}

	return TRUE;
}

BOOL CCMMvldItemNewZealandDlg::ApplyOrOK()
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
void CCMMvldItemNewZealandDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact), _LS(IDS_WG_CMD__ADDD__Lane1), _LS(IDS_WG_CMD__ADDD__Lane2), _LS(IDS_WG_CMD__ADDD__Lane3), _LS(IDS_WG_CMD__ADDD__Lane4), _LS(IDS_WG_CMD__ADDD__Lane5), _LS(IDS_WG_CMD__ADDD__More) };
	int nColWidth[COLCOUNT] = { 108, 50, 60, 60, 60, 60, 60, 48 };

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for (i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemNewZealandDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.arSubLoadData.GetSize();
	if (nItemCount == 0) return;

	CString str;
	int nCount;

	if (nItemCount)
	{
		for (nCount = 0; nCount < nItemCount; nCount++)
		{
			InsertItem(nCount, m_Data.arSubLoadData[nCount]);
		}
	}
}

BOOL CCMMvldItemNewZealandDlg::InsertItem(int nIndex, T_MVLD_BASE& Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for (int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
		{
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemNewZealandDlg::DeleteItem(int nIndex, T_MVLD_BASE& Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_List.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemNewZealandDlg::ModifyItem(int nIndex, T_MVLD_BASE& Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for (int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Data);
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemNewZealandDlg::DataToStr(int i, T_MVLD_BASE& Data)
{
	CString str;

	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if (i == 0)
	{
		if (Data.nVehicleType == 1)
		{
			m_pDoc->m_pAttrCtrl->GetMvhc(Data.VehicleKey, mvhc);
			str = _T("VC:") + mvhc.VehicleClassName;
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(Data.VehicleKey, mvhl);
			str = _T("VL:") + mvhl.VehicleLoadName;
		}
	}
	else if (i == 1) str.Format(_T("%g"), Data.dVehicleScaleFactor);
	else if (i >= 2 && i <= 6)
	{
		if (Data.aSelectedLanes.GetSize() > i - 2)
		{
			if (Data.aSelectedLanes[i - 2] == 0) str = _T("");
			else
			{
				str = GetLaneName(Data.aSelectedLanes[i - 2]);
			}
		}
		else str = _T("");
	}
	else if (i == 7)
	{
		if (Data.aSelectedLanes.GetSize() > i - 2)
		{
			if (Data.aSelectedLanes[i - 2] == 0) str = _T("");
			else str = _T("...");
		}
		else str = _T("");
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

CString CCMMvldItemNewZealandDlg::GetLaneName(UINT nLaneKey)
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


BEGIN_MESSAGE_MAP(CCMMvldItemNewZealandDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemNewZealandDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_LCPV_CHK, OnCmdLcpvChk)
	ON_BN_CLICKED(IDC_GENERAL_LOAD_RDO, OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_TRAFFIC_LOAD_RDO, OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_RATING_POSTING_RDO, OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_FATIGUE_LOAD_RDO, OnSelLoadModelRdo)

	ON_BN_CLICKED(IDC_CMD_MVLD_OPTIM_CHK, OnCmdOptmChk)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_ADD_BTN, OnCmdBtnAdd_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_MOD_BTN, OnCmdBtnModify_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_DEL_BTN, OnCmdBtnDelete_AssignVehl)

	ON_BN_CLICKED(IDC_CMD_SUB_BTN_ADD, OnCmdSubBtnAdd)
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_DEL, OnCmdSubBtnDelete)
	ON_BN_CLICKED(IDC_CMD_SUB_STRAD_ADD_BTN, OnCmdHOBtnAdd)
	ON_BN_CLICKED(IDC_CMD_SUB_STRAD_DEL_BTN, OnCmdHOBtnDelete)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemNewZealandDlg message handlers

BOOL CCMMvldItemNewZealandDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	m_chkLoadModel.InitControl(this, CCMMvldItemNewZealandDlg::IDD, IDC_SEL_LOAD_MODEL_GRP, TRUE, TRUE);
	m_chkLoadModel.SetFoldCurHeight(TRUE);

	m_chkMultiLaneFactor.InitControl(this, CCMMvldItemNewZealandDlg::IDD, IDC_CMD_MPF_FRM, TRUE, TRUE);
	m_chkMultiLaneFactor.SetFoldCurHeight(TRUE);

	m_chkAccompanying.InitControl(this, CCMMvldItemNewZealandDlg::IDD, IDC_CMD_MPF_FRM2, TRUE, TRUE);
	m_chkAccompanying.SetFoldCurHeight(TRUE);

	InitCtrlPos();

	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0 || m_pDoc->m_pAttrCtrl->GetCountSlanop() > 0)
	{
		m_pDoc->m_pAttrCtrl->GetSlanKeyList(m_aLaneList);
		m_bSurface = TRUE;
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetLlanKeyList(m_aLaneList);
		m_bSurface = FALSE;
	}

	// TODO: Add extra initialization here
	if (!m_bModify)
	{
		m_Data.Initialize();
		m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, D_MOVE_CODE_NEWZEALAND);
	}

	CDlgUtil::CtrlRadioSetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);

	InitCtrl();
	InitUnit();
	InitCombo();

	SetHeaderTitle();
	SetHeaderTitle_AssignVehl();
	Data2Dlg();
	MakeItemEx_AssignVehl();

	m_bInitDlg = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemNewZealandDlg::InitSelList()
{
	m_Data.aNALanes.RemoveAll();
	m_Data.aNCLanes.RemoveAll();
	m_Data.arSubLoadData.RemoveAll();
	Data2Dlg_LaneListByVehicleIndex();
}

void CCMMvldItemNewZealandDlg::SetHeaderTitle_AssignVehl()
{
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact) };
	int nColWidth[COLCOUNT_ASSIGNVEHL] = { 118, 80 };

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_AssignVehlList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_AssignVehlList.GetSafeHwnd(), dwStyle);

	// Set Title
	for (i = 0; i < COLCOUNT_ASSIGNVEHL; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_AssignVehlList.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemNewZealandDlg::MakeItemEx_AssignVehl()
{
	m_AssignVehlList.DeleteAllItems();

	int nItemCount = m_Data.aOptimize.GetSize();
	if (nItemCount == 0) return;

	if (nItemCount)
	{
		for (int nCount = 0; nCount < nItemCount; nCount++)
		{
			InsertItem_AssignVehl(nCount, m_Data.aOptimize[nCount]);
		}
	}
}

void CCMMvldItemNewZealandDlg::Data2Dlg_LaneListByVehicleIndex()
{
	int nAllSize = m_aLaneList.GetSize();
	int nSelSize = 0;
	for (nSelSize = 0; nSelSize < m_Data.aNALanes.GetSize(); nSelSize++)
	{
		if (m_Data.aNALanes[nSelSize] == 0) break;
	}

	int nHOSelSize = 0;
	for (nHOSelSize = 0; nHOSelSize < m_Data.aNCLanes.GetSize(); nHOSelSize++)
	{
		if (m_Data.aNALanes[nHOSelSize] == 0) break;
	}

	m_aSelFlag.SetSize(nAllSize);
	m_aHOSelFlag.SetSize(nAllSize);

	for (int i = 0; i < nAllSize; i++)
	{
		BOOL bFoundInSel = FALSE;
		BOOL bFoundInHOSel = FALSE;

		for (int j = 0; j < nSelSize; j++)
		{
			if (m_aLaneList[i] == m_Data.aNALanes[j])
			{
				bFoundInSel = TRUE; break;
			}
		}
		for (int j = 0; j < nHOSelSize; ++j)
		{
			if (m_aLaneList[i] == m_Data.aNCLanes[j])
			{
				bFoundInHOSel = TRUE; break;
			}
		}

		m_aSelFlag[i] = bFoundInSel;
		m_aHOSelFlag[i] = bFoundInHOSel;
	}


	int nIndex;
	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();
	m_wndHOSelList.ResetContent();

	// Add String
	CString LaneName;
	for (int i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i] || m_aHOSelFlag[i])
		{
			if (m_aSelFlag[i])
			{
				nIndex = m_wndSelList.AddString(LaneName);
				m_wndSelList.SetItemData(nIndex, i);
			}
			if (m_aHOSelFlag[i])
			{
				nIndex = m_wndHOSelList.AddString(LaneName);
				m_wndHOSelList.SetItemData(nIndex, i);
			}
		}
		else
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemNewZealandDlg::InitCtrlPos()
{
	// LCPV
	CRect rRef, rToMove;
	GetDlgItem(IDC_SEL_LOAD_MODEL_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_PV_FRM)->GetWindowRect(rToMove);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlLCPV, rRef.top - rToMove.top);

	// Dynamic Accompanying factor
	int nDistX, nDistY;
	CArray<UINT, UINT> aMoveCtrl;

	GetDlgItem(IDC_CMD_SF1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_1_EDT)->GetWindowRect(rToMove);

	aMoveCtrl.RemoveAll();
	aMoveCtrl.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_1_EDT);
	aMoveCtrl.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_2_EDT);
	aMoveCtrl.Add(IDC_CMD_ML_MVLD_ITEM_KS_RAIL_3LANE_3_EDT);

	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aMoveCtrl, nDistX, nDistY);

	GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MPF_FRM2)->GetWindowRect(rToMove);
	aMoveCtrl.RemoveAll();
	aMoveCtrl.Add(IDC_CMD_MPF_FRM2);

	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aMoveCtrl, nDistX, nDistY);
}

void CCMMvldItemNewZealandDlg::InitCtrl()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

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
		pDoc->m_pAttrCtrl->GetLlanKeyList(aLlanK);
		T_LLAN_D LlanD;
		for (int i = 0; i < aLlanK.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetLlan(aLlanK[i], LlanD);
			CDlgUtil::CobxAddItem(m_cmbRefLane, LlanD.LineLaneName, aLlanK[i]);
		}
		if (aLlanK.GetSize() > 0) m_cmbRefLane.SetCurSel(0);
	}
}

void CCMMvldItemNewZealandDlg::OnOK()
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;

	m_chkLoadModel.SetFoldState(FALSE, FALSE);
	m_chkMultiLaneFactor.SetFoldState(FALSE, FALSE);
	m_chkAccompanying.SetFoldState(FALSE, FALSE);
	CDialogMove::OnOK();
}


void CCMMvldItemNewZealandDlg::OnCancel()
{

	m_chkLoadModel.SetFoldState(FALSE, FALSE);
	m_chkMultiLaneFactor.SetFoldState(FALSE, FALSE);
	m_chkAccompanying.SetFoldState(FALSE, FALSE);
	CDialogMove::OnCancel();
}


void CCMMvldItemNewZealandDlg::OnCmdApply()
{
	// TODO: Add your control notification handler code here
	m_chkLoadModel.SetFoldState(FALSE, FALSE);
	m_chkMultiLaneFactor.SetFoldState(FALSE, FALSE);
	m_chkAccompanying.SetFoldState(FALSE, FALSE);
	ApplyOrOK();
}

LRESULT CCMMvldItemNewZealandDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{

	AlignControl();
	ShowHideCtrl();
	BOOL bFold = m_chkLoadModel.GetFoldState();
	CDlgUtil::CtrlShowHide(this, m_aLoadModelRdo, !bFold);


	if (m_Data.nLoadModel != 2 && m_Data.nLoadModel != 3)
	{
		bFold = m_chkMultiLaneFactor.GetFoldState();
		CDlgUtil::CtrlShowHide(this, m_aMPFCtrl, !bFold);
	}
	if (m_Data.nLoadModel == 2)
	{
		bFold = m_chkAccompanying.GetFoldState();

		CDlgUtil::CtrlShowHide(this, m_aAccompanyingCtrl, !bFold);
	}
	return 0L;
}

void CCMMvldItemNewZealandDlg::OnCmdBtnAdd()
{
	// TODO: Add your control notification handler code here
	CCMMvldSubItemDlg dlg(this);
	dlg.m_nLoadModel = m_Data.nLoadModel;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemNewZealandDlg::OnCmdBtnModify()
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}

	CCMMvldSubItemDlg dlg(this);
	dlg.m_nLoadModel = m_Data.nLoadModel;
	dlg.SetParamData(m_Data.arSubLoadData[iItem], iItem);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

BOOL CCMMvldItemNewZealandDlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
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

void CCMMvldItemNewZealandDlg::OnCmdBtnDelete()
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
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
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvldItemNewZealandDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();
	*pResult = 0;
}

void CCMMvldItemNewZealandDlg::EnableCtrl()
{
	//EnableCtrl()은 꼭 ShowHideCtrl()을 부르기 전에 불러야 함
	BOOL bAutoOptimEnable = FALSE;
	BOOL bAutoOptm = m_chkAutoOptim.GetCheck();
	BOOL bLCPV = m_chkLCPV.GetCheck();

	//GetDlgItem(IDC_CMD_LCPV_CHK)->EnableWindow(!bAutoOptm);
	//GetDlgItem(IDC_CMD_MVLD_OPTIM_CHK)->EnableWindow(!bLCPV);

	if (m_Data.nLoadModel == 3)
	{
		GetDlgItem(IDC_CMD_RADIO_COMBINED)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_RADIO_INDEPENDENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_RADIO_COMBINED2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_RADIO_INDEPENDENT2)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMD_MVLD_MAX_NUM_VEHL_EDT)->EnableWindow(FALSE);

		m_edtMaxNumVehl.SetEditUnit(min(m_Data.nMaxNumVehicle, 2));

		m_nCombOption2 = 1;

		CDlgUtil::CtrlRadioSetCheck(this, m_aCombOption2, m_nCombOption2);
		CDlgUtil::CtrlRadioSetCheck(this, m_aCombOption, m_nCombOption2);
	}
	else
	{
		GetDlgItem(IDC_CMD_RADIO_COMBINED)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_RADIO_INDEPENDENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_RADIO_COMBINED2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_RADIO_INDEPENDENT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_MVLD_MAX_NUM_VEHL_EDT)->EnableWindow(TRUE);

		m_edtMaxNumVehl.SetEditUnit(m_Data.nMaxNumVehicle);

		m_nCombOption2 = m_Data.nCombOption;
		CDlgUtil::CtrlRadioSetCheck(this, m_aCombOption2, m_nCombOption2);
		CDlgUtil::CtrlRadioSetCheck(this, m_aCombOption, m_nCombOption2);
	}
}

void CCMMvldItemNewZealandDlg::ShowHideCtrl()
{
	//ShowHideCtrl()은 꼭 EnableCtrl를 부른 후에 불러야 함
	T_MVCD_D DataMvcd;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	BOOL bLCPV = m_chkLCPV.GetCheck();
	BOOL bAutoOptim = m_chkAutoOptim.GetCheck();

	if (!bLCPV)
	{
		CDlgUtil::CtrlShowHide(this, m_aLoadModel, TRUE);
		if (m_Data.nLoadModel == 2)
		{
			CDlgUtil::CtrlShowHide(this, m_aMPF, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aAccompanying, TRUE);
			GetDlgItem(IDC_MULTI_STATIC3)->SetWindowText(_T("3 or more"));
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, m_aAccompanying, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMPF, TRUE);

			GetDlgItem(IDC_MULTI_STATIC3)->SetWindowText(_T("3"));
		}

		if (m_Data.nLoadModel == 3)
		{
			CDlgUtil::CtrlShowHide(this, m_aMPF, FALSE);
		}
		CDlgUtil::CtrlShowHide(this, m_aHOVehicle, (m_Data.nLoadModel == 1));
		CDlgUtil::CtrlShowHide(this, m_aHOLaneCtrl, (m_Data.nLoadModel == 1) && !bAutoOptim);

		CArray<UINT, UINT> aMoveCtrl;
		aMoveCtrl.RemoveAll();
		aMoveCtrl.Add(IDC_ST_LOAD_TXT);
		aMoveCtrl.Add(IDC_ST_LOAD_CBX);
		CDlgUtil::CtrlShowHide(this, aMoveCtrl, (m_Data.nLoadModel == 1) && !bAutoOptim);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aLoadModel, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aMPF, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aHOVehicle, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aAccompanying, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aHOLaneCtrl, FALSE);
	}

	// PermitVehicle
	CDlgUtil::CtrlShowHide(this, m_aCtrlLCPV, bLCPV && !bAutoOptim);

	// Optim
	CDlgUtil::CtrlShowHide(this, m_aOptimCtrl, bAutoOptim);
	CDlgUtil::CtrlShowHide(this, m_aEtc, !bLCPV && !bAutoOptim && (m_Data.nLoadModel != 1));

	GetDlgItem(IDOK)->ShowWindow(TRUE);
	GetDlgItem(IDCANCEL)->ShowWindow(TRUE);
	GetDlgItem(IDC_CMD_APPLY)->ShowWindow(TRUE);
}

void CCMMvldItemNewZealandDlg::ResizeDialog()
{
	BOOL bLCPV = m_chkLCPV.GetCheck();
	BOOL bOptm = m_chkAutoOptim.GetCheck();
	if (bLCPV && bOptm)
	{
		ASSERT(0);
		return;
	}
	CRect rSLC, rPV, rOP, rToMove, rHOLn;
	GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rSLC);
	GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rOP);
	GetDlgItem(IDC_CMD_PV_FRM)->GetWindowRect(rPV);
	GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rHOLn);
	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);

	int nDistY;
	if (bLCPV)
	{
		nDistY = (rPV.bottom + globalUtils.ScaleByDPI(16)) - rToMove.top;
	}
	else if (bOptm)
	{
		nDistY = (rOP.bottom + globalUtils.ScaleByDPI(16)) - rToMove.top;
	}
	else
	{
		if (m_Data.nLoadModel == 1)
		{
			nDistY = (rHOLn.bottom + globalUtils.ScaleByDPI(16)) - rToMove.top;
		}
		else
		{
			nDistY = (rSLC.bottom + globalUtils.ScaleByDPI(16)) - rToMove.top;
		}
	}
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlBtn, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
	RedrawWindow();
}

void CCMMvldItemNewZealandDlg::OnCmdLcpvChk()
{
	// TODO: Add your control notification handler code here

	InitSelList();
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);
	m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, D_MOVE_CODE_NEWZEALAND);

	MakeItemEx();
	MakeItemEx_AssignVehl();

	m_chkAutoOptim.SetCheck(0);
	m_chkLoadModel.SetFoldState(FALSE, TRUE);
	m_chkMultiLaneFactor.SetFoldState(FALSE, TRUE);
	m_chkAccompanying.SetFoldState(FALSE, TRUE);
	EnableCtrl();
	ShowHideCtrl();
	AlignControl();
	ResizeDialog();
}

void CCMMvldItemNewZealandDlg::OnCmdOptmChk()
{
	InitSelList();
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);
	m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, D_MOVE_CODE_NEWZEALAND);
	MakeItemEx();
	MakeItemEx_AssignVehl();

	m_chkLCPV.SetCheck(0);
	EnableCtrl();
	ShowHideCtrl();
	AlignControl();
	ResizeDialog();

	BOOL bFold = m_chkLoadModel.GetFoldState();
	CDlgUtil::CtrlShowHide(this, m_aLoadModelRdo, !bFold);

	if (m_Data.nLoadModel != 2 && m_Data.nLoadModel != 3)
	{
		bFold = m_chkMultiLaneFactor.GetFoldState();
		CDlgUtil::CtrlShowHide(this, m_aMPFCtrl, !bFold);
	}
	if (m_Data.nLoadModel == 2)
	{
		bFold = m_chkAccompanying.GetFoldState();

		CDlgUtil::CtrlShowHide(this, m_aAccompanyingCtrl, !bFold);
	}
}

void CCMMvldItemNewZealandDlg::OnSelLoadModelRdo()
{
	UpdateData(TRUE);

	InitSelList();

	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);
	m_chkMultiLaneFactor.SetFoldState(FALSE, TRUE);
	m_chkAccompanying.SetFoldState(FALSE, TRUE);
	m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, D_MOVE_CODE_NEWZEALAND);

	MakeItemEx();
	MakeItemEx_AssignVehl();

	if (m_Data.nLoadModel == 2)
	{
		CString csVal;
		csVal.Format(_T("%g"), m_Data.dScaleFactor[0]);
		m_wndAccompanying1.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dScaleFactor[1]);
		m_wndAccompanying2.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dScaleFactor[2]);
		m_wndAccompanying3.SetWindowText(csVal);
	}

	UpdateCombo();

	EnableCtrl();
	ShowHideCtrl();
	AlignControl();
	ResizeDialog();
}

void CCMMvldItemNewZealandDlg::AlignControl()
{
	CRect rRef, rToMove;
	int nDistY = 0.;
	int nDistX = 0.;
	CArray<UINT, UINT> aMoveCtrl;

	BOOL bLCPV = m_chkLCPV.GetCheck();
	BOOL bAutoOptm = m_chkAutoOptim.GetCheck();

	if (!bLCPV)
	{

		if (m_Data.nLoadModel == 2)
		{

			GetDlgItem(IDC_CMD_MPF_FRM2)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rToMove);

			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aHOVehicle, nDistY);
		}
		else
		{
			GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rToMove);

			nDistY = rRef.top - rToMove.top;
			nDistX = rRef.left - rToMove.left;
			CDlgUtil::CtrlMoveDistXY(this, m_aHOLaneCtrl, nDistX, nDistY);

			GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rToMove);

			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aHOVehicle, nDistY);
		}

		aMoveCtrl.RemoveAll();
		aMoveCtrl.Add(IDC_CMD_MVLD_SEL_VEHL_STC2);
		aMoveCtrl.Add(IDC_CMD_MVLD_SEL_VEHL_CMB2);
		GetDlgItem(IDC_ST_LOAD_TXT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_SEL_VEHL_STC2)->GetWindowRect(rToMove);

		if (m_Data.nLoadModel == 1)
		{
			if (bAutoOptm)
			{
				nDistY = rRef.top - rToMove.top;
				CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);
			}
			else
			{
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
				CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);
			}
		}
		else
		{
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);
		}

		GetDlgItem(IDC_CMD_MVLD_SEL_VEHL_STC2)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.bottom + globalUtils.ScaleByDPI(8);;
		CDlgUtil::CtrlResizeBottom(this, IDC_CMD_LOAD_CASE_TXT, nDistY);

		if (m_Data.nLoadModel == 3)
		{
			GetDlgItem(IDC_SEL_LOAD_MODEL_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		}
		else
		{
			GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rToMove);
			if (m_Data.nLoadModel == 1)
			{
				GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rRef);
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			}
			else
			{
				GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rRef);
				nDistY = rRef.top - rToMove.top;
			}
		}
		CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aHOLaneCtrl, nDistY);
	}

	// Optim
	if (m_Data.nLoadModel == 3)
	{
		GetDlgItem(IDC_SEL_LOAD_MODEL_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	}
	else
	{
		GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
		if (m_Data.nLoadModel == 1)
		{
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		}
		else
		{
			nDistY = rRef.top - rToMove.top;
		}
	}
	CDlgUtil::CtrlMoveDistY(this, m_aOptimCtrl, nDistY);
}

void CCMMvldItemNewZealandDlg::InitUnit()
{
	m_edtEccen.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dEccentricity);
	m_untEccen.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dEccentricity);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);

	InitUnitOptm();
}

void CCMMvldItemNewZealandDlg::InitUnitOptm()
{
	m_edtMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_untMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_edtMinNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtMaxNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactorOptim.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvldItemNewZealandDlg::OnCmdBtnAdd_AssignVehl()
{
	UpdateData(TRUE);

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cmbSelVehl.GetItemData(m_cmbSelVehl.GetCurSel());

	Data.nVehicleType = TypeKey.typekey.type + 1;
	Data.VehicleK = TypeKey.typekey.key;
	Data.dScaleFactor = m_edtScaleFactorOptim.GetEditValue();

	if (Data.nVehicleType == 1)
	{
		if (!m_pDoc->m_pAttrCtrl->ExistMvhc(Data.VehicleK)) return;
	}
	else
	{
		if (!m_pDoc->m_pAttrCtrl->ExistMvhl(Data.VehicleK)) return;
	}

	int nCount = m_Data.aOptimize.GetSize();
	m_Data.aOptimize.Add(Data);
	InsertItem_AssignVehl(nCount, Data);
}

void CCMMvldItemNewZealandDlg::OnCmdBtnModify_AssignVehl()
{
	UpdateData(TRUE);

	int iItem = m_AssignVehlList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
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

	if (Data.nVehicleType == 1)
	{
		if (!m_pDoc->m_pAttrCtrl->ExistMvhc(Data.VehicleK)) return;
	}
	else
	{
		if (!m_pDoc->m_pAttrCtrl->ExistMvhl(Data.VehicleK)) return;
	}

	m_Data.aOptimize.SetAt(iItem, Data);
	ModifyItem_AssignVehl(iItem, Data);
}

void CCMMvldItemNewZealandDlg::OnCmdBtnDelete_AssignVehl()
{
	int iItem = m_AssignVehlList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
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
		m_AssignVehlList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

BOOL CCMMvldItemNewZealandDlg::InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE& Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for (int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr_AssignVehl(i, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
		{
			nItem = m_AssignVehlList.InsertItem(&lvitem);
		}
		else m_AssignVehlList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemNewZealandDlg::DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE& Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_AssignVehlList.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemNewZealandDlg::ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE& Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for (int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr_AssignVehl(i, Data);
			m_AssignVehlList.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemNewZealandDlg::DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE& Data)
{
	CString str;

	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if (i == 0)
	{
		if (Data.nVehicleType == 1)
		{
			m_pDoc->m_pAttrCtrl->GetMvhc(Data.VehicleK, mvhc);
			str = _T("VC:") + mvhc.VehicleClassName;
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(Data.VehicleK, mvhl);
			str = _T("VL:") + mvhl.VehicleLoadName;
		}
	}
	else if (i == 1) str.Format(_T("%g"), Data.dScaleFactor);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

void CCMMvldItemNewZealandDlg::OnCmdHOBtnAdd()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount - 1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aHOSelFlag[nIndex] = TRUE;
	}

	CString csLaneName;
	m_wndHOSelList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aHOSelFlag[i])
		{
			nIndex = m_wndHOSelList.AddString(csLaneName);
			m_wndHOSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemNewZealandDlg::OnCmdSubBtnAdd()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndUnselList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount - 1; i >= 0; i--)
	{
		nIndex = m_wndUnselList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = TRUE;
		m_wndUnselList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndSelList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i])
		{
			nIndex = m_wndSelList.AddString(csLaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemNewZealandDlg::OnCmdHOBtnDelete()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndHOSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount - 1; i >= 0; i--)
	{
		nIndex = m_wndHOSelList.GetItemData(aIndex[i]);
		m_aHOSelFlag[nIndex] = FALSE;
		m_wndHOSelList.DeleteString(aIndex[i]);
	}
}

void CCMMvldItemNewZealandDlg::OnCmdSubBtnDelete()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount - 1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);

		if (m_aHOSelFlag[aIndex[i]])
		{
			nIndex = m_wndHOSelList.GetItemData(aIndex[i]);
			m_aHOSelFlag[nIndex] = FALSE;
			m_wndHOSelList.DeleteString(aIndex[i]);
		}
	}

	CString csLaneName;
	m_wndUnselList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFlag[i])
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}
