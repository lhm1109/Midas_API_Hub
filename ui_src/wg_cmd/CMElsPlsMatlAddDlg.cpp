// CMElsPlsMatlAddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMElsPlsMatlAddDlg.h"
#include "CMElsPlsParamDlg.h"

#include "CMElsPlsMason1Dlg.h" // MNET:2603 20061212
#include "CMElsPlsMason2Dlg.h"
#include "CMElsPlsMason3Dlg.h"
#include "CMElsPlsMason4Dlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\DBMatlCoordDlg.h"    // for CDBMatlCoordDlg

#include "..\wg_base\wg_base_DlgUtil.h"
#include "CMElsPlsConcDmgGraphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_base\TestEnvMgr.h"

#define D_PLS_TRESCA  0
#define D_PLS_MISES   1
#define D_PLS_MOHR    2
#define D_PLS_DRUCKER 3
#define D_PLS_MICRO   4
#define D_PLS_MASON   5
#define D_PLS_CONCDMG 6

#define D_PLS_MASON_MATL_COORD_GLOBAL    0    // [Masonry 재료축] Global-Y : Global-X // Global
#define D_PLS_MASON_MATL_COORD_LOCAL    -1    // [Masonry 재료축] Local-y : Local-x   // Element Local
#define D_PLS_MASON_MATL_COORD_ANGLE    -4    // [Masonry 재료축] Global-Z : Angle

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMatlAddDlg dialog


