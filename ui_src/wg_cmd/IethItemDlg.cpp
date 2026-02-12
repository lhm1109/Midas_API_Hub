// IethItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IethItemDlg.h"
#include "ThisCalcDamping.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIethItemDlg dialog


CIethItemDlg::CIethItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIethItemDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_nFreqPeriodRdoDlg = 0;
	m_dFrequencyDlg = 1.0;
	m_dPeriodDlg = 1.0;  
	//{{AFX_DATA_INIT(CIethItemDlg)
	m_nAnalTypeRdo = 0;	
	m_nAnalMethodRdo = 0;
	m_nTimeHistoryTypeRdo = 0;
	m_nDampingTypeRdo = 0;
	m_nCoefDefineRdo = 0;
	m_nFreqPeriodRdo = 0;
	m_nIntegrationParaRdo = 0;
	//}}AFX_DATA_INIT
}


void CIethItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIethItemDlg)
	DDX_Control(pDX, IDC_CMD_INITIAL_CONDITION_CHK, m_wndInitCondition);
	DDX_Control(pDX, IDC_CMD_FINAL_STEP_LOAD_CHK, m_wndFinalStepLoad);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN, m_wndShowBtn);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_TYPE_MASS_CHK, m_wndMassProp);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_TYPE_STIFF_CHK, m_wndStiffProp);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_DIRECT_RM_EDIT, m_wndDirectRm);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_DIRECT_RK_EDIT, m_wndDirectRk);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_CALC_RM_EDIT, m_wndCalcRm);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_CALC_RK_EDIT, m_wndCalcRk);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_F1_EDIT, m_wndF1);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_F2_EDIT, m_wndF2);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_T1_EDIT, m_wndT1);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_T2_EDIT, m_wndT2);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_X1_EDIT, m_wndX1);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_X2_EDIT, m_wndX2);
	DDX_Control(pDX, IDC_CMD_INTEGRATION_PARA_GAMMA_EDIT, m_wndGamma);
	DDX_Control(pDX, IDC_CMD_INTEGRATION_PARA_BETA_EDIT, m_wndBeta);
	DDX_Control(pDX, IDC_CMD_MIN_SUBSTEP_SIZE_EDIT, m_wndMinSubStepSize);
	DDX_Control(pDX, IDC_CMD_MIN_SUBSTEP_SIZE_UNIT, m_wndMinSubStepSizeUnit);  
	DDX_Control(pDX, IDC_CMD_MAX_ITER_NUM_EDIT, m_wndMaxIterNum);	
	DDX_Control(pDX, IDC_CMD_EDIT_CONV_TOL, m_wndConvTol);  
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadcaseName);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDesc);
	DDX_Control(pDX, IDC_CMD_DAMPING_LIST, m_DampingList);
	DDX_Control(pDX, IDC_CMD_DAMPING_EDIT, m_wndDamping);
	DDX_Control(pDX, IDC_CMD_MODE_EDIT, m_wndMode);
	DDX_Control(pDX, IDC_CMD_DAMPING_ALL_MODES, m_wndDampAllMode);
	DDX_Control(pDX, IDC_CMD_OUTPUT_STEP_EDIT, m_wndOutputStep);
	DDX_Control(pDX, IDC_CMD_TIME_INC_UNIT, m_wndTimeIncUnit);
	DDX_Control(pDX, IDC_CMD_TIME_INC_EDIT, m_wndTimeInc);
	DDX_Control(pDX, IDC_CMD_END_TIME_UNIT, m_wndEndTimeUnit);
	DDX_Control(pDX, IDC_CMD_END_TIME_EDIT, m_wndEndTime);  
	DDX_Control(pDX, IDC_CMD_FINAL_STATE_COMBO, m_wndFinalStateCmb);
	DDX_Control(pDX, IDC_CMD_APPLY, m_wndBtnApply);  
	DDX_Radio(pDX, IDC_CMD_ANAL_TYPE_RDO,      m_nAnalTypeRdo);
	DDX_Radio(pDX, IDC_CMD_ANAL_METHOD_RDO,       m_nAnalMethodRdo);
	DDX_Radio(pDX, IDC_CMD_TIME_HISTORY_TYPE_RDO, m_nTimeHistoryTypeRdo);
	DDX_Radio(pDX, IDC_CMD_MODAL_DAMPING_TYPE_RDO, m_nDampingTypeRdo);
	DDX_Radio(pDX, IDC_CMD_DAMPING_MASS_DEFINE_RDO, m_nCoefDefineRdo);
	DDX_Radio(pDX, IDC_CMD_DAMPING_MASS_FREQ_PERIOD_RDO, m_nFreqPeriodRdo); 
	DDX_Radio(pDX, IDC_CMD_INTEGRATION_PARA_RDO, m_nIntegrationParaRdo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIethItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIethItemDlg)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_F1_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_F2_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_T1_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_T2_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_X1_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_X2_EDIT, OnCallAutoCalcCoef)	
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_END_TIME_SPIN, OnDeltaposDouble)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_OUTPUT_STEP_SPIN, OnDeltaposInteger)	
	ON_BN_CLICKED(IDC_CMD_DAMPING_ADD, OnDampingAdd)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MODIFY, OnDampingModify)
	ON_BN_CLICKED(IDC_CMD_DAMPING_DELETE, OnDampingDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_DAMPING_LIST, OnChangedCurIethBase)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_MAX_ITER_NUM_SPIN, OnDeltaposCmdMaxIterNumSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TIME_INC_SPIN, OnDeltaposDouble)	
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_MODE_SPIN, OnDeltaposInteger)  
	ON_BN_CLICKED(IDC_CMD_INITIAL_CONDITION_CHK, OnInitConditionChk)
	ON_BN_CLICKED(IDC_CMD_ANAL_TYPE_RDO, OnChangeAnalType)
	ON_BN_CLICKED(IDC_CMD_ANAL_TYPE_RDO2, OnChangeAnalType)
	ON_BN_CLICKED(IDC_CMD_ANAL_METHOD_RDO, OnChangeAnalMethod)
	ON_BN_CLICKED(IDC_CMD_ANAL_METHOD_RDO2, OnChangeAnalMethod)
	ON_BN_CLICKED(IDC_CMD_TIME_HISTORY_TYPE_RDO, OnChangeTimeHistoryType)
	ON_BN_CLICKED(IDC_CMD_TIME_HISTORY_TYPE_RDO2, OnChangeTimeHistoryType)
	ON_BN_CLICKED(IDC_CMD_MODAL_DAMPING_TYPE_RDO, OnChangeDampingType)
	ON_BN_CLICKED(IDC_CMD_MODAL_DAMPING_TYPE_RDO2, OnChangeDampingType)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_DEFINE_RDO, OnChangeCoefDefine)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_DEFINE_RDO2, OnChangeCoefDefine)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_FREQ_PERIOD_RDO, OnChangeFreqPeriod)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_FREQ_PERIOD_RDO2, OnChangeFreqPeriod)
	ON_BN_CLICKED(IDC_CMD_INTEGRATION_PARA_RDO, OnChangeIntegrationPara)
	ON_BN_CLICKED(IDC_CMD_INTEGRATION_PARA_RDO2, OnChangeIntegrationPara)
	ON_BN_CLICKED(IDC_CMD_INTEGRATION_PARA_RDO3, OnChangeIntegrationPara)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN, OnShowDampingBtn)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_TYPE_MASS_CHK, OnMassStiffChk)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_TYPE_STIFF_CHK, OnMassStiffChk)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// Interfaces
