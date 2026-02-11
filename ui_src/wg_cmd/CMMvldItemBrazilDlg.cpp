// CMMvldItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemBrazilDlg.h"

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
// CCMMvldItemBrazilDlg dialog
union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key : 31;
	} typekey;
};
CCMMvldItemBrazilDlg::CCMMvldItemBrazilDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemBrazilDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_bInitDlg = FALSE;

	m_aLaneList.RemoveAll();

	//{{AFX_DATA_INIT(CCMMvldItemBrazilDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aLoadModelRdo.RemoveAll();
	m_aLoadModelRdo.Add(IDC_CMD_ROADWAY_BRIDGE_RDO);
	m_aLoadModelRdo.Add(IDC_CMD_RAILWAY_BRIDGE_RDO);

	m_aCtrlLaneFactor.RemoveAll();
	m_aCtrlLaneFactor.Add(IDC_CMD_MPF_FRM);
	m_aCtrlLaneFactor.Add(IDC_CMD_MPF_FRM_NUM_LANES);
	m_aCtrlLaneFactor.Add(IDC_CMD_MPF_FRM_FACTOR);
	m_aCtrlLaneFactor.Add(IDC_MULTI_STATIC1);
	m_aCtrlLaneFactor.Add(IDC_MULTI_STATIC2);
	m_aCtrlLaneFactor.Add(IDC_MULTI_STATIC3);
	m_aCtrlLaneFactor.Add(IDC_MULTI_STATIC4);
	m_aCtrlLaneFactor.Add(IDC_CMD_SF1);
	m_aCtrlLaneFactor.Add(IDC_CMD_SF2);
	m_aCtrlLaneFactor.Add(IDC_CMD_SF3);
	m_aCtrlLaneFactor.Add(IDC_CMD_SF4);

	m_aCtrlRoadVehicle.RemoveAll();
	m_aCtrlRoadVehicle.Add(IDC_CMD_LOAD_CASE_TXT);
	m_aCtrlRoadVehicle.Add(IDC_ST_LOAD_TXT);
	m_aCtrlRoadVehicle.Add(IDC_CMD_MVLD_SEL_VEHL_STC2);
	m_aCtrlRoadVehicle.Add(IDC_ST_LOAD_CBX);
	m_aCtrlRoadVehicle.Add(IDC_CMD_MVLD_SEL_VEHL_CMB2);

	m_aCtrlRoadSubCase.RemoveAll();
	m_aCtrlRoadSubCase.Add(IDC_CMD_LANES_GRUP);
	m_aCtrlRoadSubCase.Add(IDC_CMD_LIST_TXT);
	m_aCtrlRoadSubCase.Add(IDC_CMD_UNSEL_LIST);
	m_aCtrlRoadSubCase.Add(IDC_CMD_SELECTED_TXT);
	m_aCtrlRoadSubCase.Add(IDC_CMD_SEL_LIST);
	m_aCtrlRoadSubCase.Add(IDC_CMD_FOOTWAY_LANE_STC);
	m_aCtrlRoadSubCase.Add(IDC_CMD_FOOTWAY_LANE_LST);
	m_aCtrlRoadSubCase.Add(IDC_CMD_SUB_BTN_ADD);
	m_aCtrlRoadSubCase.Add(IDC_CMD_SUB_BTN_DEL);
	m_aCtrlRoadSubCase.Add(IDC_CMD_FOOTWAY_ADD_BTN);
	m_aCtrlRoadSubCase.Add(IDC_CMD_FOOTWAY_DEL_BTN);

	m_aCtrlRailSubCase.RemoveAll();
	m_aCtrlRailSubCase.Add(IDC_CMD_SLC_FRM);
	m_aCtrlRailSubCase.Add(IDC_CMD_LOAD_EFFECT_STC);
	m_aCtrlRailSubCase.Add(IDC_CMD_RADIO_COMBINED);
	m_aCtrlRailSubCase.Add(IDC_CMD_RADIO_INDEPENDENT);
	m_aCtrlRailSubCase.Add(IDC_CMD_LIST);
	m_aCtrlRailSubCase.Add(IDC_CMD_BTN_ADD);
	m_aCtrlRailSubCase.Add(IDC_CMD_BTN_MODIFY);
	m_aCtrlRailSubCase.Add(IDC_CMD_BTN_DELETE);

	m_aCtrlRailComb.RemoveAll();
	m_aCtrlRailComb.Add(IDC_CMD_RADIO_COMBINED);
	m_aCtrlRailComb.Add(IDC_CMD_RADIO_INDEPENDENT);

	m_aCtrl.RemoveAll();
	m_aCtrl.Add(IDOK);
	m_aCtrl.Add(IDCANCEL);
	m_aCtrl.Add(IDC_CMD_APPLY);

	m_aCtrlOptMinDist.RemoveAll();
	m_aCtrlOptMinDist.Add(IDC_CMD_MVLD_OPTIM_GRP);
	m_aCtrlOptMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_STC);
	m_aCtrlOptMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_EDT);
	m_aCtrlOptMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_UNT);

	m_aCtrlOptRail.RemoveAll();
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_LCASE_GRP);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_LANE_STC);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_LANE_CMB);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_STC);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_EDT);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_STC);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_EDT);
	m_aCtrlOptRail.Add(IDC_CMD_LOAD_EFFECT_STC2);
	m_aCtrlOptRail.Add(IDC_CMD_RADIO_COMBINED2);
	m_aCtrlOptRail.Add(IDC_CMD_RADIO_INDEPENDENT2);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_ASSIGN_VEHL_GRP);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_SEL_VEHL_STC);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_SEL_VEHL_CMB);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_SCALE_FACTOR_STC);
	m_aCtrlOptRail.Add(IDC_CMD_MVLD_SCALE_FACTOR_EDT);
	m_aCtrlOptRail.Add(IDC_CMD_ASSIGN_VEHL_LIST);
	m_aCtrlOptRail.Add(IDC_CMD_ASSIGN_VEHL_ADD_BTN);
	m_aCtrlOptRail.Add(IDC_CMD_ASSIGN_VEHL_MOD_BTN);
	m_aCtrlOptRail.Add(IDC_CMD_ASSIGN_VEHL_DEL_BTN);

	m_aCtrlOptRailComb.RemoveAll();
	m_aCtrlOptRailComb.Add(IDC_CMD_RADIO_COMBINED2);
	m_aCtrlOptRailComb.Add(IDC_CMD_RADIO_INDEPENDENT2);

	m_aCtrlOptRoad.RemoveAll();
	m_aCtrlOptRoad.Add(IDC_CMD_LANES_GRUP2);
	m_aCtrlOptRoad.Add(IDC_CMD_MVLD_LANE_STC3);
	m_aCtrlOptRoad.Add(IDC_CMD_MVLD_LANE_CMB3);
	m_aCtrlOptRoad.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_STC);
	m_aCtrlOptRoad.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_EDT);
	m_aCtrlOptRoad.Add(IDC_CMD_OPTIM_UNSEL_STC);
	m_aCtrlOptRoad.Add(IDC_CMD_OPTIM_FOOTWAY_STC);
	m_aCtrlOptRoad.Add(IDC_CMD_OPTIM_UNSEL_LST);
	m_aCtrlOptRoad.Add(IDC_CMD_OPTIM_FOOTWAY_ADD_BTN);
	m_aCtrlOptRoad.Add(IDC_CMD_OPTIM_FOOTWAY_DEL_BTN);
	m_aCtrlOptRoad.Add(IDC_CMD_OPTIM_FOOTWAY_LST);
}

void CCMMvldItemBrazilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemBrazilDlg)

	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadCase);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDesc);

	DDX_Control(pDX, IDC_CMD_MVLD_OPTIM_CHK, m_chkAutoOptim);

	DDX_Control(pDX, IDC_CMD_SF1, m_wndSF1);
	DDX_Control(pDX, IDC_CMD_SF2, m_wndSF2);
	DDX_Control(pDX, IDC_CMD_SF3, m_wndSF3);
	DDX_Control(pDX, IDC_CMD_SF4, m_wndSF4);
	DDX_Control(pDX, IDC_ST_LOAD_CBX, m_cmbSelRoadVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB2, m_cmbSelFootVehl);

	DDX_Control(pDX, IDC_CMD_UNSEL_LIST, m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST, m_wndSelList);
	DDX_Control(pDX, IDC_CMD_FOOTWAY_LANE_LST, m_wndFootSelList);

	DDX_Control(pDX, IDC_CMD_LIST, m_List);

	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_EDT, m_edtMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_UNT, m_untMinVehlDist);

	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB, m_cmbOptimRailLane);

	DDX_Control(pDX, IDC_CMD_MVLD_MIN_NUM_VEHL_EDT, m_edtMinNumVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_MAX_NUM_VEHL_EDT, m_edtMaxNumVehl);

	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB, m_cmbSelOptRailVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_SCALE_FACTOR_EDT, m_edtScaleFactorOptim);
	DDX_Control(pDX, IDC_CMD_ASSIGN_VEHL_LIST, m_AssignOptRailVehlList);

	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB3, m_cmbOptimRoadLane);

	DDX_Control(pDX, IDC_CMD_MVLD_NUM_LOADED_LANE_EDT, m_edtOptimRoadNumVehl);

	DDX_Control(pDX, IDC_CMD_OPTIM_UNSEL_LST, m_wndOptUnselList);
	DDX_Control(pDX, IDC_CMD_OPTIM_FOOTWAY_LST, m_wndOptFootSelList);

	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldItemBrazilDlg::Data2Dlg()
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

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRailComb, m_Data.nCombOption);
	CDlgUtil::CtrlRadioSetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);
