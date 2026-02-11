// ThisItemDlg_MEC.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ThisItemDlg_MEC.h"
#include "ThisCalcDamping.h"
#include "ThisIterationCtrlDlg.h"
#include "NtisIterationCtrlDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\QSort.h"

#include "ThisNoticeDlg.h"
#include "DampGrid.h"
#include "ThisItemDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThisItemDlg_MEC dialog


CThisItemDlg_MEC::CThisItemDlg_MEC(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CThisItemDlg_MEC::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_nFreqPeriodRdoDlg = 0;
	m_dFrequencyDlg = 1.0;
	m_dPeriodDlg = 1.0;
	m_Data.Initialize();
	//{{AFX_DATA_INIT(CThisItemDlg_MEC)  
	m_nAnalTypeRdo = 0;	
	m_nAnalMethodRdo = 0;
	m_nTimeHistoryTypeRdo = 0;
	m_nGeomNonlinearTypeRdo = 0;
	m_nDampingTypeRdo = 0;
	m_nCoefDefineRdo = 0;
	m_nFreqPeriodRdo = 0;
  //m_nIntegrationParaRdo = 0;
	m_nIntegrationParam = 1;
	m_nStaticIncrementRdo = 0;
	m_nStaticCtrlOptRdo = 0;
	m_nSubsequentType = -1;
	m_nTimeIntegrationParam = 0;

	int nMovingType = CProduct::GetMovingType();
	m_bJapanMode = (nMovingType == D_PRODUCT_MOVING_JP || nMovingType == D_PRODUCT_MOVING_ALL) ? TRUE : FALSE;
	//}}AFX_DATA_INIT

	m_aCtrlUpdate.Add(IDC_CMD_DAMP_UPDATE_TEXT);
	m_aCtrlUpdate.Add(IDC_CMD_DAMP_UPDATE_CMB);
	//m_aCtrlUpdate.Add(IDC_CMD_DAMP_UPDATE_RD2);
	m_aCtrlUpdate.Add(IDC_CMD_THIS_ITEM_DESC_BTN);
	
	m_aCtrlUpdateRadio.Add(IDC_CMD_DAMP_UPDATE_RD1);
	m_aCtrlUpdateRadio.Add(IDC_CMD_DAMP_UPDATE_RD2);

	m_aCtrlOption.Add(IDC_CMD_CTRL_OPT_RDO1);
	m_aCtrlOption.Add(IDC_CMD_CTRL_OPT_RDO2);
	m_aCtrlOption.Add(IDC_CMD_GLOBAL_TEXT);
	m_aCtrlOption.Add(IDC_CMD_CTRL_OPT_EDIT1);
	m_aCtrlOption.Add(IDC_CMD_CTRL_OPT_UNIT1);
	m_aCtrlOption.Add(IDC_CMD_MASTER_NODE);
	m_aCtrlOption.Add(IDC_CMD_CTRL_OPT_EDIT2);
	m_aCtrlOption.Add(IDC_CMD_MASTER_DIRECTION);
	m_aCtrlOption.Add(IDC_CMD_CTRL_OPT_DIR_CMB);
	m_aCtrlOption.Add(IDC_CMD_MAX_DISP);
	m_aCtrlOption.Add(IDC_CMD_CTRL_OPT_EDIT3);
	m_aCtrlOption.Add(IDC_CMD_CTRL_OPT_UNIT2);

	m_aCtrlOption2.Add(IDC_CMD_LOAD_CONT_STATIC);
	m_aCtrlOption2.Add(IDC_CMD_CTRL_OPT_SCALE_EDT);

	m_aCtrlInitLoad_Sequence.Add(IDC_CMD_SUBSEQUENT_CHK);
	m_aCtrlInitLoad_Sequence.Add(IDC_CMD_SUBSEQUENT_TYPE_RDO);
	m_aCtrlInitLoad_Sequence.Add(IDC_CMD_FINAL_STATE_COMBO);
	m_aCtrlInitLoad_Sequence.Add(IDC_CMD_SUBSEQUENT_TYPE_RDO2);
//#ifdef _MGEN
	m_aCtrlInitLoad_Sequence.Add(IDC_CMD_SUBSEQUENT_TYPE_RDO3);
//#endif // _MGEN
	//m_aGridModelList.Add(IDC_CMD_SUBSEQUENT_TYPE_RDO4);
	m_aGridModelList.Add(IDC_CMD_SEC_STATIC);
	m_aGridModelList.Add(IDC_CMD_GRID_LOAD_CASE_CMB);
	m_aGridModelList.Add(IDC_CMD_LD_LIST2);
	m_aGridModelList.Add(IDC_CMD_BTN_LD_ADD2);
	m_aGridModelList.Add(IDC_CMD_BTN_LD_DEL2);

	m_aCtrlInitLoad_Initial.Add(IDC_USE_INITIAL_CHK);
	m_aCtrlInitLoad_Initial.Add(IDC_USE_INITIAL_STC);

	m_aCtrlNEWMARK.RemoveAll();
	m_aCtrlNEWMARK.Add(IDC_CMD_INTEGRATION_PARA_NEWMARK_CMB);
	m_aCtrlNEWMARK.Add(IDC_CMD_INTEGRATION_PARA_GAMMA_STATIC);
	m_aCtrlNEWMARK.Add(IDC_CMD_INTEGRATION_PARA_GAMMA_EDIT);
	m_aCtrlNEWMARK.Add(IDC_CMD_INTEGRATION_PARA_BETA_STATIC);
	m_aCtrlNEWMARK.Add(IDC_CMD_INTEGRATION_PARA_BETA_EDIT);

	m_aCtrl_GeomNonlinearType.RemoveAll();
	m_aCtrl_GeomNonlinearType.Add(IDC_CMD_GEOM_NONLINEAR_TYPE1_RDO);  //None
	m_aCtrl_GeomNonlinearType.Add(IDC_CMD_GEOM_NONLINEAR_TYPE3_RDO);  //P-Delta
	m_aCtrl_GeomNonlinearType.Add(IDC_CMD_GEOM_NONLINEAR_TYPE2_RDO);  //Large Disp.          

	m_pGrid = new CDampGrid;
	m_bUseValidNllp=FALSE;
}

CThisItemDlg_MEC::~CThisItemDlg_MEC()
{
	if(m_pGrid) delete m_pGrid;
}

BOOL CThisItemDlg_MEC::ExternalSetting(const unsigned key)
{
	BOOL bFlag = TRUE;
	bFlag = ::IsWindow(this->GetSafeHwnd());                // 윈도우가 생성되지 않았으면
	bFlag = m_pDoc->m_pAttrCtrl->ExistThis(key);        // 키값이 존재하지 않으면
	bFlag = m_pDoc->m_pAttrCtrl->GetThis(key, m_Data);  // Data를 가져올 수 없으면
	if(!bFlag) return bFlag;

	m_csOldName = m_Data.LoadCaseName;
	m_nOldID = m_Data.LoadCaseId;
	m_bModify = TRUE;

	m_bInitCmb = TRUE;

	SetFinalStateCmb();
	SetGridDeadLoadCmb();
	Data2Dlg();
	CtrlManager();

	return TRUE;
}

