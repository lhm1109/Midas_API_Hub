// CMMvldItemEuroBSNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemEuroBSNewDlg.h"
#include "CMMvldSubItemDlg.h"
#include "CMMvldItemEuroBSRailDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\VehlDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT_ASSIGNVEHL 2

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemEuroBSNewDlg dialog

union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key  : 31;
	} typekey;
};

CCMMvldItemEuroBSNewDlg::CCMMvldItemEuroBSNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemEuroBSNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemEuroBSNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aLoadModelType.RemoveAll();
	m_aLoadModelType.Add(IDC_CMD_LM1_RDO);
	m_aLoadModelType.Add(IDC_CMD_FLM1_RDO);
	m_aLoadModelType.Add(IDC_CMD_LM2_RDO);
	m_aLoadModelType.Add(IDC_CMD_LM1_SPECCIAL_RDO);
	m_aLoadModelType.Add(IDC_CMD_RAILWAY_BRIDGE_RDO);

	m_aSubLoadCase.RemoveAll();
	m_aSubLoadCase.Add(IDC_CMD_SUB_GRUP);
	m_aSubLoadCase.Add(IDC_CMD_LOADING_GRUP);
	m_aSubLoadCase.Add(IDC_CMD_COMB_RDO);
	m_aSubLoadCase.Add(IDC_CMD_INDEPENEDNT_RDO);
	m_aSubLoadCase.Add(IDC_CMD_LIST);
	m_aSubLoadCase.Add(IDC_CMD_BTN_ADD);
	m_aSubLoadCase.Add(IDC_CMD_BTN_MODIFY);
	m_aSubLoadCase.Add(IDC_CMD_BTN_DELETE);

	m_aCombOption.RemoveAll();
	m_aCombOption.Add(IDC_CMD_COMB_RDO);
	m_aCombOption.Add(IDC_CMD_INDEPENEDNT_RDO);

	m_aLM3Special.RemoveAll();
	m_aLM3Special.Add(IDC_CMD_LM3_SPECIAL_STC);
	m_aLM3Special.Add(IDC_CMD_LM3_SPECIAL_CMB);

	m_aStraddLanes.RemoveAll();
	m_aStraddLanes.Add(IDC_CMD_STRAD_LANE_STC);
	m_aStraddLanes.Add(IDC_CMD_STRAD_LANE_LST);
	m_aStraddLanes.Add(IDC_CMD_SUB_STRAD_ADD_BTN);
	m_aStraddLanes.Add(IDC_CMD_SUB_STRAD_DEL_BTN);

	m_aRemainLanes.RemoveAll();
	m_aRemainLanes.Add(IDC_CMD_REMAIN_AREA_STC);
	m_aRemainLanes.Add(IDC_CMD_REMAIN_AREA_LST);
	m_aRemainLanes.Add(IDC_CMD_SUB_REMAIN_ADD_BTN);
	m_aRemainLanes.Add(IDC_CMD_SUB_REMAIN_DEL_BTN);

	m_aFootway.RemoveAll();
	m_aFootway.Add(IDC_CMD_FOOTWAY_STC);
	m_aFootway.Add(IDC_CMD_FOOTWAY_CMB);

	m_aFootwayLanes.RemoveAll();
	m_aFootwayLanes.Add(IDC_CMD_FOOTWAY_LANE_STC);
	m_aFootwayLanes.Add(IDC_CMD_FOOTWAY_LANE_LST);
	m_aFootwayLanes.Add(IDC_CMD_SUB_FOOTWAY_ADD_BTN);
	m_aFootwayLanes.Add(IDC_CMD_SUB_FOOTWAY_DEL_BTN);

	m_aTrainLoad.RemoveAll();
	m_aTrainLoad.Add(IDC_CMD_RAIL_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_BTN);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);

	m_aMinDist.RemoveAll();
	m_aMinDist.Add(IDC_CMD_MVLD_OPTIM_GRP);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_STC);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_EDT);
	m_aMinDist.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_UNT);

	m_aOptimCtrl.RemoveAll();
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

	m_aOptimAssignLane.RemoveAll();
	m_aOptimAssignLane.Add(IDC_CMD_LANES_GRUP2);
	m_aOptimAssignLane.Add(IDC_CMD_MVLD_LANE_STC2);
	m_aOptimAssignLane.Add(IDC_CMD_MVLD_LANE_CMB2);
	m_aOptimAssignLane.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_STC);
	m_aOptimAssignLane.Add(IDC_CMD_MVLD_NUM_LOADED_LANE_EDT);
	m_aOptimAssignLane.Add(IDC_CMD_OPTIM_UNSEL_STC);
	m_aOptimAssignLane.Add(IDC_CMD_OPTIM_UNSEL_LST);

	m_aOptimStraddLane.RemoveAll();
	m_aOptimStraddLane.Add(IDC_CMD_MVLD_LANE_STC3);
	m_aOptimStraddLane.Add(IDC_CMD_MVLD_LANE_CMB3);

	m_aOptimRemain.RemoveAll();
	m_aOptimRemain.Add(IDC_CMD_OPTIM_REMAIN_ADD_BTN);
	m_aOptimRemain.Add(IDC_CMD_OPTIM_REMAIN_DEL_BTN);
	m_aOptimRemain.Add(IDC_CMD_OPTIM_REMAIN_STC);
	m_aOptimRemain.Add(IDC_CMD_OPTIM_REMAIN_LST);

	m_aOptimRemain2.RemoveAll();
	m_aOptimRemain2.Add(IDC_CMD_OPTIM_FOOTWAY_ADD_BTN);
	m_aOptimRemain2.Add(IDC_CMD_OPTIM_FOOTWAY_DEL_BTN);
	m_aOptimRemain2.Add(IDC_CMD_OPTIM_FOOTWAY_STC);
	m_aOptimRemain2.Add(IDC_CMD_OPTIM_FOOTWAY_LST);

	m_aOptimFootway.RemoveAll();
	m_aOptimFootway.Add(IDC_CMD_OPTIM_REMAIN_ADD_BTN2);
	m_aOptimFootway.Add(IDC_CMD_OPTIM_REMAIN_DEL_BTN2);
	m_aOptimFootway.Add(IDC_CMD_OPTIM_REMAIN_STC2);
	m_aOptimFootway.Add(IDC_CMD_OPTIM_REMAIN_LST2);

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	m_bModify = FALSE;
	m_bSurface = FALSE;

	m_aLaneList.RemoveAll();
	m_aSelFlag.RemoveAll();
	m_aAreaFlag.RemoveAll();
	m_aFootFlag.RemoveAll();
	
	m_nLMSelected1 = 0;
	m_nLMSelected3 = 0;
	m_nLM3SpecialSel = 0;
	m_nPreLoadModel = 0;

	m_bAutoOptim = FALSE;
	m_nCombOption2 = 1;
}

void CCMMvldItemEuroBSNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemEuroBSNewDlg)
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME,                 m_wndLoadCase);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION,                   m_wndDesc);
	DDX_Control(pDX, IDC_CMD_LEADING_CHK,                   m_chkLeading);
	DDX_Control(pDX, IDC_CMD_LM1_CMB,                       m_cmbLM1);
	DDX_Control(pDX, IDC_CMD_LM3_CMB,                       m_cmbLM3);
	DDX_Control(pDX, IDC_CMD_LIST,                          m_List);
	DDX_Control(pDX, IDC_CMD_UNSEL_LIST,                    m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST,                      m_wndSelList);
	DDX_Control(pDX, IDC_CMD_AREA_LIST,                     m_wndAreaList);
	DDX_Control(pDX, IDC_CMD_LM3_SPECIAL_CMB,               m_cmbLM3Special);
	DDX_Control(pDX, IDC_CMD_STRAD_LANE_LST,                m_lstStrad);
	DDX_Control(pDX, IDC_CMD_REMAIN_AREA_LST,               m_lstRemain);
	DDX_Control(pDX, IDC_CMD_FOOTWAY_LANE_LST,              m_lstFootway);
	DDX_Control(pDX, IDC_CMD_FOOTWAY_CMB,                   m_cmbFootway);

	DDX_Check  (pDX, IDC_CMD_MVLD_OPTIM_CHK,                m_bAutoOptim);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_EDT,        m_edtMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_UNT,        m_untMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB,                 m_cmbOptimLane);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_NUM_VEHL_EDT,         m_edtMinNumVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_MAX_NUM_VEHL_EDT,         m_edtMaxNumVehl);
	DDX_Radio  (pDX, IDC_CMD_RADIO_COMBINED2,               m_nCombOption2);
	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB,             m_cmbSelVehl);  
	DDX_Control(pDX, IDC_CMD_ASSIGN_VEHL_LIST,              m_AssignVehlList);
	DDX_Control(pDX, IDC_CMD_MVLD_SCALE_FACTOR_EDT,         m_edtScaleFactorOptim);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB2,                m_cmbOptimLane2);
	DDX_Control(pDX, IDC_CMD_MVLD_NUM_LOADED_LANE_EDT,      m_edtNumLoadLanes);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB3,                m_cmbOptimLane3);
	DDX_Control(pDX, IDC_CMD_OPTIM_UNSEL_LST,               m_lstOptimUnsel);
	DDX_Control(pDX, IDC_CMD_OPTIM_REMAIN_LST,              m_lstOptimRemain);
	DDX_Control(pDX, IDC_CMD_OPTIM_REMAIN_LST2,             m_lstOptimRemain2);
	DDX_Control(pDX, IDC_CMD_OPTIM_FOOTWAY_LST,             m_lstOptimFootway);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg implementation functions
BOOL CCMMvldItemEuroBSNewDlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
{
	if(m_nPreLoadModel != m_Data.nLoadModel) 
	{
		m_Data.arSubLoadData.RemoveAll();
		m_List.DeleteAllItems();
	}	
	
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

void CCMMvldItemEuroBSNewDlg::InitCombo()
{
	m_cmbLM1.ResetContent();
	m_cmbFootway.ResetContent();
	m_cmbLM3.ResetContent();
	m_cmbLM3Special.ResetContent();

	if(m_Data.nLoadModel == 0) CDlgUtil::CobxAddItem(m_cmbFootway,    _T("None"), 0);
	if(m_Data.nLoadModel == 2) CDlgUtil::CobxAddItem(m_cmbLM3,        _T("None"), 0);
	if(m_Data.nLoadModel == 3) CDlgUtil::CobxAddItem(m_cmbLM3Special, _T("None"), 0);
		
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVHL_D MvhlD;
	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		if(MvhlD.nLoadType==4)
		{
			if(MvhlD.bStandard)   
			{
				int nLoadType = 0; 
				int nSelVehicle = 0;
				GetIndex(MvhlD, nLoadType, nSelVehicle);
				
				if(m_Data.nLoadModel == 0) // LM1, FLM1
				{
					// RoadBridge - LoadModel1, RoadBridge Fatigue - LoadModel1
					if ( (MvhlD.nStandardCode == 19 && nLoadType == 0) || (MvhlD.nStandardCode == 21 && nLoadType == 0) )
					{
						CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
					
					if(MvhlD.nStandardCode == 20 && nLoadType == 0)
					{
						CDlgUtil::CobxAddItem(m_cmbFootway, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
				}
				else if(m_Data.nLoadModel==2) // LM1, LM3 Multi
				{
					if (MvhlD.nStandardCode == 19 && nLoadType == 0)
					{
						CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
					if(MvhlD.nStandardCode==19 && nLoadType == 2) // LM3
					{
						CDlgUtil::CobxAddItem(m_cmbLM3, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
				}
				else if(m_Data.nLoadModel==3) // LM1 & Special Multi
				{
					if (MvhlD.nStandardCode == 19 && nLoadType ==0)
					{
						CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
					
					if(MvhlD.nStandardCode==19 && nLoadType == 3) // LM3
					{
						CDlgUtil::CobxAddItem(m_cmbLM3Special, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
				}
			}
			else
			{
				if(m_Data.nLoadModel == 0) // LM1, FLM1
				{
					if(MvhlD.nSubType == 0)
					{
						CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
				}
				else if(m_Data.nLoadModel == 2) // LM1, LM3 Multi
				{
					if(MvhlD.nSubType == 0)
					{
						CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
					else if(MvhlD.nSubType == 2)
					{
						CDlgUtil::CobxAddItem(m_cmbLM3, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
				}
				else if(m_Data.nLoadModel==3) // LM1 & Special Multi
				{
					if(MvhlD.nSubType == 0)
					{
						CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
					else if(MvhlD.nSubType == 3)
					{
						CDlgUtil::CobxAddItem(m_cmbLM3Special, MvhlD.VehicleLoadName, aMvhlK[i]);
					}
				}
			}
		}
	}

	if (m_cmbLM1.GetCount() > 0) m_cmbLM1.SetCurSel(0);
	if (m_cmbFootway.GetCount() > 0) m_cmbFootway.SetCurSel(0);
	if (m_cmbLM3.GetCount() > 0) m_cmbLM3.SetCurSel(0);
	if (m_cmbLM3Special.GetCount() > 0) m_cmbLM3Special.SetCurSel(0);

	AdjustComboListBox(m_cmbLM1);
	AdjustComboListBox(m_cmbFootway);
	AdjustComboListBox(m_cmbLM3);
	AdjustComboListBox(m_cmbLM3Special);

	// Loaded Lane
	m_cmbOptimLane.ResetContent();
	m_cmbOptimLane2.ResetContent();
	m_cmbOptimLane3.ResetContent();

	if(m_bSurface)
	{
		T_SLANop_D SlanD; SlanD.Initialize();
		CArray<T_SLANop_K, T_SLANop_K> aSlanK; aSlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetSlanopKeyList(aSlanK);

		for(int i=0; i<aSlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSlanop(aSlanK[i], SlanD)) continue;
			
			if(SlanD.bStradd) 
			{
				m_cmbOptimLane3.SetItemData(m_cmbOptimLane3.AddString(SlanD.LaneName), aSlanK[i]);
			}
			else
			{
				m_cmbOptimLane .SetItemData(m_cmbOptimLane .AddString(SlanD.LaneName), aSlanK[i]);
				m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(SlanD.LaneName), aSlanK[i]);
			}
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

			if(LlanD.bStradd) 
			{
				m_cmbOptimLane3.SetItemData(m_cmbOptimLane3.AddString(LlanD.LineLaneName), aLlanK[i]);
			}
			else
			{
				m_cmbOptimLane .SetItemData(m_cmbOptimLane.AddString(LlanD.LineLaneName), aLlanK[i]);
				m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(LlanD.LineLaneName), aLlanK[i]);
			}
		}
	}

	m_cmbOptimLane .SetCurSel(0);
	m_cmbOptimLane2.SetCurSel(0);
	m_cmbOptimLane3.SetCurSel(0);
	AdjustComboListBox(m_cmbOptimLane);
	AdjustComboListBox(m_cmbOptimLane2);
	AdjustComboListBox(m_cmbOptimLane3);

	m_cmbSelVehl.ResetContent();

	int nIndex;
	T_MVLD_TYPE_KEY TypeKey;

	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;

		if(MvhlD.bStandard)
		{
			if(MvhlD.nStandardCode == D_MVHL_EURO_EN1991_2_2003_ROAD_BRG)
			{
				if(m_Data.nLoadModel == 4) continue;

				if(MvhlD.VehicleTypeName == _T("Load Model 1"))
					continue;
			}
			else if(MvhlD.nStandardCode == D_MVHL_EURO_EN1991_2_2003_FOOTWAY)
			{
				if(m_Data.nLoadModel == 4) continue;
			}
			else if(MvhlD.nStandardCode == D_MVHL_EURO_EN1991_2_2003_ROAD_BRG_FTG)
			{
				if(m_Data.nLoadModel == 4) continue;

				if(MvhlD.VehicleTypeName==_T("Fatigue Load Model 1"))
					continue;
			}
			else if(MvhlD.nStandardCode == D_MVHL_EURO_EN1991_2_2003_RAIL)
			{
				if(m_Data.nLoadModel != 4) continue;
			}
		}
		else
		{
			if(m_Data.nLoadModel == 4)
			{
				if(MvhlD.nSubType < 5 || MvhlD.nSubType > 11)
					continue;
			}
			else
			{
				if(MvhlD.nSubType ==0 || MvhlD.nSubType > 5)
					continue;
			}

			if(m_bAutoOptim)
			{
				if (m_pDoc->m_pAttrCtrl->IsPermitVehicle(MvhlD))
					continue;
			}
		}
		nIndex = m_cmbSelVehl.AddString(_T("VL:")+MvhlD.VehicleLoadName);
		TypeKey.typekey.type = 1;
		TypeKey.typekey.key = aMvhlK[i];
		m_cmbSelVehl.SetItemData(nIndex, TypeKey.value);
	}
	
	m_cmbSelVehl.SetCurSel(0);
	AdjustComboListBox(m_cmbSelVehl);
}

void CCMMvldItemEuroBSNewDlg::AlignControls()
{
	UpdateData(TRUE);

	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;

	int nLoadModel = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelType, nLoadModel);

	GetDlgItem(IDC_CMD_LM3_CMB        )->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_LM3_SPECIAL_CMB)->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aLM3Special, nDistX, nDistY);

	GetDlgItem(IDC_CMD_LM3_CMB    )->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_FOOTWAY_CMB)->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aFootway, nDistX, nDistY);

	GetDlgItem(IDC_CMD_LEADING_CHK)->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_RAIL_STC   )->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aTrainLoad, nDistX, nDistY);

	if(m_bAutoOptim)
	{
		if(nLoadModel == 0 || nLoadModel == 2 || nLoadModel == 3)
		{
			GetDlgItem(IDC_CMD_LANES_GRUP    )->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.top-rMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);

			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_LANES_GRUP2   )->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.bottom-rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimAssignLane, nDistX, nDistY);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimRemain, nDistX, nDistY);

			GetDlgItem(IDC_CMD_OPTIM_REMAIN_LST )->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_OPTIM_FOOTWAY_LST)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.top-rMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimRemain2, nDistX, nDistY);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimFootway, nDistX, nDistY);

			////////////////////////////////////////////////////////////////////////
			GetDlgItem(IDC_CMD_MVLD_LANE_CMB2)->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_LANE_CMB3)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.top-rMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimStraddLane, nDistX, nDistY);
		}		
		else if(nLoadModel == 1)
		{
			GetDlgItem(IDC_CMD_LOAD_CASE_TXT )->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.top-rMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);

			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP )->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.bottom-rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimCtrl, nDistX, nDistY);
		}
		else
		{
			GetDlgItem(IDC_CMD_LOAD_CASE_TXT )->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.top-rMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aMinDist, nDistX, nDistY);

			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP )->GetWindowRect(rTo);
			GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rMove);
			nDistX = rTo.left-rMove.left;
			nDistY = rTo.bottom-rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptimCtrl, nDistX, nDistY);
		}
	}
	else
	{
		GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rTo);
		GetDlgItem(IDC_CMD_SUB_GRUP     )->GetWindowRect(rMove);
		nDistX = rTo.left-rMove.left;
		nDistY = rTo.top-rMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aSubLoadCase, nDistX, nDistY);

		GetDlgItem(IDC_CMD_AREA_LIST     )->GetWindowRect(rTo);
		GetDlgItem(IDC_CMD_STRAD_LANE_LST)->GetWindowRect(rMove);
		nDistX = rTo.left-rMove.left;
		nDistY = rTo.top-rMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aStraddLanes, nDistX, nDistY);

		GetDlgItem(IDC_CMD_AREA_LIST      )->GetWindowRect(rTo);
		GetDlgItem(IDC_CMD_REMAIN_AREA_LST)->GetWindowRect(rMove);
		nDistX = rTo.left-rMove.left;
		nDistY = rTo.bottom-rMove.bottom;
		CDlgUtil::CtrlMoveDistXY(this, m_aRemainLanes, nDistX, nDistY);
		
		GetDlgItem(IDC_CMD_AREA_LIST       )->GetWindowRect(rTo);
		GetDlgItem(IDC_CMD_FOOTWAY_LANE_LST)->GetWindowRect(rMove);
		nDistX = rTo.left-rMove.left;
		nDistY = rTo.top-rMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aFootwayLanes, nDistX, nDistY);
	}
}