//	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlAddImpactRdo, m_Data.nLoadCombType);

	if (m_Data.nLoadModel == 0)
	{
		if (m_cmbSelRoadVehl.GetCount() > 0 && m_Data.Vehicle1 != 0 && m_Data.nLoadModel == 0)
		{
			T_MVLD_TYPE_KEY TypeKey;
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = m_Data.Vehicle1;
			CDlgUtil::CobxSetCurSelItemData(m_cmbSelRoadVehl, (DWORD&)TypeKey.value);
		}
		else
		{
			m_Data.Vehicle1 = 0;
		}

		if (m_cmbSelFootVehl.GetCount() > 0 && m_Data.Vehicle2 != 0 && m_Data.nLoadModel == 0)
		{
			T_MVLD_TYPE_KEY TypeKey;
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = m_Data.Vehicle2;
			CDlgUtil::CobxSetCurSelItemData(m_cmbSelFootVehl, (DWORD&)TypeKey.value);
		}
		else
		{
			m_Data.Vehicle2 = 0;
		}
	}
	else
	{
		m_Data.Vehicle1 = 0;
		m_Data.Vehicle2 = 0;
	}

	MakeItemEx();

	Data2DlgOptm();
	Data2Dlg_LaneListByVehicleIndex();

	UpdateData(FALSE);

	AlignControl();
	ShowHideCtrl();
}