//////////////////////////////////////////////////////////////////////////////
void CIethItemDlg::SetParamData(T_IETH_D &data) 
{ 
	m_Data = data; 
	m_csOldName = m_Data.LoadCaseName;
	m_nOldID = m_Data.LoadCaseId;
	m_bModify = TRUE; 
}

//////////////////////////////////////////////////////////////////////////////
// Implementations
//////////////////////////////////////////////////////////////////////////////
BOOL CIethItemDlg::Dlg2Data(T_IETH_D &data)  
{
	// 일단 데이터를 받고 에러 체크는 DB에서 한다.
	UpdateData(TRUE);
	data.Initialize();

	m_wndLoadcaseName.GetWindowText(data.LoadCaseName);
	data.dEndTime = m_wndEndTime.GetEditValue();
	data.dDelta = m_wndTimeInc.GetEditValue();
	data.nOut = m_wndOutputStep.GetEditValue();
	m_wndDesc.GetWindowText(data.Description);
	
	BOOL bInitCondition = m_wndInitCondition.GetCheck();
	BOOL bFinalStepLoad = m_wndFinalStepLoad.GetCheck();
	int nInitCondition = 0;
	if(!bInitCondition) nInitCondition = 1;
	else if(bInitCondition && !bFinalStepLoad) nInitCondition = 2;
	else if(bInitCondition && bFinalStepLoad)  nInitCondition = 3;
	data.nInitCondition = nInitCondition;

	if(nInitCondition == 2 || nInitCondition == 3)
	{
		CString csFinalState;
		m_wndFinalStateCmb.GetWindowText(csFinalState);
		data.KeyFinalState = m_pDoc->m_pAttrCtrl->GetIethKey(csFinalState);
	}
	else data.KeyFinalState = 0;

	data.nAnalType = m_nAnalTypeRdo + 1;
	data.nAnalMethod = m_nAnalMethodRdo + 1;
	data.nTimeHistoryType = m_nTimeHistoryTypeRdo + 1;
	data.nDampingType = m_nDampingTypeRdo + 1;
	data.dDampAll = m_wndDampAllMode.GetEditValue();
	data.aDampData.Copy(m_Data.aDampData);  
	data.PropDamping.bMassProp = m_wndMassProp.GetCheck();
	data.PropDamping.bStiffProp = m_wndStiffProp.GetCheck();
	data.PropDamping.nCoefDefine = m_nCoefDefineRdo + 1;
	if(m_nCoefDefineRdo == 0) // Direct Specification
	{
		data.PropDamping.dRm = m_wndDirectRm.GetEditValue();
		data.PropDamping.dRk = m_wndDirectRk.GetEditValue();
	}
	else if(m_nCoefDefineRdo == 1) // Calculate from Modal Damping
	{
		data.PropDamping.dRm = m_wndCalcRm.GetEditValue();
		data.PropDamping.dRk = m_wndCalcRk.GetEditValue();
	}
	data.PropDamping.nFreqPeriod = m_nFreqPeriodRdo + 1;
	data.PropDamping.dFreqOne = m_wndF1.GetEditValue();
	data.PropDamping.dFreqTwo = m_wndF2.GetEditValue();
	data.PropDamping.dPeriodOne = m_wndT1.GetEditValue();
	data.PropDamping.dPeriodTwo = m_wndT2.GetEditValue();
	data.PropDamping.dDampingOne = m_wndX1.GetEditValue();
	data.PropDamping.dDampingTwo = m_wndX2.GetEditValue();

	data.nIntegrationPara = m_nIntegrationParaRdo + 1;
	data.dGamma = m_wndGamma.GetEditValue();
	data.dBeta = m_wndBeta.GetEditValue();

	data.BoundaryNL.dMinSubStepSize = m_wndMinSubStepSize.GetEditValue();
	CString StrTemp;      
	m_wndMaxIterNum.GetWindowText(StrTemp);
	CStrParser::GetINumber(StrTemp,data.BoundaryNL.nMaxIterNum);
	data.BoundaryNL.dConvergenceTol = m_wndConvTol.GetEditValue();

	return TRUE;
}