void CThisItemDlg_MEC::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThisItemDlg_MEC)  
	DDX_Control(pDX, IDC_CMD_SUBSEQUENT_CHK, m_wndSubsequentChk);
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadcaseName);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDesc);
	DDX_Control(pDX, IDC_CMD_NONLINEAR_LINK_CHK, m_wndNonlinearLink);
	DDX_Control(pDX, IDC_CMD_INELASTIC_HINGE_CHK, m_wndInelasticHinge);
	DDX_Control(pDX, IDC_CMD_FINAL_STEP_LOAD_CHK, m_wndFinalStepLoad);
	DDX_Control(pDX, IDC_CMD_FINAL_STEP_ACC_CHK, m_wndFinalStepAcc);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN, m_wndShowBtn);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_TYPE_MASS_CHK, m_wndMassProp);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_TYPE_STIFF_CHK, m_wndStiffProp);
	DDX_Control(pDX, IDC_CMD_APPLY, m_wndBtnApply);  
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
	DDX_Control(pDX, IDC_CMD_DAMPING_ALL_MODES, m_wndDampAllMode);
	DDX_Control(pDX, IDC_CMD_OUTPUT_STEP_EDIT, m_wndOutputStep);
	DDX_Control(pDX, IDC_CMD_TIME_INC_UNIT, m_wndTimeIncUnit);
	DDX_Control(pDX, IDC_CMD_TIME_INC_EDIT, m_wndTimeInc);
	DDX_Control(pDX, IDC_CMD_END_TIME_UNIT, m_wndEndTimeUnit);
	DDX_Control(pDX, IDC_CMD_END_TIME_EDIT, m_wndEndTime);  
	DDX_Control(pDX, IDC_CMD_CTRL_OPT_EDIT1, m_wndOptEdit1);
	DDX_Control(pDX, IDC_CMD_CTRL_OPT_EDIT2, m_wndOptEdit2);
	DDX_Control(pDX, IDC_CMD_CTRL_OPT_EDIT3, m_wndOptEdit3);
	DDX_Control(pDX, IDC_CMD_FINAL_STATE_COMBO, m_wndFinalStateCmb);
	DDX_Control(pDX, IDC_CMD_CTRL_OPT_DIR_CMB, m_wndOptDirCmb);
	DDX_Control(pDX, IDC_CMD_THIS_DAMP_METHOD_CMB, m_wndDampingMethodCmb);
	DDX_Radio(pDX, IDC_CMD_ANAL_TYPE_RDO,      m_nAnalTypeRdo);
	DDX_Radio(pDX, IDC_CMD_ANAL_METHOD_RDO,       m_nAnalMethodRdo);
	DDX_Radio(pDX, IDC_CMD_TIME_HISTORY_TYPE_RDO, m_nTimeHistoryTypeRdo);
	//DDX_Radio(pDX, IDC_CMD_GEOM_NONLINEAR_TYPE1_RDO, m_nGeomNonlinearTypeRdo);
	DDX_Radio(pDX, IDC_CMD_DAMPING_MASS_DEFINE_RDO, m_nCoefDefineRdo);
	DDX_Radio(pDX, IDC_CMD_DAMPING_MASS_FREQ_PERIOD_RDO, m_nFreqPeriodRdo); 
	//DDX_Radio(pDX, IDC_CMD_INTEGRATION_PARA_RDO, m_nIntegrationParaRdo);
	DDX_Radio(pDX, IDC_CMD_STATIC_CONTROL_RDO1, m_nStaticIncrementRdo);
	DDX_Radio(pDX, IDC_CMD_CTRL_OPT_RDO1, m_nStaticCtrlOptRdo);
	DDX_Control(pDX, IDC_CMD_CTRL_OPT_UNIT1, m_wndTransUnit);
	DDX_Control(pDX, IDC_CMD_CTRL_OPT_UNIT2, m_wndIncreDispUnit);
	DDX_Control(pDX, IDC_CMD_STATIC_OUTPUT_CHK, m_wndOptputOpt);
	DDX_Control(pDX, IDC_CMD_CUMULATE_DVA_CHK, m_wndCumulateDVACheck);
	DDX_Control(pDX, IDC_CMD_INCRESTEP_EDT, m_wndIncreStep);
	DDX_Control(pDX, IDC_CMD_CTRL_OPT_SCALE_EDT, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_ITERATION_CHECK,    m_wndIteration);
	DDX_Radio(pDX, IDC_CMD_SUBSEQUENT_TYPE_RDO, m_nSubsequentType);
	DDX_Control(pDX, IDC_USE_INITIAL_CHK, m_chkUseInitial);
	DDX_Control(pDX, IDC_CMD_INITLOAD_METHOD_CMB, m_cmbInitLoadMethod);
	DDX_Radio(pDX, IDC_CMD_INTEGRATION_PARA_NEWMARK_RDO, m_nTimeIntegrationParam);
	DDX_Control(pDX, IDC_CMD_DAMP_UPDATE_CMB, m_cmbDampUpdate);
	DDX_Control(pDX, IDC_CMD_INTEGRATION_PARA_NEWMARK_CMB, m_cmbIntegrationParam);
	DDX_Control(pDX, IDC_CMD_GRID_LOAD_CASE_CMB, m_cmbGridLoadCase);
	DDX_Control(pDX, IDC_CMD_LD_LIST2, m_GridList);
	DDX_Control(pDX, IDC_CMD_INITIAL_CONDITION_FRM, m_wndInitLoadGroupBox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThisItemDlg_MEC, CCMDlgBase)
	//{{AFX_MSG_MAP(CThisItemDlg_MEC)
	ON_BN_CLICKED(IDC_CMD_ITERATION_CHECK, OnChangeIteration)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_END_TIME_SPIN, OnDeltaposDouble)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_OUTPUT_STEP_SPIN, OnDeltaposInteger)	
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_ANAL_TYPE_RDO, OnChangeAnalType)
	ON_BN_CLICKED(IDC_CMD_ANAL_METHOD_RDO, OnChangeAnalMethod)
	ON_BN_CLICKED(IDC_CMD_TIME_HISTORY_TYPE_RDO, OnChangeTimeHistoryType)
	ON_BN_CLICKED(IDC_CMD_GEOM_NONLINEAR_TYPE1_RDO, OnChangeGeomNonlinearType)
	ON_BN_CLICKED(IDC_CMD_GEOM_NONLINEAR_TYPE2_RDO, OnChangeGeomNonlinearType)
	ON_BN_CLICKED(IDC_CMD_GEOM_NONLINEAR_TYPE3_RDO, OnChangeGeomNonlinearType)
	ON_BN_CLICKED(IDC_CMD_SEQUENCE_LOAD_RDO, OnChangeSequenceLoad)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_DEFINE_RDO, OnChangeCoefDefine)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_FREQ_PERIOD_RDO, OnChangeFreqPeriod)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN, OnShowDampingBtn)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_TYPE_MASS_CHK, OnMassStiffChk)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_F1_EDIT, OnCallAutoCalcCoef)
	ON_BN_CLICKED(IDC_CMD_STATIC_CONTROL_RDO1, OnStaticIncMethod)
	ON_BN_CLICKED(IDC_CMD_CTRL_OPT_RDO1, OnStaticCtrlOpt)
	ON_CBN_SELCHANGE(IDC_CMD_THIS_DAMP_METHOD_CMB, OnSelchangeCmdThisDampMethodCmb)
	ON_CBN_SELCHANGE(IDC_CMD_FINAL_STATE_COMBO, OnSelchangeCmdThisDampMethodCmb)
	ON_BN_CLICKED(IDC_CMD_SUBSEQUENT_TYPE_RDO, OnCmdSubsequentTypeRdo)
	ON_BN_CLICKED(IDC_CMD_SUBSEQUENT_TYPE_RDO2, OnCmdSubsequentTypeRdo)
	ON_BN_CLICKED(IDC_CMD_SUBSEQUENT_TYPE_RDO3, OnCmdSubsequentTypeRdo)
	ON_BN_CLICKED(IDC_CMD_SUBSEQUENT_TYPE_RDO4, OnCmdSubsequentTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ANAL_TYPE_RDO2, OnChangeAnalType)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_F2_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_T1_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_T2_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_X1_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_X2_EDIT, OnCallAutoCalcCoef)	
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TIME_INC_SPIN, OnDeltaposDouble)	
	ON_BN_CLICKED(IDC_CMD_ANAL_METHOD_RDO2, OnChangeAnalMethod)
	ON_BN_CLICKED(IDC_CMD_ANAL_METHOD_RDO3, OnChangeAnalMethod)
	ON_BN_CLICKED(IDC_CMD_TIME_HISTORY_TYPE_RDO2, OnChangeTimeHistoryType)
	ON_BN_CLICKED(IDC_CMD_SEQUENCE_LOAD_RDO2, OnChangeSequenceLoad)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_DEFINE_RDO2, OnChangeCoefDefine)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_FREQ_PERIOD_RDO2, OnChangeFreqPeriod)
// 	ON_BN_CLICKED(IDC_CMD_INTEGRATION_PARA_RDO, OnChangeIntegrationPara)
// 	ON_BN_CLICKED(IDC_CMD_INTEGRATION_PARA_RDO2, OnChangeIntegrationPara)
// 	ON_BN_CLICKED(IDC_CMD_INTEGRATION_PARA_RDO3, OnChangeIntegrationPara)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_TYPE_STIFF_CHK, OnMassStiffChk)
	ON_BN_CLICKED(IDC_CMD_STATIC_CONTROL_RDO2, OnStaticIncMethod)
	ON_BN_CLICKED(IDC_CMD_CTRL_OPT_RDO2, OnStaticCtrlOpt)
	ON_BN_CLICKED(IDC_CMD_SUBSEQUENT_CHK, OnCmdSubsequentChk)
	ON_BN_CLICKED(IDC_CMD_ITERATION_BTN, OnCmdIterationCtrlBtn)
	ON_BN_CLICKED(IDC_CMD_THIS_ITEM_DESC_BTN, OnCmdDescBtn)
	ON_BN_CLICKED(IDC_CMD_DAMP_UPDATE_RD1, CtrlManager)
	ON_BN_CLICKED(IDC_CMD_DAMP_UPDATE_RD2, CtrlManager)
	ON_BN_CLICKED(IDC_USE_INITIAL_CHK           , OnChkUseInitial)
	ON_CBN_SELCHANGE(IDC_CMD_INITLOAD_METHOD_CMB, OnCmdInitLoadMethod)
	ON_BN_CLICKED(IDC_CMD_FINAL_STEP_LOAD_CHK, OnChkKeepFinalStep)
	ON_BN_CLICKED(IDC_CMD_INTEGRATION_PARA_NEWMARK_RDO, OnNewmarkOpt)
	ON_BN_CLICKED(IDC_CMD_INTEGRATION_PARA_NEWMARK_RDO2, OnNewmarkOpt)
	ON_CBN_SELCHANGE(IDC_CMD_INTEGRATION_PARA_NEWMARK_CMB, OnSelchangeCmdNemark)
	ON_BN_CLICKED(IDC_CMD_BTN_LD_ADD2, OnCmdBtnLdGridAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_LD_DEL2, OnCmdBtnLdGridDel)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnBnClickedInitLoadFoldButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// Interfaces
//////////////////////////////////////////////////////////////////////////////
void CThisItemDlg_MEC::SetParamData(T_THIS_D &data) 
{ 
	m_Data = data; 
	m_csOldName = m_Data.LoadCaseName;
	m_nOldID = m_Data.LoadCaseId;
	m_bModify = TRUE; 
}