void CCMMvldItemBrazilDlg::InitCombo()
{
	// Optimization Lane
	m_cmbOptimRoadLane.ResetContent();
	m_cmbOptimRailLane.ResetContent();

	if (m_bSurface)
	{
		T_SLANop_D SlanD; SlanD.Initialize();
		CArray<T_SLANop_K, T_SLANop_K> aSlanK; aSlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetSlanopKeyList(aSlanK);

		for (int i = 0; i < aSlanK.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetSlanop(aSlanK[i], SlanD)) continue;
			m_cmbOptimRoadLane.SetItemData(m_cmbOptimRoadLane.AddString(SlanD.LaneName), aSlanK[i]);
			m_cmbOptimRailLane.SetItemData(m_cmbOptimRailLane.AddString(SlanD.LaneName), aSlanK[i]);
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
			m_cmbOptimRoadLane.SetItemData(m_cmbOptimRoadLane.AddString(LlanD.LineLaneName), aLlanK[i]);
			m_cmbOptimRailLane.SetItemData(m_cmbOptimRailLane.AddString(LlanD.LineLaneName), aLlanK[i]);
		}
	}

	m_cmbOptimRoadLane.SetCurSel(0);
	m_cmbOptimRailLane.SetCurSel(0);

	// Selected Vehicle
	m_cmbSelRoadVehl.ResetContent();
	m_cmbSelFootVehl.ResetContent();
	m_cmbSelOptRailVehl.ResetContent();

	int nIndex = 0, nIndexHN = 0;
	T_MVHL_D MvhlD; MvhlD.Initialize();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK; aMvhlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = 0;
	nIndex = m_cmbSelRoadVehl.AddString(_LS(IDS_WG_CMD__ADDD__None));
	m_cmbSelRoadVehl.SetItemData(nIndex, TypeKey.value);

	nIndex = m_cmbSelFootVehl.AddString(_LS(IDS_WG_CMD__ADDD__None));
	m_cmbSelFootVehl.SetItemData(nIndex, TypeKey.value);

	nIndex = m_cmbSelOptRailVehl.AddString(_LS(IDS_WG_CMD__ADDD__None));
	m_cmbSelOptRailVehl.SetItemData(nIndex, TypeKey.value);

	TypeKey.typekey.type = 1;
	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		TypeKey.typekey.key = aMvhlK[i];

		if (MvhlD.nStandardCode == D_MVHL_NBR7188_ROADWAY)
		{
			nIndex = m_cmbSelRoadVehl.AddString(_T("VL:") + MvhlD.VehicleLoadName);
			m_cmbSelRoadVehl.SetItemData(nIndex, TypeKey.value);
		}
		else if (MvhlD.nStandardCode == D_MVHL_NBR7188_FOOTWAY)
		{
			nIndex = m_cmbSelFootVehl.AddString(_T("VL:") + MvhlD.VehicleLoadName);
			m_cmbSelFootVehl.SetItemData(nIndex, TypeKey.value);
		}
		else if (MvhlD.nStandardCode == D_MVHL_NBR7189_RAILWAY)
		{
			nIndex = m_cmbSelOptRailVehl.AddString(_T("VL:") + MvhlD.VehicleLoadName);
			m_cmbSelOptRailVehl.SetItemData(nIndex, TypeKey.value);
		}
	}
	m_cmbSelRoadVehl.SetCurSel(0);
	m_cmbSelFootVehl.SetCurSel(0);
	m_cmbSelOptRailVehl.SetCurSel(0);
}

void CCMMvldItemBrazilDlg::Data2DlgOptm()
{
	m_chkAutoOptim.SetCheck(m_Data.bAutoOptimize);
	m_edtMinVehlDist.SetEditUnit(m_Data.dMinVehlDist);

	int nSelect = 0;
	int i = 0;
	for (i = 0; i < m_cmbOptimRoadLane.GetCount(); i++)
	{
		if (m_Data.OptimizeLane != m_cmbOptimRoadLane.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbOptimRoadLane.SetCurSel(nSelect);
	m_cmbOptimRailLane.SetCurSel(nSelect);

	m_edtMinNumVehl.SetEditUnit(m_Data.nMinNumVehicle);
	m_edtMaxNumVehl.SetEditUnit(m_Data.nMaxNumVehicle);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOptRailComb, m_Data.nCombOption);

	// Default Value
	m_edtScaleFactorOptim.SetWindowText(_T("1.0"));
}

BOOL CCMMvldItemBrazilDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_wndLoadCase.GetWindowText(m_Data.LoadCaseName);
	if (m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_wndDesc.GetWindowText(m_Data.Description);

	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);

	m_Data.aNALanes.RemoveAll();
	m_Data.aFootwayLanes.RemoveAll();

	if (m_Data.nLoadModel == 0)
	{
//		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAddImpactRdo, m_Data.nLoadCombType);

		T_MVLD_TYPE_KEY TypeKey;
		CDlgUtil::CobxGetItemDataByCurSel(m_cmbSelRoadVehl, (DWORD&)TypeKey.value);
		m_Data.Vehicle1 = TypeKey.typekey.key;

		CDlgUtil::CobxGetItemDataByCurSel(m_cmbSelFootVehl, (DWORD&)TypeKey.value);
		m_Data.Vehicle2 = TypeKey.typekey.key;

		for (int i = 0; i < m_aLaneList.GetSize(); ++i)
		{
			if (m_aSelFlag[i] && m_Data.Vehicle1 != 0)
			{
				m_Data.aNALanes.Add(m_aLaneList[i]);
			}
			if (m_aFootSelFlag[i] && m_Data.Vehicle2 != 0)
			{
				m_Data.aFootwayLanes.Add(m_aLaneList[i]);
			}
		}

		m_Data.arSubLoadData.RemoveAll();

		CFormulaEdit::GetEditValue(&m_wndSF1, m_Data.dMultiLaneFactor[0]);
		CFormulaEdit::GetEditValue(&m_wndSF2, m_Data.dMultiLaneFactor[1]);
		CFormulaEdit::GetEditValue(&m_wndSF3, m_Data.dMultiLaneFactor[2]);
		CFormulaEdit::GetEditValue(&m_wndSF4, m_Data.dMultiLaneFactor[3]);
	}
	else
	{
		m_Data.Vehicle1 = 0;
		m_Data.Vehicle2 = 0;
		m_Data.aNALanes.RemoveAll();
	}
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRailComb, m_Data.nCombOption);

	Dlg2DataOptm();

	return TRUE;
}