void CCMMvldItemEuroBSNewDlg::ShowHideControls()
{
	BOOL bShow = (!m_bAutoOptim && m_Data.nLoadModel!=1 && m_Data.nLoadModel!=4);
	GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->ShowWindow(m_Data.nLoadModel!=1 && m_Data.nLoadModel!=4);
	GetDlgItem(IDC_CMD_LM1_TXT)      ->ShowWindow(m_Data.nLoadModel!=1 && m_Data.nLoadModel!=4);
	GetDlgItem(IDC_CMD_LM1_CMB)      ->ShowWindow(m_Data.nLoadModel!=1 && m_Data.nLoadModel!=4);
	GetDlgItem(IDC_CMD_LM3_TXT)      ->ShowWindow(m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_LM3_CMB)      ->ShowWindow(m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_LANES_GRUP)   ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_LIST_TXT)     ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_UNSEL_LIST)   ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_SELECTED_TXT) ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_SEL_LIST)     ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_AREA_TXT)     ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_AREA_LIST)    ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_SUB_BTN_ADD)  ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_SUB_BTN_DEL)  ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_SUB_BTN_ADD2) ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_SUB_BTN_DEL2) ->ShowWindow(bShow);

	CDlgUtil::CtrlShowHide(this, m_aSubLoadCase,  !m_bAutoOptim && (m_Data.nLoadModel==1 || m_Data.nLoadModel==4));
	CDlgUtil::CtrlShowHide(this, m_aRemainLanes,  !m_bAutoOptim && (m_Data.nLoadModel==0 || m_Data.nLoadModel==3));
	CDlgUtil::CtrlShowHide(this, m_aFootwayLanes, !m_bAutoOptim && m_Data.nLoadModel==0);
	CDlgUtil::CtrlShowHide(this, m_aStraddLanes,  !m_bAutoOptim && m_Data.nLoadModel==3);	
	CDlgUtil::CtrlShowHide(this, m_aFootway,      m_Data.nLoadModel==0);
	CDlgUtil::CtrlShowHide(this, m_aTrainLoad,    m_Data.nLoadModel==4);
	CDlgUtil::CtrlShowHide(this, m_aLM3Special,   m_Data.nLoadModel==3);  

	GetDlgItem(IDC_CMD_AREA_TXT)    ->ShowWindow(!m_bAutoOptim && m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_AREA_LIST)   ->ShowWindow(!m_bAutoOptim && m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_SUB_BTN_ADD2)->ShowWindow(!m_bAutoOptim && m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_SUB_BTN_DEL2)->ShowWindow(!m_bAutoOptim && m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_LEADING_CHK) ->ShowWindow(m_Data.nLoadModel!=4);

	CDlgUtil::CtrlShowHide(this, m_aMinDist,          m_bAutoOptim);
	CDlgUtil::CtrlShowHide(this, m_aOptimCtrl,        m_bAutoOptim && (m_Data.nLoadModel==1 || m_Data.nLoadModel==4));
	CDlgUtil::CtrlShowHide(this, m_aOptimAssignLane,  m_bAutoOptim && (m_Data.nLoadModel==0 || m_Data.nLoadModel==2 || m_Data.nLoadModel==3));
	CDlgUtil::CtrlShowHide(this, m_aOptimRemain,      m_bAutoOptim && (m_Data.nLoadModel==2 || m_Data.nLoadModel==3));
	CDlgUtil::CtrlShowHide(this, m_aOptimRemain2,     m_bAutoOptim && m_Data.nLoadModel==0);
	CDlgUtil::CtrlShowHide(this, m_aOptimFootway,     m_bAutoOptim && m_Data.nLoadModel==0);

	GetDlgItem(IDC_CMD_MVLD_LANE_STC2)->ShowWindow(m_bAutoOptim && (m_Data.nLoadModel==0 || m_Data.nLoadModel==2));
	GetDlgItem(IDC_CMD_MVLD_LANE_CMB2)->ShowWindow(m_bAutoOptim && (m_Data.nLoadModel==0 || m_Data.nLoadModel==2));
	CDlgUtil::CtrlShowHide(this, m_aOptimStraddLane,  m_bAutoOptim && m_Data.nLoadModel==3);
}

void CCMMvldItemEuroBSNewDlg::ControlsEnableDisable()
{
	if(m_bAutoOptim && m_Data.nLoadModel!=4)
	{
		GetDlgItem(IDC_CMD_MVLD_SCALE_FACTOR_EDT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVLD_SCALE_FACTOR_EDT)->EnableWindow(TRUE);
	}
}

void CCMMvldItemEuroBSNewDlg::ResizeWindow()
{
	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;
	
	int nLoadModel = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelType, nLoadModel);  

	if(!m_bAutoOptim)
	{
		if(nLoadModel == 4) GetDlgItem(IDC_CMD_SUB_GRUP  )->GetWindowRect(rTo);
		else                GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rTo);
	}
	else
	{
		if(nLoadModel == 1 || nLoadModel == 4) GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rTo);
		else                                   GetDlgItem(IDC_CMD_LANES_GRUP2   )->GetWindowRect(rTo);
	}

	GetDlgItem(IDOK)->GetWindowRect(rMove);
	nDistY = rTo.bottom-rMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right = rTo.right + globalUtils.ScaleByDPI(8);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMMvldItemEuroBSNewDlg::ChangeCtrlText()
{
	CString strText;
#if defined(_US) || defined(_RUS)
	strText.Format(_T("Ignore %s Factor:"), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_LEADING_CHK)->SetWindowText(strText);
#endif

	CString strMsg = _LS(IDS_CMD_VEHICLE_);
	if(m_Data.nLoadModel==2 || m_Data.nLoadModel==3) strMsg = _LS(IDS_CMD_LM1_);
	GetDlgItem(IDC_CMD_LM1_TXT)->SetWindowText(strMsg);
}

