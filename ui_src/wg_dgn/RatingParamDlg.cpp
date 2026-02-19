// RatingParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingParamDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingParamDlg dialog


CRatingParamDlg::CRatingParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CRatingParamDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	T_RACD_D RacdD; RacdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(RacdD);

	//{{AFX_DATA_INIT(CRatingParamDlg)
	m_dSystemFactor = 1.0;
	m_bFlexure = FALSE;
	m_iMn_Method = -1;
	m_bShear = FALSE;
	m_bStress = FALSE;
	m_bStress_Sl = FALSE;
	m_bStress_Legal = FALSE;
	m_bStress_Sl_Legal = FALSE;
	m_bStress_Permit = FALSE;
	m_bSteel = FALSE;
	m_iShear = -1;
	m_bStrain = TRUE;
	//}}AFX_DATA_INIT
}


CRatingParamDlg::~CRatingParamDlg()
{
	
}


void CRatingParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingParamDlg)
	DDX_Control(pDX, IDC_DGN_RATING_PARAM_STEEL_UNIT, m_TensSteelUnit);
	DDX_Control(pDX, IDC_DGN_RATING_PARAM_STRESS2_UNIT, m_TensStressUnit);
	DDX_Control(pDX, IDC_DGN_RATING_PARAM_STRESS1_UNIT, m_CompStressUnit);
	DDX_Control(pDX, IDC_DGN_RATING_PARAM_STRESS2_SL_UNIT, m_TensStressUnit_Sl);
	DDX_Control(pDX, IDC_DGN_RATING_PARAM_STRESS1_SL_UNIT, m_CompStressUnit_Sl);
	DDX_Control(pDX, IDC_DGN_RATING_PRARM_TENS_STEEL_EDIT, m_dAlwTendonStress);
	DDX_Control(pDX, IDC_DGN_RATING_PRARM_TENS_STRESS_EDIT, m_dAlwTensStress);
	DDX_Control(pDX, IDC_DGN_RATING_PRARM_COMP_STRESS_EDIT, m_dAlwCompStress);
	DDX_Control(pDX, IDC_DGN_RATING_PRARM_TENS_STRESS_SL_EDIT, m_dAlwTensStress_Sl);
	DDX_Control(pDX, IDC_DGN_RATING_PRARM_COMP_STRESS_SL_EDIT, m_dAlwCompStress_Sl);
	DDX_Text(pDX, IDC_DGN_RATING_PRARM_SYSTEM_FACTOR_EDIT, m_dSystemFactor);
	DDX_Check(pDX, IDC_DGN_RATING_PARAM_FLEXURE_CHECK, m_bFlexure);
	DDX_Radio(pDX, IDC_DGN_RATING_PRARM_NOMINAL1_RADIO, m_iMn_Method);
	DDX_Check(pDX, IDC_DGN_RATING_PARAM_SHEAR_CHECK, m_bShear);
	DDX_Check(pDX, IDC_DGN_RATING_PARAM_STRESS_CHECK, m_bStress);
	DDX_Check(pDX, IDC_DGN_RATING_PARAM_STRESS_SLAB_CHECK, m_bStress_Sl);

	//Legal / Permit Load Ãß°¡
	DDX_Control(pDX, IDC_DGN_RATING_PARAM_STRESS2_UNIT3, m_TensStressUnit_Legal);
	DDX_Control(pDX, IDC_DGN_RATING_PARAM_STRESS1_UNIT2, m_CompStressUnit_Legal);
	DDX_Control(pDX, IDC_DGN_RATING_PARAM_STRESS2_SL_UNIT2, m_TensStressUnit_Sl_Legal);
	DDX_Control(pDX, IDC_DGN_RATING_PARAM_STRESS1_SL_UNIT2, m_CompStressUnit_Sl_Legal);
	DDX_Control(pDX, IDC_DGN_RATING_PRARM_TENS_STRESS_EDIT2, m_dAlwTensStress_Legal);
	DDX_Control(pDX, IDC_DGN_RATING_PRARM_COMP_STRESS_EDIT2, m_dAlwCompStress_Legal);
	DDX_Control(pDX, IDC_DGN_RATING_PRARM_TENS_STRESS_SL_EDIT2, m_dAlwTensStress_Sl_Legal);
	DDX_Control(pDX, IDC_DGN_RATING_PRARM_COMP_STRESS_SL_EDIT2, m_dAlwCompStress_Sl_Legal);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_PRES_EDT , m_dAlwStressPermit_Prest);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_PRES_UNIT, m_AlwStressPermit_Prest_Unit);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_REIN_EDT , m_dAlwStressPermit_Reinf);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_REIN_UNIT, m_AlwStressPermit_Reinf_Unit);
	DDX_Check(pDX, IDC_DGN_RATING_PARAM_STRESS_CHECK2, m_bStress_Legal);
	DDX_Check(pDX, IDC_DGN_RATING_PARAM_STRESS_SLAB_CHECK2, m_bStress_Sl_Legal);
	DDX_Check(pDX, IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_CHK, m_bStress_Permit);
	DDX_Check(pDX, IDC_DGN_RATING_PARAM_PRES_STEEL_CHECK, m_bSteel);
	DDX_Radio(pDX, IDC_DGN_RATING_PRARM_SHEAR1_RADIO, m_iShear);
	DDX_Radio(pDX, IDC_DGN_MEASURE_STRAIN_RDO, m_bStrain);


	//DDX_Control(pDX, IDC_RDO_DEFLECTION, m_ratioDeflection);
	//DDX_Control(pDX, IDC_RDO_STRAIN, m_ratioStrain);

	//}}AFX_DATA_MAP
}

