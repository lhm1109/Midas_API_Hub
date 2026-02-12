// CMIehcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMIehcDlg.h"
#include "CMIehcShearRDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// [GEN-1160] Fiber Core Cover 기능 Gen에서 사용 안 함. 885버전 UI 보여줌

/////////////////////////////////////////////////////////////////////////////
// CCMIehcDlg
CCMIehcDlg::CCMIehcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMIehcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMIehcDlg)
	//}}AFX_DATA_INIT
	m_nAssignType   = D_FIBR_TYPE_IEHP;
	m_nBeamLoc      = D_IEHC_BEAM_LOC_I;
	m_bWallConsOut  = TRUE;
	m_nFAreaSizeCore=0;	
	m_nWAreaSizeCore=0;
	m_nFAreaSizeCover=0;
	m_nWAreaSizeCover=0;
}

void CCMIehcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMIehcDlg)
	DDX_Radio  (pDX, IDC_WG_CMD_IEHC_LOC_BEAM_IEND_RDO, m_nBeamLoc);
	DDX_Check  (pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_RAREA_CHK, m_bBeamConsiderRebar);
	DDX_Radio  (pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_FAREA_AUTO_SIZE_RDO, m_nFAreaSizeCore);	
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NY_EDT, m_edtBeamDivNumNyCore);
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NZ_EDT, m_edtBeamDivNumNzCore);
	DDX_Radio  (pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_FAREA_AUTO_SIZE_RDO, m_nFAreaSizeCover);
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NY_EDT, m_edtBeamDivNumNyCover);
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NZ_EDT, m_edtBeamDivNumNzCover);
	DDX_Check  (pDX, IDC_WG_CMD_IEHC_FIBR_WALL_RAREA_CHK, m_bWallConsiderRebar);
	DDX_Check  (pDX, IDC_WG_CMD_IEHC_FIBR_WALL_OUT_CHK, m_bWallConsOut);		
	DDX_Radio  (pDX, IDC_WG_CMD_IEHC_FIBR_WALL_CORE_FAREA_AUTO_SIZE_RDO, m_nWAreaSizeCore);	
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_EDT, m_edtWallDivNumYCore);	
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Z_EDT, m_edtWallDivNumZCore);
	DDX_Radio  (pDX, IDC_WG_CMD_IEHC_FIBR_WALL_COVER_FAREA_AUTO_SIZE_RDO, m_nWAreaSizeCover);
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_EDT, m_edtWallDivNumYCover);
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Z_EDT, m_edtWallDivNumZCover);
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_R_EDT, m_edtShearR);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMIehcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMIehcDlg)
	ON_BN_CLICKED(IDC_WG_CMD_IEHC_FIBR_WALL_OUT_CHK      , OnOnConsOutplBtn)
	ON_BN_CLICKED(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_LOC_BTN, OnShearRBtn)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NY_SPIN, OnSpinBeamNyCore)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NZ_SPIN, OnSpinBeamNzCore)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Z_SPIN , OnSpinWallZCore )
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_SPIN , OnSpinWallYCore )
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NY_SPIN, OnSpinBeamNyCover)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NZ_SPIN, OnSpinBeamNzCover)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Z_SPIN , OnSpinWallZCover )
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_SPIN , OnSpinWallYCover )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMIehcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitCtrl();
	Data2Dlg();
	EnableDisableCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMIehcDlg::InitCtrl()
{
	(static_cast<CEdit*>(GetDlgItem(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_R_EDT)))->SetReadOnly(TRUE);

	if(m_nAssignType == D_FIBR_TYPE_PHGT)
	{
		this->SetWindowText(_LS(IDS_CMD_IEHC_PO_TITLE));
	}
	else if (m_nAssignType == D_FIBR_TYPE_IEHP)
	{
#ifdef _MEC
		this->SetWindowText(_LS(IDS_CMD_FIBER_DIVISION_AUTOGEN));
#else
		this->SetWindowText(_LS(IDS_DB_DT_IEHC));
#endif
	}

	AlignCtrls();

	// PMS 5317-50 : Shear R은 국내출시에서 제외함. 솔버에는 0.5를 넘겨줌.
	BOOL bShowFiberWallShearR = CTestEnvMgr::GetTestEnvST(_LSX(ShowFiberWallShearR)) == _LSX(yes);
	if(!bShowFiberWallShearR)
	{
		GetDlgItem(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_LOC_BTN)->ShowWindow(FALSE);
		GetDlgItem(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_R_STC  )->ShowWindow(FALSE);
		GetDlgItem(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_R_EDT  )->ShowWindow(FALSE);
	}

	m_edtBeamDivNumNyCore.SetRange(6, 100);
	m_edtBeamDivNumNyCore.SetInteger(TRUE);
	m_edtBeamDivNumNzCore.SetRange(6, 100);
	m_edtBeamDivNumNzCore.SetInteger(TRUE);
	m_edtBeamDivNumNyCover.SetRange(6, 100);
	m_edtBeamDivNumNyCover.SetInteger(TRUE);
	m_edtBeamDivNumNzCover.SetRange(6, 100);
	m_edtBeamDivNumNzCover.SetInteger(TRUE);
	m_edtWallDivNumZCore.SetRange(6, 100);
	m_edtWallDivNumZCore.SetInteger(TRUE);
	m_edtWallDivNumYCore.SetRange(1, 100);
	m_edtWallDivNumYCore.SetInteger(TRUE);
	m_edtWallDivNumZCover.SetRange(6, 100);
	m_edtWallDivNumZCover.SetInteger(TRUE);
	m_edtWallDivNumYCover.SetRange(1, 100);
	m_edtWallDivNumYCover.SetInteger(TRUE);
}

