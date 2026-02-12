// EtcActlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EtcActlDlg_MEC.h"
#include "CMNXLoadStepDlg.h"
#include "CMNlctNSAdvNonlinearParam.h"
#include "EtcActlTrussElemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtcActlDlg_MEC dialog


CEtcActlDlg_MEC::CEtcActlDlg_MEC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEtcActlDlg_MEC::IDD, pParent)
{
	m_bAutoCons1 = FALSE;
	m_bAutoCons2 = FALSE;
	m_bConsiderSecf = FALSE;
	m_bConsiderRbar = FALSE;
	m_bConsiderSectOffset = FALSE;
	m_bConsiderSCS = FALSE;
	m_bConsiderTDF = FALSE;
	m_bTransferReac = FALSE;
	m_bChangeLcAxOfTapSect = FALSE;
	m_bAutoWallConstraint = TRUE;
	m_bCalculateBeamStresses = FALSE;
	m_bDKFormulaShellElem = FALSE;
	m_bClassicalFormulaSolid = FALSE;
	m_nBeamSectionChange = 1;
	m_bConsiderCable = TRUE;
}


void CEtcActlDlg_MEC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_ETC_PC_ACONS1, m_bAutoCons1);
	DDX_Check(pDX, IDC_ETC_PC_ACONS2, m_bAutoCons2);
	DDX_Check(pDX, IDC_CMD_CONSIDER_STIFFNESS_CHK, m_bConsiderSecf);
	DDX_Check(pDX, IDC_CMD_CONSIDER_RBAR_CHK, m_bConsiderRbar);
	DDX_Check(pDX, IDC_CMD_CONSIDER_SECT_OFFSET_CHK, m_bConsiderSectOffset);
	DDX_Check(pDX, IDC_CMD_CONSIDER_SELF_CHK, m_bConsiderSCS);
	DDX_Check(pDX, IDC_CMD_CONSIDER_TENDON_CHK, m_bConsiderTDF);
	DDX_Check(pDX, IDC_CMD_SLAVE_NODES_CHK, m_bTransferReac);
	DDX_Check(pDX, IDC_CMD_CHANGE_LOCAL_AXIS_CHK, m_bChangeLcAxOfTapSect);
	DDX_Check(pDX, IDC_CMD_AUTO_WALL_CONSTRAINT_CHK, m_bAutoWallConstraint);
	DDX_Check(pDX, IDC_CMD_CALC_BEAM_STRESSES_CHK, m_bCalculateBeamStresses);
	DDX_Check(pDX, IDC_ETC_DK_FORMULA_SHELL_ELEM_CHK, m_bDKFormulaShellElem);
	DDX_Check(pDX, IDC_ETC_CLASSICAL_FORMULA_SOLID_CHK, m_bClassicalFormulaSolid);
	DDX_Radio(pDX, IDC_CMD_BEAMPROP_RDO1, m_nBeamSectionChange);
	DDX_Check(pDX, IDC_CMD_CONSIDER_CABLE_ELEM_CHK, m_bConsiderCable);
}

