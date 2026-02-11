// CMHydrationAnalyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMHydrationAnalyDlg_MEC.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHydrationAnalyDlg_MEC dialog


CCMHydrationAnalyDlg_MEC::CCMHydrationAnalyDlg_MEC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMHydrationAnalyDlg_MEC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMHydrationAnalyDlg_MEC)
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	m_nFinalStage = 0;
	m_nEvaluation = 0;
	m_bCreepShrink = FALSE;
	m_nCreepShrinkType = 0;
	m_nCreepCalcMethod = 0;
	m_bUseEquivalent = FALSE;
	m_bSelfWeight = FALSE;
	m_bDisp = FALSE;
	m_bLoad = FALSE;
	m_bWork = FALSE;

	m_num = 0;

	m_aCreep.RemoveAll();
	m_aCreep.Add(IDC_HHCT_CREEP_CHK);
	m_aCreep.Add(IDC_HHCT_SHRINK_CHK);
	m_aCreep.Add(IDC_HHCT_CANDS_CHK);
	m_aCreep.Add(IDC_CMD_STATIC_NUMBER);
	m_aCreep.Add(IDC_CMD_STATIC_TOLERANCE);
	m_aCreep.Add(IDC_HHCT_GENERAL_CHK);
	m_aCreep.Add(IDC_HHCT_EM_CHK);
	m_aCreep.Add(IDC_HHCT_NUM);
	m_aCreep.Add(IDC_HHCT_NUM_SPIN);
	m_aCreep.Add(IDC_HHCT_TOL);
	m_aCreep.Add(IDC_HHCT_P1EDT);
	m_aCreep.Add(IDC_HHCT_T1EDT);
	m_aCreep.Add(IDC_HHCT_P2EDT);
	m_aCreep.Add(IDC_HHCT_T2EDT);

	m_aEffModul.RemoveAll();
	m_aEffModul.Add(IDC_STATIC_EEFF_FRM);
	m_aEffModul.Add(IDC_STATIC_EEFF_T);
	m_aEffModul.Add(IDC_STATIC_PHI1);
	m_aEffModul.Add(IDC_HHCT_P1EDT);
	m_aEffModul.Add(IDC_STATIC_TR);
	m_aEffModul.Add(IDC_HHCT_T1EDT);
	m_aEffModul.Add(IDC_STATIC_DAY1);
	m_aEffModul.Add(IDC_STATIC_PHI2);
	m_aEffModul.Add(IDC_HHCT_P2EDT);
	m_aEffModul.Add(IDC_STATIC_TL);
	m_aEffModul.Add(IDC_HHCT_T2EDT);
	m_aEffModul.Add(IDC_STATIC_DAY2);
}