void CCMIehcDlg::AlignCtrls()
{
	CRect rectRef, rectMove;
	int nYDist;

	// Step1 : Pushover 일 경우 Data for Strength Auto-Calculation of Hinges 를 Hide 하고 나머지 컨트럴 올림
	if(m_nAssignType == D_FIBR_TYPE_PHGT)
	{
		CArray<UINT,UINT> aCtrls_BeamLoc;
		aCtrls_BeamLoc.Add(IDC_WG_CMD_IEHC_DATA_GRP);		
		aCtrls_BeamLoc.Add(IDC_WG_CMD_IEHC_LOC_GRP);
		aCtrls_BeamLoc.Add(IDC_WG_CMD_IEHC_LOC_BEAM_STC);
		aCtrls_BeamLoc.Add(IDC_WG_CMD_IEHC_LOC_BEAM_IEND_RDO);
		aCtrls_BeamLoc.Add(IDC_WG_CMD_IEHC_LOC_BEAM_CENT_RDO);
		aCtrls_BeamLoc.Add(IDC_WG_CMD_IEHC_LOC_BEAM_JEND_RDO);
		CDlgUtil::CtrlShowHide(this, aCtrls_BeamLoc, FALSE);

		CRect rectBeamLoc, rectFibrOpt;
		GetDlgItem(IDC_WG_CMD_IEHC_DATA_GRP)->GetWindowRect(rectBeamLoc);
		GetDlgItem(IDC_WG_CMD_IEHC_FIBR_GRP)->GetWindowRect(rectFibrOpt);  
		nYDist = rectBeamLoc.top - rectFibrOpt.top;


		CArray<UINT,UINT> m_aCtrlIDMoveUp;
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_GRP);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_GRP);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_RAREA_CHK);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_NUM_GRP);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_GRP);		
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_FAREA_SIZE_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_FAREA_AUTO_SIZE_RDO);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_FAREA_EQUAL_SIZE_RDO);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NY_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NY_EDT);
		//m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NY_SPIN);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NZ_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NZ_EDT);
		//m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NZ_SPIN);		
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_GRP);		
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_FAREA_SIZE_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_FAREA_AUTO_SIZE_RDO);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_FAREA_EQUAL_SIZE_RDO);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NY_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NY_EDT);
		//m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NY_SPIN);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NZ_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NZ_EDT);
		//m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NZ_SPIN);
		
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_GRP);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_RAREA_CHK);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_OUT_CHK);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_NUM_GRP);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_GRP);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_FAREA_SIZE_STC);		
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_FAREA_AUTO_SIZE_RDO);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_FAREA_EQUAL_SIZE_RDO);		
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Z_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Z_EDT);
		//m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Z_SPIN);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_EDT);
		//m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_SPIN);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y2_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_GRP);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_FAREA_SIZE_STC);		
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_FAREA_AUTO_SIZE_RDO);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_FAREA_EQUAL_SIZE_RDO);		
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Z_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Z_EDT);
		//m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Z_SPIN);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_EDT);
		//m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_SPIN);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y2_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_LOC_BTN);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_R_STC);
		m_aCtrlIDMoveUp.Add(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_R_EDT);		

		CDlgUtil::CtrlMoveDistY(this, m_aCtrlIDMoveUp, nYDist);
	}

	// Step2 : Civil 일 경우 Wall 그룹을 Hide 하고 컨트럴을 조절 함