CCMElsPlsMatlAddDlg::CCMElsPlsMatlAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMElsPlsMatlAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMElsPlsMatlAddDlg)
	m_chkHardTr = FALSE;
	m_nHardTr = 0;
	m_chkHardMhr = FALSE;
	m_strName = _T("");
	m_nHardMhr = 0;
	m_Key = m_OldKey = NULL;

	//}}AFX_DATA_INIT

	m_aOKCancel.RemoveAll();
	m_aOKCancel.Add(IDOK);
	m_aOKCancel.Add(IDCANCEL);
	m_aOKCancel.Add(IDC_ELSPLS_APPLY_BTN);

	m_aTresca.RemoveAll();
	m_aTresca.Add(IDC_ELSPLS_STATIC_PLS_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_STATIC_YIELD);
	m_aTresca.Add(IDC_ELSPLS_YIELD_EDIT);
	m_aTresca.Add(IDC_ELSPLS_YIELD_UNIT);
	m_aTresca.Add(IDC_ELSPLS_STATIC_HARD_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_HARD_CHK_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_ISO_RD_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_KINE_RD_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_MIXED_RD_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_STATIC_COEF_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_COEF_EDIT_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_COEF_UNIT_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_STATIC_STRESS_COEF_TRESCA);
	m_aTresca.Add(IDC_ELSPLS_STRESS_COEF_EDIT_TRESCA);
	m_aTresca.FreeExtra();

	m_aHardTresca.RemoveAll();
	m_aHardTresca.Add(IDC_ELSPLS_ISO_RD_TRESCA);
	m_aHardTresca.Add(IDC_ELSPLS_KINE_RD_TRESCA);
	m_aHardTresca.Add(IDC_ELSPLS_MIXED_RD_TRESCA);
	m_aHardTresca.Add(IDC_ELSPLS_STRESS_COEF_EDIT_TRESCA);
	m_aHardTresca.Add(IDC_ELSPLS_COEF_EDIT_TRESCA);
	m_aHardTresca.FreeExtra();

	m_aVmisesFibr.RemoveAll();
	m_aVmisesFibr.Add(IDC_ELSPLS_FIBR_CHK_VMISES);
	m_aVmisesFibr.FreeExtra();
	
	m_aMohrCl.RemoveAll();
	m_aMohrCl.Add(IDC_ELSPLS_STATIC_PLS_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_STATIC_COHESION);
	m_aMohrCl.Add(IDC_ELSPLS_COHE_EDIT);
	m_aMohrCl.Add(IDC_ELSPLS_COHE_UNIT);
	m_aMohrCl.Add(IDC_ELSPLS_STATIC_FRIC);
	m_aMohrCl.Add(IDC_ELSPLS_FRIC_EDIT);
	m_aMohrCl.Add(IDC_ELSPLS_FRIC_UNIT);
	m_aMohrCl.Add(IDC_ELSPLS_STATIC_HARD_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_HARD_CHK_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_ISO_RD_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_KINE_RD_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_MIXED_RD_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_STATIC_COEF_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_COEF_EDIT_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_COEF_UNIT_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_STATIC_STRESS_COEF_MOHR);
	m_aMohrCl.Add(IDC_ELSPLS_STRESS_COEF_EDIT_MOHR);
	m_aMohrCl.FreeExtra();

	m_aHardMohrCl.RemoveAll();
	m_aHardMohrCl.Add(IDC_ELSPLS_ISO_RD_MOHR);
	m_aHardMohrCl.Add(IDC_ELSPLS_KINE_RD_MOHR);
	m_aHardMohrCl.Add(IDC_ELSPLS_MIXED_RD_MOHR);
	m_aHardMohrCl.Add(IDC_ELSPLS_STRESS_COEF_EDIT_MOHR);
	m_aHardMohrCl.Add(IDC_ELSPLS_COEF_EDIT_MOHR);
	m_aHardMohrCl.FreeExtra();

	m_aMicroP.RemoveAll();
	m_aMicroP.Add(IDC_ELSPLS_STATIC_PLS_MICRO);
	m_aMicroP.Add(IDC_ELSPLS_STATIC_MODEL);
	m_aMicroP.Add(IDC_ELSPLS_STATIC_K1);
	m_aMicroP.Add(IDC_ELSPLS_STATIC_K2);
	m_aMicroP.Add(IDC_ELSPLS_STATIC_K3);
	m_aMicroP.Add(IDC_ELSPLS_STATIC_K4);
	m_aMicroP.Add(IDC_ELSPLS_STATIC_VALUE);
	m_aMicroP.Add(IDC_ELSPLS_K1_EDIT);
	m_aMicroP.Add(IDC_ELSPLS_K2_EDIT);
	m_aMicroP.Add(IDC_ELSPLS_K3_EDIT);
	m_aMicroP.Add(IDC_ELSPLS_K4_EDIT);
	m_aMicroP.Add(IDC_ELSPLS_MOD_BTN);
	m_aMicroP.FreeExtra();

	m_aMasonAngle.RemoveAll();
	m_aMasonAngle.Add(IDC_CMD_MASON_MATL_ANGLE_STC);
	m_aMasonAngle.Add(IDC_CMD_MASON_MATL_ANGLE_CMB);
	m_aMasonAngle.Add(IDC_CMD_MASON_MATL_ANGLE_UNT);
	m_aMasonAngle.FreeExtra();

	m_aMason.RemoveAll();
	m_aMason.Add(IDC_CMD_MASON_BOX_GRP);
	m_aMason.Add(IDC_CMD_MASON_PROP1_STC);
	m_aMason.Add(IDC_CMD_MASON_PROP1_BTN);
	m_aMason.Add(IDC_CMD_MASON_PROP2_STC);
	m_aMason.Add(IDC_CMD_MASON_PROP2_BTN);
	m_aMason.Add(IDC_CMD_MASON_PROP3_STC);
	m_aMason.Add(IDC_CMD_MASON_PROP3_BTN);
	m_aMason.Add(IDC_CMD_MASON_PROP4_STC);
	m_aMason.Add(IDC_CMD_MASON_PROP4_BTN);
	m_aMason.Add(IDC_CMD_MASON_MATL_COORD_GRP);
	m_aMason.Add(IDC_CMD_MASON_MATL_COORD_STC);
	m_aMason.Add(IDC_CMD_MASON_MATL_COORD_CMB);
	m_aMason.Add(IDC_CMD_MASON_MATL_COORD_BTN);
	m_aMason.Append(m_aMasonAngle);
	m_aMason.FreeExtra();

	m_aConcDmg.RemoveAll();
	m_aConcDmg.Add(IDC_CONCDMG_BOX_GRP);
	m_aConcDmg.Add(IDC_CONCDMG_DILIATION_ANG_STC);
	m_aConcDmg.Add(IDC_CONCDMG_DILIATION_ANG_EDT);
	m_aConcDmg.Add(IDC_CONCDMG_DILIATION_ANG_UNT);
	m_aConcDmg.Add(IDC_CONCDMG_ECCENTRICITY_STC);
	m_aConcDmg.Add(IDC_CONCDMG_ECCENTRICITY_EDT);
	m_aConcDmg.Add(IDC_CONCDMG_FBO_FCO_STC);
	m_aConcDmg.Add(IDC_CONCDMG_FBO_FCO_EDT);
	m_aConcDmg.Add(IDC_CONCDMG_K_STC);
	m_aConcDmg.Add(IDC_CONCDMG_K_EDT);
	m_aConcDmg.Add(IDC_CONCDMG_VISCOSITY_STC);
	m_aConcDmg.Add(IDC_CONCDMG_VISCOSITY_EDT);
	m_aConcDmg.Add(IDC_CONCDMG_COMP_BEH_STC);
	m_aConcDmg.Add(IDC_CONCDMG_COMP_BEH_BTN);
	m_aConcDmg.Add(IDC_CONCDMG_TENSILE_BEH_STC);
	m_aConcDmg.Add(IDC_CONCDMG_TENSILE_BEH_BTN);

	m_csOldName.Empty();
	m_Data.Initialize();
	m_bModify = FALSE;
	m_bMicroMod = FALSE;
	for(int i=0; i<4; i++) m_dKValue[i] = 0.;
}


void CCMElsPlsMatlAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMElsPlsMatlAddDlg)
	DDX_Control(pDX, IDC_ELSPLS_STRESS_COEF_EDIT_MOHR, m_edtStrCoefMhr);
	DDX_Control(pDX, IDC_ELSPLS_STRESS_COEF_EDIT_TRESCA, m_edtStrCoefTr);
	DDX_Control(pDX, IDC_ELSPLS_COHE_UNIT, m_untCohe);
	DDX_Control(pDX, IDC_ELSPLS_COHE_EDIT, m_edtCohe);
	DDX_Control(pDX, IDC_ELSPLS_COEF_UNIT_TRESCA, m_untCoefTr);
	DDX_Control(pDX, IDC_ELSPLS_COEF_UNIT_MOHR, m_untCoefMhr);
	DDX_Control(pDX, IDC_ELSPLS_COEF_EDIT_TRESCA, m_edtCoefTr);
	DDX_Control(pDX, IDC_ELSPLS_COEF_EDIT_MOHR, m_edtCeofMhr);
	DDX_Control(pDX, IDC_ELSPLS_FRIC_EDIT, m_edtFric);
	DDX_Control(pDX, IDC_ELSPLS_FRIC_UNIT, m_untFric);
	DDX_Control(pDX, IDC_ELSPLS_YIELD_UNIT, m_untYield);
	DDX_Control(pDX, IDC_ELSPLS_YIELD_EDIT, m_edtYield);
	DDX_Control(pDX, IDC_ELSPLS_STATIC_MODEL_COMBO, m_cmbModel);
	DDX_Check(pDX, IDC_ELSPLS_HARD_CHK_TRESCA, m_chkHardTr);
	DDX_Control(pDX, IDC_ELSPLS_FIBR_CHK_VMISES, m_chkFiberModel);
	DDX_Radio(pDX, IDC_ELSPLS_ISO_RD_TRESCA, m_nHardTr);
	DDX_Check(pDX, IDC_ELSPLS_HARD_CHK_MOHR, m_chkHardMhr);
	DDX_Text(pDX, IDC_ELSPLS_STATIC_NAME_EDIT, m_strName);
	DDX_Radio(pDX, IDC_ELSPLS_ISO_RD_MOHR, m_nHardMhr);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CMD_MASON_MATL_COORD_CMB, m_cmbMasonryMatlCoord);
	DDX_Control(pDX, IDC_CMD_MASON_MATL_ANGLE_CMB, m_cmbMasonryMatlAngle);
 
	UINT nKValue[4] = { IDC_ELSPLS_K1_EDIT, IDC_ELSPLS_K2_EDIT,
											IDC_ELSPLS_K3_EDIT, IDC_ELSPLS_K4_EDIT, };

	for(int i=0; i<4; i++) DDX_Text(pDX, nKValue[i], m_dKValue[i]);

	DDX_Control(pDX, IDC_CONCDMG_DILIATION_ANG_EDT, m_edtDiliationAng);
	DDX_Control(pDX, IDC_CONCDMG_ECCENTRICITY_EDT, m_edtEccentricity);
	DDX_Control(pDX, IDC_CONCDMG_FBO_FCO_EDT, m_edtfbo_fco);
	DDX_Control(pDX, IDC_CONCDMG_K_EDT, m_edtK);
	DDX_Control(pDX, IDC_CONCDMG_VISCOSITY_EDT, m_edtViscosityParam);
}

BEGIN_MESSAGE_MAP(CCMElsPlsMatlAddDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMElsPlsMatlAddDlg)
	ON_BN_CLICKED(IDC_ELSPLS_APPLY_BTN, OnApplyBtn)
	ON_CBN_SELCHANGE(IDC_ELSPLS_STATIC_MODEL_COMBO, OnSelchangeModelCombo)
	ON_CBN_SELCHANGE(IDC_CMD_MASON_MATL_COORD_CMB, OnSelchangeMatlCoordCombo)
	ON_BN_CLICKED(IDC_ELSPLS_MOD_BTN, OnElsplsModBtn)
	ON_BN_CLICKED(IDC_CMD_MASON_PROP1_BTN, OnElsplsMasonBtn1) // MNET:2603 20061212
	ON_BN_CLICKED(IDC_CMD_MASON_PROP2_BTN, OnElsplsMasonBtn2)
	ON_BN_CLICKED(IDC_CMD_MASON_PROP3_BTN, OnElsplsMasonBtn3)
	ON_BN_CLICKED(IDC_CMD_MASON_PROP4_BTN, OnElsplsMasonBtn4)
	ON_BN_CLICKED(IDC_ELSPLS_HARD_CHK_TRESCA, OnElsplsHardChk)
	ON_BN_CLICKED(IDC_ELSPLS_ISO_RD_TRESCA, OnHardeningTypeTresca)
	ON_BN_CLICKED(IDC_ELSPLS_MIXED_RD_MOHR, OnHardeningTypeMohr)
	ON_BN_CLICKED(IDC_ELSPLS_KINE_RD_TRESCA, OnHardeningTypeTresca)
	ON_BN_CLICKED(IDC_ELSPLS_MIXED_RD_TRESCA, OnHardeningTypeTresca)
	ON_BN_CLICKED(IDC_ELSPLS_KINE_RD_MOHR, OnHardeningTypeMohr)
	ON_BN_CLICKED(IDC_ELSPLS_ISO_RD_MOHR, OnHardeningTypeMohr)
	ON_BN_CLICKED(IDC_ELSPLS_HARD_CHK_MOHR, OnElsplsHardChk)
	ON_BN_CLICKED(IDC_CMD_MASON_MATL_COORD_BTN, OnMatlCoord)
	ON_BN_CLICKED(IDC_CONCDMG_COMP_BEH_BTN, OnCompBehBtn)
	ON_BN_CLICKED(IDC_CONCDMG_TENSILE_BEH_BTN, OnTensileBehBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMElsPlsMatlAddDlg::InitCtrls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(IDC_ELSPLS_STATIC_PLS_TRESCA)->GetWindowRect(rRef);

	GetDlgItem(IDC_ELSPLS_STATIC_PLS_MOHR)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aMohrCl, nDistY);

	GetDlgItem(IDC_ELSPLS_STATIC_PLS_MICRO)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aMicroP, nDistY);

	GetDlgItem(IDC_CMD_MASON_BOX_GRP)->GetWindowRect(rToMove); // MNET:2603 20061212 mylee
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aMason, nDistY);

	GetDlgItem(IDC_CONCDMG_BOX_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aConcDmg, nDistY);