//////////////////////////////////////////////////////////////////////////////
// Implementations
//////////////////////////////////////////////////////////////////////////////
BOOL CThisItemDlg_MEC::Dlg2Data()  
{
	// 일단 데이터를 받고 에러 체크는 DB에서 한다.
	UpdateData(TRUE);
	m_wndLoadcaseName.GetWindowText(m_Data.LoadCaseName);
	m_Data.dEndTime = m_wndEndTime.GetEditValue();
	m_Data.dDelta = m_wndTimeInc.GetEditValue();
	m_Data.nOut = m_wndOutputStep.GetEditValueInt();
	m_wndDesc.GetWindowText(m_Data.Description);
	m_Data.nAnalType = m_nAnalTypeRdo + 1;
	m_Data.nAnalMethod = m_nAnalMethodRdo + 1;
	m_Data.nTimeHistoryType = m_nTimeHistoryTypeRdo + 1;
	m_Data.nGeomNonlinearType = m_nGeomNonlinearTypeRdo;
	m_Data.nInitCondition = 0;
	if(m_wndSubsequentChk.GetCheck()) m_Data.nInitCondition = 1;  
	m_Data.nSubsequentType = m_nSubsequentType;
	m_Data.bFinalStepLoad = m_wndFinalStepLoad.GetCheck();
	m_Data.bFinalStepAcc = m_wndFinalStepAcc.GetCheck();
	m_Data.nIncrementStep = m_wndIncreStep.GetEditValue();
	m_Data.bCumulateDVA = m_wndCumulateDVACheck.GetCheck();
		
	m_Data.nInitLoadMethod = CDlgUtil::CobxGetCurSelItemData(m_cmbInitLoadMethod, m_cmbInitLoadMethod.GetCurSel());
	if(m_Data.nInitLoadMethod==D_THIS_INITMETHOD_ORDER_IN_SEQLOAD && m_Data.nInitCondition == 1 && m_Data.nSubsequentType == 0)
	{
		CString strFinalStateName;    
		m_wndFinalStateCmb.GetWindowText(strFinalStateName);
		CString strLoadCaseType = strFinalStateName.Left(2);
		int nStrLength = strFinalStateName.GetLength();
		CString strLoadCaseName = strFinalStateName.Right(nStrLength - 5);
		if(strLoadCaseType == _T("ST"))
		{
			m_Data.nSubLoadCaseType = 0;
			m_Data.KeyFinalState = m_pDoc->m_pAttrCtrl->GetStldKey(strLoadCaseName);
		}
		else if(strLoadCaseType == _T("CS"))
		{
			m_Data.nSubLoadCaseType = 1;
			if(strLoadCaseName.CompareNoCase(_LS(IDS_CMD_THIS_STAG_TYPE1)) == 0) m_Data.KeyFinalState = 1;
			else if(strLoadCaseName.CompareNoCase(_LS(IDS_CMD_THIS_STAG_TYPE2)) == 0) m_Data.KeyFinalState = 2;      
		}
		else if(strLoadCaseType == _T("TH"))
		{
			m_Data.nSubLoadCaseType = 2;
			m_Data.KeyFinalState = m_pDoc->m_pAttrCtrl->GetThisKey(strLoadCaseName);
		}    
		else
		{
			m_Data.KeyFinalState = 0;
		}
	}
	else 
	{
		m_Data.nSubLoadCaseType = 0;
		m_Data.KeyFinalState = 0;
	}

	// Grid Case
	m_Data.aGilcCase.RemoveAll();
	if (m_Data.nInitLoadMethod == D_THIS_INITMETHOD_ORDER_IN_SEQLOAD && m_Data.nInitCondition == 1 && m_Data.nSubsequentType == 3)
	{
		CArray<T_GILC_K, T_GILC_K> aGridGilcK;

		int nNumGilcCase = m_GridList.GetItemCount();
		for (int i = 0; i < nNumGilcCase; i++)
		{
			UINT GilcK = m_GridList.GetItemData(i);
			aGridGilcK.Add(GilcK);
		}
		CQSort::QSortUInt(aGridGilcK.GetData(), aGridGilcK.GetSize());
		for (int i = 0; i < nNumGilcCase; i++)
		{
			m_Data.aGilcCase.Add(aGridGilcK[i]);
		}
	}

	if (m_Data.nAnalType == 1 && m_Data.nAnalMethod == 1 && m_Data.nTimeHistoryType == 2) // Linear - Modal - Periodic
	{
		if (m_Data.nSubLoadCaseType == 2)
		{
			T_THIS_D DataThisPre;
			m_pDoc->m_pAttrCtrl->GetThis(m_Data.KeyFinalState, DataThisPre);
			if (DataThisPre.nTimeHistoryType == 2)
			{
				CString strText;  
				strText.Format(_LS(IDS_WG_CMD__ADDD__Error___Time_Periodic), m_Data.LoadCaseName);
				AfxMessageBox(strText);
				return FALSE;
			}
		}
	}

	int nIndex = m_wndDampingMethodCmb.GetCurSel();
	if (nIndex == CB_ERR) return FALSE;
	m_Data.nDampingType = m_wndDampingMethodCmb.GetItemData(nIndex);  
//  m_Data.nDampingType = m_nDampingMethod;
	m_Data.dDampAll = m_wndDampAllMode.GetEditValue();
	m_pGrid->GetData(m_Data.aDampData);
	m_Data.PropDamping.bMassProp = m_wndMassProp.GetCheck();
	m_Data.PropDamping.bStiffProp = m_wndStiffProp.GetCheck();
	m_Data.PropDamping.nCoefDefine = m_nCoefDefineRdo + 1;
	if(m_nCoefDefineRdo == 0) // Direct Specification
	{
		m_Data.PropDamping.dRm = m_wndDirectRm.GetEditValue();
		m_Data.PropDamping.dRk = m_wndDirectRk.GetEditValue();
	}
	else if(m_nCoefDefineRdo == 1) // Calculate from Modal Damping
	{
		m_Data.PropDamping.dRm = m_wndCalcRm.GetEditValue();
		m_Data.PropDamping.dRk = m_wndCalcRk.GetEditValue();
	}
	m_Data.PropDamping.nFreqPeriod = m_nFreqPeriodRdo + 1;
	m_Data.PropDamping.dFreqOne = m_wndF1.GetEditValue();
	m_Data.PropDamping.dFreqTwo = m_wndF2.GetEditValue();
	m_Data.PropDamping.dPeriodOne = m_wndT1.GetEditValue();
	m_Data.PropDamping.dPeriodTwo = m_wndT2.GetEditValue();
	m_Data.PropDamping.dDampingOne = m_wndX1.GetEditValue();
	m_Data.PropDamping.dDampingTwo = m_wndX2.GetEditValue();

	m_nDampingMethod = CDlgUtil::CobxGetCurSelItemData(m_wndDampingMethodCmb, m_wndDampingMethodCmb.GetCurSel());

	m_Data.nIntegrationPara = m_nIntegrationParam;
  //m_Data.nIntegrationPara = m_nIntegrationParaRdo + 1;
	m_Data.dGamma = m_wndGamma.GetEditValue();
	m_Data.dBeta = m_wndBeta.GetEditValue();

	m_Data.BoundaryNL_Old.bPerformIteration = m_wndIteration.GetCheck(); 
	m_Data.BoundaryNL.nDampingMatrixUpdate = m_cmbDampUpdate.GetCurSel();

	//CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlUpdateRadio, m_Data.BoundaryNL_Old.nDampingMatrixUpdate);

	//추기
	if (m_Data.nAnalMethod == D_THIS_ANALMETHOD_STATIC)
	{
		// Static Loading Control
		m_Data.StaticCtrl.nCtrlMethod = m_nStaticIncrementRdo;
		m_Data.StaticCtrl.bCumulateHistory = (m_wndOptputOpt.GetCheck()==1) ? TRUE: FALSE;   //m_bOutputOpt;
		if (m_nStaticIncrementRdo == 1)
		{
			m_Data.StaticCtrl.nCtrlOption = m_nStaticCtrlOptRdo;

			if (m_nStaticCtrlOptRdo == 0)
			{
				m_Data.StaticCtrl.dTotalTransDisp = m_wndOptEdit1.GetEditValue();
			}
			else
			{
				int nNodeKey = 0;
				CFormulaEdit::GetEditValue(&m_wndOptEdit2, nNodeKey);
				m_Data.StaticCtrl.MasterNode = nNodeKey;
				m_Data.StaticCtrl.dTotalIncDisp = m_wndOptEdit3.GetEditValue();
				m_Data.StaticCtrl.nMasterDir = m_wndOptDirCmb.GetCurSel() + 1;
			}
		}
		else if (m_nStaticIncrementRdo == 0)
		{
			m_Data.StaticCtrl.dScaleFactor = m_wndScaleFactor.GetEditValue();
		}
	}
	m_Data.bUseInitial = m_chkUseInitial.GetCheck();
	m_Data.nTimeIntegrationParam = m_nTimeIntegrationParam;
	// Use Line Search Method  
	BOOL bChk = (m_Data.nAnalMethod == 3 && m_Data.StaticCtrl.nCtrlMethod == 1);   // static && Displacement control
	if (bChk)
	{
		m_Data.BoundaryNL_Old.bUseLineSearch = FALSE;
	}

	return TRUE;
}

BOOL CThisItemDlg_MEC::Data2Dlg()
{
	CString csVal;
	m_wndLoadcaseName.SetWindowText(m_Data.LoadCaseName);
	m_wndEndTime.SetValue(m_Data.dEndTime);
	m_wndTimeInc.SetValue(m_Data.dDelta);
	m_wndOutputStep.SetValue(m_Data.nOut);
	m_wndDesc.SetWindowText(m_Data.Description);
	csVal.Format(_T("%d"), m_Data.nIncrementStep);
	m_wndIncreStep.SetWindowText(csVal);
	
	m_nAnalTypeRdo = m_Data.nAnalType - 1;
	m_nAnalMethodRdo = m_Data.nAnalMethod - 1;
	m_nTimeHistoryTypeRdo = m_Data.nTimeHistoryType - 1;

	T_THGC_D ThgcD;
	m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD);
	if(m_Data.nInitLoadMethod==D_THIS_INITMETHOD_INITIALLOAD)
		m_nGeomNonlinearTypeRdo = ThgcD.nGeomNonlinearType; // 이 변수는 Time History Global Control에서 입력받은 값을 사용
	else
		m_nGeomNonlinearTypeRdo = m_Data.nGeomNonlinearType;
	SetCheckGeomNonlinearType(m_nGeomNonlinearTypeRdo);

	m_wndSubsequentChk.SetCheck(m_Data.nInitCondition);  
	m_nSubsequentType = m_Data.nSubsequentType;
	m_wndFinalStepLoad.SetCheck(m_Data.bFinalStepLoad);  
	m_wndFinalStepAcc.SetCheck(m_Data.bFinalStepAcc);
	m_wndCumulateDVACheck.SetCheck(m_Data.bCumulateDVA);
	if(m_Data.nInitCondition == 1 && m_Data.nSubsequentType == 0) 
	{    
		int nSelect = GetFinalStateIndex(m_Data.nSubLoadCaseType, m_Data.KeyFinalState);
		m_wndFinalStateCmb.SetCurSel(nSelect);
	}
		
	m_nDampingMethod = m_Data.nDampingType;
	int nNum = m_wndDampingMethodCmb.GetCount();
	int i = 0;
	for (i = 0; i < nNum ; i++)
		if (m_wndDampingMethodCmb.GetItemData(i) == m_nDampingMethod) break;
	ASSERT(i != nNum);
	m_wndDampingMethodCmb.SetCurSel(i);
	
//  m_nDampingTypeRdo = m_Data.nDampingType - 1;
	
	csVal.Format(_T("%g"), m_Data.dDampAll);
	m_wndDampAllMode.SetWindowText(csVal);
	m_pGrid->SetData(m_Data.aDampData);

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

  //m_nIntegrationParaRdo = m_Data.nIntegrationPara - 1;
	m_nIntegrationParam   = m_Data.nIntegrationPara;
	CDlgUtil::CobxSetCurSelItemData(m_cmbIntegrationParam, m_Data.nIntegrationPara);
	m_wndGamma.SetEditUnit(m_Data.dGamma);
	m_wndBeta.SetEditUnit(m_Data.dBeta);

	m_wndIteration.SetCheck(m_Data.BoundaryNL_Old.bPerformIteration);

	// 추가
	if (m_Data.nAnalMethod == D_THIS_ANALMETHOD_STATIC)
	{
		// Static Loading Control
		m_nStaticIncrementRdo = m_Data.StaticCtrl.nCtrlMethod;
		m_wndOptputOpt.SetCheck(m_Data.StaticCtrl.bCumulateHistory ? 1 : 0);
		if (m_nStaticIncrementRdo == 1)
		{
			m_nStaticCtrlOptRdo = m_Data.StaticCtrl.nCtrlOption;

			if (m_nStaticCtrlOptRdo == 0)
			{
				m_wndOptEdit1.SetEditUnit(m_Data.StaticCtrl.dTotalTransDisp);
			}
			else
			{
				m_wndOptEdit2.SetKeyPressedFlag(FALSE);
				m_wndOptEdit2.AddNodeKey(m_Data.StaticCtrl.MasterNode);
				m_wndOptEdit3.SetEditUnit(m_Data.StaticCtrl.dTotalIncDisp);
				m_wndOptDirCmb.SetCurSel(m_Data.StaticCtrl.nMasterDir-1);
			}
		}
		else if (m_nStaticIncrementRdo == 0)
		{
			m_wndScaleFactor.SetEditUnit(m_Data.StaticCtrl.dScaleFactor);
		}
	}
	m_chkUseInitial.SetCheck(m_Data.bUseInitial && ThgcD.aInitLoad.IsEmpty()==FALSE);
	m_nTimeIntegrationParam = m_Data.nTimeIntegrationParam;

	MakeItemInitialLoadCmb(m_nAnalTypeRdo, m_nAnalMethodRdo);
	CDlgUtil::CobxSetCurSelItemData(m_cmbInitLoadMethod, m_Data.nInitLoadMethod);
	SetInitialLoadText();  

	m_cmbDampUpdate.SetCurSel(m_Data.BoundaryNL.nDampingMatrixUpdate);

	MakeGridItemEx();

	UpdateData(FALSE);

	//CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlUpdateRadio, m_Data.BoundaryNL_Old.nDampingMatrixUpdate);
	OnChangeIteration();

	return TRUE;
}