#ifdef _CIVIL		
	{
		CArray<UINT,UINT> aCtrls_Wall;	
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_GRP);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_RAREA_CHK);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_OUT_CHK);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_NUM_GRP);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_GRP);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_FAREA_SIZE_STC);		
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_FAREA_AUTO_SIZE_RDO);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_FAREA_EQUAL_SIZE_RDO);		
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Z_STC);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Z_EDT);
		//aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Z_SPIN);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_STC);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_EDT);
		//aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_SPIN);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y2_STC);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_GRP);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_FAREA_SIZE_STC);		
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_FAREA_AUTO_SIZE_RDO);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_FAREA_EQUAL_SIZE_RDO);		
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Z_STC);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Z_EDT);
		//aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Z_SPIN);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_STC);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_EDT);
		//aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_SPIN);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y2_STC);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_LOC_BTN);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_R_STC);
		aCtrls_Wall.Add(IDC_WG_CMD_IEHC_FIBR_WALL_SHEAR_R_EDT);	
		CDlgUtil::CtrlShowHide(this, aCtrls_Wall, FALSE);

		GetDlgItem(IDC_WG_CMD_IEHC_FIBR_BEAM_GRP)->GetWindowRect(rectRef);
		GetDlgItem(IDC_WG_CMD_IEHC_FIBR_GRP)->GetWindowRect(rectMove);
		rectMove.bottom = rectRef.bottom + globalUtils.ScaleByDPI(10);
		ScreenToClient(rectMove);
		GetDlgItem(IDC_WG_CMD_IEHC_FIBR_GRP)->MoveWindow(rectMove);
	}
#endif // _CIVIL

	// Step3 : OK, Cancle 컨트럴 Move + 대화상자 크기 조절
	CArray<UINT,UINT> aCtrlsOKCancel;	
	aCtrlsOKCancel.Add(IDOK);
	aCtrlsOKCancel.Add(IDCANCEL);

	GetDlgItem(IDC_WG_CMD_IEHC_FIBR_GRP)->GetWindowRect(rectRef);
	GetDlgItem(IDOK)->GetWindowRect(rectMove);
	nYDist = rectRef.bottom - rectMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aCtrlsOKCancel, nYDist);
	
	CRect rectThis, rectLast;
	GetDlgItem(IDOK)->GetWindowRect(rectLast);	
	GetWindowRect(rectThis);
	rectThis.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);
	MoveWindow(rectThis);
}

void CCMIehcDlg::Data2Dlg()
{
	m_edtBeamDivNumNyCore.SetValue(m_nBeamDivNumNyCore);
	m_edtBeamDivNumNzCore.SetValue(m_nBeamDivNumNzCore);
	m_edtWallDivNumZCore .SetValue(m_nWallDivNumZCore );
	m_edtWallDivNumYCore .SetValue(m_nWallDivNumYCore );
	m_edtBeamDivNumNyCover.SetValue(m_nBeamDivNumNyCover);
	m_edtBeamDivNumNzCover.SetValue(m_nBeamDivNumNzCover);
	m_edtWallDivNumZCover .SetValue(m_nWallDivNumZCover );
	m_edtWallDivNumYCover .SetValue(m_nWallDivNumYCover );
	m_edtShearR      .SetEditUnit(m_dR);

	UpdateData(FALSE);
}

void CCMIehcDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_nBeamDivNumNyCore = m_edtBeamDivNumNyCore.GetEditValueInt();
	m_nBeamDivNumNzCore = m_edtBeamDivNumNzCore.GetEditValueInt();
	m_nWallDivNumZCore  = m_edtWallDivNumZCore .GetEditValueInt();
	m_nWallDivNumYCore  = m_edtWallDivNumYCore .GetEditValueInt();
	m_nBeamDivNumNyCover = m_edtBeamDivNumNyCover.GetEditValueInt();
	m_nBeamDivNumNzCover = m_edtBeamDivNumNzCover.GetEditValueInt();
	m_nWallDivNumZCover  = m_edtWallDivNumZCover .GetEditValueInt();
	m_nWallDivNumYCover  = m_edtWallDivNumYCover .GetEditValueInt();
	m_dR            = m_edtShearR      .GetEditValue();
}

void CCMIehcDlg::OnOK() 
{	
	Dlg2Data();
		
	T_IEHC_D IehcD;
	GetData(IehcD.nBeamLoc,IehcD.nBeamDivNumNy,IehcD.nBeamDivNumNz,IehcD.nBeamDivNumNyCover,IehcD.nBeamDivNumNzCover,IehcD.bWallConsOut,IehcD.nWallDivNumZ,IehcD.nWallDivNumY,IehcD.nWallDivNumZCover,IehcD.nWallDivNumYCover,IehcD.dR,IehcD.nFAreaSizeCore,IehcD.nFAreaSizeCover,IehcD.nWAreaSize,IehcD.nWAreaSizeCover, IehcD.bConsiderRebarArea1D, IehcD.bConsiderRebarAreaWall);
	if(m_nAssignType==D_FIBR_TYPE_IEHP)
	{
		if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddIehc(IehcD))
			CDialogMove::OnOK();
	}
	else
	{
		if(CDBDoc::GetDocPoint()->m_pEditData->CheckIehc(IehcD, m_nAssignType))
			CDialogMove::OnOK();
	}
}

void CCMIehcDlg::EnableDisableCtrls() 
{	
	UpdateData(TRUE);

	CArray<UINT,UINT> aCtrlsOutPlane;
	aCtrlsOutPlane.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_STC);
	aCtrlsOutPlane.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y2_STC);
	aCtrlsOutPlane.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_EDT);
	//aCtrlsOutPlane.Add(IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_SPIN);
	aCtrlsOutPlane.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_STC);
	aCtrlsOutPlane.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y2_STC);
	aCtrlsOutPlane.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_EDT);
	//aCtrlsOutPlane.Add(IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_SPIN);
	CDlgUtil::CtrlEnableDisable(this, aCtrlsOutPlane, m_bWallConsOut);
}

void CCMIehcDlg::OnShearRBtn()
{
	CCMIehcShearRDlg dlg;
	dlg.SetGlobal(TRUE);
	dlg.SetData(D_IEHC_R_GLOBAL, m_dR);
	dlg.SetInitPos(D_INIT_POS_RT, 10);
	if(dlg.DoModal()==IDOK)
	{
		m_dR = dlg.GetShearRValue();
		m_edtShearR.SetEditUnit(m_dR);
	}
}

void CCMIehcDlg::OnSpinBeamNyCore(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NY_EDT,6,100,pNMHDR,pResult);
	*pResult = 0;
}

void CCMIehcDlg::OnSpinBeamNzCore(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NZ_EDT,6,100,pNMHDR,pResult);
	*pResult = 0;
}

void CCMIehcDlg::OnSpinWallZCore(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Z_EDT,6,100,pNMHDR,pResult);
	*pResult = 0;
}

void CCMIehcDlg::OnSpinWallYCore(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_WG_CMD_IEHC_FIBR_WALL_CORE_Y_EDT,1,100,pNMHDR,pResult);
	*pResult = 0;
}

void CCMIehcDlg::OnSpinBeamNyCover(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NY_EDT,6,100,pNMHDR,pResult);
	*pResult = 0;
}

void CCMIehcDlg::OnSpinBeamNzCover(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NZ_EDT,6,100,pNMHDR,pResult);
	*pResult = 0;
}

void CCMIehcDlg::OnSpinWallZCover(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Z_EDT,6,100,pNMHDR,pResult);
	*pResult = 0;
}

void CCMIehcDlg::OnSpinWallYCover(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_WG_CMD_IEHC_FIBR_WALL_COVER_Y_EDT,1,100,pNMHDR,pResult);
	*pResult = 0;
}