void CCMHydrationAnalyDlg_MEC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydrationAnalyDlg_MEC)
	DDX_Radio(pDX, IDC_HHCT_FINAL_STAGE_LAST, m_nFinalStage);
	DDX_Control(pDX, IDC_HHCT_COBX_HSTG, m_cmbHstg);
	DDX_Control(pDX, IDC_HHCT_CN_FACTOR, m_edtFactor);
	DDX_Control(pDX, IDC_HHCT_INIT_TEMPER, m_edtTemper);
	DDX_Control(pDX, IDC_HHCT_INIT_TEMPER_UNIT, m_untTemper);
	DDX_Radio(pDX, IDC_HHCT_EVALU_CENTER, m_nEvaluation);
	DDX_Check(pDX, IDC_HHCT_CS_CHK, m_bCreepShrink);
	DDX_Radio(pDX, IDC_HHCT_CREEP_CHK, m_nCreepShrinkType);
	DDX_Radio(pDX, IDC_HHCT_GENERAL_CHK, m_nCreepCalcMethod);
	DDX_Control(pDX, IDC_HHCT_P1EDT, m_edtphi1);
	DDX_Control(pDX, IDC_HHCT_T1EDT, m_edtDay1);
	DDX_Control(pDX, IDC_HHCT_P2EDT, m_edtphi2);
	DDX_Control(pDX, IDC_HHCT_T2EDT, m_edtDay2);
	DDX_Check(pDX, IDC_HHCT_UEA_CHK, m_bUseEquivalent);
	DDX_Check(pDX, IDC_HHCT_SELFWEIGHT_CHK, m_bSelfWeight);
	DDX_Control(pDX, IDC_HHCT_SELFEDT, m_edtSelfWeightFact);
	DDX_Control(pDX, IDC_HHCT_MAX_ITER_PER_INCRE_EDT, m_edtMaxIterPerIncre);
	DDX_Check(pDX, IDC_HHCT_DISP_TOL_CHK, m_bDisp);
	DDX_Control(pDX, IDC_HHCT_DISP_TOL_EDT, m_edtDisp);
	DDX_Check(pDX, IDC_HHCT_LOAD_TOL_CHK, m_bLoad);
	DDX_Control(pDX, IDC_HHCT_LOAD_TOL_EDT, m_edtLoad);
	DDX_Check(pDX, IDC_HHCT_WORK_TOL_CHK, m_bWork);
	DDX_Control(pDX, IDC_HHCT_WORK_TOL_EDT, m_edtWork);

	DDX_Control(pDX, IDC_HHCT_NUM, m_num_spin);
	//DDX_Control(pDX, IDC_HHCT_NUM_SPIN, m_num_spin);
	DDX_Control(pDX, IDC_HHCT_TOL, m_tol);
	//}}AFX_DATA_MAP
}

void CCMHydrationAnalyDlg_MEC::InitUnit()
{
	m_edtFactor.SetUnitType(D_UNITSYS_NONE);

	m_edtTemper.SetUnitType(D_UNITSYS_BASE_TEMPER);
	m_untTemper.SetUnitType(D_UNITSYS_BASE_TEMPER);

	m_edtphi1.SetUnitType(D_UNITSYS_NONE);
	m_edtDay1.SetUnitType(D_UNITSYS_NONE);
	m_edtphi2.SetUnitType(D_UNITSYS_NONE);
	m_edtDay2.SetUnitType(D_UNITSYS_NONE);
	m_edtSelfWeightFact.SetUnitType(D_UNITSYS_NONE);
	m_edtMaxIterPerIncre.SetUnitType(D_UNITSYS_NONE);
	m_edtDisp.SetUnitType(D_UNITSYS_NONE);
	m_edtLoad.SetUnitType(D_UNITSYS_NONE);
	m_edtWork.SetUnitType(D_UNITSYS_NONE);
	m_tol.SetUnitType(D_UNITSYS_NONE);
}

void CCMHydrationAnalyDlg_MEC::InitCombo()
{
	CArray<T_HSTG_K, T_HSTG_K> aHstgK;
	m_pDoc->m_pAttrCtrl->GetHstgKeyList(aHstgK);

	T_HSTG_D HstgD;

	m_cmbHstg.ResetContent();

	for (int i = 0; i < aHstgK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetHstg(aHstgK[i], HstgD)) continue;

		CDlgUtil::CobxAddItem(m_cmbHstg, HstgD.StageName, aHstgK[i]);
	}

	m_cmbHstg.SetCurSel(0);
}

void CCMHydrationAnalyDlg_MEC::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_HHCT_COBX_HSTG)->EnableWindow(m_nFinalStage == 1);

	CDlgUtil::CtrlEnableDisable(this, m_aCreep, m_bCreepShrink);

	CDlgUtil::CtrlEnableDisable(this, m_aEffModul, m_bCreepShrink && m_nCreepCalcMethod == 1);

	GetDlgItem(IDC_HHCT_SELFEDT)->EnableWindow(m_bSelfWeight);

	GetDlgItem(IDC_HHCT_DISP_TOL_EDT)->EnableWindow(m_bDisp);
	GetDlgItem(IDC_HHCT_LOAD_TOL_EDT)->EnableWindow(m_bLoad);
	GetDlgItem(IDC_HHCT_WORK_TOL_EDT)->EnableWindow(m_bWork);

	GetDlgItem(IDC_HHCT_REMOVE)->EnableWindow(m_pDoc->m_pAttrCtrl->ExistHhct());
}