BOOL CIethItemDlg::Data2Dlg()
{
	CString csVal;
	m_wndLoadcaseName.SetWindowText(m_Data.LoadCaseName);
	csVal.Format(_T("%g"), m_Data.dEndTime);
	m_wndEndTime.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.dDelta);
	m_wndTimeInc.SetWindowText(csVal);
	csVal.Format(_T("%d"), m_Data.nOut);
	m_wndOutputStep.SetWindowText(csVal);
	m_wndDesc.SetWindowText(m_Data.Description);
	
	if(m_Data.nInitCondition == 1) 
	{
		m_wndInitCondition.SetCheck(FALSE);
		m_wndFinalStepLoad.SetCheck(FALSE);
	}
	else if(m_Data.nInitCondition == 2) 
	{
		m_wndInitCondition.SetCheck(TRUE);
		m_wndFinalStepLoad.SetCheck(FALSE);
	}
	else if(m_Data.nInitCondition == 3) 
	{
		m_wndInitCondition.SetCheck(TRUE);
		m_wndFinalStepLoad.SetCheck(TRUE);
	}
	else ASSERT(FALSE);

	if(m_Data.nInitCondition == 2 || m_Data.nInitCondition == 3) 
	{    
		int nSelect = GetFinalStateIndex(m_Data.KeyFinalState);
		m_wndFinalStateCmb.SetCurSel(nSelect);
	}

	m_nAnalTypeRdo = m_Data.nAnalType - 1;
	m_nAnalMethodRdo = m_Data.nAnalMethod - 1;
	m_nTimeHistoryTypeRdo = m_Data.nTimeHistoryType - 1;
	m_nDampingTypeRdo = m_Data.nDampingType - 1;
	
	csVal.Format(_T("%g"), m_Data.dDampAll);
	m_wndDampAllMode.SetWindowText(csVal);
	MakeItemExDamping();

	m_wndMassProp.SetCheck(m_Data.PropDamping.bMassProp);
	m_wndStiffProp.SetCheck(m_Data.PropDamping.bStiffProp);
	m_nCoefDefineRdo = m_Data.PropDamping.nCoefDefine - 1;
	if(m_nCoefDefineRdo == 0) // Direct Specification
	{
		m_wndDirectRm.SetEditUnit(m_Data.PropDamping.dRm);
		m_wndDirectRk.SetEditUnit(m_Data.PropDamping.dRk);
		m_wndCalcRm.SetEditUnit(0.);
		m_wndCalcRk.SetEditUnit(0.);
	}
	else if(m_nCoefDefineRdo == 1) // Calculate from Modal Damping
	{
		m_wndDirectRm.SetEditUnit(0.);
		m_wndDirectRk.SetEditUnit(0.);
		m_wndCalcRm.SetEditUnit(m_Data.PropDamping.dRm);
		m_wndCalcRk.SetEditUnit(m_Data.PropDamping.dRk);
	}
	m_nFreqPeriodRdo = m_Data.PropDamping.nFreqPeriod - 1;
	m_wndF1.SetEditUnit(m_Data.PropDamping.dFreqOne);
	m_wndF2.SetEditUnit(m_Data.PropDamping.dFreqTwo);
	m_wndT1.SetEditUnit(m_Data.PropDamping.dPeriodOne);
	m_wndT2.SetEditUnit(m_Data.PropDamping.dPeriodTwo);
	m_wndX1.SetEditUnit(m_Data.PropDamping.dDampingOne);
	m_wndX2.SetEditUnit(m_Data.PropDamping.dDampingTwo);

	m_nIntegrationParaRdo = m_Data.nIntegrationPara - 1;
	m_wndGamma.SetEditUnit(m_Data.dGamma);
	m_wndBeta.SetEditUnit(m_Data.dBeta);

	m_wndMinSubStepSize.SetEditUnit(m_Data.BoundaryNL.dMinSubStepSize);
	CString StrTemp;  
	StrTemp.Format(_T("%d"),m_Data.BoundaryNL.nMaxIterNum);
	m_wndMaxIterNum.SetWindowText (StrTemp);
	m_wndConvTol.SetEditUnit(m_Data.BoundaryNL.dConvergenceTol);

	UpdateData(FALSE);

	return TRUE;
}

