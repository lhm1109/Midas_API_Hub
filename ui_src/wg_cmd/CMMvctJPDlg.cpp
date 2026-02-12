// CMMvctJPDlg.cpp : implementation file
// 2002. 7. 18 by TAE
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctJPDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvctJPDlg dialog


CCMMvctJPDlg::CCMMvctJPDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvctJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctJPDlg)
	m_nReaction = 0;
	m_nDisplacement = 0;  
	m_nForceMoment = 0;  
	m_nFrameOpt = 0;
	m_nPlateOpt = 0;
	m_bDisplacement = FALSE;
	m_bForceMoment = FALSE;
	m_bReaction = FALSE;
	m_bStressCalc = FALSE;
	m_bStressCalcFrame = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();

	arReactionGroup.Add(IDC_CMD_MVCT_FILTERS_REACTION_ALL);
	arReactionGroup.Add(IDC_CMD_MVCT_FILTERS_REACTION_GROUP);
	arReactionGroup.Add(IDC_CMD_MVCT_FILTERS_REACTION_CBO);

	arDisplaceGroup.Add(IDC_CMD_MVCT_FILTERS_DISP_ALL);
	arDisplaceGroup.Add(IDC_CMD_MVCT_FILTERS_DISP_GROUP);
	arDisplaceGroup.Add(IDC_CMD_MVCT_FILTERS_DISP_CBO);

	arForceMomGroup.Add(IDC_CMD_MVCT_FILTERS_FORCE_ALL);
	arForceMomGroup.Add(IDC_CMD_MVCT_FILTERS_FORCE_GROUP);
	arForceMomGroup.Add(IDC_CMD_MVCT_FILTERS_FORCE_CBO);
}




void CCMMvctJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctJPDlg)
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_FORCE_CBO, m_cboForceMoment);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_DISP_CBO, m_cboDisplacement);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_REACTION_CBO, m_cboReaction);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_REACTION_ALL, m_nReaction);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_DISP_ALL, m_nDisplacement);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_FORCE_ALL, m_nForceMoment);
	DDX_Radio(pDX, IDC_CMD_FRAME_OPT1, m_nFrameOpt);
	DDX_Radio(pDX, IDC_CMD_PLATE_OPT1, m_nPlateOpt);
	DDX_Check(pDX, IDC_CMD_MVCT_FILTERS_DISP, m_bDisplacement);
	DDX_Check(pDX, IDC_CMD_MVCT_FILTERS_FORCE, m_bForceMoment);
	DDX_Check(pDX, IDC_CMD_MVCT_FILTERS_REACTION, m_bReaction);
	DDX_Check(pDX, IDC_CMD_PLATE_STRESS_CALC, m_bStressCalc);
	DDX_Check(pDX, IDC_CMD_FRAME_STRESS_CALC, m_bStressCalcFrame);  
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvctJPDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvctJPDlg)
	ON_BN_CLICKED(IDC_CMD_MVCT_OK, OnCmdMvctOk)
	ON_BN_CLICKED(IDC_CMD_MVCT_CANCEL, OnCmdMvctCancel)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_DISP, OnCmdMvctCtrls)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_DISP_GROUP, OnCmdMvctCtrls)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_FORCE, OnCmdMvctCtrls)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_FORCE_GROUP, OnCmdMvctCtrls)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_REACTION, OnCmdMvctCtrls)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_REACTION_GROUP, OnCmdMvctCtrls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCMMvctJPDlg message handlers


/////////////////////////////////////////////////////////////////////////////
//
// Initialize
//
void CCMMvctJPDlg::InitComboBox() 
{
	CArray<T_GRUP_K, T_GRUP_K> GroupKeyList;
	T_GRUP_D GroupData;	

	m_pDoc->m_pAttrCtrl->GetGrupKeyList(GroupKeyList);

	for(int i=0; i<GroupKeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(GroupKeyList.GetAt(i), GroupData);
		m_cboReaction.AddString(GroupData.GroupName);
		m_cboDisplacement.AddString(GroupData.GroupName);
		m_cboForceMoment.AddString(GroupData.GroupName);
	}
}