BEGIN_MESSAGE_MAP(CEtcActlDlg_MEC, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_TENS_COMP_TRUSS_BTN, OnCmdTrussElemBtn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtcActlDlg_MEC message handlers

BOOL CEtcActlDlg_MEC::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();

	InitDialogData();
	SetCtrlText();
	AlignControls();
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEtcActlDlg_MEC::AlignControls()
{
#if defined(_MGEN)
	GetDlgItem(IDC_CMD_CONSIDER_RBAR_CHK)->ShowWindow(SW_HIDE);
#endif

	BOOL bShow = (CProduct::IsMovingType(D_PRODUCT_MOVING_JP));
#if defined(_MGEN)
	bShow = FALSE;
#endif

#if defined(_CIVIL)
	GetDlgItem(IDC_CMD_CHANGE_LOCAL_AXIS_CHK)->ShowWindow(SW_SHOW);
#else
	GetDlgItem(IDC_CMD_CHANGE_LOCAL_AXIS_CHK)->ShowWindow(SW_HIDE);
#endif

	CRect rRef, rMove;
	int nDistY = 0;
	CArray<UINT, UINT> aControls; aControls.RemoveAll();

#if defined(_CIVIL)
	GetDlgItem(IDC_CMD_AUTO_WALL_CONSTRAINT_CHK)->ShowWindow(SW_HIDE);
		
	aControls.Add(IDC_CMD_CALC_BEAM_STRESSES_CHK);
	aControls.Add(IDC_CMD_CONSIDER_RBAR_CHK);
	aControls.Add(IDC_CMD_CHANGE_LOCAL_AXIS_CHK);
	aControls.Add(IDC_CMD_CONSIDER_CABLE_ELEM_CHK);
	aControls.Add(IDC_CMD_CONSIDER_TENDON_CHK);
	aControls.Add(IDC_CMD_CONSIDER_SECT_OFFSET_CHK);
	aControls.Add(IDC_CMD_CONSIDER_SELF_CHK);	

	aControls.Add(IDC_CMD_BEAM_SECTION_FRM);
	aControls.Add(IDC_CMD_BEAMPROP_RDO1);
	aControls.Add(IDC_CMD_BEAMPROP_RDO2);

	aControls.Add(IDC_CMD_TENS_COMP_TRUSS_STC);
	aControls.Add(IDC_CMD_TENS_COMP_TRUSS_BTN);

	GetDlgItem(IDC_CMD_AUTO_WALL_CONSTRAINT_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_CALC_BEAM_STRESSES_CHK)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
#endif

	GetDlgItem(IDC_CMD_CONSIDER_SECT_OFFSET_CHK)->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_CONSIDER_SELF_CHK)->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_CONSIDER_TENDON_CHK)->ShowWindow(bShow);

	if (!bShow)
	{
		// 올리는 작업 필요
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_BEAM_SECTION_FRM);
		aControls.Add(IDC_CMD_BEAMPROP_RDO1);
		aControls.Add(IDC_CMD_BEAMPROP_RDO2);

		aControls.Add(IDC_CMD_TENS_COMP_TRUSS_STC);
		aControls.Add(IDC_CMD_TENS_COMP_TRUSS_BTN);


#if defined(_CIVIL)
		GetDlgItem(IDC_CMD_CONSIDER_TENDON_CHK)->GetWindowRect(rRef);
#else
		GetDlgItem(IDC_CMD_CONSIDER_CABLE_ELEM_CHK)->GetWindowRect(rRef);
#endif
		GetDlgItem(IDC_CMD_BEAM_SECTION_FRM)->GetWindowRect(rMove);
		nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		GetDlgItem(IDC_CMD_TENS_COMP_TRUSS_STC)->GetWindowRect(rRef);
		CArray<UINT, UINT> aOkCancel;
		aOkCancel.Add(IDOK);
		aOkCancel.Add(IDCANCEL);
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		nDistY = (rRef.bottom + globalUtils.ScaleByDPI(16)) - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, aOkCancel, nDistY);

		GetDlgItem(IDOK)->GetWindowRect(rRef);
		GetWindowRect(rMove);
		rMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);

		MoveWindow(rMove);
	}
}

void CEtcActlDlg_MEC::InitDialogData()
{
	T_ACTL_D data;
	if (!m_pDoc->m_pAttrCtrl->GetActl(m_Data))
		m_Data.Initialize();

	m_bAutoCons1 = m_Data.bAutoConstraintNormal;
	m_bAutoCons2 = m_Data.bAutoConstraintRotation;

	m_Data.nIterationType = 2; // MNET:2567 20061108 JHKIM

	m_bConsiderSecf = m_Data.bConsiderSecf ;  // v6.7.1 추가사항....by HP...
	m_bConsiderRbar = m_Data.bConsiderRbar ;
	m_bConsiderSectOffset = m_Data.bConsiderSectOffset;
	m_bConsiderSCS = m_Data.bConsiderSCS;
	m_bConsiderTDF = m_Data.bConsiderTDF;
	m_bTransferReac = m_Data.bTransferReac;
	m_bChangeLcAxOfTapSect = m_Data.bChangeLcAxOfTapSect;
	m_bAutoWallConstraint = m_Data.bAutoWallConstraint;
	m_bCalculateBeamStresses = m_Data.bCalculateBeamStresses;

	m_bClassicalFormulaSolid = m_Data.bClassicalFormulaSolid;
	m_nBeamSectionChange = m_Data.nBeamSectionChange;
	m_bConsiderCable = m_Data.bConsiderCable;
}