BOOL CCMMvldItemBrazilDlg::Dlg2DataOptm()
{
	m_Data.bAutoOptimize = m_chkAutoOptim.GetCheck();
	m_Data.dMinVehlDist = m_edtMinVehlDist.GetEditValue();

	if (m_Data.nLoadModel == 0)
	{
		m_Data.nMinNumVehicle = 1;
		m_Data.nMaxNumVehicle = m_edtOptimRoadNumVehl.GetEditValue();
		m_Data.OptimizeLane = m_cmbOptimRoadLane.GetItemData(m_cmbOptimRoadLane.GetCurSel());
		if (m_Data.bAutoOptimize)
		{
			m_Data.aOptimize.RemoveAll();
			m_Data.aFootwayLanes.RemoveAll();
			for (int i = 0; i < m_aLaneList.GetSize(); ++i)
			{
				if (m_aOptFootSelFlag[i] && m_Data.Vehicle2 != 0)
				{
					m_Data.aFootwayLanes.Add(m_aLaneList[i]);
				}
			}
			m_Data.nCombOption = 1;
		}
	}
	else
	{
		m_Data.nMinNumVehicle = m_edtMinNumVehl.GetEditValue();
		m_Data.nMaxNumVehicle = m_edtMaxNumVehl.GetEditValue();
		m_Data.OptimizeLane = m_cmbOptimRailLane.GetItemData(m_cmbOptimRailLane.GetCurSel());

		if (m_Data.bAutoOptimize)
		{
			m_Data.aFootwayLanes.RemoveAll();
			CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOptRailComb, m_Data.nCombOption);
		}
	}

	return TRUE;
}

BOOL CCMMvldItemBrazilDlg::ApplyOrOK()
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
void CCMMvldItemBrazilDlg::SetHeaderTitle()
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

void CCMMvldItemBrazilDlg::MakeItemEx()
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

BOOL CCMMvldItemBrazilDlg::InsertItem(int nIndex, T_MVLD_BASE& Data)
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

BOOL CCMMvldItemBrazilDlg::DeleteItem(int nIndex, T_MVLD_BASE& Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_List.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemBrazilDlg::ModifyItem(int nIndex, T_MVLD_BASE& Data)
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

CString CCMMvldItemBrazilDlg::DataToStr(int i, T_MVLD_BASE& Data)
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

CString CCMMvldItemBrazilDlg::GetLaneName(UINT nLaneKey)
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


BEGIN_MESSAGE_MAP(CCMMvldItemBrazilDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemBrazilDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_ROADWAY_BRIDGE_RDO, OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_RAILWAY_BRIDGE_RDO, OnSelLoadModelRdo)

	ON_BN_CLICKED(IDC_CMD_MVLD_OPTIM_CHK, OnCmdOptmChk)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_ADD_BTN, OnCmdBtnAdd_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_MOD_BTN, OnCmdBtnModify_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_DEL_BTN, OnCmdBtnDelete_AssignVehl)

	ON_BN_CLICKED(IDC_CMD_SUB_BTN_ADD, OnCmdSubBtnAdd)
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_DEL, OnCmdSubBtnDelete)
	ON_BN_CLICKED(IDC_CMD_FOOTWAY_ADD_BTN, OnCmdFootBtnAdd)
	ON_BN_CLICKED(IDC_CMD_FOOTWAY_DEL_BTN, OnCmdFootBtnDelete)

	ON_BN_CLICKED(IDC_CMD_OPTIM_FOOTWAY_ADD_BTN, OnCmdOptFootBtnAdd)
	ON_BN_CLICKED(IDC_CMD_OPTIM_FOOTWAY_DEL_BTN, OnCmdOptFootBtnDelete)

	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_ADD_BTN, OnCmdAssignVehlAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_MOD_BTN, OnCmdAssignVehlModBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_DEL_BTN, OnCmdAssignVehlDelBtn)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemBrazilDlg message handlers

BOOL CCMMvldItemBrazilDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

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
		m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, D_MOVE_CODE_BRAZIL);
	}

	CDlgUtil::CtrlRadioSetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);
//	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlAddImpactRdo, m_Data.nLoadCombType);

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

void CCMMvldItemBrazilDlg::InitSelList()
{
	m_Data.aNALanes.RemoveAll();
	m_Data.aFootwayLanes.RemoveAll();
	m_Data.arSubLoadData.RemoveAll();
	Data2Dlg_LaneListByVehicleIndex();
}

void CCMMvldItemBrazilDlg::SetHeaderTitle_AssignVehl()
{
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact) };
	int nColWidth[COLCOUNT_ASSIGNVEHL] = { 118, 80 };

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_AssignOptRailVehlList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_AssignOptRailVehlList.GetSafeHwnd(), dwStyle);

	// Set Title
	for (i = 0; i < COLCOUNT_ASSIGNVEHL; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_AssignOptRailVehlList.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemBrazilDlg::MakeItemEx_AssignVehl()
{
	m_AssignOptRailVehlList.DeleteAllItems();

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

void CCMMvldItemBrazilDlg::Data2Dlg_LaneListByVehicleIndex()
{

	BOOL bAutoOptm = m_chkAutoOptim.GetCheck();

	int nAllSize = m_aLaneList.GetSize();
	int nSelSize = 0;
	for (nSelSize = 0; nSelSize < m_Data.aNALanes.GetSize(); nSelSize++)
	{
		if (m_Data.aNALanes[nSelSize] == 0) break;
	}

	int nFootSelSize = 0;
	for (nFootSelSize = 0; nFootSelSize < m_Data.aFootwayLanes.GetSize(); nFootSelSize++)
	{
		if (m_Data.aFootwayLanes[nFootSelSize] == 0) break;
	}

	m_aSelFlag.SetSize(nAllSize);
	m_aFootSelFlag.SetSize(nAllSize);
	m_aOptFootSelFlag.SetSize(nAllSize);
	for (int i = 0; i < nAllSize; i++)
	{
		BOOL bFoundInSel = FALSE;
		BOOL bFoundInFootSel = FALSE;

		for (int j = 0; j < nSelSize; j++)
		{
			if (m_aLaneList[i] == m_Data.aNALanes[j])
			{
				bFoundInSel = TRUE; break;
			}
		}
		for (int j = 0; j < nFootSelSize; ++j)
		{
			if (m_aLaneList[i] == m_Data.aFootwayLanes[j])
			{
				bFoundInFootSel = TRUE; break;
			}
		}

		m_aSelFlag[i] = bFoundInSel;
		if (bAutoOptm)
		{
			m_aOptFootSelFlag[i] = bFoundInFootSel;
			m_aFootSelFlag[i] = FALSE;
		}
		else
		{
			m_aFootSelFlag[i] = bFoundInFootSel;
			m_aOptFootSelFlag[i] = FALSE;
		}
	}


	int nIndex;
	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();
	m_wndOptUnselList.ResetContent();
	m_wndFootSelList.ResetContent();
	m_wndOptFootSelList.ResetContent();

	// Add String
	CString LaneName;
	for (int i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i] || m_aFootSelFlag[i])
		{
			if (m_aFootSelFlag[i])
			{
				nIndex = m_wndFootSelList.AddString(LaneName);
				m_wndFootSelList.SetItemData(nIndex, i);
			}
			else if (m_aSelFlag[i])
			{
				nIndex = m_wndSelList.AddString(LaneName);
				m_wndSelList.SetItemData(nIndex, i);
			}
		}
		else
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}

		if (m_aOptFootSelFlag[i])
		{
			nIndex = m_wndOptFootSelList.AddString(LaneName);
			m_wndOptFootSelList.SetItemData(nIndex, i);
		}
		else
		{
			nIndex = m_wndOptUnselList.AddString(LaneName);
			m_wndOptUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemBrazilDlg::OnOK()
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;

	CDialogMove::OnOK();
}

void CCMMvldItemBrazilDlg::OnCmdApply()
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();
}