void CCMHydrationAnalyDlg_MEC::Data2Dlg()
{
	m_nFinalStage = m_Data.nFinalStage;

	if (m_Data.nFinalStage == 1)
		CDlgUtil::CobxSetCurSelItemData(m_cmbHstg, m_Data.FinalHstgKey);

	m_edtFactor.SetEditUnit(m_Data.dTheta);
	m_edtTemper.SetEditUnit(m_Data.dInitTemp);
	m_nEvaluation = m_Data.nEvaluation - 1;
	m_bCreepShrink = m_Data.bCreepShrinkage;
	m_nCreepShrinkType = m_Data.nType - 1;
	m_nCreepCalcMethod = m_Data.nCreepCalcMethod;
	m_edtphi1.SetEditUnit(m_Data.dPhi1);
	m_edtDay1.SetEditUnit(m_Data.nDay1);
	m_edtphi2.SetEditUnit(m_Data.dPhi2);
	m_edtDay2.SetEditUnit(m_Data.nDay2);
	m_bUseEquivalent = m_Data.bUseEquivalentAge;
	m_bSelfWeight = m_Data.bIncludeSelfWeight;
	m_edtSelfWeightFact.SetEditUnit(m_Data.dSelfWeightFactor);
	m_edtMaxIterPerIncre.SetEditUnit(m_Data.nMaxIterPerIncre);
	m_bDisp = m_Data.bConv[0];
	m_edtDisp.SetEditUnit(m_Data.dConv[0]);
	m_bLoad = m_Data.bConv[1];
	m_edtLoad.SetEditUnit(m_Data.dConv[1]);
	m_bWork = m_Data.bConv[2];
	m_edtWork.SetEditUnit(m_Data.dConv[2]);

	// 사용 안함 
	//m_num = m_Data.nIter;
	CString csIter;
	csIter.Format(_T("%d"), m_Data.nIter);
	m_num_spin.SetWindowText(csIter);

	m_num_spin.SetRange(1, 10000);
	m_num_spin.SetInteger(TRUE);
	m_tol.SetEditUnit(m_Data.dTol);

	UpdateData(FALSE);
}