BOOL CRatingParamDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CCheckDialogOpen::SetSoftwareRendering();

	SetInitUnit();

	if(m_pDoc->m_pAttrCtrl->ExistBlrs())
	{
		m_pDoc->m_pAttrCtrl->GetBlrs(m_MainData);
	}
	else
	{
		m_MainData.Initialize();
		//m_MainData.bStrength[2] = FALSE;
		m_MainData.bService[2] = FALSE;
		m_MainData.bService[4] = FALSE;
		m_MainData.iDiagnostic_Method = 0;
	}

	InitCtrl();

	LoadDlgSysTemFactor();
	LoadDlgStrength();
	LoadDlgService();
	LoadDlgDiagnostic_Method();

	OnDgnRatingParamFlexureCheck();
	OnDgnRatingParamStressCheck();
	OnDgnRatingParamStressSlabCheck();
	OnDgnRatingParamStressCheck_Legal();
	OnDgnRatingParamStressSlabCheck_Legal();
	OnDgnRatingParamPresSteelCheck();	
	OnDgnRatingParamShearCheck();
	OnDgnRatingParamStressCheck_Permit();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRatingParamDlg::InitCtrl()
{
	//////////////////////////////////////////////////////////////////////////
	// Enable
	m_pDoc = CDBDoc::GetDocPoint();
	T_RACD_D RacdD; RacdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(RacdD);
	BOOL bComposite = m_pDoc->m_pPostCtrl->IsPSCCompositeSect();
	if (!bComposite)
	{
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS_SLAB_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS_SLAB_CHECK2)->EnableWindow(FALSE);
	}
	if(RacdD.nRatingCode !=AASHTO_LRFD12_PSC_RATING && RacdD.nRatingCode !=AASHTO_LRFD19_PSC_RATING )
	{
		m_MainData.bService[3] = FALSE;
		m_MainData.bService[2] = FALSE;
		m_MainData.bService[4] = FALSE;
		m_MainData.iDiagnostic_Method = 0;
		//Diagnostic Test Group
		GetDlgItem(IDC_DGN_MEASURE_GRP)->EnableWindow(FALSE);
		//Diagnostic Test String
		GetDlgItem(IDC_DGN_MEASURE_STC)->EnableWindow(FALSE);
		//Diagnostic Test Strain
		GetDlgItem(IDC_DGN_MEASURE_STRAIN_RDO)->EnableWindow(FALSE);
		//Diagnostic Test Displacement
		GetDlgItem(IDC_DGN_MEASURE_DISPL_RDO)->EnableWindow(FALSE);
		//For Composite Slab Check
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS_SLAB_CHECK)->EnableWindow(FALSE);
		//For LRFR12 Design Load / Legal Load / Permit Load
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS_CHECK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS_SLAB_CHECK2)->EnableWindow(FALSE);
	}

	CArray<UINT, UINT> aCtrlPrestressSteel;
	aCtrlPrestressSteel.RemoveAll();
	aCtrlPrestressSteel.Add(IDC_DGN_RATING_PARAM_PRES_STEEL_GROUP);
	aCtrlPrestressSteel.Add(IDC_DGN_RATING_PARAM_PRES_STEEL_CHECK);
	aCtrlPrestressSteel.Add(IDC_DGN_RATING_PRARM_STEEL_STATIC);
	aCtrlPrestressSteel.Add(IDC_DGN_RATING_PRARM_TENS_STEEL_EDIT);
	aCtrlPrestressSteel.Add(IDC_DGN_RATING_PARAM_STEEL_UNIT);

	//////////////////////////////////////////////////////////////////////////
	// Showhide
	CArray<UINT, UINT> aControls19;
	aControls19.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, aControls19,  IDC_DGN_DEFINE_RATING_EVALU_PERMIT_GRP,  TRUE);
	if(RacdD.nRatingCode != AASHTO_LRFD19_PSC_RATING)
	{
		CDlgUtil::CtrlShowHide(this, aControls19, FALSE);
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP2)->SetWindowText(_T("Legal Load/Permit Load"));

	}
	else
	{
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP2)->SetWindowText(_T("Legal Load"));

		// Prestressing => Hide
		CDlgUtil::CtrlShowHide(this, aCtrlPrestressSteel, FALSE);
	}
	//////////////////////////////////////////////////////////////////////////
	// Move Ctrl
	CRect rRef;
	CRect rToMove;
	int nDistY;
	CArray<UINT, UINT> aControls;

	// Design Load