/*
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_ELSPLS_APPLY_BTN);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
*/
	// set unit
	m_edtYield.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dInitYieldStress);
	m_untYield.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dInitYieldStress);
	m_edtCohe.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dInitCohesion);
	m_untCohe.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dInitCohesion);
	m_edtCoefTr.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dHardeningCoef);
	m_untCoefTr.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dHardeningCoef);
	m_edtCeofMhr.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dHardeningCoef);
	m_untCoefMhr.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dHardeningCoef);

	// 
	m_cmbModel.ResetContent();
	int nIndex = 0;

	// make Model combo
	nIndex = m_cmbModel.AddString(_LS(IDS_CMD_ELSPLS_TRESCA ));  m_cmbModel.SetItemData(nIndex, D_PLS_TRESCA );
	nIndex = m_cmbModel.AddString(_LS(IDS_CMD_ELSPLS_VON    ));  m_cmbModel.SetItemData(nIndex, D_PLS_MISES  );
	nIndex = m_cmbModel.AddString(_LS(IDS_CMD_ELSPLS_MOHR   ));  m_cmbModel.SetItemData(nIndex, D_PLS_MOHR   );
	nIndex = m_cmbModel.AddString(_LS(IDS_CMD_ELSPLS_DRUCKER));  m_cmbModel.SetItemData(nIndex, D_PLS_DRUCKER);
	//nIndex = m_cmbModel.AddString(_LS(IDS_CMD_ELSPLS_MICRO));    m_cmbModel.SetItemData(nIndex, D_PLS_MICRO);
	nIndex = m_cmbModel.AddString(_LS(IDS_CMD_ELSPLS_MASONRY));  m_cmbModel.SetItemData(nIndex, D_PLS_MASON);
	nIndex = m_cmbModel.AddString(_LS(IDS_CMD_ELSPLS_CONCDMG));  m_cmbModel.SetItemData(nIndex, D_PLS_CONCDMG);

	// make masonry material coord. combobox
	m_cmbMasonryMatlCoord.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbMasonryMatlCoord, _LS(IDS_CMD_ELSPLS_GLOBAL_COORD), D_PLS_MASON_MATL_COORD_GLOBAL);          // Global-Y : Global-X // Global
	CDlgUtil::CobxAddItem(m_cmbMasonryMatlCoord, _LS(IDS_CMD_ELSPLS_ELEMENT_LOCAL_COORD), D_PLS_MASON_MATL_COORD_LOCAL);    // Local-y : Local-x   // Element Local
	CDlgUtil::CobxAddItem(m_cmbMasonryMatlCoord, _LS(IDS_CMD_ELSPLS_GLOBAL_ANGLE), D_PLS_MASON_MATL_COORD_ANGLE);           // Global-Z : Angle
	CDlgUtil::CobxSetCurSelItemData(m_cmbMasonryMatlCoord, D_PLS_MASON_MATL_COORD_GLOBAL);

	m_cmbMasonryMatlAngle.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbMasonryMatlAngle, _LSX(0), 0);      // Global-Y : Global-X // Global
	CDlgUtil::CobxAddItem(m_cmbMasonryMatlAngle, _LSX(45), 45);    // Local-y : Local-x   // Element Local
	CDlgUtil::CobxAddItem(m_cmbMasonryMatlAngle, _LSX(90), 90);    // Global-Z : Angle
	CDlgUtil::CobxSetCurSelItemData(m_cmbMasonryMatlAngle, 0);

}
void CCMElsPlsMatlAddDlg::AlignControl()
{
	CRect rRef, rToMove;
	int nSel = m_cmbModel.GetItemData(m_cmbModel.GetCurSel());
	switch(nSel)
	{
		case D_PLS_TRESCA:  // Tresca
		{
			GetDlgItem(IDC_ELSPLS_STATIC_PLS_TRESCA)->GetWindowRect(rRef);
			break;
		}
		case D_PLS_MISES:  // Von Mises
		{
			if (CTestEnvMgr::GetTestEnvST(_LSX(Mnl_Fiber)) == _LSX(yes))
			{
				GetDlgItem(IDC_ELSPLS_FIBR_CHK_VMISES)->GetWindowRect(rRef);
			}
			else
			{
				GetDlgItem(IDC_ELSPLS_STATIC_PLS_TRESCA)->GetWindowRect(rRef);
			}
			break;
		}
		case D_PLS_MOHR:
		case D_PLS_DRUCKER: // Mohr-Coulomb, Drucker-Prager
		{
			GetDlgItem(IDC_ELSPLS_STATIC_PLS_MOHR)->GetWindowRect(rRef);
			break;
		}
		case D_PLS_MICRO: // Microplane
		{
			GetDlgItem(IDC_ELSPLS_STATIC_PLS_MICRO)->GetWindowRect(rRef);
			break;
		}
		case D_PLS_MASON: // Masonry
		{
			GetDlgItem(IDC_CMD_MASON_BOX_GRP)->GetWindowRect(rRef);
			break;
		}
		case D_PLS_CONCDMG:
		{
			GetDlgItem(IDC_CONCDMG_BOX_GRP)->GetWindowRect(rRef);
			break;
		}
	}

	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	int nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aOKCancel, nDistY);

	// resize self size
	CRect rectLast;
	GetDlgItem(IDOK)->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMElsPlsMatlAddDlg::Data2Dlg()
{
	if(!m_bModify) m_Data.Initialize();

	m_strName = m_Data.strMatlName;
	int i = 0;
	for (i=0 ; i<m_cmbModel.GetCount() ; ++i)
		if (m_cmbModel.GetItemData(i) == m_Data.nModelType) break;
	m_cmbModel.SetCurSel(i);

	SetDataCtrl(m_Data.nModelType);

	// set masonry matl coord combo
	CDlgUtil::CobxSetCurSelItemData(m_cmbMasonryMatlCoord, m_Data.MASONRY.nMatlCoordKey);

	CString str;    str.Format(_T("%g"), m_Data.MASONRY.dAngle);
	m_cmbMasonryMatlAngle.SetWindowText(str);

	OnHardeningTypeTresca();
	OnHardeningTypeMohr();
}
void CCMElsPlsMatlAddDlg::SetDataCtrl(int nType)
{
	CString str; str.Empty();
	switch(nType)
	{
	case D_PLS_TRESCA:  // Tresca
		{
			m_chkHardTr = m_Data.TRESCA.bHardening;
			m_edtYield.SetEditUnit(m_Data.TRESCA.dInitYieldStress);
			m_nHardTr = m_Data.TRESCA.nHardeningType;
			m_edtCoefTr.SetEditUnit(m_Data.TRESCA.dHardeningCoef);
			str.Format(_T("%g"), m_Data.TRESCA.dBackStressCoef);
			m_edtStrCoefTr.SetWindowText(str);
			break;
		}
	case D_PLS_MISES:  // Von Mises
		{
			m_chkHardTr = m_Data.VMISES.bHardening;
			m_edtYield.SetEditUnit(m_Data.VMISES.dInitYieldStress);
			m_nHardTr = m_Data.VMISES.nHardeningType;
			m_edtCoefTr.SetEditUnit(m_Data.VMISES.dHardeningCoef);
			str.Format(_T("%g"), m_Data.VMISES.dBackStressCoef);
			m_chkFiberModel.SetCheck((int)m_Data.VMISES.bUseFiber);
			m_edtStrCoefTr.SetWindowText(str);
			break;
		}
	case D_PLS_MOHR:
		{
			m_chkHardMhr = m_Data.MOHRCL.bHardening;
			m_edtCohe.SetEditUnit(m_Data.MOHRCL.dInitCohesion);
			str.Format(_T("%g"), m_Data.MOHRCL.dInitFricAngle);
			m_edtFric.SetWindowText(str);
			m_nHardMhr = m_Data.MOHRCL.nHardeningType;
			str.Format(_T("%g"), m_Data.MOHRCL.dBackStressCoef);
			m_edtStrCoefMhr.SetWindowText(str);
			m_edtCeofMhr.SetEditUnit(m_Data.MOHRCL.dHardeningCoef);
			break;
		}
	case D_PLS_DRUCKER: // Mohr-Coulomb, Drucker-Prager
		{
			m_chkHardMhr = m_Data.DRUCKER.bHardening;
			m_edtCohe.SetEditUnit(m_Data.DRUCKER.dInitCohesion);
			str.Format(_T("%g"), m_Data.DRUCKER.dInitFricAngle);
			m_edtFric.SetWindowText(str);
			m_nHardMhr = m_Data.DRUCKER.nHardeningType;
			str.Format(_T("%g"), m_Data.DRUCKER.dBackStressCoef);
			m_edtStrCoefMhr.SetWindowText(str);
			m_edtCeofMhr.SetEditUnit(m_Data.DRUCKER.dHardeningCoef);
			break;
		}
	case D_PLS_MICRO: // Microplane
		{ 
			//Data.MICROPL.dMu;
			for(int i=0; i<4;  i++) m_dKValue[i] = m_Data.MICROPL.dParamK[i];
			//for(int i=0; i<17; i++) Data.MICROPL.dParamC[i] = ;
			break;
		}
	case D_PLS_MASON: // MNET:2603 20061212
		{ 
			// m_Data -> 대화창에 값 세팅
			break;
		}
	case D_PLS_CONCDMG:
		{
			m_edtDiliationAng.SetEditUnit(m_Data.CONCDMG.dDilationAng);
			m_edtEccentricity.SetEditUnit(m_Data.CONCDMG.dEccentricity);
			m_edtfbo_fco.SetEditUnit(m_Data.CONCDMG.dfboOffco);
			m_edtK.SetEditUnit(m_Data.CONCDMG.dK);
			m_edtViscosityParam.SetEditUnit(m_Data.CONCDMG.dViscosityParm);
			break;
		}
	}
	if(nType!=D_PLS_MASON)
	{
		T_EPMT_D EpmtD;
		m_pDoc->m_pAttrCtrl->GetDefaultEpmt(EpmtD);
		m_Data.MASONRY = EpmtD.MASONRY;
	}
	UpdateData(FALSE);
}
void CCMElsPlsMatlAddDlg::Dlg2Data(BOOL& bSuc)
{
	UpdateData(TRUE);

	CString str; str.Empty();

	m_Data.strMatlName = m_strName;
	int nType = m_cmbModel.GetItemData(m_cmbModel.GetCurSel());
	m_Data.nModelType = nType;

	switch(nType)
	{
	case D_PLS_TRESCA:  // Tresca
		{
			m_Data.TRESCA.bHardening = m_chkHardTr;
			m_Data.TRESCA.dInitYieldStress = m_edtYield.GetEditValue();
			m_Data.TRESCA.nHardeningType = m_nHardTr;
			m_Data.TRESCA.dHardeningCoef = m_edtCoefTr.GetEditValue();
			m_edtStrCoefTr.GetWindowText(str);
			m_Data.TRESCA.dBackStressCoef = _tstof(str);
			break;
		}
	case D_PLS_MISES:  // Von Mises
		{
			m_Data.VMISES.bHardening = m_chkHardTr;
			m_Data.VMISES.dInitYieldStress = m_edtYield.GetEditValue();
			m_Data.VMISES.nHardeningType = m_nHardTr;
			m_Data.VMISES.dHardeningCoef = m_edtCoefTr.GetEditValue();
			m_edtStrCoefTr.GetWindowText(str);
			m_Data.VMISES.dBackStressCoef = _tstof(str);
			m_Data.VMISES.bUseFiber = m_chkFiberModel.GetCheck();
			break;
		}
	case D_PLS_MOHR:
		{
			m_Data.MOHRCL.bHardening = m_chkHardMhr;
			m_Data.MOHRCL.dInitCohesion = m_edtCohe.GetEditValue();
			m_edtFric.GetWindowText(str);
			m_Data.MOHRCL.dInitFricAngle = _tstof(str);
			m_Data.MOHRCL.nHardeningType = m_nHardMhr;
			m_edtStrCoefMhr.GetWindowText(str);
			m_Data.MOHRCL.dBackStressCoef = _tstof(str);
			m_Data.MOHRCL.dHardeningCoef = m_edtCeofMhr.GetEditValue();
			break;
		}
	case D_PLS_DRUCKER: // Mohr-Coulomb, Drucker-Prager
		{
			m_Data.DRUCKER.bHardening = m_chkHardMhr;
			m_Data.DRUCKER.dInitCohesion = m_edtCohe.GetEditValue();
			m_edtFric.GetWindowText(str);
			m_Data.DRUCKER.dInitFricAngle = _tstof(str);
			m_Data.DRUCKER.nHardeningType = m_nHardMhr;
			m_edtStrCoefMhr.GetWindowText(str);
			m_Data.DRUCKER.dBackStressCoef = _tstof(str);
			m_Data.DRUCKER.dHardeningCoef = m_edtCeofMhr.GetEditValue();
			break;
		}
	case D_PLS_MICRO: // Microplane
		{ 
			//Data.MICROPL.dMu;
			for(int i=0; i<4;  i++) m_Data.MICROPL.dParamK[i] = m_dKValue[i];
			//for(int i=0; i<17; i++) Data.MICROPL.dParamC[i] = ;
			break;
		}
	case D_PLS_MASON: // MNET:2603 20061212
		{ 
			// 대화창 값 -> m_Data
			DWORD dwData = 0;
			CDlgUtil::CobxGetItemDataByCurSel(m_cmbMasonryMatlCoord, dwData);
			m_Data.MASONRY.nMatlCoordKey = (int) dwData;
			ASSERT(m_Data.MASONRY.nMatlCoordKey < 10000);   // local coord는 -1이 들어가야 함.

			m_cmbMasonryMatlAngle.GetWindowText(str);
			double dAngle = _tstof(str);
			double dAng_mod = fmod(dAngle, 360.0);    // 각도의 범위는 0부터 360도 가능. 0부터 -360도 가능.
			m_Data.MASONRY.dAngle = dAng_mod;
			break;
		}
	case D_PLS_CONCDMG:
		{
			m_Data.CONCDMG.dDilationAng = m_edtDiliationAng.GetEditValue();
			m_Data.CONCDMG.dEccentricity = m_edtEccentricity.GetEditValue();
			m_Data.CONCDMG.dfboOffco = m_edtfbo_fco.GetEditValue();
			m_Data.CONCDMG.dK = m_edtK.GetEditValue();
			m_Data.CONCDMG.dViscosityParm = m_edtViscosityParam.GetEditValue();
			break;
		}
	}
	T_EPMT_D data;
	m_pDoc->m_pAttrCtrl->GetEpmt(m_OldKey, data);   // T_NLLP_D의 key를 주고 T_NLLP_D를 받는다.
	if(m_bModify)
	{
		m_Data.nSeq = data.nSeq;   // old data의 seq를 현재 data의 seq로 넣어준다.
		bSuc = m_pDoc->m_pDataCtrl->ModifyEpmt(m_csOldName, m_Data);
	}
	else bSuc = m_pDoc->m_pDataCtrl->AddEpmt(m_Data);

	if(bSuc) 
	{
		m_OldKey = m_Key;
		m_csOldName = m_Data.strMatlName;
	}

}
/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMatlAddDlg message handlers
BOOL CCMElsPlsMatlAddDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	
	InitCtrls();
	Data2Dlg();
	OnSelchangeModelCombo();
	OnSelchangeMatlCoordCombo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMElsPlsMatlAddDlg::OnApplyBtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bSuccess = TRUE;
	Dlg2Data(bSuccess);

	if(!bSuccess) return;
} 