int CThisItemDlg_MEC::GetFinalStateIndex(int nSubLoadCaseType, UINT KeyFinalState)
{
	int nSelect = 0;
	T_THIS_D DataThis;
	T_STLD_D DataStld;
	CString strFinalStateName = _T("");      
	CString strLoadCaseName;
	CString strLoadCaseType;
	int nCount = m_wndFinalStateCmb.GetCount();
	if(nSubLoadCaseType == 0)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStld(KeyFinalState, DataStld)) return -1;
		for(int i = 0; i < nCount; i++)
		{    
			m_wndFinalStateCmb.GetLBText(i, strFinalStateName);
			strLoadCaseType = strFinalStateName.Left(2);
			if(strLoadCaseType != _T("ST")) continue;
			int nStrLength = strFinalStateName.GetLength();
			strLoadCaseName = strFinalStateName.Right(nStrLength - 5);
			if(DataStld.LoadCaseName.CompareNoCase(strLoadCaseName) == 0) {nSelect = i; break;}
			if(i == nCount - 1) nSelect = -1; 
		}  
	}
	else if(nSubLoadCaseType == 1)
	{
		CString strConstructionType;
		if(KeyFinalState == 1)      strConstructionType = _LS(IDS_CMD_THIS_STAG_TYPE1);
		else if(KeyFinalState == 2) strConstructionType = _LS(IDS_CMD_THIS_STAG_TYPE2);
		else return -1;
		
		for(int i = 0; i < nCount; i++)
		{    
			m_wndFinalStateCmb.GetLBText(i, strFinalStateName);
			strLoadCaseType = strFinalStateName.Left(2);
			if(strLoadCaseType != _T("CS")) continue;
			int nStrLength = strFinalStateName.GetLength();
			strLoadCaseName = strFinalStateName.Right(nStrLength - 5);
			if(strConstructionType.CompareNoCase(strLoadCaseName) == 0) {nSelect = i; break;}
			if(i == nCount - 1) nSelect = -1; 
		}  
	}
	else if(nSubLoadCaseType == 2)
	{
		if(!m_pDoc->m_pAttrCtrl->GetThis(KeyFinalState, DataThis)) return -1;
		for(int i = 0; i < nCount; i++)
		{    
			m_wndFinalStateCmb.GetLBText(i, strFinalStateName);
			strLoadCaseType = strFinalStateName.Left(2);
			if(strLoadCaseType != _T("TH")) continue;
			int nStrLength = strFinalStateName.GetLength();
			strLoadCaseName = strFinalStateName.Right(nStrLength - 5);
			if(DataThis.LoadCaseName.CompareNoCase(strLoadCaseName) == 0) {nSelect = i; break;}
			if(i == nCount - 1) nSelect = -1; 
		}
	}
	else ASSERT(FALSE);
	
	return nSelect; 
}
/*
int CThisItemDlg_MEC::GetFinalStateIndex(T_THIS_K KeyThis)
{
	int nSelect = 0;
	T_THIS_D DataThis;
	int nCount = m_wndFinalStateCmb.GetCount();
	if(!m_pDoc->m_pAttrCtrl->GetThis(KeyThis, DataThis)) return -1;
	CString strFinalStateName = _T("");    
	for(int i = 0; i < nCount; i++)
	{    
		m_wndFinalStateCmb.GetLBText(i, strFinalStateName);
		if(DataThis.LoadCaseName.CompareNoCase(strFinalStateName) == 0) {nSelect = i; break;}
		if(i == nCount - 1) nSelect = -1; 
	}  
	return nSelect; 
}
*/
//CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Mode), _LS(IDS_WG_CMD__ADDD__Damping_Ratio)};
/////////////////////////////////////////////////////////////////////////////
// CThisItemDlg_MEC message handlers

BOOL CThisItemDlg_MEC::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CCMDlgBase::OnInitDialog();

	if(m_bModify) m_wndBtnApply.EnableWindow(FALSE);

	AlignControl();  

	m_wndInitLoadGroupBox.InitControl(this, CThisItemDlg_MEC::IDD, IDC_CMD_INITIAL_CONDITION_FRM, TRUE, TRUE);

	m_bUseValidNllp = CheckValidNllp();
	//m_wndEndTime.SetUnitType(CUnitCtrl::m_THIS_UNIT.dEndTime);
	m_wndEndTimeUnit.SetUnitType(CUnitCtrl::m_THIS_UNIT.dEndTime);
	//m_wndTimeInc.SetUnitType(CUnitCtrl::m_THIS_UNIT.dDelta);
	m_wndTimeIncUnit.SetUnitType(CUnitCtrl::m_THIS_UNIT.dDelta);
	m_wndTransUnit.SetUnitType(CUnitCtrl::m_THIS_UNIT.dTotalTransDisp);
	m_wndIncreDispUnit.SetUnitType(CUnitCtrl::m_THIS_UNIT.dTotalIncDisp);

	m_wndEndTime.SetRange(0, SHRT_MAX);
	m_wndTimeInc.SetRange(0, SHRT_MAX);
	m_wndOutputStep.SetRange(1, SHRT_MAX);
	m_wndOutputStep.SetInteger(TRUE);

	m_wndOptDirCmb.AddString(_LS(IDS_CMD_GLOBAL_DX));
	m_wndOptDirCmb.AddString(_LS(IDS_CMD_GLOBAL_DY));
	m_wndOptDirCmb.AddString(_LS(IDS_CMD_GLOBAL_DZ));
	m_wndOptDirCmb.SetCurSel(0);
	
	// 초기값 강제설정 : Data2Dlg는 해당 옵션인 경우만 값을 설정하므로 이 경우가 아닌 경우
	// 초기값이 설정되지 않는다. 따라서, Data2Dlg 이전에 옵션에 관계없이 초기값이 필요한 경우
	// 이를 설정한다.
	m_wndScaleFactor.SetEditUnit(1.0);

	m_bInitCmb = TRUE;

	m_wndDampingMethodCmb.ResetContent();
	CDlgUtil::CobxAddItem(m_wndDampingMethodCmb, _LS(IDS_CMD_SPLC_Direct_Modal), 1);
	CDlgUtil::CobxAddItem(m_wndDampingMethodCmb, _LS(IDS_CMD_SPLC_Mass_and_Stiff_Proportional), 2);
	CDlgUtil::CobxAddItem(m_wndDampingMethodCmb, _LS(IDS_CMD_THIS_GROUP1), 3);
	CDlgUtil::CobxAddItem(m_wndDampingMethodCmb, _LS(IDS_CMD_THIS_GROUP2), 4);    
	m_wndDampingMethodCmb.SetCurSel(0);
	m_nDampingMethod = CDlgUtil::CobxGetCurSelItemData(m_wndDampingMethodCmb, m_wndDampingMethodCmb.GetCurSel());  

	m_cmbDampUpdate.ResetContent();
	m_cmbDampUpdate.AddString(_LS(IDS_CMD_DAMP_UPDATE_NO_USE_LINEAR_STIFF));
	m_cmbDampUpdate.AddString(_LS(IDS_CMD_DAMP_UPDATE_NO_USE_INITIAL_STIFF));
	m_cmbDampUpdate.AddString(_LS(IDS_CMD_DAMP_UPDATE_YES_USE_CURRENT_STIFF));
	m_cmbDampUpdate.SetCurSel(0);
	//AdjustComboListBox(m_cmbDampUpdate);

	m_cmbIntegrationParam.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbIntegrationParam, _LS(IDS_CMD_DAMP_NEWMARK_CONST),  D_THIS_NEWMARK_CONST);
	CDlgUtil::CobxAddItem(m_cmbIntegrationParam, _LS(IDS_CMD_DAMP_NEWMARK_LINEAR), D_THIS_NEWMARK_LINEAR);
	CDlgUtil::CobxAddItem(m_cmbIntegrationParam, _LS(IDS_CMD_DAMP_NEWMARK_USER),   D_THIS_NEWMARK_USER);
	m_cmbIntegrationParam.SetCurSel(0);
	m_nIntegrationParam = CDlgUtil::CobxGetCurSelItemData(m_cmbIntegrationParam, m_cmbIntegrationParam.GetCurSel());

	m_pGrid->SubclassDlgItem(IDC_CMD_GRID, this);
	m_pGrid->Initialize(m_pDoc);
	m_pGrid->SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	SetFinalStateCmb();
	SetGridDeadLoadCmb();
	Data2Dlg();  
	CtrlManager();

	m_wndOptEdit2.SetAttNodeList();
	m_wndOptEdit2.SetLButtonDownNotifyWindow(this);
	m_wndOptEdit2.SetEnterNotifyWindow(this);
	m_wndOptEdit2.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndOptEdit2.SetMaxNodeKeyNum(1);	

	SetGridListCtrlHeader();
	MakeGridItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThisItemDlg_MEC::SetGridListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Load_Case) };
	int nColWidth[] = { 200 };
	int nColNum = 1;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_GridList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_GridList.GetSafeHwnd(), dwStyle);

	// Set Title
	for (i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_GridList.InsertColumn(i, &lvcolumn);
		m_GridList.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CThisItemDlg_MEC::MakeGridItemEx()
{
	m_GridList.DeleteAllItems();

	int nItemCount = m_Data.aGilcCase.GetSize();
	if (nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_GridList.InsertItem(nCount, _T(""));
		SetGridItem(nCount, m_Data.aGilcCase[nCount]);
	}
	m_GridList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CThisItemDlg_MEC::SetGridItem(int nIndex, T_GILC_K key)
{
	T_GILC_D GilcD;
	m_pDoc->m_pAttrCtrl->GetGilcFromBaseDB(key, GilcD);

	LVITEM lvitem;
	CString str;
	lvitem.iItem = nIndex;
	lvitem.iSubItem = 0;
	str = GilcD.LoadCaseName;
	lvitem.pszText = str.GetBuffer(0);
	lvitem.mask = LVIF_TEXT;

	m_GridList.SetItem(&lvitem);
	m_GridList.SetItemData(nIndex, key);
	str.ReleaseBuffer();
}

void CThisItemDlg_MEC::AlignControl()
{
	// Ctrl을 Align하기 전에 같이 다룰 Ctrl을 모은다. 
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlGeoNonlinear, IDC_CMD_GEOM_NONLINEAR_TYPE_GRB, TRUE);

	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDirect, IDC_CMD_DAMPING_DIRECT_FRM, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlMass, IDC_CMD_DAMPING_MASS_FRM, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlAttribute, IDC_CMD_DAMPING_MASS_ATTRIBUTE_FRM, FALSE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlMassStiff, IDC_CMD_DAMPING_MASS_ATTRIBUTE_FRM, FALSE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDamping, IDC_CMD_MODAL_DAMPING_TYPE_FRM, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlStatic, IDC_CMD_STATIC_GROUP_FRM, TRUE);

	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlTimeParam, IDC_CMD_INTEGRATION_PARA_FRM, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlNonAnalParam, IDC_CMD_NONLINEAR_ANAL_CONTROL_FRM, TRUE);
	
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_DEFINE_RDO);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_DEFINE_RDO2);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_DIRECT_RM_EDIT);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_DIRECT_RK_EDIT);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_CALC_RM_EDIT);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_CALC_RK_EDIT);
	m_aCtrlMassStiff.Add(IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN);  

	CRect rRef;
	CRect rToMove;
	int nDistY, nDistX;
	//////////////////////////////////////////////////////////////////////////
	CArray<UINT, UINT> arCrtls01; 
	if (!m_bJapanMode)
	{
		arCrtls01.RemoveAll();
		arCrtls01.Add(IDC_CMD_CUMULATE_DVA_CHK);
		arCrtls01.Add(IDC_CMD_FINAL_STEP_LOAD_CHK);
		arCrtls01.Add(IDC_CMD_FINAL_STEP_ACC_CHK);
		GetDlgItem(IDC_CMD_SUBSEQUENT_TYPE_RDO4)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_CUMULATE_DVA_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, arCrtls01, 0/*nDistX*/, nDistY);

		// Resize INITIAL_CONDITION_FRM
		CRect rectResize;
		GetDlgItem(IDC_CMD_INITIAL_CONDITION_FRM)->GetWindowRect(rectResize);
		GetDlgItem(IDC_CMD_FINAL_STEP_LOAD_CHK)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + 8;
		ScreenToClient(rectResize);
		GetDlgItem(IDC_CMD_INITIAL_CONDITION_FRM)->MoveWindow(rectResize);

		// Move - Geometric Nonlinearity Type
		GetDlgItem(IDC_CMD_INITIAL_CONDITION_FRM  )->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_GEOM_NONLINEAR_TYPE_GRB)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + 10;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlGeoNonlinear, 0/*nDistX*/, nDistY);

		// Move - Damping
		GetDlgItem(IDC_CMD_GEOM_NONLINEAR_TYPE_GRB)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MODAL_DAMPING_TYPE_FRM )->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + 10;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlDamping, 0/*nDistX*/, nDistY);

		// Move - Time Integration Parameters
		GetDlgItem(IDC_CMD_MODAL_DAMPING_TYPE_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_INTEGRATION_PARA_FRM  )->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + 10;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlTimeParam, 0/*nDistX*/, nDistY);

		// Move - Nonlinear Analysis Control Parameters
		GetDlgItem(IDC_CMD_INTEGRATION_PARA_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_NONLINEAR_ANAL_CONTROL_FRM)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + 10;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlNonAnalParam, 0/*nDistX*/, nDistY);

		arCrtls01.RemoveAll();
		arCrtls01.Add(IDOK);
		arCrtls01.Add(IDCANCEL);
		arCrtls01.Add(IDC_CMD_APPLY);
		GetDlgItem(IDC_CMD_NONLINEAR_ANAL_CONTROL_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + 12;
		CDlgUtil::CtrlMoveDistXY(this, arCrtls01, 0/*nDistX*/, nDistY);
	}

	//////////////////////////////////////////////////////////////////////////

	int i;
	UINT aFirstCtrlID[] = {IDC_CMD_DAMPING_DIRECT_FRM};
	
	GetDlgItem(IDC_CMD_DAMPING_MASS_FRM)->GetWindowRect(rRef);
	for (i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXYByRect(this, IDC_CMD_DAMPING_DIRECT_FRM, 0, nDistY, TRUE, TRUE);
	}

	// 추가 
	GetDlgItem(IDC_CMD_MODAL_DAMPING_TYPE_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_STATIC_GROUP_FRM)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXYByRect(this, IDC_CMD_STATIC_GROUP_FRM, 0, nDistY, TRUE, TRUE);
	
	CArray<UINT,UINT> arIncreStep;
	arIncreStep.Add(IDC_CMD_INCRESTEP_STATIC);
	arIncreStep.Add(IDC_CMD_INCRESTEP_EDT);
	GetDlgItem(IDC_CMD_ENDTIME_STATIC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_INCRESTEP_STATIC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistXY(this, arIncreStep, nDistX, nDistY);

	// Control Option 추가사항
	GetDlgItem(IDC_CMD_GLOBAL_TEXT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LOAD_CONT_STATIC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlOption2, nDistX, nDistY);

	// Initial Load
	GetDlgItem(IDC_CMD_SUBSEQUENT_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_USE_INITIAL_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlInitLoad_Initial, nDistX, nDistY);
	
	// P-delta
// #ifdef _CIVIL
// 	CArray<UINT, UINT> arCrtls;
// 	arCrtls.Add(IDC_CMD_GEOM_NONLINEAR_TYPE2_RDO);
// 	GetDlgItem(IDC_CMD_FINAL_STEP_LOAD_CHK)->GetWindowRect(rRef);
// 	GetDlgItem(IDC_CMD_GEOM_NONLINEAR_TYPE2_RDO)->GetWindowRect(rToMove);
// 	nDistY = 0;
// 	nDistX = rRef.left - rToMove.left;
// 	CDlgUtil::CtrlMoveDistXY(this, arCrtls, nDistX, nDistY);
// #endif // _CIVIL

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(12);
	MoveWindow(r);
}

