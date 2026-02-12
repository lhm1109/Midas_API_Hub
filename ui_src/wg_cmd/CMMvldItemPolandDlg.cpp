// CMMvldItemPolandDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemPolandDlg.h"
#include "CMMvldSubItemDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
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
// CCMMvldItemPolandDlg dialog

CCMMvldItemPolandDlg::CCMMvldItemPolandDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemPolandDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemPolandDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = NULL;

	m_Data.Initialize();
	
	m_bModify = FALSE;
	m_bSurface= FALSE;
	
	m_bPermit = FALSE;
	m_bAutoOptim = FALSE;
	m_nLoadModel = 0;
	m_nCombOption = 1;
	m_nCombOption2 = 1;

	m_aLaneList.RemoveAll();
	m_aSelFlag.RemoveAll();

	m_aLoadModel.RemoveAll();
	m_aLoadModel.Add(IDC_CMD_MVLD_LOAD_MODEL_GRP);
	m_aLoadModel.Add(IDC_CMD_MVLD_LOAD_MODEL_S_RDO);
	m_aLoadModel.Add(IDC_CMD_MVLD_LOAD_MODEL_K_RDO);
	m_aLoadModel.Add(IDC_CMD_MVLD_LOAD_MODEL_MLC_RDO);
	
	m_aVehicle.RemoveAll();
	m_aVehicle.Add(IDC_CMD_MVLD_LOAD_CASE_GRP);
	m_aVehicle.Add(IDC_CMD_MVLD_LOAD_CASE_VEHL_STC);
	m_aVehicle.Add(IDC_CMD_MVLD_LOAD_CASE_VEHL_CMB);

	m_aAssignLane.RemoveAll();
	m_aAssignLane.Add(IDC_CMD_MVLD_LANES_GRP);
	m_aAssignLane.Add(IDC_CMD_MVLD_LANES_UNSEL_STC);
	m_aAssignLane.Add(IDC_CMD_MVLD_LANES_UNSEL_LST);
	m_aAssignLane.Add(IDC_CMD_MVLD_LANES_ADD_BTN);
	m_aAssignLane.Add(IDC_CMD_MVLD_LANES_DEL_BTN);
	m_aAssignLane.Add(IDC_CMD_MVLD_LANES_SEL_STC);
	m_aAssignLane.Add(IDC_CMD_MVLD_LANES_SEL_LST);

	m_aSubCase.RemoveAll();
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_GRP);
	m_aSubCase.Add(IDC_CMD_MVLD_LOAD_EFFECT_GRP);
	m_aSubCase.Add(IDC_CMD_MVLD_COMBINED_RDO);
	m_aSubCase.Add(IDC_CMD_MVLD_INDEPENDENT_RDO);
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_LST);
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_ADD_BTN);
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_MOD_BTN);
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_DEL_BTN);
	
	m_aPermit.RemoveAll();
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_GRP);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_STC);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_CMB);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_REF_LANE_STC);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_REF_LANE_CMB);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_ECCEN_STC);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_ECCEN_EDT);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_ECCEN_UNT);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_FACT_STC);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_FACT_EDT);
	
	m_aMinDist.RemoveAll();
	m_aMinDist.Add(IDC_CMD_MVLD_OPTIM_GRP);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_STC);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_EDT);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_UNT);

	m_aOptim1.RemoveAll();
	m_aOptim1.Add(IDC_CMD_MVLD_LCASE_GRP);
	m_aOptim1.Add(IDC_CMD_MVLD_LANE_STC);
	m_aOptim1.Add(IDC_CMD_MVLD_LANE_CMB);
	m_aOptim1.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_STC);
	m_aOptim1.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_EDT);
	m_aOptim1.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_STC);
	m_aOptim1.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_EDT);
	m_aOptim1.Add(IDC_CMD_MVLD_LOAD_EFFECT_GRP2);
	m_aOptim1.Add(IDC_CMD_MVLD_COMBINED_RDO2);
	m_aOptim1.Add(IDC_CMD_MVLD_INDEPENDENT_RDO2);
	m_aOptim1.Add(IDC_CMD_MVLD_ASSIGN_VEHL_GRP);
	m_aOptim1.Add(IDC_CMD_MVLD_SEL_VEHL_STC);
	m_aOptim1.Add(IDC_CMD_MVLD_SEL_VEHL_CMB);
	m_aOptim1.Add(IDC_CMD_MVLD_SCALE_FACTOR_STC);
	m_aOptim1.Add(IDC_CMD_MVLD_SCALE_FACTOR_EDT);
	m_aOptim1.Add(IDC_CMD_MVLD_ASSIGN_VEHL_LST);
	m_aOptim1.Add(IDC_CMD_MVLD_ASSIGN_VEHL_ADD_BTN);
	m_aOptim1.Add(IDC_CMD_MVLD_ASSIGN_VEHL_MOD_BTN);
	m_aOptim1.Add(IDC_CMD_MVLD_ASSIGN_VEHL_DEL_BTN);
		
	m_aOptim2.RemoveAll();
	m_aOptim2.Add(IDC_CMD_MVLD_LANES_GRP2);	
	m_aOptim2.Add(IDC_CMD_MVLD_LANE_STC2);	
	m_aOptim2.Add(IDC_CMD_MVLD_LANE_CMB2);	
	m_aOptim2.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_STC);
	m_aOptim2.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_EDT);

	m_aOkCancel.RemoveAll();
	m_aOkCancel.Add(IDOK);
	m_aOkCancel.Add(IDCANCEL);
	m_aOkCancel.Add(IDC_CMD_APPLY);		
}

void CCMMvldItemPolandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemPolandDlg)
	DDX_Control(pDX, IDC_CMD_MVLD_NAME_EDT,            m_edtLoadCase);
	DDX_Control(pDX, IDC_CMD_MVLD_DESC_EDT,            m_edtDesc);
	DDX_Check  (pDX, IDC_CMD_MVLD_PERMIT_CHK,          m_bPermit);
	DDX_Check  (pDX, IDC_CMD_MVLD_OPTIM_CHK,           m_bAutoOptim);
	DDX_Radio  (pDX, IDC_CMD_MVLD_LOAD_MODEL_S_RDO,    m_nLoadModel);
	DDX_Control(pDX, IDC_CMD_MVLD_LOAD_CASE_VEHL_CMB,  m_cmbVehicle);
	DDX_Control(pDX, IDC_CMD_MVLD_LANES_UNSEL_LST,     m_lstUnSel);
	DDX_Control(pDX, IDC_CMD_MVLD_LANES_SEL_LST,       m_lstSel);
	DDX_Radio  (pDX, IDC_CMD_MVLD_COMBINED_RDO,        m_nCombOption);
	DDX_Control(pDX, IDC_CMD_MVLD_SUB_CASE_LST,        m_lstSubCase);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_CMB,          m_cmbPermit);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_REF_LANE_CMB, m_cmbRefLane);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_ECCEN_EDT,    m_edtEccen);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_ECCEN_UNT,    m_untEccen);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_FACT_EDT,     m_edtScaleFactor);
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
void CCMMvldItemPolandDlg::InitUnit()
{
	m_edtEccen.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dEccentricity);
	m_untEccen.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dEccentricity);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);
	m_edtMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_untMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_edtMinNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtMaxNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactorOptim.SetUnitType(D_UNITSYS_NONE);
	m_edtNumLoadLanes.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvldItemPolandDlg::InitCombo()
{
	// Vehicle
	m_cmbVehicle.ResetContent();

	T_MVHL_D MvhlD; MvhlD.Initialize();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK; aMvhlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);

	int nIndex = 0;
	T_MVLD_TYPE_KEY TypeKey;

	for(int i=0; i<aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		if(m_nLoadModel == 2)
		{
			if(MvhlD.bStandard)
			{
				if(MvhlD.VehicleTypeName != _T("Tracked Vehicle") &&
					 MvhlD.VehicleTypeName != _T("Wheeled Vehicle")) continue;
			}
			else
			{
				if(MvhlD.nLoadType != 1) continue;
			}
		}
		else
		{
			if(MvhlD.bStandard)
			{
				if(MvhlD.VehicleTypeName != _T("Vehicle K")) continue;
			}
			else
			{
				if( !(MvhlD.nLoadType == 0 && MvhlD.nSubType == 0) ) continue;
			}		
		}
		
		nIndex = m_cmbVehicle.AddString(MvhlD.VehicleLoadName);
		m_cmbVehicle.SetItemData(nIndex, aMvhlK[i]);
	}
	m_cmbVehicle.SetCurSel(0);

	// Permit Vehicle
	m_cmbPermit.ResetContent();

	for(int i=0; i<aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;
		if(!m_pDoc->m_pAttrCtrl->IsPermitVehicle(MvhlD)) continue;

		nIndex = m_cmbPermit.AddString(MvhlD.VehicleLoadName);
		m_cmbPermit.SetItemData(nIndex, aMvhlK[i]);
	}
	m_cmbPermit.SetCurSel(0);

	// Ref. Lane
	m_cmbRefLane.ResetContent();

	for(int i=0; i<m_aLaneList.GetSize(); i++)
	{
		nIndex = m_cmbRefLane.AddString(GetLaneName(m_aLaneList[i]));
		m_cmbRefLane.SetItemData(nIndex, m_aLaneList[i]);
	}
	m_cmbRefLane.SetCurSel(0);
	
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

		if(MvhlD.bStandard)
		{
			if( MvhlD.VehicleTypeName == _T("Vehicle K")       ||
					MvhlD.VehicleTypeName == _T("Tracked Vehicle") ||
				  MvhlD.VehicleTypeName == _T("Wheeled Vehicle")) continue;
		}
		else
		{
			if(MvhlD.nLoadType == 0 && MvhlD.nSubType == 0) continue;
			if(MvhlD.nLoadType == 1) continue;
		}		
		
		nIndex = m_cmbSelVehl.AddString(_T("VL:")+MvhlD.VehicleLoadName);
		TypeKey.typekey.type = 1;
		TypeKey.typekey.key = aMvhlK[i];
		m_cmbSelVehl.SetItemData(nIndex, TypeKey.value);
	}

	m_cmbSelVehl.SetCurSel(0);
}