int CIethItemDlg::GetFinalStateIndex(T_IETH_K KeyIeth)
{
	int nSelect = 0;
	T_IETH_D DataIeth;
	int nCount = m_wndFinalStateCmb.GetCount();
	if(!m_pDoc->m_pAttrCtrl->GetIeth(KeyIeth, DataIeth)) return -1;
	CString strFinalStateName = _T("");    
	for(int i = 0; i < nCount; i++)
	{    
		m_wndFinalStateCmb.GetLBText(i, strFinalStateName);
		if(DataIeth.LoadCaseName.CompareNoCase(strFinalStateName) == 0) {nSelect = i; break;}
		if(i == nCount - 1) nSelect = -1; 
	}  
	return nSelect; 
}

//---------------------------------------------------------------------------
// For Damping
void CIethItemDlg::SetListCtrlHeaderDamping()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Mode), _LS(IDS_WG_CMD__ADDD__Damping_Ratio)};
	int nColWidth[] = {90, 155};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_DampingList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_DampingList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_DampingList.InsertColumn(i,&lvcolumn);
		m_DampingList.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CIethItemDlg::GetSelectedDataDamping(int &nIndex)
{
	int iItem = m_DampingList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CIethItemDlg::MakeItemExDamping()
{
	m_DampingList.DeleteAllItems();

	int nItemCount = m_Data.aDampData.GetSize();
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_DampingList.InsertItem(nCount, _T(""));
		SetItemDamping(nCount, m_Data.aDampData[nCount]);
	}
	m_DampingList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CIethItemDlg::DataToStrDamping(int i, T_IETH_BASE &data)
{
	CString value;
	if (i == 0) value.Format(_T("%d"), data.nModeId);
	else if (i == 1) value.Format(_T("%g"), data.dDamping);
	else value = _LS(IDS_WG_CMD__ADDD__Error);
	return value;
}

void CIethItemDlg::SetItemDamping(int nIndex, T_IETH_BASE &data)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < 3; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStrDamping(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_DampingList.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CIethItemDlg::Dlg2IethBase(T_IETH_BASE &data)
{
	data.nModeId = (int)m_wndMode.GetEditValue();
	if(data.nModeId <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Mode_must_be_greater_than));
		return FALSE;
	}
	data.dDamping = m_wndDamping.GetEditValue();
	if(data.dDamping < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD_DAMPING_ERROR));
		return FALSE;  
	}
	return TRUE;
}

