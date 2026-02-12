// CMMvldItemSouthAfricaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemSouthAfricaDlg.h"
#include "CMMvldSubItemDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\UnitCtrl.h"


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
// CCMMvldItemSouthAfricaDlg dialog

CCMMvldItemSouthAfricaDlg::CCMMvldItemSouthAfricaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemSouthAfricaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemSouthAfricaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = NULL;

	m_Data.Initialize();
	
	m_bModify = FALSE;
	m_bSurface= FALSE;
	
	m_bAutoOptim = FALSE;
	m_nLoadModel = 0;
	m_nCombOption = 1;
	m_nCombOption2 = 1;

	m_aLaneList.RemoveAll();
	m_aSelFlag.RemoveAll();
	m_aNASelFlag.RemoveAll();
	m_aNCSelFlag.RemoveAll();

	m_aVehicle.RemoveAll();
	m_aVehicle.Add(IDC_CMD_MVLD_LOAD_CASE_GRP);
	m_aVehicle.Add(IDC_CMD_MVLD_LOAD_CASE_VEHL1_STC);
	m_aVehicle.Add(IDC_CMD_MVLD_LOAD_CASE_VEHL1_CMB);
	m_aVehicle.Add(IDC_CMD_MVLD_LOAD_CASE_VEHL2_STC);
	m_aVehicle.Add(IDC_CMD_MVLD_LOAD_CASE_VEHL2_CMB);
	
	m_aUser.RemoveAll();
	m_aUser.Add(IDC_CMD_MVLD_SUB_CASE_GRP);
	m_aUser.Add(IDC_CMD_MVLD_LOAD_EFFECT_GRP);
	m_aUser.Add(IDC_CMD_MVLD_COMBINED_RDO);
	m_aUser.Add(IDC_CMD_MVLD_INDEPENDENT_RDO);
	m_aUser.Add(IDC_CMD_MVLD_SUB_CASE_LST);
	m_aUser.Add(IDC_CMD_MVLD_SUB_CASE_ADD_BTN);
	m_aUser.Add(IDC_CMD_MVLD_SUB_CASE_MOD_BTN);
	m_aUser.Add(IDC_CMD_MVLD_SUB_CASE_DEL_BTN);

	m_aNotUser.RemoveAll();
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_GRP);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_UNSEL_STC);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_UNSEL_LST);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_ADD_BTN);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_DEL_BTN);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_SEL_STC);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_SEL_LST);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_NC_ADD_BTN);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_NC_DEL_BTN);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_NC_STC);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_NC_LST);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_NA_ADD_BTN);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_NA_DEL_BTN);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_NA_STC);
	m_aNotUser.Add(IDC_CMD_MVLD_LANES_NA_LST);
		
	m_aSelLane.RemoveAll();
	m_aSelLane.Add(IDC_CMD_MVLD_LANES_ADD_BTN);
	m_aSelLane.Add(IDC_CMD_MVLD_LANES_DEL_BTN);
	m_aSelLane.Add(IDC_CMD_MVLD_LANES_SEL_STC);
	m_aSelLane.Add(IDC_CMD_MVLD_LANES_SEL_LST);

	m_aNANCLane.RemoveAll();
	m_aNANCLane.Add(IDC_CMD_MVLD_LANES_NC_ADD_BTN);
	m_aNANCLane.Add(IDC_CMD_MVLD_LANES_NC_DEL_BTN);
	m_aNANCLane.Add(IDC_CMD_MVLD_LANES_NC_STC);
	m_aNANCLane.Add(IDC_CMD_MVLD_LANES_NC_LST);
	m_aNANCLane.Add(IDC_CMD_MVLD_LANES_NA_ADD_BTN);
	m_aNANCLane.Add(IDC_CMD_MVLD_LANES_NA_DEL_BTN);
	m_aNANCLane.Add(IDC_CMD_MVLD_LANES_NA_STC);
	m_aNANCLane.Add(IDC_CMD_MVLD_LANES_NA_LST);
	
	m_aMinDist.RemoveAll();
	m_aMinDist.Add(IDC_CMD_MVLD_OPTIM_GRP);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_STC);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_EDT);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_UNT);

	m_aOptimUser.RemoveAll();
	m_aOptimUser.Add(IDC_CMD_MVLD_LCASE_GRP);
	m_aOptimUser.Add(IDC_CMD_MVLD_LANE_STC);
	m_aOptimUser.Add(IDC_CMD_MVLD_LANE_CMB);
	m_aOptimUser.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_STC);
	m_aOptimUser.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_EDT);
	m_aOptimUser.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_STC);
	m_aOptimUser.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_EDT);
	m_aOptimUser.Add(IDC_CMD_MVLD_LOAD_EFFECT_GRP2);
	m_aOptimUser.Add(IDC_CMD_MVLD_COMBINED_RDO2);
	m_aOptimUser.Add(IDC_CMD_MVLD_INDEPENDENT_RDO2);
	m_aOptimUser.Add(IDC_CMD_MVLD_ASSIGN_VEHL_GRP);
	m_aOptimUser.Add(IDC_CMD_MVLD_SEL_VEHL_STC);
	m_aOptimUser.Add(IDC_CMD_MVLD_SEL_VEHL_CMB);
	m_aOptimUser.Add(IDC_CMD_MVLD_SCALE_FACTOR_STC);
	m_aOptimUser.Add(IDC_CMD_MVLD_SCALE_FACTOR_EDT);
	m_aOptimUser.Add(IDC_CMD_MVLD_ASSIGN_VEHL_LST);
	m_aOptimUser.Add(IDC_CMD_MVLD_ASSIGN_VEHL_ADD_BTN);
	m_aOptimUser.Add(IDC_CMD_MVLD_ASSIGN_VEHL_MOD_BTN);
	m_aOptimUser.Add(IDC_CMD_MVLD_ASSIGN_VEHL_DEL_BTN);
		
	m_aNotOptimUser.RemoveAll();
	m_aNotOptimUser.Add(IDC_CMD_MVLD_LANES_GRP2);	
	m_aNotOptimUser.Add(IDC_CMD_MVLD_LANE_STC2);	
	m_aNotOptimUser.Add(IDC_CMD_MVLD_LANE_CMB2);	
	m_aNotOptimUser.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_STC);
	m_aNotOptimUser.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_EDT);

	m_aOkCancel.RemoveAll();
	m_aOkCancel.Add(IDOK);
	m_aOkCancel.Add(IDCANCEL);
	m_aOkCancel.Add(IDC_CMD_APPLY);		
}

void CCMMvldItemSouthAfricaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemSouthAfricaDlg)
	DDX_Control(pDX, IDC_CMD_MVLD_NAME_EDT,            m_edtLoadCase);
	DDX_Control(pDX, IDC_CMD_MVLD_DESC_EDT,            m_edtDesc);
	DDX_Check  (pDX, IDC_CMD_MVLD_OPTIM_CHK,           m_bAutoOptim);
	DDX_Radio  (pDX, IDC_CMD_MVLD_LOAD_MODEL_NA_RDO,   m_nLoadModel);
	DDX_Control(pDX, IDC_CMD_MVLD_LOAD_CASE_VEHL1_CMB, m_cmbVehicle1);
	DDX_Control(pDX, IDC_CMD_MVLD_LOAD_CASE_VEHL2_CMB, m_cmbVehicle2);
	DDX_Control(pDX, IDC_CMD_MVLD_LANES_UNSEL_LST,     m_lstUnSel);
	DDX_Control(pDX, IDC_CMD_MVLD_LANES_SEL_LST,       m_lstSel);
	DDX_Control(pDX, IDC_CMD_MVLD_LANES_NC_LST,        m_lstNCSel);
	DDX_Control(pDX, IDC_CMD_MVLD_LANES_NA_LST,        m_lstNASel);
	DDX_Radio  (pDX, IDC_CMD_MVLD_COMBINED_RDO,        m_nCombOption);
	DDX_Control(pDX, IDC_CMD_MVLD_SUB_CASE_LST,        m_lstSubCase);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_EDT,   m_edtMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_UNT,   m_untMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB,            m_cmbOptimLane);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_NUM_VEHL_EDT,    m_edtMinNumVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_MAX_NUM_VEHL_EDT,    m_edtMaxNumVehl);
	DDX_Radio  (pDX, IDC_CMD_MVLD_COMBINED_RDO2,       m_nCombOption2);
	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB,        m_cmbSelVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_SCALE_FACTOR_EDT,    m_edtScaleFactorOptim);
	DDX_Control(pDX, IDC_CMD_MVLD_ASSIGN_VEHL_LST,     m_lstAssignVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB2,           m_cmbOptimLane2);
	DDX_Control(pDX, IDC_CMD_MVLD_NUM_LOADED_LANE_EDT, m_edtNumLoadLanes);
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldItemSouthAfricaDlg::InitUnit()
{
	m_edtMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_untMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_edtMinNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtMaxNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactorOptim.SetUnitType(D_UNITSYS_NONE);
	m_edtNumLoadLanes.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvldItemSouthAfricaDlg::InitCombo()
{
	// Vehicle1, Vehicle2
	m_cmbVehicle1.ResetContent();
	m_cmbVehicle2.ResetContent();

	T_MVHL_D MvhlD; MvhlD.Initialize();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK; aMvhlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);

	int nIndex = 0;
	T_MVLD_TYPE_KEY TypeKey;

	for(int i=0; i<aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		if(m_nLoadModel == 0) // NA
		{
			if(!IsNAVehicle(MvhlD)) continue;
		}
		else if(m_nLoadModel == 1) // NB
		{
			if(!IsNBVehicle(MvhlD)) continue;
		}
		else if(m_nLoadModel == 2) // NC
		{
			if(!IsNCVehicle(MvhlD)) continue;
		}
		else if(m_nLoadModel == 3) // NC+2/3NA
		{
			if(!IsNCVehicle(MvhlD)) continue;
		}
		else
		{
			continue;
		}

		nIndex = m_cmbVehicle1.AddString(MvhlD.VehicleLoadName);
		m_cmbVehicle1.SetItemData(nIndex, aMvhlK[i]);
	}

	for(int i=0; i<aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		if(m_nLoadModel == 3) // NC+2/3NA
		{
			if(!IsNAVehicle(MvhlD)) continue;
		}
		else
		{
			continue;
		}

		nIndex = m_cmbVehicle2.AddString(MvhlD.VehicleLoadName);
		m_cmbVehicle2.SetItemData(nIndex, aMvhlK[i]);
	}

	m_cmbVehicle1.SetCurSel(0);
	m_cmbVehicle2.SetCurSel(0);

	// Loaded Lane
	m_cmbOptimLane.ResetContent();
	m_cmbOptimLane2.ResetContent();

	if(m_bSurface)
	{
		T_SLANop_D SlanD; SlanD.Initialize();
		CArray<T_SLANop_K, T_SLANop_K> aSlanK; aSlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetSlanopKeyList(aSlanK);

		for(int i=0; i<aSlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSlanop(aSlanK[i], SlanD)) continue;
			m_cmbOptimLane.SetItemData(m_cmbOptimLane.AddString(SlanD.LaneName), aSlanK[i]);
			m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(SlanD.LaneName), aSlanK[i]);
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
			m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(LlanD.LineLaneName), aLlanK[i]);
		}
	}

	m_cmbOptimLane.SetCurSel(0);
	m_cmbOptimLane2.SetCurSel(0);
		
	// Selected Vehicle
	m_cmbSelVehl.ResetContent();

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