void CCMMvldItemEuroBSNewDlg::MaintainVehicleIndex()
{
	if     (m_Data.nLoadModel == 0) m_nLMSelected1 = m_cmbLM1.GetCurSel();
	else if(m_Data.nLoadModel == 2) m_nLMSelected3 = m_cmbLM3.GetCurSel();
	else if(m_Data.nLoadModel == 3) m_nLM3SpecialSel = m_cmbLM3Special.GetCurSel();

	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelType, m_Data.nLoadModel);

	if(m_Data.nLoadModel == 0)
	{
		if(m_cmbLM1.GetCount() > m_nLMSelected1)
			m_cmbLM1.SetCurSel(m_nLMSelected1);
	}
	else if(m_Data.nLoadModel == 2)
	{
		if(m_cmbLM3.GetCount() > m_nLMSelected3)
			m_cmbLM3.SetCurSel(m_nLMSelected3 );
	}
	else if(m_Data.nLoadModel == 3)
	{
		if(m_cmbLM3Special.GetCount() > m_nLM3SpecialSel)
			m_cmbLM3Special.SetCurSel(m_nLM3SpecialSel );
	}
}

void CCMMvldItemEuroBSNewDlg::Data2Dlg()
{
	m_wndLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.SetWindowText(m_Data.Description);

	CDlgUtil::CtrlRadioSetCheck(this, m_aLoadModelType, m_Data.nLoadModel);

	if(m_Data.VehicleK1 > 0 && m_cmbLM1.GetCount() > 0)
		CDlgUtil::CobxSetCurSelItemData(m_cmbLM1, (DWORD&)m_Data.VehicleK1);

	if(m_Data.nLoadModel == 0)
	{
		if(m_Data.FootwayK >0 && m_cmbFootway.GetCount() > 0)
			CDlgUtil::CobxSetCurSelItemData(m_cmbFootway, (DWORD&)m_Data.FootwayK);
	}

	if(m_Data.nLoadModel == 2)
	{
		if(m_Data.VehicleK3 >0 && m_cmbLM3.GetCount() > 0)
			CDlgUtil::CobxSetCurSelItemData(m_cmbLM3, (DWORD&)m_Data.VehicleK3);
	}

	if(m_Data.nLoadModel == 3)
	{
		if(m_Data.VehicleSpecial >0 && m_cmbLM3Special.GetCount() > 0)
			CDlgUtil::CobxSetCurSelItemData(m_cmbLM3Special, (DWORD&)m_Data.VehicleSpecial);
	}

	Data2Dlg_LaneListByVehicleIndex();

	m_chkLeading.SetCheck(m_Data.bLeading);
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCombOption, m_Data.nCombOption);
	MakeItemEx();
	MakeItemEx_AssignVehl();

	m_bAutoOptim = m_Data.bAutoOptimize;
	m_edtMinVehlDist.SetEditUnit(m_Data.dMinVehlDist);
	int i = 0;
	for(i=0; i<m_cmbOptimLane.GetCount(); i++)
	{
		if(m_Data.OptimizeLane != m_cmbOptimLane.GetItemData(i)) continue;
		break;
	}
	m_cmbOptimLane.SetCurSel(i);

	for(i=0; i<m_cmbOptimLane2.GetCount(); i++)
	{
		if(m_Data.OptimizeLane != m_cmbOptimLane2.GetItemData(i)) continue;
		break;
	}
	m_cmbOptimLane2.SetCurSel(i);

	for(i=0; i<m_cmbOptimLane3.GetCount(); i++)
	{
		if(m_Data.OptimizeLane2 != m_cmbOptimLane3.GetItemData(i)) continue;
		break;
	}
	m_cmbOptimLane3.SetCurSel(i);

	m_edtMinNumVehl.SetEditUnit(m_Data.nMinNumVehicle);
	m_edtMaxNumVehl.SetEditUnit(m_Data.nMaxNumVehicle);
	m_nCombOption2 = m_Data.nCombOption;
	m_edtNumLoadLanes.SetEditUnit(m_Data.nNumLoadedLanes);

	// Default Value
	m_edtScaleFactorOptim.SetWindowText(_T("1.0"));
	
	UpdateData(FALSE);
}

void CCMMvldItemEuroBSNewDlg::Data2Dlg_LaneListByVehicleIndex()
{
	int nAllSize = m_aLaneList.GetSize();
	int nSelSize = 0;
	for (nSelSize = 0; nSelSize < m_Data.SubLoadCase.aSelectedLanes.GetSize(); nSelSize++)
	{
		if (m_Data.SubLoadCase.aSelectedLanes[nSelSize] == 0) break;
	}
	
	int nFootSize = 0;
	if(m_Data.nLoadModel==0)
	{
		for (nFootSize = 0; nFootSize < m_Data.aFootwayLanes.GetSize(); nFootSize++)
		{
			if (m_Data.aFootwayLanes[nFootSize] == 0) break;
		}
	}
	
	int nAreaSize = 0;
	for (nAreaSize = 0; nAreaSize < m_Data.aSelectedRemainingArea.GetSize(); nAreaSize++)
	{
		if (m_Data.aSelectedRemainingArea[nAreaSize] == 0) break;
	}

	int nStrdSize = 0;
	if(m_Data.nLoadModel==3)
	{
		if (m_bAutoOptim) 
		{
			m_Data.aStraddLanes.RemoveAll();
		}

		for (nStrdSize = 0; nStrdSize < m_Data.aStraddLanes.GetSize(); nStrdSize++)
		{
			if (m_Data.aStraddLanes[nStrdSize].Lane1 == 0 || m_Data.aStraddLanes[nStrdSize].Lane2 == 0) break;
		}
	}
	
	m_aSelFlag.SetSize(nAllSize);
	m_aFootFlag.SetSize(nAllSize);
	m_aAreaFlag.SetSize(nAllSize);

	CArray<BOOL, BOOL> aStrdFlag;
	aStrdFlag.SetSize(nAllSize);
	
	for (int i=0; i<nAllSize; i++)
	{
		BOOL bFoundInSel = FALSE;
		BOOL bFoundInFoot = FALSE;
		BOOL bFoundInArea = FALSE;
		BOOL bFoundInStrd = FALSE;
		
		for (int j=0; j<nSelSize; j++)
		{
			if (m_aLaneList[i] == m_Data.SubLoadCase.aSelectedLanes[j]) 
			{
				bFoundInSel = TRUE; break;
			}
		}
		for (int j=0 ; j<nFootSize ; ++j) 
		{
			if (m_aLaneList[i] == m_Data.aFootwayLanes[j])
			{
				bFoundInFoot = TRUE; break;
			}
		}
		for (int j=0 ; j<nAreaSize ; ++j)
		{
			if (m_aLaneList[i] == m_Data.aSelectedRemainingArea[j])
			{
				bFoundInArea = TRUE; break;
			}
		}
		for (int j=0 ; j<nStrdSize ; ++j)
		{
			if (m_aLaneList[i] == m_Data.aStraddLanes[j].Lane1 || m_aLaneList[i] == m_Data.aStraddLanes[j].Lane2)
			{
				bFoundInStrd = TRUE; break;
			}
		}
		m_aSelFlag[i] = bFoundInSel;
		m_aFootFlag[i] = bFoundInFoot;
		m_aAreaFlag[i] = bFoundInArea;
		aStrdFlag[i] = bFoundInStrd;
	}
	
	int nIndex;
	
	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();
	m_wndAreaList.ResetContent(); 
	m_lstStrad.ResetContent();
	m_lstFootway.ResetContent();
	m_lstRemain.ResetContent();

	m_lstOptimUnsel.ResetContent();
	m_lstOptimRemain.ResetContent();
	m_lstOptimRemain2.ResetContent();
	m_lstOptimFootway.ResetContent();

	// Add String
	CString LaneName;
	for (int i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(LaneName);
			m_wndSelList.SetItemData(nIndex, i);

			// Optimize 에는 Select Lane 이 없다.
			nIndex = m_lstOptimUnsel.AddString(LaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);
		}
		else if (m_aAreaFlag[i]) 
		{
			nIndex = m_wndAreaList.AddString(LaneName);
			m_wndAreaList.SetItemData(nIndex, i);
			
			nIndex = m_lstRemain.AddString(LaneName);
			m_lstRemain.SetItemData(nIndex, i);

			nIndex = m_lstOptimRemain.AddString(LaneName);
			m_lstOptimRemain.SetItemData(nIndex, i);

			nIndex = m_lstOptimRemain2.AddString(LaneName);
			m_lstOptimRemain2.SetItemData(nIndex, i);
		}
		else if (m_aFootFlag[i])
		{
			nIndex = m_lstFootway.AddString(LaneName);
			m_lstFootway.SetItemData(nIndex, i);

			nIndex = m_lstOptimFootway.AddString(LaneName);
			m_lstOptimFootway.SetItemData(nIndex, i);
		}
		else if (aStrdFlag[i])
		{
			continue;
		}
		else
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);

			nIndex = m_lstOptimUnsel.AddString(LaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);
		}
	}

	T_LLAN_EURO_NEW_KEY key;
	m_aSelLlan.RemoveAll();
	for (int i=0; i<m_Data.aStraddLanes.GetSize(); i++)
	{
		if (m_Data.aStraddLanes[i].Lane1 == 0 || m_Data.aStraddLanes[i].Lane2 == 0) break;
		
		key.key.key1 = m_Data.aStraddLanes[i].Lane1;
		key.key.key2 = m_Data.aStraddLanes[i].Lane2;
		
		LaneName = GetLaneName(key.key.key1);
		LaneName += _T(" : ");
		LaneName += GetLaneName(key.key.key2);
		nIndex = m_lstStrad.AddString(LaneName);
		m_lstStrad.SetItemData(nIndex, key.keymap);
		
		m_aSelLlan.Add(key);
	}
}

BOOL CCMMvldItemEuroBSNewDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_wndLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_wndDesc.GetWindowText(m_Data.Description);

	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelType, m_Data.nLoadModel);

	if(m_Data.nLoadModel == 0 || m_Data.nLoadModel == 2)
	{
		if(m_cmbLM1.GetCount() > 0)
			CDlgUtil::CobxGetItemDataByCurSel(m_cmbLM1, (DWORD&)m_Data.VehicleK1);

		if(m_Data.nLoadModel == 0)
		{
			if(m_cmbFootway.GetCount() > 0)
				CDlgUtil::CobxGetItemDataByCurSel(m_cmbFootway, (DWORD&)m_Data.FootwayK);
		}

		if(m_Data.nLoadModel == 2)
		{
			if(m_cmbLM3.GetCount() > 0)
				CDlgUtil::CobxGetItemDataByCurSel(m_cmbLM3, (DWORD&)m_Data.VehicleK3);
		}
		m_Data.SubLoadCase.Initialize();
		int nSize = m_aLaneList.GetSize();
		int nCount = 0;
		int nCountFoot = 0;
		int nCountArea = 0;
		// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
		int nMaxLane = 20;
		//for (int j=0 ; j<nMax ; ++j) m_Data.aSelectedRemainingArea[j] = 0;
		m_Data.aSelectedRemainingArea.RemoveAll();
		m_Data.aFootwayLanes.RemoveAll();

		for (int i=0; i<nSize; i++)
		{
			if (m_aSelFlag[i]) 
			{
				if (nCount >= nMaxLane)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMaxLane);
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.SubLoadCase.aSelectedLanes.Add(m_aLaneList[i]);
				nCount++;
			}
			else if (m_aAreaFlag[i]) 
			{
				if (nCountArea >= nMaxLane)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX_AREA), nMaxLane); 
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aSelectedRemainingArea.Add(m_aLaneList[i]);
				nCountArea++;
			}
			else if (m_aFootFlag[i]) 
			{
				if (nCountFoot >= nMaxLane)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX_AREA), nMaxLane); 
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aFootwayLanes.Add(m_aLaneList[i]);
				nCountFoot++;
			}
		}
	}
	else if(m_Data.nLoadModel == 1 || m_Data.nLoadModel == 4)
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aCombOption, m_Data.nCombOption);
	}
	else if(m_Data.nLoadModel == 3)
	{
		if(m_cmbLM1.GetCount() > 0)
			CDlgUtil::CobxGetItemDataByCurSel(m_cmbLM1, (DWORD&)m_Data.VehicleK1);
		if(m_cmbLM3Special.GetCount() > 0)
			CDlgUtil::CobxGetItemDataByCurSel(m_cmbLM3Special, (DWORD&)m_Data.VehicleSpecial);

		m_Data.SubLoadCase.Initialize();
		int nSize = m_aLaneList.GetSize();
		int nCount = 0;
		int nCountArea = 0;
		// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
		int nMaxLane = 20;
		//for (int j=0 ; j<nMax ; ++j) m_Data.aSelectedRemainingArea[j] = 0;
		m_Data.aSelectedRemainingArea.RemoveAll();

		for (int i=0; i<nSize; i++)
		{
			if (m_aSelFlag[i]) 
			{
				if (nCount >= nMaxLane)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMaxLane);
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.SubLoadCase.aSelectedLanes.Add(m_aLaneList[i]);
				nCount++;
			}
			else if (m_aAreaFlag[i]) 
			{
				if (nCountArea >= nMaxLane)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX_AREA), nMaxLane); 
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aSelectedRemainingArea.Add(m_aLaneList[i]);
				nCountArea++;
			}
		}

		T_LLAN_EURO_NEW_KEY key;
		// 초기화
		m_Data.aStraddLanes.RemoveAll();
		m_Data.aStraddLanes.SetSize(m_aSelLlan.GetSize());

		for (int i=0; i<m_aSelLlan.GetSize(); i++)
		{
			key = m_aSelLlan[i];
			m_Data.aStraddLanes[i].Lane1 = key.key.key1;
			m_Data.aStraddLanes[i].Lane2 = key.key.key2;
		}
	}

	m_Data.bLeading = m_chkLeading.GetCheck();

	m_Data.bAutoOptimize = m_bAutoOptim;
	m_Data.dMinVehlDist = m_edtMinVehlDist.GetEditValue();

	if(m_Data.nLoadModel == 1 || m_Data.nLoadModel == 4)
	{
		m_Data.OptimizeLane = m_cmbOptimLane.GetItemData(m_cmbOptimLane.GetCurSel());
	}
	else
	{
		m_Data.OptimizeLane = m_cmbOptimLane2.GetItemData(m_cmbOptimLane2.GetCurSel());
	}

	m_Data.nNumLoadedLanes = m_edtNumLoadLanes.GetEditValue();
	m_Data.OptimizeLane2 = m_cmbOptimLane3.GetItemData(m_cmbOptimLane3.GetCurSel());
	m_Data.nMinNumVehicle = m_edtMinNumVehl.GetEditValue();
	m_Data.nMaxNumVehicle = m_edtMaxNumVehl.GetEditValue();
	if(m_Data.bAutoOptimize) m_Data.nCombOption = m_nCombOption2;
	
	return TRUE;
}

void CCMMvldItemEuroBSNewDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle), _LS(IDS_WG_CMD__ADDD__ReducFact), _LS(IDS_WG_CMD__ADDD__Lane1), _LS(IDS_WG_CMD__ADDD__Lane2), _LS(IDS_WG_CMD__ADDD__Lane3), _LS(IDS_WG_CMD__ADDD__Lane4), _LS(IDS_WG_CMD__ADDD__Lane5), _LS(IDS_WG_CMD__ADDD__More)};
	int nColWidth[COLCOUNT] = {108, 50, 60, 60, 60, 60, 60, 48};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	///////////////////////////////////////////////////////////////////////////////////
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	///////////////////////////////////////////////////////////////////////////////////

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

void CCMMvldItemEuroBSNewDlg::MakeItemEx()
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

BOOL CCMMvldItemEuroBSNewDlg::InsertItem(int nIndex, T_MVLD_BASE &Data)
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
		else 
		{
			m_List.SetItem(&lvitem);
		}

		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CCMMvldItemEuroBSNewDlg::DeleteItem(int nIndex, T_MVLD_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0)
	{
		m_List.DeleteItem(nIndex);	
	}
	return TRUE;
}

BOOL CCMMvldItemEuroBSNewDlg::ModifyItem(int nIndex, T_MVLD_BASE &Data)
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

BOOL CCMMvldItemEuroBSNewDlg::IsDuplicatedItem(T_LLAN_EURO_NEW_KEY& key)
{
	int nNum = m_aSelLlan.GetSize();
	for (int i=0; i<nNum; i++)
	{
		if ((m_aSelLlan[i].key.key1 == key.key.key1 && m_aSelLlan[i].key.key2 == key.key.key2) ||
				(m_aSelLlan[i].key.key1 == key.key.key2 && m_aSelLlan[i].key.key2 == key.key.key1)) 
				return TRUE;
	}
	return FALSE;
}