// 	aControls.RemoveAll();
// 	CDlgUtil::GetCtrlIDByIncRect(this, aControls,  IDC_DGN_DEFINE_RATING_EVALU_GRP1,  TRUE);
// 	GetDlgItem(IDC_DGN_RATING_PRARM_SERVICE_AUTO_CHK )->GetWindowRect(rRef);
// 	GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP1   )->GetWindowRect(rToMove);
// 	nDistY = rRef.top - rToMove.top;
// 	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Legal Load
	aControls.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, aControls,  IDC_DGN_DEFINE_RATING_EVALU_GRP2,  TRUE);
	GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP1 )->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP2 )->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	if(RacdD.nRatingCode == AASHTO_LRFD19_PSC_RATING)
	{
		// Permit Load
		aControls.RemoveAll();
		CDlgUtil::GetCtrlIDByIncRect(this, aControls,  IDC_DGN_DEFINE_RATING_EVALU_PERMIT_GRP,  TRUE);
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP2       )->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_GRP )->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		// Prestressing => Hide
// 		aControls.RemoveAll();
// 		CDlgUtil::GetCtrlIDByIncRect(this, aControls,  IDC_DGN_RATING_PARAM_PRES_STEEL_GROUP,  TRUE);
// 		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_GRP)->GetWindowRect(rRef);
// 		GetDlgItem(IDC_DGN_RATING_PARAM_PRES_STEEL_GROUP )->GetWindowRect(rToMove);
// 		nDistY = rRef.bottom - rToMove.top + 8;
// 		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	}
	else
	{
		// Prestressing		
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP2)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_RATING_PARAM_PRES_STEEL_GROUP )->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aCtrlPrestressSteel, nDistY);
	}

	// SERVICE_GROUP resize
	CRect rectResize;
	GetDlgItem(IDC_DGN_RATING_PRARM_SERVICE_GROUP)->GetWindowRect(rectResize);
	if(RacdD.nRatingCode == AASHTO_LRFD19_PSC_RATING)
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_GRP)->GetWindowRect(rRef);
	else
		GetDlgItem(IDC_DGN_RATING_PARAM_PRES_STEEL_GROUP)->GetWindowRect(rRef);
	rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rectResize);
	GetDlgItem(IDC_DGN_RATING_PRARM_SERVICE_GROUP)->MoveWindow(rectResize);

	// Test result
	aControls.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, aControls,  IDC_DGN_MEASURE_GRP,  TRUE);
	GetDlgItem(IDC_DGN_RATING_PRARM_SERVICE_GROUP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_MEASURE_GRP                  )->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// OK, Cancel
	aControls.RemoveAll();
	aControls.Add(IDOK);
	aControls.Add(IDCANCEL);
	GetDlgItem(IDC_DGN_MEASURE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	ResizeDialog();

}

