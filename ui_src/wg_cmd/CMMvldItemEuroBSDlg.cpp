// CMMvldItemEuroBSDlg.cpp : implementation file
//

#include "stdafx.h"
/* 이 대화상자 안씀
#include "wg_cmd.h"
#include "CMMvldItemEuroBSDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\UnitCtrl.h"

#include "CMMvldSubItemDlg.h"
#include "..\wg_base\SpecialChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemEuroBSDlg dialog

CString CCMMvldItemEuroBSDlg::m_aTypeList[D_TYPE_COUNT][D_MAX_COUNT] = {
		{_T("Load Model 1"), _T("Load Model 2"), _T("Load Model 3 (600/150)"),_T("Load Model 3 (900/150)"),_T("Load Model 3 (1200/150/200)"),_T("Load Model 3 (1500/150/200)"),_T("Load Model 3 (1800/150/200)"),_T("Load Model 3 (2400/200)"),_T("Load Model 3 (3000/200)"),_T("Load Model 3 (3600/200)"),_T("Load Model 3 (SV 80)"),_T("Load Model 3 (SV 100)"),_T("Load Model 3 (SV 196)"),_T("Load Model 3 (SOV 250)"),_T("Load Model 3 (SOV 350)"),_T("Load Model 3 (SOV 450)"),_T("Load Model 3 (SOV 600)"),_T("Load Model 4")},
		{_T("Uniform load (Road bridge footway)"), _T("Uniform load (Footbridge)"), _T("Concentrated Load"), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T("Fatigue Load Model 1"), _T("Fatigue Load Model 2 (280)"), _T("Fatigue Load Model 2 (360)"), _T("Fatigue Load Model 2 (630)"),_T("Fatigue Load Model 2 (560)"),_T("Fatigue Load Model 2 (610)"),_T("Fatigue Load Model 3 (One Vehicle)"),_T("Fatigue Load Model 3 (Two Vehicle)"),_T("Fatigue Load Model 4 (200)"),_T("Fatigue Load Model 4 (310)"),_T("Fatigue Load Model 4 (490)"),_T("Fatigue Load Model 4 (390)"),_T("Fatigue Load Model 4 (450)"), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T("Load Model 71"), _T("Load Model SW/0"), _T("Load Model SW/2"), _T("Unloaded Train"), _T("HSLM A1 ~ HSLM A10"), _T("HSLM B"), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")}
	};

CCMMvldItemEuroBSDlg::CCMMvldItemEuroBSDlg(CWnd* pParent )
	: CDialogMove(CCMMvldItemEuroBSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemEuroBSDlg)
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

	m_aLM3Lanes.RemoveAll();
	m_aLM3Lanes.Add(IDC_CMD_STRAD_LANE_STC);
	m_aLM3Lanes.Add(IDC_CMD_STRAD_LANE_LST);
	m_aLM3Lanes.Add(IDC_CMD_SUB_STRAD_ADD_BTN);
	m_aLM3Lanes.Add(IDC_CMD_SUB_STRAD_DEL_BTN);
	m_aLM3Lanes.Add(IDC_CMD_REMAIN_AREA_STC);
	m_aLM3Lanes.Add(IDC_CMD_REMAIN_AREA_LST);
	m_aLM3Lanes.Add(IDC_CMD_SUB_REMAIN_ADD_BTN);
	m_aLM3Lanes.Add(IDC_CMD_SUB_REMAIN_DEL_BTN);

	m_aTrainLoad.RemoveAll();
	m_aTrainLoad.Add(IDC_CMD_RAIL_LOAD_GRP);
	m_aTrainLoad.Add(IDC_CMD_RAIL_LOADED_LANES_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_LOADED_LANES_1_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_LOADED_LANES_2_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_LOADED_LANES_3_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_PSI1_FACTOR_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_PSI1_FACTOR_1_EDT);
	m_aTrainLoad.Add(IDC_CMD_RAIL_PSI1_FACTOR_2_EDT);
	m_aTrainLoad.Add(IDC_CMD_RAIL_PSI1_FACTOR_3_EDT);
	m_aTrainLoad.Add(IDC_CMD_RAIL_PSI1_FACTOR_CHK);
	m_aTrainLoad.Add(IDC_CMD_RAIL_PSI_COMMENT_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_MULTI_GRP);
	m_aTrainLoad.Add(IDC_CMD_RAIL_MULTI_LOADED_LANES_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_MULTI_SCALE_FACTOR_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_MULTI_LOADED_LANES_1_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_MULTI_LOADED_LANES_2_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_MULTI_LOADED_LANES_3_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_MULTI_SCALE_FACTOR_1_EDT);
	m_aTrainLoad.Add(IDC_CMD_RAIL_MULTI_SCALE_FACTOR_2_EDT);
	m_aTrainLoad.Add(IDC_CMD_RAIL_MULTI_SCALE_FACTOR_3_EDT);
	m_aTrainLoad.Add(IDC_CMD_RAIL_SUB_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_SUB_LOADING_STC);
	m_aTrainLoad.Add(IDC_CMD_RAIL_SUB_LOADING_COMBINED_RDO);
	m_aTrainLoad.Add(IDC_CMD_RAIL_SUB_LOADING_INDEPENDENT_RDO);
	m_aTrainLoad.Add(IDC_CMD_RAIL_SUB_LST);
	m_aTrainLoad.Add(IDC_CMD_RAIL_SUB_ADD_BTN);
	m_aTrainLoad.Add(IDC_CMD_RAIL_SUB_MOD_BTN);
	m_aTrainLoad.Add(IDC_CMD_RAIL_SUB_DEL_BTN);


	m_aEtc.RemoveAll();
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	m_bModify = FALSE;
	m_bSurface = FALSE;

	m_aLaneList.RemoveAll();
	m_aSelFlag.RemoveAll();
	m_aAreaFlag.RemoveAll();

	m_nLMSelected1 = 0;
	m_nLMSelected3 = 0;
	m_nLM3SpecialSel = 0;
	m_nPreLoadModel = 0;

	m_bPsi1Fact = FALSE;
	m_nTrainLoading = 1;
}

void CCMMvldItemEuroBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemEuroBSDlg)
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
	DDX_Control(pDX, IDC_CMD_RAIL_PSI1_FACTOR_1_EDT,        m_edtPsi1Fact1);
	DDX_Control(pDX, IDC_CMD_RAIL_PSI1_FACTOR_2_EDT,        m_edtPsi1Fact2);
	DDX_Control(pDX, IDC_CMD_RAIL_PSI1_FACTOR_3_EDT,        m_edtPsi1Fact3);
	DDX_Check  (pDX, IDC_CMD_RAIL_PSI1_FACTOR_CHK,          m_bPsi1Fact);
	DDX_Control(pDX, IDC_CMD_RAIL_MULTI_SCALE_FACTOR_1_EDT, m_edtMultiFact1);
	DDX_Control(pDX, IDC_CMD_RAIL_MULTI_SCALE_FACTOR_2_EDT, m_edtMultiFact2);
	DDX_Control(pDX, IDC_CMD_RAIL_MULTI_SCALE_FACTOR_3_EDT, m_edtMultiFact3);
	DDX_Radio  (pDX, IDC_CMD_RAIL_SUB_LOADING_COMBINED_RDO, m_nTrainLoading);
	DDX_Control(pDX, IDC_CMD_RAIL_SUB_LST,                  m_TrainList);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg implementation functions
BOOL CCMMvldItemEuroBSDlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
{
	if(m_nPreLoadModel != m_Data.nLoadModel) 
	{
		m_Data.arSubLoadData.RemoveAll();

		if(m_Data.nLoadModel == 4) m_List.DeleteAllItems();
		else                       m_TrainList.DeleteAllItems(); 
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

void CCMMvldItemEuroBSDlg::InitCombo()
{
	m_cmbLM1.ResetContent();
	m_cmbLM3.ResetContent();
	m_cmbLM3Special.ResetContent();

	if(m_Data.nLoadModel == 2) // 3rd Option
	{
		CDlgUtil::CobxAddItem(m_cmbLM3, _T("None"), 0);
	}

	if(m_Data.nLoadModel == 3)
	{
		CDlgUtil::CobxAddItem(m_cmbLM3Special, _T("None"), 0);
	}
		
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVHL_D MvhlD;
	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		int nType,nSubType;
		m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD);
		if(MvhlD.nLoadType!=4) continue;
		if(MvhlD.bStandard)   
		{
			nType = GetIndex(MvhlD.VehicleTypeName,&nSubType);
			if(m_Data.nLoadModel == 0) // LM1, FLM1
			{
				// RoadBridge - LoadModel1, RoadBridge Fatigue - LoadModel1
				if ( (MvhlD.nStandardCode == 19 && nSubType ==0) || (MvhlD.nStandardCode == 21 && nSubType ==0) )
				{
					CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
				}
			}
			else if(m_Data.nLoadModel==2) // LM1, LM3 Multi
			{
				if (MvhlD.nStandardCode == 19 && nSubType ==0)
				{
					CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
				}
				if(MvhlD.nStandardCode==19 && nSubType >=2 && nSubType<=9) // LM3
				{
					CDlgUtil::CobxAddItem(m_cmbLM3, MvhlD.VehicleLoadName, aMvhlK[i]);
				}
			}
			else if(m_Data.nLoadModel==3) // LM1 & Special Multi
			{
				if (MvhlD.nStandardCode == 19 && nSubType ==0)
				{
					CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
				}

				if(MvhlD.nStandardCode==19 && nSubType >=10 && nSubType<=16) // LM3
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
				// LM3 는 Special Vehicle 이므로 User Defined 는 들어오지 않는다.
				if(MvhlD.nSubType == 0)
				{
					CDlgUtil::CobxAddItem(m_cmbLM1, MvhlD.VehicleLoadName, aMvhlK[i]);
				}
			}
		}
	}

	if (m_cmbLM1.GetCount() > 0) m_cmbLM1.SetCurSel(0);
	if (m_cmbLM3.GetCount() > 0) m_cmbLM3.SetCurSel(0);
	if (m_cmbLM3Special.GetCount() > 0) m_cmbLM3Special.SetCurSel(0);
}

void CCMMvldItemEuroBSDlg::AlignControls()
{
	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;
	GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_SUB_GRUP     )->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aSubLoadCase, nDistX, nDistY);

	GetDlgItem(IDC_CMD_LM3_TXT        )->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_LM3_SPECIAL_STC)->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aLM3Special, nDistX, nDistY);

	GetDlgItem(IDC_CMD_AREA_LIST     )->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_STRAD_LANE_LST)->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aLM3Lanes, nDistX, nDistY);

	GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_RAIL_LOAD_GRP)->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;

	GetDlgItem(IDC_CMD_LEADING_CHK  )->GetWindowRect(rTo);
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aTrainLoad, nDistX, nDistY);
}

void CCMMvldItemEuroBSDlg::ShowHideCtrl()
{
	BOOL bShow = (m_Data.nLoadModel!=1 && m_Data.nLoadModel!=4);
	GetDlgItem(IDC_CMD_LOAD_CASE_TXT)->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_LM1_TXT)      ->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_LM1_CMB)      ->ShowWindow(bShow);
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

	CDlgUtil::CtrlShowHide(this, m_aSubLoadCase, m_Data.nLoadModel==1);
	CDlgUtil::CtrlShowHide(this, m_aTrainLoad,   m_Data.nLoadModel==4);
	CDlgUtil::CtrlShowHide(this, m_aLM3Special,  m_Data.nLoadModel==3);
	CDlgUtil::CtrlShowHide(this, m_aLM3Lanes,    m_Data.nLoadModel==3);

	GetDlgItem(IDC_CMD_AREA_TXT)    ->ShowWindow(m_Data.nLoadModel==0 || m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_AREA_LIST)   ->ShowWindow(m_Data.nLoadModel==0 || m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_SUB_BTN_ADD2)->ShowWindow(m_Data.nLoadModel==0 || m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_SUB_BTN_DEL2)->ShowWindow(m_Data.nLoadModel==0 || m_Data.nLoadModel==2);
	GetDlgItem(IDC_CMD_LEADING_CHK) ->ShowWindow(m_Data.nLoadModel!=4);
}

void CCMMvldItemEuroBSDlg::ResizeWindow()
{
	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;
	
	int nLoadModel = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelType, nLoadModel);  

	if(nLoadModel == 4) GetDlgItem(IDC_CMD_RAIL_LOAD_GRP)->GetWindowRect(rTo);
	else                GetDlgItem(IDC_CMD_LANES_GRUP   )->GetWindowRect(rTo);

	GetDlgItem(IDOK)->GetWindowRect(rMove);
	nDistY = rTo.bottom-rMove.bottom+30;
	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

	GetWindowRect(rMove);
	rMove.right = rTo.right + globalUtils.ScaleByDPI(12);
	rMove.bottom = rTo.bottom + globalUtils.ScaleByDPI(50);

	MoveWindow(rMove);
}

void CCMMvldItemEuroBSDlg::ChangeCtrlText()
{
	CString strText;
#if defined(_US) || defined(_RUS)
	strText.Format(_T("Ignore %s Factor:"), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_LEADING_CHK)->SetWindowText(strText);
#endif

	CString strMsg = _LS(IDS_CMD_VEHICLE_);
	if(m_Data.nLoadModel==2 || m_Data.nLoadModel==3) strMsg = _LS(IDS_CMD_LM1_);
	GetDlgItem(IDC_CMD_LM1_TXT)->SetWindowText(strMsg);

	strText.Format(_LS(IDS_CMD_MVLD_EURO_PSI1_FACTOR), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_STC)->SetWindowText(strText);

	strText.Format(_LS(IDS_CMD_MVLD_EURO_IGNORE_PSI1_FACTOR), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_CHK)->SetWindowText(strText);

	strText.Format(_LS(IDS_CMD_MVLD_EURO_PSI_COMMENT), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_RAIL_PSI_COMMENT_STC)->SetWindowText(strText);
}

void CCMMvldItemEuroBSDlg::MaintainVehicleIndex()
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

void CCMMvldItemEuroBSDlg::Data2Dlg()
{
	m_wndLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.SetWindowText(m_Data.Description);

	CDlgUtil::CtrlRadioSetCheck(this, m_aLoadModelType, m_Data.nLoadModel);

	if(m_Data.VehicleK1 > 0 && m_cmbLM1.GetCount() > 0)
		CDlgUtil::CobxSetCurSelItemData(m_cmbLM1, (DWORD&)m_Data.VehicleK1);

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

	int nAllSize = m_aLaneList.GetSize();
	int nSelSize;
	int nMaxCount = m_Data.SubLoadCase.aSelectedLanes.GetSize();
	for (nSelSize = 0; nSelSize < nMaxCount; nSelSize++)
		if (m_Data.SubLoadCase.aSelectedLanes[nSelSize] == 0) break;

	int nAreaSize;
	int nMaxCount2 = m_Data.aSelectedRemainingArea.GetSize();
	for (nAreaSize = 0; nAreaSize < nMaxCount2; nAreaSize++)
		if (m_Data.aSelectedRemainingArea[nAreaSize] == 0) break;

	m_aSelFlag.SetSize(nAllSize);
	m_aAreaFlag.SetSize(nAllSize);
	for (int i=0; i<nAllSize; i++)
	{
		BOOL bFoundInSel = FALSE;
		BOOL bFoundInArea = FALSE;
		for (int j=0; j<nSelSize; j++)
		{
			if (m_aLaneList[i] == m_Data.SubLoadCase.aSelectedLanes[j]) 
			{
				bFoundInSel = TRUE; break;
			}
		}
		for (int k=0 ; k<nAreaSize ; ++k) 
		{
			if (m_aLaneList[i] == m_Data.aSelectedRemainingArea[k]) 
			{
				bFoundInArea = TRUE; break;
			}
		}
		m_aSelFlag[i] = bFoundInSel;
		m_aAreaFlag[i] = bFoundInArea;
	}

	int nIndex;

	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();
	m_wndAreaList.ResetContent(); 
	m_lstStrad.ResetContent();
	m_lstRemain.ResetContent();

	// Add String
	CString LaneName;
	for (i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(LaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
		else if (m_aAreaFlag[i]) 
		{
			nIndex = m_wndAreaList.AddString(LaneName);
			m_wndAreaList.SetItemData(nIndex, i);

			nIndex = m_lstRemain.AddString(LaneName);
			m_lstRemain.SetItemData(nIndex, i);
		}
		else
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}

	T_LLAN_EURO_KEY key;
	for (i=0; i<m_Data.aStraddLanes.GetSize(); i++)
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

	m_chkLeading.SetCheck(m_Data.bLeading);
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCombOption, m_Data.nCombOption);
	MakeItemEx();

	double dPsi1Fact1 = 0.8;
	double dPsi1Fact2 = 0.7;
	double dPsi1Fact3 = 0.6;

	if(!m_bModify)
	{
		m_edtPsi1Fact1.SetEditUnit(dPsi1Fact1);
		m_edtPsi1Fact2.SetEditUnit(dPsi1Fact2);
		m_edtPsi1Fact3.SetEditUnit(dPsi1Fact3);
	}
	else
	{
		m_edtPsi1Fact1.SetEditUnit(m_Data.dScaleFactor[0]);
		m_edtPsi1Fact2.SetEditUnit(m_Data.dScaleFactor[1]);
		m_edtPsi1Fact3.SetEditUnit(m_Data.dScaleFactor[2]);
	}

	m_bPsi1Fact = m_Data.bPsi1Factor;

	double dMultiFact1 = 1.0;
	double dMultiFact2 = 1.0;
	double dMultiFact3 = 0.75;

	if(!m_bModify)
	{
		m_edtMultiFact1.SetEditUnit(dMultiFact1);
		m_edtMultiFact2.SetEditUnit(dMultiFact2);
		m_edtMultiFact3.SetEditUnit(dMultiFact3);
	}
	else
	{
		m_edtMultiFact1.SetEditUnit(m_Data.dMultipleFactor1);
		m_edtMultiFact2.SetEditUnit(m_Data.dMultipleFactor2);
		m_edtMultiFact3.SetEditUnit(m_Data.dMultipleFactor3);
	}

	m_nTrainLoading = m_Data.nCombOption;

	UpdateData(FALSE);
}

BOOL CCMMvldItemEuroBSDlg::Dlg2Data()
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
		if(m_Data.nLoadModel == 2)
		{
			if(m_cmbLM3.GetCount() > 0)
				CDlgUtil::CobxGetItemDataByCurSel(m_cmbLM3, (DWORD&)m_Data.VehicleK3);
		}
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
	}
	else if(m_Data.nLoadModel == 1)
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

		T_LLAN_EURO_KEY key;
		// 초기화
		m_Data.aStraddLanes.RemoveAll();
		m_Data.aStraddLanes.SetSize(m_aSelLlan.GetSize());

		for (i=0; i<m_aSelLlan.GetSize(); i++)
		{
			key = m_aSelLlan[i];
			m_Data.aStraddLanes[i].Lane1 = key.key.key1;
			m_Data.aStraddLanes[i].Lane2 = key.key.key2;
		}
	}

	m_Data.bLeading = m_chkLeading.GetCheck();

	if(m_Data.nLoadModel == 4) // Train
	{
		m_Data.dScaleFactor[0] = m_edtPsi1Fact1.GetEditValue();
		m_Data.dScaleFactor[1] = m_edtPsi1Fact2.GetEditValue();
		m_Data.dScaleFactor[2] = m_edtPsi1Fact3.GetEditValue();

		m_Data.bPsi1Factor = m_bPsi1Fact;

		m_Data.dMultipleFactor1 = m_edtMultiFact1.GetEditValue();
		m_Data.dMultipleFactor2 = m_edtMultiFact2.GetEditValue();
		m_Data.dMultipleFactor3 = m_edtMultiFact3.GetEditValue();

		m_Data.nCombOption = m_nTrainLoading;
	}
	
	return TRUE;
}

void CCMMvldItemEuroBSDlg::SetHeaderTitle()
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
	dwStyle = ListView_GetExtendedListViewStyle(m_TrainList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_TrainList.GetSafeHwnd(), dwStyle);
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
		m_TrainList.InsertColumn(i,&lvcolumn);  // Train Sub Load Case
		title.ReleaseBuffer();
	}
}

void CCMMvldItemEuroBSDlg::MakeItemEx()
{
	if(m_Data.nLoadModel == 4) m_TrainList.DeleteAllItems();
	else											 m_List.DeleteAllItems();
	
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

BOOL CCMMvldItemEuroBSDlg::InsertItem(int nIndex, T_MVLD_BASE &Data)
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
			if(m_Data.nLoadModel == 4) nItem = m_TrainList.InsertItem(&lvitem);
			else                       nItem = m_List.InsertItem(&lvitem);
		}
		else 
		{
			if(m_Data.nLoadModel == 4) m_TrainList.SetItem(&lvitem);
			else                       m_List.SetItem(&lvitem);
		}

		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CCMMvldItemEuroBSDlg::DeleteItem(int nIndex, T_MVLD_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0)
	{
		if(m_Data.nLoadModel == 4) m_TrainList.DeleteItem(nIndex);
		else											 m_List.DeleteItem(nIndex);	
	}
	return TRUE;
}

BOOL CCMMvldItemEuroBSDlg::ModifyItem(int nIndex, T_MVLD_BASE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Data);

			if(m_Data.nLoadModel == 4) m_TrainList.SetItemText(nItem, i, str);
			else	                     m_List.SetItemText(nItem, i, str);	
		}
	}
	return TRUE;
}

BOOL CCMMvldItemEuroBSDlg::IsDuplicatedItem(T_LLAN_EURO_KEY& key)
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

CString CCMMvldItemEuroBSDlg::DataToStr(int i, T_MVLD_BASE &Data)
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

CString CCMMvldItemEuroBSDlg::GetLaneName(UINT nLaneKey)
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

int CCMMvldItemEuroBSDlg::GetIndex(CString &csName, int *sitem)
{
	for (int i = 0; i < D_TYPE_COUNT; i++)
	{
		for (int j = 0; j < D_MAX_COUNT && m_aTypeList[i][j] != csName; j++);
		if (j != D_MAX_COUNT) 
		{
			if(sitem) *sitem = j;
			return i;
		}
	}

	ASSERT(0);
	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
	return -1;
}

BOOL CCMMvldItemEuroBSDlg::ApplyOrOK()
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


BEGIN_MESSAGE_MAP(CCMMvldItemEuroBSDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemEuroBSDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_RAIL_SUB_LST, OnDblclkCmdList)
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
	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST, OnDblclkCmdSubSelList)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST, OnDblclkCmdSubUnselList)
	ON_LBN_DBLCLK(IDC_CMD_AREA_LIST, OnDblclkCmdSubAreaList) 
	ON_LBN_DBLCLK(IDC_CMD_STRAD_LANE_LST, OnDblclkCmdSubStradList)
	ON_LBN_DBLCLK(IDC_CMD_REMAIN_AREA_LST, OnDblclkCmdSubRemainList)

	ON_BN_CLICKED(IDC_CMD_RAIL_PSI1_FACTOR_CHK, OnCmdIgnorePsi1FactorChk)

	ON_BN_CLICKED(IDC_CMD_RAIL_SUB_ADD_BTN, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_RAIL_SUB_MOD_BTN, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_RAIL_SUB_DEL_BTN, OnCmdBtnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemEuroBSDlg message handlers

BOOL CCMMvldItemEuroBSDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0)
	{
		m_pDoc->m_pAttrCtrl->GetSlanKeyList(m_aLaneList);
		m_bSurface = TRUE;
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetLlanKeyList(m_aLaneList);
		m_bSurface = FALSE;
	}
	
	if(!m_bModify) m_Data.Initialize();

	SetHeaderTitle();
	InitCombo();
	AlignControls();
	Data2Dlg();
	ShowHideCtrl();
	ResizeWindow();
	ChangeCtrlText();

	OnCmdIgnorePsi1FactorChk();

	m_nPreLoadModel = m_Data.nLoadModel;

	// EuroCode Renewal 에 사용되는 Controls 비활성화 
	GetDlgItem(IDC_CMD_FOOTWAY_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_FOOTWAY_CMB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_FOOTWAY_LANE_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_FOOTWAY_LANE_LST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SUB_FOOTWAY_ADD_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SUB_FOOTWAY_DEL_BTN)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemEuroBSDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CCMMvldItemEuroBSDlg::OnCmdApply() 
{
	ApplyOrOK();	
}

void CCMMvldItemEuroBSDlg::OnCmdBtnAdd() 
{
	UpdateData(TRUE);
	
	CCMMvldSubItemDlg dlg(this);
	
	if(m_Data.nLoadModel==4) dlg.m_bEuroTrain = TRUE;
	else                     dlg.m_bEuroTrain = FALSE;

	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_nPreLoadModel = m_Data.nLoadModel;
}

void CCMMvldItemEuroBSDlg::OnCmdBtnModify() 
{
	UpdateData(TRUE);

	int iItem = -1;
	if(m_Data.nLoadModel == 4)
	{
		iItem = m_TrainList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	}
	else
	{
		iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	}

	if(iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
 
	CCMMvldSubItemDlg dlg(this);
	
	if(m_Data.nLoadModel==4) dlg.m_bEuroTrain = TRUE;
	else                     dlg.m_bEuroTrain = FALSE;

	dlg.SetParamData(m_Data.arSubLoadData[iItem], iItem);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_nPreLoadModel = m_Data.nLoadModel;
}

void CCMMvldItemEuroBSDlg::OnCmdBtnDelete() 
{
	int iItem = -1;
	if(m_Data.nLoadModel == 4)
	{
		iItem = m_TrainList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	}
	else
	{
		iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	}

	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
	m_Data.arSubLoadData.RemoveAt(iItem);

	if(m_Data.nLoadModel == 4) m_TrainList.DeleteItem(iItem);
	else											 m_List.DeleteItem(iItem); 

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
	{
		if(m_Data.nLoadModel == 4) m_TrainList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		else											 m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CCMMvldItemEuroBSDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdBtnModify();	
	*pResult = 0;
}

void CCMMvldItemEuroBSDlg::OnCmdLoadModelRdo()
{
	MaintainVehicleIndex();

	InitCombo();
	ShowHideCtrl();
	ResizeWindow();
	ChangeCtrlText();
}

void CCMMvldItemEuroBSDlg::OnCmdSubBtnAdd() 
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
	for (i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(csLaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSDlg::OnCmdSubBtnAdd2() 
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
	for (i = 0; i < nAllSize; i++)
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

void CCMMvldItemEuroBSDlg::OnCmdSubBtnDelete() 
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

	for (i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndUnselList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSDlg::OnCmdSubBtnDelete2() 
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
	for (i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(csLaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSDlg::OnCmdSubStradAddBtn() 
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

	T_LLAN_EURO_KEY key;
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
		if (LlanD1.nLoadDistribution != 2 ||
				LlanD2.nLoadDistribution != 2 ) // Cross Beam 아닌 경우
		{
			AfxMessageBox(_LS(IDS_WG_CMD_MVLD_BS_LANE_TYPE_ERROR));
			return;
		}
	}

	CString csLaneName;
	csLaneName = GetLaneName(key.key.key1);
	csLaneName += _T(" : ");
	csLaneName += GetLaneName(key.key.key2);
	nIndex = m_lstStrad.AddString(csLaneName);
	m_lstStrad.SetItemData(nIndex, key.keymap);
	m_aSelLlan.Add(key);
}

void CCMMvldItemEuroBSDlg::OnCmdSubStradDelBtn() 
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
		
		for (int k=0; k<nMax; k++)
			if (idata == m_aSelLlan[k].keymap) break;
		if (k == nMax) { ASSERT(0); continue; }
		m_aSelLlan.RemoveAt(k);
	}
}

void CCMMvldItemEuroBSDlg::OnCmdSubReaminAddBtn() 
{
	OnCmdSubBtnAdd2();
}

void CCMMvldItemEuroBSDlg::OnCmdSubReaminDelBtn() 
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
	for (i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(csLaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemEuroBSDlg::OnDblclkCmdSubSelList() 
{
	OnCmdSubBtnAdd2();
}

void CCMMvldItemEuroBSDlg::OnDblclkCmdSubUnselList() 
{
	OnCmdSubBtnAdd();
}

void CCMMvldItemEuroBSDlg::OnDblclkCmdSubAreaList() 
{
	OnCmdSubBtnDelete2();
}

void CCMMvldItemEuroBSDlg::OnDblclkCmdSubStradList() 
{
	OnCmdSubStradDelBtn();
}

void CCMMvldItemEuroBSDlg::OnDblclkCmdSubRemainList() 
{
	OnCmdSubReaminDelBtn();
}

void CCMMvldItemEuroBSDlg::OnCmdIgnorePsi1FactorChk() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_1_EDT)->EnableWindow(!m_bPsi1Fact);
	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_2_EDT)->EnableWindow(!m_bPsi1Fact);
	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_3_EDT)->EnableWindow(!m_bPsi1Fact);
}

*/