CString CCMMvldItemEuroBSNewDlg::DataToStr(int i, T_MVLD_BASE &Data)
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

CString CCMMvldItemEuroBSNewDlg::GetLaneName(UINT nLaneKey)
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

int CCMMvldItemEuroBSNewDlg::GetIndex(T_MVHL_D &MvhlD, int &nLoadType, int &nSelVehicle)
{ 
	int nCode = -1;
	
	// EN 1991-2:2003 - RoadBridge
	if     (MvhlD.VehicleTypeName == _T("Load Model 1")) { nLoadType = 0; nSelVehicle = 0; nCode = 0; }
	else if(MvhlD.VehicleTypeName == _T("Load Model 2")) { nLoadType = 1; nSelVehicle = 0; nCode = 0; }
	else if(MvhlD.VehicleTypeName == _T("Load Model 3"))
	{
		if     (MvhlD.SelVehicle == _T("600/150"))      { nLoadType = 2; nSelVehicle = 0; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("900/150"))      { nLoadType = 2; nSelVehicle = 1; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("1200/150/200")) { nLoadType = 2; nSelVehicle = 2; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("1500/150/200")) { nLoadType = 2; nSelVehicle = 3; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("1800/150/200")) { nLoadType = 2; nSelVehicle = 4; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("2400/200"))     { nLoadType = 2; nSelVehicle = 5; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("3000/200"))     { nLoadType = 2; nSelVehicle = 6; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("3600/200"))     { nLoadType = 2; nSelVehicle = 7; nCode = 0; }
		else { ASSERT(0); }
	}
	else if(MvhlD.VehicleTypeName == _T("Load Model 3 (UK NA)"))
	{
		if     (MvhlD.SelVehicle == _T("SV 80"))   { nLoadType = 3; nSelVehicle = 0; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("SV 100"))  { nLoadType = 3; nSelVehicle = 1; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("SV 196"))  { nLoadType = 3; nSelVehicle = 2; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("SOV 250")) { nLoadType = 3; nSelVehicle = 3; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("SOV 350")) { nLoadType = 3; nSelVehicle = 4; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("SOV 450")) { nLoadType = 3; nSelVehicle = 5; nCode = 0; }
		else if(MvhlD.SelVehicle == _T("SOV 600")) { nLoadType = 3; nSelVehicle = 6; nCode = 0; }
		else { ASSERT(0); }
	}
	else if(MvhlD.VehicleTypeName == _T("Load Model 4")) { nLoadType = 4; nSelVehicle = 0; nCode = 0; }
	// EN 1991-2:2003 - Footway and FootBridge
	else if(MvhlD.VehicleTypeName == _T("Uniform load (Road bridge footway)")) { nLoadType = 0; nSelVehicle = 0; nCode = 1; }
	else if(MvhlD.VehicleTypeName == _T("Uniform load (Footbridge)"))          { nLoadType = 1; nSelVehicle = 0; nCode = 1; }
	else if(MvhlD.VehicleTypeName == _T("Concentrated Load"))                  { nLoadType = 2; nSelVehicle = 0; nCode = 1; }
	else if(MvhlD.VehicleTypeName == _T("Uniform load (Road bridge footway) UK NA"))    { nLoadType = 3; nSelVehicle = 0; nCode = 1; }
	// EN 1991-2:2003 - RoadBridge Fatigue
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 1"))       { nLoadType = 0;  nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 2 (280)")) { nLoadType = 1;  nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 2 (360)")) { nLoadType = 2;  nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 2 (630)")) { nLoadType = 3;  nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 2 (560)")) { nLoadType = 4;  nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 2 (610)")) { nLoadType = 5;  nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 3 (One Vehicle)")) { nLoadType = 6; nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 3 (Two Vehicle)")) { nLoadType = 7; nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 4 (200)")) { nLoadType = 8;  nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 4 (310)")) { nLoadType = 9;  nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 4 (490)")) { nLoadType = 10; nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 4 (390)")) { nLoadType = 11; nSelVehicle = 0; nCode = 2; }
	else if(MvhlD.VehicleTypeName == _T("Fatigue Load Model 4 (450)")) { nLoadType = 12; nSelVehicle = 0; nCode = 2; }
	// EN 1991-2:2003 - Rail Traffic Load
	else if(MvhlD.VehicleTypeName == _T("Load Model 71"))   { nLoadType = 0; nSelVehicle = 0; nCode = 3; }
	else if(MvhlD.VehicleTypeName == _T("Load Model SW/0")) { nLoadType = 1; nSelVehicle = 0; nCode = 3; }
	else if(MvhlD.VehicleTypeName == _T("Load Model SW/2")) { nLoadType = 2; nSelVehicle = 0; nCode = 3; }
	else if(MvhlD.VehicleTypeName == _T("Unloaded Train"))  { nLoadType = 3; nSelVehicle = 0; nCode = 3; }
	else if(MvhlD.VehicleTypeName == _T("HSLM A1 ~ HSLM A10"))
	{
		if     (MvhlD.SelVehicle == _T("A1"))  { nLoadType = 4; nSelVehicle = 0; nCode = 3; }
		else if(MvhlD.SelVehicle == _T("A2"))  { nLoadType = 4; nSelVehicle = 1; nCode = 3; }
		else if(MvhlD.SelVehicle == _T("A3"))  { nLoadType = 4; nSelVehicle = 2; nCode = 3; }
		else if(MvhlD.SelVehicle == _T("A4"))  { nLoadType = 4; nSelVehicle = 3; nCode = 3; }
		else if(MvhlD.SelVehicle == _T("A5"))  { nLoadType = 4; nSelVehicle = 4; nCode = 3; }
		else if(MvhlD.SelVehicle == _T("A6"))  { nLoadType = 4; nSelVehicle = 5; nCode = 3; }
		else if(MvhlD.SelVehicle == _T("A7"))  { nLoadType = 4; nSelVehicle = 6; nCode = 3; }
		else if(MvhlD.SelVehicle == _T("A8"))  { nLoadType = 4; nSelVehicle = 7; nCode = 3; }
		else if(MvhlD.SelVehicle == _T("A9"))  { nLoadType = 4; nSelVehicle = 8; nCode = 3; }
		else if(MvhlD.SelVehicle == _T("A10")) { nLoadType = 4; nSelVehicle = 9; nCode = 3; }
		else { ASSERT(0); }
	}
	else if(MvhlD.VehicleTypeName == _T("HSLM B")) { nLoadType = 5; nSelVehicle = 0; nCode = 3; }
	else { ASSERT(0); }
	
	if(nCode < 0) AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
	
	return nCode;

}

BOOL CCMMvldItemEuroBSNewDlg::ApplyOrOK()
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


BEGIN_MESSAGE_MAP(CCMMvldItemEuroBSNewDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemEuroBSNewDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_LM1_RDO, OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_FLM1_RDO, OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_LM2_RDO, OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_LM1_SPECCIAL_RDO, OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_RAILWAY_BRIDGE_RDO, OnCmdLoadModelRdo)
	
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_ADD, OnCmdSubBtnAdd)
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_DEL, OnCmdSubBtnDelete)
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_ADD2, OnCmdSubBtnAdd2) 
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_DEL2, OnCmdSubBtnDelete2) 
	ON_BN_CLICKED(IDC_CMD_SUB_STRAD_ADD_BTN,  OnCmdSubStradAddBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_STRAD_DEL_BTN,  OnCmdSubStradDelBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_REMAIN_ADD_BTN, OnCmdSubReaminAddBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_REMAIN_DEL_BTN, OnCmdSubReaminDelBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_FOOTWAY_ADD_BTN, OnCmdSubFootwayAddBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_FOOTWAY_DEL_BTN, OnCmdSubFootwayDelBtn)
	ON_BN_CLICKED(IDC_CMD_OPTIM_REMAIN_ADD_BTN,  OnCmdSubOptimRemainAddBtn)
	ON_BN_CLICKED(IDC_CMD_OPTIM_REMAIN_DEL_BTN,  OnCmdSubOptimRemainDelBtn)
	ON_BN_CLICKED(IDC_CMD_OPTIM_REMAIN_ADD_BTN2,  OnCmdSubOptimRemain2AddBtn)
	ON_BN_CLICKED(IDC_CMD_OPTIM_REMAIN_DEL_BTN2,  OnCmdSubOptimRemain2DelBtn)
	ON_BN_CLICKED(IDC_CMD_OPTIM_FOOTWAY_ADD_BTN,  OnCmdSubOptimFootwayAddBtn)
	ON_BN_CLICKED(IDC_CMD_OPTIM_FOOTWAY_DEL_BTN,  OnCmdSubOptimFootwayDelBtn)
	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST, OnDblclkCmdSubSelList)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST, OnDblclkCmdSubUnselList)
	ON_LBN_DBLCLK(IDC_CMD_AREA_LIST, OnDblclkCmdSubAreaList) 
	ON_LBN_DBLCLK(IDC_CMD_STRAD_LANE_LST, OnDblclkCmdSubStradList)
	ON_LBN_DBLCLK(IDC_CMD_REMAIN_AREA_LST, OnDblclkCmdSubRemainList)
	ON_LBN_DBLCLK(IDC_CMD_FOOTWAY_LANE_LST, OnDblclkCmdSubFootwayList)
	ON_LBN_DBLCLK(IDC_CMD_OPTIM_REMAIN_LST, OnDblclkCmdSubOptimRemainList)
	ON_LBN_DBLCLK(IDC_CMD_OPTIM_REMAIN_LST2, OnDblclkCmdSubOptimRemain2List)
	ON_LBN_DBLCLK(IDC_CMD_OPTIM_FOOTWAY_LST, OnDblclkCmdSubOptimFootwayList)
	ON_BN_CLICKED(IDC_CMD_RAIL_BTN, OnCmdRailLoadDataBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_OPTIM_CHK, OnCmdOptmChk)  
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_ADD_BTN, OnCmdBtnAdd_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_MOD_BTN, OnCmdBtnModify_AssignVehl)
	ON_BN_CLICKED(IDC_CMD_ASSIGN_VEHL_DEL_BTN, OnCmdBtnDelete_AssignVehl)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemEuroBSNewDlg message handlers