void CCMMvldItemBrazilDlg::OnCmdBtnAdd()
{
	// TODO: Add your control notification handler code here
	CCMMvldSubItemDlg dlg(this);
	dlg.m_nLoadModel = m_Data.nLoadModel;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemBrazilDlg::OnCmdBtnModify()
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

BOOL CCMMvldItemBrazilDlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
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

void CCMMvldItemBrazilDlg::OnCmdBtnDelete()
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

void CCMMvldItemBrazilDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();
	*pResult = 0;
}

void CCMMvldItemBrazilDlg::AlignControl()
{
	CRect rRef, rToMove;
	int nDistY;
	int nLoadModel;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, nLoadModel);
	BOOL bAutoOptm = m_chkAutoOptim.GetCheck();

	if (nLoadModel == 0)
	{
		// Optimization
		GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOptMinDist, nDistY);

		//Assignment Lanes (Road opt)
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LANES_GRUP2)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOptRoad, nDistY);

		if (bAutoOptm)
		{
			GetDlgItem(IDC_CMD_LANES_GRUP2)->GetWindowRect(rRef);
		}
		else
		{
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rRef);
		}
	}
	else
	{
		// Railway Sub-Load Cases
		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlRailSubCase, nDistY);

		// Optimization
		GetDlgItem(IDC_CMD_MPF_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOptMinDist, nDistY);

		// Load Case Data (Rail opt)
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlOptRail, nDistY);


		if (bAutoOptm)
		{
			GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rRef);
		}
		else
		{
			GetDlgItem(IDC_CMD_SLC_FRM)->GetWindowRect(rRef);
		}
	}

	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(24);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl, nDistY);

	// resize self size
	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(rToMove);
	Invalidate();
}

void CCMMvldItemBrazilDlg::ShowHideCtrl()
{
	int nLoadModel;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, nLoadModel);
	BOOL bAutoOptm = m_chkAutoOptim.GetCheck();

	if (nLoadModel == 0)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlLaneFactor, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlRoadVehicle, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlRoadSubCase, !bAutoOptm);
		CDlgUtil::CtrlShowHide(this, m_aCtrlRailSubCase, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlOptRail, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlOptRoad, bAutoOptm);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlLaneFactor, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlRoadVehicle, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlRoadSubCase, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlRailSubCase, !bAutoOptm);
		CDlgUtil::CtrlShowHide(this, m_aCtrlOptRail, bAutoOptm);
		CDlgUtil::CtrlShowHide(this, m_aCtrlOptRoad, FALSE);
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrlOptMinDist, bAutoOptm);
}

void CCMMvldItemBrazilDlg::OnCmdOptmChk()
{
	m_Data.bAutoOptimize = m_chkAutoOptim.GetCheck();
	InitSelList();
	m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, D_MOVE_CODE_BRAZIL);
	MakeItemEx();
	MakeItemEx_AssignVehl();

	AlignControl();
	ShowHideCtrl();
}

void CCMMvldItemBrazilDlg::OnSelLoadModelRdo()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);
	m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, D_MOVE_CODE_BRAZIL);
	InitSelList();

	MakeItemEx();
	MakeItemEx_AssignVehl();

	m_cmbSelRoadVehl.SetCurSel(0);
	m_cmbSelFootVehl.SetCurSel(0);
	m_cmbSelOptRailVehl.SetCurSel(0);

	AlignControl();
	ShowHideCtrl();
}