void CCMIehcDlg::SetData(int nAssignType, int nBeamLoc, int nBeamDivNumNyCore, int nBeamDivNumNzCore, int nBeamDivNumNyCover, int nBeamDivNumNzCover, BOOL bWallConsOut, int nWallDivNumZCore, int nWallDivNumYCore, int nWallDivNumZCover, int nWallDivNumYCover, double dR, int nFAreaSizeCore,int nFAreaSizeCover,int nWAreaSizeCore,int nWAreaSizeCover, BOOL bConsiderRebarArea1D, BOOL bConsiderRebarAreaWall)
{
	m_nAssignType			= nAssignType  ;
	m_nBeamLoc				= nBeamLoc     ;
	m_nBeamDivNumNyCore		= nBeamDivNumNyCore;
	m_nBeamDivNumNzCore		= nBeamDivNumNzCore;
	m_nBeamDivNumNyCover	= nBeamDivNumNyCover;
	m_nBeamDivNumNzCover	= nBeamDivNumNzCover;
	m_bWallConsOut			= bWallConsOut ;
	m_nWallDivNumZCore		= nWallDivNumZCore ;
	m_nWallDivNumYCore		= nWallDivNumYCore ;
	m_nWallDivNumZCover		= nWallDivNumZCover ;
	m_nWallDivNumYCover		= nWallDivNumYCover ;
	m_dR					= dR           ;
	m_nFAreaSizeCore		= nFAreaSizeCore   ;
	m_nFAreaSizeCover		= nFAreaSizeCover   ;
	m_nWAreaSizeCore		= nWAreaSizeCore   ;	
	m_nWAreaSizeCover		= nWAreaSizeCover   ;	
	m_bBeamConsiderRebar	= bConsiderRebarArea1D;
	m_bWallConsiderRebar	= bConsiderRebarAreaWall;
}
void CCMIehcDlg::GetData(int& nBeamLoc, int& nBeamDivNumNyCore, int& nBeamDivNumNzCore, int& nBeamDivNumNyCover, int& nBeamDivNumNzCover, BOOL& bWallConsOut, int& nWallDivNumZCore, int& nWallDivNumYCore, int& nWallDivNumZCover, int& nWallDivNumYCover, double& dR, int& nFAreaSizeCore,int& nFAreaSizeCover,int& nWAreaSizeCore,int& nWAreaSizeCover, BOOL& bConsiderRebarArea1D, BOOL& bConsiderRebarAreaWall)
{
	nBeamLoc				= m_nBeamLoc     ;
	nBeamDivNumNyCore		= m_nBeamDivNumNyCore;
	nBeamDivNumNzCore		= m_nBeamDivNumNzCore;
	nBeamDivNumNyCover		= m_nBeamDivNumNyCover;
	nBeamDivNumNzCover		= m_nBeamDivNumNzCover;
	bWallConsOut			= m_bWallConsOut ;
	nWallDivNumZCore		= m_nWallDivNumZCore ;
	nWallDivNumYCore		= m_nWallDivNumYCore ;
	nWallDivNumZCover		= m_nWallDivNumZCover ;
	nWallDivNumYCover		= m_nWallDivNumYCover ;
	dR						= m_dR           ;
	nFAreaSizeCore		    = m_nFAreaSizeCore     ;
	nFAreaSizeCover		    = m_nFAreaSizeCover     ;
	nWAreaSizeCore			= m_nWAreaSizeCore   ;
	nWAreaSizeCover			= m_nWAreaSizeCover   ;
	bConsiderRebarArea1D	= m_bBeamConsiderRebar;
	bConsiderRebarAreaWall	= m_bWallConsiderRebar;
}

void CCMIehcDlg::OnOnConsOutplBtn()
{
	EnableDisableCtrls();

	if(m_bWallConsOut)
	{
		int nWallZ = m_edtWallDivNumZCore.GetEditValueInt();
		const int nMinZDirConsOutpl=4; // 면외 비선형성 고려 시 Z방향 분할개수 4개 이상으로 (Core Cover 각각..)
		if(nMinZDirConsOutpl<4)
			m_edtWallDivNumZCore.SetValue(nMinZDirConsOutpl);

		nWallZ = m_edtWallDivNumZCover.GetEditValueInt();
		if(nMinZDirConsOutpl<4)
			m_edtWallDivNumZCover.SetValue(nMinZDirConsOutpl);
	}
}