void CCMElsPlsMatlAddDlg::OnSelchangeModelCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nSel = m_cmbModel.GetItemData(m_cmbModel.GetCurSel());

	CDlgUtil::CtrlShowHide(this, m_aVmisesFibr, FALSE);
	switch(nSel)
	{
	case D_PLS_TRESCA:  // Tresca
		{
			CDlgUtil::CtrlShowHide(this, m_aTresca, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aMohrCl, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMicroP, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMason,  FALSE);
			CDlgUtil::CtrlShowHide(this, m_aConcDmg,FALSE);
			break;
		}
	case D_PLS_MISES:  // Von Mises
		{
			CDlgUtil::CtrlShowHide(this, m_aTresca, TRUE);
			if (CTestEnvMgr::GetTestEnvST(_LSX(Mnl_Fiber)) == _LSX(yes))
			{
				CDlgUtil::CtrlShowHide(this, m_aVmisesFibr, TRUE);
			}
			CDlgUtil::CtrlShowHide(this, m_aMohrCl, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMicroP, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMason,  FALSE);
			CDlgUtil::CtrlShowHide(this, m_aConcDmg,FALSE);
			break;
		}
	case D_PLS_MOHR:
	case D_PLS_DRUCKER: // Mohr-Coulomb, Drucker-Prager
		{
			CDlgUtil::CtrlShowHide(this, m_aTresca, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMohrCl, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aMicroP, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMason,  FALSE);
			CDlgUtil::CtrlShowHide(this, m_aConcDmg,FALSE);
			break;
		}
	case D_PLS_MICRO: // Microplane
		{ 
			CDlgUtil::CtrlShowHide(this, m_aTresca, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMohrCl, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMicroP, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aMason,  FALSE);
			CDlgUtil::CtrlShowHide(this, m_aConcDmg,FALSE);
			break;
		}
	case D_PLS_MASON: // Masonry
		{ 
			CDlgUtil::CtrlShowHide(this, m_aTresca, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMohrCl, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMicroP, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMason,  TRUE);
			CDlgUtil::CtrlShowHide(this, m_aConcDmg,FALSE);
			break;
		}
	case D_PLS_CONCDMG:
		{ 
			CDlgUtil::CtrlShowHide(this, m_aTresca, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMohrCl, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMicroP, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aMason,  FALSE);
			CDlgUtil::CtrlShowHide(this, m_aConcDmg,TRUE);
			break;
		}
	}
	AlignControl();

	SetDataCtrl(nSel);
	OnElsplsHardChk();
}