void CThisItemDlg_MEC::CtrlManager()
{
	UpdateData(TRUE);

	GetCheckGeomNonlinearType(m_nGeomNonlinearTypeRdo);

	CArray<UINT,UINT> arCtrl;
	CArray<UINT,UINT> aStaticOut;

	BOOL bMass  = m_wndMassProp.GetCheck();
	BOOL bStiff = m_wndStiffProp.GetCheck();
	BOOL bSubsequent = m_wndSubsequentChk.GetCheck();

	int nInitLoadMethod = CDlgUtil::CobxGetCurSelItemData(m_cmbInitLoadMethod, m_cmbInitLoadMethod.GetCurSel());

	BOOL bEnableGeomNonlinear = FALSE;

	if (m_nAnalTypeRdo == 1)
	{
		if (m_nAnalMethodRdo == 0)
		{
			m_nGeomNonlinearTypeRdo = 0;
		}
		else
		{
			if (nInitLoadMethod == D_THIS_INITMETHOD_INITIALLOAD)
			{
				T_THGC_D ThgcD;
				m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD);
				m_nGeomNonlinearTypeRdo = ThgcD.nGeomNonlinearType; // 이 변수는 Time History Global Control에서 입력받은 값을 사용
			}
			else
			{
				if (bSubsequent && m_nSubsequentType == D_THIS_LOAD_CASE)
				{
					CString strFinalStateName;
					m_wndFinalStateCmb.GetWindowText(strFinalStateName);
					CString strLoadCaseType = strFinalStateName.Left(2);
					int nStrLength = strFinalStateName.GetLength();
					CString strLoadCaseName = strFinalStateName.Right(nStrLength - 5);

					if (strLoadCaseType == _T("TH"))
					{
						T_THIS_K PreThisK = m_pDoc->m_pAttrCtrl->GetThisKey(strLoadCaseName);
						T_THIS_D PreThisD;

						if (!m_pDoc->m_pAttrCtrl->GetThis(PreThisK, PreThisD)) { ASSERT(0); return; }

						m_nGeomNonlinearTypeRdo = PreThisD.nGeomNonlinearType;
					}
					else
					{
						bEnableGeomNonlinear = TRUE;
					}
				}
				else
				{
					bEnableGeomNonlinear = TRUE;
				}
			}
		}
	}

	SetCheckGeomNonlinearType(m_nGeomNonlinearTypeRdo);

	if(m_nGeomNonlinearTypeRdo == D_THIS_GEOMNONLIN_LARGE_DISP || m_nGeomNonlinearTypeRdo == D_THIS_GEOMNONLIN_P_DELTA)
	{
		if(m_nAnalTypeRdo == 0) m_nGeomNonlinearTypeRdo = D_THIS_GEOMNONLIN_NONE;
		else
		{
			if(m_nAnalMethodRdo == 0) m_nGeomNonlinearTypeRdo = D_THIS_GEOMNONLIN_NONE;
		}
	}

	int nGeomNonlinearType = D_THIS_GEOMNONLIN_NONE;
	if(bSubsequent && m_nAnalTypeRdo == 1 && m_nAnalMethodRdo != 0)
	{
		if(m_nGeomNonlinearTypeRdo == D_THIS_GEOMNONLIN_LARGE_DISP) nGeomNonlinearType = D_THIS_GEOMNONLIN_LARGE_DISP;
		if(m_nGeomNonlinearTypeRdo == D_THIS_GEOMNONLIN_P_DELTA   ) nGeomNonlinearType = D_THIS_GEOMNONLIN_P_DELTA;
	}
	
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SUBSEQUENT_TYPE_RDO, bSubsequent);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SUBSEQUENT_TYPE_RDO2, bSubsequent && nGeomNonlinearType == D_THIS_GEOMNONLIN_NONE);
//#ifdef _MGEN
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SUBSEQUENT_TYPE_RDO3, bSubsequent && (nGeomNonlinearType == D_THIS_GEOMNONLIN_LARGE_DISP /*|| nGeomNonlinearType == D_THIS_GEOMNONLIN_P_DELTA*/));
//#endif
	if (m_bJapanMode)
	{
#ifdef _CIVIL
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SUBSEQUENT_TYPE_RDO4, bSubsequent);
		CDlgUtil::CtrlEnableDisable(this, m_aGridModelList, m_nSubsequentType==3);  // IDC_CMD_SUBSEQUENT_TYPE_RDO4 포함됨
#else
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SUBSEQUENT_TYPE_RDO4, false);
		CDlgUtil::CtrlEnableDisable(this, m_aGridModelList, false);
#endif // _CIVIL
	}
	if(!bSubsequent || m_nSubsequentType != 0) 
	{
		m_wndFinalStateCmb.SetCurSel(-1);
		//m_wndFinalStepLoad.SetCheck(FALSE);
	}

	BOOL bNonLinearAnal = (m_nGeomNonlinearTypeRdo == D_THIS_GEOMNONLIN_LARGE_DISP || m_nGeomNonlinearTypeRdo == D_THIS_GEOMNONLIN_P_DELTA);

	if (m_nAnalTypeRdo == 0 && m_nAnalMethodRdo == 2)
		m_nAnalMethodRdo = 0;

	if(m_nSubsequentType == 1)
	{
		if(m_nAnalTypeRdo == 1 && m_nAnalMethodRdo != 0 && (m_nGeomNonlinearTypeRdo == D_THIS_GEOMNONLIN_LARGE_DISP || m_nGeomNonlinearTypeRdo == D_THIS_GEOMNONLIN_P_DELTA) ) m_nSubsequentType = 0;
	}
	else if(m_nSubsequentType == 2)
	{
		if(m_nAnalTypeRdo == 0) m_nSubsequentType = 0;
		else
		{
			if(m_nAnalMethodRdo == 0) m_nSubsequentType = 0;
			else if(m_nGeomNonlinearTypeRdo != D_THIS_GEOMNONLIN_LARGE_DISP) m_nSubsequentType = 0;
		}
	}

	int nSubLoadCaseType = 0;
	if(bSubsequent && m_nSubsequentType == 0)
	{
		CString strFinalStateName;
		m_wndFinalStateCmb.GetWindowText(strFinalStateName);
		CString strLoadCaseType = strFinalStateName.Left(2);    
		if(strLoadCaseType == _T("TH")) nSubLoadCaseType = 2;    
	}

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_GEOM_NONLINEAR_TYPE_GRB, bEnableGeomNonlinear, FALSE);

	T_THGC_D ThgcD;
	m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD);
	if(nInitLoadMethod==D_THIS_INITMETHOD_ORDER_IN_SEQLOAD)
	{
		//CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_GEOM_NONLINEAR_TYPE_GRB, m_nAnalTypeRdo == 1 && m_nAnalMethodRdo != 0, FALSE);

		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_FINAL_STATE_COMBO,   bSubsequent && m_nSubsequentType == 0);
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_CUMULATE_DVA_CHK,    bSubsequent && m_nSubsequentType == 0 && nSubLoadCaseType == 2);
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_FINAL_STEP_LOAD_CHK, bSubsequent && m_nSubsequentType == 0 && nSubLoadCaseType == 2);
	}
	else
	{
		BOOL bPerforNonlinInitialLoad = m_chkUseInitial.GetCheck() && ThgcD.nInitLoadMethod==0;
		//CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_GEOM_NONLINEAR_TYPE_GRB, FALSE, FALSE);

		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_CUMULATE_DVA_CHK,    bPerforNonlinInitialLoad);
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_FINAL_STEP_LOAD_CHK, bPerforNonlinInitialLoad);
		if(bPerforNonlinInitialLoad) m_wndFinalStepLoad.SetCheck(TRUE); // Use Initial Load일 때는 항상 check on
	}

 	if ( (m_nAnalTypeRdo == 0 && m_nAnalMethodRdo == 1) ||
		 (m_nAnalTypeRdo == 1 && m_nAnalMethodRdo == 1) ||
		 (m_nAnalTypeRdo == 1 && m_nAnalMethodRdo == 2))
	{
		if (nInitLoadMethod == D_THIS_INITMETHOD_ORDER_IN_SEQLOAD && bSubsequent && m_nSubsequentType == 0 && nSubLoadCaseType == 2)
		{
			GetDlgItem(IDC_CMD_FINAL_STEP_ACC_CHK)->EnableWindow(TRUE);
		}
		else
		{
			m_wndFinalStepAcc.SetCheck(FALSE);
			GetDlgItem(IDC_CMD_FINAL_STEP_ACC_CHK)->EnableWindow(FALSE);
		}
	}
	else
	{
		m_wndFinalStepAcc.SetCheck(FALSE);
		GetDlgItem(IDC_CMD_FINAL_STEP_ACC_CHK)->EnableWindow(FALSE);
	}