BOOL CIethItemDlg::IethBase2Dlg(T_IETH_BASE &data)
{
	CString csVal;
	csVal.Format(_T("%d"), data.nModeId);
	m_wndMode.SetWindowText(csVal);
	csVal.Format(_T("%g"), data.dDamping);
	m_wndDamping.SetWindowText(csVal);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIethItemDlg message handlers

BOOL CIethItemDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();

	if(m_bModify) m_wndBtnApply.EnableWindow(FALSE);
	else m_Data.Initialize();
		
	AlignControl();  
	SetListCtrlHeaderDamping();

	m_wndEndTime.SetUnitType(CUnitCtrl::m_IETH_UNIT.dEndTime);
	m_wndEndTimeUnit.SetUnitType(CUnitCtrl::m_IETH_UNIT.dEndTime);
	m_wndTimeInc.SetUnitType(CUnitCtrl::m_IETH_UNIT.dDelta);
	m_wndTimeIncUnit.SetUnitType(CUnitCtrl::m_IETH_UNIT.dDelta);
	m_wndMinSubStepSize.SetUnitType(CUnitCtrl::m_IETH_UNIT.Boundary_dMinSubStepSize);
	m_wndMinSubStepSizeUnit.SetUnitType(CUnitCtrl::m_IETH_UNIT.Boundary_dMinSubStepSize);

	m_bInitCmb = TRUE;
	SetFinalStateCmb();
	Data2Dlg();  
	CtrlManager();  

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIethItemDlg::AlignControl()
{
	// Ctrl을 Align하기 전에 같이 다룰 Ctrl을 모은다. 
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDirect, IDC_CMD_DAMPING_DIRECT_FRM, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlMass, IDC_CMD_DAMPING_MASS_FRM, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlAttribute, IDC_CMD_DAMPING_MASS_ATTRIBUTE_FRM, FALSE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlMassStiff, IDC_CMD_DAMPING_MASS_ATTRIBUTE_FRM, FALSE);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_DEFINE_RDO);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_DEFINE_RDO2);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_DIRECT_RM_EDIT);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_DIRECT_RK_EDIT);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_CALC_RM_EDIT);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_CALC_RK_EDIT);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN);  

	CRect rRef;
	CRect rToMove;
	int nDistY;
	int i;
	UINT aFirstCtrlID[] = {IDC_CMD_DAMPING_DIRECT_FRM};
	
	GetDlgItem(IDC_CMD_DAMPING_MASS_FRM)->GetWindowRect(rRef);
	for (i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXYByRect(this, IDC_CMD_DAMPING_DIRECT_FRM, 0, nDistY, TRUE, TRUE);
	}
	 
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CIethItemDlg::CtrlManager()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_ANAL_TYPE_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_ANAL_METHOD_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_TIME_HISTORY_TYPE_RDO2)->EnableWindow(FALSE);

	BOOL bMass  = m_wndMassProp.GetCheck();
	BOOL bStiff = m_wndStiffProp.GetCheck();
	BOOL bInitCondition = m_wndInitCondition.GetCheck();