void CCMElsPlsMatlAddDlg::OnSelchangeMatlCoordCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nSel = m_cmbMasonryMatlCoord.GetItemData(m_cmbMasonryMatlCoord.GetCurSel());
	
	BOOL bEnableAngle = FALSE;
	if (nSel == -4)  bEnableAngle = TRUE;   // = -4 : Global-Z : Angle

	CDlgUtil::CtrlEnableDisable(this, m_aMasonAngle, bEnableAngle);
}

void CCMElsPlsMatlAddDlg::OnElsplsModBtn() 
{
	T_EPMT_D data; data.Initialize();
	if(m_bModify || m_bMicroMod) data = m_Data;

	CCMElsPlsParamDlg dlg;
	dlg.SetMicroParamData(data);
	if(IDOK == dlg.DoModal())
	{
		for(int i=0; i<17; i++) m_Data.MICROPL.dParamC[i] = dlg.m_dCValue[i];
		m_Data.MICROPL.dMu = dlg.m_dMu;
		m_bMicroMod = TRUE;
	}
}

// MNET:2603 20061212 START
void CCMElsPlsMatlAddDlg::OnElsplsMasonBtn1() 
{
	CCMElsPlsMason1Dlg dlg;
	dlg.SetData(&m_Data.MASONRY);
	dlg.DoModal();
}
void CCMElsPlsMatlAddDlg::OnElsplsMasonBtn2() 
{
	CCMElsPlsMason2Dlg dlg;
	dlg.SetData(&m_Data.MASONRY);
	dlg.DoModal();
}
void CCMElsPlsMatlAddDlg::OnElsplsMasonBtn3() 
{
	CCMElsPlsMason3Dlg dlg;
	dlg.SetData(&m_Data.MASONRY);
	dlg.DoModal();
}
void CCMElsPlsMatlAddDlg::OnElsplsMasonBtn4() 
{
	CCMElsPlsMason4Dlg dlg;
	dlg.SetData(&m_Data.MASONRY);
	dlg.DoModal();
}
// MNET:2603 20061212 END mylee