BOOL CCMMvldItemEuroBSNewDlg::OnInitDialog() 
{
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
	
	if(!m_bModify)
	{
		m_Data.Initialize();
		
		m_Data.dScaleFactor[0] = 0.8;
		m_Data.dScaleFactor[1] = 0.7;
		m_Data.dScaleFactor[2] = 0.6;
		m_Data.dMultipleFactor1 = 1.0;
		m_Data.dMultipleFactor2 = 1.0;
		m_Data.dMultipleFactor3 = 0.75;
		m_Data.dMinVehlDist = M_InitValueCurUnit(1.0, KN, M, CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	}

	SetHeaderTitle();
	SetHeaderTitle_AssignVehl();
	m_bAutoOptim = m_Data.bAutoOptimize;
	InitCombo();
	Data2Dlg();

	AlignControls();
	ShowHideControls();
	ControlsEnableDisable();
	ResizeWindow();
	ChangeCtrlText();
	
	m_nPreLoadModel = m_Data.nLoadModel;

#if defined(_MGEN)
	GetDlgItem(IDC_CMD_FLM1_RDO)->SetWindowText(_LS(IDS_WG_CMD_FLM1_RDO_OLD));
	GetDlgItem(IDC_CMD_RAILWAY_BRIDGE_RDO)->SetWindowText(_LS(IDS_WG_CMD_RAILWAY_BRIDGE_RDO_OLD));

	GetDlgItem(IDC_CMD_MVLD_OPTIM_CHK)->EnableWindow(FALSE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemEuroBSNewDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CCMMvldItemEuroBSNewDlg::OnCmdApply() 
{
	ApplyOrOK();	
}

void CCMMvldItemEuroBSNewDlg::OnCmdBtnAdd() 
{
	UpdateData(TRUE);
	
	CCMMvldSubItemDlg dlg(this);
	dlg.m_nLoadModel = m_Data.nLoadModel;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_nPreLoadModel = m_Data.nLoadModel;
}

void CCMMvldItemEuroBSNewDlg::OnCmdBtnModify() 
{
	UpdateData(TRUE);

	int iItem = -1;
	iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);

	if(iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
 
	CCMMvldSubItemDlg dlg(this);
	dlg.m_nLoadModel = m_Data.nLoadModel;	
	dlg.SetParamData(m_Data.arSubLoadData[iItem], iItem);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_nPreLoadModel = m_Data.nLoadModel;
}

void CCMMvldItemEuroBSNewDlg::OnCmdBtnDelete() 
{
	int iItem = -1;
	iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);

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
	{
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CCMMvldItemEuroBSNewDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdBtnModify();	
	*pResult = 0;
}

void CCMMvldItemEuroBSNewDlg::OnCmdLoadModelRdo()
{
	UpdateData(TRUE);
	MaintainVehicleIndex();
	Data2Dlg_LaneListByVehicleIndex();

	InitCombo();
	AlignControls();
	ShowHideControls();
	ControlsEnableDisable();
	ResizeWindow();
	ChangeCtrlText();

	if(m_nPreLoadModel != m_Data.nLoadModel) 
	{
		m_Data.arSubLoadData.RemoveAll();
		m_List.DeleteAllItems();
	}	
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubBtnAdd() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndUnselList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
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

void CCMMvldItemEuroBSNewDlg::OnCmdSubBtnAdd2() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = TRUE;
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);
	}
	
	CString csLaneName;
	m_wndAreaList.ResetContent();
	m_lstRemain.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aAreaFlag[i]) 
		{
			nIndex = m_wndAreaList.AddString(csLaneName);
			m_wndAreaList.SetItemData(nIndex, i);

			nIndex = m_lstRemain.AddString(csLaneName);
			m_lstRemain.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubBtnDelete() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;

	// Error Check
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);

		int StraddK = m_aLaneList[nIndex];

		for(int j=0; j<m_aSelLlan.GetSize(); j++)
		{
			if(StraddK == m_aSelLlan[j].key.key1) { AfxMessageBox(_LS(IDS_CMD_MVLD_EURO_STRADD_LANE_ERROR)); return; }
			if(StraddK == m_aSelLlan[j].key.key2) { AfxMessageBox(_LS(IDS_CMD_MVLD_EURO_STRADD_LANE_ERROR)); return; }
		}
	}

	for (int i = nCount-1; i >= 0; i--)
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
		if (!m_aSelFlag[i] && !m_aAreaFlag[i] && !m_aFootFlag[i])
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubBtnDelete2() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndAreaList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndAreaList.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = FALSE;
		m_aSelFlag[nIndex] = TRUE;
		m_wndAreaList.DeleteString(aIndex[i]);

		nIndex = m_lstRemain.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = FALSE;
		m_aSelFlag[nIndex] = TRUE;
		m_lstRemain.DeleteString(aIndex[i]);
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