void CCMMvctJPDlg::SetData2Dlg()
{
	ASSERT(m_pDoc);
	T_GRUP_D GroupData;

	// Calculation Position
	if(m_Data.nCalcPositionPlate > 0) m_nPlateOpt = m_Data.nCalcPositionPlate-1;
	if(m_Data.nCalcPositionFrame > 0) m_nFrameOpt = m_Data.nCalcPositionFrame-1;
	m_bStressCalc = m_Data.bStressCalc;
	m_bStressCalcFrame = m_Data.bStressCalcFrame;

	// Calculation Filters
	m_bReaction = m_Data.bReaction;
	m_nReaction = m_Data.nReacFilter;
	m_pDoc->m_pAttrCtrl->GetGrup(m_Data.ReacGrup, GroupData);
	m_cboReaction.SelectString(0,GroupData.GroupName);
	
	m_bDisplacement = m_Data.bDisplacement;
	m_nDisplacement = m_Data.nDispFilter;
	m_pDoc->m_pAttrCtrl->GetGrup(m_Data.DispGrup, GroupData);
	m_cboDisplacement.SelectString(0,GroupData.GroupName);

	m_bForceMoment = m_Data.bForce;
	m_nForceMoment = m_Data.nForcFilter;
	m_pDoc->m_pAttrCtrl->GetGrup(m_Data.ForcGrup, GroupData);
	m_cboForceMoment.SelectString(0,GroupData.GroupName);

	UpdateData(FALSE);
}

BOOL CCMMvctJPDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	
	ASSERT(m_pDoc);
	if (!m_pDoc->m_pAttrCtrl->GetMvctjp(m_Data))
	{
		m_Data.Initialize();
	}
	

	InitComboBox();
	SetData2Dlg();
	UpdateCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
// OnOK
//
BOOL CCMMvctJPDlg::SetDlg2Data()
{
	UpdateData(TRUE);
	m_Data.Initialize();

	// Calculation Position
	m_Data.nCalcPositionPlate = m_nPlateOpt + 1;
	m_Data.nCalcPositionFrame = m_nFrameOpt + 1;
	m_Data.bStressCalc = m_bStressCalc;
	m_Data.bStressCalcFrame = m_bStressCalcFrame;

	// Calculation Filters
	CString strTemp = _T("");

	m_Data.bReaction = m_bReaction;
	if(m_Data.bReaction)
	{
		m_Data.nReacFilter = m_nReaction;
		if(m_nReaction)
		{
			if(m_cboReaction.GetCurSel() < 0) return FALSE;

			m_cboReaction.GetLBText(m_cboReaction.GetCurSel(), strTemp);
			m_Data.ReacGrup = m_pDoc->m_pAttrCtrl->GetGrupKey(strTemp);
		}
	}

	m_Data.bDisplacement = m_bDisplacement;
	if(m_Data.bDisplacement)
	{
		m_Data.nDispFilter = m_nDisplacement;
		if(m_nDisplacement)
		{
			if(m_cboDisplacement.GetCurSel() < 0) return FALSE;

			m_cboDisplacement.GetLBText(m_cboDisplacement.GetCurSel(), strTemp);
			m_Data.DispGrup = m_pDoc->m_pAttrCtrl->GetGrupKey(strTemp);
		}
	}

	m_Data.bForce = m_bForceMoment;
	if(m_Data.bForce)
	{
		m_Data.nForcFilter = m_nForceMoment;
		if(m_nForceMoment)
		{
			if(m_cboForceMoment.GetCurSel() < 0) return FALSE;

			m_cboForceMoment.GetLBText(m_cboForceMoment.GetCurSel(), strTemp);
			m_Data.ForcGrup = m_pDoc->m_pAttrCtrl->GetGrupKey(strTemp);
		}	
	}
	return TRUE;
}

void CCMMvctJPDlg::OnCmdMvctOk() 
{
	// TODO: Add your control notification handler code here
	if (!SetDlg2Data()) return;
	
	
	if (!m_pDoc->m_pDataCtrl->AddMvctjp(m_Data)) return;
	
	
	CDialogMove::OnOK();	
}

void CCMMvctJPDlg::OnCmdMvctCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();	
}


/////////////////////////////////////////////////////////////////////////////
//
// Controls
//
void CCMMvctJPDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, arReactionGroup, m_bReaction);
	CDlgUtil::CtrlEnableDisable(this, arDisplaceGroup, m_bDisplacement);
	CDlgUtil::CtrlEnableDisable(this, arForceMomGroup, m_bForceMoment);

	if(m_bReaction) 
		GetDlgItem(IDC_CMD_MVCT_FILTERS_REACTION_CBO)->EnableWindow(m_nReaction);

	if(m_bDisplacement) 
		GetDlgItem(IDC_CMD_MVCT_FILTERS_DISP_CBO)->EnableWindow(m_nDisplacement);

	if(m_bForceMoment) 
		GetDlgItem(IDC_CMD_MVCT_FILTERS_FORCE_CBO)->EnableWindow(m_nForceMoment);
}

void CCMMvctJPDlg::OnCmdMvctCtrls() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrls();
}