void CRatingParamDlg::ResizeDialog()
{
	UINT nLastCtrlID = IDCANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(10);

	MoveWindow(r);
}

void CRatingParamDlg::SetInitUnit()
{
	m_dAlwTendonStress.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dAlwTensStress.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dAlwCompStress.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dAlwTensStress_Sl.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dAlwCompStress_Sl.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_TensSteelUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_TensStressUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_CompStressUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_TensStressUnit_Sl.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_CompStressUnit_Sl.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_dAlwTensStress_Legal.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dAlwCompStress_Legal.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dAlwTensStress_Sl_Legal.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dAlwCompStress_Sl_Legal.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_TensStressUnit_Legal.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_CompStressUnit_Legal.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_TensStressUnit_Sl_Legal.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_CompStressUnit_Sl_Legal.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_AlwStressPermit_Prest_Unit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_AlwStressPermit_Reinf_Unit.SetUnitType(D_UNITSYS_BASE_STRESS);	
	m_dAlwStressPermit_Prest.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dAlwStressPermit_Reinf.SetUnitType(D_UNITSYS_BASE_STRESS);

}

void CRatingParamDlg::LoadDlgSysTemFactor()
{
	m_dSystemFactor = m_MainData.dSystemFactor;

	UpdateData(FALSE);
}

void CRatingParamDlg::SaveDlgSysTemFactor()
{
	UpdateData(TRUE);

	m_MainData.dSystemFactor = m_dSystemFactor;	
}

void CRatingParamDlg::LoadDlgStrength()
{
	m_bFlexure = m_MainData.bStrength[0];
	m_bShear = m_MainData.bStrength[1];
	
	m_iMn_Method = m_MainData.iMn_Method;
	m_iShear = m_MainData.iVn_Method;

	UpdateData(FALSE);
}

void CRatingParamDlg::SaveDlgStrength()
{
	UpdateData(TRUE);

	m_MainData.bStrength[0] = m_bFlexure;
	m_MainData.bStrength[1] = m_bShear;

	m_MainData.iMn_Method = m_iMn_Method;	
	m_MainData.iVn_Method = m_iShear;	
}

void CRatingParamDlg::LoadDlgDiagnostic_Method()
{
	m_bStrain = m_MainData.iDiagnostic_Method;

	UpdateData(FALSE);
}

void CRatingParamDlg::SaveDlgDiagnostic_Method()
{
	UpdateData(TRUE);

	m_MainData.iDiagnostic_Method = m_bStrain;
}

void CRatingParamDlg::LoadDlgService()
{
	m_bStress = m_MainData.bService[0];
	m_bSteel = m_MainData.bService[1];
	m_bStress_Sl = m_MainData.bService[2];

	m_bStress_Legal = m_MainData.bService[3];
	m_bStress_Sl_Legal = m_MainData.bService[4];
	m_bStress_Permit  = m_MainData.bService[5];
	m_dAlwCompStress.SetEditUnit(m_MainData.dAlwCompStress);
	m_dAlwTensStress.SetEditUnit(m_MainData.dAlwTensStress);
	m_dAlwCompStress_Sl.SetEditUnit(m_MainData.dAlwCompStress_Sl);
	m_dAlwTensStress_Sl.SetEditUnit(m_MainData.dAlwTensStress_Sl);

	m_dAlwCompStress_Legal.SetEditUnit(m_MainData.dAlwCompStress_Legal);
	m_dAlwTensStress_Legal.SetEditUnit(m_MainData.dAlwTensStress_Legal);
	m_dAlwCompStress_Sl_Legal.SetEditUnit(m_MainData.dAlwCompStress_Sl_Legal);
	m_dAlwTensStress_Sl_Legal.SetEditUnit(m_MainData.dAlwTensStress_Sl_Legal);
	m_dAlwTendonStress.SetEditUnit(m_MainData.dAlwTendonStress);

	m_dAlwStressPermit_Prest.SetEditUnit(m_MainData.dAlwStress_Pres);
	m_dAlwStressPermit_Reinf.SetEditUnit(m_MainData.dAlwStress_Rein);

	UpdateData(FALSE);
}