void CCMMvldItemEuroBSNewDlg::OnCmdSubStradAddBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount != 2)
	{
		AfxMessageBox(_LS(IDS_WG_CMD_ERR_Selected_Lane_Number2));
		return;
	}

	T_LLAN_EURO_NEW_KEY key;
	nIndex = m_wndSelList.GetItemData(aIndex[0]);
	key.key.key1 = m_aLaneList[nIndex];
	nIndex = m_wndSelList.GetItemData(aIndex[1]);
	key.key.key2 = m_aLaneList[nIndex];

	if (IsDuplicatedItem(key)) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD_ERR_EXIST_SAME_ITEM));
		return;
	}
	if (!m_bSurface)
	{
		T_LLAN_D LlanD1, LlanD2;
		m_pDoc->m_pAttrCtrl->GetLlan(key.key.key1, LlanD1);
		m_pDoc->m_pAttrCtrl->GetLlan(key.key.key2, LlanD2);

		//if (CTestEnvMgr::IsTestEnvST(_T("Test_STRADD"), _T("yes")))
		{
			BOOL bChk = TRUE;
			if(LlanD1.nLoadDistribution == 1)
			{
				if(LlanD2.nLoadDistribution != 1)        bChk = FALSE;
			}
			if(LlanD1.nLoadDistribution == 2)
			{
				if(LlanD2.nLoadDistribution != 2)        bChk = FALSE;
				if(LlanD1.ElemGroup != LlanD2.ElemGroup) bChk = FALSE;
			}
			if(!bChk)
			{
				//[오류] Heavy Load Lanes 또는 WA Rating Vehicle Group2 에 사용된 두 차선의 Vehicular Load Distribution 정보는 서로 같아야 합니다. ( Lane Name = %s, %s )
				GSaveHistoryFormatNF(_LS(IDS_DB_MOVLANE__ERROR_AS_STRADDLING_LOADDISTRNEW), LlanD1.LineLaneName, LlanD2.LineLaneName);
				return;
			}
		}
// 		else
// 		{
// 			if (LlanD1.nLoadDistribution != 2 ||
// 				LlanD2.nLoadDistribution != 2 ) // Cross Beam 아닌 경우
// 			{
// 				AfxMessageBox(_LS(IDS_WG_CMD_MVLD_BS_LANE_TYPE_ERROR));
// 				return;
// 			}
// 		}
	}

	CString csLaneName;
	csLaneName = GetLaneName(key.key.key1);
	csLaneName += _T(" : ");
	csLaneName += GetLaneName(key.key.key2);
	nIndex = m_lstStrad.AddString(csLaneName);
	m_lstStrad.SetItemData(nIndex, key.keymap);
	m_aSelLlan.Add(key);
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubStradDelBtn() 
{
	UINT idata;
	int nMax = m_aSelLlan.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstStrad.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		idata = m_lstStrad.GetItemData(aIndex[i]);
		if (m_lstStrad.DeleteString(aIndex[i]) == LB_ERR) continue;
		int k = 0;
		for (k=0; k<nMax; k++)
			if (idata == m_aSelLlan[k].keymap) break;
		if (k == nMax) { ASSERT(0); continue; }
		m_aSelLlan.RemoveAt(k);
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubReaminAddBtn() 
{
	OnCmdSubBtnAdd2();
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubReaminDelBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstRemain.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndAreaList.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = FALSE;
		m_aSelFlag[nIndex] = TRUE;
		m_wndAreaList.DeleteString(aIndex[i]);

		nIndex = m_lstRemain.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = FALSE;
		m_aSelFlag[nIndex] = TRUE;
		m_lstRemain.DeleteString(aIndex[i]);
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

void CCMMvldItemEuroBSNewDlg::OnCmdSubFootwayAddBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aFootFlag[nIndex] = TRUE;
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);
	}
	
	CString csLaneName;
	m_lstFootway.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aFootFlag[i]) 
		{
			nIndex = m_lstFootway.AddString(csLaneName);
			m_lstFootway.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubFootwayDelBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstFootway.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstFootway.GetItemData(aIndex[i]);
		m_aFootFlag[nIndex] = FALSE;
		m_aSelFlag[nIndex] = TRUE;
		m_lstFootway.DeleteString(aIndex[i]);
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

void CCMMvldItemEuroBSNewDlg::OnCmdSubOptimRemainAddBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstOptimUnsel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstOptimUnsel.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = TRUE;
		m_lstOptimUnsel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstOptimRemain.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aAreaFlag[i]) 
		{
			nIndex = m_lstOptimRemain.AddString(csLaneName);
			m_lstOptimRemain.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubOptimRemainDelBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstOptimRemain.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstOptimRemain.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = FALSE;
		m_lstOptimRemain.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstOptimUnsel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aAreaFlag[i])
		{
			nIndex = m_lstOptimUnsel.AddString(csLaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubOptimRemain2AddBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstOptimUnsel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstOptimUnsel.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = TRUE;
		m_lstOptimUnsel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstOptimRemain2.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aAreaFlag[i]) 
		{
			nIndex = m_lstOptimRemain2.AddString(csLaneName);
			m_lstOptimRemain2.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubOptimRemain2DelBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstOptimRemain2.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstOptimRemain2.GetItemData(aIndex[i]);
		m_aAreaFlag[nIndex] = FALSE;
		m_lstOptimRemain2.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstOptimUnsel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aFootFlag[i] && !m_aAreaFlag[i])
		{
			nIndex = m_lstOptimUnsel.AddString(csLaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubOptimFootwayAddBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstOptimUnsel.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstOptimUnsel.GetItemData(aIndex[i]);
		m_aFootFlag[nIndex] = TRUE;
		m_lstOptimUnsel.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstOptimFootway.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aFootFlag[i]) 
		{
			nIndex = m_lstOptimFootway.AddString(csLaneName);
			m_lstOptimFootway.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdSubOptimFootwayDelBtn() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstOptimFootway.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstOptimFootway.GetItemData(aIndex[i]);
		m_aFootFlag[nIndex] = FALSE;
		m_lstOptimFootway.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_lstOptimUnsel.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aFootFlag[i] && !m_aAreaFlag[i])
		{
			nIndex = m_lstOptimUnsel.AddString(csLaneName);
			m_lstOptimUnsel.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSNewDlg::OnDblclkCmdSubSelList() 
{
	OnCmdSubBtnAdd2();
}

void CCMMvldItemEuroBSNewDlg::OnDblclkCmdSubUnselList() 
{
	OnCmdSubBtnAdd();
}

void CCMMvldItemEuroBSNewDlg::OnDblclkCmdSubAreaList() 
{
	OnCmdSubBtnDelete2();
}

void CCMMvldItemEuroBSNewDlg::OnDblclkCmdSubStradList() 
{
	OnCmdSubStradDelBtn();
}

void CCMMvldItemEuroBSNewDlg::OnDblclkCmdSubRemainList() 
{
	OnCmdSubReaminDelBtn();
}

void CCMMvldItemEuroBSNewDlg::OnDblclkCmdSubFootwayList() 
{
	OnCmdSubFootwayDelBtn();
}

void CCMMvldItemEuroBSNewDlg::OnDblclkCmdSubOptimRemainList() 
{
	OnCmdSubOptimRemainDelBtn();
}


void CCMMvldItemEuroBSNewDlg::OnDblclkCmdSubOptimRemain2List() 
{
	OnCmdSubOptimRemain2DelBtn();
}

void CCMMvldItemEuroBSNewDlg::OnDblclkCmdSubOptimFootwayList() 
{
	OnCmdSubOptimFootwayDelBtn();
}

void CCMMvldItemEuroBSNewDlg::OnCmdRailLoadDataBtn() 
{
	UpdateData(TRUE);

	CCMMvldItemEuroBSRailDlg dlg(this);

	dlg.m_dPsi1Fact1 = m_Data.dScaleFactor[0];
	dlg.m_dPsi1Fact2 = m_Data.dScaleFactor[1];
	dlg.m_dPsi1Fact3 = m_Data.dScaleFactor[2];
	dlg.m_bPsi1Fact = m_Data.bPsi1Factor;
	dlg.m_dMultiFact1 = m_Data.dMultipleFactor1;
	dlg.m_dMultiFact2 = m_Data.dMultipleFactor2;
	dlg.m_dMultiFact3 = m_Data.dMultipleFactor3;
	
	dlg.SetInitPos(D_INIT_POS_RT);
	if(dlg.DoModal()==IDOK)
	{
		m_Data.dScaleFactor[0] = dlg.m_dPsi1Fact1;
		m_Data.dScaleFactor[1] = dlg.m_dPsi1Fact2;
		m_Data.dScaleFactor[2] = dlg.m_dPsi1Fact3;
		m_Data.bPsi1Factor = dlg.m_bPsi1Fact;
		m_Data.dMultipleFactor1 = dlg.m_dMultiFact1;
		m_Data.dMultipleFactor2 = dlg.m_dMultiFact2;
		m_Data.dMultipleFactor3 = dlg.m_dMultiFact3;
	}
}

void CCMMvldItemEuroBSNewDlg::OnCmdOptmChk() 
{
	UpdateData(TRUE);
	InitCombo();

	MaintainVehicleIndex();
	Data2Dlg_LaneListByVehicleIndex();

	AlignControls();
	ShowHideControls();
	ControlsEnableDisable();
	ResizeWindow();
}

void CCMMvldItemEuroBSNewDlg::SetHeaderTitle_AssignVehl()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact)};
	int nColWidth[COLCOUNT_ASSIGNVEHL] = {140, 140};

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

void CCMMvldItemEuroBSNewDlg::MakeItemEx_AssignVehl()
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

BOOL CCMMvldItemEuroBSNewDlg::InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
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

BOOL CCMMvldItemEuroBSNewDlg::DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_AssignVehlList.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemEuroBSNewDlg::ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
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

CString CCMMvldItemEuroBSNewDlg::DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data)
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

void CCMMvldItemEuroBSNewDlg::OnCmdBtnAdd_AssignVehl() 
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

void CCMMvldItemEuroBSNewDlg::OnCmdBtnModify_AssignVehl() 
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

void CCMMvldItemEuroBSNewDlg::OnCmdBtnDelete_AssignVehl() 
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

void CCMMvldItemEuroBSNewDlg::AdjustComboListBox(MComboBox& Cbx)
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = Cbx.GetDC();
	for (int i=0;i < Cbx.GetCount();i++)
	{
		Cbx.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	Cbx.ReleaseDC(pDC);

	if (Cbx.GetDroppedWidth() < dx)
	{
		Cbx.SetDroppedWidth(dx);
		ASSERT(Cbx.GetDroppedWidth() == dx);
	}
}