// #ifdef _CIVIL
// 	GetDlgItem(IDC_CMD_GEOM_NONLINEAR_TYPE3_RDO)->ShowWindow(SW_HIDE);
// #endif // _CIVIL

	if(m_wndCumulateDVACheck.IsWindowEnabled()==FALSE) m_wndCumulateDVACheck.SetCheck(FALSE);
	if(m_wndFinalStepLoad.IsWindowEnabled()==FALSE) m_wndFinalStepLoad.SetCheck(FALSE);

	// JP + (nInitLoadMethod==D_THIS_INITMETHOD_ORDER_IN_SEQLOAD) 일 때
	BOOL bShowJP = (m_bJapanMode && nInitLoadMethod == D_THIS_INITMETHOD_ORDER_IN_SEQLOAD)? TRUE : FALSE;

	// Dealing with JP Fold Group Box logic
	if (!m_wndInitLoadGroupBox.GetFoldState())
	{
		SetInitLoadFoldControl(bShowJP, nInitLoadMethod);
	}

	if((m_nAnalTypeRdo == 1 || m_nAnalMethodRdo == 1) && m_nTimeHistoryTypeRdo == 1) m_nTimeHistoryTypeRdo = 0;
//  if(m_nAnalMethodRdo == 1 && m_nDampingTypeRdo == 0) m_nDampingTypeRdo = 1;

	arCtrl.RemoveAll();
	arCtrl.Add(IDC_CMD_TIME_INC_STATIC);
	arCtrl.Add(IDC_CMD_TIME_INC_EDIT);
	//arCtrl.Add(IDC_CMD_TIME_INC_SPIN);
	arCtrl.Add(IDC_CMD_TIME_INC_UNIT);
	CDlgUtil::CtrlShowHide(this, arCtrl, !(m_nAnalMethodRdo==2 && m_nAnalTypeRdo==1));

	// iGen 에서는 락 옵션으로 함
#if defined(_IGEN_ONLY)
	BOOL bEnableTHS = FALSE;
	if(m_nAnalTypeRdo == 1 && m_nAnalMethodRdo != 0 && m_pDoc->AllowCommand(D_OPTN_ID_GEOM_THS))  bEnableTHS = TRUE;
	GetDlgItem(IDC_CMD_GEOM_NONLINEAR_TYPE2_RDO)->EnableWindow(bEnableTHS);
	GetDlgItem(IDC_CMD_GEOM_NONLINEAR_TYPE3_RDO)->EnableWindow(bEnableTHS);
#endif

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_INTEGRATION_PARA_FRM, m_nAnalMethodRdo == 1, FALSE); 
	CDlgUtil::CtrlEnableDisableByRect(this,IDC_CMD_NONLINEAR_ELEMENT_FRM, m_nAnalTypeRdo == 1 && m_nAnalMethodRdo == 1, FALSE);  
	
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlNEWMARK, m_nAnalMethodRdo == 1 && m_nTimeIntegrationParam == 1);

	GetDlgItem(IDC_CMD_ANAL_METHOD_RDO3)->EnableWindow(m_nAnalTypeRdo==1);
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlStatic, m_nAnalMethodRdo == 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDamping, m_nAnalMethodRdo != 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDirect, m_nDampingMethod == 1 && m_nAnalMethodRdo != 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMass,   m_nDampingMethod == 2 && m_nAnalMethodRdo != 2);

	CDlgUtil::CtrlShowHide(this, m_aCtrlOption, m_nAnalMethodRdo == 2 && m_nStaticIncrementRdo == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlOption2, m_nAnalMethodRdo == 2 && m_nStaticIncrementRdo == 0);

	// load factor history output > rsum history output 로 변경하였고, 솔버에 관련 옵션이 없어 일단 막음.
	// 솔버에 관련 옵션 반영시 필요에 따라 다시 살릴 것
	// m_aCtrlStatic 의 하위이므로 m_aCtrlStatic 이후에 동작해야 함.
	CDlgUtil::GetCtrlIDByIncRect(this,aStaticOut,IDC_CMD_OUTPUT_FRM,TRUE);
	CDlgUtil::CtrlShowHide(this,aStaticOut,FALSE);
	
	GetDlgItem(IDC_CMD_DAMPING_MASS_ATTRIBUTE_FRM)->EnableWindow(m_nDampingMethod == 2 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_DEFINE_RDO)->EnableWindow(m_nDampingMethod == 2 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_DEFINE_RDO2)->EnableWindow(m_nDampingMethod == 2 && (bMass || bStiff));  
	GetDlgItem(IDC_CMD_DAMPING_MASS_DIRECT_RM_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 0 && bMass);
	GetDlgItem(IDC_CMD_DAMPING_MASS_DIRECT_RK_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 0 && bStiff);
	GetDlgItem(IDC_CMD_DAMPING_MASS_CALC_RM_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 1 && bMass);
	GetDlgItem(IDC_CMD_DAMPING_MASS_CALC_RK_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 1 && bStiff);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlAttribute, m_nCoefDefineRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_F1_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 0 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_T1_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_X1_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_F2_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 0 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_T2_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_X2_EDIT)->EnableWindow(m_nDampingMethod == 2 && m_nCoefDefineRdo == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN)->EnableWindow(m_nDampingMethod == 2 && (bMass || bStiff));

	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_INTEGRATION_PARA_GAMMA_EDIT, m_nAnalMethodRdo == 1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_INTEGRATION_PARA_BETA_EDIT,  m_nAnalMethodRdo == 1);
	CEdit* pEditGamma = (CEdit*)GetDlgItem(IDC_CMD_INTEGRATION_PARA_GAMMA_EDIT);
	CEdit* pEditBeta  = (CEdit*)GetDlgItem(IDC_CMD_INTEGRATION_PARA_BETA_EDIT);
	if (m_nAnalMethodRdo == 1 && m_nTimeIntegrationParam == 1)
	{
		pEditGamma->SetReadOnly(m_nIntegrationParam != D_THIS_NEWMARK_USER);
		pEditBeta->SetReadOnly(m_nIntegrationParam != D_THIS_NEWMARK_USER);
	}
	else
	{
		pEditGamma->SetReadOnly(TRUE);
		pEditBeta->SetReadOnly(TRUE);
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrlUpdate, m_nAnalMethodRdo == 1 && (m_nDampingMethod == 2 || m_nDampingMethod == 4));
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlUpdate, m_nAnalTypeRdo == 1);

	//추가
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_CTRL_OPT_EDIT1, m_nStaticIncrementRdo==1 && m_nStaticCtrlOptRdo==0);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_CTRL_OPT_EDIT2, m_nStaticIncrementRdo==1 && m_nStaticCtrlOptRdo==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_CTRL_OPT_EDIT3, m_nStaticIncrementRdo==1 && m_nStaticCtrlOptRdo==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_CTRL_OPT_DIR_CMB, m_nStaticIncrementRdo==1 && m_nStaticCtrlOptRdo==1);

	int nIteration = m_wndIteration.GetCheck();
	if(m_nAnalMethodRdo == 0)    // Modal
	{
		nIteration = 1;
		m_wndIteration.SetCheck(nIteration);
	}
	BOOL bEnable = (m_nAnalMethodRdo != 0  && m_nAnalTypeRdo == 1);
	m_wndIteration.EnableWindow(bEnable);
	bEnable = (nIteration==1 && m_nAnalTypeRdo==1);
	GetDlgItem(IDC_CMD_ITERATION_BTN)->EnableWindow(bEnable);

	GetDlgItem(IDC_CMD_TIME_HISTORY_TYPE_RDO)->EnableWindow(!(m_nAnalTypeRdo==1 && m_nAnalMethodRdo==2));
	GetDlgItem(IDC_CMD_TIME_HISTORY_TYPE_RDO2)->EnableWindow(m_nAnalTypeRdo==0 && m_nAnalMethodRdo==0);

	arCtrl.RemoveAll();
	arCtrl.Add(IDC_CMD_ENDTIME_STATIC);
	arCtrl.Add(IDC_CMD_END_TIME_EDIT);
	//arCtrl.Add(IDC_CMD_END_TIME_SPIN);
	arCtrl.Add(IDC_CMD_END_TIME_UNIT);
	CDlgUtil::CtrlShowHide(this, arCtrl, m_nAnalMethodRdo!=2);

	arCtrl.RemoveAll();
	arCtrl.Add(IDC_CMD_INCRESTEP_STATIC);
	arCtrl.Add(IDC_CMD_INCRESTEP_EDT);
	CDlgUtil::CtrlShowHide(this, arCtrl, m_nAnalMethodRdo==2);
	
	m_wndDampingMethodCmb.ResetContent(); 
	CDlgUtil::CobxAddItem(m_wndDampingMethodCmb, _LS(IDS_CMD_SPLC_Direct_Modal), 1);
	CDlgUtil::CobxAddItem(m_wndDampingMethodCmb, _LS(IDS_CMD_SPLC_Mass_and_Stiff_Proportional), 2);
	CDlgUtil::CobxAddItem(m_wndDampingMethodCmb, _LS(IDS_CMD_THIS_GROUP1), 3);  
	if(m_nAnalMethodRdo == 1)  // Direct Integration
		CDlgUtil::CobxAddItem(m_wndDampingMethodCmb, _LS(IDS_CMD_THIS_GROUP2), 4);    
	if(m_nAnalMethodRdo == 0 && m_nDampingMethod == 4)   // Modal Damping 이고 Element Mass & Stiffness Proportional인 경우
	{    
		m_nDampingMethod = 3;
	}  
	int nNum = m_wndDampingMethodCmb.GetCount();
	int i = 0;
	for (i = 0; i < nNum ; i++)
		if (m_wndDampingMethodCmb.GetItemData(i) == m_nDampingMethod) break;
	ASSERT(i != nNum);
	m_wndDampingMethodCmb.SetCurSel(i);	

	// Damping Matrix Update “Yes”& General Link에 (Force Type or Seismic Control Device) 가 설정된 경우만 활성화
	int nDmpMtrxUpdate=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlUpdateRadio, nDmpMtrxUpdate);
	GetDlgItem(IDC_CMD_THIS_ITEM_DESC_BTN)->EnableWindow(nDmpMtrxUpdate==1 && m_bUseValidNllp && m_nAnalTypeRdo==1);
	GetDlgItem(IDC_USE_INITIAL_CHK)->EnableWindow(!ThgcD.aInitLoad.IsEmpty());

	UpdateData(FALSE);

	RedrawWindow();
}