void CRatingParamDlg::SaveDlgService()
{
	UpdateData(TRUE);

	m_MainData.bService[0] = m_bStress;
	m_MainData.bService[1] = m_bSteel;
	m_MainData.bService[2] = m_bStress_Sl;
	m_MainData.bService[3] = m_bStress_Legal;
	m_MainData.bService[4] = m_bStress_Sl_Legal;
	m_MainData.bService[5] = m_bStress_Permit;

	m_MainData.dAlwCompStress = m_dAlwCompStress.GetEditValue();
	m_MainData.dAlwTensStress = m_dAlwTensStress.GetEditValue();
	m_MainData.dAlwCompStress_Sl = m_dAlwCompStress_Sl.GetEditValue();
	m_MainData.dAlwTensStress_Sl = m_dAlwTensStress_Sl.GetEditValue();
	m_MainData.dAlwCompStress_Legal = m_dAlwCompStress_Legal.GetEditValue();
	m_MainData.dAlwTensStress_Legal = m_dAlwTensStress_Legal.GetEditValue();
	m_MainData.dAlwCompStress_Sl_Legal = m_dAlwCompStress_Sl_Legal.GetEditValue();
	m_MainData.dAlwTensStress_Sl_Legal = m_dAlwTensStress_Sl_Legal.GetEditValue();
	m_MainData.dAlwTendonStress = m_dAlwTendonStress.GetEditValue();
	m_MainData.dAlwStress_Pres = m_dAlwStressPermit_Prest.GetEditValue();
	m_MainData.dAlwStress_Rein = m_dAlwStressPermit_Reinf.GetEditValue();
}


BEGIN_MESSAGE_MAP(CRatingParamDlg, CDialogMove)
	//{{AFX_MSG_MAP(CRatingParamDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_DGN_RATING_PARAM_FLEXURE_CHECK, OnDgnRatingParamFlexureCheck)
	ON_BN_CLICKED(IDC_DGN_RATING_PARAM_STRESS_CHECK, OnDgnRatingParamStressCheck)
	ON_BN_CLICKED(IDC_DGN_RATING_PARAM_STRESS_SLAB_CHECK, OnDgnRatingParamStressSlabCheck)

	ON_BN_CLICKED(IDC_DGN_RATING_PARAM_PRES_STEEL_CHECK, OnDgnRatingParamPresSteelCheck)	
	ON_BN_CLICKED(IDC_DGN_RATING_PARAM_SHEAR_CHECK, OnDgnRatingParamShearCheck)

	ON_BN_CLICKED(IDC_DGN_RATING_PARAM_STRESS_CHECK2, OnDgnRatingParamStressCheck_Legal)
	ON_BN_CLICKED(IDC_DGN_RATING_PARAM_STRESS_SLAB_CHECK2, OnDgnRatingParamStressSlabCheck_Legal)

	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_CHK, OnDgnRatingParamStressCheck_Permit)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRatingParamDlg message handlers

void CRatingParamDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	SaveDlgSysTemFactor();	
	SaveDlgStrength();
	SaveDlgService();
	SaveDlgDiagnostic_Method();

	if(!m_pDoc->m_pDataCtrl->AddBlrs(m_MainData))
		return;

	CDialogMove::OnOK();		
}

void CRatingParamDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CRatingParamDlg::OnDgnRatingParamFlexureCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bFlexure)//Flexure Select
	{		
		//Nominal flexural resistance
		GetDlgItem(IDC_DGN_RATING_PRARM_FLEXURE3_GROUP)->EnableWindow(true);
		//Code
		GetDlgItem(IDC_DGN_RATING_PRARM_NOMINAL1_RADIO)->EnableWindow(true);
		//Strain compatibility
		GetDlgItem(IDC_DGN_RATING_PRARM_NOMINAL2_RADIO)->EnableWindow(true);
	}
	else
	{
		//Nominal flexural resistance
		GetDlgItem(IDC_DGN_RATING_PRARM_FLEXURE3_GROUP)->EnableWindow(false);
		//Code
		GetDlgItem(IDC_DGN_RATING_PRARM_NOMINAL1_RADIO)->EnableWindow(false);
		//Strain compatibility
		GetDlgItem(IDC_DGN_RATING_PRARM_NOMINAL2_RADIO)->EnableWindow(false);
	}
	
}