void CCMElsPlsMatlAddDlg::OnElsplsHardChk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nType = m_cmbModel.GetItemData(m_cmbModel.GetCurSel());
	switch(nType)
	{
	case D_PLS_TRESCA:
	case D_PLS_MISES:  // Tresca, Von Mises
		{
			CDlgUtil::CtrlEnableDisable(this, m_aHardTresca, m_chkHardTr);
			CDlgUtil::CtrlEnableDisable(this, m_aHardMohrCl, FALSE);
			if (m_chkHardTr) OnHardeningTypeTresca();
			break;
		}
	case D_PLS_MOHR:
	case D_PLS_DRUCKER: // Mohr-Coulomb, Drucker-Prager
		{
			CDlgUtil::CtrlEnableDisable(this, m_aHardTresca, FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_aHardMohrCl, m_chkHardMhr);

#if defined (_MEC)
			GetDlgItem(IDC_ELSPLS_KINE_RD_MOHR)->EnableWindow(FALSE);
			GetDlgItem(IDC_ELSPLS_MIXED_RD_MOHR)->EnableWindow(FALSE);
#endif

			if (m_chkHardMhr) OnHardeningTypeMohr();
			break;
		}
	}
}

void CCMElsPlsMatlAddDlg::OnOK() 
{
	// TODO: Add extra validation here
	BOOL bSuccess = TRUE;
	Dlg2Data(bSuccess);

	if(!bSuccess) return;

	CDialogMove::OnOK();
}

void CCMElsPlsMatlAddDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CCMElsPlsMatlAddDlg::OnHardeningTypeTresca() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_nHardTr == 0) // Isotropic
	{
		m_edtStrCoefTr.SetWindowText(_T("1.0"));
		m_edtStrCoefTr.SetReadOnly(TRUE);
	}
	else if(m_nHardTr == 1) // Kinematic
	{
		m_edtStrCoefTr.SetWindowText(_T("0.0"));
		m_edtStrCoefTr.SetReadOnly(TRUE);
	}
	else // Mixed
	{
		m_edtStrCoefTr.SetReadOnly(FALSE);
	}
}

void CCMElsPlsMatlAddDlg::OnHardeningTypeMohr() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_nHardMhr == 0) // Isotropic
	{
		m_edtStrCoefMhr.SetWindowText(_T("1.0"));
		m_edtStrCoefMhr.SetReadOnly(TRUE);
	}
	else if(m_nHardMhr == 1) // Kinematic
	{
		m_edtStrCoefMhr.SetWindowText(_T("0.0"));
		m_edtStrCoefMhr.SetReadOnly(TRUE);
	}
	else // Mixed
	{
		m_edtStrCoefMhr.SetReadOnly(FALSE);
	}
}

void CCMElsPlsMatlAddDlg::OnMatlCoord()
{
	CDBMatlCoordDlg dlg;
	dlg.DoModal();
}

void CCMElsPlsMatlAddDlg::OnCompBehBtn()
{
	CCMElsPlsConcDmgGraphDlg dlg(0, m_OldKey, m_Data.CONCDMG);
	if(dlg.DoModal() == IDOK)
	{
		m_Data.CONCDMG = dlg.m_Data;
	}
}

void CCMElsPlsMatlAddDlg::OnTensileBehBtn()
{  
	CCMElsPlsConcDmgGraphDlg dlg(1, m_OldKey, m_Data.CONCDMG);
	if(dlg.DoModal() == IDOK)
	{
		m_Data.CONCDMG = dlg.m_Data;
	}
}