void CEtcActlDlg_MEC::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	m_Data.bAutoConstraintNormal   = m_bAutoCons1;
	m_Data.bAutoConstraintRotation = m_bAutoCons2;
	m_Data.bConsiderSecf           = m_bConsiderSecf;	// v6.7.1 추가사항....by HP...
	m_Data.bConsiderRbar           = m_bConsiderRbar;	
	m_Data.bConsiderSectOffset     = m_bConsiderSectOffset;
	m_Data.bConsiderSCS            = m_bConsiderSCS;
	m_Data.bConsiderTDF            = m_bConsiderTDF;
	m_Data.bTransferReac           = m_bTransferReac;
	m_Data.bChangeLcAxOfTapSect  = m_bChangeLcAxOfTapSect;
	m_Data.nIterationType          = 2; // MNET:2567 20061108 JHKIM
	m_Data.bAutoWallConstraint = m_bAutoWallConstraint;
	m_Data.bCalculateBeamStresses = m_bCalculateBeamStresses;

	m_Data.bClassicalFormulaSolid = m_bClassicalFormulaSolid;
	m_Data.nBeamSectionChange = m_nBeamSectionChange;
	m_Data.bConsiderCable = m_bConsiderCable;	

	if (!m_pDoc->m_pDataCtrl->AddActl(m_Data)) return;

	CDialogMove::OnOK();
}

void CEtcActlDlg_MEC::OnCmdTrussElemBtn()
{
	CEtcActlTrussElemDlg dlg;
	dlg.SetData(m_Data);
	dlg.SetInitPos(D_INIT_POS_RT);

	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_Data);
	}
}

void CEtcActlDlg_MEC::SetCtrlText() const
{
	GetDlgItem(IDC_ETC_PC_ACONS2)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_ETC_PC_ACONS2));
	GetDlgItem(IDC_ETC_PC_ACONS1)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_ETC_PC_ACONS1));
	GetDlgItem(IDC_ETC_CLASSICAL_FORMULA_SOLID_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_ETC_CLASSICAL_FORMULA_SOLID_CHK));
	GetDlgItem(IDC_CMD_CONSIDER_STIFFNESS_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_CONSIDER_STIFFNESS_CHK));
	GetDlgItem(IDC_CMD_SLAVE_NODES_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_SLAVE_NODES_CHK));
	GetDlgItem(IDC_CMD_AUTO_WALL_CONSTRAINT_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_AUTO_WALL_CONSTRAINT_CHK));
	GetDlgItem(IDC_CMD_CALC_BEAM_STRESSES_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_CALC_BEAM_STRESSES_CHK));
	GetDlgItem(IDC_CMD_CONSIDER_RBAR_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_CONSIDER_RBAR_CHK));
	GetDlgItem(IDC_CMD_CHANGE_LOCAL_AXIS_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_CHANGE_LOCAL_AXIS_CHK));
	GetDlgItem(IDC_CMD_CONSIDER_TENDON_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_CONSIDER_TENDON_CHK));
	GetDlgItem(IDC_CMD_CONSIDER_SECT_OFFSET_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_CONSIDER_SECT_OFFSET_CHK));
	GetDlgItem(IDC_CMD_CONSIDER_SELF_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_CONSIDER_SELF_CHK));
	GetDlgItem(IDC_CMD_CONSIDER_CABLE_ELEM_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_CONSIDER_CABLE_ELEM_CHK));
	GetDlgItem(IDC_CMD_BEAM_SECTION_FRM)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_BEAM_SECTION_FRM));
	GetDlgItem(IDC_CMD_BEAMPROP_RDO1)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_BEAMPROP_RDO1));
	GetDlgItem(IDC_CMD_BEAMPROP_RDO2)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_BEAMPROP_RDO2));
	GetDlgItem(IDC_CMD_TENS_COMP_TRUSS_STC)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_CMD_TENS_COMP_TRUSS_STC));
	GetDlgItem(IDOK)->SetWindowText(_LS(IDS_CMD_ACTL_IDOK));
	GetDlgItem(IDCANCEL)->SetWindowText(_LS(IDS_CMD_ACTL_IDCANCEL));
	GetDlgItem(IDC_ETC_DK_FORMULA_SHELL_ELEM_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_IDC_ETC_DK_FORMULA_SHELL_ELEM_CHK));
}