void CCMMvldItemSouthAfricaDlg::ChangeVehiCombo()
{
	// Vehicle1, Vehicle2
	m_cmbVehicle1.ResetContent();
	m_cmbVehicle2.ResetContent();

	T_MVHL_D MvhlD; MvhlD.Initialize();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK; aMvhlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);

	int nIndex = 0;

	for(int i=0; i<aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		if(m_nLoadModel == 0) // NA
		{
			if(!IsNAVehicle(MvhlD)) continue;
		}
		else if(m_nLoadModel == 1) // NB
		{
			if(!IsNBVehicle(MvhlD)) continue;
		}
		else if(m_nLoadModel == 2) // NC
		{
			if(!IsNCVehicle(MvhlD)) continue;
		}
		else if(m_nLoadModel == 3) // NC+2/3NA
		{
			if(!IsNCVehicle(MvhlD)) continue;
		}
		else
		{
			continue;
		}

		nIndex = m_cmbVehicle1.AddString(MvhlD.VehicleLoadName);
		m_cmbVehicle1.SetItemData(nIndex, aMvhlK[i]);
	}

	for(int i=0; i<aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		if(m_nLoadModel == 3) // NC+2/3NA
		{
			if(!IsNAVehicle(MvhlD)) continue;
		}
		else
		{
			continue;
		}

		nIndex = m_cmbVehicle2.AddString(MvhlD.VehicleLoadName);
		m_cmbVehicle2.SetItemData(nIndex, aMvhlK[i]);
	}
}

void CCMMvldItemSouthAfricaDlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;	

	GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVLD_SUB_CASE_GRP )->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aUser, nDistX, nDistY);

	GetDlgItem(IDC_CMD_MVLD_LANES_SEL_LST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVLD_LANES_NC_LST )->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aNANCLane, nDistX, nDistY);

	if(m_nLoadModel == 0)
	{
		GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP    )->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP    )->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);
	}

	GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistXY(this, m_aOptimUser, nDistX, nDistY);
	
	if(m_nLoadModel == 0) 
	{
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANES_GRP2)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistXY(this, m_aNotOptimUser, nDistX, nDistY);
	}
	else
	{		
		GetDlgItem(IDC_CMD_MVLD_LANES_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANES_GRP2)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aNotOptimUser, nDistX, nDistY);
	}
		
	if(m_bAutoOptim)
	{
		if(m_nLoadModel == 4) GetDlgItem(IDC_CMD_MVLD_LCASE_GRP )->GetWindowRect(rRef);
		else                  GetDlgItem(IDC_CMD_MVLD_LANES_GRP2)->GetWindowRect(rRef);  
	}
	else
	{
		if(m_nLoadModel == 4) GetDlgItem(IDC_CMD_MVLD_SUB_CASE_GRP)->GetWindowRect(rRef);
		else                  GetDlgItem(IDC_CMD_MVLD_LANES_GRP   )->GetWindowRect(rRef);  
	}
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aOkCancel, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right  = rectLast.right+15;
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	RedrawWindow();
}

void CCMMvldItemSouthAfricaDlg::ShowHideControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aVehicle, m_nLoadModel != 4);
	GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_VEHL2_STC)->ShowWindow(m_nLoadModel == 3);
	GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_VEHL2_CMB)->ShowWindow(m_nLoadModel == 3);

	CDlgUtil::CtrlShowHide(this, m_aUser,        !m_bAutoOptim && m_nLoadModel == 4);
	CDlgUtil::CtrlShowHide(this, m_aNotUser,     !m_bAutoOptim && m_nLoadModel != 4);
	CDlgUtil::CtrlShowHide(this, m_aNANCLane,    !m_bAutoOptim && m_nLoadModel == 3);
	CDlgUtil::CtrlShowHide(this, m_aSelLane,     !m_bAutoOptim && m_nLoadModel != 3 && m_nLoadModel != 4);
	CDlgUtil::CtrlShowHide(this, m_aMinDist,      m_bAutoOptim && (m_nLoadModel == 0 || m_nLoadModel == 4));
	CDlgUtil::CtrlShowHide(this, m_aOptimUser,    m_bAutoOptim && m_nLoadModel == 4);
	CDlgUtil::CtrlShowHide(this, m_aNotOptimUser, m_bAutoOptim && m_nLoadModel != 4);
}

void CCMMvldItemSouthAfricaDlg::EnableDisableControls()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_CMD_MVLD_LOAD_MODEL_NC_RDO)->EnableWindow(!m_bAutoOptim);
	GetDlgItem(IDC_CMD_MVLD_LOAD_MODEL_NANC_RDO)->EnableWindow(!m_bAutoOptim);
	
	if(m_bAutoOptim && (m_nLoadModel == 2 || m_nLoadModel == 3))
	{
		m_nLoadModel = 0;
	}

	GetDlgItem(IDC_CMD_MVLD_NUM_LOADED_LANE_STC)->EnableWindow(m_nLoadModel == 0);
	GetDlgItem(IDC_CMD_MVLD_NUM_LOADED_LANE_EDT)->EnableWindow(m_nLoadModel == 0);

	UpdateData(FALSE);
}

void CCMMvldItemSouthAfricaDlg::ChangeText()
{
	if     (m_nLoadModel == 0) GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_VEHL1_STC)->SetWindowText(_LS(IDS_CMD_MVLD_SOUTH_AFRICA_NA_VEHICLE));
	else if(m_nLoadModel == 1) GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_VEHL1_STC)->SetWindowText(_LS(IDS_CMD_MVLD_SOUTH_AFRICA_NB_VEHICLE));
	else if(m_nLoadModel == 2) GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_VEHL1_STC)->SetWindowText(_LS(IDS_CMD_MVLD_SOUTH_AFRICA_NC_VEHICLE));
	else if(m_nLoadModel == 3) GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_VEHL1_STC)->SetWindowText(_LS(IDS_CMD_MVLD_SOUTH_AFRICA_NC_VEHICLE));
	else { } // Nothing

	GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_VEHL2_STC)->SetWindowText(_LS(IDS_CMD_MVLD_SOUTH_AFRICA_NA_VEHICLE));
}