void CCMMvldItemPolandDlg::AlignControl()
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
	CDlgUtil::CtrlMoveDistXY(this, m_aSubCase, nDistX, nDistY);

	GetDlgItem(IDC_CMD_MVLD_LOAD_MODEL_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVLD_PERMIT_GRP )->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aPermit, nDistX, nDistY);

	if(m_nLoadModel == 0)
	{
		GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP    )->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVLD_LOAD_CASE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP    )->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);		
	}
	
	GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistXY(this, m_aOptim1, nDistX, nDistY);
	
	if(m_nLoadModel == 0) 
	{
		GetDlgItem(IDC_CMD_MVLD_LANES_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANES_GRP2)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aOptim2, nDistX, nDistY);
	}
	else
	{		
		GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANES_GRP2)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistXY(this, m_aOptim2, nDistX, nDistY);		
	}

	if(m_bPermit)
	{
		GetDlgItem(IDC_CMD_MVLD_PERMIT_GRP )->GetWindowRect(rRef);
	}
	else
	{
		if(m_bAutoOptim)
		{
			if(m_nLoadModel == 0) GetDlgItem(IDC_CMD_MVLD_LCASE_GRP )->GetWindowRect(rRef);
			else                  GetDlgItem(IDC_CMD_MVLD_LANES_GRP2)->GetWindowRect(rRef);  
		}
		else
		{
			if(m_nLoadModel == 0) GetDlgItem(IDC_CMD_MVLD_SUB_CASE_GRP)->GetWindowRect(rRef);
			else                  GetDlgItem(IDC_CMD_MVLD_LANES_GRP   )->GetWindowRect(rRef);  
		}
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

void CCMMvldItemPolandDlg::ShowHideControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aLoadModel,     !m_bPermit                                       );
	CDlgUtil::CtrlShowHide(this, m_aVehicle,       !m_bPermit &&                  (m_nLoadModel == 1 || m_nLoadModel == 2));
	CDlgUtil::CtrlShowHide(this, m_aAssignLane,    !m_bPermit && !m_bAutoOptim && (m_nLoadModel == 1 || m_nLoadModel == 2));
	CDlgUtil::CtrlShowHide(this, m_aSubCase,       !m_bPermit && !m_bAutoOptim &&  m_nLoadModel == 0);
	CDlgUtil::CtrlShowHide(this, m_aPermit,         m_bPermit                                       );
	CDlgUtil::CtrlShowHide(this, m_aMinDist,       !m_bPermit &&  m_bAutoOptim                      );
	CDlgUtil::CtrlShowHide(this, m_aOptim1,        !m_bPermit &&  m_bAutoOptim &&  m_nLoadModel == 0);
	CDlgUtil::CtrlShowHide(this, m_aOptim2,        !m_bPermit &&  m_bAutoOptim && (m_nLoadModel == 1 || m_nLoadModel == 2));
}

void CCMMvldItemPolandDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	// Nothing

	UpdateData(FALSE);
}

void CCMMvldItemPolandDlg::ChangeText()
{
	UpdateData(TRUE);

	if(m_bAutoOptim) GetDlgItem(IDC_CMD_MVLD_LOAD_MODEL_S_RDO)->SetWindowText(_LS(IDS_CMD_MVLD_POLAND_LOAD_MODEL_S_OPTIM));
	else             GetDlgItem(IDC_CMD_MVLD_LOAD_MODEL_S_RDO)->SetWindowText(_LS(IDS_CMD_MVLD_POLAND_LOAD_MODEL_S));
}

void CCMMvldItemPolandDlg::Data2Dlg()
{
	m_edtLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_edtDesc.SetWindowText(m_Data.Description);

	m_bPermit = m_Data.bLoadCaseForPermitLoad;
	m_bAutoOptim = m_Data.bAutoOptimize;
	m_nLoadModel = m_Data.nLoadModel;

	if(m_nLoadModel == 2) InitCombo();
	
	int nSelect = 0;
	for(int i=0; i<m_cmbVehicle.GetCount(); i++)
	{
		if(m_Data.Vehicle1 != m_cmbVehicle.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbVehicle.SetCurSel(nSelect);

	m_nCombOption = m_Data.nCombOption;

	nSelect = 0;
	for(int i=0; i<m_cmbPermit.GetCount(); i++)
	{
		if(m_Data.PermitVehicleK != m_cmbPermit.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbPermit.SetCurSel(nSelect);
	
	nSelect = 0;
	for(int i=0; i<m_cmbRefLane.GetCount(); i++)
	{
		if(m_Data.RefLaneK != m_cmbRefLane.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbRefLane.SetCurSel(nSelect);

	m_edtEccen.SetEditUnit(m_Data.dEccentricity);
	m_edtScaleFactor.SetEditUnit(m_Data.dPermitScaleFactor);
	m_edtMinVehlDist.SetEditUnit(m_Data.dMinVehlDist);
	
	nSelect = 0;
	for(int i=0; i<m_cmbOptimLane.GetCount(); i++)
	{
		if(m_Data.OptimizeLane != m_cmbOptimLane.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbOptimLane.SetCurSel(nSelect);

	m_edtMinNumVehl.SetEditUnit(m_Data.nMinNumVehicle);
	m_edtMaxNumVehl.SetEditUnit(m_Data.nMaxNumVehicle);
	m_nCombOption2 = m_Data.nCombOption;
	
	// Default Value
	m_edtScaleFactorOptim.SetWindowText(_T("1.0"));

	nSelect = 0;
	for(int i=0; i<m_cmbOptimLane2.GetCount(); i++)
	{
		if(m_Data.OptimizeLane != m_cmbOptimLane2.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbOptimLane2.SetCurSel(nSelect);

	m_edtNumLoadLanes.SetEditUnit(m_Data.nNumLoadedLanes);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nAllSize = m_aLaneList.GetSize();

	int nSelSize = 0;
	for (nSelSize = 0; nSelSize < m_Data.SubLoadCase.aSelectedLanes.GetSize(); nSelSize++)
	{
		if (m_Data.SubLoadCase.aSelectedLanes[nSelSize] == 0) break;
	}

	m_aSelFlag.SetSize(nAllSize);
	
	for (int i=0; i<nAllSize; i++)
	{
		BOOL bFoundSel = FALSE;

		for (int j=0; j<nSelSize; j++)
		{
			if (m_aLaneList[i] == m_Data.SubLoadCase.aSelectedLanes[j]) 
			{
				bFoundSel = TRUE; break;
			}
		}

		m_aSelFlag[i]   = bFoundSel;
	}

	m_lstUnSel.ResetContent();
	m_lstSel.ResetContent();

	int nIndex;
	BOOL bFind = FALSE;
	CString LaneName;
	
	for (int i=0; i<nAllSize; i++)
	{
		bFind = FALSE;
		LaneName = GetLaneName(m_aLaneList[i]);
		
		if (m_aSelFlag[i]) 
		{
			bFind = TRUE;
			nIndex = m_lstSel.AddString(LaneName);
			m_lstSel.SetItemData(nIndex, i);			
		}

		if(!bFind)
		{
			nIndex = m_lstUnSel.AddString(LaneName);
			m_lstUnSel.SetItemData(nIndex, i);
		}
	}
	
	UpdateData(FALSE);
}

BOOL CCMMvldItemPolandDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_edtLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_edtDesc.GetWindowText(m_Data.Description);

	m_Data.bLoadCaseForPermitLoad = m_bPermit;
	m_Data.bAutoOptimize = m_bAutoOptim;
	m_Data.nLoadModel = m_nLoadModel;
	m_Data.Vehicle1 = m_cmbVehicle.GetItemData(m_cmbVehicle.GetCurSel());
	m_Data.nCombOption = m_nCombOption;
	m_Data.PermitVehicleK = m_cmbPermit.GetItemData(m_cmbPermit.GetCurSel());
	m_Data.RefLaneK = m_cmbRefLane.GetItemData(m_cmbRefLane.GetCurSel());
	m_Data.dEccentricity = m_edtEccen.GetEditValue();
	m_Data.dPermitScaleFactor = m_edtScaleFactor.GetEditValue();
	m_Data.dMinVehlDist = m_edtMinVehlDist.GetEditValue();
	m_Data.OptimizeLane = m_cmbOptimLane.GetItemData(m_cmbOptimLane.GetCurSel());
	m_Data.nMinNumVehicle = m_edtMinNumVehl.GetEditValue();
	m_Data.nMaxNumVehicle = m_edtMaxNumVehl.GetEditValue();
	if(m_bAutoOptim) m_Data.nCombOption = m_nCombOption2;
	if(m_nLoadModel == 1 || m_nLoadModel == 2) m_Data.OptimizeLane = m_cmbOptimLane2.GetItemData(m_cmbOptimLane2.GetCurSel());
	m_Data.nNumLoadedLanes = m_edtNumLoadLanes.GetEditValue();	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_Data.SubLoadCase.aSelectedLanes.RemoveAll();

	int nAllSize = m_aLaneList.GetSize();
		
	int nCount = 0;

	for (int i=0; i<nAllSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			m_Data.SubLoadCase.aSelectedLanes.Add(m_aLaneList[i]);
		}
	}

	return TRUE;
}

BOOL CCMMvldItemPolandDlg::ApplyOrOK()
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
void CCMMvldItemPolandDlg::SetHeaderTitle()
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

void CCMMvldItemPolandDlg::MakeItemEx()
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

BOOL CCMMvldItemPolandDlg::InsertItem(int nIndex, T_MVLD_BASE &Data)
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

BOOL CCMMvldItemPolandDlg::DeleteItem(int nIndex, T_MVLD_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_lstSubCase.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemPolandDlg::ModifyItem(int nIndex, T_MVLD_BASE &Data)
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

CString CCMMvldItemPolandDlg::DataToStr(int i, T_MVLD_BASE &Data)
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

CString CCMMvldItemPolandDlg::GetLaneName(UINT nLaneKey)
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

void CCMMvldItemPolandDlg::SetHeaderTitle_AssignVehl()
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

void CCMMvldItemPolandDlg::MakeItemEx_AssignVehl()
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

BOOL CCMMvldItemPolandDlg::InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
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

BOOL CCMMvldItemPolandDlg::DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_lstAssignVehl.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemPolandDlg::ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
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

CString CCMMvldItemPolandDlg::DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data)
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

BOOL CCMMvldItemPolandDlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
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


BEGIN_MESSAGE_MAP(CCMMvldItemPolandDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemPolandDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY,                     OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_MVLD_PERMIT_CHK,           OnCmdPermitChk)
	ON_BN_CLICKED(IDC_CMD_MVLD_OPTIM_CHK,            OnCmdAutoOptimChk)
	ON_BN_CLICKED(IDC_CMD_MVLD_LOAD_MODEL_S_RDO,     OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_LOAD_MODEL_K_RDO,     OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_LOAD_MODEL_MLC_RDO,   OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_LANES_ADD_BTN,        OnCmdLaneAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_LANES_DEL_BTN,        OnCmdLaneDelBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_SUB_CASE_ADD_BTN,     OnCmdSubCaseAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_SUB_CASE_MOD_BTN,     OnCmdSubCaseModBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_SUB_CASE_DEL_BTN,     OnCmdSubCaseDelBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_ADD_BTN,  OnCmdAssignVehlAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_MOD_BTN,  OnCmdAssignVehlModBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_DEL_BTN,  OnCmdAssignVehlDelBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_MVLD_SUB_CASE_LST,  OnDblclkCmdList)
	ON_LBN_DBLCLK(IDC_CMD_MVLD_LANES_UNSEL_LST,      OnDblclkCmdUnSelList)
	ON_LBN_DBLCLK(IDC_CMD_MVLD_LANES_SEL_LST,        OnDblclkCmdSelList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemPolandDlg message handlers

BOOL CCMMvldItemPolandDlg::OnInitDialog() 
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

	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	int nCodeType = DataMvcd.nCodeType;

	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dMinVehlDist = M_InitValueCurUnit(1.0, KN, M, CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
		m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, nCodeType);
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

void CCMMvldItemPolandDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldItemPolandDlg::OnCmdApply() 
{
	ApplyOrOK();	
}

void CCMMvldItemPolandDlg::OnCmdPermitChk() 
{
	UpdateData(TRUE);
	m_bAutoOptim = FALSE;
	UpdateData(FALSE);

	InitCombo();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();
	ChangeText();
}

void CCMMvldItemPolandDlg::OnCmdAutoOptimChk() 
{
	UpdateData(TRUE);
	m_bPermit = FALSE;
	UpdateData(FALSE);

	InitCombo();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();
	ChangeText();
}

void CCMMvldItemPolandDlg::OnCmdLoadModelRdo()
{
	UpdateData(TRUE);

	InitCombo();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();
	ChangeText();
}

void CCMMvldItemPolandDlg::OnCmdLaneAddBtn() 
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

void CCMMvldItemPolandDlg::OnCmdLaneDelBtn() 
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

void CCMMvldItemPolandDlg::OnCmdSubCaseAddBtn() 
{
	UpdateData(TRUE);

	CCMMvldSubItemDlg dlg(this);
	dlg.m_nLoadModel = m_Data.nLoadModel;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemPolandDlg::OnCmdSubCaseModBtn() 
{
	UpdateData(TRUE);

	int iItem = m_lstSubCase.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
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

void CCMMvldItemPolandDlg::OnCmdSubCaseDelBtn() 
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

void CCMMvldItemPolandDlg::OnCmdAssignVehlAddBtn() 
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

void CCMMvldItemPolandDlg::OnCmdAssignVehlModBtn() 
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

void CCMMvldItemPolandDlg::OnCmdAssignVehlDelBtn() 
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

void CCMMvldItemPolandDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdSubCaseModBtn();	
	*pResult = 0;
}

void CCMMvldItemPolandDlg::OnDblclkCmdUnSelList() 
{
	OnCmdLaneAddBtn();
}

void CCMMvldItemPolandDlg::OnDblclkCmdSelList() 
{
	OnCmdLaneDelBtn();
}