//  CButton* pBtnTransient = (CButton*)GetDlgItem(IDC_CMD_TIME_HISTORY_TYPE_RDO);
//  CButton* pBtnPeriodic  = (CButton*)GetDlgItem(IDC_CMD_TIME_HISTORY_TYPE_RDO2);

	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_FINAL_STATE_STATIC,  bInitCondition);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_FINAL_STATE_COMBO,   bInitCondition);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_FINAL_STEP_LOAD_CHK, bInitCondition);  
	if(!bInitCondition) 
	{
		m_wndFinalStateCmb.SetCurSel(-1);
		m_wndFinalStepLoad.SetCheck(FALSE);
	}

	if((m_nAnalTypeRdo == 1 || m_nAnalMethodRdo == 1) && m_nTimeHistoryTypeRdo == 1) m_nTimeHistoryTypeRdo = 0;
	GetDlgItem(IDC_CMD_TIME_HISTORY_TYPE_RDO2)->EnableWindow(m_nAnalTypeRdo == 0 && m_nAnalMethodRdo == 0);
	
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_INTEGRATION_PARA_FRM, m_nAnalMethodRdo == 1, FALSE); 
		
	if(m_nAnalMethodRdo == 1 && m_nDampingTypeRdo == 0) m_nDampingTypeRdo = 1;
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_MODAL_DAMPING_TYPE_RDO, m_nAnalMethodRdo == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDirect, m_nDampingTypeRdo == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMass,   m_nDampingTypeRdo == 1);
	
	GetDlgItem(IDC_CMD_DAMPING_MASS_ATTRIBUTE_FRM)->EnableWindow(m_nDampingTypeRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_DEFINE_RDO)->EnableWindow(m_nDampingTypeRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_DEFINE_RDO2)->EnableWindow(m_nDampingTypeRdo == 1 && (bMass || bStiff));  
	GetDlgItem(IDC_CMD_DAMPING_MASS_DIRECT_RM_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 0 && bMass);
	GetDlgItem(IDC_CMD_DAMPING_MASS_DIRECT_RK_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 0 && bStiff);
	GetDlgItem(IDC_CMD_DAMPING_MASS_CALC_RM_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 1 && bMass);
	GetDlgItem(IDC_CMD_DAMPING_MASS_CALC_RK_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 1 && bStiff);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlAttribute, m_nCoefDefineRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_F1_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 0 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_T1_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_X1_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_F2_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 0 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_T2_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_X2_EDIT)->EnableWindow(m_nDampingTypeRdo == 1 && m_nCoefDefineRdo == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN)->EnableWindow(m_nDampingTypeRdo == 1 && (bMass || bStiff));

	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_INTEGRATION_PARA_GAMMA_EDIT, m_nAnalMethodRdo == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_INTEGRATION_PARA_BETA_EDIT,  m_nAnalMethodRdo == 1);
	CEdit* pEditGamma = (CEdit*)GetDlgItem(IDC_CMD_INTEGRATION_PARA_GAMMA_EDIT);
	CEdit* pEditBeta  = (CEdit*)GetDlgItem(IDC_CMD_INTEGRATION_PARA_BETA_EDIT);
	pEditGamma->SetReadOnly(m_nAnalMethodRdo == 1 && m_nIntegrationParaRdo != 2);
	pEditBeta->SetReadOnly(m_nAnalMethodRdo == 1 && m_nIntegrationParaRdo != 2);
	
	CDlgUtil::CtrlEnableDisableByRect(this,IDC_CMD_NONLINEAR_ANAL_CONTROL_FRM, m_nAnalTypeRdo == 1, FALSE);  
	UpdateData(FALSE);
}

void CIethItemDlg::OnCallAutoCalcCoef()
{
	AutoCalcCoef();
}

void CIethItemDlg::OnInitConditionChk()
{
	CtrlManager();
}

void CIethItemDlg::OnChangeAnalType() 
{
	CtrlManager();
	SetFinalStateCmb();
}

void CIethItemDlg::OnChangeAnalMethod()
{
	CtrlManager();
	SetFinalStateCmb();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CIethItemDlg::OnChangeTimeHistoryType()
{
	CtrlManager();
}

void CIethItemDlg::OnChangeDampingType()
{
	CtrlManager();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CIethItemDlg::OnChangeCoefDefine()
{
	CtrlManager();  
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CIethItemDlg::OnChangeFreqPeriod()
{
	CtrlManager();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CIethItemDlg::OnMassStiffChk()
{
	CtrlManager();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CIethItemDlg::OnChangeIntegrationPara()
{
	CtrlManager();
	if(m_nIntegrationParaRdo == 0)
	{
		m_wndGamma.SetEditUnit(0.5);
		m_wndBeta.SetEditUnit(0.25);
	}
	else if(m_nIntegrationParaRdo == 1)
	{
		m_wndGamma.SetEditUnit(0.5);
		m_wndBeta.SetEditUnit(1./6.);
	}
}

void CIethItemDlg::OnDeltaposDouble(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	CEditUnit* pEdit;
	if (nID == IDC_CMD_END_TIME_SPIN) pEdit = &m_wndEndTime;
	else if (nID == IDC_CMD_TIME_INC_SPIN) pEdit = &m_wndTimeInc;
	else 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Unknowon_spin_button));
		return;
	}
	CString csVal;
	double dblValue = pEdit->GetEditValue();
	dblValue -= pNMUpDown->iDelta;
	if (dblValue < 0.0) dblValue = 0.0;
	csVal.Format(_T("%g"), dblValue);
	pEdit->SetWindowText(csVal);
	
	*pResult = 0;
}

void CIethItemDlg::OnDeltaposInteger(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	CEditUnit* pEdit;
	if (nID == IDC_CMD_OUTPUT_STEP_SPIN) pEdit = &m_wndOutputStep;
	else if (nID == IDC_CMD_MODE_SPIN) pEdit = &m_wndMode;
	else 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Unknowon_spin_button));
		return;
	}
	CString csVal;
	int      iVal = (int)pEdit->GetEditValue();
	iVal -= pNMUpDown->iDelta;
	if (iVal < 1) iVal = 1;
	csVal.Format(_T("%d"), iVal);
	pEdit->SetWindowText(csVal);
	
	*pResult = 0;
}