void CThisItemDlg_MEC::SetInitLoadFoldControl(const bool isShowJP, const int initMethod)
{
	GetDlgItem(IDC_CMD_SUBSEQUENT_TYPE_RDO4)->ShowWindow(isShowJP);
	CDlgUtil::CtrlShowHide(this, m_aCtrlInitLoad_Initial, initMethod == D_THIS_INITMETHOD_INITIALLOAD);
	CDlgUtil::CtrlShowHide(this, m_aCtrlInitLoad_Sequence, initMethod == D_THIS_INITMETHOD_ORDER_IN_SEQLOAD);
	CDlgUtil::CtrlShowHide(this, m_aGridModelList, isShowJP);
}

void CThisItemDlg_MEC::OnStaticIncMethod()
{
	CtrlManager();
}

void CThisItemDlg_MEC::OnStaticCtrlOpt()
{
	CtrlManager();
}

void CThisItemDlg_MEC::OnCallAutoCalcCoef()
{
	AutoCalcCoef();
}

void CThisItemDlg_MEC::OnChangeAnalType() 
{
	UpdateData(TRUE);
	MakeItemInitialLoadCmb(m_nAnalTypeRdo, m_nAnalMethodRdo);
	CtrlManager();
	SetFinalStateCmb();

	// Damping Matrix 초기값 : Linear면 True, Nonlinear면 False
	int nDampingMatrix = m_nAnalTypeRdo==0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlUpdateRadio, nDampingMatrix);
}

void CThisItemDlg_MEC::OnChangeAnalMethod()
{
	// 일본버전 초기 모드 일때 
	//   if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP)
	//   {
	//     UpdateData(TRUE);
	//     m_nDampingMethod = m_nAnalMethodRdo == 1 ? 2 : 3;    
	//     UpdateData(FALSE);
	//   }

	// MNET:2962 20070914 mylee - 위 코드 주석처리 후 추가
	UpdateData(TRUE);
	if(m_nAnalMethodRdo == 0) m_nDampingMethod = 1;
	else if(m_nAnalMethodRdo == 1) m_nDampingMethod = 2;
	UpdateData(FALSE);
	
	MakeItemInitialLoadCmb(m_nAnalTypeRdo, m_nAnalMethodRdo);
	SetFinalStateCmb();
	CtrlManager();	
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CThisItemDlg_MEC::OnChangeTimeHistoryType()
{
	CtrlManager();
}

void CThisItemDlg_MEC::OnChangeGeomNonlinearType()
{
	CtrlManager();
	SetFinalStateCmb();
}

void CThisItemDlg_MEC::OnChangeSequenceLoad()
{
	CtrlManager();
}

void CThisItemDlg_MEC::OnChangeCoefDefine()
{
	CtrlManager();  
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CThisItemDlg_MEC::OnChangeFreqPeriod()
{
	CtrlManager();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CThisItemDlg_MEC::OnMassStiffChk()
{
	CtrlManager();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CThisItemDlg_MEC::OnChangeIntegrationPara()
{
	CtrlManager();
	if(m_nIntegrationParam == D_THIS_NEWMARK_CONST)
	{
		m_wndGamma.SetEditUnit(0.5);
		m_wndBeta.SetEditUnit(0.25);
	}
	else if(m_nIntegrationParam == D_THIS_NEWMARK_LINEAR)
	{
		m_wndGamma.SetEditUnit(0.5);
		m_wndBeta.SetEditUnit(1./6.);
	}
}

void CThisItemDlg_MEC::OnDeltaposDouble(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	CFormulaEditSpin* pEdit;
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

void CThisItemDlg_MEC::OnDeltaposInteger(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	CFormulaEditSpin* pEdit;
	if (nID == IDC_CMD_OUTPUT_STEP_SPIN) pEdit = &m_wndOutputStep;
	else 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Unknowon_spin_button));
		return;
	}
	CString csVal;
	int      iVal = pEdit->GetEditValue();
	iVal -= pNMUpDown->iDelta;
	if (iVal < 1) iVal = 1;
	csVal.Format(_T("%d"), iVal);
	pEdit->SetWindowText(csVal);
	
	*pResult = 0;
}

BOOL CThisItemDlg_MEC::ApplyOrOK() 
{
	// TODO: Add extra validation here
	if(!Dlg2Data()) return FALSE;
	if(!ValidData()) return FALSE;

	BOOL bSuccess;
	if(m_bModify)
	{
		m_Data.LoadCaseId = m_nOldID;
		bSuccess = m_pDoc->m_pDataCtrl->ModifyThis(m_csOldName, m_Data);
	}
	else bSuccess = m_pDoc->m_pDataCtrl->AddThis(m_Data);

	if (!bSuccess) return FALSE;
	
	return TRUE;
}

void CThisItemDlg_MEC::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;

	CCMDlgBase::DestroyWindow();
}

void CThisItemDlg_MEC::OnCancel()
{
	CCMDlgBase::DestroyWindow();
}

void CThisItemDlg_MEC::OnCmdApply() 
{	
	if (!ApplyOrOK()) return;

	SetFinalStateCmb();
	m_bModify = FALSE;
//  m_OldKey = m_Data;
}

void CThisItemDlg_MEC::SetFinalStateCmb()
{
	UpdateData();

	m_wndFinalStateCmb.ResetContent();
	CString strFinalStateName;

	BOOL bEnableNoneTH = TRUE;
	BOOL bEnableLargeDispTH = TRUE;
	BOOL bEnableSTandCS = TRUE;

	int nAnalType = 0, nAnalMethod = 0;
	int nGeomNonlinearType = 0;
	T_THGC_D ThgcD;
	m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD);
	
	if(m_bInitCmb)
	{
		nAnalType = m_Data.nAnalType;
		nAnalMethod = m_Data.nAnalMethod;

		if(m_bModify)
		{
			nGeomNonlinearType = m_Data.nGeomNonlinearType;
		}
		else
		{
			// 이 변수는 Time History Global Control에서 입력받은 값을 사용
			T_THGC_D ThgcD;
			m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD);
			nGeomNonlinearType = ThgcD.nGeomNonlinearType;
		}
	}
	else
	{
		nAnalType = m_nAnalTypeRdo+1;
		nAnalMethod = m_nAnalMethodRdo+1;

		nGeomNonlinearType = m_nGeomNonlinearTypeRdo;
	}

	if(nAnalType == D_THIS_ANALTYPE_NONLINEAR && nAnalMethod != D_THIS_ANALMETHOD_MODAL)
	{
		if(nGeomNonlinearType==D_THIS_GEOMNONLIN_NONE)
		{
			bEnableLargeDispTH = FALSE;
		}
		else
		{
			bEnableNoneTH = FALSE;
			bEnableSTandCS = FALSE;
		}
	}
	
	BOOL bCheckGeomNonLinearType = FALSE;
	if (/*ThgcD.nGeomNonlinearType == D_THIS_GEOMNONLIN_NONE && */nAnalType == D_THIS_ANALTYPE_NONLINEAR && nAnalMethod == D_THIS_ANALMETHOD_DIRECT)
		bCheckGeomNonLinearType = TRUE;

	CArray<T_THIS_K, T_THIS_K> rThisKeyList;
	T_THIS_K KeyThis;
	T_THIS_D DataThis;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(rThisKeyList);
	int nCount = rThisKeyList.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		KeyThis = rThisKeyList.GetAt(i);
		m_pDoc->m_pAttrCtrl->GetThis(KeyThis, DataThis);
		if (nAnalType != DataThis.nAnalType) continue;
		if (bCheckGeomNonLinearType)
		{
			if (nGeomNonlinearType != DataThis.nGeomNonlinearType) continue;
		}
		if (!(nAnalMethod==D_THIS_ANALMETHOD_MODAL && DataThis.nAnalMethod==D_THIS_ANALMETHOD_MODAL) &&
			!(nAnalMethod!=D_THIS_ANALMETHOD_MODAL && DataThis.nAnalMethod!=D_THIS_ANALMETHOD_MODAL)) continue;
		strFinalStateName = _T("TH : ") + DataThis.LoadCaseName;

		if(m_bModify)
		{
			if(DataThis.LoadCaseId >= m_Data.LoadCaseId) continue;
		}

		if(bEnableNoneTH==FALSE || bEnableLargeDispTH==FALSE)
		{
			if(bEnableNoneTH==FALSE&&DataThis.nGeomNonlinearType==D_THIS_GEOMNONLIN_NONE) continue;
			else if(bEnableLargeDispTH==FALSE&& (DataThis.nGeomNonlinearType==D_THIS_GEOMNONLIN_LARGE_DISP || DataThis.nGeomNonlinearType == D_THIS_GEOMNONLIN_P_DELTA)) continue;
		}

		m_wndFinalStateCmb.AddString(strFinalStateName);
	}

	m_bInitCmb = FALSE;

	if(bEnableSTandCS)
	{
		T_STLD_D DataStld;
		CArray<T_STLD_K, T_STLD_K> aStldKeyList;
		m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldKeyList);
		for(int i = 0; i < aStldKeyList.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetStld(aStldKeyList[i], DataStld);
			if(DataStld.LoadCaseType == _T("CS") || DataStld.LoadCaseType == _T("TCS")) continue;
			strFinalStateName = _T("ST : ");
			strFinalStateName += DataStld.LoadCaseName; 
			m_wndFinalStateCmb.AddString(strFinalStateName);
		}

		if(m_pDoc->m_pAttrCtrl->GetCountStag() > 0)
		{
			CString strConstructionType[] = {_LS(IDS_CMD_THIS_STAG_TYPE1), _LS(IDS_CMD_THIS_STAG_TYPE2)};
			for(int i = 0; i < sizeof(strConstructionType) / sizeof(CString); i++)
			{    
				strFinalStateName = _T("CS : ") + strConstructionType[i];    
				m_wndFinalStateCmb.AddString(strFinalStateName);
			}
		}
	}

	if(m_wndFinalStateCmb.GetCount()>0)
		m_wndFinalStateCmb.SetCurSel(0);
}

