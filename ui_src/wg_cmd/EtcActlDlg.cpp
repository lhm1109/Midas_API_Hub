// EtcActlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EtcActlDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtcActlDlg dialog


CEtcActlDlg::CEtcActlDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEtcActlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEtcActlDlg)
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
	//}}AFX_DATA_INIT
}


void CEtcActlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtcActlDlg)
	DDX_Control(pDX, IDC_CMD_ACTL_EXTOL_UNIT, m_unitExTolerance);
	DDX_Control(pDX, IDC_CMD_ACTL_EXTOL_EDIT, m_edtExTolerance);
	DDX_Control(pDX, IDC_CMD_ACTL_EXLCASE_EDIT, m_edtExLoadCase);
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
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEtcActlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CEtcActlDlg)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtcActlDlg message handlers

BOOL CEtcActlDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_edtExTolerance.SetUnitType(CUnitCtrl::m_ACTL_UNIT.dblTrussTolerance);
	m_unitExTolerance.SetUnitType(CUnitCtrl::m_ACTL_UNIT.dblTrussTolerance);
	
	InitDialogData();
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

#if defined(_CIVIL)
	GetDlgItem(IDC_CMD_AUTO_WALL_CONSTRAINT_CHK)->ShowWindow(SW_HIDE);

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_CALC_BEAM_STRESSES_CHK);
	aControls.Add(IDC_CMD_CONSIDER_RBAR_CHK);
	aControls.Add(IDC_CMD_CHANGE_LOCAL_AXIS_CHK);
	aControls.Add(IDC_CMD_CONSIDER_TENDON_CHK);
	aControls.Add(IDC_CMD_CONSIDER_SECT_OFFSET_CHK);
	aControls.Add(IDC_CMD_CONSIDER_SELF_CHK);

	GetDlgItem(IDC_CMD_AUTO_WALL_CONSTRAINT_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_CALC_BEAM_STRESSES_CHK)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
#endif


	GetDlgItem(IDC_CMD_CONSIDER_SECT_OFFSET_CHK)->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_CONSIDER_SELF_CHK)->ShowWindow(bShow);
	GetDlgItem(IDC_CMD_CONSIDER_TENDON_CHK)->ShowWindow(bShow);

	if(!bShow)
	{
#if defined(_CIVIL)
		GetDlgItem(IDC_CMD_CHANGE_LOCAL_AXIS_CHK)->GetWindowRect(rRef);
#else
		GetDlgItem(IDC_CMD_CONSIDER_RBAR_CHK)->GetWindowRect(rRef);
#endif
		CArray<UINT, UINT> aOkCancel;
		aOkCancel.Add(IDOK);
		aOkCancel.Add(IDCANCEL);
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		nDistY = (rRef.bottom+globalUtils.ScaleByDPI(16)) - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, aOkCancel, nDistY);
	
		GetDlgItem(IDOK)->GetWindowRect(rRef);
		GetWindowRect(rMove);
		rMove.bottom = rRef.bottom+globalUtils.ScaleByDPI(8);

		MoveWindow(rMove);
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEtcActlDlg::InitDialogData()
{
	T_ACTL_D data;
	if (!m_pDoc->m_pAttrCtrl->GetActl(data)) 
		data.Initialize();

	m_bAutoCons1 = data.bAutoConstraintNormal;
	m_bAutoCons2 = data.bAutoConstraintRotation;

	data.nIterationType = 2; // MNET:2567 20061108 JHKIM

	m_edtExLoadCase.SetRange(0, SHRT_MAX);
	m_edtExLoadCase.SetValue(data.nIterPerLoad);
	m_edtExLoadCase.SetInteger(TRUE);
	m_edtExTolerance.SetEditUnit(data.dblTrussTolerance);

	m_bConsiderSecf = data.bConsiderSecf ;  // v6.7.1 추가사항....by HP...
	m_bConsiderRbar = data.bConsiderRbar ; 
	m_bConsiderSectOffset = data.bConsiderSectOffset;
	m_bConsiderSCS = data.bConsiderSCS;
	m_bConsiderTDF = data.bConsiderTDF;
	m_bTransferReac = data.bTransferReac;
	m_bChangeLcAxOfTapSect = data.bChangeLcAxOfTapSect;
	m_bAutoWallConstraint = data.bAutoWallConstraint;
	m_bCalculateBeamStresses = data.bCalculateBeamStresses;
}

void CEtcActlDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	T_ACTL_D data; data.Initialize();
	data.bAutoConstraintNormal   = m_bAutoCons1;
	data.bAutoConstraintRotation = m_bAutoCons2;
	data.bConsiderSecf           = m_bConsiderSecf;	// v6.7.1 추가사항....by HP...
	data.bConsiderRbar           = m_bConsiderRbar;	
	data.bConsiderSectOffset     = m_bConsiderSectOffset;
	data.bConsiderSCS            = m_bConsiderSCS;
	data.bConsiderTDF            = m_bConsiderTDF;
	data.bTransferReac           = m_bTransferReac;
	data.bChangeLcAxOfTapSect  = m_bChangeLcAxOfTapSect;
	data.nIterationType          = 2; // MNET:2567 20061108 JHKIM
	m_edtExLoadCase.GetEditValue(data.nIterPerLoad);
	data.dblTrussTolerance = m_edtExTolerance.GetEditValue();
	data.bAutoWallConstraint = m_bAutoWallConstraint;
	data.bCalculateBeamStresses = m_bCalculateBeamStresses;
	
	if (!m_pDoc->m_pDataCtrl->AddActl(data)) return;

	CDialogMove::OnOK();
}

