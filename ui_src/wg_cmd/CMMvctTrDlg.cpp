// CMMvctTrDlg.cpp : implementation file
//
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctTrDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvctTrDlg dialog

CCMMvctTrDlg::CCMMvctTrDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvctTrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctTrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_nLoadPointSelection.Add(IDC_CMD_MVCT_INFL_RDO);
	m_nLoadPointSelection.Add(IDC_CMD_MVCT_ALL_RDO);
	m_nAnalysisResult.Add(IDC_CMD_MVCT_NORMAL_RDO);
	m_nAnalysisResult.Add(IDC_CMD_MVCT_NORMAL_FORCE_RDO);
	m_aCtrlIGM.Add(IDC_CMD_IGM1_RDO); // added by mylee
	m_aCtrlIGM.Add(IDC_CMD_IGM2_RDO);
}

void CCMMvctTrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctTrDlg)
	DDX_Control(pDX, IDC_CMD_MVCT_DISP_CHK, m_chkDisp);
	DDX_Control(pDX, IDC_CMD_MVCT_FORCES_CHK, m_chkForce);
	DDX_Control(pDX, IDC_CMD_MVCT_REAC_CHK, m_chkReact);
	DDX_Control(pDX, IDC_CMD_MVCT_COMBINED_CHK, m_chkCombinedStress);
	DDX_Control(pDX, IDC_CMD_IGM1_EDT, m_spinIGM1); // added by mylee
	DDX_Control(pDX, IDC_CMD_IGM2_EDT, m_edtIGM2);
	DDX_Control(pDX, IDC_CMD_IGM2_UNT, m_untIGM2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctTrDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvctTrDlg)
	ON_BN_CLICKED(IDC_CMD_IGM1_RDO, OnChangeIGM)
	ON_BN_CLICKED(IDC_CMD_IGM2_RDO, OnChangeIGM)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvctTrDlg message handlers

BOOL CCMMvctTrDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if (!m_pDoc->m_pAttrCtrl->GetMvcttr(m_Data))
	{
		m_Data.Initialize();    
	}

	m_spinIGM1.SetRange(2, 10);
	m_spinIGM1.SetInteger(TRUE);	
	m_edtIGM2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untIGM2.SetUnitType(D_UNITSYS_BASE_LENGTH);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctTrDlg::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_nLoadPointSelection, m_Data.nLoadPointSelection-1);
	CDlgUtil::CtrlRadioSetCheck(this, m_nAnalysisResult, m_Data.nAnalysisResult-1);
	
	m_chkDisp.SetCheck(m_Data.bDispl);
	m_chkForce.SetCheck(m_Data.bForce);
	m_chkReact.SetCheck(m_Data.bReact);
	m_chkCombinedStress.SetCheck(m_Data.bCombinedStress);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlIGM, m_Data.nInflGenPoint); // added by mylee
	OnChangeIGM();
	m_spinIGM1.SetValue(m_Data.nNumUnitLoad);
	m_edtIGM2.SetEditUnit(m_Data.dDistance);
	
	UpdateData(FALSE);
}

BOOL CCMMvctTrDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_nLoadPointSelection, m_Data.nLoadPointSelection);
	m_Data.nLoadPointSelection += 1;
	CDlgUtil::CtrlRadioGetCheck(this, m_nAnalysisResult, m_Data.nAnalysisResult);
	m_Data.nAnalysisResult += 1;
	m_Data.bDispl=m_chkDisp.GetCheck();
	m_Data.bForce=m_chkForce.GetCheck();
	m_Data.bReact=m_chkReact.GetCheck();
	m_Data.bCombinedStress=m_chkCombinedStress.GetCheck();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIGM, m_Data.nInflGenPoint); // added by mylee
	if (m_Data.nInflGenPoint == 0)
		m_Data.nNumUnitLoad = m_spinIGM1.GetEditValueInt();
	else m_Data.dDistance = m_edtIGM2.GetEditValue();
	
	return TRUE;
}

void CCMMvctTrDlg::OnOK() 
{
	if (!Dlg2Data()) return;
	if (!m_pDoc->m_pDataCtrl->AddMvcttr(m_Data)) return;
	
	CDialogMove::OnOK();
}

void CCMMvctTrDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CCMMvctTrDlg::OnChangeIGM() 
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIGM, nCheck);

	m_spinIGM1.EnableWindow(nCheck == 0);
	m_edtIGM2.EnableWindow(nCheck == 1);
	m_untIGM2.EnableWindow(nCheck == 1);
}