BOOL CThisItemDlg_MEC::ValidData()
{
	if (m_Data.LoadCaseName.IsEmpty()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Loadcase_name_is_not_inpu));
		return FALSE;
	}
	if (m_Data.dEndTime <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___End_of_time_must_be_great));
		return FALSE;
	}
	if (m_Data.dDelta <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Time_increment_must_be_gr));
		return FALSE;
	}
	if (m_Data.nAnalMethod == D_THIS_ANALMETHOD_DIRECT)
	{
		if (m_Data.dGamma <= 0 || m_Data.dGamma > 1.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Newmark_Method_Gamma_must_be_between_0_1));
			return FALSE;
		}
		if (m_Data.dBeta <= 0 || m_Data.dBeta > 0.5)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Newmark_Method_Beta_must_be_between_0_0_5));
			return FALSE;
		}
	}

	if (m_Data.nOut < 1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Number_of_output_step_mus));
		return FALSE;
	}

	if (m_Data.nAnalType == D_THIS_ANALTYPE_NONLINEAR)
	{
		if (m_Data.BoundaryNL_Old.bDispNorm && m_Data.BoundaryNL_Old.dDispNorm <= 0.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___DISPNORM_must_be_great));
			return FALSE;
		}

		if (m_Data.BoundaryNL_Old.bForceNorm && m_Data.BoundaryNL_Old.dForceNorm <= 0.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___FORCENORM_must_be_great));
			return FALSE;
		}

		if (m_Data.BoundaryNL_Old.bEnergyNorm && m_Data.BoundaryNL_Old.dEnergyNorm <= 0.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___ENERGYNORM_must_be_great));
			return FALSE;
		}
	}
		
	return TRUE;
}

void CThisItemDlg_MEC::AutoCalcCoef()
{
	UpdateData(TRUE);
	T_THIS_D rData;
	rData.Initialize();
	rData.nDampingType            = m_nDampingMethod;  
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
	
	if(rData.nDampingType != 2 || rData.PropDamping.nCoefDefine != 2) return;

	if(m_pDoc->m_pEditData->AutoCalcThisCoef(rData.PropDamping, rData.PropDamping.dRm, rData.PropDamping.dRk))
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

void CThisItemDlg_MEC::OnShowDampingBtn()
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

void CThisItemDlg_MEC::OnChangeIteration()
{
	CtrlManager();
}

void CThisItemDlg_MEC::OnSelchangeCmdThisDampMethodCmb() 
{
	int nIndex = m_wndDampingMethodCmb.GetCurSel();
	if (nIndex == CB_ERR) return;
	m_nDampingMethod = m_wndDampingMethodCmb.GetItemData(nIndex);  

	CtrlManager();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation	
}

void CThisItemDlg_MEC::OnSelchangeCmdFinalStateCmb()
{
	CtrlManager();
}

void CThisItemDlg_MEC::OnCmdSubsequentTypeRdo() 
{
	CtrlManager();	
}

void CThisItemDlg_MEC::OnCmdSubsequentChk() 
{
	CtrlManager();	
}

void CThisItemDlg_MEC::OnCmdIterationCtrlBtn()
{
	if(!Dlg2Data()) return;
	//if(!ValidData()) return;
// 	T_NTIS_D ntisD;
// 	ntisD.Initialize();
// 	ntisD.nAnalMethod = D_NTIS_ANALMETHOD_NONLINEAR_STATIC;
// 	ntisD.StaticCtrl.nCtrlMethod = 0;
// 	ntisD.BoundaryNL = m_Data.BoundaryNL;

	T_NTIS_BOUNDARY BoundaryNL = m_Data.BoundaryNL;
	CNtisIterationCtrlDlg dlg(&BoundaryNL, TRUE, NULL);
	dlg.SetInitPos(D_INIT_POS_LT);

	if (dlg.DoModal() == IDOK)
	{
		m_Data.BoundaryNL = BoundaryNL;
	}
}

void CThisItemDlg_MEC::OnCmdDescBtn()
{
	CThisItemDescDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

// 이 모델에서 (Force Type or Seismic Control Device) 가 설정된 General Link를 사용중인 경우 TRUE 반환
BOOL CThisItemDlg_MEC::CheckValidNllp()
{
	BOOL bUseGeneralLink_forceType = FALSE;
	BOOL bUseGeneralLink_SeisDev   = FALSE;

	CArray<T_NLNK_K,T_NLNK_K> aNlnkK;
	T_NLNK_K NlnkK;
	T_NLNK_D NlnkD;

	T_NLLP_K NllpK = 0; 
	T_NLLP_D NllpD;
	NllpD.Initialize();
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartNlnk();
	while(pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextNlnk(pos, NlnkK, NlnkD);

		if(!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD)) { ASSERT(0); continue; }

		if(NllpD.nApplicationType == 1)  // 1=Force
		{
			bUseGeneralLink_forceType = TRUE;
		}
		if(NllpD.nApplicationType==2)    // 2=Element2:Seismic control devices
		{
			bUseGeneralLink_SeisDev=TRUE;
		}
	}

	return bUseGeneralLink_forceType || bUseGeneralLink_SeisDev;
}

void CThisItemDlg_MEC::OnChkUseInitial()
{
	SetInitialLoadText();
	CtrlManager();
}

void CThisItemDlg_MEC::OnCmdInitLoadMethod()
{
	CtrlManager();
}

void CThisItemDlg_MEC::OnSelchangeCmdNemark()
{
	OnNewmarkOpt();
}

void CThisItemDlg_MEC::SetInitialLoadText()
{
	CString strInitialLoadText;
	BOOL bUseInitialLoad = m_chkUseInitial.GetCheck();
	if(bUseInitialLoad)
	{
		T_THGC_D ThgcD;
		m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD);
		strInitialLoadText = ThgcD.nInitLoadMethod==0 ? _LS(IDS_CMD_THIS_INITLOAD_PERFORM_NONLIN):_LS(IDS_CMD_THIS_INITLOAD_IMPORT_STATIC);
	}
	GetDlgItem(IDC_USE_INITIAL_STC)->SetWindowText(strInitialLoadText);
}

void CThisItemDlg_MEC::MakeItemInitialLoadCmb(int nAnalType, int nAnalMethod)
{
	m_cmbInitLoadMethod.ResetContent();
	if(nAnalType==1 && nAnalMethod!=0)
		CDlgUtil::CobxAddItem(m_cmbInitLoadMethod, _LS(IDS_CMD_THIS_INIT_LOAD   ), D_THIS_INITMETHOD_INITIALLOAD);
	CDlgUtil::CobxAddItem(m_cmbInitLoadMethod, _LS(IDS_CMD_THIS_ORDER_IN_SEQ), D_THIS_INITMETHOD_ORDER_IN_SEQLOAD);
	m_cmbInitLoadMethod.SetCurSel(0);
}

void CThisItemDlg_MEC::SetGridDeadLoadCmb()
{
	T_GILC_D data;
	CArray<T_GILC_K, T_GILC_K> arKey;
	m_pDoc->m_pAttrCtrl->GetGilcKeyListFromBaseDB(arKey);

	int nSize = arKey.GetSize();
	int nIndex = 0;
	for (int i = 0; i < nSize; i++)
	{
		if (m_pDoc->m_pAttrCtrl->GetGilcFromBaseDB(arKey[i], data))
		{
			if (data.nLoadCaseType == D_GILC_TYPE_DEAD || data.nLoadCaseType == D_GILC_TYPE_DEADOTHR)
			{
				m_cmbGridLoadCase.AddString(data.LoadCaseName);
				m_cmbGridLoadCase.SetItemData(nIndex, arKey[i]);
				nIndex++;
			}
		}
		else
		{
			ASSERT(!_T("CThisItemDlg_MEC::SetSecondaryDeadLoadCmb"));
		}
	}
	if (nSize != 0)
		m_cmbGridLoadCase.SetCurSel(0);

}

void CThisItemDlg_MEC::OnChkKeepFinalStep()
{
	CtrlManager();
}

void CThisItemDlg_MEC::OnNewmarkOpt()
{
	m_nIntegrationParam = CDlgUtil::CobxGetCurSelItemData(m_cmbIntegrationParam, m_cmbIntegrationParam.GetCurSel());

	OnChangeIntegrationPara();
}

void CThisItemDlg_MEC::SetCheckGeomNonlinearType(int nGeomNonlinearType)
{
	int nType = 0;
	if      (nGeomNonlinearType == D_THIS_GEOMNONLIN_NONE)       nType = 0;
	else if (nGeomNonlinearType == D_THIS_GEOMNONLIN_P_DELTA)    nType = 1;
	else if (nGeomNonlinearType == D_THIS_GEOMNONLIN_LARGE_DISP) nType = 2;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrl_GeomNonlinearType, nType);
}

void CThisItemDlg_MEC::GetCheckGeomNonlinearType(int& nGeomNonlinearType)
{
	int nType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrl_GeomNonlinearType, nType);

	nGeomNonlinearType = D_THIS_GEOMNONLIN_NONE;

	if      (nType == 0) nGeomNonlinearType = D_THIS_GEOMNONLIN_NONE;
	else if (nType == 1) nGeomNonlinearType = D_THIS_GEOMNONLIN_P_DELTA;
	else if (nType == 2) nGeomNonlinearType = D_THIS_GEOMNONLIN_LARGE_DISP;
}

void CThisItemDlg_MEC::OnCmdBtnLdGridAdd()
{
	int nSel = m_cmbGridLoadCase.GetCurSel();
	if (nSel == CB_ERR)
		return;

	T_GILC_K itemData;
	CString strName;
	m_cmbGridLoadCase.GetLBText(nSel, strName);
	itemData = (T_GILC_K)(m_cmbGridLoadCase.GetItemData(nSel));

	CString listString;
	int nCount = m_GridList.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		listString = m_GridList.GetItemText(i, 0);

		if (strName.CompareNoCase(listString) == 0)
		{
			CString errStr;
			errStr.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), strName);
			AfxMessageBox(errStr);
			return;
		}
	}
	m_GridList.InsertItem(nCount, _T(""));

	SetGridItem(nCount, itemData);
	m_GridList.SetItemState(nCount, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CThisItemDlg_MEC::OnCmdBtnLdGridDel()
{
	int iItem = m_GridList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	m_GridList.DeleteItem(iItem);

	int nCount = m_GridList.GetItemCount();
	if (nCount == 0)
		return;

	if (nCount == iItem)
		iItem--;
	m_GridList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

LRESULT CThisItemDlg_MEC::OnBnClickedInitLoadFoldButton(WPARAM wParam, LPARAM lParam)
{
	if (m_wndInitLoadGroupBox.GetFoldState())
		return 0L;

	const int initMethod = CDlgUtil::CobxGetCurSelItemData(m_cmbInitLoadMethod, m_cmbInitLoadMethod.GetCurSel());
	const bool bShowJP = (m_bJapanMode && initMethod == D_THIS_INITMETHOD_ORDER_IN_SEQLOAD) ? TRUE : FALSE;

	SetInitLoadFoldControl(bShowJP, initMethod);
	return 0L;
}