void CIethItemDlg::OnDampingAdd() 
{
	// TODO: Add your control notification handler code here
	T_IETH_BASE data;
	
	if (!Dlg2IethBase(data)) return;
	int nSize = m_Data.aDampData.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (m_Data.aDampData[i].nModeId == data.nModeId)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Mode__d_already_exist_), data.nModeId);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.aDampData.Add(data);
	m_DampingList.InsertItem(nSize, _T(""));
	SetItemDamping(nSize, data);
	m_DampingList.SetItemState(nSize, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

	CString csVal;
	csVal.Format(_T("%d"), data.nModeId+1);
	m_wndMode.SetWindowText(csVal);
	GotoDlgCtrl(&m_wndDamping);
}

void CIethItemDlg::OnDampingModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedDataDamping(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_modal_damping));
		return;
	}

	T_IETH_BASE data;
	if (!Dlg2IethBase(data)) return;

	int nSize = m_Data.aDampData.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		if (m_Data.aDampData[i].nModeId == data.nModeId)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Mode__d_already_exist_), data.nModeId);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.aDampData[nIndex] = data;
	SetItemDamping(nIndex, data);
	m_DampingList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CIethItemDlg::OnDampingDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedDataDamping(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_modal_damping));
		return;
	}

	m_DampingList.DeleteItem(nIndex);
	m_Data.aDampData.RemoveAt(nIndex);
	int nSize = m_Data.aDampData.GetSize();
	if (nSize > 0)
	{
		if (nIndex >= nSize) nIndex--;
		m_DampingList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}	
}

void CIethItemDlg::OnChangedCurIethBase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex;
	T_IETH_BASE data;
	if (!GetSelectedDataDamping(nIndex))
		data.Initialize();
	else data = m_Data.aDampData[nIndex];

	// 값을 설정한다.
	IethBase2Dlg(data);

	*pResult = 0;
}

BOOL CIethItemDlg::ApplyOrOK() 
{
	// TODO: Add extra validation here
	T_IETH_D data;
	if (!Dlg2Data(data)) return FALSE;
	if (!ValidData(data)) return FALSE;

	BOOL bSuccess;
	if (m_bModify)
	{
		data.LoadCaseId = m_nOldID;
		bSuccess = m_pDoc->m_pDataCtrl->ModifyIeth(m_csOldName, data);
	}
	else bSuccess = m_pDoc->m_pDataCtrl->AddIeth(data);

	if (!bSuccess) return FALSE;
	
	return TRUE;
}

void CIethItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;

	CDialogMove::OnOK();
}

void CIethItemDlg::OnCmdApply() 
{	
	if (!ApplyOrOK()) return;

	SetFinalStateCmb();
	m_bModify = FALSE;
//  m_OldKey = m_Data;
}

void CIethItemDlg::SetFinalStateCmb()
{
	UpdateData();
	m_wndFinalStateCmb.ResetContent();
	CArray<T_IETH_K, T_IETH_K> rIethKeyList;
	T_IETH_K KeyIeth;
	T_IETH_D DataIeth;
	m_pDoc->m_pAttrCtrl->GetIethKeyList(rIethKeyList);
	int nCount = rIethKeyList.GetSize();

	for(int i = 0; i < nCount; i++)
	{
		KeyIeth = rIethKeyList.GetAt(i);
		m_pDoc->m_pAttrCtrl->GetIeth(KeyIeth, DataIeth);
		if(m_bModify)
		{
			if(m_bInitCmb)  // 처음 수정모드로 열 때
			{        
				if(m_Data.nAnalType == DataIeth.nAnalType && m_Data.nAnalMethod == DataIeth.nAnalMethod)
					if(DataIeth.LoadCaseId < m_Data.LoadCaseId)
						m_wndFinalStateCmb.AddString(DataIeth.LoadCaseName);
			}
			else  // 수정모드이지만 처음이 아닐 때
			{
				if(m_nAnalTypeRdo == DataIeth.nAnalType - 1 && m_nAnalMethodRdo == DataIeth.nAnalMethod - 1)
					if(DataIeth.LoadCaseId < m_Data.LoadCaseId)
						m_wndFinalStateCmb.AddString(DataIeth.LoadCaseName);
			}
		}
		else  // 수정모드가 아닐 때
		{
			if(m_nAnalTypeRdo == DataIeth.nAnalType - 1 && m_nAnalMethodRdo == DataIeth.nAnalMethod - 1)
				m_wndFinalStateCmb.AddString(DataIeth.LoadCaseName);
		}
	}
	m_bInitCmb = FALSE;
}