BOOL CCMHydrationAnalyDlg_MEC::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.nFinalStage = m_nFinalStage;

	if (m_Data.nFinalStage == 1)
	{
		if (!CDlgUtil::CobxGetItemDataByCurSel(m_cmbHstg, (DWORD&)m_Data.FinalHstgKey))
		{
			AfxMessageBox(_LS(IDS_CMD0417__Error___Final_stage_is_not_selected_));
			return FALSE;
		}
	}

	m_Data.dInitTemp = m_edtTemper.GetEditValue();
	m_Data.dTheta = m_edtFactor.GetEditValue();
	m_Data.nEvaluation = m_nEvaluation + 1;// + 1;
	m_Data.bCreepShrinkage = m_bCreepShrink;
	m_Data.nType = m_nCreepShrinkType + 1;
	m_Data.nCreepCalcMethod = m_nCreepCalcMethod;
	m_Data.dPhi1 = m_edtphi1.GetEditValue();
	m_Data.nDay1 = m_edtDay1.GetEditValue();
	m_Data.dPhi2 = m_edtphi2.GetEditValue();
	m_Data.nDay2 = m_edtDay2.GetEditValue();
	m_Data.bUseEquivalentAge = m_bUseEquivalent;
	m_Data.bIncludeSelfWeight = m_bSelfWeight;
	m_Data.dSelfWeightFactor = m_edtSelfWeightFact.GetEditValue();
	m_Data.nMaxIterPerIncre = m_edtMaxIterPerIncre.GetEditValue();
	m_Data.bConv[0] = m_bDisp;
	m_Data.dConv[0] = m_edtDisp.GetEditValue();
	m_Data.bConv[1] = m_bLoad;
	m_Data.dConv[1] = m_edtLoad.GetEditValue();
	m_Data.bConv[2] = m_bWork;
	m_Data.dConv[2] = m_edtWork.GetEditValue();

	// 사용 안함 	
	m_Data.dTol = m_tol.GetEditValue();
	CString csIter;
	m_num_spin.GetWindowText(csIter);
	if ( !CStrParser::GetINumber(csIter, m_Data.nIter) ) return FALSE;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMHydrationAnalyDlg_MEC, CDialogMove)
	//{{AFX_MSG_MAP(CCMHydrationAnalyDlg_MEC)
	ON_BN_CLICKED(IDC_HHCT_FINAL_STAGE_LAST, OnHhctFinalStageRdo)
	ON_BN_CLICKED(IDC_HHCT_FINAL_STAGE_OTHER, OnHhctFinalStageRdo)
	ON_BN_CLICKED(IDC_HHCT_CS_CHK, OnHhctCreepShrinkChk)
	ON_BN_CLICKED(IDC_HHCT_GENERAL_CHK, OnHhctCreepCalcMethodChk)
	ON_BN_CLICKED(IDC_HHCT_EM_CHK, OnHhctCreepCalcMethodChk)
	ON_BN_CLICKED(IDC_HHCT_SELFWEIGHT_CHK, OnHhctSelfWeightChk)
	ON_BN_CLICKED(IDC_HHCT_DISP_TOL_CHK, OnHhctDispChk)
	ON_BN_CLICKED(IDC_HHCT_LOAD_TOL_CHK, OnHhctLoadChk)
	ON_BN_CLICKED(IDC_HHCT_WORK_TOL_CHK, OnHhctWorkChk)
	ON_BN_CLICKED(IDC_HHCT_REMOVE, OnHhctRemove)
	ON_BN_CLICKED(IDC_HHCT_OK, OnHhctOk)
	ON_BN_CLICKED(IDC_HHCT_CANCEL, OnHhctCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMHydrationAnalyDlg_MEC message handlers

BOOL CCMHydrationAnalyDlg_MEC::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	if (!m_pDoc->m_pAttrCtrl->GetHhct(m_Data))
	{
		m_Data.Initialize();
	}

	InitUnit();
	InitCombo();

	Data2Dlg();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMHydrationAnalyDlg_MEC::OnHhctFinalStageRdo()
{
	EnableDisableControls();
}

void CCMHydrationAnalyDlg_MEC::OnHhctCreepShrinkChk()
{
	EnableDisableControls();
}

void CCMHydrationAnalyDlg_MEC::OnHhctCreepCalcMethodChk()
{
	EnableDisableControls();
}

void CCMHydrationAnalyDlg_MEC::OnHhctSelfWeightChk()
{
	EnableDisableControls();
}

void CCMHydrationAnalyDlg_MEC::OnHhctDispChk()
{
	EnableDisableControls();
}

void CCMHydrationAnalyDlg_MEC::OnHhctLoadChk()
{
	EnableDisableControls();
}

void CCMHydrationAnalyDlg_MEC::OnHhctWorkChk()
{
	EnableDisableControls();
}

void CCMHydrationAnalyDlg_MEC::OnHhctRemove()
{
	if (m_pDoc->m_pDataCtrl->DelHhct())
	{
		CDialogMove::OnOK();
	}
}

void CCMHydrationAnalyDlg_MEC::OnHhctOk()
{
	if (!Dlg2Data()) return;
	if (!m_pDoc->m_pDataCtrl->AddHhct(m_Data)) return;;

	CDialogMove::OnOK();
}

void CCMHydrationAnalyDlg_MEC::OnHhctCancel()
{
	CDialogMove::OnCancel();
}