void CCMMvldItemBrazilDlg::InitUnit()
{
	m_edtMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_untMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_edtMinNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtMaxNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactorOptim.SetUnitType(D_UNITSYS_NONE);
	m_edtOptimRoadNumVehl.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvldItemBrazilDlg::OnCmdBtnAdd_AssignVehl()
{
	UpdateData(TRUE);

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cmbSelOptRailVehl.GetItemData(m_cmbSelOptRailVehl.GetCurSel());

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

void CCMMvldItemBrazilDlg::OnCmdBtnModify_AssignVehl()
{
	UpdateData(TRUE);

	int iItem = m_AssignOptRailVehlList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cmbSelOptRailVehl.GetItemData(m_cmbSelOptRailVehl.GetCurSel());

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

void CCMMvldItemBrazilDlg::OnCmdBtnDelete_AssignVehl()
{
	int iItem = m_AssignOptRailVehlList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	m_Data.aOptimize.RemoveAt(iItem);
	m_AssignOptRailVehlList.DeleteItem(iItem);

	int nCount = m_AssignOptRailVehlList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0)
		m_AssignOptRailVehlList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

BOOL CCMMvldItemBrazilDlg::InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE& Data)
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
			nItem = m_AssignOptRailVehlList.InsertItem(&lvitem);
		}
		else m_AssignOptRailVehlList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemBrazilDlg::DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE& Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_AssignOptRailVehlList.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemBrazilDlg::ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE& Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for (int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr_AssignVehl(i, Data);
			m_AssignOptRailVehlList.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemBrazilDlg::DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE& Data)
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

void CCMMvldItemBrazilDlg::OnCmdFootBtnAdd()
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
		m_aFootSelFlag[nIndex] = TRUE;
		m_wndSelList.DeleteString(aIndex[i]);
	}

	CString csLaneName;

	m_wndFootSelList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aFootSelFlag[i])
		{
			nIndex = m_wndFootSelList.AddString(csLaneName);
			m_wndFootSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemBrazilDlg::OnCmdSubBtnAdd()
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

void CCMMvldItemBrazilDlg::OnCmdOptFootBtnAdd()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndOptUnselList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount - 1; i >= 0; i--)
	{
		nIndex = m_wndOptUnselList.GetItemData(aIndex[i]);
		m_aOptFootSelFlag[nIndex] = TRUE;
		m_wndOptUnselList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndOptFootSelList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aOptFootSelFlag[i])
		{
			nIndex = m_wndOptFootSelList.AddString(csLaneName);
			m_wndOptFootSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemBrazilDlg::OnCmdFootBtnDelete()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndFootSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount - 1; i >= 0; i--)
	{
		nIndex = m_wndFootSelList.GetItemData(aIndex[i]);
		m_aFootSelFlag[nIndex] = FALSE;
		m_aSelFlag[nIndex] = TRUE;
		m_wndFootSelList.DeleteString(aIndex[i]);
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

void CCMMvldItemBrazilDlg::OnCmdSubBtnDelete()
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

void CCMMvldItemBrazilDlg::OnCmdOptFootBtnDelete()
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndOptFootSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount - 1; i >= 0; i--)
	{
		nIndex = m_wndOptFootSelList.GetItemData(aIndex[i]);
		m_aOptFootSelFlag[nIndex] = FALSE;
		m_wndOptFootSelList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndOptUnselList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aOptFootSelFlag[i])
		{
			nIndex = m_wndOptUnselList.AddString(csLaneName);
			m_wndOptUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemBrazilDlg::OnCmdAssignVehlAddBtn()
{
	UpdateData(TRUE);

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cmbSelOptRailVehl.GetItemData(m_cmbSelOptRailVehl.GetCurSel());

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

void CCMMvldItemBrazilDlg::OnCmdAssignVehlModBtn()
{
	UpdateData(TRUE);

	int iItem = m_AssignOptRailVehlList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cmbSelOptRailVehl.GetItemData(m_cmbSelOptRailVehl.GetCurSel());

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

void CCMMvldItemBrazilDlg::OnCmdAssignVehlDelBtn()
{
	int iItem = m_AssignOptRailVehlList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	m_Data.aOptimize.RemoveAt(iItem);
	m_AssignOptRailVehlList.DeleteItem(iItem);

	int nCount = m_AssignOptRailVehlList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0)
		m_AssignOptRailVehlList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}