void CIethItemDlg::OnDeltaposCmdMaxIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//IDC_CMD_MAX_ITER_NUM_SPIN
	/*
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
	*/
	CDlgUtil::SetSpinValue(this,IDC_CMD_MAX_ITER_NUM_EDIT,1,-1,pNMHDR,pResult);
}

BOOL CIethItemDlg::ValidData(T_IETH_D& data)
{
	if (data.LoadCaseName.IsEmpty()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Loadcase_name_is_not_inpu));
		return FALSE;
	}
	if (data.dEndTime <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___End_of_time_must_be_great));
		return FALSE;
	}
	if (data.dDelta <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Time_increment_must_be_gr));
		return FALSE;
	}
	if (data.nOut < 1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Number_of_output_step_mus));
		return FALSE;
	}
		
	return TRUE;
}

void CIethItemDlg::AutoCalcCoef()
{
	UpdateData(TRUE);
	T_IETH_D rData;
	rData.Initialize();
	rData.nDampingType            = m_nDampingTypeRdo + 1;  
	rData.PropDamping.nCoefDefine = m_nCoefDefineRdo + 1;
	rData.PropDamping.nFreqPeriod = m_nFreqPeriodRdo + 1;  
	rData.PropDamping.bMassProp   = m_wndMassProp.GetCheck();
	rData.PropDamping.bStiffProp  = m_wndStiffProp.GetCheck();
	rData.PropDamping.dFreqOne    = m_wndF1.GetEditValue();
	rData.PropDamping.dFreqTwo    = m_wndF2.GetEditValue();
	rData.PropDamping.dPeriodOne  = m_wndT1.GetEditValue();
	rData.PropDamping.dPeriodTwo  = m_wndT2.GetEditValue();
	rData.PropDamping.dDampingOne = m_wndX1.GetEditValue();
	rData.PropDamping.dDampingTwo = m_wndX2.GetEditValue();   
	
	if(m_pDoc->m_pEditData->AutoCalcIethCoef(rData))
	{
		m_wndCalcRm.SetEditUnit(rData.PropDamping.dRm);
		m_wndCalcRk.SetEditUnit(rData.PropDamping.dRk);
	}
	else
	{
		if(rData.PropDamping.bMassProp)  m_wndCalcRm.SetWindowText(_T("--"));
		if(rData.PropDamping.bStiffProp) m_wndCalcRk.SetWindowText(_T("--"));
	}
}

void CIethItemDlg::OnShowDampingBtn()
{
	UpdateData();
	
	CThisCalcDamping dlg;
	
	CRect rectShowBtn, rMain;  
	m_wndShowBtn.GetWindowRect(rectShowBtn);    
	GetWindowRect(rMain);
	int nDistY = rMain.bottom - rectShowBtn.top;
	dlg.SetInitPos(D_INIT_POS_RB, 0, -nDistY);
		
	double dRm = 0., dRk = 0.;
	switch(m_nCoefDefineRdo)
	{
		case 0 : 
			dRm = m_wndDirectRm.GetEditValue();
			dRk = m_wndDirectRk.GetEditValue();
			break;
		case 1:
		default:
			dRm = m_wndCalcRm.GetEditValue();
			dRk = m_wndCalcRk.GetEditValue();
			break;
	}

	dlg.m_bMass  = m_wndMassProp.GetCheck();
	dlg.m_bStiff = m_wndStiffProp.GetCheck();
	dlg.m_dRm		 = dRm;
	dlg.m_dRk	   = dRk;
	dlg.m_nFreqPeriodRdo = m_nFreqPeriodRdoDlg;
	dlg.m_dFrequency =  m_dFrequencyDlg;
	dlg.m_dPeriod = m_dPeriodDlg;  
	
	if(dlg.DoModal() == IDOK)
	{
		m_nFreqPeriodRdoDlg = dlg.m_nFreqPeriodRdo;
		m_dFrequencyDlg = dlg.m_dFrequency;
		m_dPeriodDlg = dlg.m_dPeriod;
	}
}