void CRatingParamDlg::OnDgnRatingParamShearCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bShear)//Flexure Select
	{		
		GetDlgItem(IDC_DGN_RATING_PRARM_SHEAR1_RADIO)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_SHEAR2_RADIO)->EnableWindow(true);		
	}
	else
	{
		GetDlgItem(IDC_DGN_RATING_PRARM_SHEAR1_RADIO)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_SHEAR2_RADIO)->EnableWindow(false);		
	}
	
}

void CRatingParamDlg::OnDgnRatingParamStressCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bStress)//Stress Select
	{		
		//Cpmpressive stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS1_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_COMP_STRESS_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS1_UNIT)->EnableWindow(true);

		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS2_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STRESS_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS2_UNIT)->EnableWindow(true);		
	}
	else
	{
		//Cpmpressive stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS1_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_COMP_STRESS_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS1_UNIT)->EnableWindow(false);

		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS2_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STRESS_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS2_UNIT)->EnableWindow(false);		
	}
}

void CRatingParamDlg::OnDgnRatingParamStressCheck_Legal() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bStress_Legal)//Stress Select
	{		
		//Cpmpressive stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS1_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_COMP_STRESS_EDIT2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS1_UNIT2)->EnableWindow(true);

		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS2_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STRESS_EDIT2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS2_UNIT3)->EnableWindow(true);		
	}
	else
	{
		//Cpmpressive stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS1_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_COMP_STRESS_EDIT2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS1_UNIT2)->EnableWindow(false);

		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS2_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STRESS_EDIT2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS2_UNIT3)->EnableWindow(false);		
	}
}

void CRatingParamDlg::OnDgnRatingParamStressSlabCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bStress_Sl)//Stress Select
	{		
		//Cpmpressive stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS1_SL_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_COMP_STRESS_SL_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS1_SL_UNIT)->EnableWindow(true);

		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS2_SL_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STRESS_SL_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS2_SL_UNIT)->EnableWindow(true);		
	}
	else
	{
		//Cpmpressive stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS1_SL_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_COMP_STRESS_SL_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS1_SL_UNIT)->EnableWindow(false);
		
		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS2_SL_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STRESS_SL_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS2_SL_UNIT)->EnableWindow(false);		
	}
}

void CRatingParamDlg::OnDgnRatingParamStressSlabCheck_Legal() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bStress_Sl_Legal)//Stress Select
	{		
		//Cpmpressive stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS1_SL_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_COMP_STRESS_SL_EDIT2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS1_SL_UNIT2)->EnableWindow(true);

		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS2_SL_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STRESS_SL_EDIT2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS2_SL_UNIT2)->EnableWindow(true);		
	}
	else
	{
		//Cpmpressive stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS1_SL_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_COMP_STRESS_SL_EDIT2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS1_SL_UNIT2)->EnableWindow(false);

		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STRESS2_SL_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STRESS_SL_EDIT2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PARAM_STRESS2_SL_UNIT2)->EnableWindow(false);		
	}
}

void CRatingParamDlg::OnDgnRatingParamPresSteelCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bSteel)//Prestressing Steel Select
	{		
		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STEEL_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STEEL_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_RATING_PARAM_STEEL_UNIT)->EnableWindow(true);
	}
	else
	{
		//Tensile stress
		GetDlgItem(IDC_DGN_RATING_PRARM_STEEL_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PRARM_TENS_STEEL_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_RATING_PARAM_STEEL_UNIT)->EnableWindow(false);
	}	
}

void CRatingParamDlg::OnDgnRatingParamStressCheck_Permit()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_PRES_STC)->EnableWindow(m_bStress_Permit);
	GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_PRES_EDT)->EnableWindow(m_bStress_Permit);
	GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_PRES_UNIT)->EnableWindow(m_bStress_Permit);
	GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_REIN_STC)->EnableWindow(m_bStress_Permit);
	GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_REIN_EDT)->EnableWindow(m_bStress_Permit);
	GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_PERMIT_STRES_REIN_UNIT)->EnableWindow(m_bStress_Permit);
}