void CCMMvldItemSouthAfricaDlg::ChangeLaneList()
{
	m_lstUnSel.ResetContent();
	m_lstSel.ResetContent();
	m_lstNCSel.ResetContent();
	m_lstNASel.ResetContent();

	int nIndex;
	BOOL bFind = FALSE;
	CString LaneName;
	int nAllSize = m_aLaneList.GetSize();

	for (int i=0; i<nAllSize; i++)
	{
		bFind = FALSE;
		LaneName = GetLaneName(m_aLaneList[i]);

		if(m_nLoadModel != 3)
		{
			if (m_aSelFlag[i]) 
			{
				bFind = TRUE;
				nIndex = m_lstSel.AddString(LaneName);
				m_lstSel.SetItemData(nIndex, i);			
			}
		}
		else
		{
			if (m_aNCSelFlag[i]) 
			{
				bFind = TRUE;
				nIndex = m_lstNCSel.AddString(LaneName);
				m_lstNCSel.SetItemData(nIndex, i);			
			}
			else if (m_aNASelFlag[i])
			{ 
				bFind = TRUE;
				nIndex = m_lstNASel.AddString(LaneName);
				m_lstNASel.SetItemData(nIndex, i);			
			}
		}

		if(!bFind)
		{
			nIndex = m_lstUnSel.AddString(LaneName);
			m_lstUnSel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemSouthAfricaDlg::Data2Dlg()
{
	m_edtLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_edtDesc.SetWindowText(m_Data.Description);

	m_bAutoOptim = m_Data.bAutoOptimize;
	m_nLoadModel = m_Data.nLoadModel;

	ChangeVehiCombo();

	int nSelect = 0;
	for(int i=0; i<m_cmbVehicle1.GetCount(); i++)
	{
		if(m_Data.Vehicle1 != m_cmbVehicle1.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbVehicle1.SetCurSel(nSelect);

	for(int i=0; i<m_cmbVehicle2.GetCount(); i++)
	{
		if(m_Data.Vehicle2 != m_cmbVehicle2.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbVehicle2.SetCurSel(nSelect);

	m_nCombOption = m_Data.nCombOption;
	m_edtMinVehlDist.SetEditUnit(m_Data.dMinVehlDist);
	int i = 0;
	for( i=0; i<m_cmbOptimLane.GetCount(); i++)
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

	for( i=0; i<m_cmbOptimLane2.GetCount(); i++)
	{
		if(m_Data.OptimizeLane != m_cmbOptimLane2.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbOptimLane2.SetCurSel(i);
	m_edtNumLoadLanes.SetEditUnit(m_Data.nNumLoadedLanes);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nAllSize = m_aLaneList.GetSize();

	int nSelSize = 0;
	for (nSelSize = 0; nSelSize < m_Data.SubLoadCase.aSelectedLanes.GetSize(); nSelSize++)
	{
		if (m_Data.SubLoadCase.aSelectedLanes[nSelSize] == 0) break;
	}

	int nNCSelSize = 0;
	for (nNCSelSize = 0; nNCSelSize < m_Data.aNCLanes.GetSize(); nNCSelSize++)
	{
		if (m_Data.aNCLanes[nNCSelSize] == 0) break;
	}
	
	int nNASelSize = 0;
	for (nNASelSize = 0; nNASelSize < m_Data.aNALanes.GetSize(); nNASelSize++)
	{
		if (m_Data.aNALanes[nNASelSize] == 0) break;
	}

	m_aSelFlag.SetSize(nAllSize);
	m_aNCSelFlag.SetSize(nAllSize);
	m_aNASelFlag.SetSize(nAllSize);
	
	for ( i=0; i<nAllSize; i++)
	{
		BOOL bFoundSel = FALSE;
		BOOL bFoundNCSel = FALSE;
		BOOL bFoundNASel = FALSE;

		for (int j=0; j<nSelSize; j++)
		{
			if (m_aLaneList[i] == m_Data.SubLoadCase.aSelectedLanes[j]) 
			{
				bFoundSel = TRUE; break;
			}
		}
		for (int j=0 ; j<nNCSelSize ; ++j)
		{
			if (m_aLaneList[i] == m_Data.aNCLanes[j])
			{
				bFoundNCSel = TRUE; break;
			}
		}
		for (int j=0 ; j<nNASelSize ; ++j)
		{
			if (m_aLaneList[i] == m_Data.aNALanes[j])
			{
				bFoundNASel = TRUE; break;
			}
		}

		m_aSelFlag[i]   = bFoundSel;
		m_aNCSelFlag[i] = bFoundNCSel;
		m_aNASelFlag[i] = bFoundNASel;
	}

	m_lstUnSel.ResetContent();
	m_lstSel.ResetContent();
	m_lstNCSel.ResetContent();
	m_lstNASel.ResetContent();

	int nIndex;
	BOOL bFind = FALSE;
	CString LaneName;
	
	for (i=0; i<nAllSize; i++)
	{
		bFind = FALSE;
		LaneName = GetLaneName(m_aLaneList[i]);
		
		if (m_aSelFlag[i]) 
		{
			bFind = TRUE;
			nIndex = m_lstSel.AddString(LaneName);
			m_lstSel.SetItemData(nIndex, i);			
		}

		if (m_aNCSelFlag[i]) 
		{
			bFind = TRUE;
			nIndex = m_lstNCSel.AddString(LaneName);
			m_lstNCSel.SetItemData(nIndex, i);			
		}
		else if (m_aNASelFlag[i])
		{ 
			bFind = TRUE;
			nIndex = m_lstNASel.AddString(LaneName);
			m_lstNASel.SetItemData(nIndex, i);			
		}

		if(!bFind)
		{
			nIndex = m_lstUnSel.AddString(LaneName);
			m_lstUnSel.SetItemData(nIndex, i);
		}
	}
	
	UpdateData(FALSE);
}

BOOL CCMMvldItemSouthAfricaDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_edtLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_edtDesc.GetWindowText(m_Data.Description);

	m_Data.bAutoOptimize = m_bAutoOptim;
	m_Data.nLoadModel = m_nLoadModel;
	m_Data.Vehicle1 = m_cmbVehicle1.GetItemData(m_cmbVehicle1.GetCurSel());
	m_Data.Vehicle2 = m_cmbVehicle2.GetItemData(m_cmbVehicle2.GetCurSel());
	m_Data.nCombOption = m_nCombOption;
	m_Data.dMinVehlDist = m_edtMinVehlDist.GetEditValue();
	m_Data.OptimizeLane = m_cmbOptimLane.GetItemData(m_cmbOptimLane.GetCurSel());
	m_Data.nMinNumVehicle = m_edtMinNumVehl.GetEditValue();
	m_Data.nMaxNumVehicle = m_edtMaxNumVehl.GetEditValue();
	if(m_bAutoOptim) m_Data.nCombOption = m_nCombOption2;
	if(m_nLoadModel != 4) m_Data.OptimizeLane = m_cmbOptimLane2.GetItemData(m_cmbOptimLane2.GetCurSel());
	m_Data.nNumLoadedLanes = m_edtNumLoadLanes.GetEditValue();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_Data.SubLoadCase.aSelectedLanes.RemoveAll();
	m_Data.aNCLanes.RemoveAll();
	m_Data.aNALanes.RemoveAll();

	int nAllSize = m_aLaneList.GetSize();
		
	int nCount = 0;
	int nNCCount = 0;
	int nNACount = 0;
	
	for (int i=0; i<nAllSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			m_Data.SubLoadCase.aSelectedLanes.Add(m_aLaneList[i]);
		}

		if (m_aNCSelFlag[i]) 
		{
			m_Data.aNCLanes.Add(m_aLaneList[i]);
		}
		else if (m_aNASelFlag[i]) 
		{
			m_Data.aNALanes.Add(m_aLaneList[i]);
		}
	}

	return TRUE;
}

BOOL CCMMvldItemSouthAfricaDlg::ApplyOrOK()
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
void CCMMvldItemSouthAfricaDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact), _LS(IDS_WG_CMD__ADDD__Lane1), _LS(IDS_WG_CMD__ADDD__Lane2), _LS(IDS_WG_CMD__ADDD__Lane3), _LS(IDS_WG_CMD__ADDD__Lane4), _LS(IDS_WG_CMD__ADDD__Lane5), _LS(IDS_WG_CMD__ADDD__More)};
	int nColWidth[COLCOUNT] = {108, 50, 60, 60, 60, 60, 60, 48};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstSubCase.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstSubCase.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstSubCase.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemSouthAfricaDlg::MakeItemEx()
{
	m_lstSubCase.DeleteAllItems();

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

BOOL CCMMvldItemSouthAfricaDlg::InsertItem(int nIndex, T_MVLD_BASE &Data)
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
			nItem = m_lstSubCase.InsertItem(&lvitem);
		}
		else m_lstSubCase.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemSouthAfricaDlg::DeleteItem(int nIndex, T_MVLD_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_lstSubCase.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemSouthAfricaDlg::ModifyItem(int nIndex, T_MVLD_BASE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Data);
			m_lstSubCase.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemSouthAfricaDlg::DataToStr(int i, T_MVLD_BASE &Data)
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

CString CCMMvldItemSouthAfricaDlg::GetLaneName(UINT nLaneKey)
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

void CCMMvldItemSouthAfricaDlg::SetHeaderTitle_AssignVehl()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle), _LS(IDS_WG_CMD__ADDD__ReducFact)};
	int nColWidth[COLCOUNT_ASSIGNVEHL] = {140, 140};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstAssignVehl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstAssignVehl.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT_ASSIGNVEHL; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstAssignVehl.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemSouthAfricaDlg::MakeItemEx_AssignVehl()
{
	m_lstAssignVehl.DeleteAllItems();

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

BOOL CCMMvldItemSouthAfricaDlg::InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
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
			nItem = m_lstAssignVehl.InsertItem(&lvitem);
		}
		else m_lstAssignVehl.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemSouthAfricaDlg::DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_lstAssignVehl.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemSouthAfricaDlg::ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr_AssignVehl(i, Data);
			m_lstAssignVehl.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemSouthAfricaDlg::DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data)
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

BOOL CCMMvldItemSouthAfricaDlg::IsNAVehicle(T_MVHL_D& Data)
{
	if(Data.bStandard)
	{
		if(Data.VehicleTypeName != _T("NA")) return FALSE;
	}
	else
	{
		if(Data.nSubType != 0) return FALSE;
	}

	return TRUE;
}

BOOL CCMMvldItemSouthAfricaDlg::IsNBVehicle(T_MVHL_D& Data)
{
	if(Data.bStandard)
	{
		if(Data.VehicleTypeName != _T("NB")) return FALSE;
	}
	else
	{
		if(Data.nSubType != 1) return FALSE;
	}

	return TRUE;
}

BOOL CCMMvldItemSouthAfricaDlg::IsNCVehicle(T_MVHL_D& Data)
{
	if(Data.bStandard)
	{
		if(Data.VehicleTypeName != _T("NC")) return FALSE;
	}
	else
	{
		if(Data.nSubType != 2) return FALSE;
	}

	return TRUE;
}

BOOL CCMMvldItemSouthAfricaDlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
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


BEGIN_MESSAGE_MAP(CCMMvldItemSouthAfricaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemSouthAfricaDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY,                     OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_MVLD_OPTIM_CHK,            OnCmdAutoOptimChk)
	ON_BN_CLICKED(IDC_CMD_MVLD_LOAD_MODEL_NA_RDO,    OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_LOAD_MODEL_NB_RDO,    OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_LOAD_MODEL_NC_RDO,    OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_LOAD_MODEL_NANC_RDO,  OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_LOAD_MODEL_USER_RDO,  OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_LANES_ADD_BTN,        OnCmdLaneAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_LANES_DEL_BTN,        OnCmdLaneDelBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_LANES_NC_ADD_BTN,     OnCmdNCLaneAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_LANES_NC_DEL_BTN,     OnCmdNCLaneDelBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_LANES_NA_ADD_BTN,     OnCmdNALaneAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_LANES_NA_DEL_BTN,     OnCmdNALaneDelBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_SUB_CASE_ADD_BTN,     OnCmdSubCaseAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_SUB_CASE_MOD_BTN,     OnCmdSubCaseModBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_SUB_CASE_DEL_BTN,     OnCmdSubCaseDelBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_ADD_BTN,  OnCmdAssignVehlAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_MOD_BTN,  OnCmdAssignVehlModBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_DEL_BTN,  OnCmdAssignVehlDelBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_MVLD_SUB_CASE_LST,  OnDblclkCmdList)
	ON_LBN_DBLCLK(IDC_CMD_MVLD_LANES_UNSEL_LST,      OnDblclkCmdUnSelList)
	ON_LBN_DBLCLK(IDC_CMD_MVLD_LANES_SEL_LST,        OnDblclkCmdSelList)
	ON_LBN_DBLCLK(IDC_CMD_MVLD_LANES_NC_LST,         OnDblclkCmdNCSelList)
	ON_LBN_DBLCLK(IDC_CMD_MVLD_LANES_NA_LST,         OnDblclkCmdNASelList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemSouthAfricaDlg message handlers

BOOL CCMMvldItemSouthAfricaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	
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

	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dMinVehlDist = M_InitValueCurUnit(1.0, KN, M, CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	}

	InitCombo();
	InitUnit();

	SetHeaderTitle();
	SetHeaderTitle_AssignVehl();
	Data2Dlg();
	MakeItemEx();
	MakeItemEx_AssignVehl();

	AlignControl();
	ShowHideControls();
	EnableDisableControls();
	ChangeText();

	if(m_bModify) GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemSouthAfricaDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldItemSouthAfricaDlg::OnCmdApply() 
{
	ApplyOrOK();	
}

void CCMMvldItemSouthAfricaDlg::OnCmdAutoOptimChk() 
{
	InitCombo();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();
	ChangeText();
}

void CCMMvldItemSouthAfricaDlg::OnCmdLoadModelRdo()
{
	int nPreLoadModel = m_nLoadModel;

	UpdateData(TRUE);

	InitCombo();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();
	ChangeText();

	if(nPreLoadModel != m_nLoadModel) ChangeLaneList();
}

void CCMMvldItemSouthAfricaDlg::OnCmdLaneAddBtn() 
{
	if(m_nLoadModel == 3) return;

	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstUnSel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstUnSel.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = TRUE;
		m_lstUnSel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstSel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_lstSel.AddString(csLaneName);
			m_lstSel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemSouthAfricaDlg::OnCmdLaneDelBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstSel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstSel.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_lstSel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstUnSel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFlag[i]) 
		{
			nIndex = m_lstUnSel.AddString(csLaneName);
			m_lstUnSel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemSouthAfricaDlg::OnCmdNCLaneAddBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstUnSel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstUnSel.GetItemData(aIndex[i]);
		m_aNCSelFlag[nIndex] = TRUE;
		m_aNASelFlag[nIndex] = FALSE;
		m_lstUnSel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstNCSel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aNCSelFlag[i]) 
		{
			nIndex = m_lstNCSel.AddString(csLaneName);
			m_lstNCSel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemSouthAfricaDlg::OnCmdNCLaneDelBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstNCSel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;

	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstNCSel.GetItemData(aIndex[i]);
		m_aNCSelFlag[nIndex] = FALSE;
		m_aNASelFlag[nIndex] = FALSE;
		m_lstNCSel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstUnSel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aNCSelFlag[i]) 
		{
			nIndex = m_lstUnSel.AddString(csLaneName);
			m_lstUnSel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemSouthAfricaDlg::OnCmdNALaneAddBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstUnSel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstUnSel.GetItemData(aIndex[i]);
		m_aNASelFlag[nIndex] = TRUE;
		m_aNCSelFlag[nIndex] = FALSE;
		m_lstUnSel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstNASel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aNASelFlag[i]) 
		{
			nIndex = m_lstNASel.AddString(csLaneName);
			m_lstNASel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemSouthAfricaDlg::OnCmdNALaneDelBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstNASel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;

	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstNASel.GetItemData(aIndex[i]);
		m_aNASelFlag[nIndex] = FALSE;
		m_aNCSelFlag[nIndex] = FALSE;		
		m_lstNASel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstUnSel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aNASelFlag[i]) 
		{
			nIndex = m_lstUnSel.AddString(csLaneName);
			m_lstUnSel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemSouthAfricaDlg::OnCmdSubCaseAddBtn() 
{
	UpdateData(TRUE);

	CCMMvldSubItemDlg dlg(this);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemSouthAfricaDlg::OnCmdSubCaseModBtn() 
{
	UpdateData(TRUE);

	int iItem = m_lstSubCase.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
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

void CCMMvldItemSouthAfricaDlg::OnCmdSubCaseDelBtn() 
{
	int iItem = m_lstSubCase.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
	m_Data.arSubLoadData.RemoveAt(iItem);
	m_lstSubCase.DeleteItem(iItem);

	int nCount = m_lstSubCase.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstSubCase.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvldItemSouthAfricaDlg::OnCmdAssignVehlAddBtn() 
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

void CCMMvldItemSouthAfricaDlg::OnCmdAssignVehlModBtn() 
{
	UpdateData(TRUE);

	int iItem = m_lstAssignVehl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
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

void CCMMvldItemSouthAfricaDlg::OnCmdAssignVehlDelBtn() 
{
	int iItem = m_lstAssignVehl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	m_Data.aOptimize.RemoveAt(iItem);
	m_lstAssignVehl.DeleteItem(iItem);

	int nCount = m_lstAssignVehl.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstAssignVehl.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvldItemSouthAfricaDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdSubCaseModBtn();	
	*pResult = 0;
}

void CCMMvldItemSouthAfricaDlg::OnDblclkCmdUnSelList() 
{
	OnCmdLaneAddBtn();
}

void CCMMvldItemSouthAfricaDlg::OnDblclkCmdSelList() 
{
	OnCmdLaneDelBtn();
}

void CCMMvldItemSouthAfricaDlg::OnDblclkCmdNCSelList() 
{
	OnCmdNCLaneDelBtn();
}

void CCMMvldItemSouthAfricaDlg::OnDblclkCmdNASelList() 
{
	OnCmdNALaneDelBtn();
}